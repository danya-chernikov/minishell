CC=cc
LIBFT_NAME=ft
LIBFT_PATH=Libft/
LIBVECTOR_NAME=vector
LIBVECTOR_PATH=vector/
NAME=minishell
DEBUG_LVL=1

CFLAGS=-Wall -Werror -Wextra -O0 -g3

SRCS=main.c \
	 shell.c shell_launcher.c aux_shell.c shell_configs.c shell_history.c \
	 cmdargs_parser.c \
	 init_paramvars.c init_localvars.c init_localvars2.c \
	 init_localvars3.c init_localvars4.c init_localvars5.c \
	 init_envars.c init_envars2.c init_envars3.c \
	 env.c history.c config.c \
	 prompt_parser.c aux_prompt_parser.c par.c quote.c token.c operand.c \
	 engine.c exec.c builtin.c \
	 error.c aux_common.c debug.c test.c \
	 ft_cd.c ft_echo.c ft_env.c ft_exit.c ft_pwd.c ft_export.c ft_unset.c \
	 path_finder.c redirect.c signals.c \

OBJS=main.o \
	 shell.o shell_launcher.o aux_shell.o shell_configs.o shell_history.o \
	 cmdargs_parser.o \
	 init_paramvars.o init_localvars.o init_localvars2.o \
	 init_localvars3.o init_localvars4.o init_localvars5.o \
	 init_envars.o init_envars2.o init_envars3.o \
	 env.o history.o config.o \
	 prompt_parser.o aux_prompt_parser.o par.o quote.o token.o operand.o \
	 engine.o exec.o builtin.o \
	 error.o aux_common.o debug.o test.o \
	 ft_cd.o ft_echo.o ft_env.o ft_exit.o ft_pwd.o ft_export.o ft_unset.o \
	 path_finder.o redirect.o signals.o \

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

main.o : main.c shell.h libft.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c main.c

# ========== Shell initialization and command line arguments parser ==========
shell.o : shell.c shell.h cmdargs_parser.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell.c

shell_launcher.o : shell_launcher.c shell.h engine.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_launcher.c

aux_shell.o : aux_shell.c shell.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_shell.c

shell_configs.o : shell_configs.c shell.h engine.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_configs.c

shell_history.o : shell_history.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_history.c

cmdargs_parser.o : cmdargs_parser.c cmdargs_parser.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c cmdargs_parser.c


# ==================== Parameter-variables initialization ====================
init_paramvars.o : init_paramvars.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_paramvars.c


# ===================== Special variables initialization =====================
init_localvars.o : init_localvars.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars.c

init_localvars2.o : init_localvars2.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars2.c

init_localvars3.o : init_localvars3.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars3.c

init_localvars4.o : init_localvars4.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars4.c

init_localvars5.o : init_localvars5.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars5.c


# =================== Environment variables initialization ===================
init_envars.o : init_envars.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars.c

init_envars2.o : init_envars2.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars2.c

init_envars3.o : init_envars3.c shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars3.c

# ====================== Environment, history, configs =======================
env.o : env.c env.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env.c

history.o : history.c history.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c history.c

config.o : config.c config.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c config.c


# ============================== Prompt parser ===============================
prompt_parser.o : prompt_parser.c prompt_parser.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser.c

aux_prompt_parser.o : aux_prompt_parser.c aux_prompt_parser.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_prompt_parser.c

par.o : par.c par.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c par.c

quote.o : quote.c prompt_parser.h quote.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c quote.c

token.o : token.c prompt_parser.h token.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c token.c

operand.o : operand.c operand.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c operand.c


# =========================== Engine and execution ===========================
engine.o : engine.c engine.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c engine.c

exec.o : exec.c exec.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec.c

builtin.o : builtin.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin.c

ft_cd.o : ft_cd.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c ft_cd.c

ft_echo.o : ft_echo.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c ft_echo.c

ft_env.o : ft_env.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c ft_env.c

ft_exit.o : ft_exit.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c ft_exit.c

ft_pwd.o : ft_pwd.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c ft_pwd.c

ft_export.o : ft_export.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c ft_export.c

ft_unset.o : ft_unset.c builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c ft_unset.c

path_finder.o : path_finder.c path_finder.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c path_finder.c

redirect.o : redirect.c redirect.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect.c

#heredoc.o : heredoc.c heredoc.h debug.h
	#$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	#-c heredoc.c

signals.o : signals.c signals.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c signals.c

# ============================ Auxiliary services ============================
error.o : error.c error.h shell.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c error.c

aux_common.o : aux_common.c aux_common.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_common.c

debug.o : debug.c debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c debug.c

test.o : test.c test.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c test.c

.PHONY: all clean fclean re libft libvector
