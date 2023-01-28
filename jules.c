/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:49:38 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 18:55:46 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	g_env;

t_cmd	*lst_next_cmd(t_cmd *temp)
{
	t_cmd	*next_cmd;

	temp->piped = 1;
	next_cmd = malloc(sizeof(t_cmd));
	next_cmd->piped = 0;
	next_cmd->redir[0] = 0;
	next_cmd->redir[1] = 0;
	next_cmd->infile = temp->out_pipe[0];
	next_cmd->redir_in = -1;
	next_cmd->outfile = STDOUT_FILENO;
	next_cmd->env_vars = temp->env_vars;
	next_cmd->quoted = temp->quoted;
	next_cmd->space_after = temp->space_after;
	next_cmd->next = NULL;
	next_cmd->args = NULL;
	next_cmd->out_pipe[0] = -1;
	next_cmd->out_pipe[1] = -1;
	next_cmd->heredoc[0] = -1;
	next_cmd->heredoc[1] = -1;
	next_cmd->in_pipe[0] = temp->out_pipe[0];
	next_cmd->in_pipe[1] = temp->out_pipe[1];
	temp->next = next_cmd;
	return (next_cmd);
}

int	token_type(char *token, int quoted)
{
	if (!token)
		return (-1);
	else if (!ft_strncmp(token, "<<", 2))
		return (0);
	else if (!ft_strncmp(token, ">>", 2))
		return (1);
	else if (!ft_strncmp(token, "<", 1))
		return (2);
	else if (!ft_strncmp(token, ">", 1))
		return (3);
	else if (!ft_strncmp(token, "|", 1))
		return (4);
	else if (quoted && token[ft_strlen(token) - 1] == '\"' \
			&& (token[0] == '\"' || (token[0] == '\'' \
			&& token[1] == '\"')) && ft_strlen(token) > 1)
		return (5);
	else if (quoted && token[ft_strlen(token) - 1] == '\'' \
			&& (token[0] == '\'' || (token[0] == '\"' \
			&& token[1] == '\'')) && ft_strlen(token) > 1)
		return (7);
	else
		return (6);
}

void	p_init(t_pars *p, t_cmd *parse_list)
{
	p->i = -1;
	p->str = NULL;
	parse_list->next = NULL;
	parse_list->args = NULL;
}

void	expand_tokens(char **lex_tab, t_cmd *parse_list, t_cmd *temp, t_pars *p)
{
	if (p->type == 6)
		parse_list->env_vars = create_env_vars(ft_strdup(lex_tab[p->i]), \
		parse_list->env_vars, g_env.env_list);
	if (p->type < 7)
		p->str = fuse_quotes(get_env_vars(lex_tab[p->i], \
		parse_list->env_vars, g_env.env_list), lex_tab, parse_list, p->i);
	else
		p->str = fuse_quotes(ft_strdup(lex_tab[p->i]), \
		lex_tab, parse_list, p->i);
	temp->args = token_join(temp->args, p->str);
	p->i += quotes_skip(lex_tab + p->i, parse_list->space_after + p->i);
}

t_cmd	*parsing(char **lex_tab, t_cmd *parse_list)
{
	t_cmd	*temp;
	t_pars	*p;

	p = malloc(sizeof(t_pars));
	temp = parse_list;
	p_init(p, parse_list);
	while (lex_tab[++p->i])
	{
		p->type = token_type(lex_tab[p->i], parse_list->quoted[p->i]);
		if (p->type < 4)
		{
			p->err = redir(temp, lex_tab + p->i, p->type);
			if (p->err <= 0 && redir_err_msg(p, lex_tab[p->i + 1], \
					lex_tab, parse_list))
				return (NULL);
			p->i++;
		}
		else if (p->type == 4)
			temp = lst_next_cmd(temp);
		else
			expand_tokens(lex_tab, parse_list, temp, p);
	}
	free(p);
	return (parse_list);
}
