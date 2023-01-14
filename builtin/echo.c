/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 11:48:42 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/14 18:18:25 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>

int	ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (needle[j] == '\0')
		return (0);
	if (len == 0)
		return (0);
	if (len == 1)
		if (haystack[i] == needle[i])
			return (1);
	while (haystack[i] && i + j < len - 1)
	{
		while (haystack[i + j] == needle[j] && i + j < len)
		{
			j++;
			if (needle[j] == '\0')
				return (1);
		}
	i++;
	j = 0;
	}
	return (0);
}

int	ft_is_echo_nl(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-')
		i++;
	else
		return (0);
	while (s[i])
	{
		if (s[i] == 'n')
			i++;
		else
			return (0);
	}
	return (1);
}

void	ft_do_echo(t_cmd *cmd)
{
	int	i;
	int	j;
	int	nl;

	i = 1;
	j = 0;
	nl = 0;
	while (cmd->args[i])
	{
		if (ft_is_echo_nl(cmd->args[i]))
		{
			nl = 1;
			i++;
		}
		else
			break ;
	}
	while (cmd->args[i])
	{
		while (cmd->args[i][j])
		{
			write(1, &cmd->args[i][j], 1);
			j++;
		}
		if (cmd->args[i + 1] != NULL)
			write(1, " ", 1);
		j = 0;
		i++;
	}
	if (nl == 0)
		write(1, "\n", 1);
}

void	ft_echo(t_cmd *cmd)
{
	printf("etneres\n");
	if (cmd->args[1] == NULL)
		write(1, "\n", 1);
	ft_do_echo(cmd);
}
