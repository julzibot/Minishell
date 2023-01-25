/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:39:23 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 09:38:31 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	g_env;

char	*ft_var_content(char *line)
{
	t_env	*curr;

	curr = g_env.env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(line)) == 0)
			return (line);
		curr = curr->next;
	}
	return (NULL);
}

int	ft_update_var(t_env **env_list, char *s)
{
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, s, ft_varlen(s)) == 0)
		{
			free(curr->line);
			curr->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
			ft_strcpy(curr->line, s);
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

void	ft_loop_assign_vars(char **env_vars, char *line)
{
	int		i;
	char	*exp;

	i = 0;
	exp = NULL;
	exp = ft_add_quotes(line);
	exp = ft_strjoin("declare -x ", exp, 2);
	while (env_vars[i])
	{
		if (ft_strcmp(env_vars[i], line) == 0)
		{
			ft_add_after(&g_env.env_list, 15, line);
			ft_add_queue(&g_env.exp_list, exp);
			free(exp);
			return ;
		}
		i++;
	}
	free(exp);
}
