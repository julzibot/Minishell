/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 11:48:42 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/16 18:26:55 by mstojilj         ###   ########.fr       */
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

int	ft_loop_echo_nl(char **args, int i, int *nl)
{
	int	j;

	j = i;
	while (args[j])
	{
		if (ft_is_echo_nl(args[j]))
		{
			*nl = 1;
			j++;
		}
		else
			return (j);
	}
	return (1);
}

void	ft_do_echo(t_cmd *cmd, int fd)
{
	int	i;
	int	j;
	int	nl;

	i = 1;
	j = 0;
	nl = 0;
	i = ft_loop_echo_nl(cmd->args, i, &nl);
	while (cmd->args[i])
	{
		while (cmd->args[i][j])
		{
			write(fd, &cmd->args[i][j], 1);
			j++;
		}
		if (cmd->args[i + 1] != NULL)
			write(fd, " ", 1);
		j = 0;
		i++;
	}
	if (nl == 0)
		write(fd, "\n", 1);
}

void	ft_echo(t_cmd *cmd)
{
	int	fd;

	fd = cmd->outfile;
	if (cmd->redir[1] == 0 && cmd->piped == 1)
		fd = cmd->out_pipe[1];
	if (cmd->args[1] == NULL)
		write(fd, "\n", 1);
	ft_do_echo(cmd, fd);
}
