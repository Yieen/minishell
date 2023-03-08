# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/07 22:00:28 by jharrach          #+#    #+#              #
#    Updated: 2023/03/08 20:18:19 by inovomli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# readline installation:
# 	cd ~ && mkdir ./homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C homebrew
# 	echo 'PATH="$HOME/homebrew/bin:$PATH"' >> ~/.bash_profile
# 	echo 'PATH="$HOME/homebrew/bin:$PATH"' >> ~/.zshrc
# 	brew install readline

NAME		= minishell

SRC_NAME	= main pipes execute pathname parser_and_add wrk_w_envp builtins lexer lexer2 work_with_dollar quot_connect auxilar

SRC_DIR		= src/
OBJ_DIR		= obj/

OBJ			+= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_NAME)))

CFLAGS		= -Wall -Wextra -Werror -I$(shell brew --prefix readline)/include  -fsanitize=address -fsanitize=undefined #-Wno-gnu-include-next -I LeakSanitizer/include
LDFLAGS		= -L$(shell brew --prefix readline)/lib/ -lreadline #-LLeakSanitizer  -llsan -lc++  

LIBFT_NAME	= libft.a
LIBFT_DIR	= libft/
LIBFT		= $(addprefix $(LIBFT_DIR), $(LIBFT_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) | sed -e 's/^/libft-> /;'
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LIBFT) -o $@
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
