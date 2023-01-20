/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 10:51:57 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/20 16:18:15 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_get_err_code(int error_code)
{
	ft_printf(2, "Mini_chelou: %d: command not found\n", error_code);
	return ;
}

void	ft_print_error(int error_code, t_cmd *cmd, char *line)
{
	ft_printf(2, "Mini_chelou: ");
	if (error_code == CMD)
		ft_printf(2, "%s: %s: No such file or directory\n", cmd->args[0], cmd->args[1]);
	else if (error_code == ENV_VAR)
		ft_printf(2, "%s: `%s': not a valid identifier\n", cmd->args[0], line);
	else if (error_code == NOT_EXEC)
		ft_printf(2, "%s: Permission denied\n", cmd->args[0]);
	else if (error_code == NOT_CMD)
		ft_printf(2, "%s: command not found\n", cmd->args[0]);
	//else if (error_code == SIG_C)
	return ;
}