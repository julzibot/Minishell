#include "minishell.h"

int	seg_size(char *line, int i)
{
	int	count;
	int	quoted;
	char	q_type;

	quoted = 0;
	count = 1;
	if (is_delim(line[i]) == 1)
	{
		quoted = 1;
		q_type = line[i];
	}
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		count++;
	while (line[++i] && ((quoted && line[i] != q_type) 
		|| (!quoted && !is_delim(line[i]))))
		count++;
	if (!quoted && is_delim(line[i]) == 1)
		count++;
	return (count + quoted + 1);
}

int	lineseg(char *line, int i, char **lex_tab, int quoted)
{
	int	s_i;
	char	*seg;
	int	q_type;

	s_i = 0;
	q_type = i;
	seg = malloc(seg_size(line, i));
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		seg[s_i++] = line[i - 1];
	seg[s_i++] = line[i];
	while (line[++i] && ((quoted && line[i] != line[q_type]) 
		|| (!quoted && !is_delim(line[i]))))
		seg[s_i++] = line[i];
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
	str[count] = c[0];
	str[1 + count] = '\0';
	*lex_tab = str;
	return (count);
}

int	arg_count(char *line)
{
	int	count;
	int	i;

	i = -1;
	count = 1;
	while (line && line[++i])
	{
		if (is_delim(line[i]) == 3 && line[0] == line[1])
			i += 2;
		else if (is_delim(line[i]) == 2 || is_delim(line[i]) == 3)
			i++;
		else if (is_delim(line[i]) == 1 || !is_delim(line[i]))
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
	
	i = -1;
	if (lex_tab == NULL || parse_list == NULL || line == NULL)
		return ;
	lex_tab = malloc(sizeof(char*) * (arg_count(line) + 1));
	parse_list->quoted = malloc(sizeof(int) * arg_count(line) + 1);
	parse_list->space_after = malloc(sizeof(int) * arg_count(line) + 1);
	while (++i < arg_count(line) + 1)
	{
		parse_list->quoted[i] = 0;
		parse_list->space_after[i] = 0;
	}
	lex_tab[arg_count(line) + 1] = NULL;
}