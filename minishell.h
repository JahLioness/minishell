/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:44:59 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/02 10:50:07 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

static int			g_sig = 0;

typedef enum s_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}					t_redir_type;

typedef enum s_token_type
{
	T_CMD,
	T_AND,
	T_AND_E,
	T_OR,
	T_PIPE,
	O_BRACKET,
	C_BRACKET
}					t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_redir
{
	char			*file;
	int				fd_redir;
	t_redir_type	type;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				error;
	t_redir			*redir;
}					t_cmd;

typedef struct s_token
{
	int				status;
	int				prev_fd;
	t_token_type	type;
	t_cmd			*cmd;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_mini
{
	int				error;
	int				is_heredoc;
	char			*cell;
	t_env			*env;
	t_token			*tokens;
	struct s_mini	*prev;
	struct s_mini	*next;
}					t_mini;

typedef struct s_wildcard
{
	int				i;
	DIR				*dir;
	struct dirent	*entry;
}					t_wildcard;

typedef struct s_export_utils
{
	char			*key;
	char			*value;
	char			*op;
}					t_export_utils;

typedef struct s_ast
{
	t_token			*token;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*parent;
}					t_ast;

/*			UTILS           */
int					ft_check_whitespace(char *str, int i);
int					ft_is_not_stop(char c);
char				*ft_strjoin_free(char *s1, char *s2);
char				*ft_trim_empty_quotes(char *cell);
char				*ft_trim_quote(char *line, int i, int j);
char				*ft_tabchr(char **tab, char *str, char c);
void				ft_print_lst(t_mini *mini);

/*			PROMPT          */
char				*ft_get_prompt(t_env *env);
void				ft_prompt(t_mini **mini_lst, char **envp);

/*			ENV             */
int					ft_print_env(t_env **env, int fd);
int					ft_envsize(t_env *env);
void				ft_change_env(t_env **env, char *pwd, char *old_dir);
void				ft_envadd_back(t_env **env, t_env *new);
void				ft_change_exit_status(t_env *status, char *value);
void				ft_env_delone(t_env *env);
t_env				*ft_get_env(char **envp);
t_env				*ft_env_copy(t_env *env);
t_env				*ft_envnew(char *key, char *value);
t_env				*ft_envlast(t_env *env);
t_env				*ft_get_exit_status(t_env **env);

/*			CLEAR			*/
void				ft_clearenv(t_env **env);
void				ft_clear_lst(t_mini **mini);
void				ft_clear_token(t_token **token);

/*			MINI			*/
int					ft_check_quote(char *line, int i);
int					ft_verif_tokens(t_mini *mini);
char				**ft_trim_quote_args(char **args);
void				ft_is_heredoc(t_mini *mini);
void				ft_miniadd_back(t_mini **mini, t_mini *new);
t_mini				*ft_new_mini(char *line, t_mini **mini);
t_mini				*ft_mini_init(void);
t_mini				*ft_new_mini_part(t_mini **mini, t_mini *new);
t_mini				*ft_minilast(t_mini *mini);

/*			TOKEN			*/
void				ft_init_token(t_mini *mini, char *line);
void				ft_tokenadd_back(t_token **token, t_token *new);
void				ft_init_token_cmd(t_token *new, char *cell, int *i);
void				ft_token_delone(t_token *token);
void				ft_token_delone_utils(t_token *token);
void				ft_token_clear_redir(t_redir *redir);
t_token				*ft_token_init(void);
t_token				*ft_tokenlast(t_token *token);

/*			ARGS				*/
int					ft_countwords_args(char *str);
int					ft_get_index_arg_utils(char *str, int *i);
int					ft_compare(char *pattern, char *str);
int					ft_check_star(char *arg);
int					ft_get_redir_args_len(t_token *redir);
int					ft_check_operator(t_token *token);
int					ft_verif_line(t_token *token);
char				**ft_get_args(char *cell, int *i);
char				*ft_check_expand(char *str, t_env **env, int j);
char				**ft_wildcard_check(char **args, int j);
char				**ft_init_wildcard(char **args, int j);
char				**ft_check_redir_args(t_token *token);
char				**ft_check_redir_args_utils(t_token *redir, char **ret);
char				*ft_verif_arg(char **str, t_env **env, t_cmd *cmd, int j);
char				*ft_check_acc_expand(char *str, t_env **env, t_cmd *cmd,
						int j);
void				ft_redir_addback(t_redir **redir, t_redir *new);
t_token				*ft_redir_token(t_token *token);
t_redir				*ft_init_redir(void);

/*			REDIR			*/
int					ft_is_redir(char *str, int *i);
char				*ft_get_redir_file(char *str, int *i);
char				*ft_get_redir_type(char *str, int *i);

/*			BUILTINS		*/
int					ft_echo(int fd, char **str, char **flag);
int					ft_export(t_export_utils *utils, t_env **env, int fd);
void				ft_unset(char *key, t_env **env);
int					ft_cd(char **path, t_env **env);
int					ft_pwd(int fd, t_env **env);
int					ft_exit(t_ast *root, t_mini **mini, char *prompt,
						char **envp);

/*		BUILTINS UITLS		*/
int					ft_check_value(t_env **env, char *key, char *op,
						char *value);
int					ft_is_num(char *str);
char				*ft_check_key(char *key);
void				ft_echo_putendl(int fd, char **str);
void				ft_echo_putstr(int fd, char **str);
void				ft_echo_wt_flag(int fd, char **str, char **flag);
void				ft_free_exit(t_ast *root, t_mini **mini, char **envp,
						char *prompt);
void				ft_print_exit(char *str);

/*			EXEC			*/
int					ft_exec_builtin(t_token *token, t_env **env, int fd);
int					ft_exec_unset(t_cmd *cmd, t_env **env);
int					ft_exec_export_utils(char *arg, t_export_utils *utils);
int					ft_exec_export(t_token *token, t_env **env, int fd);
int					ft_exec_cmd(t_ast *root, t_mini **mini, char *prompt);
int					ft_exec_cmd_pipe(t_ast *root, t_mini **mini, char *prompt);
int					ft_exec_cmd_path(t_ast *root, t_mini **mini, char **envp,
						char *prompt);
char				**ft_get_args_echo(char **args, t_env **env);
char				**ft_get_flag_echo(char **args);
char				**ft_get_envp(t_env **env);
char				*ft_get_cmd_path_env(char *cmd, char **env);
void				ft_exec_token(t_mini **mini, char *prompt);
void				ft_set_var_underscore(char **args, t_env **env,
						char **envp);

/*			EXEC_UTILS		*/
// int					ft_is_redir(t_token *token);
int					ft_is_operator(t_token *token);
int					ft_is_pipe(t_token *token);
int					ft_is_bracket(t_token *token);
int					ft_is_builtin(char *cmd);
int					ft_exec_cmd_error(t_ast *root, t_mini **mini, char **envp,
						char *prompt);
int					ft_handle_redir_file(int redir_fd, t_ast *root,
						char *file_heredoc);

/*			AST				*/
int					ft_check_bracket(t_token *token);
void				ft_clear_ast(t_ast *root);
void				print_ast(t_ast *node, int depth, char c);
t_ast				*ast_init(void);
t_ast				*ast_new_node(t_token *token);
t_ast				*create_ast(t_token *first, t_token *last);
t_ast				*create_operator_node(t_token *token, t_ast *left,
						t_ast *right);

/*			SIGNALS		*/
void				ft_get_signal_cmd(void);
void				ft_get_signal(void);
void				ft_handler_sigquit(int signum);

#endif