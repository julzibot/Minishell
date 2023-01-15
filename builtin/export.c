/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/15 14:07:57 by mstojilj         ###   ########.fr       */
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
	if (cmd->env_vars == NULL)
		return ;
	while (cmd->env_vars[i])
	{
		cmd->env_vars = create_env_vars(cmd->args[i], cmd->env_vars);
		i++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

// 4 EXPORT CASES:
// -----------------------------
// 1) "export a=b" -> "unset a"
// Export a variable, then unset it
// -----------------------------
// 2) "export a=b" -> "export a=c"
// Export a variable, then change it's value
// -----------------------------
// 3) "a=b c=d" -> "export a"
// Declare a variable, then export it
// -----------------------------
// 4) "a=b c=d" -> "export a c"
// Declare variables, then export them
// -----------------------------
// 5) "a=b" -> "export a c=d"
// Declare variables, then export them

int	ft_check_double_var(t_cmd *cmd, char *line) // Check if there is a duplicate
{
	int	i;

	i = 0;
	while (cmd->env_vars[i])
	{
		if (ft_strcmp(cmd->env_vars[i], line) == 0)
		{
			printf("IS DOUBLE\n");
			printf("env_vars[i] %s line %s\n", cmd->env_vars[i], line);
			return (1);
		}
		i++;
	}
	printf("NO DOUBLE");
	return (0);
}

int	ft_change_var_content(t_cmd *cmd, char *line) // Check if there is an existing variable that changed content
{
	int	i;

	i = 0;
	while (cmd->env_vars[i])
	{
		if (ft_strncmp(cmd->env_vars[i], line, ft_varlen(line)) == 0) // Same VAR found
		{
			if (ft_strcmp(cmd->env_vars[i], line) != 0) // Check if VAR is the same
				return (1);
		}
		i++;
	}
	return (0);
}

// void	ft_update_var(void)
// {
	
// }

void	ft_loop_assign_vars(char **env_vars, char *line)
{
	int	i;

	i = 0;
	while (env_vars[i])
	{
		if (ft_strcmp(env_vars[i], line) == 0)
		{
			ft_add_after(&env.env_list, 17, line);
			ft_add_after(&env.exp_list, 17, ft_strjoin("declare -x ",
					ft_add_quotes(line)));
			return ;
		}
		i++;
	}

}

void	ft_equal_var(t_cmd *cmd, char *line)
{
	printf("EQUAL\n");
	// if (ft_check_double_var(cmd, line) == 1) // Check double
	// 	return ;
	// printf("Double passed\n");
	for (int i = 0; cmd->env_vars[i]; i++)
		printf("%s\n", cmd->env_vars[i]);
	if (ft_change_var_content(cmd, line)) // Check if content change
	{
		printf("content change\n");
		ft_update_var(&env.env_list, line); // Function needs change
		ft_update_var(&env.exp_list, line);
		return ;
	}
	// Assign to env_vars
	// create_env_vars(line, cmd->env_vars);
	// Loop through env_vars to find VAR and export
	printf("LOOP\n");
	ft_loop_assign_vars(cmd->env_vars, line);
	return ;
}

void	ft_export(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (cmd->args[1] == NULL)
		return ;
	while (cmd->env_vars[j])
	{
		while (cmd->args[i])
		{
			if (ft_strcmp(cmd->args[i], cmd->env_vars[j]) == 0)
			{
				printf("IS EQUAL\n");
				if (ft_verify_equal(cmd->args[i])) // Equal sign found
					ft_equal_var(cmd, cmd->args[i]);
				else
					return ;
			}
			j = 0;
			i++;
		}
		j++;
	}
}














// void	ft_export(t_cmd *cmd)
// {
// 	int	i;

// 	i = 0;
// 	for (int j = 0; cmd->env_vars[j]; j++)
// 		printf("env var: %s\n", cmd->env_vars[j]);
// 	ft_line_to_var(cmd);
// 	if (cmd->env_vars == NULL)
// 		return ;
// 	// if (ft_var_content(cmd, cmd->args[1]))
// 	// {
// 	// 	ft_update_var(&env.env_list, cmd->args[1]);
// 	// 	ft_update_var(&env.exp_list, ft_strjoin("declare -x ", ft_add_quotes(cmd->args[1])));
// 	// 	return ;
// 	// }
// 	while (cmd->env_vars[i])
// 	{
// 		if (ft_strncmp(cmd->env_vars[i], cmd->args[1],
// 				ft_strlen(cmd->args[1])) == 0)
// 			break ;
// 		i++;
// 	}
// 	if (ft_verify_double(env.env_list, cmd->env_vars[i]) == 1)
// 		return ;
// 	cmd->env_vars[i] = ft_verify_env_var(cmd->env_vars[i]); // Verify format
// 	ft_add_after(&env.env_list, 17, cmd->env_vars[i]);
// 	ft_add_after(&env.exp_list, 17, ft_strjoin("declare -x ",
// 			ft_add_quotes(cmd->env_vars[i])));
// 	i++;
// }
