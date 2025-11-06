#include <stdio.h>
#include <readline/readline.h>

int main() {
    char *input = "";
    char *new_line = "This is the new line content";

    // get initial input
    input = readline("Enter some text: ");
    printf("Original input: %s\n", input);

    // replace the current line with new content
    rl_replace_line(new_line, 0);
    printf("New input: %s\n", rl_line_buffer);
	
    return 0;
}
