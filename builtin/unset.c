/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 17:45:56 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 15:54:45 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

void	ft_remove_line(t_env **env_list, t_env *node)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_list;
	while (curr)
	{
		if (ft_strcmp(curr->line, node->line) == 0)
		{
			prev->next = curr->next;
			free(curr->line);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset_variable(t_env **list, char *s)
{
	t_env	*curr;

	curr = *list;
	if (curr == NULL)
		return (1);
	while (curr)
	{
		if (ft_strncmp(curr->line, s, ft_varlen(s)) == 0
			&& ft_strncmp(curr->line, s, ft_varlen(curr->line)) == 0)
		{
			ft_remove_line(list, curr);
			return (0);
		}
		curr = curr->next;
	}
	return (0);
}

	// if (ft_tablen(env_vars) == 2)
	// {
	// 	if (ft_strncmp(env_vars[0], line, ft_varlen(env_vars[0])) == 0)
	// 	{
	// 		ft_free_char_array(env_vars);
	// 		return (NULL);
	// 	}
	// 	return (env_vars);
	// }
char	**ft_free_env_vars(char **env_vars, char *line)
{
	char	**ret;
	int		i;
	int		j;

	i = -1;
	j = -1;
	ret = NULL;
	if (!env_vars)
		return (NULL);
	while (env_vars[++i])
	{
		if (ft_strncmp(env_vars[i], line, ft_varlen(env_vars[i])) == 0
			&& ft_strncmp(env_vars[i], line, ft_varlen(line)) == 0)
		{
			ret = malloc(sizeof(char *) * ft_tablen(env_vars));
			while (++j < i)
				ret[j] = ft_strdup(env_vars[j]);
			while (env_vars[++j])
				ret[j - 1] = ft_strdup(env_vars[j]);
			ret[j - 1] = NULL;
			ft_free_char_array(env_vars);
			return (ret);
		}
	}
	return (env_vars);
}

int	ft_do_unset(t_cmd *cmd, char *line)
{
	char	*exp;
	int		err;

	exp = NULL;
	err = 0;
	if (ft_verify_err_var(line))
	{
		err = 1;
		ft_print_error(ENV_VAR, cmd, line);
		return (err);
	}
	cmd->env_vars = ft_free_env_vars(cmd->env_vars, line);
	ft_unset_variable(&g_env.env_list, line);
	exp = ft_strjoin("declare -x ", line, -1);
	ft_unset_variable(&g_env.exp_list, exp);
	if (exp)
		free(exp);
	return (0);
}

// Unsets ENV variable
int	ft_unset(t_cmd *cmd)
{
	int		i;
	int		err;

	i = 1;
	err = 0;
	while (cmd->args[i])
	{
		err = ft_do_unset(cmd, cmd->args[i]);
		i++;
	}
	return (err);
}
