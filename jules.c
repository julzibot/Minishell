/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:49:38 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/22 15:36:14 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern g_t_env	env;

int	check_lexpr_error(char *line, t_lex *l)
{
	if (l->delim == 2 && !line[l->i + 1])
		return (1);
	else if (l->delim == 2 && !l->i)
		return (2);
	else if (l->delim == 3 && \
		((is_delim(line[l->i + 1]) == 3 && line[l->i + 1] != line[l->i]) \
		|| (line[l->i] == line[l->i + 1] && is_delim(line[l->i + 2]) == 3)))
		return (3);
	return (0);
}

int	syntax_err(t_lex *l, int lexpr)
{
	if (lexpr || l->i < 0)
	{
		l->lex_tab = NULL;
		if (lexpr == 1)
			printf("Error : input ends with a pipe\n");
		else if (lexpr == 2)
			printf("Error : pipe at start of input is CHELOU !\n");
		else if (lexpr == 3)
			printf("Error : this redirection is chelou !\n");
		else if (l->i == -1)
			printf("Error : unclosed quotes !\n");
		else if (l->i == -2)
			printf("Error : chelou input detected\n");
		l->error = 1;
	}
	if (l->error)
		env.error_code = 258;
	return (l->error);
}

int	lex_pr(t_lex *l, char *line, t_cmd *parse_list)
{
	int	lexpr;

	lexpr = check_lexpr_error(line, l);
	if (syntax_err(l, lexpr))
		return (1);
	l->i += lex_pipe_redir(line + l->i, l->lex_tab + l->j++);
	if (is_delim(line[l->i]) != 4 /*is not a space or tab*/)
		l->i--;
	else
		parse_list->space_after[l->j] = 1;
	return (0);
}

int	lex_qw(t_lex *l, char *line, t_cmd *parse_list)
{
	parse_list->quoted[l->j] = 0;
	if (l->delim == 1)
		parse_list->quoted[l->j] = 1;
	l->i = lineseg(line, l->i, l->lex_tab + l->j, parse_list->quoted[l->j]);
	if (syntax_err(l, 0))
		return (1);
	if (is_delim(line[l->i]) != 4 /*is not a space or tab*/)
		l->i--;
	else
		parse_list->space_after[l->j] = 1;
	l->j++;
	return (0);
}

char	**lexing(char *line, t_cmd *parse_list)
{
	t_lex	*l;
	char	**ret;

	if (line == NULL)
		return (NULL);
	l = malloc(sizeof(t_lex));
	l->i = 0;
	l->j = 0;
	l->error = 0;
	l->delim = -1;
	l->lex_tab = malloc(sizeof(char*) * arg_count(line));
	tab_list_init(l->lex_tab, line, parse_list);
	while (line[l->i])
	{
		l->delim = is_delim(line[l->i]);
		if (((l->delim == 2 || l->delim == 3) && lex_pr(l, line, parse_list)) \
			|| ((!l->delim || l->delim == 1) && lex_qw(l, line, parse_list)))
				return (l->lex_tab);
		l->i++;
	}
	l->lex_tab[l->j] = NULL;
	ret = ft_tabdup(l->lex_tab, 1);
	free(l);
	return (ret);
}

t_cmd	*lst_next_cmd(t_cmd *temp)
{
	t_cmd	*next_cmd;

	temp->piped = 1;
	next_cmd = malloc(sizeof(t_cmd));
	next_cmd->piped = 0;
	next_cmd->infile = temp->out_pipe[0];
	next_cmd->outfile = STDOUT_FILENO;
	next_cmd->redir[0] = 0;
	next_cmd->redir[1] = 0;
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
	next_cmd->redir_in = -1;
	next_cmd->cmd_done = 0;
	temp->next = next_cmd;

	return (next_cmd);
}

int	heredoc_handle(t_cmd *cmd, char *filename_delim)
{
	char	*line;
	
	signal(SIGINT, &ft_handle_sigint); // CTRL-C
	signal(SIGQUIT, SIG_IGN); // CTRL-/
	if (cmd->redir_in != -1)
		close(cmd->redir_in);
	if (pipe(cmd->heredoc) == -1)
		return (0);
	cmd->redir_in = cmd->heredoc[0];
	line = readline("> ");
	if (line == NULL)
	{
		free(filename_delim);
		close(cmd->heredoc[1]);
		return (1);
	}
	while (ft_strcmp(line, filename_delim))
	{
		if (line == NULL)
		{
			free(filename_delim);
			close(cmd->heredoc[1]);
			return (1);
		}
		ft_printf(cmd->heredoc[1], "%s\n", line);
		free(line);
		line = readline("> ");
	}
	if (line)
		free(line);
	if (filename_delim)
		free(filename_delim);
	close(cmd->heredoc[1]);
	return (1);
}

int	redir(t_cmd *cmd, char **redir_ptr, int type)
{
	char	*filename_delim;

	filename_delim = redir_ptr[1];
	if (!filename_delim)
		return (0);
	if (type % 2 == 1 && cmd->outfile != STDOUT_FILENO)
		close (cmd->outfile);
	cmd->redir[type % 2] = 1;
	
	if (!type && !heredoc_handle(cmd, filename_delim))
		return (0);
	else if (type == 2)
	{
		if (cmd->redir_in != -1)
			close(cmd->redir_in);
		cmd->redir_in = open(filename_delim, O_RDONLY, 0644);
	}
	else if (type == 1)
		cmd->outfile = open(filename_delim, O_CREAT | O_RDWR| O_APPEND, 0644);
	else if (type == 3)
		cmd->outfile = open(filename_delim, O_CREAT | O_RDWR | O_TRUNC, 0644);
	return (1);
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
			&& (token[0] == '\"' || (token[0] == '\'' && token[1] == '\"')) && ft_strlen(token) > 1)
		return (5);
	else if (quoted && token[ft_strlen(token) - 1] == '\'' \
			&& (token[0] == '\'' || (token[0] == '\"' && token[1] == '\'')) && ft_strlen(token) > 1)
		return (7);
	else
		return (6);
}

t_cmd	*parsing(char **lex_tab, t_cmd *parse_list)
{
	t_cmd	*temp;
	int		i;
	int		type;
	char	*str;

	// i = -1;
	// while (lex_tab[++i])
	// 	printf("lex %d : %s\n", i, lex_tab[i]);
	
	// INIT
	i = -1;
	str = NULL;
	parse_list->next = NULL;
	parse_list->args = NULL;
	temp = parse_list;
	
	// EXPANSION
	while (lex_tab[++i])
	{
		type = token_type(lex_tab[i], parse_list->quoted[i]);
		if (type < 4)
		{
			if (redir(temp, lex_tab + i, type))
				i++;
			else
			{
				printf("Error : this redirection is chelou !\n");
				env.error_code = 258;
				return (NULL);
			}
		}
		else if (type == 4)
			temp = lst_next_cmd(temp);
		else
		{
			if (type == 6)
				parse_list->env_vars = create_env_vars(ft_strdup(lex_tab[i]), parse_list->env_vars, env.env_list);
			if (type < 7)
			{
				str = fuse_quotes(get_env_vars(lex_tab[i], parse_list->env_vars, env.env_list), lex_tab, parse_list, i);
			}
			else
				str = fuse_quotes(ft_strdup(lex_tab[i]), lex_tab, parse_list, i);
			temp->args = token_join(temp->args, str);
			i += quotes_skip(lex_tab + i, parse_list->space_after + i);
		}
		// if (str != NULL)
		// 	free(str);
	}



	// temp = parse_list;
	// while (temp->next != NULL)
	// {
	// 	printf("%p\n", temp);
	// 	i = -1;
	// 	while (temp->args && temp->args[++i])
	// 		printf("%s\n", temp->args[i]);
	// 	temp = temp->next;
	// }
	// printf("%p\n", temp);
	// i = -1;
	// while (temp->args && temp->args[++i])
	// 	printf("%s\n", temp->args[i]);
		
	// printf("\n");
	// i = -1;
	// while (parse_list->env_vars && parse_list->env_vars[++i])
	// 	printf("env_var %d : %s\n", i, parse_list->env_vars[i]);

	return(parse_list);
}

	// TEST PRINTS
	// temp = parse_list;
	// while (temp->next != NULL)
	// {
	// 	printf("%p\n", temp);
	// 	i = -1;
	// 	while (temp->args && temp->args[++i])
	// 		printf("%s\n", temp->args[i]);
	// 	// printf("%d %d %d\n", temp->redir_in, temp->out_pipe[1], temp->out_pipe[0]);
	// 	temp = temp->next;
	// }
	// printf("%p\n", temp);
	// i = -1;
	// while (temp->args && temp->args[++i])
	// 	printf("%s\n", temp->args[i]);
	// // printf("%d %d %d\n", temp->redir_in, temp->out_pipe[1], temp->out_pipe[0]);
	// printf("\n");
	
	// i = -1;
	// while (parse_list->env_vars && parse_list->env_vars[++i])
	// 	printf("env_var %d : %s\n", i, parse_list->env_vars[i]);