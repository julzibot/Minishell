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

int	ft_tablen(char **tab)
{
	int	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	char	*dup;
	int	i;

	if (!str)
		return (NULL);
	dup = malloc(ft_strlen(str) + 1);
	i = -1;
	while (str[++i])
		dup[i] = str[i];
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1)
		return (s2);
	else if (!s2)
		return (s1);
	i = 0;
	j = 0;
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(s1);
	free(s2);
	return (str);
}

char	**token_join(char **args, char *token) // add a string to the end of a char**
{
	char	**tab;
	int		i;

	i = -1;
	tab = malloc(sizeof(char *) * (ft_tablen(args) + 2));
	if (!args)
	{
		tab[0] = ft_strdup(token);
		tab[1] = NULL;
		return (tab);
	}
	while (args[++i])
		tab[i] = ft_strdup(args[i]);
	tab[i] = ft_strdup(token);
	tab[i + 1] = NULL;
	// free(args);
	free(token); 
	return (tab);
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

// int	lineseg(char *line, char **lex_tab)
// {
// 	int	i;
// 	int	charcount;
// 	int	quoted;
// 	char	*seg;

// 	i = 0;
// 	charcount = 1;
// 	quoted = 0;
// 	if (is_delim(line[i]) == 3)
// 		quoted = 1;
// 	while (line[++i] && ((quoted && line[i] != line[0]) \
// 		|| (!quoted && !is_delim(line[i]))))
// 		charcount++;
// 	seg = malloc(charcount + 1);
// 	i = 0;
// 	seg[0] = line[0];
// 	while (line[++i] && ((quoted && line[i] != line[0]) 
// 		|| (!quoted && !is_delim(line[i]))))
// 		seg[i] = line[i];
// 	seg[i] = '\0';
// 	*lex_tab = seg;
// 	return (i + quoted);
// }
 
// int	lex_pipe_redir(char *c, char **lex_tab)
// {
// 	char	*str;
// 	int		count;

// 	if (is_delim(c[0]) == 1 && c[0] == c[1])
// 	{
// 		str = malloc(3);
// 		str[1] = c[0];
// 		str[2] = '\0';
// 		count = 1;
// 	}
// 	else
// 	{
// 		str = malloc(2);
// 		str[1] = '\0';
// 		count = 0;
// 	}
// 	str[0] = c[0];
// 	*lex_tab = str;
// 	return (count);
// }


// char	**lexing(char *line)
// {
// 	int	i;
// 	int j;
// 	char	**lex_tab;

// 	i = -1;
// 	j = 0;
// 	lex_tab = malloc(sizeof(char*) * 15/*arg_count(line)*/);
// 	while (line[++i])
// 	{
// 		if (is_delim(line[i]) == 1 || is_delim(line[i]) == 2/*is pipe or redir*/)
// 		{
// 			i += lex_pipe_redir(line + i, lex_tab + j);
// 			j++;
// 		}
// 		else if (is_delim(line[i]) == 3 || !is_delim(line[i]) /*is quote, or beginning of word*/)
// 		{
// 			i += lineseg(line + i, lex_tab + j);
// 			if (is_delim(line[i]) != 4 /*is not a space or tab*/)
// 				i--;
// 			j++;
// 		}
// 	}
// 	lex_tab[j] = NULL;
// 	return (lex_tab);
// }

// void	lst_next_cmd(t_cmd *parse_list)
// {
// 	t_cmd	next_cmd;
	
// 	next_cmd.next = NULL;
// 	parse_list->next = &next_cmd;
// 	parse_list = &next_cmd;
// }

// int	redir(t_cmd *parse_cmd, char **redir_ptr, int type)
// {
// 	char	*filename_delim;
// 	char	*line;

// 	filename_delim = redir_ptr[1];
// 	if (!filename_delim)
// 		return (0); // error handling here
// 	line = NULL;
// 	if (!type)
// 	{
// 		parse_cmd->infile = open(".here_doc", O_CREAT | O_RDWR, 0644);
// 		line = readline("> ");
// 		while (!(ft_strlen(filename_delim) == ft_strlen(line) \
// 			&& !ft_strncmp(line, filename_delim, ft_strlen(filename_delim))))
// 		{
// 			ft_printf(parse_cmd->infile, "%s\n", line);
// 			line = readline("> ");
// 		}
// 	}
// 	else if (type == 2)
// 		parse_cmd->infile = open(filename_delim, O_CREAT | O_RDWR, 0644);
// 	else if (type == 1)
// 		parse_cmd->outfile = open(filename_delim, O_CREAT | O_RDWR| O_APPEND, 0644);
// 	else if (type == 3)
// 		parse_cmd->outfile = open(filename_delim, O_CREAT | O_RDWR, 0644);
// 	return (1);
// }

// int	token_type(char *token)
// {
// 	if (!ft_strncmp(token, "<<", 2))
// 		return (0);
// 	else if (!ft_strncmp(token, ">>", 2))
// 		return (1);
// 	else if (!ft_strncmp(token, "<", 1))
// 		return (2);
// 	else if (!ft_strncmp(token, ">", 1))
// 		return (3);
// 	else if (!ft_strncmp(token, "|", 1))
// 		return (4);
// 	else if (token[0] == '\"')
// 		return (5);
// 	else if (token[0] == '\'')
// 		return (7);
// 	else
// 		return (6);
// }

// char	**create_env_vars(char	*token, char **env_vars) //search for NAME=VALUE in unquoted tokens, store in env_vars
// {
// 	int i;
// 	int	j;
// 	int	namelen;
// 	char	*cpy;

// 	i = 0;
// 	cpy = get_env_vars(ft_strdup(token), env_vars);
// 	while (cpy[i] && cpy[i] != '=')
// 		i++;
// 	if (cpy[i] && cpy[i] == '=') 
// 	{
// 		j = 0;
// 		while (env_vars && env_vars[j])
// 		{
// 			namelen = 0;
// 			while (env_vars[j][namelen] != '=')
// 				namelen++;
// 			if (!ft_strncmp(env_vars[j], cpy, namelen) && cpy[namelen] == '=')
// 			{
// 				free(env_vars[j]);
// 				env_vars[j] = cpy;
// 				return (env_vars);
// 			}
// 			j++;
// 		}
// 		env_vars = token_join(env_vars, cpy);
// 	}
// 	return (env_vars);
// }

// char	*get_env_vars(char *token, char **env_vars) // replace all $NAME by their values in the parsing arguments. TODO : handle ${NAME}
// {
// 	int	i;
// 	int	v_i;
// 	int	j;
// 	int	namelen;
// 	int	v_len;
// 	char	*value;
// 	char	*str;

// 	i = 0;
// 	value = NULL;
// 	str = NULL;
// 	printf("--%s\n", token);
// 	while (token[i] && token[i] != '$')
// 		i++;
	
// 	str = malloc(i + 1);
// 	v_i = -1;
// 	while (++v_i < i)
// 		str[v_i] = token[v_i];
// 	str[v_i] = '\0';

// 	while (token[i])
// 	{
// 		j = 0;
// 		while (env_vars && env_vars[j])
// 		{
// 			namelen = 0;
// 			while (env_vars[j][namelen] != '=')
// 				namelen++;
// 			if (!ft_strncmp(env_vars[j], token + i + 1, namelen) \
// 					&& (!token[i + namelen + 1] || token[i + namelen + 1] == '$'))
// 			{
// 				value = malloc(ft_strlen(env_vars[j]) - namelen);
// 				v_i = -1;
// 				v_len = namelen;
// 				while (env_vars[j][++v_len])
// 					value[++v_i] = env_vars[j][v_len];
// 				// while (token[++i + namelen])
// 				// 	value[v_i++] = token[i + namelen];
// 				value[++v_i] = '\0';
// 				str = ft_strjoin(str, value);
// 				i += namelen + 1;
// 				while (env_vars[j + 1])
// 					j++;
// 			}
// 			j++;
// 		}
// 		if (i == ft_strlen(str))
// 			return (str);
// 	}
// 	return (str);
// }

// char	**parsing(char **lex_tab, char **env_vars)
// {
// 	t_cmd	parse_list;
// 	t_cmd	*temp;
// 	int		i;
// 	int		type;

// 	i = -1;
// 	parse_list.next = NULL;
// 	parse_list.args = NULL;
// 	temp = &parse_list;
// 	while (lex_tab[++i])
// 	{
// 		type = token_type(lex_tab[i]);
// 		if (type < 4)
// 			i += redir(&parse_list, lex_tab + i, type);
// 		else if (type == 4)
// 			lst_next_cmd(temp); // handle pipes here
// 		else
// 		{
// 			if (type == 6)
// 				env_vars = create_env_vars(lex_tab[i], env_vars);
// 			if (type < 7)
// 				temp->args = token_join(temp->args, get_env_vars(lex_tab[i], env_vars));
// 			else
// 				temp->args = token_join(temp->args, lex_tab[i]);
// 		}
// 	}

// 	i = -1;
// 	while (temp->args && temp->args[++i])
// 		printf("%s\n", temp->args[i]);
// 	i = -1;
// 	while (env_vars && env_vars[++i])
// 		printf("env_var %d : %s\n", i, env_vars[i]);
// 	return(env_vars);	
// }

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*line;
	char	**tokens;
	char	**env_vars;
	
	env_vars = NULL;
	while (1)
	{
		line = readline("Mini_chiale> ");
		tokens = lexing(line);
		env_vars = parsing(tokens, env_vars);
	}
	return (0);
}
