/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 18:54:53 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/07 17:07:06 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern pid_t	g_pid;

char	*ft_cmd_check(char **envp, char *cmd)
{
	char	*env_line;
	char	**paths;
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
		paths[i] = ft_strjoin(paths[i], "/");
		paths[i] = ft_strjoin(paths[i], cmd);
		if (access(paths[i], F_OK | X_OK) == 0)
			return (paths[i]);
		i++;
	}
	perror("Command not found"); // Align with bash output
	exit(127);
}

void	ft_handle_sigquit(int sig)
{
	if (sig == SIGQUIT)
		printf("Quit: 3\n");
}

int		ft_exec(t_cmd *cmd, char **env) // Execute a command
{
	char	*path;
	if (!cmd->redir && cmd->infile != STDIN_FILENO)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	else if (cmd->redir != 0 && cmd->redir_in != STDIN_FILENO)
	{
		dup2(cmd->redir_in, STDIN_FILENO);
		close(cmd->redir_in);
	}
	if (cmd->outfile == STDOUT_FILENO && cmd->next)
	{
		dup2(cmd->out_pipe[1], STDOUT_FILENO);
		close(cmd->out_pipe[1]);
	}
	else if (cmd->outfile != STDOUT_FILENO)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
	close (cmd->in_pipe);
	signal(SIGQUIT, ft_handle_sigquit);
	signal(SIGINT, ft_handle_sigint);
	path = ft_cmd_check(env, cmd->args[0]);
	// ft_printf(1, "\n");
	execve(path, cmd->args, env);
	return (1);
}

void	exec_builtin(t_cmd *cmd, int builtin)
{
	if (builtin == 1)
		ft_cd(&cmd->exp_list, &cmd->env_list, cmd->args[1]);
	else if (builtin == 2)
		ft_print_env(cmd->env_list);
	else if (builtin == 3)
		ft_echo(cmd->args);
	else if (builtin == 4) // FT_STRCMP!
		ft_pwd();
	else if (builtin == 5)
		ft_unset(&cmd->env_list, &cmd->exp_list, cmd->args[1]);
	else if (builtin == 6)
	{
		if (cmd->args[1] == NULL)
			ft_print_env(cmd->exp_list);
		else
			ft_export(cmd);
	}
}

int	is_builtin(t_cmd *cmd)
{
	if (ft_strncmp(cmd->args[0], "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(cmd->args[0], "env", 3) == 0)
		return (2);
	// else if (ft_strncmp(cmd->args[0], "echo", 4) == 0)
	// 	return (3);
	else if (strcmp(cmd->args[0], "pwd") == 0) // FT_STRCMP!
		return (4);
	else if (ft_strncmp(cmd->args[0], "unset", 5) == 0)
		return (5);
	else if (ft_strncmp(cmd->args[0], "export", 6) == 0)
		return (6);
	else
		return (0);
}

void	ft_exec_cmd(t_cmd *cmd, char **env)
{
	(void)env;
	int	builtin;

	if (cmd == NULL || cmd->args == NULL || ft_verify_equal(cmd->args[0]))
		return ;
	cmd->shell_pid = fork();
	if (cmd->shell_pid == 0)
	{
		builtin = is_builtin(cmd);
		if (builtin)
			exec_builtin(cmd, builtin);
		else
			ft_exec(cmd, env); // execve
	}
	else
		waitpid(cmd->shell_pid, NULL, 0);
	return ;
}

// int	main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	char	*line;
// 	t_env 	*env_list;
// 	t_env	*exp_list;
	
// 	env_list = NULL;
// 	exp_list = NULL;
// 	ft_get_env(&env_list, env); // For env command
// 	ft_get_env(&exp_list, env); // For export command
// 	ft_get_export(&exp_list);   // Declare -x PWD="somewhere/nice/and/cozy"
// 	while (1)
// 	{
// 		line = readline("MiniShelly: ");
// 		ft_exec_cmd()
// 	}
// 	return (0);
// }
