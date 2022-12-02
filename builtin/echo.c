/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 11:48:42 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/01 21:25:54 by mstojilj         ###   ########.fr       */
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

int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int	ft_get_echo_line(char *s)
{
	char	*remove;
	int		i;

	i = 0;
	remove = "echo";
	while (ft_isspace(s[i])) // isspace
		i++;
	while (remove[i] && s[i] == remove[i])
		i++;
	while (ft_isspace(s[i])) // isspace
		i++;
	return (i);
}

int	ft_get_echo_n(char *s, int start)
{
	char	*remove;
	int		i;
	int		j;

	i = start;
	j = 0;
	remove = "-n";
	while (ft_isspace(s[i])) // isspace
		i++;
	while (remove[j] && s[i] == remove[j])
	{
		j++;
		i++;
	}
	while (ft_isspace(s[i])) // isspace
		i++;
	return (i);
}

void	ft_do_echo(char *s)
{
	int	i;
	int	space;

	i = ft_get_echo_line(s);
	space = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
		{
			space = 1;
			i++;
		}
		if (space == 1)
		{
			write(1, " ", 1);
			space = 0;
		}
		write(1, &s[i], 1);
		i++;
	}
	write(1, "\n", 1);
}

void	ft_do_nl_echo(char *s)
{
	int	i;
	int	space;

	i = ft_get_echo_line(s);
	i = ft_get_echo_n(s, i);
	space = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
		{
			space = 1;
			i++;
		}
		if (space == 1)
		{
			write(1, " ", 1);
			space = 0;
		}
		write(1, &s[i], 1);
		i++;
	}
}

void	ft_echo(char *s, int option)
{
	if (s == NULL)
		write(1, "\n", 1);
	else if (option == 1)
		ft_do_nl_echo(s);
	else
		ft_do_echo(s);
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