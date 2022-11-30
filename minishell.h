/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2022/11/30 19:07:59 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include "./ft_printf/ft_printf.h"

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				infile;
	int				outfile;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*line;
	struct s_env	*next;
} t_env;

int		is_delim(char c);
char	**parsing(char **lex_tab, char **env_vars);
char	**lexing(char *line);
char	*get_env_vars(char *token, char **env_vars);

/***********BASIC**********/
int		ft_isspace(char c);
void	ft_strcpy(char *dst, char *src);
int		ft_abs(int a);
int		ft_tablen(char **tab);

/***********LIBFT**********/
int		ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_strlen(char *s);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

/**********UTILS***********/
char	**token_join(char **args, char *token);

/***********ENV************/
void	ft_get_env(t_env **env_list, char **env);
void	ft_print_env(t_env **env_lst);
void	ft_add_queue(t_env **root, char *s);

/***********ECHO***********/
void	ft_echo(char *s, int option);        // Echo function
void	ft_do_echo(char *s);				 // Echo (no options)
void	ft_do_nl_echo(char *s);				 // Echo -n
int		ft_get_echo_line(char *s);			 // Gets the echo line
int		ft_get_echo_n(char *s, int start);	 // Gets the echo -n line

/***********EXPORT*********/
void	ft_export(t_env **env_list, char *line);
void	ft_add_after(t_env **env_list, int line_nb, char *s);


/***********UNSET**********/
void	ft_unset(t_env **env_list, char *s);
void	ft_remove_line(t_env **env_list, t_env *node);

#endif