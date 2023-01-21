/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 17:45:56 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/21 16:56:34 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	env;

void	ft_remove_line(t_env **env_list, t_env *node)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_list;
	while (curr)
	{
		if (ft_strcmp(curr->line, node->line) == 0)
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

	curr = *list;
	if (curr == NULL)
		return (1);
	while (curr)
	{
		if (ft_strncmp(curr->line, s, ft_varlen(s)) == 0)
		{
			ft_remove_line(list, curr);
			return (0);
		}
		curr = curr->next;
	}
	return (0);
}

// Unsets ENV variable
int	ft_unset(t_cmd *cmd)
{
	int		i;
	int		err;
	char	*exp;

	i = 1;
	err = 0;
	exp = NULL;
	while (cmd->args[i])
	{
		if (ft_verify_err_var(cmd->args[i]))
		{
			err = 1;
			ft_print_error(ENV_VAR, cmd, cmd->args[i]);
			return (err);
		}
		if (ft_unset_variable(&env.env_list, cmd->args[i]) == 1)
			return (1);
		exp = ft_strjoin("declare -x ", cmd->args[i], -1);
		if (ft_unset_variable(&env.exp_list, exp) == 1)
		{
			free(exp);
			return (1);
		}
		i++;
	}
	if (exp)
		free(exp);
	return (err);
}
