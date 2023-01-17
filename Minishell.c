/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/17 20:30:41 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gl_env	env;

void	parse_init(t_cmd *parse_list, char **envp, char **env_vars)
{
	(void)envp;
	env.gl = 0;
	env.error_code = 0;
	parse_list->env_vars = env_vars;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
	parse_list->env_list = env.env_list;
	parse_list->exp_list = NULL;
	parse_list->infile = STDIN_FILENO;
	parse_list->outfile = STDOUT_FILENO;
	parse_list->next = NULL;
	parse_list->piped = 0;
	parse_list->redir[0] = 0;
	parse_list->redir[1] = 0;
	parse_list->out_pipe[0] = -1;
	parse_list->out_pipe[1] = -1;
	parse_list->heredoc[0] = -1;
	parse_list->heredoc[1] = -1;
	parse_list->in_pipe[0] = -1;
	parse_list->in_pipe[1] = -1;
	parse_list->redir_in = -1;
	parse_list->cmd_done = 0;
	parse_list->term = NULL;
}

void	ft_init_env(char **envp)
{
	ft_get_env(&env.env_list, envp); // For env command
	ft_get_env(&env.exp_list, envp); // For export command
	ft_get_export(&env.exp_list);    // Declare -x PWD="somewhere/nice/and/cozy"
}

int	ft_exec_parent(t_cmd *cmd)
{
	if (!cmd->args)
		return (0);
	if (ft_strncmp(cmd->args[0], "exit", 4) == 0 &&
			ft_strlen(cmd->args[0]) == 4)
		ft_exit(cmd);
	else if (ft_strncmp(cmd->args[0], "env", 3) == 0 &&
			ft_strlen(cmd->args[0]) == 3)
	{
		ft_print_env(env.env_list);
		return (1);
	}
	else if (ft_strncmp(cmd->args[0], "unset", 5) == 0 &&
			ft_strlen(cmd->args[0]) == 5)
	{
		ft_unset(cmd);
		return (1);
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
		close (temp->in_pipe[1]);
		if (ft_exec_parent(temp))
			return ;
		ft_exec_cmd(temp, envp);
		// if (temp->infile != STDIN_FILENO)
		// 	close (temp->infile);
		// if (temp->redir[0] == 1)
		// 	close (temp->redir_in);
		// if (temp->redir[1] == 1)
		// 	close(temp->outfile);
		// close (temp->in_pipe[0]);
		// // close (temp->in_pipe[1]);
		// close (temp->out_pipe[1]);
		// parse_list->cmd_done = 1;
		if (temp->next)
			temp = temp->next;
		for (int fd = 0; fd < 30; fd++) {
			int flags = fcntl(fd, F_GETFD);
			if (flags != -1) {
				printf("after closes : fd %d is open\n", fd);
			}
		}
		printf ("---\n");
	}
	i = 0;
	while (i++ < len)
	{
		waitpid(-1, NULL, 0);
		temp = parse_list;
		if (temp->next)
			parse_list = temp->next;
		free(temp);
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
		line = readline(PROMPT);
		check_line_exists(line);
		add_history(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		env_vars = parse_list->env_vars;
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