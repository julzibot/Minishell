/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:58:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/16 19:11:59 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

int	ft_is_digit(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
			i++;
		else
			break ;
	}
	if (s[i] == '\0')
		return (0);
	return (1);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	while (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res *= 10;
		res += str[i] - '0';
		i++;
	}
	return (res * sign);
}

void	ft_exit(t_cmd *cmd)
{
	int	code;

	code = 0;
	if (cmd->args[2] != NULL)
	{
		ft_printf(2, "Minichelou: exit: too many arguments\n");
		return ;		
	}
	if (cmd->args[1] != NULL)
	{
		if (ft_is_digit(cmd->args[1]))
		{
			ft_printf(2, "Minichelou: exit: %s: numeric argument required\n", cmd->args[1]);
			return ;
		}
		code = ft_atoi(cmd->args[1]);
		code %= 256;
	}
	free(cmd->env_vars);
	exit(code);
}