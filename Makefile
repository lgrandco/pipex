NAME = pipex

SRC_PATH = srcs

SRC = main.c

SRC := $(addprefix $(SRC_PATH)/,$(SRC))

OBJ = $(SRC:.c=.o)

CC = clang

CFLAGS = -Wall -Werror -Wextra -g3 -I. -Ilibft

RM = rm -f

LIBFT_PATH = ./libft

LIBFT = $(LIBFT_PATH)/libft.a

all : $(NAME)

$(LIBFT) :
	make bonus -C $(LIBFT_PATH)

$(NAME) : $(LIBFT) ${OBJ} pipex.h
	$(CC) $(CFLAGS) -o $(NAME) -g3 -Ilibft $(SRC) $(LIBFT)

bonus : $(NAME)

clean :
	make clean -C $(LIBFT_PATH)
	$(RM) $(NAME) $(OBJ)

fclean : clean
	make fclean -C $(LIBFT_PATH)
	$(RM) $(NAME)

c :
	make -C $(LIBFT_PATH)
	${CC} -g3 -Ilibft $(SRC) $(LIBFT)

re : fclean all

.PHONY : all clean fclean re
