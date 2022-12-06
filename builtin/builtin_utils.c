/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 14:51:18 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/06 18:11:44 by mstojilj         ###   ########.fr       */
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
	t_env	*tmp;
	int		i;

	curr = *env_list;
	i = 0;
	new = malloc(sizeof(t_env));
	if (!new)
		exit(1);
	new->next = NULL;
	new->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!new->line)
		exit(1);
	ft_strcpy(new->line, s);
	while (curr->next)
	{
		if (i == line_nb)
		{
			tmp = curr->next;
			curr->next = new;
			new->next = tmp;
			return ;
		}
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

// Removes cmd from string
// Ex. char	*ft_remove_cmd("cd ../past", "cd ")
// "cd ../past" -> "../past"
// 🚨 Don't forget to free afterwards! 🚨
char	*ft_remove_cmd(char *line, char *remove)
{
	char	*s;
	int		i;
	int		j;

	i = 0;
	j = 0;
	
	if (line == NULL)
		return (NULL);
	if (remove == NULL)
		return (line);
	i = ft_get_to_str(line, remove, &j);
	if (j == 0)
		return (line);
	i -= j;
	s = malloc(sizeof(char) * (i + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (line[j])
	{
		s[i] = line[j];
		i++;
		j++;
	}
	s[i] = '\0';
	return (s);
}