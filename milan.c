/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 13:12:46 by mstojilj          #+#    #+#             */
/*   Updated: 2022/11/30 10:37:42 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

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

// Removes the line from ENV
// Called from ft_unset

void	ft_remove_line(t_env **env_list, t_env *node)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_list;
	while (curr)
	{
		if (strcmp(curr->line, node->line) == 0) // Add FT_STRCMP
		{
			prev->next = curr->next;
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

// Unsets ENV variable

void	ft_unset(t_env **env_list, char *s)
{
	t_env	*curr;
	int		n;

	n = ft_strlen(s) - 1;
	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, s, n) == 0)
		{
			ft_remove_line(env_list, curr);
			return ;
		}
		curr = curr->next;
	}
	if (curr == NULL)
		ft_printf(2, "%s: not a valid identifier\n", s);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	t_env	*env_lst;
	char	*line;
	
	env_lst = NULL;
	ft_get_env(&env_lst, env); // Copy original ENV to env_list

	int i = 0;
	for (t_env *curr = env_lst; curr; curr = curr->next) // Prints the ENV
	{
		if (curr == NULL)
					break ;
		printf("%d - %s\n", i, curr->line);
		i++;
	}
	ft_add_after(&env_lst, 18, "hello=milan"); // Add new ENV variable
	ft_add_after(&env_lst, 18, "JULES=VERNE");
	while (1)
	{
		line = readline("Minichelou: ");
		if (strcmp(line, "export") == 0)       // If command is export
		{
			i = 0;
			for (t_env *curr = env_lst; curr; curr = curr->next) // Prints the ENV
			{
				if (curr == NULL)
					break ;
				printf("%d - %s\n", i, curr->line);
				i++;
			}
		}
		else if (strcmp(line, "unset hello") == 0) // If command is "unset hello"
			ft_unset(&env_lst, "hello");           // Unsets the variable
	}
	return (0);
}