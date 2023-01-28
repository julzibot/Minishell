/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 09:31:54 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 15:04:13 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

int	ft_do_quotes(char *var, char *env_var, int i, int j)
{
	while (var[i])
	{
		env_var[j++] = var[i++];
		if (j != 0 && env_var[j - 1] == '=')
		{
			env_var[j++] = '\"';
			break ;
		}
	}
	while (var[i])
	{
		env_var[j] = var[i];
		if (var[i + 1] == '\0')
		{
			env_var[++j] = '\"';
			return (j);
		}
		i++;
		j++;
	}
	return (j);
}

int	ft_empty_var(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=' && s[i + 1] == '\0')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_empty_quotes(char *var, char *env_var)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (var[i])
	{
		if (var[i] == '=')
		{
			env_var[j] = var[i];
			j++;
			env_var[j] = '\"';
			j++;
			env_var[j] = '\"';
			j++;
			env_var[j] = '\0';
			return (env_var);
		}
		env_var[j] = var[i];
		j++;
		i++;
	}
	return (NULL);
}

int	ft_quotes_exist(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '=' && line[i + 1] == '\"')
		{
			while (line[i])
			{
				if (line[i] == '\"' && line[i + 1] == '\0')
					return (1);
				i++;
			}
		}
		i++;
	}
	return (0);
}

char	*ft_add_quotes(char *var)
{
	int		i;
	int		j;
	char	*env_var;

	i = 0;
	j = 0;
	if (var == NULL)
		return (NULL);
	if (ft_simple_equal(var) || ft_quotes_exist(var))
		return (ft_strdup(var));
	env_var = malloc(sizeof(char) * (ft_strlen(var) + 3));
	if (ft_empty_var(var))
		return (ft_empty_quotes(var, env_var));
	j = ft_do_quotes(var, env_var, i, j);
	env_var[++j] = '\0';
	return (env_var);
}
