/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 18:50:02 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/19 16:32:38 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

int	ft_update_pwd(t_env **exp_list, t_env **env_list, char *env)
{
	char	*s;

	s = malloc(sizeof(char) * (PATH_MAX - 1));
	if (!s)
		exit(1);
	if (getcwd(s, PATH_MAX - 1) == NULL) // If NULL no path found
		return (1);
	ft_update_env(env_list, exp_list, ft_strjoin(env, s, -1));
	return (0);
}

void	ft_cd_error(t_cmd *cmd)
{
	if (access(cmd->args[1], R_OK) == -1)
	{
		env.error_code = 1;
		ft_printf(2, "Minichelou: cd: %s: Permission denied\n", cmd->args[1]);
		return ;
	}
	env.error_code = 1;
	ft_printf(2, "Minichelou: cd: %s: No such file or directory\n", cmd->args[1]);
}

void	ft_cd(t_cmd *cmd)
{
	int		ret;
	char	*s;

	ret = 0;
	if (ft_update_pwd(&env.exp_list, &env.env_list, "OLDPWD=") == 1) // Update OLDPWD=
		return ;
	if (cmd->args[1] == NULL || ft_strcmp(cmd->args[1], "~") == 0)
	{
		s = malloc(sizeof(char) * (PATH_MAX - 1));
		if (!s)
			exit(1);
		s = getenv("HOME");
		ret = chdir(s);
		free(s);
		// ft_update_pwd(&env.exp_list, &env.env_list, "PWD=");
	}
	else if (ft_strcmp(cmd->args[1], "..") == 0 && cmd->args[2] == NULL)
		ret = chdir("..");
	else if (cmd->args[1] != NULL)
		ret = chdir(cmd->args[1]);
	if (ret == -1)
	{
		ft_cd_error(cmd);
		return ;
	}
	if (ft_update_pwd(&env.exp_list, &env.env_list, "PWD=") == 1) // Update PWD=
		return ;
}

char	*ft_pwd(t_cmd *cmd)
{
	char	*cwd;
	int		fd;

	fd = cmd->outfile;
	if (cmd->redir[1] == 0 && cmd->piped == 1)
		fd = cmd->out_pipe[1];
	cwd = malloc(sizeof(char) * (PATH_MAX - 1));
	if (!cwd)
		exit(1);
	cwd = getcwd(cwd, PATH_MAX - 1);
	cwd[PATH_MAX - 1] = '\0';
	if (cwd == NULL)
		return (NULL);
	printf("enter PWD fd %d\n", fd);
	ft_printf(fd, "%s\n", cwd);
	//free(cwd);
	return (cwd);
}
