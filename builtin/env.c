/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:58 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/06 16:50:58 by mstojilj         ###   ########.fr       */
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

// int	main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	char	*line;
// 	t_env 	*env_list;
// 	t_env	*exp_list;
	
// 	env_list = NULL;
// 	exp_list = NULL;
// 	ft_get_env(&env_list, env); // For env command
// 	ft_get_env(&exp_list, env); // For export command
// 	ft_get_export(&exp_list);   // Declare -x PWD="somewhere/nice/and/cozy"
// 	while (1)
// 	{
// 		line = readline("MiniShelly: ");
// 		if (strcmp(line, "export") == 0)
// 			ft_print_env(&exp_list);
// 		else if (ft_strncmp(line, "export", 6) == 0)
// 			ft_export(&env_list, &exp_list, ft_remove_cmd(line, "export "));
// 		else if (ft_strncmp(line, "env", 3) == 0)
// 			ft_print_env(&env_list);
// 		else if (ft_strncmp(line, "unset", 5) == 0)
// 		{
// 			ft_unset(&env_list, &exp_list, ft_remove_cmd(line, "unset "));
// 		}
// 	}
// 	ft_print_env(&exp_list);
// 	return (0);
// }