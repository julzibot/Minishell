/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:58:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/24 15:29:24 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	g_env;

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
	if (!s )
		return ;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	if (s[i])
		free(s[i]);
	if (s)
		free(s);
}

void	ft_exit(t_cmd *cmd, int error_code, char **tokens)
{
	t_cmd	*temp;

	temp = NULL;
	if (cmd && cmd->args && cmd->args[1] != NULL)
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
		error_code = ft_atoi(cmd->args[1]);
		error_code %= 256;
	}
	if (cmd->env_vars)
		ft_free_char_array(cmd->env_vars);
	if (cmd->space_after)
		free(cmd->space_after);
	if (cmd->quoted)
		free(cmd->quoted);
	while (cmd)
	{
		if (cmd->args)
			ft_free_char_array(cmd->args);
		temp = cmd->next;
		if (cmd)
			free(cmd);
		cmd = temp;
	}
	ft_free_char_array(tokens);
	system("leaks --list minishell");
	exit(error_code);
}
