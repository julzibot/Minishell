/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 18:54:53 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 18:30:40 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

void	ft_pipe_redir(t_cmd *cmd)
{
	close(cmd->out_pipe[0]);
	close(cmd->in_pipe[1]);
	if (!cmd->redir[0] && cmd->in_pipe[0] != -1)
		dup2(cmd->in_pipe[0], STDIN_FILENO);
	else if (cmd->redir[0] && cmd->redir_in != STDIN_FILENO)
	{
		dup2(cmd->redir_in, STDIN_FILENO);
		close(cmd->redir_in);
	}
	if (cmd->outfile == STDOUT_FILENO && cmd->next)
		dup2(cmd->out_pipe[1], STDOUT_FILENO);
	else if (cmd->outfile != STDOUT_FILENO)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
	close(cmd->out_pipe[1]);
	close(cmd->in_pipe[0]);
}

int	ft_exec(t_cmd *cmd, char **envp, char **tokens)
{
	char	*path;
	int		status;

	status = 0;
	ft_pipe_redir(cmd);
	if (is_builtin(cmd))
	{
		cmd->outfile = STDOUT_FILENO;
		cmd->out_pipe[1] = STDOUT_FILENO;
		status = exec_builtin(cmd, is_builtin(cmd), tokens);
		exit(status);
	}
	path = ft_cmd_check(cmd, cmd->args[0], 1);
	if (path == NULL)
		exit(127);
	if (access(path, X_OK) < 0)
	{
		g_env.error_code = 126;
		ft_print_error(126, cmd, NULL);
		exit(126);
	}
	status = execve(path, cmd->args, envp);
	exit(1);
}

int	ft_close_cmd_redir(t_cmd *cmd)
{
	if (cmd->redir[0] == 1)
		close (cmd->redir_in);
	if (cmd->redir[1] == 1)
		close(cmd->outfile);
	return (1);
}

int	ft_not_cmd(t_cmd *cmd)
{
	ft_print_error(NOT_CMD, cmd, NULL);
	g_env.error_code = 127;
	return (NOT_CMD);
}

int	ft_exec_cmd(t_cmd *cmd, char **envp, char **tokens, int cmd_i)
{
	int	status;

	status = 0;
	if (cmd == NULL || cmd->args == NULL || ft_verify_equal(cmd->args[0]))
		return (ft_close_cmd_redir(cmd));
	if (is_builtin(cmd) != 0 && !cmd_i && !cmd->piped)
		return (ft_do_builtin(cmd, is_builtin(cmd), tokens));
	cmd->shell_pid = fork();
	ft_child_sig();
	if (cmd->shell_pid == 0)
		ft_exec(cmd, envp, tokens);
	else if (cmd->shell_pid == -1)
	{
		ft_close_fds(cmd);
		ft_print_error(3, NULL, NULL);
		return (35);
	}
	else
	{
		signal(SIGINT, &ft_display_nl);
		signal(SIGQUIT, SIG_IGN);
		ft_close_fds(cmd);
	}
	return (status);
}
