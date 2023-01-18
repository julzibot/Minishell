/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 16:39:23 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/18 15:40:53 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	env;

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
	while (var[i])
	{
		env_var[j] = var[i];
		if (env_var[j - 1] == '=')
		{
			env_var[j] = '\"';
			j++;
			break ;
		}
		j++;
		i++;
	}
	while (var[i])
	{
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
	//free(var);
	return (env_var);
}

int	ft_verify_double(t_env *env_list, char *line) // Checks duplicate
{
	t_env	*curr;

	curr = env_list;
	while (curr)
	{
		if (strcmp(curr->line, line) == 0) // put ft_strcmp!
			return (1);
		curr = curr->next;
	}
	curr = env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(curr->line)) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	ft_verify_equal(char *s) // Checks if there is "="
{
	int	i;

	if (!s)
		return (0);
	i = -1;
	while (s[++i])
	{
		if (!ft_is_alnum(s[i]) && !((s[i] == '=') || \
			(s[i] == '+' && s[i + 1] == '=')))
			return (0);
		else if (s[i] == '=')
			return (1);
	}
	return (0);
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
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	free(s); // NEW !!!
	return (str);
}

char	*ft_var_content(t_cmd *cmd, char *line)
{
	(void)cmd;
	t_env	*curr;

	curr = env.env_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, line, ft_varlen(line)) == 0) // ft_strcmp!
			return (line);
		curr = curr->next;
	}
	return (NULL);
}
