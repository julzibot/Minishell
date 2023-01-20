/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 18:54:53 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/20 15:01:08 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

void	ft_child_termios(struct termios *original_termios, struct termios *modified_termios)
{
	tcgetattr(STDIN_FILENO, original_termios);
	modified_termios = original_termios;
	modified_termios->c_cc[VEOF] = -1;
	//modified_termios.c_lflag &= ~ICANON; // Disable canonical mode
	tcsetattr(STDIN_FILENO, TCSANOW, modified_termios);
}

void	ft_close_fds(t_cmd *cmd)
{
	if (cmd->redir[0] == 1)
		close (cmd->redir_in);
	if (cmd->redir[1] == 1)
		close(cmd->outfile);
	if (cmd->in_pipe[0] != -1)
		close (cmd->in_pipe[0]);
	if (cmd->in_pipe[1] != -1)
		close (cmd->out_pipe[1]);
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

char	*ft_cmd_check(char **envp, char *cmd)
{
	char	*env_line;
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	if (access(cmd, F_OK | X_OK) == 0)
			return (cmd);
	while (envp[i])
	{
		if (ft_strstr(envp[i], "PATH"))
			env_line = ft_substr(envp[i], 5);
		i++;
	}
	paths = ft_split(env_line, ':');
	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/", 1);
		paths[i] = ft_strjoin(paths[i], cmd, 1);
		// printf("HELLO %d\n", access(paths[i], F_OK | X_OK));
		if (access(paths[i], F_OK | X_OK) == 0)
		{
			path = ft_strdup(paths[i]);
			ft_free_char_array(paths);
			return (path);
		}
		else
			i++;
	}
	return (NULL);
}

void		ft_exec(t_cmd *cmd, char **envp) // Execute a command
{
	char	*path;

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
	// for (int fd = 0; fd < 30; fd++) {
	// 		int flags = fcntl(fd, F_GETFD);
	// 		if (flags != -1) {
	// 			printf("in exec : fd %d is open\n", fd);
	// 		}
	// 	}
	// printf ("---\n");
	path = ft_cmd_check(envp, cmd->args[0]);
	execve(path, cmd->args, envp);
	//exit(0);
	return ;
}

int	exec_builtin(t_cmd *cmd, int builtin)
{
	int	status;

	status = 0;
	if (builtin == 1)
		status = ft_cd(cmd);
	else if (builtin == 2)
		ft_print_env(env.env_list, cmd);
	if (builtin == 3)
		ft_echo(cmd);
	else if (builtin == 4)
		ft_pwd(cmd);
	else if (builtin == 5)
		ft_unset(cmd);
	else if (builtin == 6)
	{
		if (cmd->args[1] == NULL)
			ft_print_env(env.exp_list, cmd);
		else
			ft_export(cmd);
	}
	else if (builtin == 7)
		ft_exit(cmd);
	return (status);
}

int	ft_exec_cmd(t_cmd *cmd, char **envp)
{
	struct termios original_termios;
    struct termios modified_termios;
	int	status;

	status = 0;
	if (cmd == NULL || cmd->args == NULL || ft_verify_equal(cmd->args[0]))
	{
		if (cmd->redir[0] == 1)
			close (cmd->redir_in);
		if (cmd->redir[1] == 1)
			close(cmd->outfile);
		return (status);
	}
	if (is_builtin(cmd) != 0)
		{
			status = exec_builtin(cmd, is_builtin(cmd));
			ft_close_fds(cmd);
			return (status);
		}
	if (ft_cmd_check(envp, cmd->args[0]) == NULL && is_builtin(cmd) == 0)
	{
		//env.error_code = 127;
		status = 127;
		ft_get_err_code(status);
		return (status);
	}
	cmd->shell_pid = fork();
	if (cmd->shell_pid == 0)
	{
		ft_child_termios(&original_termios, &modified_termios);
		ft_child_sig();
		ft_exec(cmd, envp); // execve
		tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
		// exit(0);
	}
	else
		ft_close_fds(cmd);
	return (status);
}
