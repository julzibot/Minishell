/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:48:58 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/21 14:07:37 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Adds a variable to env
// t_env **env_list - environment variable list
// int line_nb      - line after which we add new variable (line)
// char *s          - line to add
void	ft_add_queue(t_env **root, char *s)
{
	t_env	*node;
	t_env	*curr;

	curr = *root;
	node = malloc(sizeof(t_env));
	node->next = NULL;
	node->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
	ft_strcpy(node->line, s);
	if (*root == NULL || root == NULL)
	{
		*root = node;
		return ;
	}
	while (curr)
	{
		if (curr->next == NULL)
		{
			curr->next = node;
			return ;
		}
		curr = curr->next;
	}
	curr->next = node;
}

void	ft_get_env(t_env **env_list, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_add_queue(env_list, env[i]);
		i++;
	}
	env[i] = NULL;
}

void	ft_get_exp(t_env **exp_list, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_arrlen(env);
	while (i < len - 1)
	{
		if (ft_strncmp(env[i], "_=", 2) == 0)
			i++;
		ft_add_queue(exp_list, env[i]);
		i++;
	}
	env[i] = NULL;
}

void	ft_update_env(t_env **env_list, t_env **exp_list, char *line)
{
	if (ft_verify_double(*env_list, line) == 1 || ft_verify_equal(line) == 1)
		return ;
	line = ft_verify_env_var(line);
	if (line == NULL)
		return ;
	ft_update_var(env_list, line);
	ft_update_var(exp_list, ft_strjoin("declare -x ", ft_add_quotes(line), -1));
}

int	ft_print_env(t_env *env_lst, t_cmd *cmd)
{
	t_env	*curr;
	int		i;
	int		fd;

	i = 0;
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
		ft_printf(fd, "%d - %s\n", i, curr->line);
		i++;
		curr = curr->next;
	}
	return (0);
}
