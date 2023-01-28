/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 12:21:39 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 18:36:27 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

char	*ft_substr(char *s, unsigned int start)
{
	char				*sub;
	unsigned int		i;
	unsigned int		j;

	i = start;
	j = 0;
	while (s[start + j] && start + j < (unsigned int)ft_strlen(s))
		j++;
	sub = malloc(j * sizeof(char) + 1);
	if (!sub)
	{
		free(sub);
		return (NULL);
	}
	j = 0;
	while (s[i])
	{
		sub[j] = s[i];
		j++;
		i++;
	}
	sub[j] = '\0';
	return (sub);
}

int	ft_if_exec(char *cmd)
{
	if (!ft_strncmp(cmd, "./", 2))
		return (1);
	return (0);
}

char	*ft_access_check(char *cmd, char *env_line)
{
	if (access(cmd, X_OK) == 0)
	{
		if (env_line)
			free(env_line);
		return (cmd);
	}
	if (!access(cmd, F_OK) && ft_if_exec(cmd))
	{
		if (env_line)
			free(env_line);
		return (cmd);
	}
	return (NULL);
}

char	*ft_cmd_check(t_cmd *pl, char *cmd, int option)
{
	char	*env_line;
	char	**paths;
	int		i;

	i = -1;
	env_line = ft_get_gl_env_path();
	if (ft_access_check(cmd, env_line))
		return (cmd);
	if (!env_line)
	{
		ft_print_error(99, pl, NULL);
		return (NULL);
	}
	paths = ft_split(env_line, ':');
	free(env_line);
	while (paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/", 1);
		paths[i] = ft_strjoin(paths[i], cmd, 1);
		if (access(paths[i], F_OK | X_OK) == 0)
			return (ft_path_check(paths, ft_strdup(paths[i]), option));
	}
	ft_free_paths(paths);
	ft_print_error(NOT_CMD, pl, NULL);
	return (NULL);
}
