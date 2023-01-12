/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/10 18:18:25 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	g_pid;

void	parse_init(t_cmd *parse_list, char **envp)
{
	parse_list->env_vars = NULL;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
	parse_list->env_list = NULL;
	parse_list->exp_list = NULL;
	parse_list->infile = STDIN_FILENO;
	parse_list->outfile = STDOUT_FILENO;
	parse_list->next = NULL;
	parse_list->piped = 0;
	parse_list->redir = 0;
	// parse_list->out_pipe = NULL;
	ft_get_env(&parse_list->env_list, envp); // For env command
	ft_get_env(&parse_list->exp_list, envp); // For export command
	ft_get_export(&parse_list->exp_list);    // Declare -x PWD="somewhere/nice/and/cozy"
}

void	check_line_exists(char *line)
{
	if (line == NULL) // CTRL-D
		{
			rl_redisplay();
			ft_printf(1, "\x1b[A");
			ft_printf(1, "exit\n");
			exit(0);
		}
	else
		add_history(line);
}

void	ft_handle_sigint(int sig)
{
	(void)sig;
	if (g_pid != 0)
	{
		write(1, "\n", 1);
		kill(g_pid, SIGCONT);
	}
	else
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// void	ft_termios(int option)
// {
// 	struct termios	term;

// 	tcgetattr(0, &term);
	
// }

void	exec_pipeline(t_cmd *parse_list, char **envp)
{
	t_cmd	*temp;

	while (parse_list->next != NULL)
	{
		ft_exec_cmd(parse_list, envp);
		if (parse_list->infile != STDIN_FILENO)
			close (parse_list->infile);
		close (parse_list->out_pipe[1]);
		if (parse_list->redir)
			close(parse_list->outfile);
		temp = parse_list;
		parse_list = temp->next;
		free(temp);
	}
	ft_exec_cmd(parse_list, envp);
	if (parse_list->infile != STDIN_FILENO)
		close (parse_list->infile);
	free (parse_list);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char				*line;
	char				**tokens;
	t_cmd 				*parse_list;

	while (1)
	{
		parse_list = malloc(sizeof(t_cmd));
		if (!parse_list)
			exit(1);
		parse_init(parse_list, envp);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, ft_handle_sigint);
		line = readline(PROMPT);
		check_line_exists(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		exec_pipeline(parse_list, envp);
		
		//check fd's after execution
		for (int i = 0; i < 50; i++) {
			int flags = fcntl(i, F_GETFD);
			if (flags != -1) {
				printf("fd %d is open\n", i);
			}
		}
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	(void)envp;
// 	char	*line;
// 	char	**tokens;
// 	t_cmd *parse_list;
	
// 	parse_list = malloc(sizeof(t_cmd));
// 	if (!parse_list)
// 		exit(1);
// 	parse_list->env_vars = NULL;
// 	parse_list->quoted = NULL;
// 	parse_list->space_after = NULL;
// 	while (1)
// 	{
// 		line = readline(PROMPT);
// 		tokens = lexing(line, parse_list);
// 		parse_list = parsing(tokens, parse_list);
// 	}
// 	return (0);
// }