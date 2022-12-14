/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 18:50:02 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/10 22:07:53 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_update_pwd(t_env **exp_list, t_env **env_list, char *env)
{
	char	*s;
	// t_env	**curr;

	// curr = env_list;
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

void	ft_cd(t_env **exp_list, t_env **env_list, char *line)
{
	int		ret;
	char	*s;

	ret = 0;
	if (ft_update_pwd(exp_list, env_list, "OLDPWD=") == 1) // Update OLDPWD=
		return ;
	if (line == NULL || strcmp(line, "~") == 0) // ft_strcmp
	{
		s = malloc(sizeof(char) * (PATH_MAX - 1));
		if (!s)
			exit(1);
		s = getenv("HOME");
		ret = chdir(s);
		ft_update_pwd(exp_list, env_list, "PWD=");
		return ;
	}
	else if (ft_strncmp(ft_remove_cmd(line, "cd "), "..", 2) == 0)
		ret = chdir(ft_remove_cmd(line, "cd "));
	else if (line != NULL)
		ret = chdir(ft_remove_cmd(line, "cd "));
	if (ret == -1)
		ft_printf(2, "cd: no such file or directory: %s\n", line);
	if (ft_update_pwd(exp_list, env_list, "PWD=") == 1) // Update PWD=
		return ;
}

char	*ft_pwd(void)
{
	char	*cwd; // cwd = Current Working Directory

	cwd = malloc(sizeof(char) * (PATH_MAX - 1));
	if (!cwd)
		exit(1);
	cwd = getcwd(cwd, PATH_MAX - 1);
	cwd[PATH_MAX - 1] = '\0';
	if (cwd == NULL)
		return (NULL);
	ft_printf(1, "%s\n", cwd);
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