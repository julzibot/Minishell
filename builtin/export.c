/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/09 17:30:03 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_update_var(t_env **env_list, char *s)
{
	t_env	*curr;
	t_env	*new;
	t_env	*prev;
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
		if (ft_strncmp(curr->line, s, ft_varlen(s)) == 0)
		{
			new->next = curr->next;
			prev->next = new;
			free(curr);
			return ;
		}
		i++;
		prev = curr;
		curr = curr->next;
	}
}

void	ft_update_env(t_env **env_list, t_env **exp_list, char *line)
{
	if (ft_verify_double(*env_list, line) == 1 || ft_verify_equal(line) == 1)
		return ;
	line = ft_verify_env_var(line);
	//line = ft_var_content(env_list, exp_list, line);
	if (line == NULL)
		return ;
	printf("line: %s\n", line);
	ft_update_var(env_list, line);
	ft_update_var(exp_list, ft_strjoin("declare -x ", ft_add_quotes(line)));
}

void	ft_get_export(t_env **exp_list) // Adds declare -x and quotes
{
	t_env	*curr;

	curr = *exp_list;
	while (curr)
	{
		curr->line = ft_add_quotes(curr->line);
		curr->line = ft_strjoin("declare -x ", curr->line);
		curr = curr->next;
	}
}

void	ft_export(t_env **env_list, t_env **exp_list, char **vars)
{
	int	i;

	i = 0;
	if (vars == NULL)
		return ;
	while (vars[i])
	{
		printf("VARIABLE %s\n", vars[i]);
		if (ft_verify_double(*env_list, vars[i]) == 1 || ft_verify_equal(vars[i]) == 1)
			return ;
		vars[i] = ft_verify_env_var(vars[i]);
		vars[i] = ft_var_content(env_list, exp_list, vars[i]);
		if (vars[i] == NULL)
			return ;
		ft_add_after(env_list, 17, vars[i]);
		ft_add_after(exp_list, 17, ft_strjoin("declare -x ", ft_add_quotes(vars[i])));
		i++;
	}
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
