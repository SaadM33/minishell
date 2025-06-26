
CC = cc

FLAGS = -Wall -Werror -Wextra -g

NAME = minishell



SRC_PATH =	src

OBJ_PATH =	obj

LIB_PATH =	libft



LIB_NAME =	libft.a

SRC_NAME =	main.c \
			parsing/parser.c \
			parsing/lexer.c \
			parsing/syntax_checker.c \
			parsing/expand_and_join.c \
			parsing/ft_split_whitespace.c \
			utils/errors.c \
			utils/linked_list.c \
			utils/general.c \
			utils/envirenment.c
			
OBJ_NAME = $(SRC_NAME:.c=.o)



INC =	libft/includes/libft.h

SRC =	$(addprefix $(SRC_PATH)/, $(SRC_NAME))

OBJ =	$(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

LIB =	$(addprefix $(LIB_PATH)/, $(LIB_NAME))

INC_FLAGS =	-Iincludes -Ilibft/includes/



all: $(NAME)

val: fresh
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

run: fresh
	./$(NAME)

fresh: all clean
	clear

# ----------------------------------- #


$(NAME): $(OBJ)
	$(MAKE) -C $(LIB_PATH)
	$(CC) $(OBJ) $(LIB) -lreadline  -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INC)
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(dir $(OBJ))
	@$(CC) $(FLAGS) -o $@ -c $< $(INC_FLAGS)
	@echo $<
	
clean:
	$(MAKE) -C $(LIB_PATH) fclean
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re val run fresh 
