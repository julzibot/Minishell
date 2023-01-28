/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 12:06:51 by jibot             #+#    #+#             */
/*   Updated: 2023/01/26 12:06:55 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	end_lex_pipetest(t_lex *l)
{
	char	*ptest;

	ptest = NULL;
	if (l->lex_tab)
		ptest = l->lex_tab[ft_tablen(l->lex_tab) - 1];
	if ((l->lex_tab && l->lex_tab[0] && l->lex_tab[0][0] == '|') \
		|| ptest[ft_strlen(ptest) - 1] == '|')
	{
		ft_free_char_array(l->lex_tab);
		free(l);
		printf("Error : this pipe is CHELOU !\n");
		return (1);
	}
	return (0);
}

int	f_return(char *seg, t_seg *sv, int ret)
{
	if (seg)
		free(seg);
	if (sv)
		free(sv);
	return (ret);
}

int	lex_pipe_redir(char *c, char **pr_token)
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
	*pr_token = str;
	return (1 + count);
}

int	arg_count(char *line)
{
	int	count;
	int	i;
	int	q;
	int	delim;

	i = -1;
	count = 1;
	while (line && line[++i] && count++)
	{
		delim = is_delim(line[i]);
		if (delim == 3 && line[i + 1] && (line[i] == line[i + 1]))
			i++;
		else if (delim == 1)
		{
			q = i;
			while (line[++i] && line[i] != line[q])
				;
		}
		else if (!delim)
			while (line[i] && line[i + 1] && !is_delim(line[i + 1]))
				i++;
		else if (delim == 4)
			count--;
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
	return ;
}
