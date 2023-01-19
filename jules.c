/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:49:38 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/19 16:45:12 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_gl_env	env;

char	**lexing(char *line, t_cmd *parse_list)
{
	int		i;
	int 	j;
	char	**lex_tab;

	i = -1;
	j = 0;
	lex_tab = malloc(sizeof(char*) * arg_count(line));
	tab_list_init(line, lex_tab, parse_list);
	if (line == NULL)
		return (NULL);
	printf ("malloc : %d\n", arg_count(line) + 1);
	while (line[++i])
	{
		if (is_delim(line[i]) == 2 || is_delim(line[i]) == 3/*is pipe or redir*/)
		{
			i += lex_pipe_redir(line + i, lex_tab + j++);
			if (is_delim(line[i]) != 4 /*is not a space or tab*/)
				i--;
			else
				parse_list->space_after[j] = 1;
		}
		else if (is_delim(line[i]) == 1 || !is_delim(line[i]) /*is quote, or beginning of word*/)
		{
			parse_list->quoted[j] = 0;
			if (is_delim(line[i]) == 1)
				parse_list->quoted[j] = 1;
			i = lineseg(line, i, lex_tab + j, parse_list->quoted[j]);
			if (i < 0)
			{
				lex_tab = NULL;
				if (i == -1)
					printf("Error : unclosed quotes !\n");
				else if (i == -2)
					printf("Error : chelou input detected\n");
				return (lex_tab);
			}
			if (is_delim(line[i]) != 4 /*is not a space or tab*/)
				i--;
			else
				parse_list->space_after[j] = 1;
			j++;
		}
	}
	printf ("after : %d\n", j);
	lex_tab[j] = NULL;
	return (lex_tab);
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

int	redir(t_cmd *cmd, char **redir_ptr, int type)
{
	char	*filename_delim;
	char	*line;

	if (type % 2 == 1 && cmd->outfile != STDOUT_FILENO)
		close (cmd->outfile);
	cmd->redir[type % 2] = 1;
	filename_delim = redir_ptr[1];
	if (!filename_delim)
		return (0); // error handling here
	line = NULL;
	

	if (!type)
	{
		if (cmd->redir_in != -1)
			close(cmd->redir_in);
		if (pipe(cmd->heredoc) == -1)
			return (0);
		cmd->redir_in = cmd->heredoc[0];
		line = readline("> ");
		while (ft_strcmp(line, filename_delim))
		{
			ft_printf(cmd->heredoc[1], "%s\n", line);
			line = readline("> ");
		}
		close(cmd->heredoc[1]);
	}
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

	if (lex_tab == NULL)
		return (parse_list);
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
			i += redir(temp, lex_tab + i, type);
		else if (type == 4)
			temp = lst_next_cmd(temp);
		else
		{
			if (type == 6)
				parse_list->env_vars = create_env_vars(ft_strdup(lex_tab[i]), parse_list->env_vars, env.env_list);
			if (type < 7)
				str = fuse_quotes(get_env_vars(lex_tab[i], parse_list->env_vars, env.env_list), lex_tab, parse_list, i);
			else
				str = fuse_quotes(ft_strdup(lex_tab[i]), lex_tab, parse_list, i);
			temp->args = token_join(temp->args, str);
			i += quotes_skip(lex_tab + i, parse_list->space_after + i);
		}
	}
	
	// TEST PRINTS
	temp = parse_list;
	while (temp->next != NULL)
	{
		printf("%p\n", temp);
		i = -1;
		while (temp->args && temp->args[++i])
			printf("%s\n", temp->args[i]);
		// printf("%d %d %d\n", temp->redir_in, temp->out_pipe[1], temp->out_pipe[0]);
		temp = temp->next;
	}
	printf("%p\n", temp);
	i = -1;
	while (temp->args && temp->args[++i])
		printf("%s\n", temp->args[i]);
	// printf("%d %d %d\n", temp->redir_in, temp->out_pipe[1], temp->out_pipe[0]);
	printf("\n");
	
	i = -1;
	while (parse_list->env_vars && parse_list->env_vars[++i])
		printf("env_var %d : %s\n", i, parse_list->env_vars[i]);
	return(parse_list);	
}