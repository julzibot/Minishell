/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:39:38 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 19:05:13 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/signal.h>
# include <termios.h>
# include <termcap.h>
# include "./ft_printf/ft_printf.h"

# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif

# ifndef PROMPT
#  define PROMPT "Mini_chelou: "
# endif

# ifndef CMD
#  define CMD 1
# endif

# ifndef ENV_VAR
#  define ENV_VAR 2
# endif

# ifndef NOT_EXEC
#  define NOT_EXEC 126
# endif

# ifndef NOT_CMD
#  define NOT_CMD 127
# endif

# ifndef SIG_C
#  define SIG_C 130
# endif

typedef struct s_env
{
	char			*line;
	struct s_env	*next;
}	t_env;

typedef struct s_seg
{
	int	count;
	int	quoted;
	int	q_type;
	int	s_i;
	int	var_q_type;
	int	is_var;
	int	var_quoted;
	int	match;
	int	namelen;
	int	j;
}	t_seg;

typedef struct s_lex
{
	char	**lex_tab;
	int		delim;
	int		error;
	int		i;
	int		j;
}	t_lex;

typedef struct s_pars
{
	int		i;
	int		err;
	int		type;
	char	*str;
}	t_pars;

typedef struct s_gl_env
{
	struct termios	term;
	pid_t			gl;
	long long int	error_code;
	char			*curr_pwd;
	char			*old_pwd;
	char			*abs_path;
	t_env			*env_list;
	t_env			*exp_list;
	struct termios	term_orig;
	struct termios	term_mod;
}	t_gl_env;

typedef struct s_cmd
{
	pid_t			shell_pid;
	pid_t			heredoc_pid;
	char			**args;
	char			**env_vars;
	int				infile;
	int				redir_in;
	int				outfile;
	int				piped;
	int				redir[2];
	int				in_pipe[2];
	int				out_pipe[2];
	int				heredoc[2];
	int				*quoted;
	int				*space_after;
	struct s_cmd	*next;
}	t_cmd;

/**************INIT***************/
void	parse_init(t_cmd *parse_list, char **env_vars);
void	ft_init_env(char **envp);

/*********MILAN 2/12/2022*********/
void	rl_replace_line(const char *text, int clear_undo);

/***********LEXING**********/
int		arg_count(char *line);
int		lex_pipe_redir(char *c, char **pr_token);
int		lineseg(char *line, int i, char **lex_tab, int quoted);
int		seg_size(char *line, int i);
int		is_delim(char c);
void	tab_list_init(char *line, t_cmd *parse_list);
char	**lexing(char *line, t_cmd *parse_list);
int		f_return(char *seg, t_seg *sv, int ret);
int		end_lex_pipetest(t_lex *l);
int		only_spaces(char *str);

/***********PARSING**********/
int		token_type(char *token, int quoted);
int		quotes_skip(char **tab, int *space_after);
char	*fuse_quotes(char *token, char **lex_tab, t_cmd *plist, int j);
char	**create_env_vars(char *token, char **env_vars, t_env *env_list);
t_cmd	*parsing(char **lex_tab, t_cmd *parse_list);
/*---GEt_env_VARS---*/
char	*get_env_vars(char *token, char **env_vars, t_env *env_list);
char	*get_vars_init(char *token);
int		value_size(char *env_var, int namelen, int quoted);
int		reset_namelen(char *token);
	/*---REDIRS---*/
int		heredoc_handle(t_cmd *cmd, char *filename_delim);
int		redir(t_cmd *cmd, char **redir_ptr, int type);
int		redir_err_msg(t_pars *p, char *filename,
			char **lex_tab, t_cmd *parse_list);

/***********BASIC**********/
int		ft_isspace(char c);
void	ft_strcpy(char *dst, char *src);
int		ft_abs(int a);
int		ft_tablen(char **tab);
char	*char_cat(char *str, char c);

/***********LIBFT**********/
int		ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_strlen(char *s);
int		ft_is_alnum(char c);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2, int must_free);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_itoa(int n);

/***********UTILS**********/
char	**token_join(char **args, char *token);
void	*ft_calloc(size_t count, size_t size);
int		cmd_lstsize(t_cmd *list);
int		env_lstsize(t_env *list);
// static char			*strseg(const char *str, char c);
// static unsigned int	size_count(const char *str, char c);
char	**ft_split(char const *s, char c);
char	**ft_tabdup(char **tab, int must_free);
int		ft_atoi(const char *str);
int		ft_is_digit(char *s);

/***********EXEC***********/
int		ft_exec_cmd(t_cmd *cmd, char **env, char **tokens, int cmd_i);
int		ft_exec(t_cmd *cmd, char **envp, char **tokens);
void	ft_close_fds(t_cmd *cmd);
/*BUILTIN*/
int		ft_do_builtin(t_cmd *cmd, int builtin, char **tokens);
int		exec_builtin(t_cmd *cmd, int builtin, char **tokens);
int		is_builtin(t_cmd *cmd);
/*CHECK CMD*/
char	*ft_cmd_check(t_cmd *pl, char *cmd, int option);
char	*ft_path_check(char **paths, char *path, int option);
char	*ft_get_gl_env_path(void);
void	ft_free_paths(char **paths);
char	*ft_substr(char *s, unsigned int start);
char	*ft_strstr(char *haystack, char *needle);

/***********ENV************/
int		ft_varlen(char *str);
void	ft_get_env(t_env **env_list, char **env);
void	ft_get_exp(t_env **exp_list, char **env);
int		ft_print_env(t_env *env_lst, t_cmd *cmd);
char	*ft_get_env_var(char *line, char *remove);
int		ft_update_var(t_env **env_list, char *s);
void	ft_add_queue(t_env **root, char *s);
void	ft_add_after(t_env **env_list, int line_nb, char *s);
void	ft_sort_list(t_env **list);
void	ft_update_env(char *line);
void	ft_get_export(t_env **exp_list);

/***********ECHO***********/
void	ft_echo(t_cmd *cmd);
void	ft_do_echo(t_cmd *cmd, int fd);
int		ft_is_echo_nl(char *s);

/***********EXPORT*********/
int		ft_export(t_cmd *cmd);
void	ft_equal_var(char *line);
void	ft_not_equal_var(t_cmd *cmd, char *line);
void	ft_loop_assign_vars(char *line);
int		ft_change_var_content(char *line);
/*VERIFY*/
int		ft_simple_equal(char *s);
int		ft_verify_double(char *line);
int		ft_verify_equal(char *s);
int		ft_verify_exp_var(char *line);
int		ft_verify_err_var(char *line);
int		ft_verify_alphanum(char c);
int		ft_find_double(t_env *list, char *line);
/*QUOTES*/
char	*ft_add_quotes(char *var);
int		ft_do_quotes(char *var, char *env_var, int i, int j);
char	*ft_clean_var(char *s);
int		ft_find_quotes(char *s);

/***********DIRECTORY******/
int		ft_cd(t_cmd *cmd);
int		ft_cd_error(t_cmd *cmd);
int		ft_update_pwd(char *var);
char	*ft_pwd(t_cmd *cmd);

/***********UNSET**********/
int		ft_unset(t_cmd *cmd);
void	ft_remove_line(t_env **env_list, t_env *node);
int		ft_do_unset(t_cmd *cmd, char *line);
int		ft_unset_variable(t_env **list, char *s);

/***********EXIT***********/
void	ft_exit(t_cmd *cmd, int error_code, char **tokens);
void	ft_free_env(t_env *env);
void	ft_free_list(t_env **list);
int		ft_arrlen(char **s);
void	ft_free_char_array(char **s);
void	free_list(t_cmd *parse_list, char **tokens);
void	ft_free_exit(t_cmd *parse_list, char **tokens);
int		ft_get_exit_code(int status);

/**********SIGNALS*********/
void	ft_handle_sigint(int sig);
void	ft_handle_sigquit(int sig);
void	ft_child_sigint(int sig);
void	ft_child_sig(void);
void	check_line_exists(char *line, t_cmd *pl, char **tokens);
void	ft_display_nl(int sig);
void	ft_afterchild_sig(void);
void	ft_printquit(int sig);
void	f__kthenorm(int argc, char **argv);
void	ft_sig_init(void);

/***********ERRORS*********/
int		ft_print_error(int error_code, t_cmd *cmd, char *line);
void	ft_get_err_code(int error_code);

#endif