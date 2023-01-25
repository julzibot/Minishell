/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 12:21:39 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 12:22:25 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*ft_get_env_path(char **envp, int i)
{
	char	*env_line;

	env_line = NULL;
	while (envp && envp[++i])
	{
		if (ft_strstr(envp[i], "PATH"))
		{
			env_line = ft_substr(envp[i], 5);
			return (env_line);
		}
	}
	return (NULL);
}

char	*ft_cmd_check(char **envp, char *cmd, int option)
{
	char	*env_line;
	char	**paths;
	int		i;

	i = -1;
	env_line = NULL;
	if (access(cmd, F_OK | X_OK) == 0)
		return (cmd);
	env_line = ft_get_env_path(envp, i);
	if (!env_line)
		return (NULL);
	paths = ft_split(env_line, ':');
	free(env_line);
	i = -1;
	while (paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/", 1);
		paths[i] = ft_strjoin(paths[i], cmd, 1);
		if (access(paths[i], F_OK | X_OK) == 0)
			return (ft_path_check(paths, ft_strdup(paths[i]), option));
	}
	ft_free_paths(paths);
	return (NULL);
}
