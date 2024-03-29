/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/28 18:16:51 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gl_env	g_env;

void	free_list(t_cmd *parse_list, char **tokens)
{
	t_cmd	*temp;

	if (parse_list)
	{
		if (parse_list->env_vars)
			ft_free_char_array(parse_list->env_vars);
		if (parse_list->space_after)
			free(parse_list->space_after);
		if (parse_list->quoted)
			free(parse_list->quoted);
		while (parse_list)
		{
			if (parse_list->args)
				ft_free_char_array(parse_list->args);
			temp = parse_list->next;
			free(parse_list);
			parse_list = temp;
		}
	}
	ft_free_char_array(tokens);
	return ;
}

void	inpipe_next_cmd(t_cmd *temp)
{
	temp->next->in_pipe[0] = temp->out_pipe[0];
	temp->next->in_pipe[1] = temp->out_pipe[1];
}

int	exec_pipeline(t_cmd *parse_list, char **envp, char **tokens)
{
	t_cmd	*temp;
	int		status;
	int		len;
	int		i;

	status = 0;
	len = cmd_lstsize(parse_list);
	i = -1;
	temp = parse_list;
	while (++i < len)
	{
		if (temp->piped && pipe(temp->out_pipe) == -1)
			return (1);
		if (temp->piped && temp->next)
			inpipe_next_cmd(temp);
		status = ft_exec_cmd(temp, envp, tokens, i);
		if (temp->next)
			temp = temp->next;
	}
	while (--i > -1)
	{
		waitpid(-1, &status, 0);
		g_env.error_code = ft_get_exit_code(status);
	}
	return (status);
}

t_cmd	*parse_exec_loop(char **tokens, t_cmd *pl, \
	char **envp, char **env_vars)
{
	if (tokens)
	{
		pl = parsing(tokens, pl);
		if (pl)
		{
			exec_pipeline(pl, envp, tokens);
			ft_free_char_array(env_vars);
		}
	}
	return (pl);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;
	char	**env_vars;
	t_cmd	*parse_list;

	f__kthenorm(argc, argv);
	env_vars = NULL;
	ft_init_env(envp);
	while (1)
	{
		ft_sig_init();
		parse_list = malloc(sizeof(t_cmd));
		parse_init(parse_list, ft_tabdup(env_vars, 0));
		line = readline(PROMPT);
		add_history(line);
		tokens = lexing(line, parse_list);
		check_line_exists(line, parse_list, tokens);
		parse_list = parse_exec_loop(tokens, parse_list, envp, env_vars);
		if (parse_list)
			env_vars = ft_tabdup(parse_list->env_vars, 0);
		if (parse_list || !tokens)
			free_list(parse_list, tokens);
		free(line);
	}
	return (0);
}
