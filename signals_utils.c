/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 17:06:49 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 18:16:23 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_afterchild_sig(void)
{
	signal(SIGINT, &ft_display_nl);
	signal(SIGQUIT, &ft_printquit);
}

void	ft_child_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ft_sig_init(void)
{
	signal(SIGINT, &ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	f__kthenorm(int argc, char **argv)
{
	(void)argc;
	(void)argv;
}
