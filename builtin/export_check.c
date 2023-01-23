/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 17:42:34 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/23 22:30:00 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_verify_alphanum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '=' || c == '_')
		return (0);
	return (1);
}

int	ft_verify_err_var(char *line)
{
	int	i;

	i = 0;
	if (!(line[0] >= 'a' && line[0] <= 'z')
		&& !(line[0] >= 'A' && line[0] <= 'Z'))
		return (1);
	i++;
	if (line[i] == '\0')
		return (0);
	while (line[i])
	{
		if (ft_verify_alphanum(line[i]))
			return (1);
		i++;
	}
	return (0);
}

int	ft_verify_double(t_env *env_list, char *line)
{
	t_env	*curr;

	curr = env_list;
	while (curr)
	{
		if (ft_strcmp(curr->line, line) == 0)
			return (1);
		curr = curr->next;
	}
	curr = env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(curr->line)) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	ft_simple_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

int	ft_verify_equal(char *s)
{
	int	i;

	if (!s)
		return (0);
	if (s && s[0] && '0' <= s[0] && s[0] <= '9')
		return (0);
	i = -1;
	while (s[++i])
	{
		if (!ft_is_alnum(s[i]) && !((s[i] == '=') || \
			(s[i] == '+' && s[i + 1] == '=')))
			return (0);
		else if (s[i] == '=')
			return (1);
	}
	return (0);
}
