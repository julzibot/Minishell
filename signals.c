/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 17:41:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/17 21:05:24 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	env;

void	check_line_exists(char *line) // CTRL-D
{
	if (line == NULL)
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
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_child_sigint(int sig)
{
	(void)sig;
	printf("killed gl\n");
	kill(env.gl, SIGTERM);
	write(1, "\n", 1);
}

void	ft_child_sig(void)
{
	signal(SIGINT, &ft_child_sigint);
	signal(SIGQUIT, SIG_DFL);
}

void	ft_handle_sigquit(int sig)
{
	if (sig == SIGQUIT)
		printf("Quit: 3\n");
}