CC=cc
LIBFT_NAME=ft
LIBFT_PATH=Libft/
LIBVECTOR_NAME=vector
LIBVECTOR_PATH=vector/
NAME=minishell
DEBUG_LVL=1

CFLAGS=-Wall -Werror -Wextra -O0 -g3

SRCS=main.c \
	 shell.c shell_launcher.c aux_shell.c \
	 cmdargs_parser.c \
	 init_paramvars.c init_localvars.c init_localvars2.c \
	 init_localvars3.c init_localvars4.c init_localvars5.c \
	 init_envars.c init_envars2.c init_envars3.c \
	 env.c env_setters.c env_getters.c env_printers.c env_service.c \
	 history.c shell_history.c \
	 config.c shell_configs.c \
	 prompt_parser.c prompt_parser2.c prompt_parser3.c \
	 prompt_parser4.c prompt_parser5.c \
	 par.c quote.c token.c operand.c \
	 engine.c redirect_parser.c redirect.c heredoc.c exec.c wildcards.c builtin.c \
	 signals.c signals2.c \
	 aux_common.c aux_common2.c aux_io.c error.c debug.c test.c

OBJS=main.o \
	 shell.o shell_launcher.o aux_shell.o \
	 cmdargs_parser.o \
	 init_paramvars.o init_localvars.o init_localvars2.o \
	 init_localvars3.o init_localvars4.o init_localvars5.o \
	 init_envars.o init_envars2.o init_envars3.o \
	 env.o env_setters.o env_getters.o env_printers.o env_service.o \
	 history.o shell_history.o \
	 config.o shell_configs.o \
	 prompt_parser.o prompt_parser2.o prompt_parser3.o \
	 prompt_parser4.o prompt_parser5.o \
	 par.o quote.o token.o operand.o \
	 engine.o redirect_parser.o redirect.o heredoc.o exec.o wildcards.o builtin.o \
	 signals.o signals2.o \
	 aux_common.o aux_common2.o aux_io.o error.o debug.o test.o

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

main.o : main.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c main.c

# ============================== Shell launcher  =============================
shell.o : shell.c shell.h cmdargs_parser.h prompt_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell.c

shell_launcher.o : shell_launcher.c shell.h engine.h \
	libft.h error.h vector.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_launcher.c

aux_shell.o : aux_shell.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_shell.c


# ====================== Command line arguments parser =======================
cmdargs_parser.o : cmdargs_parser.c cmdargs_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c cmdargs_parser.c


# ==================== Parameter-variables initialization ====================
init_paramvars.o : init_paramvars.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_paramvars.c


# ================= Special local variables initialization ===================
init_localvars.o : init_localvars.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars.c

init_localvars2.o : init_localvars2.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars2.c

init_localvars3.o : init_localvars3.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars3.c

init_localvars4.o : init_localvars4.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars4.c

init_localvars5.o : init_localvars5.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_localvars5.c


# =================== Environment variables initialization ===================
init_envars.o : init_envars.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars.c

init_envars2.o : init_envars2.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars2.c

init_envars3.o : init_envars3.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c init_envars3.c


# ============================== Environment =================================
env.o : env.c env.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env.c

env_setters.o : env_setters.c env.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_setters.c

env_getters.o : env_getters.c env.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_getters.c

env_printers.o : env_printers.c env.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_printers.c

env_service.o : env_service.c env.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c env_service.c


# ================================ History ===================================
history.o : history.c history.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c history.c

shell_history.o : shell_history.c shell.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_history.c


# ================================= Configs ==================================
config.o : config.c config.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c config.c

shell_configs.o : shell_configs.c shell.h engine.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c shell_configs.c


# ============================== Prompt parser ===============================
prompt_parser.o : prompt_parser.c prompt_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser.c

prompt_parser2.o : prompt_parser2.c prompt_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser2.c

prompt_parser3.o : prompt_parser3.c prompt_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser3.c

prompt_parser4.o : prompt_parser4.c prompt_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser4.c

prompt_parser5.o : prompt_parser5.c prompt_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c prompt_parser5.c

par.o : par.c par.h prompt_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c par.c

quote.o : quote.c prompt_parser.h quote.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c quote.c

token.o : token.c token.h prompt_parser.h \
	libft.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c token.c

operand.o : operand.c operand.h env.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c operand.c


# =========================== Engine and execution ===========================
engine.o : engine.c engine.h shell.h heredoc.h exec.h quote.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c engine.c

redirect_parser.o : redirect_parser.c operand.h redirect_parser.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect_parser.c

redirect.o : redirect.c redirect.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c redirect.c

heredoc.o : heredoc.c heredoc.h prompt_parser.h shell.h signals.h aux_io.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c heredoc.c

exec.o : exec.c exec.h shell.h operand.h token.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c exec.c

wildcards.o : wildcards.c wildcards.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c wildcards.c

builtin.o : builtin.c builtin.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c builtin.c


# ================================= Signals ==================================
signals.o : signals.c signals.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c signals.c

signals2.o : signals2.c signals.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c signals2.c


# ============================ Auxiliary services ============================
aux_common.o : aux_common.c aux_common.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_common.c

aux_common2.o : aux_common2.c aux_common.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_common2.c

aux_io.o : aux_io.c aux_io.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c aux_io.c

error.o : error.c error.h shell.h \
	libft.h vector.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c error.c

debug.o : debug.c debug.h \
	libft.h vector.h error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c debug.c

test.o : test.c test.h \
	libft.h vector.h error.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -DDEBUG=$(DEBUG_LVL) \
	-c test.c

.PHONY: all clean fclean re libft libvector
