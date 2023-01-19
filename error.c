/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 18:48:15 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/19 20:03:06 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_error(int error_code)
{
	if (error_code == 0)
		ft_printf(2, "Mini_chelou: 0: command not found\n");
	else if (error_code == CMD)
		ft_printf(2, "Mini_chelou: 1: command not found\n");
	else if (error_code == NOT_EXEC)
		ft_printf(2, "Mini_chelou: 1: command not found\n");
	else if (error_code == NOT_CMD)
		ft_printf(2, "Mini_chelou: 127: command not found\n");
	else if (error_code == SIG_C)
		ft_printf(2, "Mini_chelou: 130: command not found\n");
}
