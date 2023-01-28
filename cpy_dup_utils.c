/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy_dup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:43:44 by jibot             #+#    #+#             */
/*   Updated: 2023/01/27 18:43:49 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strcpy(char *dst, char *src)
{
	int	i;

	i = 0;
	if (src == NULL)
		return ;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	*ft_strdup(char *str)
{
	char	*dup;
	int		i;

	if (!str)
		return (NULL);
	dup = malloc(ft_strlen(str) + 1);
	i = -1;
	while (str[++i])
		dup[i] = str[i];
	dup[i] = '\0';
	return (dup);
}

char	**ft_tabdup(char **tab, int must_free)
{
	char	**dup;
	int		i;

	if (!tab || (tab && !tab[0]))
		return (NULL);
	i = -1;
	dup = malloc(sizeof(char *) * (ft_tablen(tab) + 1));
	if (!dup)
		return (NULL);
	while (tab[++i])
		dup[i] = ft_strdup(tab[i]);
	dup[i] = NULL;
	if (must_free && tab)
		ft_free_char_array(tab);
	return (dup);
}
