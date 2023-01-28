/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 17:41:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 18:21:34 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	g_env;

// CTRL-D
void	check_line_exists(char *line, t_cmd *pl, char **tokens)
{
	if (line == NULL)
	{
		ft_printf(2, "\e[1A\e[%dCexit\n", 13);
		ft_free_exit(pl, tokens);
		exit(g_env.error_code);
	}
	else
		return ;
}

void	ft_handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ft_handle_sigquit(int sig)
{
	(void)sig;
	ft_printf(2, "Quit: 3\n");
	g_env.gl = 131;
	exit(131);
}

void	ft_display_nl(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	ft_printquit(int sig)
{
	(void)sig;
	ft_printf(2, "Quit: 3\n");
}
