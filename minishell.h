/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/03 18:51:19 by mstojilj         ###   ########.fr       */
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
# include "./ft_printf/ft_printf.h"

# ifndef PATH_MAX
# define PATH_MAX 1024
# endif

typedef	struct	s_cmd
{
	char			*cmd;       // cd
	char			**args;     // cd -> ../folder <-
	char			**env_vars; // a=b (export)
	int				infile;     // fd
	int				outfile;    // fd
	int				*quoted;
	int				*space_after;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*line;
	struct s_env	*next;
} t_env;

/*********MILAN 2/12/2022*********/
char	*ft_remove_cmd(char *line, char *remove);
int		ft_get_to_str(char *line, char *remove, int *j);
void 	rl_replace_line(const char *text, int clear_undo);

/***********LEXING**********/
int		arg_count(char *line);
int		lex_pipe_redir(char *c, char **lex_tab);
int		lineseg(char *line, int i, char **lex_tab, int quoted);
int		seg_size(char *line, int i);
int		is_delim(char c);
void	tab_list_init(char *line, char **lex_tab, t_cmd *parse_list);
char	**lexing(char *line, t_cmd *parse_list);

//static  char	*rem_quotes(char *str, int pos);
void			tab_list_init(char *line, char **lex_tab, t_cmd *parse_list);

/***********PARSING**********/
int		token_type(char *token, int quoted);
int		quotes_skip(char **tab, int *space_after);
char	*fuse_quotes(char *token, char **lex_tab, t_cmd *plist, int j);
char	*get_env_vars(char *token, char **env_vars);
char	**create_env_vars(char	*token, char **env_vars);
t_cmd	*parsing(char **lex_tab, t_cmd *parse_list); //returns a t_cmd*, each element in the list being a segment of the pipeline.

/***********BASIC**********/
int		ft_isspace(char c);
void	ft_strcpy(char *dst, char *src);
int		ft_abs(int a);
int		ft_tablen(char **tab);
char    *char_cat(char *str, char c);

/***********LIBFT**********/
int		ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_strlen(char *s);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

/***********UTILS**********/
char	**token_join(char **args, char *token);
char	*ft_strdup_free(char *str);
void				*ft_calloc(size_t count, size_t size);
// static char			*strseg(const char *str, char c);
// static unsigned int	size_count(const char *str, char c);
char				**ft_split(char const *s, char c);

/***********EXEC***********/
void	ft_exec_cmd(t_cmd *cmd, t_env **env_list, t_env **exp_list, char **env);
int		ft_exec(t_cmd *cmd, char **env);
char	*ft_substr(char *s, unsigned int start);
char	*ft_strstr(char *haystack, char *needle);

/***********ENV************/
int		ft_varlen(char *str);
void	ft_get_env(t_env **env_list, char **env);
void	ft_print_env(t_env **env_lst);
char	*ft_get_env_var(char *line, char *remove);
void	ft_add_queue(t_env **root, char *s);
void	ft_update_env(t_env **env_list, t_env **exp_list, char *line);

/***********ECHO***********/
void	ft_echo(char *s, int option);        // Echo function
void	ft_do_echo(char *s);				 // Echo (no options)
void	ft_do_nl_echo(char *s);				 // Echo -n
int		ft_get_echo_line(char *s);			 // Gets the echo line
int		ft_get_echo_n(char *s, int start);	 // Gets the echo -n line

/***********EXPORT*********/
void	ft_export(t_env **env_list, t_env **exp_list, char **vars);
void	ft_get_export(t_env **exp_list);
void	ft_add_after(t_env **env_list, int line_nb, char *s);
char	*ft_add_quotes(char *var);
int		ft_verify_double(t_env *env_list, char *line);
char	*ft_verify_env_var(char *s);
char	*ft_var_content(t_env **env_list, t_env **exp_list, char *line);
int		ft_verify_equal(char *s);

/***********DIRECTORY******/
void	ft_cd(t_env **exp_list, t_env **env_list, char *line);
int		ft_update_pwd(t_env **exp_list, t_env **env_list, char *var);
char	*ft_pwd(void);

/***********UNSET**********/
void	ft_unset(t_env **env_list, t_env **exp_list, char *s);
void	ft_remove_line(t_env **env_list, t_env *node);

#endif