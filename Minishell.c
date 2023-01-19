/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/19 21:06:13 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gl_env	env;

void	exec_pipeline(t_cmd *parse_list, char **envp)
{
	t_cmd	*temp;
	int		len;
	int		i;

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

void	free_list(t_cmd *parse_list, char **tokens)
{
	t_cmd	*temp;

	while (parse_list && parse_list->next)
	{
		temp = parse_list->next;
		// free(parse_list->space_after);
		// free(parse_list->quoted);
		free(parse_list);
		parse_list = temp;
	}
	// free(parse_list->space_after);
	// free(parse_list->quoted);
	free(parse_list);
	free(tokens);
	return;
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
	env.gl = 0;
	env.error_code = 0;
	// ft_init_termios(term);
	// ft_init_env(envp);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, ft_handle_sigint);
		parse_list = malloc(sizeof(t_cmd));
		if (!parse_list)
			exit(1);
		parse_init(parse_list, envp, env_vars);
		line = readline(PROMPT);
		check_line_exists(line);
		add_history(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		env_vars = parse_list->env_vars;
		// exec_pipeline(parse_list, envp);
		free_list(parse_list, tokens);
		// free(line);
		//system("leaks minishell\n");
	}
	return (0);
}
