/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 18:58:10 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/27 17:07:32 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

void	ft_free_char_array(char **s)
{
	int	i;

	i = -1;
	if (!s)
		return ;
	while (s[++i])
		free(s[i]);
	free(s);
}

void	ft_free_exit(t_cmd *parse_list, char **tokens)
{
	t_cmd	*temp;

	if (parse_list->env_vars)
		ft_free_char_array(parse_list->env_vars);
	if (parse_list->space_after)
		free(parse_list->space_after);
	if (parse_list->quoted)
		free(parse_list->quoted);
	while (parse_list)
	{
		if (parse_list && parse_list->args)
			ft_free_char_array(parse_list->args);
		temp = parse_list->next;
		if (parse_list)
			free(parse_list);
		parse_list = temp;
	}
	if (tokens)
		ft_free_char_array(tokens);
}

void	ft_exit(t_cmd *cmd, int error_code, char **tokens)
{
	t_cmd	*temp;

	temp = NULL;
	if (cmd && cmd->args && cmd->args[1] != NULL)
	{
		if (cmd->args[2] != NULL)
		{
			ft_printf(2, "Mini_chelou: ");
			ft_printf(2, "exit: too many arguments\n");
			return ;
		}
		if (ft_is_digit(cmd->args[1]))
		{
			ft_printf(2, "Mini_chelou: ");
			ft_printf(2, "exit: %s: numeric argument required\n", cmd->args[1]);
			return ;
		}
		error_code = ft_atoi(cmd->args[1]);
		error_code %= 256;
	}
	ft_free_exit(cmd, tokens);
	exit(error_code);
}
