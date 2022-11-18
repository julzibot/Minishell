/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2022/03/23 17:49:12 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_abs(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (*s1 && *s2 && *s1 == *s2 && ++i < n)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return	(i);
}

//is_delim :
//1 -> redir
//2 -> pipe
//3 -> quote
//4 -> isspace
//0 -> not a delim
int	is_delim(char c)
{	
	if (c == '<' || c == '>')
		return (1);
	else if (c == '|')
		return (2);
	else if (c == '\'' || c == '\"')
		return (3);
	else if (c == ' ' || c == '	')
		return (4);
	else
		return(0);
}

int	lineseg(char *line, char **lex_tab)
{
	int	i;
	int	charcount;
	int	quoted;
	char	*seg;

	i = 0;
	charcount = 1;
	quoted = 0;
	if (is_delim(line[i]) == 3)
		quoted = 1;
	while (line[++i] && ((quoted && line[i] != line[0]) \
		|| (!quoted && !is_delim(line[i]))))
		charcount++;
	seg = malloc(charcount + 1);
	i = 0;
	seg[0] = line[0];
	while (line[++i] && ((quoted && line[i] != line[0]) 
		|| (!quoted && !is_delim(line[i]))))
		seg[i] = line[i];
	seg[i] = '\0';
	*lex_tab = seg;
	return (i + quoted);
}
 
int	pipe_redir(char *c, char **lex_tab)
{
	char	*str;
	int		count;

	if (is_delim(c[0]) == 1 && c[0] == c[1])
	{
		str = malloc(3);
		str[1] = c[0];
		str[2] = '\0';
		count = 1;
	}
	else
	{
		str = malloc(2);
		str[1] = '\0';
		count = 0;
	}
	str[0] = c[0];
	*lex_tab = str;
	return (count);
}


char	**lexing(char *line)
{
	int	i;
	int j;
	char	**lex_tab;

	i = -1;
	j = 0;
	lex_tab = malloc(sizeof(char*) * 8/*arg_count(line)*/);
	while (line[++i])
	{
		if (is_delim(line[i]) == 1 || is_delim(line[i]) == 2/*is pipe or redir*/)
		{
			i += pipe_redir(line + i, lex_tab + j);
			j++;
		}
		else if (is_delim(line[i]) == 3 || !is_delim(line[i]) /*is quote, or beginning of word*/)
		{
			i += lineseg(line + i, lex_tab + j);
			if (is_delim(line[i]) != 4 /*is not a space or tab*/)
				i--;
			j++;
		}
	}
	lex_tab[j] = NULL;
	return (lex_tab);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*line;
	char	**test;
	int	i;
	// int	hist_count;
	// int hist_fd;

	// if (access("history", F_OK))
	// 	hist_fd = open("history", O_CREAT | O_RDWR | O_TRUNC, 0000644);
	// else
	// 	hist_fd = open("history", O_WONLY);
	// hist_count = 1;
	while (1)
	{
		i = -1;
		line = readline("Mini_chiale> ");
		test = lexing(line);
		while (test[++i])
		{
			printf("%s\n", test[i]);
		}
	}
	return (0);
}
