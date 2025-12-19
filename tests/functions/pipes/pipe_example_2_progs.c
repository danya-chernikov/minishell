/* pipe_example_2_progs.c
 *
 * Example from
 * https://www.idc-online.com/technical_references/pdfs/information_technology/
 * How_Linux_pipes_work_under_the_hood.pdf
 *
 * In this guy's example the program implements this shell
 * command 'ls -l argv[1] | grep argv[2]'
 * when run passing to it two arguments:
 * ./pipe_example_2_progs dir_to_ls string_to_search_for
 *
 * First the rightmost command (grep) is launched! We go from right to left
 * */
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ_END	0
#define WRITE_END	1

int	main(int argc, char *argv[])
{
	int	pid_ls, pid_grep;
	int	pipefd[2];

	if (argc < 3)
	{
		fprintf(stderr, "Please specify the directory to search"
				" and the filename to search for\n");
		return -1;
	}
	fprintf(stdout, "Parent: Grepping %s for %s\n", argv[1], argv[2]);

	// Create an unnamed pipe
	if (pipe(pipefd) == -1)	
	{
		fprintf(stderr, "Parent: Failed to create pipe\n");
		return -1;
	}

	// Fork a process to run grep	
	pid_grep = fork();
	if (pid_grep == -1)
	{
		fprintf(stderr, "Parent: Could not fork process to run grep\n");
		return -1;
	}
	else if (pid_grep == 0)
	{
		// Here we're already in another process (our copy)!
		
		fprintf(stdout, "Child: grep child will now run\n");

		// Set fd[0] (stdin) to the read end of the pipe
		if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
		{
			fprintf(stderr, "Child: grep dup2() failed\n");
			return -1;
		}

		// We have inherited all our parent's variables!
		// Close the pipe now that we've duplicated it
		close(pipefd[READ_END]);
		close(pipefd[WRITE_END]);

		// Setup the arguments/environment to call
		char *new_argv[] = { "/usr/bin/grep", argv[2], 0 };

		char *envp[] = { "HOME=/home/dchernik",
						 "PATH=/bin:/usr/bin",
						 "USER=dchernik", 0 };

		// Call execve(2) which will replace the executable image of this process
		execve(new_argv[0], &new_argv[0], envp);

		// Executing will never continue in this process unless execve() returns
		// because of an error
		fprintf(stderr, "Child: Oops, grep failed\n");
		return -1;
	} // else if (pid_grep == 0)
	
	// Fork a process to run ls	
	pid_ls = fork();

	if (pid_ls == -1)
	{
		fprintf(stderr, "Parent: Could not fork process to run ls\n");
		return -1;
	}
	else if (pid_ls == 0)
	{
		// Here we're already in another process (our copy)!
		
		fprintf(stdout, "Child: ls child will now run\n");
		
		// Set fd[1] (stdout) to the write end of the pipe
		if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
		{
			fprintf(stderr, "ls dup2() failed\n");
			return -1;
		}

		// We have inherited all our parent's variables!
		// Close the pipe now that we've duplicated it
		close(pipefd[READ_END]);
		close(pipefd[WRITE_END]);
		
		// Setup the arguments/environment to call
		char *new_argv[] = { "/bin/ls", "-la", argv[1], 0 };

		char *envp[] = { "HOME=/home/dchernik",
						 "PATH=/bin:/usr/bin",
						 "USER=dchernik", 0 };

		// Call execve(2) which will replace the executable image of this process
		execve(new_argv[0], &new_argv[0], envp);

		// Execution will never continue in this process unless execve returns
		// because of an error
		fprintf(stderr, "Child: Oops, ls failed\n");
		return -1;

	} // else if (pid_ls == 0)

	// Parent doesn't need the pipes
	close(pipefd[READ_END]);
	close(pipefd[WRITE_END]);

	fprintf(stdout, "Parent: Parent will now wait for children to finish execution\n");
	// Wait for all children to finish
	while (wait(NULL) > 0)
	{
		fprintf(stdout, "Parent: Chidren have finished the execution, parent is done\n");
	}

	return 0;
}
