/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 16:38:05 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/14 17:32:05 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strstr(char *haystack, char *needle)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (needle[j] == '\0')
		return ((char *)haystack);
	while (haystack[i])
	{
		while (haystack[i + j] == needle[j])
		{
			j++;
			if (needle[j] == '\0')
				return (&((char *)haystack)[i]);
		}
	i++;
	j = 0;
	}
	return (NULL);
}

char	*ft_substr(char *s, unsigned int start)
{
	char				*sub;
	unsigned int		i;
	unsigned int		j;

	i = start;
	j = 0;
	while (s[start + j] && start + j < (unsigned int)ft_strlen(s))
		j++;
	sub = malloc(j * sizeof(char) + 1);
	if (!sub)
	{
		free(sub);
		return (NULL);
	}
	j = 0;
	while (s[i])
	{
		sub[j] = s[i];
		j++;
		i++;
	}
	sub[j] = '\0';
	return (sub);
}