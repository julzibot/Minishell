/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:44:02 by jibot             #+#    #+#             */
/*   Updated: 2023/01/27 18:44:06 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str || (str && !str[0]))
		return (0);
	while (str && str[i])
		i++;
	return (i);
}

int	ft_varlen(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	ft_tablen(char **tab)
{
	int	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	env_lstsize(t_env *list)
{
	int	len;

	if (!list)
		return (0);
	len = 1;
	while (list->next)
	{
		list = list->next;
		len++;
	}
	return (len);
}

int	cmd_lstsize(t_cmd *list)
{
	int	len;

	if (!list)
		return (0);
	len = 1;
	while (list->next)
	{
		list = list->next;
		len++;
	}
	return (len);
}
