/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:58:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/14 15:00:19 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

// void	ft_free_list(t_env **list)
// {
// 	t_env	*curr;
// 	t_env	*prev;

// 	curr = *list;
// 	prev = NULL;
// 	while (curr)
// 	{
// 		if (prev != NULL)
// 			free(prev);
// 		prev = curr;
// 		curr = curr->next;
// 	}
// }

// void	ft_free_env(t_cmd *cmd)
// {
// 	ft_free_list(&env.env_list);
// 	ft_free_list(&env.exp_list);
// }

void	ft_exit(t_cmd *cmd)
{
	//ft_free_env(cmd);
	free(cmd->env_vars);
	exit(0);
}