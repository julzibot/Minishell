/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/19 11:46:07 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gl_env	env;

int	ft_exec_parent(t_cmd *cmd)
{
	if (!cmd->args)
		return (0);
	//printf("EXIT %s\n", cmd->args[0]);
	if (ft_strncmp(cmd->args[0], "exit", 4) == 0 &&
			ft_strlen(cmd->args[0]) == 4)
		ft_exit(cmd);
	else if (ft_strncmp(cmd->args[0], "env", 3) == 0 &&
			ft_strlen(cmd->args[0]) == 3)
	{
		printf("is env\n");
		ft_print_env(env.env_list);
		return (1);
	}
	else if (ft_strncmp(cmd->args[0], "unset", 5) == 0 &&
			ft_strlen(cmd->args[0]) == 5)
	{
		ft_unset(cmd);
		return (1);
	}
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
	}
	else if (ft_strncmp(cmd->args[0], "export", 6) == 0 &&
			ft_strlen(cmd->args[0]) == 6)
	{
		if (cmd->args[1] == NULL)
			ft_print_env(env.exp_list);
		else
			ft_export(cmd);
		return (1);
	}
	return (0);
}

void	exec_pipeline(t_cmd *parse_list, char **envp)
{
	t_cmd	*temp;
	int	len;
	int	i;

	len = cmd_lstsize(parse_list);
	i = 0;
	temp = parse_list;
	while (i++ < len)
	{
		// close (temp->in_pipe[0]);
		if (temp->piped && pipe(temp->out_pipe) == -1)
			return ;
		if (temp->piped && temp->next)
		{
			temp->next->in_pipe[0] = temp->out_pipe[0];
			temp->next->in_pipe[1] = temp->out_pipe[1];
		}
		if (ft_exec_parent(temp))
			return ;
		ft_exec_cmd(temp, envp);
		if (temp->next)
			temp = temp->next;
		// for (int fd = 0; fd < 30; fd++) {
		// 	int flags = fcntl(fd, F_GETFD);
		// 	if (flags != -1) {
		// 		printf("after closes : fd %d is open\n", fd);
		// 	}
		// }
		// printf ("---\n");
	}
	i = 0;
	while (i++ < len)
	{
		waitpid(-1, NULL, 0);
	}
}

void	free_list(t_cmd *parse_list)
{
	t_cmd	*temp;

	while (parse_list && parse_list->next)
	{
		temp = parse_list->next;
		free(parse_list);
		parse_list = temp;
	}
	free(parse_list);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char			*line;
	char			**tokens;
	char			**env_vars;
	t_cmd 			*parse_list;
	struct termios			*term;

	term = NULL;
	env_vars = NULL;
	ft_init_termios(term);
	ft_init_env(envp);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, ft_handle_sigint);
		parse_list = malloc(sizeof(t_cmd));
		if (!parse_list)
			exit(1);
		parse_init(parse_list, envp, env_vars);
		// line = ft_strdup("echo hello world");
		line = readline(PROMPT);
		check_line_exists(line);
		// add_history(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		env_vars = parse_list->env_vars;
		exec_pipeline(parse_list, envp);
		free_list(parse_list);
		free(line);
		//system("leaks minishell\n");
	}
	return (0);
}
