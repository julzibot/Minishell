/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:43:19 by jibot             #+#    #+#             */
/*   Updated: 2023/01/27 18:43:27 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*char_cat(char *str, char c)
{
	int		len;
	int		i;
	char	*new;

	i = 0;
	len = ft_strlen(str) + 2;
	new = malloc(len);
	while (str && str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = c;
	new[++i] = '\0';
	free(str);
	return (new);
}

char	**token_join(char **args, char *token)
{
	char	**tab;
	int		i;

	if (!token)
		return (args);
	i = -1;
	tab = malloc(sizeof(char *) * (ft_tablen(args) + 2));
	if (!args)
	{
		tab[0] = ft_strdup(token);
		tab[1] = NULL;
		free(token);
		return (tab);
	}
	while (args[++i])
		tab[i] = ft_strdup(args[i]);
	tab[i] = ft_strdup(token);
	tab[i + 1] = NULL;
	ft_free_char_array(args);
	free(token);
	return (tab);
}

void	ft_free_join(char *s1, char *s2, int must_free)
{
	if (must_free == 1)
		free(s1);
	if (must_free == 2)
		free(s2);
	if (must_free == 3)
	{
		free(s1);
		free(s2);
	}
}

char	*ft_strjoin(char *s1, char *s2, int must_free)
{
	char	*str;
	int		i;
	int		j;

	if (!s1)
		return (s2);
	else if (!s2)
		return (s1);
	i = -1;
	j = -1;
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		exit(1);
	while (s1[++i])
		str[i] = s1[i];
	while (s2 && s2[++j])
		str[i++] = s2[j];
	str[i] = '\0';
	ft_free_join(s1, s2, must_free);
	return (str);
}
