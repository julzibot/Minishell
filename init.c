/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 11:43:39 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 13:59:41 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	g_env;

void	parse_init(t_cmd *parse_list, char **env_vars)
{
	parse_list->args = NULL;
	parse_list->env_vars = env_vars;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
	parse_list->infile = STDIN_FILENO;
	parse_list->outfile = STDOUT_FILENO;
	parse_list->next = NULL;
	parse_list->piped = 0;
	parse_list->redir[0] = 0;
	parse_list->redir[1] = 0;
	parse_list->out_pipe[0] = -1;
	parse_list->out_pipe[1] = -1;
	parse_list->heredoc[0] = -1;
	parse_list->heredoc[1] = -1;
	parse_list->in_pipe[0] = -1;
	parse_list->in_pipe[1] = -1;
	parse_list->redir_in = -1;
}

void	ft_init_empty_envp(void)
{
	char	*cwd;
	char	*var;
	char	*quo;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	var = ft_strjoin("PWD=", cwd, 0);
	if (cwd)
		free(cwd);
	ft_add_queue(&g_env.env_list, "OLDPWD=");
	ft_add_queue(&g_env.env_list, var);
	ft_add_queue(&g_env.exp_list, "OLDPWD=\"\"");
	quo = ft_add_quotes(var);
	if (var)
		free(var);
	ft_add_queue(&g_env.exp_list, quo);
	if (quo)
		free(quo);
}

void	ft_init_env(char **envp)
{
	g_env.gl = 0;
	g_env.error_code = 0;
	g_env.curr_pwd = NULL;
	g_env.old_pwd = NULL;
	if (envp[0] == NULL)
	{
		ft_init_empty_envp();
		return ;
	}
	g_env.abs_path = getenv("HOME");
	ft_get_env(&g_env.env_list, envp);
	ft_get_exp(&g_env.exp_list, envp);
	ft_get_export(&g_env.exp_list);
	ft_sort_list(&g_env.exp_list);
}
