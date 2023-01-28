/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:04 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 18:20:40 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

// Check if there is an existing variable that changed content
// 3 change exp only
// 2 change both
int	ft_change_var_content(char *line)
{
	int		ret;
	t_env	*curr;

	ret = 0;
	curr = g_env.env_list;
	ret = ft_verify_exp_var(line);
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(line)) == 0
			&& ft_strncmp(curr->line, line, ft_varlen(line)) == 0)
			return (2);
		curr = curr->next;
	}
	return (ret);
}

void	ft_add_to_both(char *line)
{
	char	*var;

	var = NULL;
	if (ft_change_var_content(line) == 3)
	{
		var = ft_add_quotes(line);
		var = ft_strjoin("declare -x ", var, 2);
		if (ft_simple_equal(line) == 0)
			ft_add_after(&g_env.env_list, 15, line);
		ft_update_var(&g_env.exp_list, var);
		if (var)
			free(var);
		return ;
	}
	ft_add_after(&g_env.env_list, 15, line);
	var = ft_add_quotes(line);
	var = ft_strjoin("declare -x ", var, 2);
	ft_add_queue(&g_env.exp_list, var);
	if (var)
		free(var);
}

void	ft_equal_var(char *line)
{
	char	*cleaned;

	cleaned = NULL;
	if (ft_find_quotes(line))
		cleaned = ft_clean_var(line);
	else
		ft_add_to_both(line);
	if (cleaned)
		ft_add_to_both(cleaned);
	return ;
}

void	ft_not_equal_var(t_cmd *cmd, char *line)
{
	char	*exp;

	(void)cmd;
	exp = ft_add_quotes(line);
	exp = ft_strjoin("declare -x ", exp, 2);
	ft_add_queue(&g_env.exp_list, exp);
	if (exp)
		free(exp);
}

int	ft_export(t_cmd *cmd)
{
	int		i;
	int		err;

	i = 0;
	err = 0;
	while (cmd->args[++i])
	{
		if (ft_verify_err_var(cmd->args[i]))
		{
			err = ft_print_error(ENV_VAR, cmd, cmd->args[i]);
			break ;
		}
		if (ft_verify_double(cmd->args[i]) == 0)
		{
			if (ft_verify_equal(cmd->args[i]))
				ft_equal_var(cmd->args[i]);
			else
				ft_not_equal_var(cmd, cmd->args[i]);
		}
		else if (ft_verify_double(cmd->args[i]) == 2)
			ft_update_env(cmd->args[i]);
	}
	return (err);
}
