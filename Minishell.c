/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:58:48 by jibot             #+#    #+#             */
/*   Updated: 2023/01/04 18:35:22 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_init(t_cmd *parse_list)
{
	if (!parse_list)
		exit(1);
	parse_list->env_vars = NULL;
	parse_list->quoted = NULL;
	parse_list->space_after = NULL;
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

	/*Milan 16/12*/
	t_env			 	*env_list;
	t_env				*exp_list;
	
	env_list = NULL;
	exp_list = NULL;
	ft_get_env(&env_list, envp); // For env command
	ft_get_env(&exp_list, envp); // For export command
	ft_get_export(&exp_list);    // Declare -x PWD="somewhere/nice/and/cozy"
	/*Milan 16/12*/
	parse_list = malloc(sizeof(t_cmd));
	parse_init(parse_list);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		line = readline(PROMPT);
		check_line_exists(line);
		add_history(line);
		tokens = lexing(line, parse_list);
		parse_list = parsing(tokens, parse_list);
		ft_exec_cmd(parse_list, &env_list, &exp_list, envp);
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