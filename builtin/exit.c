/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:58:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 09:30:36 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	g_env;

void	ft_free_char_array(char **s)
{
	int	i;

	i = -1;
	if (!s)
		return ;
	while (s[++i])
		free(s[i]);
	free(s);
}

void	ft_exit(t_cmd *cmd, int error_code, char **tokens)
{
	t_cmd	*temp;

	temp = NULL;
	if (cmd && cmd->args && cmd->args[1] != NULL)
	{
		if (cmd->args[2] != NULL)
		{
			ft_printf(2, "Mini_chelou: ");
			ft_printf(2, "exit: too many arguments\n");
			return ;
		}
		if (ft_is_digit(cmd->args[1]))
		{
			ft_printf(2, "Mini_chelou: ");
			ft_printf(2, "exit: %s: numeric argument required\n", cmd->args[1]);
			return ;
		}
		error_code = ft_atoi(cmd->args[1]);
		error_code %= 256;
	}
	if (cmd->env_vars)
		ft_free_char_array(cmd->env_vars);
	if (cmd->space_after)
		free(cmd->space_after);
	if (cmd->quoted)
		free(cmd->quoted);
	while (cmd)
	{
		if (cmd->args)
			ft_free_char_array(cmd->args);
		temp = cmd->next;
		if (cmd)
			free(cmd);
		cmd = temp;
	}
	ft_free_char_array(tokens);
	system("leaks --list minishell");
	exit(error_code);
}
