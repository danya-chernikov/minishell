CC=cc
LIBFT_NAME=ft
LIBFT_PATH=Libft/
LIBVECTOR_NAME=vector
LIBVECTOR_PATH=vector/
NAME=minishell

CFLAGS=-Wall -Werror -Wextra -O0 -g3

SRCS=main.c shell.c cmdargs_parser.c init_paramvars.c init_localvars.c \
	 init_localvars2.c init_localvars3.c init_localvars4.c init_localvars5.c \
	 init_envars.c init_envars2.c init_envars3.c \
	 env.c engine.c prompt_parser.c exec.c history.c par.c \
	 quote.c token.c operand.c aux_prompt_parser.c builtin.c error.c \
	 aux_common.c debug.c test.c

OBJS=main.o shell.o cmdargs_parser.o init_paramvars.o init_localvars.o \
	 init_localvars2.o init_localvars3.o init_localvars4.o init_localvars5.o \
	 init_envars.o init_envars2.c init_envars3.o \
	 env.o engine.o prompt_parser.o exec.o history.o par.o \
	 quote.o token.o operand.o aux_prompt_parser.o builtin.o error.o \
	 aux_common.o debug.o test.o

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
	-DDEBUG \
	-o $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

main.o : main.c shell.h engine.h builtin.h libft.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c main.c

shell.o : shell.c shell.h cmdargs_parser.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c shell.c

cmdargs_parser.o : cmdargs_parser.c cmdargs_parser.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c cmdargs_parser.c

# Parameter-variables
init_paramvars.o : init_paramvars.c init_paramvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_paramvars.c

# Special variables initialization
init_localvars.o : init_localvars.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_localvars.c

init_localvars2.o : init_localvars2.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_localvars2.c

init_localvars3.o : init_localvars3.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_localvars3.c

init_localvars4.o : init_localvars4.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_localvars4.c

init_localvars5.o : init_localvars5.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_localvars5.c


# Environment variables initialization
init_envars.o : init_envars.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_envars.c

init_envars2.o : init_envars2.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_envars2.c

init_envars3.o : init_envars3.c init_localvars.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init_envars3.c

# Environment
env.o : env.c env.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c env.c

prompt_parser.o : prompt_parser.c prompt_parser.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c prompt_parser.c

engine.o : engine.c engine.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c engine.c

exec.o : exec.c exec.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c exec.c

history.o : history.c history.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c history.c

par.o : par.c par.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c par.c

quote.o : quote.c prompt_parser.h quote.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c quote.c

token.o : token.c prompt_parser.h token.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c token.c

operand.o : operand.c operand.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c operand.c

aux_prompt_parser.o : aux_prompt_parser.c aux_prompt_parser.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c aux_prompt_parser.c

builtin.o : builtin.c builtin.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c builtin.c

error.o : error.c error.h shell.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c error.c

debug.o : debug.c debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c debug.c

test.o : test.c test.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c test.c

.PHONY: all clean fclean re libft libvector
