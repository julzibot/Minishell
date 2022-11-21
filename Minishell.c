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
	if (!s1 || !s2)
		return (-1);
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

	if (!str)
		return (0);
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

void	lst_addback(t_cmd *parse_list)
{
	t_cmd	next_cmd;
	
	next_cmd.next = NULL;
	// next_cmd.dflt_in = parse_list->dflt_in;
	// next_cmd.dflt_out = parse_list->dflt_out;
	parse_list->next = &next_cmd;
}

// void	list_init(t_cmd parse_list)
// {
// 	parse_list.next = NULL;
// 	// parse_list.p_in = open("dflt_in", O_CREAT | 0644);
// 	// parse_list.p_out = open("dflt_out", O_CREAT | 0644);
// }

void	outfile_redir(t_cmd *parse_cmd, char **redir_ptr, int type)
{
	char	*filename;

	filename = redir_ptr[1];
	if (!type)
		parse_cmd->outfile = open(filename, O_CREAT | O_RDWR, 0644);
	else if (type)
		parse_cmd->outfile = open(filename, O_CREAT | O_RDWR| O_APPEND, 0644);
}

void	infile_redir(t_cmd *parse_cmd, char **redir_ptr, int type)
{
	char	*filename_delim;
	char	*line;

	filename_delim = redir_ptr[1];
	line = NULL;
	if (!type)
		parse_cmd->infile = open(filename_delim, O_CREAT | O_RDWR, 0644);
	else if (type)
	{
		parse_cmd->infile = open(".here_doc", O_CREAT | O_RDWR, 0644);
		while (!(ft_strlen(filename_delim) == ft_strlen(line) \
			&& !ft_strncmp(line, filename_delim, ft_strlen(filename_delim))))
		{
			line = readline("> ");
			ft_printf(parse_cmd->infile, "%s\n", line);
		}
	}
}

void	parsing(char **lex_tab)
{
	t_cmd	parse_list;
	t_cmd	*temp;
	int		i;

	i = -1;
	parse_list.next = NULL;
	temp = &parse_list;
	while (lex_tab[++i])
	{
		if (!ft_strncmp(lex_tab[i], ">", 1) || !ft_strncmp(lex_tab[i], ">>", 2))
			outfile_redir(&parse_list, lex_tab + i, ft_strlen(lex_tab[i]) - 1);
		else if (!ft_strncmp(lex_tab[i], "<", 1) || !ft_strncmp(lex_tab[i], "<<", 2))
			infile_redir(&parse_list, lex_tab + i, ft_strlen(lex_tab[i]));
		else if (!ft_strncmp(lex_tab[i], "|", 1))
		{
			lst_addback(temp);
			temp = temp->next;
		}

	}
	//return (parse_list);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*line;
	char	**test;
	int	i;
	
	while (1)
	{
		i = -1;
		line = readline("Mini_chiale> ");
		test = lexing(line);
		while (test[++i])
		{
			printf("%s\n", test[i]);
		}
		printf("\n\n");
		parsing(test);
	}
	return (0);
}
