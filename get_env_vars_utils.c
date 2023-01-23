/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_vars_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 14:45:24 by jibot             #+#    #+#             */
/*   Updated: 2023/01/23 14:45:31 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_vars_init(char *token)
{
	int		i;
	int		v_i;
	char	*str;

	i = 0;
	v_i = 0;
	str = NULL;
	while (token[i] == '\"' || token[i] == '\'')
		i++;
	while (token[i] && token[i] != '$')
		i++;
	if (i > 0)
	{
		str = malloc(i + 1);
		v_i = -1;
		while (++v_i < i && token[v_i])
			str[v_i] = token[v_i];
		str[v_i] = '\0';
	}
	return (str);
}

int	value_size(char *env_var, int namelen, int quoted)
{
	int	v_len;
	int	v_i;

	v_i = -1;
	v_len = 0;
	while (env_var[++v_i] && !quoted)
	{
		if (is_delim(env_var[v_i] == 1))
			v_len++;
	}
	return (ft_strlen(env_var) - v_len - namelen + quoted);
}

int	reset_namelen(char *token)
{
	int	namelen;

	namelen = 0;
	while (token[namelen + 1] && token[namelen + 1] != '$' \
			&& is_delim(token[namelen + 1]) != 1 \
			&& is_delim(token[namelen + 1]) != 4)
		namelen++;
	return (namelen);
}
