/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/14 17:30:08 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

int	ft_update_var(t_env **env_list, char *s)
{
	t_env	*curr;
	t_env	*new;
	t_env	*prev;
	int		i;

	curr = *env_list;
	i = 0;
	new = malloc(sizeof(t_env));
	new->next = NULL;
	new->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
	ft_strcpy(new->line, s);
	while (curr->next)
	{
		if (ft_strncmp(curr->line, s, ft_varlen(s)) == 0)
		{
			new->next = curr->next;
			prev->next = new;
			free(curr);
			return (1);
		}
		i++;
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

void	ft_update_env(t_env **env_list, t_env **exp_list, char *line)
{
	if (ft_verify_double(*env_list, line) == 1 || ft_verify_equal(line) == 1)
		return ;
	line = ft_verify_env_var(line);
	//line = ft_var_content(env_list, exp_list, line);
	if (line == NULL)
		return ;
	printf("update var\n");
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

void	ft_line_to_var(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->env_vars[i])
	{
		cmd->env_vars = create_env_vars(cmd->args[i], cmd->env_vars);
		i++;
	}
}

void	ft_export(t_cmd *cmd)
{
	int	i;

	i = 0;
	ft_line_to_var(cmd);
	if (cmd->env_vars == NULL)
		return ;
	if (ft_var_content(cmd, cmd->args[1]))
	{
		ft_update_var(&env.env_list, cmd->args[1]);
		ft_update_var(&env.exp_list, ft_strjoin("declare -x ", ft_add_quotes(cmd->args[1])));
		return ;
	}
	while (cmd->env_vars[i])
	{
		if (ft_strncmp(cmd->env_vars[i], cmd->args[1],
				ft_strlen(cmd->args[1])) == 0)
			break ;
		i++;
	}
	// if (ft_verify_double(cmd->env_list, cmd->env_vars[i]) == 1 || ft_verify_equal(cmd->env_vars[i]) == 1)
	// 	return ;
	if (ft_verify_double(env.env_list, cmd->env_vars[i]) == 1)
		return ;
	cmd->env_vars[i] = ft_verify_env_var(cmd->env_vars[i]); // Verify format
	// if (cmd->env_vars[i] == NULL)
	// 	return ;
	ft_add_after(&env.env_list, 17, cmd->env_vars[i]);
	ft_add_after(&env.exp_list, 17, ft_strjoin("declare -x ",
			ft_add_quotes(cmd->env_vars[i])));
	i++;
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
