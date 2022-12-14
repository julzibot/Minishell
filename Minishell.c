/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2022/12/14 16:37:40 by mstojilj         ###   ########.fr       */
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
	t_cmd 	*parse_list;

	/*Milan 10/12*/
	t_env 	*env_list;
	t_env	*exp_list;
	
	env_list = NULL;
	exp_list = NULL;
	ft_get_env(&env_list, envp); // For env command
	ft_get_env(&exp_list, envp); // For export command
	ft_get_export(&exp_list);   // Declare -x PWD="somewhere/nice/and/cozy"
	/*Milan 10/12*/

	parse_list = malloc(sizeof(t_cmd));
	if (!parse_list)
		exit(1);
	parse_list->env_vars = NULL;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
	while (1)
	{
		line = readline("Mini_chelou: ");
		add_history(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		ft_exec_cmd(parse_list, &env_list, &exp_list, envp); /*Milan 10/12*/
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
