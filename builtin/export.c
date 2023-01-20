/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/20 17:49:47 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

 // Adds declare -x and quotes
void	ft_get_export(t_env **exp_list)
{
	t_env	*curr;
	int		i;

	i = 0;

	curr = *exp_list;
	if (!curr)
		return ;
	while (curr)
	{
		curr->line = ft_add_quotes(curr->line); // 19/01 HERE POSSIBLE LEAKS
		curr->line = ft_strjoin("declare -x ", curr->line, 0); // if other than 0 gives segfault
		curr = curr->next;
	}
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
					ft_add_quotes(line), -1));
			return ;
		}
		i++;
	}
}

void	ft_equal_var(t_cmd *cmd, char *line) // Line is cmd->args[i]
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
					ft_add_quotes(cmd->env_vars[i]), -1));
			return ;
		}
		i++;
	}
}

void	ft_do_export(char *args, char *env_vars, t_cmd *cmd)
{
	if (env_vars == NULL)
		return ;
	if (ft_strncmp(args, env_vars, ft_varlen(args)) == 0 && ft_verify_double(env.env_list, args) == 0)
	{
		if (ft_verify_equal(args)) // Equal sign found
			ft_equal_var(cmd, args);
		else
			ft_not_equal_var(cmd, args);
	}
}

int	ft_export(t_cmd *cmd)
{
	int	i;
	int	j;
	int	err;

	i = 1;
	j = 0;
	err = 0;
	if (cmd->env_vars == NULL || cmd->env_vars[0] == NULL)
		return (0);
	while (cmd->args[i])
	{
		while (cmd->env_vars[j])
		{
			if (ft_verify_err_var(cmd->args[i]))
			{
				err = 1;
				ft_print_error(ENV_VAR, cmd, cmd->args[i]);
				break ;
			}
			ft_do_export(cmd->args[i], cmd->env_vars[j], cmd);
			j++;
		}
		j = 0;
		i++;
	}
	return (err);
}
