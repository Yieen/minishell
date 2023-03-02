# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/07 22:00:28 by jharrach          #+#    #+#              #
#    Updated: 2023/02/27 20:07:07 by jharrach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

SRC_NAME	= main pipes execute pathname parser_and_add wrk_w_envp builtins

SRC_DIR		= src/
OBJ_DIR		= obj/

OBJ			+= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_NAME)))

# CFLAGS		= -Wall -Wextra -Werror
CFLAGS		= -I/Users/jharrach/homebrew/opt/readline/include

LIBFT_NAME	= libft.a
LIBFT_DIR	= libft/
LIBFT		= $(addprefix $(LIBFT_DIR), $(LIBFT_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) | sed -e 's/^/libft-> /;'
	@$(CC) $(CFLAGS) -lreadline -L /Users/jharrach/homebrew/opt/readline/lib/ $^ $(LIBFT) -o $@
	@echo "make: $@"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | objdir
	@$(CC) $(CFLAGS) -c $^ -o $@

objdir:
	@mkdir -p $(OBJ_DIR)

clean:
	@$(MAKE) clean -C $(LIBFT_DIR) | sed -e 's/^/libft-> /;'
	@$(RM) -r $(OBJ_DIR)
	@echo "make: $@"

fclean: clean
	@$(RM) $(NAME)
	@echo "make: $@"
	@$(RM) $(LIBFT)

re: fclean all

.PHONY: all clean fclean re objdir
