/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:54:09 by mstojilj          #+#    #+#             */
/*   Updated: 2023/01/28 16:29:12 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern t_gl_env	g_env;

int	ft_verify_exp_var(char *line)
{
	char	*exp;
	t_env	*curr;

	exp = ft_strjoin("declare -x ", line, 0);
	curr = g_env.exp_list;
	while (curr)
	{
		if (ft_strncmp(curr->line, exp, ft_varlen(exp)) == 0
			&& ft_strncmp(curr->line, exp, ft_varlen(curr->line)) == 0)
		{
			if (exp)
				free(exp);
			return (3);
		}
		curr = curr->next;
	}
	if (exp)
		free(exp);
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
		if (curr->line)
			free(curr->line);
		tmp = ft_strjoin("declare -x ", tmp, 2);
		curr->line = malloc(sizeof(char) * (ft_strlen(tmp) + 1));
		ft_strcpy(curr->line, tmp);
		if (tmp)
			free(tmp);
		if (curr->next == NULL)
			break ;
		curr = curr->next;
	}
}

int	ft_find_quotes(char *s)
{
	int	i;
	int	cmpt;

	i = 0;
	cmpt = 0;
	while (s[i])
	{
		if (s[i] == '\"')
			cmpt++;
		i++;
	}
	if (cmpt == 2)
		return (1);
	return (0);
}

char	*ft_clean_var(char *s)
{
	char	*cleaned;
	int		i;
	int		j;

	cleaned = malloc(sizeof(char) * ft_strlen(s) - 1);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == '\"')
			i++;
		cleaned[j] = s[i];
		j++;
		i++;
	}
	cleaned[i] = '\0';
	return (cleaned);
}

void	ft_add_queue(t_env **root, char *s)
{
	t_env	*node;
	t_env	*curr;

	curr = *root;
	node = malloc(sizeof(t_env));
	node->line = malloc(sizeof(char) * (ft_strlen(s) + 1));
	node->next = NULL;
	ft_strcpy(node->line, s);
	if (*root == NULL || root == NULL)
	{
		*root = node;
		return ;
	}
	while (curr)
	{
		if (curr->next == NULL)
		{
			curr->next = node;
			return ;
		}
		curr = curr->next;
	}
	curr->next = node;
}
