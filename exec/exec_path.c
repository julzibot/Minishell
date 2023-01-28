/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 19:53:36 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/27 19:58:54 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

void	ft_free_paths(char **paths)
{
	int	i;

	i = 0;
	if (!paths)
		return ;
	while (i < ft_arrlen(paths))
	{
		if (paths[i] == NULL)
			break ;
		if (paths[i])
			free(paths[i]);
		i++;
	}
	free(paths);
}

char	*ft_path_check(char **paths, char *path, int option)
{
	ft_free_paths(paths);
	if (option == 0)
	{
		free(path);
		return ("ok");
	}
	return (path);
}

char	*ft_get_gl_env_path(void)
{
	char	*env_line;
	t_env	*curr;

	env_line = NULL;
	curr = g_env.env_list;
	while (curr)
	{
		if (curr == NULL)
			return (NULL);
		if (ft_strncmp(curr->line, "PATH", 3) == 0)
		{
			env_line = ft_substr(curr->line, 5);
			return (env_line);
		}
		curr = curr->next;
	}
	return (NULL);
}
