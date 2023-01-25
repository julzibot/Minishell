/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 18:54:53 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 12:22:48 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	g_env;

// void	ft_child_termios(struct termios *original_termios,
// struct termios *modified_termios)
// {
// 	// tcgetattr(STDIN_FILENO, original_termios);
// 	// modified_termios = original_termios;
// 	// modified_termios->c_cc[VEOF] = -1;
// 	// //modified_termios.c_lflag &= ~ICANON; // Disable canonical mode
// 	// tcsetattr(STDIN_FILENO, TCSANOW, modified_termios);
// }

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


int	ft_exec(t_cmd *cmd, char **envp, char **tokens)
{
	char	*path;
	int		status;

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
	if (is_builtin(cmd))
	{
		cmd->outfile = STDOUT_FILENO;
		cmd->out_pipe[1] = STDOUT_FILENO;
		status = exec_builtin(cmd, is_builtin(cmd), tokens);
		exit(status);
		return (status);
	}
	path = ft_cmd_check(envp, cmd->args[0], 1);
	execve(path, cmd->args, envp);
	return (1);
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

int	ft_fork(t_cmd *cmd, char **envp, char **tokens)
{
	// struct termios original_termios;
    // struct termios modified_termios;

	//ft_child_termios(&original_termios, &modified_termios);
	ft_child_sig();
	ft_exec(cmd, envp, tokens); // execve
	//tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
	signal(SIGINT, SIG_IGN);
	return (0);
}

int	ft_close_cmd_redir(t_cmd *cmd)
{
	if (cmd->redir[0] == 1)
		close (cmd->redir_in);
	if (cmd->redir[1] == 1)
		close(cmd->outfile);
	return (1);
}

int	ft_do_builtin(t_cmd *cmd, int builtin, char **tokens)
{
	int	status;

	status = 0;
	status = exec_builtin(cmd, builtin, tokens);
	ft_close_fds(cmd);
	return (status);
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
	if (is_builtin(cmd) != 0 && !cmd_i)
		return (ft_do_builtin(cmd, is_builtin(cmd), tokens));
	if (ft_cmd_check(envp, cmd->args[0], 0) == NULL && is_builtin(cmd) == 0)
		return (ft_not_cmd(cmd));
	cmd->shell_pid = fork();
	if (cmd->shell_pid == 0)
		ft_fork(cmd, envp, tokens);
	else if (cmd->shell_pid == -1)
	{
		ft_close_fds(cmd);
		ft_print_error(3, NULL, NULL);
		return (35);
	}
	else
	{
		ft_close_fds(cmd);
		signal(SIGINT, SIG_IGN);
	}
	signal(SIGINT, SIG_IGN);
	return (status);
}
