/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:58 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/01 21:55:38 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Adds a variable to env
// t_env **env_list - environment variable list
// int line_nb      - line after which we add new variable (line)
// char *s          - line to add

void	ft_add_queue(t_env **root, char *s) // Add line to ENV linked list
{
	t_env	*node;
	t_env	*curr;

	curr = *root;
	node = malloc(sizeof(t_env));
	if (!node)
		exit(1);

	node->next = NULL;
	node->line = malloc(sizeof(char) * (ft_strlen(s) + 1)); 
	if (!node->line)
		exit(1);
	ft_strcpy(node->line, s);
	
	if (*root == NULL || root == NULL)
	{
		*root = node;
		return ;
	}
	while (curr)
	{
		if (curr->next == NULL)
		{
			curr->next = malloc(sizeof(t_env));
			curr->next = node;
			return ;
		}
		curr = curr->next;
	}
	curr->next = node;
}

void	ft_get_env(t_env **env_list, char **env) // Copies env to env_list
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_add_queue(env_list, env[i]);
		i++;
	}
	env[i] = NULL;
}

void	ft_print_env(t_env **env_lst)
{
	int	i;

	i = 0;
	for (t_env *curr = *env_lst; curr; curr = curr->next)
	{
		if (curr == NULL)
			break ;
		printf("%d - %s\n", i, curr->line);
		i++;
	}
}

char	*ft_get_env_var(char *line, char *remove)
{
	char	*env_var;
	int		i;
	int		j;


	i = 0;
	j = 0;
	while (ft_isspace(line[i])) // isspace
		i++;
	while (line[i] == remove[j] && remove[j])
	{
		i++;
		j++;
	}
	while (ft_isspace(line[i])) // isspace
		i++;
	
	j = i;
	while (line[i] && ft_isspace(line[i] == 0))
		i++;
	i -= j;
	env_var = malloc(sizeof(char) * (i + 1));
	if (!env_var)
		return (NULL);
	i = 0;
	while (line[j])
	{
		env_var[i] = line[j];
		i++;
		j++;
	}
	env_var[i] = '\0';
	return (env_var);
}