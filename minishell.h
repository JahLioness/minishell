/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:44:59 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/25 12:15:13 by ede-cola         ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

extern int			g_sig;

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
	char			*file_heredoc;
	int				fd_redir;
	t_redir_type	type;
	struct s_redir	*next;
	struct s_redir	*prev;
}					t_redir;

typedef struct s_exec
{
	pid_t			pid;
	int				pipe_fd[2];
	int				prev_fd;
	int				error_ex;
	int				redir_in;
	int				redir_out;
	int				status;
}					t_exec;

typedef struct s_cmd
{
	char			*cmd;
	int				error;
	char			**args;
	t_redir			*redir;
	t_exec			exec;
	int				pipe;
	int				heredoc;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_token
{
	int				status;
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

typedef struct s_exec_utils
{
	t_ast			*granny;
	t_ast			*current;
	t_ast			*parent;
	t_mini			**mini;
	char			*prompt;
	char			**envp;
	int				len_cmd;
}					t_exec_utils;

/*			UTILS           */
int					ft_check_whitespace(char *str, int i);
int					ft_is_not_stop(char c);
char				*ft_strjoin_free(char *s1, char *s2);
char				*ft_trim_empty_quotes(char *cell);
char				*ft_trim_quote(char *line, int i, int j);
char				*ft_tabchr(char **tab, char *str, char c);
void				ft_print_lst(t_mini *mini);
void				ft_check_acco(char *str, int *i);
int					ft_set_quote(char *str, int *i);
void				ft_skip_betwen_quote(char *str, int *i, char quote);
int					ft_skip_spaces(char *str, int *i);
int					ft_is_pipe_init(char *line, int *i);
int					ft_is_pipe_alone(t_token *token);
void				ft_free_envp(t_exec_utils *e_utils);
char				*ft_ltoa(long long n);
char				*ft_get_quotes(char *str, char *ret, int i);

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
int					ft_is_var_in_env(t_env **env, char *var);

/*			CLEAR			*/
void				ft_clearenv(t_env **env);
void				ft_clear_lst(t_mini **mini);
void				ft_clear_token(t_token **token);
void				ft_clear_token_redir(t_redir *redir);
void				ft_clear_redir(t_redir *redir);
void				ft_clear_cmd(t_cmd **cmd);

/*			MINI			*/
int					ft_check_quote(char *line, int i);
int					ft_verif_tokens(t_mini *mini);
void				ft_cmd_syntax_error(t_token *token);
int					ft_check_redir_file(t_token *token);
char				**ft_trim_quote_args(char **args);
void				ft_is_heredoc(t_mini *mini);
void				ft_set_heredoc_node(t_cmd *cmd, t_mini *mini);
void				ft_miniadd_back(t_mini **mini, t_mini *new);
t_mini				*ft_new_mini(char *line, t_mini **mini);
t_mini				*ft_mini_init(void);
t_mini				*ft_new_mini_part(t_mini *new);
t_mini				*ft_minilast(t_mini *mini);

/*			TOKEN			*/
void				ft_init_token(t_mini *mini, char *line);
void				ft_tokenadd_back(t_token **token, t_token *new);
void				ft_init_token_cmd(t_token *new, char *cell, int *i);
void				ft_token_delone(t_token *token);
void				ft_token_delone_utils(t_token *token);
void				ft_init_token_cmd_pipe(t_token *new, t_cmd *cmd, char *cell,
						int *i);
void				ft_get_wildcard(t_cmd *cmd, int j);
void				ft_init_cmd_redir(t_cmd *new, char *cell, int *i);
t_token				*ft_token_init(void);
t_token				*ft_tokenlast(t_token *token);
void				init_exec(t_cmd *cmd);

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
char				**ft_check_redir_args_utils(t_token *redir, char **ret);
char				*ft_verif_arg(char **str, t_env **env, t_cmd *cmd, int j);
char				*ft_check_acc_expand(char *str, t_env **env, t_cmd *cmd,
						int j);
void				ft_redir_addback(t_redir **redir, t_redir *new);
t_token				*ft_redir_token(t_token *token);
t_redir				*ft_init_redir(void);
void				ft_skip_m_s_idx(int *i, int j, int *s_idx, int *m_idx);
char				*ft_get_value_from_varu(t_env *env, char *str, int i,
						char *ret);
int					ft_is_expandable(char *str);
int					ft_check_len(char *str, int i, char *key);
char				*ft_search_value(char *ret, t_env **env, int i, char *str);
int					ft_get_index_ba_var(char *str, int i);

/*			REDIR			*/
int					ft_is_redir(char *str, int *i);
char				*ft_get_redir_file(char *str, int *i, char *tofree);
char				*ft_get_redir_type(char *str, int *i);
void				set_redir_append(t_redir *current, t_exec *exec);
void				set_redir_output(t_redir *current, t_exec *exec);
void				ft_check_redir_arg(t_cmd *new, char *cell, int *i);

/*			BUILTINS		*/
int					ft_echo(int fd, char **str, char **flag);
int					ft_export(t_export_utils *utils, t_env **env, int fd);
int					ft_cd(char **path, t_env **env);
int					ft_pwd(int fd, t_env **env);
int					ft_exit(t_exec_utils *e_utils, t_cmd *cmd);
void				ft_unset(char *key, t_env **env);

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
char				*ft_get_old_pwd(t_env **env);
char				*ft_get_home(t_env **env);
char				*ft_gestion_tilde(t_env **env, char *new_dir);
void				ft_old_inexistent(t_env **env);
int					ft_no_oldir(t_env **env);
char				*ft_get_dir(char *new_dir, char *value);
int					ft_flags_len(char **flags);
char				*ft_get_arg_echo_loop(char *arg);
void				ft_value_len(char *value, char *key, t_env **env);

/*			EXEC			*/
int					exec_command(t_cmd *cmd, t_exec_utils *e_utils);
int					ft_exec_lst_cmd(t_ast *root, t_exec_utils *e_utils);
int					ft_exec_cmd(t_ast *root, t_exec_utils *e_utils);
int					ft_exec_multiple_cmd(t_exec_utils *e_utils, t_ast *current);
char				**ft_get_envp(t_env **env);
char				*ft_get_cmd_path_env(char *cmd, char **env);
void				ft_exec_token(t_mini **mini, char *prompt);
void				ft_set_var_underscore(char **args, t_env **env,
						char **envp);
void				ft_exec_builtins(t_cmd *cmd, t_exec_utils *e_utils);

/*			EXEC BUILTINS	*/
int					ft_exec_builtin(t_cmd *cmd, t_env **env, int fd);
int					ft_exec_unset(t_cmd *cmd, t_env **env, int fd);
int					ft_exec_export_utils(char *arg, t_export_utils *utils);
int					ft_exec_export(t_cmd *cmd, t_env **env, int fd);
char				**ft_get_args_echo(char **args, t_env **env);
char				**ft_get_flag_echo(char **args);
void				handle_builtin(t_cmd *cmd, t_mini *last, t_redir *tmp,
						t_exec *exec);
void				handle_exit(t_exec_utils *e_utils, t_cmd *cmd);

/*			EXEC HEREDOC	*/
int					handle_heredoc(t_cmd *node_heredoc, t_mini **mini,
						char *prompt);
char				*handle_expand_heredoc(t_cmd *cmd, t_mini *last,
						char *line);
void				ft_get_heredoc(t_cmd *cmd, t_mini *last,
						t_redir *current_redir);
void				generate_heredoc_file(t_redir *redir);
void				unlink_files(t_cmd *cmd);
// t_cmd				*get_heredoc_node(t_mini *last);
t_cmd				*get_heredoc_node(t_cmd *cmd);
int					ft_count_heredoc(t_cmd *cmd);
void				ft_get_heredoc_loop(t_token *token, t_mini **mini,
						char *prompt);
char				*ft_verif_arg_heredoc(char **str, t_env **env, t_cmd *cmd,
						int j);
char				*ft_get_ret_heredoc(t_env *env, char *ret, char *str,
						int i);

/*			EXEC_UTILS		*/
int					set_e_status(int status, t_mini *last);
int					ft_is_operator(t_token *token);
int					ft_is_pipe(t_token *token);
int					ft_is_bracket(t_token *token);
int					ft_is_builtin(char *cmd);
int					ft_cmdsize(t_cmd *cmd);
void				process_child(t_cmd *cmd, int i, int len_cmd);
void				reset_fd(t_exec *exec);
void				handle_expand(t_cmd *cmd, t_mini *last);
void				close_fd(int *fd, int prev_fd);
int					ft_waitpid(t_cmd *cmd, t_mini *last, int len_cmd);
int					ft_exec_multi_lst_cmd(t_exec_utils *e_utils, t_cmd *cmd,
						int i, int len_cmd);
void				ft_close_pipe(t_cmd *cmd);
void				ft_check_builtin(t_cmd *cmd, t_exec_utils *e_utils,
						t_mini *last);
void				ft_expand_redir_gestion(t_cmd *cmd, t_exec_utils *e_utils,
						t_mini *last);

/*			EXEC_REDIR		*/
void				handle_redir(t_cmd *cmd, t_mini **mini);
void				ft_handle_redir_file(t_cmd *cmd, t_mini *last);
void				cat_wt_symbole(t_cmd *cmd, t_exec *exec);
void				builtin_w_redir(t_redir *tmp_redir, t_exec *exec,
						t_cmd *cmd);
void				handle_redir_dup(t_exec *exec, t_cmd *cmd, t_mini *last);
void				set_redir(t_redir *current, t_exec *exec, t_cmd *cmd);
void				ft_handle_redir_file_loop(t_redir *redir, t_exec *exec,
						t_cmd *cmd, t_mini *last);

/*			AST				*/
int					ft_check_bracket(t_token *token);
void				ft_clear_ast(t_ast *root);
void				print_ast(t_ast *node, int depth, char c);
t_ast				*ast_init(void);
t_ast				*ast_new_node(t_token *token);
t_ast				*create_ast(t_token *first, t_token *last);
t_ast				*create_operator_node(t_token *token, t_ast *left,
						t_ast *right);
/*			ERROR		*/
int					exit_free(t_ast *granny, t_mini **mini, char **envp,
						char *prompt);
void				msg_error(char *msg, char *cmd, char *strerror);
void				ft_exec_cmd_error(t_exec_utils *e_utils, char **envp);

/*			SIGNALS		*/
int					handle_sigint(t_exec *exec, t_mini *last);
int					handle_sigquit(t_exec *exec, t_mini *last);
void				ft_get_signal_cmd(void);
void				ft_get_signal(void);
void				ft_get_signal_heredoc(void);

#endif