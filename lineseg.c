/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lineseg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:29:44 by jibot             #+#    #+#             */
/*   Updated: 2023/01/26 12:29:53 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	segvars_init(t_seg *segvars, int i, int quoted, int option)
{
	segvars->is_var = 0;
	segvars->var_quoted = 0;
	segvars->var_q_type = i;
	if (!option)
	{
		segvars->quoted = quoted;
		segvars->s_i = 0;
		segvars->q_type = i;
	}
	else
		segvars->count = 0;
}

int	count_loop(char *line, int i, int quoted, char q_type)
{
	t_seg	*cl;

	cl = malloc(sizeof(t_seg));
	segvars_init(cl, i, 0, 1);
	while (line[++i] && ((quoted && line[i] != q_type) \
		|| (!quoted && !is_delim(line[i])) \
		|| (!quoted && cl->is_var && (is_delim(line[i]) != 4 \
		|| cl->var_quoted))))
	{
		if (line[i] == '=' && !quoted && !cl->is_var)
			cl->is_var = 1;
		else if (cl->is_var && !cl->var_quoted && is_delim(line[i]) == 1)
		{
			cl->var_quoted = 1;
			cl->var_q_type = i;
		}
		else if (cl->is_var && cl->var_quoted \
				&& line[i] == line[cl->var_q_type])
			cl->var_quoted = 0;
		cl->count++;
	}
	if (!quoted && is_delim(line[i]) == 1)
		cl->count++;
	return (f_return (NULL, cl, cl->count));
}

int	seg_size(char *line, int i)
{
	int		size;
	int		quoted;
	char	q_type;

	quoted = 0;
	size = 1;
	q_type = line[i];
	if (is_delim(line[i]) == 1)
		quoted = 1;
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		size++;
	size += count_loop(line, i, quoted, q_type);
	return (size + quoted);
}

int	seg_loop(char *line, int i, char *seg, t_seg *sv)
{
	while (line[++i] && ((sv->quoted && line[i] != line[sv->q_type]) \
		|| (!sv->quoted && !is_delim(line[i])) \
		|| (!sv->quoted && sv->is_var \
		&& (is_delim(line[i]) != 4 || sv->var_quoted))) \
		&& !(!sv->quoted && (line[i] == '\\' || line[i] == ';')))
	{
		if (line[i] == '=' && !sv->quoted && !sv->is_var)
			sv->is_var = 1;
		else if (sv->is_var && !sv->var_quoted && is_delim(line[i]) == 1)
		{
			sv->var_quoted = 1;
			sv->var_q_type = i;
		}
		else if (sv->is_var && sv->var_quoted \
				&& line[i] == line[sv->var_q_type])
			sv->var_quoted = 0;
		seg[sv->s_i++] = line[i];
	}
	return (i);
}

int	lineseg(char *line, int i, char **lex_tab, int quoted)
{
	char	*seg;
	t_seg	*sv;

	sv = malloc(sizeof(t_seg));
	segvars_init(sv, i, quoted, 0);
	seg = malloc(seg_size(line, i) + 1);
	if (i > 0 && is_delim(line[i - 1]) == 1 && !(line[i - 1] == line[i]))
		seg[sv->s_i++] = line[i - 1];
	seg[sv->s_i++] = line[i];
	i = seg_loop(line, i, seg, sv);
	seg[sv->s_i] = line[i];
	if (!quoted && is_delim(line[i]) == 1)
		sv->q_type = 1;
	else
		sv->q_type = 0;
	seg[sv->s_i + quoted + sv->q_type] = '\0';
	*lex_tab = ft_strdup(seg);
	return (f_return(seg, sv, i + quoted));
}
