/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:39:23 by mstojilj          #+#    #+#             */
/*   Updated: 2022/12/09 17:17:14 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_add_quotes(char *var)
{
	int		i;
	int		j;
	char	*env_var;
	
	i = 0;
	j = 0;
	env_var = malloc(sizeof(char) * (ft_strlen(var) + 3));
	if (!env_var)
		exit(1);
	while (var[i])
	{
		if (env_var[j - 1] == '=')
		{
			env_var[j] = '\"';
			j++;
		}
		env_var[j] = var[i];
		if (var[i + 1] == '\0')
		{
			j++;
			env_var[j] = '\"';
			j++;
		}
		i++;
		j++;
	}
	env_var[j] = '\0';
	return (env_var);
}

int	ft_verify_double(t_env *env_list, char *line) // Checks if there is a duplicate
{
	t_env	*curr;

	curr = env_list;
	while (curr)
	{
		if (strcmp(curr->line, line) == 0) // put ft_strcmp!
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	ft_verify_equal(char *s) // Checks if there is "="
{
	int	i;
	int	equal;

	i = 0;
	equal = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (0);
		i++;
	}
	return (1);
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
	i = 0;
	str = malloc(sizeof(char) * len);
	if (!str)
		exit(1);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_var_content(t_env **env_list, t_env **exp_list, char *line)
{
	t_env	*curr;

	curr = *env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(line)) == 0) // put ft_strcmp!
		{
			ft_unset(env_list, exp_list, line);
			ft_export(env_list, exp_list, &line);
			printf("exported: %s\n", line);
			return (NULL);
		}
		curr = curr->next;
	}
	return (line);
}