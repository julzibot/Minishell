/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/14 17:47:50 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gl_env	env;

void	parse_init(t_cmd *parse_list, char **envp)
{
	(void)envp;
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
	parse_list->out_pipe[0] = -1;
	parse_list->out_pipe[1] = -1;
	parse_list->in_pipe = -2;
	parse_list->redir_in = -1;
	parse_list->term = NULL;
}

void	ft_init_env(char **envp)
{
	ft_get_env(&env.env_list, envp); // For env command
	ft_get_env(&env.exp_list, envp); // For export command
	ft_get_export(&env.exp_list);    // Declare -x PWD="somewhere/nice/and/cozy"
}

void	check_line_exists(char *line)
{
	if (line == NULL) // CTRL-D
		{
			ft_printf(2, "\e[1A\e[%dCexit\n", 13);
			exit(0);
		}
	else
		return ;
}

void	ft_handle_sigint(int sig)
{
	(void)sig;
	// if (g_pid != 0)
	// {
	// 	write(1, "\n", 1);
	// 	kill(g_pid, SIGCONT);
	// }
	// else
	// {
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	// }
}

int	lstsize(t_cmd *list)
{
	int	len;

	if (!list)
		return (0);
	len = 1;
	while (list->next)
	{
		list = list->next;
		len++;
	}
	return (len);
}

int	ft_exec_parent(t_cmd *cmd)
{
	if (ft_strncmp(cmd->args[0], "exit", 4) == 0)
		ft_exit(cmd);
	else if (ft_strncmp(cmd->args[0], "env", 3) == 0)
	{
		ft_print_env(env.env_list);
		return (1);
	}
	else if (ft_strncmp(cmd->args[0], "unset", 5) == 0)
	{
		ft_unset(&env.env_list, &env.exp_list, cmd->args[1]);
		return (1);
	}
	else if (ft_strncmp(cmd->args[0], "export", 6) == 0)
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

	len = lstsize(parse_list);
	while (len-- > 0)
	{
		// close (parse_list->out_pipe[0]);
		if (ft_exec_parent(parse_list))
			return ;
		ft_exec_cmd(parse_list, envp);
		if (parse_list->infile != STDIN_FILENO)
			close (parse_list->infile);
		if (parse_list->redir == 1)
			close (parse_list->redir_in);
		close (parse_list->in_pipe);
		close (parse_list->out_pipe[1]);
		// close (parse_list->out_pipe[0]);
		if (parse_list->redir == 2)
			close(parse_list->outfile);
		temp = parse_list;
		if (temp->next)
			parse_list = temp->next;
		free(temp);
		for (int i = 0; i < 20; i++) {
			int flags = fcntl(i, F_GETFD);
			if (flags != -1) {
				printf("fd %d is open\n", i);
			}
		}
	}
}

void	ft_init_termios(struct termios *term)
{
	term = malloc(sizeof(struct termios));
	if (!term)
		exit(1);
	tcgetattr(STDOUT_FILENO, term);
	term->c_lflag = term->c_lflag ^ ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, term);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char			*line;
	char			**tokens;
	t_cmd 			*parse_list;
	struct termios			*term;

	term = NULL;
	ft_init_termios(term);
	ft_init_env(envp);
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
		add_history(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		exec_pipeline(parse_list, envp);
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