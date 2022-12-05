/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 18:50:02 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/05 14:20:21 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_cd(char *line)
{
	int		ret;
	char	*s;

	ret = 0;
	if (ft_strncmp(line, "cd", 2) == 0 || ft_strncmp(line, "cd ~", 1) == 0)
	{
		s = getenv("HOME");
		ret = chdir(s);
		return ;
	}
	else if (ft_strncmp(line, "..", 2) == 0)
		ret = chdir(line);
	else if (line != NULL)
		ret = chdir(line);
	if (ret == -1)
		ft_printf(2, "cd: no such file or directory: ", line);
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