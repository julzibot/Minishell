#include "minishell.h"

static  char	*get_vars_init(char *token, char *str)
{
	int	i;
	int	v_i;

	i = 0;
	v_i = 0;
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

static  char	*get_value(char *token, int namelen, char *env_var, char *str)
{
	int	v_i;
	int	v_len;
	int	quoted;
	char	*value;

	quoted = 0;
	if ((token[0] == '\"' || token[1] == '\"') && token[namelen + 1] == '\"')
		quoted = 1;
	v_i = -1;
	v_len = 0;
	while (env_var[++v_i] && !quoted)
	{
		if (is_delim(env_var[v_i] == 1))
			v_len++;
	}
	value = malloc(ft_strlen(env_var) - v_len - namelen + quoted);
	v_i = -1;
	v_len = namelen;
	while (env_var[++v_len])
	{
		while (is_delim(env_var[v_len]) == 1)
			v_len++;
		value[++v_i] = env_var[v_len];
	}
	value[++v_i] = '\"';
	value[v_i + quoted] = '\0';
	str = ft_strjoin(str, value);
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

static  char	*check_var_name(char *token, char **env_vars, char *str)
{
	int	j;
	int	namelen;
	int match;

	j = -1;
	match = 0;
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

char	*get_env_vars(char *token, char **env_vars, t_env *env_list) // replace all $NAME by their values in the parsing arguments. TODO : handle ${NAME}
{
	int	i;
	char	*str;
	// printf("GET_IN %s\n", token);

	i = 0;
	(void)env_list;
	str = NULL;
	if (!token)
		return (NULL);
	else if (token[0] == '$' && !token[1])
		return (ft_strdup(token));
	str = get_vars_init(token, str);
	while (str && str[i])
		i++;
	while (token[i])
	{
		str = check_var_name(token + i, env_vars, str);
		while (token[++i] && token[i] != '$')
			;
	}
	if (token[i - 1] == '\"' &&  (!str || str[ft_strlen(str) - 1] != '\"'))
		str = ft_strjoin(str, ft_strdup("\""));
	else if (token[i - 1] == '\'' &&  (!str || str[ft_strlen(str) - 1] != '\''))
		str = ft_strjoin(str, ft_strdup("\'"));
	// printf("GET_OUT %s\n", str);
	return (str);
}