#include "minishell.h"

extern g_t_env	env;

static  char	*get_vars_init(char *token)
{
	int	i;
	int	v_i;
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

static  char	*get_value(char *token, int namelen, char *env_var, char *str)
{
	int	v_i;
	int	quoted;
	char	*value;

	quoted = 0;
	v_i = -1;
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
	return (str);
}

static  int	reset_namelen(char *token)
{
	int	namelen;

	namelen = 0;
	while (token[namelen + 1] && token[namelen + 1] != '$' && is_delim(token[namelen + 1]) != 1 && is_delim(token[namelen + 1]) != 4)
		namelen++;
	return (namelen);
}

int	env_check_name(char *token, char *str, t_env *env_list)
{
	t_env	*temp;
	int	j;
	int	match;
	int	namelen;

	temp = env_list;
	match = 0;
	j = env_lstsize(env_list);
	while (j-- > 0 && !match)
	{
		namelen = 0;
		while (temp->line && temp->line[namelen] != '=')
			namelen++;
		if (!ft_strncmp(temp->line, token + 1, namelen) \
				&& (!token[namelen + 1] || token[namelen + 1] == '$' \
				|| is_delim(token[namelen + 1]) == 1 || is_delim(token[namelen + 1]) == 4))
		{
			str = get_value(token, namelen, temp->line, str);
			match = 1;
		}
		temp = temp->next;
	}
	return (match);
}

static  char	*check_var_name(char *token, char **env_vars, char *str, t_env *env_list)
{
	int	j;
	int	namelen;
	int match;

	j = -1;
	match = 0;
	match = env_check_name(token, str, env_list);
	while (env_vars && env_vars[++j] && !match)
	{
		namelen = 0;
		while (env_vars && env_vars[j][namelen] != '=')
			namelen++;
		if (!ft_strncmp(env_vars[j], token + 1, namelen) \
				&& (!token[namelen + 1] || token[namelen + 1] == '$' \
				|| is_delim(token[namelen + 1]) == 1 || is_delim(token[namelen + 1]) == 4))
		{
			str = get_value(token, namelen, env_vars[j], str);
			match = 1;
		}
	}
	if (!match)
		namelen = reset_namelen(token);
	while (token[++namelen] && token[namelen] != '$')
		str = char_cat(str, token[namelen]);
	return (str);
}

char	*expand_vars(char *token, char *str, char **env_vars, t_env *env_list)
{
	int	i;

	i = 0;
	if (token[i] == '$' && token[i + 1] == '?')
	{
		str = ft_strjoin(str, ft_itoa(env.error_code), 3);
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

char	*get_env_vars(char *token, char **env_vars, t_env *env_list) // replace all $NAME by their values in the parsing arguments. TODO : handle ${NAME}
{
	int	i;
	char	*str;
	// printf("GET_IN %s\n", token);

	i = 0;
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
	if (token[i - 1] == '\"' &&  (!str || str[ft_strlen(str) - 1] != '\"'))
		str = ft_strjoin(str, ft_strdup("\""), 3);
	else if (token[i - 1] == '\'' &&  (!str || str[ft_strlen(str) - 1] != '\''))
		str = ft_strjoin(str, ft_strdup("\'"), 3);
	// printf("GET_OUT %s\n", str);
	return (str);
}