# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/07 22:00:28 by jharrach          #+#    #+#              #
#    Updated: 2023/03/09 16:08:26 by inovomli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# readline installation:
# 	cd ~ && mkdir ./homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C homebrew
# 	echo 'PATH="$HOME/homebrew/bin:$PATH"' >> ~/.bash_profile
# 	echo 'PATH="$HOME/homebrew/bin:$PATH"' >> ~/.zshrc
# 	brew install readline

NAME		= minishell

SRC_NAME	= main pipe1 pipe2 execute pathname parser_and_add wrk_w_envp builtins lexer1 \
lexer2 work_with_dollar quot_connect auxilar parser2 parser3 wwenv_qutcon
SRC_BUILTIN	= b_cd b_echo b_env b_exit b_export b_pwd b_unset

SRC_DIR		= src/
OBJ_DIR		= obj/
BUILTIN_DIR	= builtin/

OBJ			+= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_NAME)))
OBJ			+= $(addprefix $(OBJ_DIR), $(addprefix $(BUILTIN_DIR), $(addsuffix .o, $(SRC_BUILTIN))))

CFLAGS		= -Wall -Wextra -Werror -I$(shell brew --prefix readline)/include 
LDFLAGS		= -L$(shell brew --prefix readline)/lib/ -lreadlin 

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
	@mkdir -p $(OBJ_DIR)$(BUILTIN_DIR)

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
