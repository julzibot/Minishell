/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 17:37:15 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 18:22:41 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

void	ft_close_fds(t_cmd *cmd)
{
	if (cmd->redir[0] == 1)
		close (cmd->redir_in);
	if (cmd->redir[1] == 1)
		close(cmd->outfile);
	if (cmd->in_pipe[0] != -1)
		close (cmd->in_pipe[0]);
	if (cmd->in_pipe[1] != -1)
		close (cmd->in_pipe[1]);
}

int	is_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (2);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (3);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (4);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (5);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (6);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (7);
	else
		return (0);
}

int	exec_builtin(t_cmd *cmd, int builtin, char **tokens)
{
	int	status;

	status = 0;
	if (builtin == 1)
		status = ft_cd(cmd);
	else if (builtin == 2)
		status = ft_print_env(g_env.env_list, cmd);
	if (builtin == 3)
		ft_echo(cmd);
	else if (builtin == 4)
		ft_pwd(cmd);
	else if (builtin == 5)
		status = ft_unset(cmd);
	else if (builtin == 6)
	{
		if (cmd->args[1] == NULL)
			ft_print_env(g_env.exp_list, cmd);
		else
			status = ft_export(cmd);
	}
	else if (builtin == 7)
		ft_exit(cmd, status, tokens);
	return (status);
}

int	ft_do_builtin(t_cmd *cmd, int builtin, char **tokens)
{
	int	status;

	status = 0;
	status = exec_builtin(cmd, builtin, tokens);
	ft_close_fds(cmd);
	return (status);
}
