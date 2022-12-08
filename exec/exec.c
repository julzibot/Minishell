/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 18:54:53 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/08 15:30:43 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char	*ft_cmd_check(char **envp, char *cmd)
// {
// 	char	*env_line;
// 	char	**paths;
// 	int		i;

// 	i = 0;
// 	if (ft_strcheck(cmd, '/'))
// 		cmd = ft_strrchr(cmd, '/');
// 	while (envp[i])
// 	{
// 		if (ft_strstr(envp[i], "PATH"))
// 			env_line = ft_substr(envp[i], 5);
// 		i++;
// 	}
// 	paths = ft_split(env_line, ':');
// 	i = 0;
// 	while (paths[i])
// 	{
// 		paths[i] = ft_strjoin(paths[i], "/");
// 		paths[i] = ft_strjoin(paths[i], cmd);
// 		if (access(paths[i], F_OK | X_OK) == 0)
// 			return (paths[i]);
// 		i++;
// 	}
// 	perror("Command not found");
// 	exit(127);
// }

// int	ft_exec(char **env, char *cmd) // Execute a command
// {
// 	char	*path;

// 	path = ft_cmd_check(env, cmd);
// }