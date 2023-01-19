/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 17:45:56 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/19 16:34:36 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

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
			//free(curr);
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
		if (curr == NULL)
		{
			ft_printf(2, "%s: not a valid identifier\n", s);
			return (1);
		}
		if (ft_strncmp(curr->line, s, n) == 0)
		{
			ft_remove_line(list, curr);
			return (0);
		}
		curr = curr->next;
	}
	return (0);
}

// Unsets ENV variable
void	ft_unset(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
			if (ft_unset_variable(&env.env_list, cmd->args[i]) == 1)
				return ;
			if (ft_unset_variable(&env.exp_list, ft_strjoin("declare -x ", cmd->args[i], -1)) == 1)
				return ;
		i++;
	}
}
