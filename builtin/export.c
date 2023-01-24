/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/24 15:20:04 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	g_env;

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

// Adds declare -x and quotes
void	ft_get_export(t_env **exp_list)
{
	t_env	*curr;
	char	*tmp;

	tmp = NULL;
	curr = *exp_list;
	if (!curr)
		return ;
	while (curr)
	{
		tmp = ft_add_quotes(curr->line);
		free(curr->line);
		tmp = ft_strjoin("declare -x ", tmp, 2);
		curr->line = malloc(sizeof(char) * (ft_strlen(tmp) + 1));
		ft_strcpy(curr->line, tmp);
		free(tmp);
		if (curr->next == NULL)
			break ;
		curr = curr->next;
	}
}

// Check if there is an existing variable that changed content
int	ft_change_var_content(t_cmd *cmd, char *line)

{
	int	i;

	i = 0;
	while (cmd->env_vars[i])
	{
		if (ft_strncmp(cmd->env_vars[i],
				line, ft_varlen(line)) == 0)
		{
			if (ft_strcmp(cmd->env_vars[i], line) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}

void	ft_loop_assign_vars(char **env_vars, char *line)
{
	int		i;
	char	*exp;

	i = 0;
	exp = NULL;
	exp = ft_add_quotes(line);
	exp = ft_strjoin("declare -x ", exp, 2);
	while (env_vars[i])
	{
		if (ft_strcmp(env_vars[i], line) == 0)
		{
			ft_add_after(&g_env.env_list, 15, line);
			ft_add_queue(&g_env.exp_list, exp);
			free(exp);
			return ;
		}
		i++;
	}
	free(exp);
}

void	ft_equal_var(t_cmd *cmd, char *line)
{
	if (ft_change_var_content(cmd, line))
	{
		ft_update_var(&g_env.env_list, line);
		ft_update_var(&g_env.exp_list, line);
		return ;
	}
	ft_loop_assign_vars(cmd->env_vars, line);
	return ;
}

void	ft_not_equal_var(t_cmd *cmd, char *line)
{
	int		i;
	char	*exp;

	i = 0;
	exp = NULL;
	while (cmd->env_vars[i])
	{
		if (ft_strncmp(cmd->env_vars[i], line, ft_strlen(line)) == 0)
		{
			exp = ft_add_quotes(cmd->env_vars[i]);
			exp = ft_strjoin("declare -x ", exp, 2);
			ft_add_after(&g_env.env_list, 15, cmd->env_vars[i]);
			ft_add_queue(&g_env.exp_list, exp);
			return ;
		}
		i++;
	}
}

void	ft_do_export(char *args, char *env_vars, t_cmd *cmd)
{
	char	*tmp;

	tmp = NULL;
	if (ft_strncmp(args, env_vars, ft_varlen(args)) == 0
		&& ft_verify_double(g_env.env_list, args) == 0)
	{
		if (ft_verify_equal(args))
			ft_equal_var(cmd, args);
		else
			ft_not_equal_var(cmd, args);
	}
	else
	{
		tmp = ft_strjoin("declare -x ", args, 0);
		ft_add_queue(&g_env.exp_list, tmp);
		free(tmp);
	}
}

int	ft_export(t_cmd *cmd)
{
	int		i;
	int		j;
	int		err;
	char	*tmp;

	i = 1;
	j = 0;
	err = 0;
	tmp = NULL;
	// if (cmd->env_vars == NULL || cmd->env_vars[0] == NULL)
	// {
	// 	if (!ft_verify_err_var(cmd->args[i]))
	// 	{
	// 		tmp = ft_strjoin("declare -x ", cmd->args[1], 0);
	// 		ft_add_queue(&g_env.exp_list, tmp);
	// 		free(tmp);
	// 	}
	// 	return (0);
	// }
	while (cmd->args[i])
	{
		//printf("arg is %s\n", cmd->args[i]);
		if (ft_verify_err_var(cmd->args[i]))
		{
			err = 1;
			ft_print_error(ENV_VAR, cmd, cmd->args[i]);
			break ;
		}
		//printf("arg is %s\n", cmd->args[i]);
		if (cmd->env_vars[0] == NULL)
		{
			printf("env null\n");
			tmp = ft_strjoin("declare -x ", cmd->args[1], 0);
			ft_add_queue(&g_env.exp_list, tmp);
			free(tmp);
		}
		while (cmd->env_vars[j])
		{
			printf("arg is %s\n", cmd->args[i]);
			ft_do_export(cmd->args[i], cmd->env_vars[j], cmd);
			j++;
		}
		j = 0;
		i++;
	}
	return (err);
}
