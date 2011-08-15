CC	=	gcc

NAME	=	maze_gen

SRC	=	main.c		\
		maze_gen.c	\
		sort.c

OBJ	=	$(SRC:.c=.o)

CFLAGS	=	-W -Wall -ansi -pedantic -O3

$(NAME):	$(OBJ)
		$(CC) -o $(NAME) $(OBJ)
		cp $(NAME) ../bin

all:		$(NAME)

clean:
		rm -f *~ $(OBJ)

fclean:		clean
		rm -f *# $(NAME)

re:		fclean all