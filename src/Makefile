CC=cc
LIBFT_NAME=ft
LIBFT_PATH=Libft/
LIBVECTOR_NAME=vector
LIBVECTOR_PATH=vector/
NAME=minishell
DEBUG_LVL=0

CFLAGS=-Wall -Werror -Wextra -O0 -g3

SRCS=main.c \
	 shell.c \
	 shell_launcher.c shell_launch_int.c shell_launch_script.c \
	 aux_shell.c aux_shell2.c \
	 cmdargs_parser.c cmdargs_parser2.c cmdargs_parser3.c cmdargs_parser4.c \
	 init_paramvars.c init_localvars.c init_localvars2.c \
	 init_localvars3.c init_localvars4.c init_localvars5.c \
	 init_envars.c init_envars2.c init_envars3.c \
	 env.c env_setters.c env_getters.c env_printers.c env_service.c \
	 env_exec.c \
	 history.c shell_history.c \
	 config.c shell_configs.c shell_login_configs.c shell_nonlogin_configs.c \
	 prompt_parser.c prompt_parser2.c prompt_parser3.c \
	 prompt_parser4.c prompt_parser5.c \
	 prompt_parser_proc_spec_char.c prompt_parser_proc_spec_char2.c \
	 prompt_parser_proc_nonspec_char.c \
	 par.c quote.c quote2.c quote3.c token.c operand.c operand2.c \
	 engine.c \
	 redirect_parser.c redirect_parser2.c redirect_parser3.c redirect_parser4.c \
	 redirect.c \
	 heredoc.c heredoc2.c heredoc3.c heredoc4.c heredoc5.c \
	 exp_common.c exp_common2.c \
	 exp_preparation.c exp_preparation2.c \
	 exp_assignment.c exp_assignment2.c \
	 exp_argredir.c exp_argredir2.c exp_argredir3.c \
	 exp_service.c exp_service2.c exp_service3.c \
	 wildcards.c wildcards_filesclt.c wildcards_filesclt2.c \
	 wildcards_sieve.c wildcards_sieve2.c \
	 wildcards_common.c wildcards_common2.c \
	 exec_expassign.c exec_ops.c exec_ops2.c \
	 exec_pipeline.c exec_pipeline2.c exec_pipeline3.c \
	 exec_pipeline4.c exec_pipeline5.c \
	 exec_execve.c exec_execve2.c exec_execve3.c \
	 exec_execve4.c exec_execve5.c exec_execve6.c \
	 exec_execve7.c exec_execve8.c \
	 builtin.c builtin_echo.c builtin_cd.c builtin_exit.c \
	 builtin_export.c builtin_others.c \
	 signals.c signals2.c \
	 aux_common.c aux_common2.c aux_common3.c \
	 aux_io.c error.c

OBJS=main.o \
	 shell.o \
	 shell_launcher.o shell_launch_int.o shell_launch_script.o \
	 aux_shell.o aux_shell2.o \
	 cmdargs_parser.o cmdargs_parser2.o cmdargs_parser3.o cmdargs_parser4.o \
	 init_paramvars.o init_localvars.o init_localvars2.o \
	 init_localvars3.o init_localvars4.o init_localvars5.o \
	 init_envars.o init_envars2.o init_envars3.o \
	 env.o env_setters.o env_getters.o env_printers.o env_service.o \
	 env_exec.o \
	 history.o shell_history.o \
	 config.o shell_configs.o shell_login_configs.o shell_nonlogin_configs.o \
	 prompt_parser.o prompt_parser2.o prompt_parser3.o \
	 prompt_parser4.o prompt_parser5.o \
	 prompt_parser_proc_spec_char.o prompt_parser_proc_spec_char2.o \
	 prompt_parser_proc_nonspec_char.o \
	 par.o quote.o quote2.o quote3.o token.o operand.o operand2.o \
	 engine.o \
	 redirect_parser.o redirect_parser2.o redirect_parser3.o redirect_parser4.o \
	 redirect.o \
	 heredoc.o heredoc2.o heredoc3.o heredoc4.o heredoc5.o \
	 exp_common.o exp_common2.o \
	 exp_preparation.o exp_preparation2.o \
	 exp_assignment.o exp_assignment2.o \
	 exp_argredir.o exp_argredir2.o exp_argredir3.o \
	 exp_service.o exp_service2.o exp_service3.o \
	 wildcards.o wildcards_filesclt.o wildcards_filesclt2.o \
	 wildcards_sieve.o wildcards_sieve2.o \
	 wildcards_common.o wildcards_common2.o \
	 exec_expassign.o exec_ops.o exec_ops2.o \
	 exec_pipeline.o exec_pipeline2.o exec_pipeline3.o \
	 exec_pipeline4.o exec_pipeline5.o \
	 exec_execve.o exec_execve2.o exec_execve3.o \
	 exec_execve4.o exec_execve5.o exec_execve6.o \
	 exec_execve7.o exec_execve8.o \
	 builtin.o builtin_echo.o builtin_cd.o builtin_exit.o \
	 builtin_export.o builtin_others.o \
	 signals.o signals2.o \
	 aux_common.o aux_common2.o aux_common3.o \
	 aux_io.o error.o

all : libft libvector $(NAME)

libft :
	cd $(LIBFT_PATH) && $(MAKE) all

libvector :
	cd $(LIBVECTOR_PATH) && $(MAKE) all

$(NAME) : $(OBJS)
	$(CC) -L$(LIBFT_PATH) -L$(LIBVECTOR_PATH) \
	$(OBJS) \
	-l$(LIBFT_NAME) -l$(LIBVECTOR_NAME) -lreadline \
	-no-pie \
	-o $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

main.o : main.c shell.h engine.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c main.c

# ============================== Shell launcher  =============================
shell.o : shell.c shell.h cmdargs_parser.h prompt_parser.h expansion.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell.c

shell_launcher.o : shell_launcher.c shell.h engine.h signals.h \
	libft.h error.h vector.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_launcher.c

shell_launch_int.o : shell_launch_int.c shell.h engine.h signals.h aux_common.h \
	libft.h error.h vector.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_launch_int.c

shell_launch_script.o : shell_launch_script.o engine.h shell.h \
	libft.h error.h vector.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_launch_script.c

aux_shell.o : aux_shell.c shell.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_shell.c

aux_shell2.o : aux_shell2.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_shell2.c

# ====================== Command line arguments parser =======================
cmdargs_parser.o : cmdargs_parser.c cmdargs_parser.h shell.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c cmdargs_parser.c

cmdargs_parser2.o : cmdargs_parser2.c cmdargs_parser.h shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c cmdargs_parser2.c

cmdargs_parser3.o : cmdargs_parser3.c cmdargs_parser.h shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c cmdargs_parser3.c

cmdargs_parser4.o : cmdargs_parser4.c cmdargs_parser.h shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c cmdargs_parser4.c


# ==================== Parameter-variables initialization ====================
init_paramvars.o : init_paramvars.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_paramvars.c


# ================= Special local variables initialization ===================
init_localvars.o : init_localvars.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars.c

init_localvars2.o : init_localvars2.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars2.c

init_localvars3.o : init_localvars3.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars3.c

init_localvars4.o : init_localvars4.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars4.c

init_localvars5.o : init_localvars5.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars5.c


# =================== Environment variables initialization ===================
init_envars.o : init_envars.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars.c

init_envars2.o : init_envars2.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars2.c

init_envars3.o : init_envars3.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars3.c


# ============================== Environment =================================
env.o : env.c env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env.c

env_setters.o : env_setters.c env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_setters.c

env_getters.o : env_getters.c env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_getters.c

env_printers.o : env_printers.c env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_printers.c

env_service.o : env_service.c env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_service.c

env_exec.o : env_exec.c env.h shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_exec.c


# ================================ History ===================================
history.o : history.c history.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c history.c

shell_history.o : shell_history.c shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_history.c


# ================================= Configs ==================================
config.o : config.c config.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c config.c

shell_configs.o : shell_configs.c shell.h engine.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_configs.c

shell_login_configs.o : shell_login_configs.c shell.h engine.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_login_configs.c

shell_nonlogin_configs.o : shell_nonlogin_configs.c shell.h engine.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_nonlogin_configs.c


# ============================== Prompt parser ===============================
prompt_parser.o : prompt_parser.c prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser.c

prompt_parser2.o : prompt_parser2.c prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser2.c

prompt_parser3.o : prompt_parser3.c prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser3.c

prompt_parser4.o : prompt_parser4.c prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser4.c

prompt_parser5.o : prompt_parser5.c prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser5.c

prompt_parser_proc_spec_char.o : prompt_parser_proc_spec_char.c prompt_parser.h \
	aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser_proc_spec_char.c

prompt_parser_proc_spec_char2.o : prompt_parser_proc_spec_char2.c prompt_parser.h \
	aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser_proc_spec_char2.c

prompt_parser_proc_nonspec_char.o : prompt_parser_proc_nonspec_char.c prompt_parser.h \
	aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser_proc_nonspec_char.c

par.o : par.c par.h prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c par.c

quote.o : quote.c prompt_parser.h quote.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c quote.c

quote2.o : quote2.c prompt_parser.h quote.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c quote2.c

quote3.o : quote3.c quote.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c quote3.c

token.o : token.c token.h prompt_parser.h \
	libft.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c token.c

operand.o : operand.c operand.h env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c operand.c

operand2.o : operand2.c operand.h env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c operand2.c


# ====================== Engine and redirections parser ======================
engine.o : engine.c engine.h shell.h heredoc.h exec.h quote.h expansion.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c engine.c

redirect_parser.o : redirect_parser.c redirect_parser.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect_parser.c

redirect_parser2.o : redirect_parser2.c redirect_parser.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect_parser2.c

redirect_parser3.o : redirect_parser3.c redirect_parser.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect_parser3.c

redirect_parser4.o : redirect_parser4.c redirect_parser.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect_parser4.c

redirect.o : redirect.c redirect.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect.c

heredoc.o : heredoc.c heredoc.h prompt_parser.h shell.h signals.h aux_io.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c heredoc.c

heredoc2.o : heredoc2.c heredoc.h prompt_parser.h shell.h signals.h aux_io.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c heredoc2.c

heredoc3.o : heredoc3.c heredoc.h prompt_parser.h shell.h signals.h aux_io.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c heredoc3.c

heredoc4.o : heredoc4.c heredoc.h prompt_parser.h shell.h signals.h aux_io.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c heredoc4.c

heredoc5.o : heredoc5.c heredoc.h prompt_parser.h shell.h signals.h aux_io.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c heredoc5.c

heredoc2.o : heredoc2.c heredoc.h shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c heredoc2.c

# ======================= Variable expantion/assignment ======================

exp_assignment.o : exp_assignment.c expansion.h shell.h prompt_parser.h \
	operand.h token.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_assignment.c

exp_assignment2.o : exp_assignment2.c expansion.h shell.h prompt_parser.h \
	operand.h token.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_assignment2.c

exp_argredir.o : exp_argredir.c expansion.h shell.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_argredir.c

exp_argredir2.o : exp_argredir2.c expansion.h shell.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_argredir2.c

exp_argredir3.o : exp_argredir3.c expansion.h shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_argredir3.c

exp_preparation.o : exp_preparation.c expansion.h shell.h prompt_parser.h \
	operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_preparation.c

exp_preparation2.o : exp_preparation2.c expansion.h shell.h prompt_parser.h \
	operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_preparation2.c

exp_common.o : exp_common.c expansion.h shell.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_common.c

exp_common2.o : exp_common2.c expansion.h shell.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_common2.c

exp_service.o : exp_service.c expansion.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_service.c

exp_service2.o : exp_service2.c expansion.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_service2.c

exp_service3.o : exp_service3.c expansion.h operand.h prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exp_service3.c


# ================================ Wildcards =================================

wildcards.o : wildcards.c wildcards.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards.c

wildcards_filesclt.o : wildcards_filesclt.c wildcards.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards_filesclt.c

wildcards_filesclt2.o : wildcards_filesclt2.c wildcards.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards_filesclt2.c

wildcards_sieve.o : wildcards_sieve.c wildcards.h operand.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards_sieve.c

wildcards_sieve2.o : wildcards_sieve2.c wildcards.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards_sieve2.c

wildcards_common.o : wildcards_common.c wildcards.h operand.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards_common.c

wildcards_common2.o : wildcards_common2.c wildcards.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards_common2.c


# ================================ Execution =================================

exec_expassign.o : exec_expassign.c exec.h shell.h token.h operand.h \
	expansion.h aux_common.h prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_expassign.c

exec_ops.o : exec_ops.c exec.h shell.h redirect.h prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_ops.c

exec_ops2.o : exec_ops2.c exec.h prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_ops2.c

exec_pipeline.o : exec_pipeline.c exec.h shell.h builtin.h signals.h \
	expansion.h aux_common.h prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_pipeline.c

exec_pipeline2.o : exec_pipeline2.c exec.h shell.h operand.h builtin.h \
	prompt_parser.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_pipeline2.c

exec_pipeline3.o : exec_pipeline3.c exec.h operand.h aux_common.h signals.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_pipeline3.c

exec_pipeline4.o : exec_pipeline4.c exec.h operand.h aux_common.h signals.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_pipeline4.c

exec_pipeline5.o : exec_pipeline5.c exec.h shell.h builtin.h prompt_parser.h \
	signals.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_pipeline5.c

exec_execve.o : exec_execve.c exec.h env.h token.h operand.h builtin.h \
	expansion.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve.c

exec_execve2.o : exec_execve2.c exec.h token.h shell.h operand.h builtin.h \
	expansion.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve2.c

exec_execve3.o : exec_execve3.c exec.h shell.h builtin.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve3.c

exec_execve4.o : exec_execve4.c exec.h aux_io.h operand.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve4.c

exec_execve5.o : exec_execve5.c exec.h env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve5.c

exec_execve6.o : exec_execve6.c exec.h builtin.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve6.c

exec_execve7.o : exec_execve7.c exec.h shell.h env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve7.c

exec_execve8.o : exec_execve8.c exec.h shell.h token.h operand.h builtin.h \
	expansion.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec_execve8.c


# ================================ Built-ins =================================
builtin.o : builtin.c builtin.h aux_common.h operand.h shell.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin.c

builtin_echo.o : builtin_echo.c builtin.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin_echo.c

builtin_cd.o : builtin_cd.c builtin.h shell.h operand.h env.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin_cd.c

builtin_exit.o : builtin_exit.c builtin.h shell.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin_exit.c

builtin_export.o : builtin_export.c builtin.h shell.h operand.h aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin_export.c

builtin_others.o : builtin_others.c builtin.h shell.h operand.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin_others.c


# ================================= Signals ==================================
signals.o : signals.c signals.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c signals.c

signals2.o : signals2.c signals.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c signals2.c


# ============================ Auxiliary services ============================
aux_common.o : aux_common.c aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_common.c

aux_common2.o : aux_common2.c aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_common2.c

aux_common3.o : aux_common3.c aux_common.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_common3.c

aux_io.o : aux_io.c aux_io.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_io.c

error.o : error.c error.h shell.h \
	libft.h vector.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c error.c

.PHONY: all clean fclean re libft libvector
