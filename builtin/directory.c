/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 18:50:02 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/22 13:10:41 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	env;

int	ft_update_pwd(t_env **exp_list, t_env **env_list, char *env)
{
	char	*s;
	char	*var;

	s = NULL;
	var = NULL;
	s = getcwd(s, 0);
	if (!s)
		return (1);
	var = ft_strjoin(env, s, 0);
	ft_update_env(env_list, exp_list, var);
	free(s);
	free(var);
	return (0);
}

int	ft_cd_error(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->args[1], O_DIRECTORY);
	if (access(cmd->args[1], F_OK) != 0)
	{
		close(fd);
		ft_printf(2, "Mini_chelou: ");
		ft_printf(2, "cd: %s: No such file or directory\n", cmd->args[1]);
		return (1);
	}
	if (fd == -1)
	{
		close(fd);
		ft_printf(2, "Mini_chelou: ");
		ft_printf(2, "cd: %s: Permission denied\n", cmd->args[1]);
		return (1);
	}
	return (1);
}

int	ft_cd(t_cmd *cmd)
{
	int		ret;
	char	*s;

	ret = 0;
	if (ft_update_pwd(&env.exp_list, &env.env_list, "OLDPWD="))
		return (0);
	if (cmd->args[1] == NULL || ft_strcmp(cmd->args[1], "~") == 0)
	{
		// s = malloc(sizeof(char) * (PATH_MAX - 1));
		// if (!s)
		// 	exit(1);
		s = getenv("HOME");
		ret = chdir(s);
		// free(s);
	}
	else if (ft_strcmp(cmd->args[1], "..") == 0 && cmd->args[2] == NULL)
		ret = chdir("..");
	else if (cmd->args[1] != NULL)
		ret = chdir(cmd->args[1]);
	if (ret == -1)
		return (ft_cd_error(cmd));
	ft_update_pwd(&env.exp_list, &env.env_list, "PWD=");
	return (0);
}

char	*ft_pwd(t_cmd *cmd)
{
	char	*cwd;
	int		fd;

	cwd = NULL;
	fd = cmd->outfile;
	if (cmd->redir[1] == 0 && cmd->piped == 1)
		fd = cmd->out_pipe[1];
	cwd = getcwd(cwd, 0);
	if (cwd == NULL)
		return (NULL);
	ft_printf(fd, "%s\n", cwd);
	free(cwd);
	return (cwd);
}
