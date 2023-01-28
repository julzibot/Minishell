/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:42:34 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/27 18:26:03 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

int	ft_verify_alphanum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '=' || c == '_')
		return (0);
	return (1);
}

int	ft_var_check(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '=' || c == '_' || c == '.'
		|| c == '\'' || c == '\"' || c == ' ' || c == '/' || c == ':')
		return (0);
	return (1);
}

int	ft_first_char_check(char c)
{
	if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_verify_err_var(char *line)
{
	int	i;

	i = 0;
	if (ft_first_char_check(line[0]))
		return (1);
	i++;
	if (line[i] == '\0')
		return (0);
	while (line[i])
	{
		if (line[i] == '=')
			break ;
		if (ft_verify_alphanum(line[i]))
			return (1);
		i++;
	}
	while (line[i])
	{
		if ((line[i] == ' ' && line[i - 1] == '='))
			i++;
		if (ft_var_check(line[i]))
			return (1);
		i++;
	}
	return (0);
}

int	ft_find_double(t_env *list, char *line)
{
	t_env	*curr;

	curr = list;
	while (curr)
	{
		if (ft_strcmp(curr->line, line) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}
