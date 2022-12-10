#include "minishell.h"

static  char *check_concat(char *cpy)
{
    int i;
    char    *str;

    i = -1;
    while (cpy && cpy[++i] && cpy[i] != '=')
        ;
    if (cpy && cpy[i] && cpy[i] == '=')
    {
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
            return (cpy);

    }
    return(NULL);
}

static  char	**create_var(char **env_vars, char *cpy)
{
	int	j;
	int	namelen;
    int concat;

	j = -1;
	while (env_vars && env_vars[++j])
	{
        concat = 0;
		namelen = 0;
		while (env_vars[j][namelen] != '=')
			namelen++;
        if (cpy && cpy[namelen] && cpy[namelen] == '+' && cpy[namelen + 1] == '=')
            concat = 1;
		if (!ft_strncmp(env_vars[j], cpy, namelen) && cpy[namelen + concat] == '=')
		{
            if (!concat)
            {
                free(env_vars[j]);
			    env_vars[j] = ft_strdup(cpy);
            }
            else   
                env_vars[j] = ft_strjoin(env_vars[j], ft_strdup(cpy + namelen + concat + 1));
			return (env_vars);
		}
	}
	env_vars = token_join(env_vars, check_concat(ft_strdup(cpy)));
	return (env_vars);
}

char	**create_env_vars(char	*token, char **env_vars) //search for NAME=VALUE in unquoted tokens, store in env_vars
{
	int i;
	char	*cpy;

	i = 0;
	cpy = get_env_vars(token, env_vars);
	while (cpy && cpy[i] && cpy[i] != '=')
		i++;
	if (cpy && cpy[i] && cpy[i] == '=')
		env_vars = create_var(env_vars, cpy);
	free(token);
	free(cpy);
	return (env_vars);
}