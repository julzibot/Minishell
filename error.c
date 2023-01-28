/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 10:51:57 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 18:43:08 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	g_env;

void	ft_get_err_code(int error_code)
{
	ft_printf(2, "Mini_chelou: %d: command not found\n", error_code);
	return ;
}

int	ft_print_error(int error_code, t_cmd *cmd, char *line)
{
	ft_printf(2, "Mini_chelou: ");
	if (error_code == 99)
		ft_printf(2, "%s: No such file or directory\n", cmd->args[0]);
	if (error_code == CMD)
		ft_printf(2, "%s: %s: No such file or directory\n",
			cmd->args[0], cmd->args[1]);
	else if (error_code == ENV_VAR)
		ft_printf(2, "%s: `%s': not a valid identifier\n", cmd->args[0], line);
	else if (error_code == 3)
		ft_printf(2, "fork: Resource temporarily unavailable\n");
	else if (error_code == 11)
		ft_printf(2, "Segmentation fault: 11");
	else if (error_code == NOT_EXEC)
		ft_printf(2, "%s: Permission denied\n", cmd->args[0]);
	else if (error_code == NOT_CMD)
	{
		ft_printf(2, "%s: command not found\n", cmd->args[0]);
		g_env.error_code = 0;
	}
	return (1);
}

int	ft_get_exit_code(int status)
{
	int	err;

	err = 0;
	if (status == 1)
		return (status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 3)
			ft_printf(2, "Quit: %d\n", WTERMSIG(status));
		if (WTERMSIG(status) == 13)
			err = 0;
		else
			err = WTERMSIG(status) + 128;
	}
	else if (WIFEXITED(status))
		err = WEXITSTATUS(status) + 128;
	else if (WIFSTOPPED(status))
		err = WSTOPSIG(status) + 128;
	if (err == 138)
		ft_printf(2, "Bus error: 10\n");
	if (err == 139)
		ft_printf(2, "Segmentation fault: 11\n");
	if (err == 128 || err == 254 || err == 255)
		err -= 128;
	return (err);
}
