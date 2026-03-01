# Minishell

*This project has been created as part of the 42 curriculum by dchernik and jhvalenc.*

## Description

**Minishell** is a comprehensive and robust implementation of a UNIX shell, deeply inspired by `bash`. The core objective of this project is to understand the inner workings of an operating system's command-line interface, diving into process creation, file descriptor manipulation, signal handling, and advanced command-line parsing.

Unlike basic shell implementations, this project was engineered with a highly advanced architecture to ensure maximum reliability and expandability. It handles complex edge cases, brutal signal interruptions, and memory management without leaving a single byte behind (0 leaks strictly verified by Valgrind). 

Instead of relying on hierarchical parsing models, our engine is built around a custom, highly efficient **linear, array-based architecture**. This allows for a safe, predictable memory footprint and a sequential, state-machine-like evaluation of commands. The shell is divided into several main components:

1. **Tokenizer:** Reads the raw input, intelligently processes quotes (single and double), and divides the string into logical tokens. These tokens are stored sequentially in static arrays, utilizing rigorous bounds-checking to strictly prevent buffer overflows while maintaining a safe execution state.
2. **Linear Parser & Logic Engine:** Evaluates the token array sequentially. It handles pipes (`|`), redirections (`>`, `<`, `>>`, `<<`), and complex boolean logic (`&&`, `||`) directly on the linear token stream, evaluating execution conditions on the fly based on exit statuses (`$?`).
3. **Expander:** Properly expands environment variables (`$VAR`) within the parsed structures, strictly adhering to POSIX word-splitting rules.
4. **Executor:** The engine that manages the execution flow. It handles builtins directly, connects file descriptors for pipelines, and securely forks child processes to execute system binaries.

Whether launched in interactive mode with a custom prompt, or used to execute scripts non-interactively via standard input or the `-c` flag, this shell is built to endure.

## Features

Our Minishell is packed with advanced functionalities, going beyond the standard requirements to provide a fully reliable UNIX shell experience:

- **Linear Parsing Engine:** Static arrays and rigorous state-tracking handle tokenization and parsing, allowing for a robust, linear execution completely bypassing AST overhead, linked lists, and recursive parsing.
- **Advanced Control Operators & Subshells:** Native support for boolean logic (`&&` and `||`). Crucially, we implemented **parentheses `()` to support subshells**, allowing for complex command grouping and prioritized execution (e.g., `(ls || echo fail) | cat`), relying on our sequential evaluation strategy.
- **Wildcard Expansion:** Accurate implementation of `*` wildcard expansions for file matching in the current working directory.
- **Pipelines & Redirections:** Flawless handling of pipes (`|`) and redirections (`>`, `<`, `>>`, `<<` for heredocs), respecting execution order and seamlessly managing file descriptors.
- **Robust Signal Handling:** Perfect synchronization of signals (`SIGINT`, `SIGQUIT`, `EOF`) across the parent process, blocking child processes, and heredocs, strictly matching Bash's behavior.
- **Builtins:** Custom, fully compliant implementations of `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- **Memory Safety:** Rigorously tested against Valgrind. The shell survives hardcore stress tests (including forced external `kill` signals) with **0 definitely lost bytes**.
- **Custom Boolean Logic Tester:** To guarantee the flawless execution of our linear parser, we developed a dedicated test suite (`tests/boolean_logic_tester/`) that rigorously validates thousands of combinations of pipes, boolean operators, and subshells.

## Project Architecture & Core Components

To achieve a linear parse without an AST, the project relies on heavily compartmentalized logic. Key areas of the codebase include:

- **`src/vector/`**: A custom implementation of dynamic arrays (`vector_main.c`, `vector_stack.c`, etc.) developed as a utility library for safe and broader memory management tasks throughout the project.
- **`src/engine.c` & `src/exec_pipeline.c`**: The heart of the executor. These files iterate through the token arrays, handling the complex state-machine logic required to resolve pipes and boolean operators (`&&`, `||`) sequentially.
- **`src/cmdargs_parser.c` & `src/prompt_parser.c`**: Responsible for the lexical analysis, intelligently separating commands, arguments, and metacharacters while respecting single and double quotes and ensuring macro-defined array limits are not breached.
- **`tests/boolean_logic_tester/`**: Our custom-built, comprehensive test suite designed to push the boundaries of our boolean logic and subshell `()` implementations.

### Core Data Structures

To maintain a clean execution state and strictly avoid global variables (adhering to 42's strict coding norm), the entire runtime environment is securely encapsulated within highly specialized C structures:

- **`t_shell`**: The master context structure. It acts as the central hub of the program, holding pointers to the environment, execution history, configurations, and parser data. This centralization is what makes our garbage collection so effective upon an `exit` or fatal error.
- **`t_parser_data`**: Manages the state machine during the lexical analysis, tracking quote states (single vs. double) and sequentially storing token boundaries into fixed-size arrays with strict overflow protection.
- **`t_env`**: A custom, optimized representation of the system's environment variables, built for fast retrieval, precise `$VAR` expansion, and dynamic modification (`export`/`unset`).
- **`t_configs` & `t_shell_mode`**: Define the runtime environment context. These structs allow the engine to seamlessly identify and adapt to interactive sessions, pipe-fed inputs, or script execution.
- **`t_history`**: Safely encapsulates the command history logic, providing a persistent and leak-free record of user inputs across the interactive session.

### The Parsing Strategy: A Linear & Logical Approach

Most traditional shell implementations rely on an Abstract Syntax Tree (AST) to establish execution hierarchy. We took a different, highly optimized route: **a linear, state-machine-like parsing strategy**. Here is how our shell processes input from raw text to execution:

1. **Lexical Scanning & Array Structuring:** The user's input string is read from left to right. The tokenizer identifies boundaries (spaces, quotes, and metacharacters like `|`, `>`, `&&`) and isolates them into distinct logical units (tokens). Instead of creating hierarchical nodes, these tokens are pushed sequentially into our static token array structure.
2. **Linear Iteration:** The executor (`engine.c`) reads this array linearly (from index 0 to N). It maintains an internal "state" that tracks the current execution context, such as the exit status (`$?`) of the last executed command.
3. **On-the-fly Logical Evaluation:** When the engine encounters a boolean operator (`&&` or `||`), it does not need to traverse a tree branch. It simply evaluates the current state:
   - If it reads `&&` and the previous command failed (status `!= 0`), the engine fast-forwards the array index, skipping the next command until it finds a valid entry point (like a `||` or the end of the statement).
   - If it encounters a pipe `|`, it immediately sets up the file descriptors and forks the processes without losing its place in the sequence.
4. **Subshell Resolution:** Parentheses `()` are handled seamlessly. When the engine detects an open parenthesis, it isolates the tokens inside the block and processes them as an independent sub-array (or subshell environment), returning the final exit status back to the main linear flow once completed.

**Why this approach?** By avoiding recursive tree traversal, our linear approach drastically reduces complexity, prevents stack-overflow issues on deeply nested commands, and makes memory management incredibly straightforward (which is why our Valgrind tests return 0 definitely lost bytes).

## Execution Modes

Minishell adapts its environment and reading behavior depending on how it is launched, supporting both interactive (login-like) and non-interactive (scripts) sessions smoothly:

1. **Interactive Mode:** Launched simply via `./minishell`. It provides a custom prompt, utilizes the `readline` library for input and command history, and gracefully handles keyboard signals (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`).
2. **Non-Interactive Mode (Pipes & Scripts):** If input is piped to the shell (e.g., `echo "ls" | ./minishell`) or fed via a file redirection (`./minishell < script.sh`), it intelligently disables the prompt, avoids `readline` overhead, and executes the command stream linearly.

## Installation & Usage

### Requirements
Ensure you have `gcc`, `make`, and the `readline` library installed on your system.
```Bash
git clone https://github.com/danya-chernikov/minishell.git minishell
cd minishell/src
make
./minishell
```
### Running Minishell
1. **Interactive Mode:**
```Bash
ls -la | grep ".c" | wc -l
export GREETING="Hello 42" && echo $GREETING
```
2. **Non-Interactive Mode (Through pipes):**
```Bash
echo "ls -l | wc -l" | ./minishell
```
3. **Running a Script File:**
You can write commands in a file and feed them to Minishell using standard input redirection:
```Bash
echo "ls" > script.sh
echo "ping -c 1 8.8.8.8" >> script.sh
./minishell < script.sh
```
## Resources

Building a functional UNIX shell from scratch requires a deep understanding of system calls, process management, memory allocation, and POSIX standards. The following literature and documentation were instrumental in the architectural design and development of this project:

- **Advanced Programming in the UNIX Environment** by W. Richard Stevens & Stephen A. Rago
- **Linux System Programming: Talking Directly to the Kernel and C Library** by Robert Love
- **The Linux Command Line: A Complete Introduction** by William Shotts
- **How Linux Works: What Every Super-User Should Know** by Brian Ward
- **Bash Pocket Reference** by Arnold Robbins
- **Efficient Linux at the Command Line** by Daniel J. Barrett
- *GNU Bash Reference Manual* (Official Documentation)

* **Project page in Figma** - [Figma](https://www.figma.com/design/KPBjqRNlFC41i06Imw68zt/minishell?node-id=0-1&p=f&t=dNtM5ddpFGK17vNe-0) / Figma

* **Stack overflow question** - [StackOverflow](https://stackoverflow.com/questions/79851187/parsing-pipes-with-boolean-logic-operators-in-bash) / StackOverflow

### AI Usage Declaration

THIS PROJECT WAS DONE MANUALLY.
So-called 'AI' helped us a bit at the final normalization stage (normalizing 17000 lines
of code is not easy, though we were trying to normalize things on development stage, there
were some real challanges during normalization process). We also used it to resolve some
theoretical questions (as well as asking questions on forums) and to correct grammar errors
when writing commit messages. I strongly believe that extensive use of AI when studying
something kills the educational process. The brain MUST SUFFER to learn anything.

## Authors

This project was collaboratively developed by:

* **dchernik** - [GitHub](https://github.com/danya-chernikov) / danya-chernikov
* **jhvalenc** - [GitHub](https://github.com/Jhonjairo03s) / Jhonjairo03s

---
_“Bourne again? No, just born at 42.”_
