/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 17:41:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/22 16:59:14 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern g_t_env	env;

void	ft_free_exit(t_cmd *parse_list, char **tokens)
{
	t_cmd	*temp;
	int		code;

	code = 0;
	// if (parse_list)
	// {
	// 	printf("after\n");
	// 	if (parse_list->args[2] != NULL)
	// 	{
	// 		ft_printf(2, "Mini_chelou: ");
	// 		ft_printf(2, "exit: too many arguments\n");
	// 		return ;
	// 	}
	// 	if (ft_is_digit(parse_list->args[1]))
	// 	{
	// 		ft_printf(2, "Mini_chelou: ");
	// 		ft_printf(2, "exit: %s: numeric argument required\n", parse_list->args[1]);
	// 		return ;
	// 	}
	// 	code = ft_atoi(parse_list->args[1]);
	// 	code %= 256;
	// }
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
		if (parse_list)
			free(parse_list);
		parse_list = temp;
	}
	ft_free_char_array(tokens);
	system("leaks minishell");
	exit(code);
}

void	check_line_exists(char *line, t_cmd *pl, char **tokens) // CTRL-D
{
	if (line == NULL)
	{
		ft_printf(2, "\e[1A\e[%dCexit\n", 13);
		// ft_exit(pl);
		// free_list(pl, tokens);
		ft_free_exit(pl, tokens);
		exit(0);
	}
	else
		return ;
}

void	ft_handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_child_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, &ft_handle_sigquit);
}

void	ft_handle_sigquit(int sig)
{
	if (sig == SIGQUIT)
		printf("Quit: 3\n");
	signal(SIGQUIT, SIG_DFL);
}