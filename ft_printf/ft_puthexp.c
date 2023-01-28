/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 18:04:11 by jibot             #+#    #+#             */
/*   Updated: 2021/11/04 17:46:06 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_puthexp(int fd, unsigned long n, char c)
{
	char				*base;
	static int			count;
	unsigned long		mod;

	count = 2;
	if (c == 'x')
		base = "0123456789abcdef";
	else if (c == 'X')
		base = "0123456789ABCDEF";
	if (n >= 16)
	{
		mod = n % 16;
		n /= 16;
		ft_puthexp(fd, n, c);
		count += ft_putchar_fd(base[mod], fd);
	}
	else
		count += ft_putchar_fd(base[n], fd);
	return (count);
}
