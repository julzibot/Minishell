/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/06 17:31:01 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_init(t_cmd *parse_list, char **envp)
{
	parse_list->env_vars = NULL;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
	parse_list->env_list = NULL;
	parse_list->exp_list = NULL;
	ft_get_env(&parse_list->env_list, envp); // For env command
	ft_get_env(&parse_list->exp_list, envp); // For export command
	ft_get_export(&parse_list->exp_list);    // Declare -x PWD="somewhere/nice/and/cozy"
}

void	check_line_exists(char *line)
{
	if (line == NULL) // CTRL-D
		{
			rl_redisplay();
			ft_printf(1, "\x1b[A");
			ft_printf(1, "exit\n");
			exit(0);
		}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char				*line;
	char				**tokens;
	t_cmd 				*parse_list;

	parse_list = malloc(sizeof(t_cmd));
	if (!parse_list)
		exit(1);
	parse_init(parse_list, envp);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		line = readline(PROMPT);
		check_line_exists(line);
		add_history(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		ft_exec_cmd(parse_list, envp);
		printf("enters\n");
		ft_print_env(parse_list->env_list);
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
	
// 	parse_list = malloc(sizeof(t_cmd));
// 	if (!parse_list)
// 		exit(1);
// 	parse_list->env_vars = NULL;
// 	parse_list->quoted = NULL;
// 	parse_list->space_after = NULL;
// 	while (1)
// 	{
// 		line = readline(PROMPT);
// 		tokens = lexing(line, parse_list);
// 		parse_list = parsing(tokens, parse_list);
// 	}
// 	return (0);
// }