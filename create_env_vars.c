/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:41:10 by jibot             #+#    #+#             */
/*   Updated: 2023/01/23 13:41:14 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_concat(char *cpy)
{
	int		i;
	char	*str;

	i = -1;
	while (cpy && cpy[++i] && cpy[i] != '=')
		;
	if (cpy && cpy[i] && cpy[i] == '=')
	{
		if (i == 0)
			return (NULL);
		if (cpy[i - 1] && cpy[i - 1] == '+')
		{
			i = -1;
			str = malloc(ft_strlen(cpy));
			while (cpy[++i] != '+')
				str[i] = cpy[i];
			while (cpy[++i])
				str[i - 1] = cpy[i];
			str[i - 1] = '\0';
			return (str);
		}
		else
			return (ft_strdup(cpy));
	}
	return (NULL);
}

char	*assign_new_value(char *env_var, char *cpy, int *must_ret)
{
	int	concat;
	int	namelen;

	concat = 0;
	namelen = 0;
	while (env_var[namelen] != '=')
		namelen++;
	if (cpy && ft_strlen(cpy) >= namelen + 2 \
			&& cpy[namelen] == '+' && cpy[namelen + 1] == '=')
		concat = 1;
	if (!ft_strncmp(env_var, cpy, namelen) && cpy[namelen + concat] == '=')
	{
		if (!concat)
		{
			free(env_var);
			env_var = ft_strdup(cpy);
		}
		else if (concat)
			env_var = ft_strjoin(env_var, \
				ft_strdup(cpy + namelen + concat + 1), 3);
		*must_ret = 1;
	}
	return (env_var);
}

static char	**create_var(char **env_vars, char *cpy, t_env *env_list)
{
	int		j;
	int		*must_ret;
	t_env	*temp;

	must_ret = malloc(sizeof(int));
	*must_ret = 0;
	temp = env_list;
	j = -1;
	while (env_vars && env_vars[++j])
		env_vars[j] = assign_new_value(env_vars[j], cpy, must_ret);
	j = env_lstsize(env_list);
	while (j-- > 0)
	{
		temp->line = assign_new_value(temp->line, cpy, must_ret);
		temp = temp->next;
	}
	if (!must_ret[0])
		env_vars = token_join(env_vars, check_concat(cpy));
	free(must_ret);
	return (env_vars);
}

char	**create_env_vars(char	*token, char **env_vars, t_env *env_list)
{
	int		i;
	char	*cpy;

	i = 0;
	cpy = get_env_vars(token, env_vars, env_list);
	if (!ft_verify_equal(cpy))
		i = ft_strlen(cpy);
	while (cpy && cpy[i] && cpy[i] != '=')
		i++;
	if (cpy && cpy[i] && cpy[i] == '=')
		env_vars = create_var(env_vars, cpy, env_list);
	free(token);
	free(cpy);
	return (env_vars);
}
