/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/01 21:58:34 by mstojilj         ###   ########.fr       */
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

void	ft_export(t_env **env_list, char *line)
{
	(void)env_list;
	char	*env_var;

	env_var = ft_get_env_var(line, "export");
	//printf("ENV_VAR: %s\n", env_var);
	ft_add_after(env_list, 18, env_var);
}

// int	main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	t_env	*env_lst;
// 	char	*line;
	
// 	env_lst = NULL;
// 	ft_get_env(&env_lst, env); // Copy original ENV to env_list

// 	int i = 0;
// 	for (t_env *curr = env_lst; curr; curr = curr->next) // Prints the ENV
// 	{
// 		if (curr == NULL)
// 					break ;
// 		printf("%d - %s\n", i, curr->line);
// 		i++;
// 	}
// 	ft_add_after(&env_lst, 18, "hello=milan"); // Add new ENV variable
// 	ft_add_after(&env_lst, 18, "JULES=VERNE");
// 	while (1)
// 	{
// 		line = readline("Minichelou: ");
// 		if (strcmp(line, "export") == 0)       // If command is export
// 		{
// 			i = 0;
// 			for (t_env *curr = env_lst; curr; curr = curr->next) // Prints the ENV
// 			{
// 				if (curr == NULL)
// 					break ;
// 				printf("%d - %s\n", i, curr->line);
// 				i++;
// 			}
// 		}
// 		else if (strcmp(line, "unset hello") == 0) // If command is "unset hello"
// 			ft_unset(&env_lst, "hello");           // Unsets the variable
// 	}
// 	return (0);
// }
