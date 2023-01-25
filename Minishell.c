/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/25 12:18:15 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

g_t_env	g_env;

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
	if (tokens)
		ft_free_char_array(tokens);
	return;
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
		{
			temp->next->in_pipe[0] = temp->out_pipe[0];
			temp->next->in_pipe[1] = temp->out_pipe[1];
		}
		printf("exec pipeline\n");
		status = ft_exec_cmd(temp, envp, tokens, i);
		if (temp->next)
			temp = temp->next;
	}
	i = 0;
	while (i++ < len)
	{
		waitpid(-1, &status, 0);
		g_env.error_code = status;
	}
	return (status);
}

int	ft_sprint(t_env *env_lst)
{
	t_env	*curr;
	int		i;

	i = 0;
	curr = env_lst;
	if (curr == NULL)
		return (1);
	while (curr)
	{
		ft_printf(1, "%d - %s\n", i, curr->line);
		i++;
		curr = curr->next;
	}
	return (0);
}

char	**ft_env_vars_dup(char **tab, int must_free)
{
	char **dup;
	int	i;
	int	len;

	if (!tab || !tab[0])
		return (NULL);
	len = ft_arrlen(tab);
	i = -1;
	dup = malloc(sizeof(char*) * (ft_tablen(tab) + 1));
	if (!dup)
		return (NULL);
	while (++i < len - 1)
		dup[i] = ft_strdup(tab[i]);
	dup[i] = NULL;
	if (must_free && tab)
	 	ft_free_char_array(tab);
	return (dup);
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	char			**tokens;
	char			**env_vars;
	t_cmd 			*parse_list;
	(void)argc;
	(void)argv;

	env_vars = NULL;
	ft_init_termios(&g_env.term);
	ft_init_env(envp);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, ft_handle_sigint);
		parse_list = malloc(sizeof(t_cmd));
		if (!parse_list)
			exit(1);
		parse_init(parse_list, ft_tabdup(env_vars, 1));
		line = readline(PROMPT);
		add_history(line);
		tokens = lexing(line, parse_list);
		check_line_exists(line, parse_list, tokens);
		if (tokens)
		{
			parse_list = parsing(tokens, parse_list);
			// if  (!parse_list)
			// 	printf("PARSE LIST PROBLEM\n");
			if	(parse_list)
			{
				// for (int i = 0; parse_list->env_vars[i]; i++)
				// 	printf("env %s\n", parse_list->env_vars[i]);
				exec_pipeline(parse_list, envp, tokens);
				env_vars = ft_tabdup(parse_list->env_vars, 0);
			}
		}
		check_line_exists(line, parse_list, tokens);
		if (parse_list || !tokens)
			free_list(parse_list, tokens);
		free(line);
		// system("leaks minishell\n");
	}
	return (0);
}
