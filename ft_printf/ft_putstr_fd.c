/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 17:14:40 by jibot             #+#    #+#             */
/*   Updated: 2021/11/12 12:54:22 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_putstr_fd(char *s, int fd)
{
	unsigned int	i;
	int				count;

	i = 0;
	count = 0;
	if (s == NULL)
		return (ft_putstr_fd("(null)", 1));
	while (s[i])
	{
		count += ft_putchar_fd(s[i], fd);
		i++;
	}
	return (count);
}
