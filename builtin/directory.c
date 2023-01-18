/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 18:50:02 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/18 10:44:42 by mstojilj         ###   ########.fr       */
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
	if (getcwd(s, PATH_MAX - 1) == NULL)
	{
		ft_printf(2, "Cannot get current working directory path\n");
		return (1);
	}
	ft_update_env(env_list, exp_list, ft_strjoin(env, s));
	return (0);
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
		ft_update_pwd(&env.exp_list, &env.env_list, "PWD=");
		return ;
	}
	else if (ft_strcmp(cmd->args[1], "..") == 0 && cmd->args[2] == NULL)
		ret = chdir("..");
	else if (cmd->args[1] != NULL)
	{
		ret = chdir(cmd->args[1]);
		return ;
	}
	if (ret == -1)
		ft_printf(2, "cd: no such file or directory: %s\n", cmd->args[1]);
	if (ft_update_pwd(&env.exp_list, &env.env_list, "PWD=") == 1) // Update PWD=
		return ;
}

char	*ft_pwd(t_cmd *cmd)
{
	char	*cwd; // cwd = Current Working Directory

	cwd = malloc(sizeof(char) * (PATH_MAX - 1));
	if (!cwd)
		exit(1);
	cwd = getcwd(cwd, PATH_MAX - 1);
	cwd[PATH_MAX - 1] = '\0';
	if (cwd == NULL)
		return (NULL);
	ft_printf(cmd->outfile, "%s\n", cwd);
	return (cwd);
}

// int	main(int argc, char **argv, char **env)
// {
// 	(void)argc;
// 	(void)argv;
// 	char	*line;

// 	while (1)
// 	{
// 		line = readline("Minishell: ");
// 		if (ft_strncmp(line, "cd", 4) == 0)
// 		{
// 			ft_cd(line);
// 			execve("/bin/ls", argv, env);
// 		}
// 	}
// }