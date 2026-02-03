#include "exec.h"

# if 0
/* Now we have to launch all operand-programs.
 * On error returns 0. On success returns 1 */
int	exec_ops(t_parser_data *d, int *ret_code)
{
	t_operand	*op;					// Pointer to the current operand
	pid_t		progs[MAX_OPS_NUM];		// Program PIDs
	pid_t		subshs[MAX_SUBSHS_NUM];	// Our subshells' PIDs
	int			ti;						// Token index (must be int)
	size_t		sh_i;					// Subshell index
	size_t		pi;						// Program index
	t_ll		cpar_pars_ind;			// Index of closing parenthesis in `d->pars`
	t_ll		opar_token_ind;			// Index of corresponding opening parenthesis in `d->tokens`

	int			fd_log;
	char		buf[BUF_SIZE];			// Buffer to write into the log file
	ssize_t		nr;						// Number of bytes written to the log file

	*ret_code = 0; // TEMPORARY!

	// Create a log file to write the PIDs of all subshells we launch
	fd_log = open(SUBSHELLS_PIDS_LOG,
				  O_WRONLY | O_CREAT | O_APPEND,
				  S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH); // 644
	if (fd_log == -1)
	{
		print_sys_error("Can't create/open the log file");
		return (0);
	}
	// Write the parent shell's PID into the log file
	snprintf(buf, BUF_SIZE, "Hi! I'm a parent shell! My PID is: %jd\n",
		(intmax_t)getpid());
	nr = write(fd_log, buf, ft_strlen(buf));
	if (nr != (ssize_t)ft_strlen(buf))
	{
		print_sys_error("Couldn't write into the log file");
		return (0);
	}

	// Traversing from right to left the tokens array	
	pi = 0;
	sh_i = 0;
	ti = (int)(d->token_cnt) - 1;
	while (ti >= 0)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			progs[pi] = fork();
			if (progs[pi] == -1)
			{
				print_sys_error("Can't fork");
				return (0);
			}
			else if (progs[pi] == 0) // 0 always is returned in the child
			{
				++pi;

				// We're in the new process
				char    *op_argv[2] = { 0, 0 };
				char    *envp[] = { "HOME=/home/user",
									"PATH=/bin:/usr/bin",
									"USER=user", 0 };

				op = d->tokens[ti].op;
				op_argv[0] = &op->name[0];

				// Let's attach pipes to each process (operand)
				if (op->write_end != -1)
					dup2(d->pipes[op->write_end][WRITE_END], STDOUT_FILENO);

				if (op->read_end != -1)
					dup2(d->pipes[op->read_end][READ_END], STDIN_FILENO);

				// Let's close all inherited parent's pipes

				if (!close_pipes(d))
					return (0);

				// Replace the executable image of this process
				execve(op_argv[0], &op_argv[0], envp);

				//fprintf(stderr, "Opps, %s failed\n", op_argv[0]);
				return (0);

			} // else if (progs[op_i] == 0)
		}
		else if (d->tokens[ti].type == PIPE)
		{
			// Do nothing
		}
		else if (d->tokens[ti].type == CLOSE_PAR)
		{
			subshs[sh_i] = fork();
			if (subshs[sh_i] == -1)
			{
				print_sys_error("Can't fork");
				return (0);
			}
			if (subshs[sh_i] == 0)
			{
				snprintf(buf, BUF_SIZE, "Hi! I'm a subshell! My PID is: %jd\n",
					(intmax_t)getpid());
				nr = write(fd_log, buf, ft_strlen(buf));
				if (nr != (ssize_t)ft_strlen(buf))
				{
					print_sys_error("Couldn't write into the log file");
					return (0);
				}

				// In child
				++sh_i;
				--ti;
				continue ;
			}
			// In parent
			// Now we should skip all tokens from this ')' to the corresponding '('
			cpar_pars_ind = get_par_by_prompt_ind(d, d->tokens[ti].start_pi, CLOSING_PAR);
			opar_token_ind = get_token_by_prompt_ind(d, d->pars[cpar_pars_ind].first);
			ti = opar_token_ind - 1;
			// Let's launch a subshell
		}
		else if (d->tokens[ti].type == OPEN_PAR)
		{
			// Exit the current subshell
			while (wait(NULL) > 0) {} // I doubt we need it here...
			exit(EXIT_SUCCESS);
		}

		--ti;

	} // while (ti >= 0)

	// Wait for all children to finish
	//printf("Waiting for children...\n");
	while (wait(NULL) > 0)
	{
		/*fprintf(stdout, "Parent: Children have finished "
			"the execution, parent is done\n");*/
	}
	close(fd_log);
	return (1);
}
#endif

int	close_pipes(t_parser_data *d)
{
	size_t	i;

	i = 0;
	while (i < d->pipe_cnt)
	{
		if (close(d->pipes[i][READ_END]) == -1)
		{
			perror("close()");
			return 0;
		}
		if (close(d->pipes[i][WRITE_END]) == -1)
		{
			perror("close()");
			return 0;
		}
		++i;
	}
	return 1;
}

static int	ft_strcmp(char *str1, char *str2)
{
	while ((*str1 != '\0' || *str2 != '\0') && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}

static int	is_parent_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strcmp(cmd, CD_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, EXPORT_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, UNSET_CMD) == 0)
		return (1);
	if (ft_strcmp(cmd, EXIT_CMD) == 0)
		return (1);
	return (0);
}

static void	error_msg(void)
{
	const char	*error;

	error = strerror(errno);
	write(STDERR_FILENO, error, ft_strlen(error));
	write(STDERR_FILENO, "\n", ft_strlen("\n"));
}

static void	free_cmd_args(char **args)
{
	int	idx;

	if (args == NULL)
		return ;
	idx = 0;
	while (args[idx])
	{
		free(args[idx]);
		idx++;
	}
	free(args);
}

int	exec_ops(t_parser_data *d, t_shell *msh, int *ret_code)
{
	t_operand	*op;
	pid_t		progs[MAX_OPS_NUM];
	int		ti;
	size_t		pi;
	char		**cmd_args;
	char		**env_tab;
	char		*path;
	char		*error_path;

	*ret_code = 0;
	pi = 0;
	ti = (int)(d->token_cnt) - 1;
	while (ti >= 0)
	{
		if (d->tokens[ti].type == OPERAND)
		{
			op = d->tokens[ti].op;
			// Dividir cmds y args (ls -la -> {"ls", "-la", NULL})
			cmd_args = ft_split(op->name, ' ');
			if (cmd_args == NULL || cmd_args[0] == NULL)
			{
				free_cmd_args(cmd_args);
				ti--;
				continue ;
			}
			// Ejecución Built-ins de Padre (solo si no hay pipes)
			if (d->pipe_cnt == 0 && is_parent_builtin(cmd_args[0]) == 1)
			{
				*ret_code = exec_builtin(cmd_args, &msh->env);
				free_cmd_args(cmd_args);
				ti--;
				continue ;
			}
			// Proceso hijo
			progs[pi] = fork();
			if (progs[pi] == -1)
			{
				write(STDERR_FILENO, "fork error", ft_strlen("fork error"));
				free_cmd_args(cmd_args);
				return (0);
			}
			else if (progs[pi] == 0)
			{
				if (op->write_end != -1)
				{
					if (dup2(d->pipes[op->write_end][WRITE_END], STDOUT_FILENO) == -1)
					{
						error_msg();
						close_pipes(d);
						exit(1);
					}
				}
				if (op->read_end != -1)
				{
					if (dup2(d->pipes[op->read_end][READ_END], STDIN_FILENO) == -1)
					{
						error_msg();
						close_pipes(d);
						exit(1);
					}
				}
				// Cierre de pipes heredados posterior a dup2
				close_pipes(d);
				// Ejecución Built-ins de hijo
				if (is_builtin(cmd_args[0]) == 1)
					exit(exec_builtin(cmd_args, &msh->env));
				// Ejecución comando externo
				env_tab = env_to_tab(&msh->env);
				path = get_cmd_path(cmd_args[0], env_tab);
				if (path == NULL)
				{
					error_path = "minishell: command not found: ";
					write(STDERR_FILENO, error_path, ft_strlen(error_path));
					write(STDERR_FILENO, cmd_args[0], ft_strlen(cmd_args[0]));
					write(STDERR_FILENO, "\n", ft_strlen("\n"));
					exit(127);
				}
				execve(path, cmd_args, env_tab);
				write(STDERR_FILENO, "execve", ft_strlen("execve"));
				exit(126);
			}
			// En el padre: liberamos memoria y seguimos
			free_cmd_args(cmd_args);
			pi++;
		}
		else if (d->tokens[ti].type == PIPE)
		{
		}
		else if (d->tokens[ti].type == CLOSE_PAR)
		{
		}
		else if (d->tokens[ti].type == OPEN_PAR)
		{
			while (wait(NULL) > 0);
		}
		ti--;
	}
	close_pipes(d);
	// Esperar a todos los hijos lanzados
	while (wait(NULL) > 0);
	return (1);
}
