/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/15 17:34:02 by mstojilj         ###   ########.fr       */
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
	if (line == NULL)
		return ;
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

// void	ft_line_to_var(t_cmd *cmd)
// {
// 	int	i;

// 	i = 1;
// 	if (cmd->env_vars == NULL)
// 		return ;
// 	while (cmd->env_vars[i])
// 	{
// 		cmd->env_vars = create_env_vars(cmd->args[i], cmd->env_vars);
// 		i++;
// 	}
// }

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

// 6 EXPORT CASES:
// -----------------------------
// 1) "export a=b" -> "unset a" ✅
// Export a variable, then unset it
// -----------------------------
// 2) "export a=b" -> "a=c" -> Changes the value in ENV/export ✅
// Export a variable, then change it's value
// -----------------------------
// 3) "a=b c=d" -> "export a" ✅
// Declare a variable, then export it
// -----------------------------
// 4) "a=b c=d" -> "export a c" ✅
// Declare variables, then export them
// -----------------------------
// 5) "a=b" -> "export a c=d" ✅
// Declare variables, then export them
// -----------------------------
// 6) "export a=b c=d" -> "export a c=d" ✅
// Export more than one variable at once
// -----------------------------
// 7) "export a=$HOME"
// Export variable assigned with another variable's value
// -----------------------------
// 8) "export a="$HOME""
// Export variable assigned with another variable's value and double quotes
// -----------------------------
// 9) "export a='$HOME'"
// Export variable assigned with another variable name and single quotes

// CHECK UNSET:
// -----------------------------
// 5) "export a=b c=d" -> "unset a c" ✅
// Unset more than one variable

int	ft_check_double_var(t_cmd *cmd, char *line) // Check if there is a duplicate
{
	int	i;

	i = 0;
	while (cmd->env_vars[i])
	{
		if (ft_strcmp(cmd->env_vars[i], line) == 0)
			return (1);
		i++;
	}
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

void	ft_loop_assign_vars(char **env_vars, char *line)
{
	int	i;

	i = 0;
	while (env_vars[i])
	{
		if (ft_strcmp(env_vars[i], line) == 0)
		{
			ft_add_after(&env.env_list, 15, line);
			ft_add_after(&env.exp_list, 15, ft_strjoin("declare -x ",
					ft_add_quotes(line)));
			return ;
		}
		i++;
	}
}

void	ft_equal_var(t_cmd *cmd, char *line)
{
	if (ft_change_var_content(cmd, line)) // Check if content change
	{
		ft_update_var(&env.env_list, line); // Function needs change
		ft_update_var(&env.exp_list, line);
		return ;
	}
	ft_loop_assign_vars(cmd->env_vars, line);
	return ;
}

void	ft_not_equal_var(t_cmd *cmd, char *line)
{
	int	i;

	i = 0;
	while (cmd->env_vars[i])
	{
		if (ft_strncmp(cmd->env_vars[i], line, ft_strlen(line)) == 0)
		{
			ft_add_after(&env.env_list, 15, cmd->env_vars[i]);
			ft_add_after(&env.exp_list, 15, ft_strjoin("declare -x ",
					ft_add_quotes(cmd->env_vars[i])));
			return ;
		}
		i++;
	}
}

void	ft_export(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (cmd->args[1] == NULL || cmd->env_vars == NULL || cmd->env_vars[0] == NULL)
		return ;
	while (cmd->args[i])
	{
		while (cmd->env_vars[j])
		{
			if (ft_strncmp(cmd->args[i], cmd->env_vars[j], ft_varlen(cmd->args[i])) == 0
					&& ft_verify_double(env.env_list, cmd->args[i]) == 0)
			{
				if (ft_verify_equal(cmd->args[i])) // Equal sign found
					ft_equal_var(cmd, cmd->args[i]);
				else
					ft_not_equal_var(cmd, cmd->args[i]);
			}
			j++;
		}
		j = 0;
		i++;
	}
}
