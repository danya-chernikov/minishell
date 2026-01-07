CC=cc
LIBFT_NAME=ft
LIBFT_PATH=Libft/
LIBVECTOR_NAME=vector
LIBVECTOR_PATH=vector/
NAME=minishell

CFLAGS=-Wall -Werror -Wextra -O0 -g3

SRCS=main.c parser.c engine.c shell.c init.c par.c quote.c \
	 token.c operand.c aux.c builtin.c error.c debug.c test.c \
	 vector1.c vector2.c vector3.c vector4.c

OBJS=main.o parser.o engine.o shell.o init.o par.o quote.o \
	 token.o operand.o aux.o builtin.o error.o debug.o test.o \
	 vector1.o vector2.o vector3.o vector4.o

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

main.o : main.c libft.h parser.h engine.h quote.h builtin.h debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c main.c

parser.o : parser.c parser.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c parser.c

engine.o : engine.c engine.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c engine.c

shell.o : shell.c shell.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c shell.c

init.o : init.c shell.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c init.c

par.o : par.c par.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c par.c

quote.o : quote.c parser.h quote.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c quote.c

token.o : token.c parser.h token.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c token.c

operand.o : operand.c operand.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c operand.c

aux.o : aux.c aux.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c aux.c

builtin.o : builtin.c builtin.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c builtin.c

error.o : error.c error.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c error.c

debug.o : debug.c debug.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c debug.c

test.o : test.c test.h
	$(CC) $(CFLAGS) -I$(LIBFT_PATH) -I$(LIBVECTOR_PATH) -c test.c

.PHONY: all clean fclean re libft libvector
