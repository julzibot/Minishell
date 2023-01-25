/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 09:37:00 by mstojilj         ###   ########.fr       */
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

void	ft_export_only(char *var)
{
	char	*tmp;

	tmp = NULL;
	tmp = ft_strjoin("declare -x ", var, 0);
	ft_add_queue(&g_env.exp_list, tmp);
	free(tmp);
}

int	ft_export(t_cmd *cmd)
{
	int		i;
	int		j;
	int		err;

	i = 0;
	j = -1;
	err = 0;
	while (cmd->args[++i])
	{
		if (ft_verify_err_var(cmd->args[i]))
		{
			err = ft_print_error(ENV_VAR, cmd, cmd->args[i]);
			break ;
		}
		if (cmd->env_vars == NULL || cmd->env_vars[0] == NULL)
			ft_export_only(cmd->args[i]);
		while (cmd->env_vars && cmd->env_vars[++j])
			ft_do_export(cmd->args[i], cmd->env_vars[j], cmd);
		j = 0;
	}
	return (err);
}
