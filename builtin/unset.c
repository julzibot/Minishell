/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 17:45:56 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/06 15:42:44 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_remove_line(t_env **env_list, t_env *node)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_list;
	while (curr)
	{
		if (strcmp(curr->line, node->line) == 0) // Add FT_STRCMP
		{
			prev->next = curr->next;
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset_variable(t_env **list, char *s)
{
	t_env	*curr;
	int		n;

	n = ft_varlen(s);
	curr = *list;
	while (curr)
	{
		if (ft_strncmp(curr->line, s, n) == 0)
		{
			ft_remove_line(list, curr);
			return (0);
		}
		curr = curr->next;
	}
	if (curr == NULL)
	{
		ft_printf(2, "%s: not a valid identifier\n", s);
		return (1);
	}
	return (0);
}

// Unsets ENV variable
void	ft_unset(t_env **env_list, t_env **exp_list, char *s)
{
	if (ft_unset_variable(env_list, s) == 1)
		return ;
	if (ft_unset_variable(exp_list, ft_strjoin("declare -x ", s)) == 1)
		return ;
}