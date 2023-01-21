/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:58:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/21 17:15:08 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	env;

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

void	ft_free_env(t_env *list)
{
	t_env	*curr;
	t_env	*tmp;

	curr = list;
	tmp = NULL;
	while (curr)
	{
		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
}

int	ft_arrlen(char **s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	ft_free_char_array(char **s)
{
	int	i;

	i = 0;
	//printf("arr %s\n", s[0]);
	while (i < ft_arrlen(s))
	{
		if (s[i] == NULL)
			break ;
		free(s[i]);
		i++;
	}
	free(s);
}

void	ft_free_cmd(t_cmd *cmd)
{
	t_cmd	*prev;

	prev = NULL;
	while (cmd)
	{
		if (prev != NULL)
			free(prev);
		ft_free_char_array(cmd->args);
		ft_free_char_array(cmd->env_vars);
		prev = cmd;
		cmd = cmd->next;
	}
}

void	ft_exit(t_cmd *cmd)
{
	int	code;

	code = 0;
	if (cmd->args[1] != NULL)
	{
		if (cmd->args[2] != NULL)
		{
			ft_printf(2, "Mini_chelou: ");
			ft_printf(2, "exit: too many arguments\n");
			return ;
		}
		if (ft_is_digit(cmd->args[1]))
		{
			ft_printf(2, "Mini_chelou: ");
			ft_printf(2, "exit: %s: numeric argument required\n", cmd->args[1]);
			return ;
		}
		code = ft_atoi(cmd->args[1]);
		code %= 256;
	}
	//ft_free_env(env.env_list);
	//ft_free_env(env.exp_list);
	ft_free_cmd(cmd);
	//system("leaks minishell");
	exit(code);
}
