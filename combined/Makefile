# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/07 22:00:28 by jharrach          #+#    #+#              #
#    Updated: 2023/02/23 15:09:26 by jharrach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

SRC_NAME	= main pipes execute pathname

SRC_DIR		= src/
OBJ_DIR		= obj/

OBJ			+= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_NAME)))

# CFLAGS		= -Wall -Wextra -Werror

LIBFT_NAME	= libft.a
LIBFT_DIR	= libft/
LIBFT		= $(addprefix $(LIBFT_DIR), $(LIBFT_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) | sed -e 's/^/libft-> /;'
	@$(CC) $(CFLAGS) -lreadline $^ $(LIBFT) -o $@
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
