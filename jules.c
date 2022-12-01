#include "minishell.h"

int	lineseg(char *line, int i, char **lex_tab)
{
	int	s_i;
	int	quoted;
	int	next_quoted;
	char	*seg;

	s_i = 0;
	quoted = 0;
	next_quoted = 0;
	if (is_delim(line[i]) == 1)
		quoted = 1;
	seg = malloc(sizeof(char*));
	if (!quoted && i > 0 && is_delim(line[i - 1]) == 1)
	{
		s_i++;
		seg[0] = line[i - 1];
	}
	seg[s_i++] = line[i];
	while (line[++i] && ((quoted && line[i] != seg[0]) 
		|| (!quoted && !is_delim(line[i]))))
		seg[s_i++] = line[i];
	seg[s_i] = line[i];
	if (!quoted && is_delim(line[i]) == 1)
		next_quoted = 1;
	seg[s_i + quoted + next_quoted] = '\0';
	*lex_tab = seg;
	return (i + quoted);
}
 
int	lex_pipe_redir(char *c, char **lex_tab)
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
	lex_tab = malloc(sizeof(char*) * 15/*arg_count(line)*/);
	while (line[++i])
	{
		if (is_delim(line[i]) == 2 || is_delim(line[i]) == 3/*is pipe or redir*/)
		{
			i += lex_pipe_redir(line + i, lex_tab + j);
			j++;
		}
		else if (is_delim(line[i]) == 1 || !is_delim(line[i]) /*is quote, or beginning of word*/)
		{
			i = lineseg(line, i, lex_tab + j);
			if (is_delim(line[i]) != 4 /*is not a space or tab*/)
				i--;
			j++;
		}
	}
	lex_tab[j] = NULL;
	return (lex_tab);
}

void	lst_next_cmd(t_cmd *parse_list)
{
	t_cmd	next_cmd;
	
	next_cmd.next = NULL;
	parse_list->next = &next_cmd;
	parse_list = &next_cmd;
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

int	token_type(char *token)
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
	else if (token[0] == '\"' && token[ft_strlen(token) - 1] == '\"')
		return (5);
	else if (token[0] == '\'' && token[ft_strlen(token) - 1] == '\'')
		return (7);
	else
		return (6);
}

char	**create_env_vars(char	*token, char **env_vars) //search for NAME=VALUE in unquoted tokens, store in env_vars
{
	int i;
	int	j;
	int	namelen;
	char	*cpy;

	i = 0;
	cpy = get_env_vars(ft_strdup(token), env_vars);
	while (cpy[i] && cpy[i] != '=')
		i++;
	if (cpy[i] && cpy[i] == '=') 
	{
		j = 0;
		while (env_vars && env_vars[j])
		{
			namelen = 0;
			while (env_vars[j][namelen] != '=')
				namelen++;
			if (!ft_strncmp(env_vars[j], cpy, namelen) && cpy[namelen] == '=')
			{
				free(env_vars[j]);
				env_vars[j] = cpy;
				return (env_vars);
			}
			j++;
		}
		env_vars = token_join(env_vars, cpy);
	}
	return (env_vars);
}

// int	search_name()

char	*get_env_vars(char *token, char **env_vars) // replace all $NAME by their values in the parsing arguments. TODO : handle ${NAME}
{
	int	i;
	int	v_i;
	int	j;
	int	quoted;
	int	namelen;
	int	v_len;
	char	*value;
	char	*str;

	i = 0;
	quoted = 0;
	value = NULL;
	str = NULL;
	printf("--%s\n", token);
	if (!token)
		return (NULL);
	if (token && token[0] == '\"')
		i++;
	while (token && token[i] && token[i] != '$')
		i++;
	
	// keep the first part of the token (eg "stuff" in "stuff$NAME")
	str = malloc(i + 1);
	v_i = -1;
	// if (token[0] == '"')
	// 	skip++;
	while (++v_i < i && token[v_i])
		str[v_i] = token[v_i];
	str[v_i] = '\0';

	// successively change $NAME# to VALUE#. if !FALSENAME, the "$FALSENAME" part of the token is eliminated
	while (token[i])
	{
		j = 0;
		while (env_vars && env_vars[j])
		{
			namelen = 0;
			while (env_vars[j][namelen] != '=')
				namelen++;
			if (!ft_strncmp(env_vars[j], token + i + 1, namelen) \
					&& (!token[i + namelen + 1] || token[i + namelen + 1] == '$' \
					|| token[i + namelen + 1] == '"')) // if NAME matches in the token
			{
				if (token[0] == '\"' && token[i + namelen + 1] == '\"')
					quoted = 1;
				value = malloc(ft_strlen(env_vars[j]) - namelen + quoted);
				v_i = -1;
				v_len = namelen;
				while (env_vars[j][++v_len])
					value[++v_i] = env_vars[j][v_len];
				value[++v_i] = '\"';
				value[++v_i + quoted] = '\0';
				str = ft_strjoin(str, value);
				while (env_vars[j + 1])
					j++;
			}
			j++;
		}
		while (token[++i] && token[i] != '$')
			;
	}
	return (str);
}

char	*rem_quotes(char *str)
{
	int	i;
	int	len;
	char	*ret;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	ret = malloc(len);
	if (str[0] == '\"')
	{
		i = 0;
		while (str[++i])
			ret[i - 1] = str[i];
		ret[i - 1] = 0;
	}
	else if (str[len - 1] == '\"')
	{
		i = -1;
		while (str[++i] != '\"')
			ret[i] = str[i];
		ret[i] = 0;
	}
	free(str);
	return (ret);
}

char	*fuse_quotes(char *token, char **lex_tab, char **env_vars)
{
	int	i;
	int	j;
	int	quote_at_end;
	char	*str;

	if (token_type(token) == 5)
		token = rem_quotes(token);
	i = ft_strlen(token) - 1;
	quote_at_end = 0;
	j = 0;
	if (token[i] == '"')
		quote_at_end = 1;
	while (quote_at_end)
	{
		str = get_env_vars(lex_tab[j], env_vars);
		printf("//%s//\n", str);
		token = rem_quotes(token);
		printf("after quotes 1: %s\n", token);
		str = rem_quotes(str);
		printf("after quotes 2: %s\n", str);
		token = ft_strjoin(token, str);
		i = ft_strlen(token) - 1;
		if (!(token[i] == '\"'))
			quote_at_end = 0;
		j++;
	}
	return (token);
}

int	quotes_skip(char **tab)
{
	int	count;

	count = 0;
	while (tab[0][ft_strlen(tab[0]) - 1] == '"' \
		&& tab[1] && tab[1][0] == '"')
	{
		count++;
		tab++;
	}
	return (count);
}

char	**parsing(char **lex_tab, char **env_vars)
{
	t_cmd	parse_list;
	t_cmd	*temp;
	int		i;
	int		type;
	char	*str;

	i = -1;
	str = NULL;
	parse_list.next = NULL;
	parse_list.args = NULL;
	temp = &parse_list;
	while (lex_tab[++i])
	{
		type = token_type(lex_tab[i]);
		if (type < 4)
			i += redir(&parse_list, lex_tab + i, type);
		else if (type == 4)
			lst_next_cmd(temp); // handle pipes here
		else
		{
			if (type == 6)
				env_vars = create_env_vars(lex_tab[i], env_vars);
			if (type < 7)
			{
				//i += quotes_skip(lex_tab + i);
				str = fuse_quotes(get_env_vars(lex_tab[i], env_vars), lex_tab + i + 1, env_vars);
				temp->args = token_join(temp->args, str);
				i += quotes_skip(lex_tab + i);
			}
			else
				temp->args = token_join(temp->args, lex_tab[i]);
		}
	}

	i = -1;
	while (temp->args && temp->args[++i])
		printf("%s\n", temp->args[i]);
	i = -1;
	while (env_vars && env_vars[++i])
		printf("env_var %d : %s\n", i, env_vars[i]);
	return(env_vars);	
}