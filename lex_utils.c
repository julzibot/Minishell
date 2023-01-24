#include "minishell.h"

int	count_loop(char *line, int i, int quoted, char q_type)
{
	t_seg	cl;

	cl.count = 0;
	cl.var_q_type = i;
	cl.var_quoted = 0;
	cl.is_var = 0;
	while (line[++i] && ((quoted && line[i] != q_type) \
		|| (!quoted && !is_delim(line[i])) \
		|| (!quoted && cl.is_var && (is_delim(line[i]) != 4 || cl.var_quoted))))
	{
		if (line[i] == '=' && !quoted && !cl.is_var)
			cl.is_var = 1;
		else if (cl.is_var && !cl.var_quoted && is_delim(line[i]) == 1)
		{
			cl.var_quoted = 1;
			cl.var_q_type = i;
		}
		else if (cl.is_var && cl.var_quoted && line[i] == line[cl.var_q_type])
			cl.var_quoted = 0;
		cl.count++;
	}
	if (!quoted && is_delim(line[i]) == 1)
		cl.count++;
	return (cl.count);
}

int	seg_size(char *line, int i)
{
	int	count;
	int	quoted;
	char	q_type;

	quoted = 0;
	count = 1;
	q_type = line[i];
	if (is_delim(line[i]) == 1)
		quoted = 1;
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		count++;
	count += count_loop(line, i, quoted, q_type);
	return (count + quoted);
}

void	segvars_init(t_seg *segvars, int i, int quoted)
{
	segvars->quoted = quoted;
	segvars->is_var = 0;
	segvars->var_quoted = 0;
	segvars->s_i = 0;
	segvars->q_type = i;
	segvars->var_q_type = i;
}

int	seg_loop(char *line, int i, char *seg, t_seg *segvars)
{
	while (line[++i] && ((segvars->quoted && line[i] != line[segvars->q_type]) \
		|| (!segvars->quoted && !is_delim(line[i])) \
		|| (!segvars->quoted && segvars->is_var && (is_delim(line[i]) != 4 || segvars->var_quoted))) \
		&& !(!segvars->quoted && (line[i] == '\\' || line[i] == ';')))
	{
		if (line[i] == '=' && !segvars->quoted && !segvars->is_var)
			segvars->is_var = 1;
		else if (segvars->is_var && !segvars->var_quoted && is_delim(line[i]) == 1)
		{
			segvars->var_quoted = 1;
			segvars->var_q_type = i;
		}
		else if (segvars->is_var && segvars->var_quoted && line[i] == line[segvars->var_q_type])
			segvars->var_quoted = 0;
		seg[segvars->s_i++] = line[i];
	}
	return (i);
}

int	f_return(char *seg, t_seg *sv, int ret)
{
	free(seg);
	free(sv);
	return (ret);
}

int	lineseg(char *line, int i, char **lex_tab, int quoted)
{
	char	*seg;
	t_seg *sv;

	sv = malloc(sizeof(t_seg));
	segvars_init(sv, i, quoted);
	seg = malloc(seg_size(line, i) + 1);
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		seg[sv->s_i++] = line[i - 1];
	if (line[i] == '\\' || line[i] == ';')
		return(f_return(seg, sv, -2));
	seg[sv->s_i++] = line[i];
	i = seg_loop(line, i, seg, sv);
	if ((quoted || sv->var_quoted) && !line[i])
		return(f_return(seg, sv, -1));
	else if (!quoted && (line[i] == '\\' || line[i] == ';'))
		return(f_return(seg, sv, -2));
	seg[sv->s_i] = line[i];
	if (!quoted && is_delim(line[i]) == 1)
		sv->q_type = 1;
	else
		sv->q_type = 0;
	seg[sv->s_i + quoted + sv->q_type] = '\0';
	*lex_tab = ft_strdup(seg);
	return(f_return(seg, sv, i + quoted));
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

void	tab_list_init(char *line, t_cmd *parse_list)
{
	int	i;
	int	lex_size;
	
	i = -1;
	lex_size = arg_count(line);
	if (parse_list)
	{
		parse_list->quoted = malloc(sizeof(int) * (lex_size));
		parse_list->space_after = malloc(sizeof(int) * (lex_size));
		while (++i < lex_size)
		{
			parse_list->quoted[i] = 0;
			parse_list->space_after[i] = 0;
		}
	}
	return;
}