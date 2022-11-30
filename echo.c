/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 11:48:42 by mstojilj          #+#    #+#             */
/*   Updated: 2022/11/30 13:21:51 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char *s, int option)
{
	int	len;

	len = ft_strlen(s) + 1;
	if (s == NULL)
		ft_printf(1, "\n");
	else if (option == 1)
		ft_printf("%s", s);
	else
		ft_printf("%s\n", s);
}

int	main(int argc, char **argv, char **env)
{
	return (0);
}