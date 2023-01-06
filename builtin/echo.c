/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 11:48:42 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/06 17:20:00 by mstojilj         ###   ########.fr       */
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

void	ft_do_echo(char **args)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (args[i])
	{
		while (args[i][j])
		{
			write(1, &args[i][j], 1);
			j++;
		}
		if (args[i + 1] != NULL)
			write(1, " ", 1);
		j = 0;
		i++;
	}
	write(1, "\n", 1);
}

void	ft_do_nl_echo(char **args)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (args[i])
	{
		if (ft_is_echo_nl(args[i]))
			i++;
		else
			break ;
	}
	while (args[i])
	{
		while (args[i][j])
		{
			write(1, &args[i][j], 1);
			j++;
		}
		if (args[i + 1] != NULL)
			write(1, " ", 1);
		j = 0;
		i++;
	}
}

void	ft_echo(char **args)
{
	if (args[1] == NULL)
		write(1, "\n", 1);
	else if (ft_is_echo_nl(args[1]))
		ft_do_nl_echo(args);
	else
		ft_do_echo(args);
}

// int	main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	(void)env;
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline("Minishell: ");
// 		if (strncmp("echo", line, 4) == 0)
// 		{
// 			if (ft_strnstr(line, "-n", ft_strlen(line)))
// 				ft_echo(line, 1);
// 			else
// 				ft_echo(line, 0);
// 		}
// 	}
// 	return (0);
// }