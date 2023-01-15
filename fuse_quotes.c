#include "minishell.h"

static  char	*rem_quotes(char *str, int pos)
{
	int	i;
	int	len;
	char	*ret;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	ret = malloc(len + 1);
	if (!pos && (str[0] == '\"' || str[0] == '\''))
	{
		i = 0;
		while (str[++i])
			ret[i - 1] = str[i];
		ret[i - 1] = '\0';
	}
	else if (pos && (str[len - 1] == '\"' || str[len - 1] == '\''))
	{
		i = -1;
		while (++i < len - 1)
			ret[i] = str[i];
		ret[i] = '\0';
	}
	free(str);
	return (ret);
}

static  char	*join_tokens(char *token, char **lex_tab, t_cmd *plist, int j)
{
	int	len;
	int	type;
	char	*str;

	type = token_type(lex_tab[j + 1], plist->quoted[j + 1]);
	if (type < 7)
		str = get_env_vars(lex_tab[j + 1], plist->env_vars, plist->env_list);
	else
		str = ft_strdup(lex_tab[j + 1]);
	len = ft_strlen(lex_tab[j + 1]) - 1;
	if (plist->quoted[j + 1] && lex_tab[j + 1][len] != lex_tab[j][ft_strlen(lex_tab[j]) - 1])
		str = rem_quotes(str, 0);
	str = rem_quotes(str, 0);
	token = rem_quotes(token, 1);
	token = ft_strjoin(token, str);
	return (token);
}

char	*fuse_quotes(char *token, char **lex_tab, t_cmd *plist, int j)
{
	int	type;
    int is_quoted;
	int	join_next;

    is_quoted = 0;
    join_next = 0;
    is_quoted = token_type(token, plist->quoted[j]);
	type = is_quoted;
    if (token && is_delim(token[ft_strlen(token) - 1]) == 1 && lex_tab[j + 1] && lex_tab[j + 1][0] == token[ft_strlen(token) - 1])
        join_next = 1;
	while (join_next && lex_tab[j + 1] && !plist->space_after[j])
	{
		type = token_type(lex_tab[j + 1], plist->quoted[j + 1]);
		token = join_tokens(token, lex_tab, plist, j);
        if (!(is_delim(token[ft_strlen(token) - 1]) == 1 && lex_tab[++j + 1] && lex_tab[j + 1][0] == token[ft_strlen(token) - 1]))
            join_next = 0;
    }
    if (is_quoted == 5 || is_quoted == 7)
        token = rem_quotes(token, 0);
    if (type == 5 || type == 7)
        token = rem_quotes(token, 1);
    return (token);
}

int	quotes_skip(char **tab, int *space_after)
{
	int	count;

	count = 0;
	while (!space_after[count] && ((tab[0][ft_strlen(tab[0]) - 1] == '\"' \
		&& tab[1] && tab[1][0] == '\"') || (tab[0][ft_strlen(tab[0]) - 1] == '\'' \
		&& tab[1] && tab[1][0] == '\'')))
	{
		count++;
		tab++;
	}
	return (count);
}