/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:33:24 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/27 16:28:48 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_alnum(char c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') \
		|| ('0' <= c && c <= '9'))
		return (1);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	only_spaces(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] == ' ')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (n == 0)
		return (0);
	while (*s1 && *s2 && *s1 == *s2 && ++i < n)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

//is_delim :
//1 -> redir
//2 -> pipe
//3 -> quote
//4 -> isspace
//0 -> not a delim
int	is_delim(char c)
{	
	if (c == '\'' || c == '\"')
		return (1);
	else if (c == '|')
		return (2);
	else if (c == '<' || c == '>')
		return (3);
	else if (c == ' ' || c == '	')
		return (4);
	else
		return (0);
}
