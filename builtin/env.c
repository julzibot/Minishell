/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:58 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 16:29:07 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

// Adds a variable to env
// t_gl_env **env_list - environment variable list
// int line_nb      - line after which we add new variable (line)
// char *s          - line to add

void	ft_get_env(t_env **env_list, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "OLDPWD=", 6) == 0)
		{
			ft_add_after(env_list, 3, "OLDPWD=");
			if (env[i + 1] == NULL)
				return ;
			else
				i++;
		}
		else
			ft_add_queue(env_list, env[i]);
		i++;
	}
}

void	ft_get_exp(t_env **exp_list, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_arrlen(env);
	while (i < len - 1)
	{
		if (ft_strncmp(env[i], "OLDPWD=", 6) == 0)
		{
			ft_add_queue(exp_list, "OLDPWD=");
			if (env[i + 1] == NULL)
				return ;
			else
				i++;
		}
		if (ft_strncmp(env[i], "_=", 2) == 0)
			i++;
		ft_add_queue(exp_list, env[i]);
		i++;
	}
}

void	ft_do_update_env(char *line)
{
	char	*exp;

	exp = ft_add_quotes(line);
	exp = ft_strjoin("declare -x ", exp, 2);
	ft_update_var(&g_env.env_list, line);
	ft_update_var(&g_env.exp_list, exp);
	if (exp)
		free(exp);
}

void	ft_update_env(char *line)
{
	char	*cleaned;

	cleaned = NULL;
	if (ft_simple_equal(line))
		return ;
	if (ft_find_quotes(line))
		cleaned = ft_clean_var(line);
	if (cleaned)
		ft_do_update_env(cleaned);
	else
		ft_do_update_env(line);
	if (cleaned)
		free(cleaned);
}

int	ft_print_env(t_env *env_lst, t_cmd *cmd)
{
	t_env	*curr;
	int		fd;

	fd = cmd->outfile;
	curr = env_lst;
	if (cmd->args[1] != NULL)
	{
		ft_print_error(CMD, cmd, NULL);
		return (1);
	}
	if (cmd->redir[1] == 0 && cmd->piped == 1)
		fd = cmd->out_pipe[1];
	if (curr == NULL)
		return (1);
	while (curr)
	{
		ft_printf(fd, "%s\n", curr->line);
		curr = curr->next;
	}
	return (0);
}
