/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/05 17:47:10 by mstojilj         ###   ########.fr       */
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

char	*ft_add_quotes(char *var)
{
	int		i;
	int		j;
	char	*env_var;
	
	i = 0;
	j = 0;
	env_var = malloc(sizeof(char) * (ft_strlen(var) + 3));
	if (!env_var)
		exit(1);
	while (var[i])
	{
		if (env_var[j - 1] == '=')
		{
			env_var[j] = '\"';
			j++;
		}
		env_var[j] = var[i];
		if (var[i + 1] == '\0')
		{
			j++;
			env_var[j] = '\"';
			j++;
		}
		i++;
		j++;
	}
	env_var[j] = '\0';
	return (env_var);
}

int	ft_verify_double(t_env *env_list, char *line)
{
	t_env	*curr;

	curr = env_list;
	while (curr)
	{
		if (strcmp(curr->line, line) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	ft_get_export(t_env **exp_list)
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

void	ft_export(t_env **env_list, t_env **exp_list, char *line)
{
	//printf("ENV_VAR: %s\n", env_var);
	if (ft_verify_double(*env_list, line) == 1)
		return ;
	ft_add_after(env_list, 18, line);
	ft_add_after(exp_list, 18, ft_strjoin("declare -x ", ft_add_quotes(line)));
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
