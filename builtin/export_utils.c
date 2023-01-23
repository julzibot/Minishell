/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:39:23 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/23 16:01:19 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern g_t_env	g_env;

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

char	*ft_add_quotes(char *var)
{
	int		i;
	int		j;
	char	*env_var;

	i = 0;
	j = 0;
	if (var == NULL)
		return (NULL);
	env_var = malloc(sizeof(char) * (ft_strlen(var) + 3));
	j = ft_do_quotes(var, env_var, i, j);
	env_var[++j] = '\0';
	return (env_var);
}

char	*ft_verify_env_var(char *s)
{
	int		i;
	int		len;
	int		equal;
	char	*str;

	i = 0;
	equal = 0;
	while (s[i])
	{
		if (s[i] == '=')
			equal = 1;
		if (s[i] == ' ' && equal == 0)
		{
			ft_printf(2, "export: `=': not a valid identifier\n");
			return (NULL);
		}
		if (s[i] == ' ')
			break ;
		i++;
	}
	len = i + 1;
	str = malloc(sizeof(char) * len);
	ft_strcpy(str, s);
	free(s);
	return (str);
}

char	*ft_var_content(char *line)
{
	t_env	*curr;

	curr = g_env.env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(line)) == 0)
			return (line);
		curr = curr->next;
	}
	return (NULL);
}

int	ft_update_var(t_env **env_list, char *s)
{
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, s, ft_varlen(s)) == 0)
		{
			free(curr->line);
			curr->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
			ft_strcpy(curr->line, s);
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}
