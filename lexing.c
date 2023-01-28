/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 20:02:01 by jibot             #+#    #+#             */
/*   Updated: 2023/01/25 20:02:06 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	g_env;

int	syntax_err(int err_type, t_seg *s)
{
	int	error;

	error = 0;
	if (err_type)
	{
		if (err_type == 2)
			printf("Error : this pipe is CHELOU !\n");
		else if (err_type == 3)
			printf("Error : this redirection is chelou !\n");
		else if (err_type == 4)
			printf("Error : chelou input detected\n");
		else if (err_type == 5)
			printf("Error : unclosed quotes !\n");
		error = 1;
	}
	if (error && err_type != 1)
		g_env.error_code = 258;
	free(s);
	return (error);
}

void	adjust_vars(t_seg *s, char *line, int loop)
{
	if (!loop)
	{
		s->j = -1;
		s->quoted = 0;
		s->q_type = 0;
	}
	else
	{
		if (!s->quoted)
		{
			s->quoted = 1;
			s->q_type = s->j;
		}
		else if (s->quoted && line[s->j] == line[s->q_type])
			s->quoted = 0;
	}
}

int	prelex_err(char *line)
{
	t_seg	*s;

	s = malloc(sizeof(t_seg));
	adjust_vars(s, line, 0);
	if (!line[0])
		return (syntax_err(1, s));
	if (line[0] == '|' || (ft_strlen(line) > 1 \
			&& line[ft_strlen(line) - 1] == '|'))
		return (syntax_err(2, s));
	while (line[++s->j])
	{
		if (is_delim(line[s->j]) == 1)
			adjust_vars(s, line, 1);
		else if (!s->quoted && is_delim(line[s->j]) == 3 \
				&& ((is_delim(line[s->j + 1]) == 3 \
				&& line[s->j + 1] != line[s->j]) \
				|| (line[s->j] == line[s->j + 1] \
				&& is_delim(line[s->j + 2]) == 3)))
			return (syntax_err(3, s));
		else if (!s->quoted && (line[s->j] == '\\' || line[s->j] == ';'))
			return (syntax_err(4, s));
	}
	if (s->quoted)
		return (syntax_err(5, s));
	return (syntax_err(0, s));
}

void	get_token(t_lex *l, char *line, t_cmd *parse_list)
{
	l->delim = is_delim(line[l->i]);
	if (!l->delim || l->delim == 1)
	{
		parse_list->quoted[l->j] = 0;
		if (l->delim == 1)
			parse_list->quoted[l->j] = 1;
		l->i = lineseg(line, l->i, l->lex_tab + l->j, parse_list->quoted[l->j]);
	}
	else if (l->delim == 2 || l->delim == 3)
		l->i += lex_pipe_redir(line + l->i, l->lex_tab + l->j++);
	if (is_delim(line[l->i]) != 4)
		l->i--;
	else
	{
		parse_list->space_after[l->j] = 1;
		while (is_delim(line[l->i + 1]) == 4)
			l->i++;
	}
	if (!l->delim || l->delim == 1)
		l->j++;
	return ;
}

char	**lexing(char *line, t_cmd *parse_list)
{
	t_lex	*l;
	char	**ret;
	int		m_size;	

	if (!line || prelex_err(line) || only_spaces(line))
		return (NULL);
	l = malloc(sizeof(t_lex));
	l->i = -1;
	l->j = 0;
	l->error = 0;
	l->delim = -1;
	m_size = arg_count(line);
	if (m_size == 1)
		m_size ++;
	printf("lex malloc : %d\n", arg_count(line));
	l->lex_tab = malloc(sizeof(char *) * m_size);
	tab_list_init(line, parse_list);
	while (line[++l->i])
		get_token(l, line, parse_list);
	l->lex_tab[l->j] = NULL;
	if (end_lex_pipetest(l))
		return (NULL);
	ret = ft_tabdup(l->lex_tab, 1);
	free(l);
	return (ret);
}
