#include "minishell.h"

static  char	**create_var(char **env_vars, char *cpy)
{
	int	j;
	int	namelen;

	j = 0;
	while (env_vars && env_vars[j])
	{
		namelen = 0;
		while (env_vars[j][namelen] != '=')
			namelen++;
		if (!ft_strncmp(env_vars[j], cpy, namelen) && cpy[namelen] == '=')
		{
			free(env_vars[j]);
			env_vars[j] = ft_strdup(cpy);
			return (env_vars);
		}
		j++;
	}
	env_vars = token_join(env_vars, ft_strdup(cpy));
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