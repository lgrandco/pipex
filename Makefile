NAME = libftprintf.a

SRC_PATH = srcs

SRC = ft_printf.c utils.c pad.c handler.c

SRC := $(addprefix $(SRC_PATH)/,$(SRC))

OBJ = $(SRC:.c=.o)

CC = clang

CFLAGS = -Wall -Werror -Wextra -g3 -I.

RM = rm -f

LIBFT_PATH = ./libft

LIBFT = $(LIBFT_PATH)/libft.a

all : $(NAME)

$(LIBFT) :
	make bonus -C $(LIBFT_PATH)

$(NAME) : $(LIBFT) ${OBJ}
	cp $(LIBFT) $(NAME)
	ar rcs ${NAME} ${OBJ}

bonus : $(NAME)

clean :
	make clean -C $(LIBFT_PATH)
	$(RM) $(NAME) $(OBJ)

fclean : clean
	make fclean -C $(LIBFT_PATH)
	$(RM) $(NAME)

c : ${NAME}
	${CC} -g3 -L. -lftprintf
	valgrind ./a.out
	rm ./a.out

re : fclean all

.PHONY : all clean fclean re
