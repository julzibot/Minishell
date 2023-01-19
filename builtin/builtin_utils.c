/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 14:51:18 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/19 13:39:41 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// t_env **env_list - environment variable list
// int line_nb      - line after which we add new variable (line)
// char *s          - line to add

void	ft_add_after(t_env **env_list, int line_nb, char *s)
{
	t_env	*curr;
	t_env	*new;
	int		i;

	curr = *env_list;
	i = 0;
	new = malloc(sizeof(t_env));
	new->next = NULL;
	new->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
	ft_strcpy(new->line, s);
	if (curr == NULL)
		return ;
	while (curr->next)
	{
		if (i == line_nb)
		{
			new->next = curr->next;
			curr->next = new;
			return ;
		}
		if (line_nb < i)
			return ;
		i++;
		curr = curr->next;
	}
}

int	ft_get_to_str(char *line, char *remove, int *j)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (ft_isspace(line[i]))
		i++;
	while (line[i] == remove[k] && remove[k])
	{
		i++;
		k++;
	}
	while (ft_isspace(line[i]))
		i++;
	*j = k;
	while (line[i] && ft_isspace(line[i] == 0))
		i++;
	return (i);
}

int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}
