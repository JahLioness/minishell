CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

NAME = minishell

SRC = main.c \
		all_utils.c \
		all_utils2.c \
		all_utils3.c \
		prompt.c \
		prompt_utils.c \
		signaux.c \
		signaux_heredoc.c \
		env/env_utils.c \
		env/env_utils2.c \
		env/env_init.c \
		mini/mini_init.c \
		mini/mini_utils.c \
		mini/mini_utils2.c \
		mini/mini_utils3.c \
		token/token_init.c \
		token/token_init_utils.c \
		token/token_init_utils2.c \
		token/token_utils.c \
		token/token_utils2.c \
		token/args_init.c \
		token/args_init2.c \
		token/args_wildcard.c \
		token/args_wildcard_utils.c \
		token/args_utils.c \
		token/args_utils2.c \
		token/redir_utils.c \
		token/redir_utils2.c \
		token/redir_utils3.c \
		builtin/cd.c \
		builtin/cd_utils.c \
		builtin/cd_utils2.c \
		builtin/echo.c \
		builtin/echo_utils.c \
		builtin/export.c \
		builtin/export_utils.c \
		builtin/pwd.c \
		builtin/unset.c \
		builtin/exit.c \
		builtin/exit_utils.c \
		exec/check_tokens.c \
		exec/exec_builtin.c \
		exec/exec_cmd_utils.c \
		exec/exec_cmd_utils2.c \
		exec/exec_cmd.c \
		exec/exec_cmd2.c \
		exec/exec_command.c \
		exec/exec_echo_utils.c \
		exec/exec_echo_utils2.c \
		exec/exec_unset_utils.c \
		exec/exec_export_utils.c \
		exec/exec_export_utils2.c \
		exec/exec_underscore.c \
		exec/exec_error.c \
		exec/exec_utils.c \
		exec/exec_redir.c \
		exec/exec_redir2.c \
		exec/exec_redir_utils.c \
		exec/exec_redir_utils2.c \
		exec/exec_token.c \
		exec/exec_token2.c \
		exec/process_child.c \
		exec/heredoc.c \
		exec/heredoc_prompt.c \
		exec/heredoc_expand.c \
		exec/heredoc_expand_utils.c \
		exec/heredoc_file.c \
		ast/ast_init.c \
		ast/ast_utils.c \



HEADER = minishell.h

LIBFT = libft/libft.a

OBJ = $(SRC:.c=.o)

GREEN = \033[0;32m
BLUE = \033[0;34m
CYAN = \033[0;36m
YELLOW = \033[0;33m
PURPLE = \033[0;35m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJ) $(HEADER) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) -lreadline
	@echo "$(GREEN)		*----------------------------------*\n \
			|   	[OK] $(NAME) created     |\n \
			*----------------------------------*$(NC)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C libft
	@echo "$(BLUE)		*----------------------------------*\n \
			|        [OK] LIBFT created        |\n \
			*----------------------------------*$(NC)"

clean:
	@make clean -C libft
	@echo "$(YELLOW)[OK]$(NC) libft clean completed"
	rm -f $(OBJ)
	@echo "$(PURPLE)[OK]$(NC) mini_shell clean completed"

fclean: clean
	@make fclean -C libft
	@echo "$(YELLOW)[OK]$(NC) libft fclean completed"
	rm -f $(NAME)
	@echo "$(PURPLE)[OK]$(NC) mini_shell fclean completed"

re: fclean all

.PHONY: all clean fclean re