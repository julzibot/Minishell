#include "minishell.h"

char	**lexing(char *line, t_cmd *parse_list)
{
	int		i;
	int 	j;
	char	**lex_tab;

	i = -1;
	j = 0;
	lex_tab = malloc(sizeof(char*) * (arg_count(line) + 1));
	tab_list_init(line, lex_tab, parse_list);
	if (line == NULL)
		return (NULL);
	while (line[++i])
	{
		if (is_delim(line[i]) == 2 || is_delim(line[i]) == 3/*is pipe or redir*/)
			i += lex_pipe_redir(line + i, lex_tab + j++);
		else if (is_delim(line[i]) == 1 || !is_delim(line[i]) /*is quote, or beginning of word*/)
		{
			if (is_delim(line[i]) == 1)
				parse_list->quoted[j] = 1;
			i = lineseg(line, i, lex_tab + j, parse_list->quoted[j]);
			if (is_delim(line[i]) != 4 /*is not a space or tab*/)
				i--;
			else
				parse_list->space_after[j] = 1;
			j++;
		}
	}
	lex_tab[j] = NULL;
	return (lex_tab);
}

t_cmd	*lst_next_cmd(t_cmd *temp)
{
	t_cmd	*next_cmd;
	
	next_cmd = malloc(sizeof(t_cmd));
	next_cmd->env_vars = temp->env_vars;
	next_cmd->quoted = temp->quoted;
	next_cmd->space_after = temp->space_after;
	next_cmd->next = NULL;
	next_cmd->args = NULL;
	temp->next = next_cmd;

	return (next_cmd);
}

int	redir(t_cmd *parse_cmd, char **redir_ptr, int type)
{
	char	*filename_delim;
	char	*line;

	filename_delim = redir_ptr[1];
	if (!filename_delim)
		return (0); // error handling here
	line = NULL;
	if (!type)
	{
		parse_cmd->infile = open(".here_doc", O_CREAT | O_RDWR, 0644);
		line = readline("> ");
		while (!(ft_strlen(filename_delim) == ft_strlen(line) \
			&& !ft_strncmp(line, filename_delim, ft_strlen(filename_delim))))
		{
			ft_printf(parse_cmd->infile, "%s\n", line);
			line = readline("> ");
		}
	}
	else if (type == 2)
		parse_cmd->infile = open(filename_delim, O_CREAT | O_RDWR, 0644);
	else if (type == 1)
		parse_cmd->outfile = open(filename_delim, O_CREAT | O_RDWR| O_APPEND, 0644);
	else if (type == 3)
		parse_cmd->outfile = open(filename_delim, O_CREAT | O_RDWR, 0644);
	return (1);
}

int	token_type(char *token, int quoted)
{
	if (!ft_strncmp(token, "<<", 2))
		return (0);
	else if (!ft_strncmp(token, ">>", 2))
		return (1);
	else if (!ft_strncmp(token, "<", 1))
		return (2);
	else if (!ft_strncmp(token, ">", 1))
		return (3);
	else if (!ft_strncmp(token, "|", 1))
		return (4);
	else if (quoted && token[ft_strlen(token) - 1] == '\"' \
			&& (token[0] == '\"' || (token[0] == '\'' && token[1] == '\"')) && ft_strlen(token) > 1)
		return (5);
	else if (quoted && token[ft_strlen(token) - 1] == '\'' \
			&& (token[0] == '\'' || (token[0] == '\"' && token[1] == '\'')) && ft_strlen(token) > 1)
		return (7);
	else
		return (6);
}

// void	split_var_quoted_args(t_cmd *temp)
// {
// 	int	i;

// 	i = -1;
// 	while(temp->args[++i])
// 	{
// 		if temp->quoted[]
// 	}
// }

t_cmd	*parsing(char **lex_tab, t_cmd *parse_list)
{
	t_cmd	*temp;
	int		i;
	int		type;
	char	*str;

	// i = -1;
	// if (lex_tab == NULL)
	// 	return (NULL);
	// while (lex_tab[++i])
	// 	printf("lex %d : %s\n", i, lex_tab[i]);
	
	// INIT
	i = -1;
	str = NULL;
	parse_list->next = NULL;
	parse_list->args = NULL;
	temp = parse_list;
	
	// EXPANSION
	while (lex_tab[++i])
	{
		type = token_type(lex_tab[i], parse_list->quoted[i]);
		if (type < 4)
			i += redir(parse_list, lex_tab + i, type);
		else if (type == 4)
			temp = lst_next_cmd(temp); // handle pipes here
		else
		{
			if (type == 6)
				parse_list->env_vars = create_env_vars(ft_strdup(lex_tab[i]), parse_list->env_vars);
			if (type < 7)
				str = fuse_quotes(get_env_vars(lex_tab[i], parse_list->env_vars), lex_tab, parse_list, i);
			else
				str = fuse_quotes(ft_strdup(lex_tab[i]), lex_tab, parse_list, i);
			temp->args = token_join(temp->args, str);
			i += quotes_skip(lex_tab + i, parse_list->space_after + i);
		}
	}

	// TEST PRINTS
	temp = parse_list;
	while (temp->next != NULL)
	{
		printf("%p\n", temp);
		i = -1;
		while (temp->args && temp->args[++i])
			printf("%s\n", temp->args[i]);
		temp = temp->next;
	}
	printf("%p\n", temp);
	i = -1;
	while (temp->args && temp->args[++i])
		printf("%s\n", temp->args[i]);
	
	i = -1;
	while (parse_list->env_vars && parse_list->env_vars[++i])
		printf("env_var %d : %s\n", i, parse_list->env_vars[i]);
	return(parse_list);	
}