/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2022/03/23 17:49:12 by jibot            ###   ########.fr       */
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

int	ft_abs(int a);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_strlen(char *str);
int	ft_tablen(char **tab);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
char	**token_join(char **args, char *token);
int	is_delim(char c);
char	*get_env_vars(char *token, char **env_vars);
char	**parsing(char **lex_tab, char **env_vars);
char	**lexing(char *line);

#endif