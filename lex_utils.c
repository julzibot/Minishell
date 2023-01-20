#include "minishell.h"

int	seg_size(char *line, int i)
{
	int	count;
	int	quoted;
	int is_var;
	int	var_quoted;
	char	q_type;
	int	var_q_type;

	quoted = 0;
	count = 1;
	is_var = 0;
	var_quoted = 0;
	if (is_delim(line[i]) == 1)
	{
		quoted = 1;
		q_type = line[i];
	}
	var_q_type = i;
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		count++;
	while (line[++i] && ((quoted && line[i] != q_type) \
		|| (!quoted && !is_delim(line[i])) \
		|| (!quoted && is_var && (is_delim(line[i]) != 4 || var_quoted))))
	{
		if (line[i] == '=' && !quoted && !is_var)
			is_var = 1;
		else if (is_var && !var_quoted && is_delim(line[i]) == 1)
		{
			var_quoted = 1;
			var_q_type = i;
		}
		else if (is_var && var_quoted && line[i] == line[var_q_type])
			var_quoted = 0;
		count++;
	}
	if (!quoted && is_delim(line[i]) == 1)
		count++;
	return (count + quoted);
}

int	lineseg(char *line, int i, char **lex_tab, int quoted)
{
	int	s_i;
	char	*seg;
	int	q_type;
	int var_q_type;
	int	is_var;
	int	var_quoted;

	is_var = 0;
	var_quoted = 0;
	s_i = 0;
	q_type = i;
	var_q_type = i;
	seg = malloc(seg_size(line, i) + 1);
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		seg[s_i++] = line[i - 1];
	if (line[i] == '\\' || line[i] == ';')
		return (-2);
	seg[s_i++] = line[i];
	while (line[++i] && ((quoted && line[i] != line[q_type]) \
		|| (!quoted && !is_delim(line[i])) \
		|| (!quoted && is_var && (is_delim(line[i]) != 4 || var_quoted))) \
		&& !(!quoted && (line[i] == '\\' || line[i] == ';')))
	{
		if (line[i] == '=' && !quoted && !is_var)
			is_var = 1;
		else if (is_var && !var_quoted && is_delim(line[i]) == 1)
		{
			var_quoted = 1;
			var_q_type = i;
		}
		else if (is_var && var_quoted && line[i] == line[var_q_type])
			var_quoted = 0;
		seg[s_i++] = line[i];
	}
	if ((quoted || var_quoted) && !line[i])
		return (-1);
	else if (!quoted && (line[i] == '\\' || line[i] == ';'))
		return (-2);
	seg[s_i] = line[i];
	if (!quoted && is_delim(line[i]) == 1)
		q_type = 1;
	else
		q_type = 0;
	seg[s_i + quoted + q_type] = '\0';
	*lex_tab = seg;
	return (i + quoted);
}
 
int	lex_pipe_redir(char *c, char **lex_tab)
{
	char	*str;
	int		count;

	count = 0;
	if (is_delim(c[0]) == 3 && c[0] == c[1])
		count = 1;
	str = malloc(2 + count);
	str[0] = c[0];
	if (count)
		str[count] = c[0];
	str[1 + count] = '\0';
	*lex_tab = str;
	return (1 + count);
}

int	arg_count(char *line)
{
	int	count;
	int	i;
	int	delim;

	i = -1;
	count = 1;
	while (line && line[++i])
	{
		delim = is_delim(line[i]);
		if (delim == 2 || delim == 3)
		{
			if (delim == 3 && line[0] == line[1])
				i++;
		}
		else if (delim == 1 || !delim)
		{
			i += seg_size(line, i);
			if (is_delim(line[i]) != 4)
				i--;
		}
		else
			count--;
		count++;
	}
	return (count);
}

void	tab_list_init(char *line, char **lex_tab, t_cmd *parse_list)
{
	int	i;
	int	lex_size;
	
	i = -1;
	lex_size = arg_count(line);
	if (lex_tab == NULL || parse_list == NULL || line == NULL)
		return ;
	parse_list->quoted = malloc(sizeof(int) * (lex_size + 1));
	parse_list->space_after = malloc(sizeof(int) * (lex_size + 1));
	while (++i < lex_size)
	{
		parse_list->quoted[i] = 0;
		parse_list->space_after[i] = 0;
	}
	lex_tab[lex_size - 1] = NULL;
	return;
}