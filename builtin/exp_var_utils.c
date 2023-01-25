/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 09:31:54 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 09:38:46 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Check if there is an existing variable that changed content
int	ft_change_var_content(t_cmd *cmd, char *line)

{
	int	i;

	i = 0;
	while (cmd->env_vars[i])
	{
		if (ft_strncmp(cmd->env_vars[i],
				line, ft_varlen(line)) == 0)
		{
			if (ft_strcmp(cmd->env_vars[i], line) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}

void	ft_get_export(t_env **exp_list)
{
	t_env	*curr;
	char	*tmp;

	tmp = NULL;
	curr = *exp_list;
	if (!curr)
		return ;
	while (curr)
	{
		tmp = ft_add_quotes(curr->line);
		free(curr->line);
		tmp = ft_strjoin("declare -x ", tmp, 2);
		curr->line = malloc(sizeof(char) * (ft_strlen(tmp) + 1));
		ft_strcpy(curr->line, tmp);
		free(tmp);
		if (curr->next == NULL)
			break ;
		curr = curr->next;
	}
}

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
