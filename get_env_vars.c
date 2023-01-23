/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:40:46 by jibot             #+#    #+#             */
/*   Updated: 2023/01/23 13:40:54 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern g_t_env	g_env;

static char	*get_value(char *token, t_seg *m, char *env_var, char *str)
{
	int		v_i;
	int		quoted;
	char	*value;
	int		namelen;

	quoted = 0;
	v_i = -1;
	namelen = m->namelen;
	if ((token[0] == '\"' || token[1] == '\"') && token[namelen + 1] == '\"')
		quoted = 1;
	value = malloc(value_size(env_var, namelen, quoted));
	while (env_var[++namelen])
	{
		while (is_delim(env_var[namelen]) == 1)
			namelen++;
		value[++v_i] = env_var[namelen];
	}
	value[++v_i] = '\"';
	value[v_i + quoted] = '\0';
	str = ft_strjoin(str, value, 3);
	m->match = 1;
	return (str);
}

char	*env_check_name(char *token, char *str, t_env *env_list, t_seg *m)
{
	t_env	*temp;
	int		j;

	temp = env_list;
	m->match = 0;
	j = env_lstsize(env_list);
	while (j-- > 0 && !m->match)
	{
		m->namelen = 0;
		while (temp->line && temp->line[m->namelen] != '=')
			m->namelen++;
		if (!ft_strncmp(temp->line, token + 1, m->namelen) \
				&& (!token[m->namelen + 1] || token[m->namelen + 1] == '$' \
				|| is_delim(token[m->namelen + 1]) == 1 \
				|| is_delim(token[m->namelen + 1]) == 4))
			str = get_value(token, m, temp->line, str);
		temp = temp->next;
	}
	return (str);
}

static char	*check_var_name(char *token, char **env_vars, \
	char *str, t_env *env_list)
{
	t_seg	*m;

	m = malloc(sizeof(t_seg));
	m->j = -1;
	m->match = 0;
	str = env_check_name(token, str, env_list, m);
	while (env_vars && env_vars[++m->j] && !m->match)
	{
		m->namelen = 0;
		while (env_vars && env_vars[m->j][m->namelen] != '=')
			m->namelen++;
		if (!ft_strncmp(env_vars[m->j], token + 1, m->namelen) \
				&& (!token[m->namelen + 1] || token[m->namelen + 1] == '$' \
				|| is_delim(token[m->namelen + 1]) == 1 \
				|| is_delim(token[m->namelen + 1]) == 4))
			str = get_value(token, m, env_vars[m->j], str);
	}
	if (!m->match)
		m->namelen = reset_namelen(token);
	while (token && token[++m->namelen] && token[m->namelen] != '$')
		str = char_cat(str, token[m->namelen]);
	free(m);
	return (str);
}

char	*expand_vars(char *token, char *str, char **env_vars, t_env *env_list)
{
	int		i;
	char	*str_code;

	i = 0;
	str_code = NULL;
	if (token[i] == '$' && token[i + 1] == '?')
	{
		str_code = ft_itoa(g_env.error_code);
		str = ft_strjoin(str, str_code, 3);
		i++;
		while (token[++i] && token[i] != '$')
			str = char_cat(str, token[i]);
	}
	else
	{
		if (token[i] == '$' && (!token[i + 1] || is_delim(token[i + 1]) == 1))
			str = ft_strjoin(str, ft_strdup("$"), 3);
		str = check_var_name(token + i, env_vars, str, env_list);
	}
	return (str);
}

char	*get_env_vars(char *token, char **env_vars, t_env *env_list)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	if (!token)
		return (NULL);
	str = get_vars_init(token);
	i = ft_strlen(str);
	while (token[i])
	{
		str = expand_vars(token + i, str, env_vars, env_list);
		while (token[++i] && token[i] != '$')
			;
	}
	if (token[i - 1] == '\"' && (!str || str[ft_strlen(str) - 1] != '\"'))
		str = ft_strjoin(str, ft_strdup("\""), 3);
	else if (token[i - 1] == '\'' && (!str || str[ft_strlen(str) - 1] != '\''))
		str = ft_strjoin(str, ft_strdup("\'"), 3);
	return (str);
}
