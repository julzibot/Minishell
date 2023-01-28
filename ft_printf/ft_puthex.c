/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 19:03:52 by jibot             #+#    #+#             */
/*   Updated: 2021/11/12 17:11:43 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	vabs(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

int	ft_puthex(int fd, unsigned int n, char c)
{
	int				count;
	char			*base;
	unsigned int	mod;

	count = 0;
	if (c == 'x')
		base = "0123456789abcdef";
	else if (c == 'X')
		base = "0123456789ABCDEF";
	if (n >= 16)
	{
		mod = n % 16;
		n /= 16;
		count += ft_puthex(fd, n, c);
		count += ft_putchar_fd(base[mod], fd);
	}
	else
		count += ft_putchar_fd(base[(unsigned int)n], fd);
	return (count);
}
