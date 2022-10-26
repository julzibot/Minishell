#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include "./ft_printf/ft_printf.h"

int	ft_abs(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (*s1 && *s2 && *s1 == *s2 && ++i < n)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return	(i);
}

int	lineseg(char *line, char **lex_tab)
{
	int	i;
	int	charcount;
	int	quoted;
	char	*seg;

	i = 0;
	charcount = 0;
	quoted = 0;
	if (line[i] == '\'' || line [i] == '\"')
		quoted = 1;
	while (line[++i] && ((quoted && line[i] != line[0]) 
		|| (!quoted && line[i] != '	' && line[i] != ' ' && line[i] != '|')))
		charcount++;
	seg = malloc(charcount + 1);
	i = 0;
	seg[0] = line[0];
	while (line[++i] && ((quoted && line[i] != line[0]) 
		|| (!quoted && line[i] != '	' && line[i] != ' ' && line[i] != '|')))
		seg[i] = line[i];
	seg[i] = '\0';
	*lex_tab = seg;
	return (i + quoted);
}
 
char	**lexing(char *line)
{
	int	i;
	int j;
	char	**lex_tab;

	i = -1;
	j = 0;
	lex_tab = malloc(sizeof(char*) * 8/*arg_count(line)*/);
	while (line[++i])
	{
		if (line[i] == '|')
			lex_tab[j++] = "|";
		else if (line[i] == '\'' || line [i] == '\"' || (line[i] != ' ' && line[i] != '	' && line[i] != '|'))
		{
			i += lineseg(line + i, lex_tab + j);
			if (line[i] == '\'' || line[i] == '\"' || line[i] == '|')
				i--;
			j++;
		}
	}
	lex_tab[j] = NULL;
	return (lex_tab);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char	*line;
	char	**test;
	int	i;
	// int	hist_count;
	// int hist_fd;

	// if (access("history", F_OK))
	// 	hist_fd = open("history", O_CREAT | O_RDWR | O_TRUNC, 0000644);
	// else
	// 	hist_fd = open("history", O_WONLY);
	// hist_count = 1;
	while (1)
	{
		i = -1;
		line = readline("Mini_chiale> ");
		test = lexing(line);
		while (test[++i])
		{
			printf("%s\n", test[i]);
		}
	}
	return (0);
}
