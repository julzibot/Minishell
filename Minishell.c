/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2022/12/08 16:09:43 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*line;
	char	**tokens;
	t_cmd *parse_list;
	
	parse_list = malloc(sizeof(t_cmd));
	if (!parse_list)
		exit(1);
	parse_list->env_vars = NULL;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
	while (1)
	{
		line = readline("Mini_chiale> ");
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	(void)envp;
// 	char	*line;
// 	char	**tokens;
// 	t_cmd *parse_list;
	
// 	while (1)
// 	{
// 		line = readline("Mini_chiale> ");
// 		tokens = lexing(line);
// 		parse_list = parsing(tokens);
// 	}
// 	return (0);
// }
