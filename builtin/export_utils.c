/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:39:23 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 15:17:05 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

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

int	ft_update_var(t_env **env_list, char *s)
{
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, s, ft_varlen(s)) == 0)
		{
			if (curr->line)
				free(curr->line);
			curr->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
			ft_strcpy(curr->line, s);
			return (1);
		}
		curr = curr->next;
	}
	return (0);
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
		if (!ft_is_alnum(s[i]) && s[i] != '_' \
				&& !((s[i] == '=') || \
				(s[i] == '+' && s[i + 1] == '=')))
			return (0);
		else if (s[i] == '=')
			return (1);
	}
	return (0);
}

int	ft_find_var_change(t_env *list, char *line)
{
	t_env	*curr;

	curr = list;
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(line)) == 0
			&& ft_strncmp(curr->line, line, ft_varlen(curr->line)) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

// res 2 content change
// res 1 double found
int	ft_verify_double(char *line)
{
	char	*exp;
	int		res;

	exp = NULL;
	res = 0;
	exp = ft_add_quotes(line);
	exp = ft_strjoin("declare -x ", exp, 2);
	if (ft_find_var_change(g_env.env_list, line))
		res = 2;
	if (ft_find_double(g_env.exp_list, exp))
		res = 1;
	if (exp)
		free(exp);
	return (res);
}
