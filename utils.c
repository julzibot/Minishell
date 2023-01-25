/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 21:33:24 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/25 09:48:13 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_abs(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

void	ft_strcpy(char *dst, char *src)
{
	int	i;

	i = 0;
	if (src == NULL)
		return ;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	//printf("s1 %s s2 %s\n", s1, s2);
	if (n == 0)
		return (0);
	while (*s1 && *s2 && *s1 == *s2 && ++i < n)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ft_is_alnum(char c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') \
		|| ('0' <= c && c <= '9'))
		return (1);
	return (0);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str && str[i] != '\0')
		i++;
	return (i);
}

int	ft_varlen(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return	(i);
}

int	ft_tablen(char **tab)
{
	int	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	char	*dup;
	int	i;

	if (!str)
		return (NULL);
	dup = malloc(ft_strlen(str) + 1);
	i = -1;
	while (str[++i])
		dup[i] = str[i];
	dup[i] = '\0';
	return (dup);
}

char	*ft_strdup_free(char *str)
{
	char	*dup;
	int	i;

	if (!str)
		return (NULL);
	dup = malloc(ft_strlen(str) + 1);
	i = -1;
	while (str[++i])
		dup[i] = str[i];
	dup[i] = '\0';
    free(str);
	return (dup);
}

void	ft_free_join(char *s1, char *s2, int must_free)
{
	if (must_free == 1)
		free(s1);
	if (must_free == 2)
		free(s2);
	if (must_free == 3)
	{
		free(s1);
		free(s2);
	}
}

char	*ft_strjoin(char *s1, char *s2, int must_free)
{
	char	*str;
	int		i;
	int		j;

	if (!s1)
		return (s2);
	else if (!s2)
		return (s1);
	i = -1;
	j = -1;
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		exit(1);
	while (s1[++i])
		str[i] = s1[i];
	while (s2 && s2[++j])
		str[i++] = s2[j];
	str[i] = '\0';
	ft_free_join(s1, s2, must_free);
	return (str);
}

char	**token_join(char **args, char *token) // add a string to the end of a char**
{
	char	**tab;
	int		i;

    if (!token)
        return (args);
	i = -1;
	tab = malloc(sizeof(char *) * (ft_tablen(args) + 2));
	if (!args)
	{
		tab[0] = ft_strdup(token);
		tab[1] = NULL;
		free(token);
		return (tab);
	}
	while (args[++i])
		tab[i] = ft_strdup(args[i]);
	tab[i] = ft_strdup(token);
	tab[i + 1] = NULL;
	ft_free_char_array(args);
	free(token);
	return (tab);
}

int	env_lstsize(t_env*list)
{
	int	len;

	if (!list)
		return (0);
	len = 1;
	while (list->next)
	{
		list = list->next;
		len++;
	}
	return (len);
}

int	cmd_lstsize(t_cmd *list)
{
	int	len;

	if (!list)
		return (0);
	len = 1;
	while (list->next)
	{
		list = list->next;
		len++;
	}
	return (len);
}

char	**ft_tabdup(char **tab, int must_free)
{
	char **dup;
	int	i;
	int	len;

	if (!tab || !tab[0])
		return (NULL);
	len = ft_arrlen(tab);
	i = -1;
	dup = malloc(sizeof(char*) * (ft_tablen(tab) + 1));
	if (!dup)
		return (NULL);
	// while (tab[++i])
	// 	dup[i] = ft_strdup(tab[i]);
	while (++i < len - 1)
		dup[i] = ft_strdup(tab[i]);
	dup[i] = NULL;
	if (must_free && tab)
	 	ft_free_char_array(tab);
	return (dup);
}

char    *char_cat(char *str, char c)
{
    char	*new;
    int		len;
    int 	i;

    i = 0;
    len = ft_strlen(str) + 2;
    new = malloc(len);
    while (str && str[i])
	{
        new[i] = str[i];
		i++;
	}
    new[i] = c;
    new[++i] = '\0';
    free(str);
    return (new);
}

//is_delim :
//1 -> redir
//2 -> pipe
//3 -> quote
//4 -> isspace
//0 -> not a delim
int	is_delim(char c)
{	
	if (c == '\'' || c == '\"')
		return (1);
	else if (c == '|')
		return (2);
	else if (c == '<' || c == '>')
		return (3);
	else if (c == ' ' || c == '	')
		return (4);
	else
		return(0);
}




	// //SEGSIZE
	// while (line[++i] && ((quoted && line[i] != q_type) \
	// 	|| (!quoted && !is_delim(line[i])) \
	// 	|| (!quoted && is_var && (is_delim(line[i]) != 4 || var_quoted))))
	// {
	// 	if (line[i] == '=' && !quoted && !is_var)
	// 		is_var = 1;
	// 	else if (is_var && !var_quoted && is_delim(line[i]) == 1)
	// 	{
	// 		var_quoted = 1;
	// 		var_q_type = i;
	// 	}
	// 	else if (is_var && var_quoted && line[i] == line[var_q_type])
	// 		var_quoted = 0;
	// 	count++;
	// }
	// //LINESEG
	// while (line[++i] && ((quoted && line[i] != line[q_type]) \
	// 	|| (!quoted && !is_delim(line[i])) \
	// 	|| (!quoted && is_var && (is_delim(line[i]) != 4 || var_quoted))) \
	// 	&& !(!quoted && (line[i] == '\\' || line[i] == ';')))
	// {
	// 	if (line[i] == '=' && !quoted && !is_var)
	// 		is_var = 1;
	// 	else if (is_var && !var_quoted && is_delim(line[i]) == 1)
	// 	{
	// 		var_quoted = 1;
	// 		var_q_type = i;
	// 	}
	// 	else if (is_var && var_quoted && line[i] == line[var_q_type])
	// 		var_quoted = 0;
	// 	seg[s_i++] = line[i];
	// }