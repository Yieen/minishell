# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/07 22:00:28 by jharrach          #+#    #+#              #
#    Updated: 2023/03/03 11:53:36 by inovomli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

SRC_NAME	= main pipes execute pathname parser_and_add wrk_w_envp builtins lexer lexer2 work_with_dollar

SRC_DIR		= src/
OBJ_DIR		= obj/

OBJ			+= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_NAME)))

# CFLAGS		= -Wall -Wextra -Werror
CFLAGS		= -I$(shell brew --prefix readline)/include

LIBFT_NAME	= libft.a
LIBFT_DIR	= libft/
LIBFT		= $(addprefix $(LIBFT_DIR), $(LIBFT_NAME))

all: $(NAME)
# "$(shell brew --prefix readline)/lib/"
$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) | sed -e 's/^/libft-> /;'
	@$(CC) $(CFLAGS) -lreadline -L $(shell brew --prefix readline)/lib/ $^ $(LIBFT) -o $@
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
