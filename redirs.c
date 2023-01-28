/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 19:58:43 by jibot             #+#    #+#             */
/*   Updated: 2023/01/25 19:58:48 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	g_env;

int	redir_err_msg(t_pars *p, char *filename, char **lex_tab, t_cmd *parse_list)
{
	if (!p->err)
	{
		printf("Error : this redirection is chelou !\n");
		g_env.error_code = 258;
		free_list(parse_list, lex_tab);
		free(p);
		return (1);
	}
	else
	{
		if (p->err == -1)
			printf("Error : %s does nos exist !\n", filename);
		if (p->err == -2 || p->err == -3)
			printf("Error : %s : permission denied\n", filename);
		g_env.error_code = 1;
	}
	return (0);
}

int	in_out_redir(t_cmd *cmd, char *filename_delim, int type)
{
	if (type == 2)
	{
		if (access(filename_delim, F_OK) == -1)
			return (-1);
		if (!access(filename_delim, F_OK) && access(filename_delim, R_OK) == -1)
			return (-2);
		cmd->redir_in = open(filename_delim, O_RDONLY, 0644);
		cmd->redir[0] = 1;
	}
	else
	{
		if (!access(filename_delim, F_OK) && access(filename_delim, W_OK) == -1)
			return (-3);
		if (type == 1)
			cmd->outfile = open(filename_delim, \
				O_CREAT | O_RDWR | O_APPEND, 0644);
		if (type == 3)
			cmd->outfile = open(filename_delim, \
				O_CREAT | O_RDWR | O_TRUNC, 0644);
		cmd->redir[1] = 1;
	}
	return (1);
}

int	redir(t_cmd *cmd, char **redir_ptr, int type)
{
	char	*filename_delim;

	filename_delim = redir_ptr[1];
	if (!filename_delim || !ft_strcmp("|", filename_delim))
		return (0);
	if (type % 2 == 0 && cmd->redir[0])
		close(cmd->redir_in);
	else if (type % 2 == 1 && cmd->redir[1])
		close (cmd->outfile);
	if (!type)
	{
		if (!heredoc_handle(cmd, filename_delim))
			return (0);
		return (1);
	}
	else
		return (in_out_redir(cmd, filename_delim, type));
}
