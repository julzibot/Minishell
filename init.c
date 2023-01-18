/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 11:43:39 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/18 11:44:43 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	env;

void	parse_init(t_cmd *parse_list, char **envp, char **env_vars)
{
	(void)envp;
	env.gl = 0;
	env.error_code = 0;
	parse_list->env_vars = env_vars;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
	parse_list->env_list = env.env_list;
	parse_list->exp_list = NULL;
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
	parse_list->cmd_done = 0;
	parse_list->term = NULL;
}

void	ft_init_env(char **envp)
{
	ft_get_env(&env.env_list, envp); // For env command
	ft_get_env(&env.exp_list, envp); // For export command
	ft_get_export(&env.exp_list);    // Declare -x PWD="somewhere/nice/and/cozy"
}

void	ft_init_termios(struct termios *term)
{
	term = malloc(sizeof(struct termios));
	if (!term)
		exit(1);
	tcgetattr(STDOUT_FILENO, term);
	term->c_lflag = term->c_lflag ^ ECHOCTL;
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, term);
}