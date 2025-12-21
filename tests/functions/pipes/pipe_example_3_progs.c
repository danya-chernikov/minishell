/* pipe_example_3_progs.c
 *
 * In this example the program implements this shell
 * command 'ls -l argv[1] | tr _ $ | grep argv[2]'
 * when run passing to it two arguments:
 * ./pipe_example_3_progs dir_to_ls string_to_search_for
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
	int	pid_ls, pid_tr, pid_grep;
	int	pipes[2][2];

	if (argc < 3)
	{
		fprintf(stderr, "Please specify the directory to search"
				" and the filename to search for\n");
		return -1;
	}
	fprintf(stdout, "Parent: Grepping %s for %s\n", argv[1], argv[2]);

	if ((pipe(pipes[0]) == -1) || (pipe(pipes[1]) == -1))
	{
		fprintf(stderr, "Parent: Failed to create pipe\n");
		return -1;
	}
	pid_grep = fork();
	if (pid_grep == -1)
	{
		fprintf(stderr, "Parent: Could not fork process to run grep\n");
		return -1;
	}
	else if (pid_grep == 0)
	{
		fprintf(stdout, "Child: grep child will now run\n");
		if (dup2(pipes[1][READ_END], STDIN_FILENO) == -1)
		{
			fprintf(stderr, "Child: grep dup2() failed\n");
			return -1;
		}
		close(pipes[0][READ_END]);
		close(pipes[0][WRITE_END]);
		close(pipes[1][READ_END]);
		close(pipes[1][WRITE_END]);
		char *new_argv[] = { "/usr/bin/grep", argv[2], 0 };
		char *envp[] = { "HOME=/home/dchernik",
						 "PATH=/bin:/usr/bin",
						 "USER=dchernik", 0 };
		execve(new_argv[0], &new_argv[0], envp);
		fprintf(stderr, "Child: Oops, grep failed\n");
		return -1;
	} // else if (pid_grep == 0)
	
	pid_tr = fork();
	if (pid_tr == -1)
	{
		fprintf(stderr, "Parent: Could not fork process to run tr\n");
		return -1;
	}
	else if (pid_tr == 0)
	{
		fprintf(stdout, "Child: tr child will now run\n");
		if ((dup2(pipes[1][WRITE_END], STDOUT_FILENO) == -1) ||
			(dup2(pipes[0][READ_END], STDIN_FILENO) == -1))
		{
			fprintf(stderr, "Child: tr dup2() failed\n");
			return -1;
		}
		close(pipes[0][READ_END]);
		close(pipes[0][WRITE_END]);
		close(pipes[1][READ_END]);
		close(pipes[1][WRITE_END]);
		char *new_argv[] = { "/usr/bin/tr", "_", "$", 0 };
		char *envp[] = { "HOME=/home/dchernik",
						 "PATH=/bin:/usr/bin",
						 "USER=dchernik", 0 };
		execve(new_argv[0], &new_argv[0], envp);
		fprintf(stderr, "Child: Oops, tr failed\n");
		return -1;
	} // else if (pid_tr == 0)

	pid_ls = fork();
	if (pid_ls == -1)
	{
		fprintf(stderr, "Parent: Could not fork process to run ls\n");
		return -1;
	}
	else if (pid_ls == 0)
	{
		fprintf(stdout, "Child: ls child will now run\n");
		if (dup2(pipes[0][WRITE_END], STDOUT_FILENO) == -1)
		{
			fprintf(stderr, "ls dup2() failed\n");
			return -1;
		}
		close(pipes[0][READ_END]);
		close(pipes[0][WRITE_END]);
		close(pipes[1][READ_END]);
		close(pipes[1][WRITE_END]);
		char *new_argv[] = { "/bin/ls", "-la", argv[1], 0 };
		char *envp[] = { "HOME=/home/dchernik",
						 "PATH=/bin:/usr/bin",
						 "USER=dchernik", 0 };
		execve(new_argv[0], &new_argv[0], envp);
		fprintf(stderr, "Child: Oops, ls failed\n");
		return -1;

	} // else if (pid_ls == 0)

	// Parent doesn't need the pipes
	close(pipes[0][READ_END]);
	close(pipes[0][WRITE_END]);
	close(pipes[1][READ_END]);
	close(pipes[1][WRITE_END]);

	fprintf(stdout, "Parent: Parent will now wait for children to finish execution\n");
	while (wait(NULL) > 0)
	{
		fprintf(stdout, "Parent: Chidren have finished the execution, parent is done\n");
	}
	return 0;
}
