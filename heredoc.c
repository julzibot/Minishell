/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 19:57:22 by jibot             #+#    #+#             */
/*   Updated: 2023/01/27 13:31:14 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

void	ft_heredoc_sig(void)
{
	signal(SIGINT, &ft_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_loop(t_cmd *cmd, char *filename_delim)
{
	char	*line;

	line = readline("> ");
	while (ft_strcmp(line, filename_delim) && line)
	{
		ft_printf(cmd->heredoc[1], "%s\n", line);
		free(line);
		line = readline("> ");
	}
	close(cmd->heredoc[1]);
	if (line)
		free(line);
	exit(0);
}

int	heredoc_handle(t_cmd *cmd, char *filename_delim)
{
	if (pipe(cmd->heredoc) == -1)
		return (0);
	cmd->redir_in = cmd->heredoc[0];
	cmd->redir[0] = 1;
	cmd->heredoc_pid = fork();
	if (cmd->heredoc_pid == 0)
	{
		close(cmd->heredoc[0]);
		ft_heredoc_sig();
		heredoc_loop(cmd, filename_delim);
		close(cmd->heredoc[1]);
		exit (0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		close(cmd->heredoc[1]);
		waitpid(-1, NULL, 0);
	}
	return (1);
}
