/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 18:43:43 by jibot             #+#    #+#             */
/*   Updated: 2021/11/12 17:15:28 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static int	is_negative(int n, int fd)
{
	int	count;

	count = 0;
	if (n == -2147483648)
		count += ft_putstr_fd("-2147483648", fd);
	else if (n < 0)
		count += ft_putchar_fd('-', fd);
	return (count);
}

int	ft_putnbr_fd(int n, int fd)
{
	int		count;
	int		mod;
	char	c;

	count = 0;
	count += is_negative(n, fd);
	if (count == 11)
		return (count);
	else if (n < 0)
		n = -n;
	if (n >= 10)
	{
		mod = (n % 10);
		n /= 10;
		c = mod + '0';
		count += ft_putnbr_fd(n, fd);
		count += ft_putchar_fd(c, fd);
	}
	else if (n < 10 && n >= 0)
	{
		c = n + '0';
		count += ft_putchar_fd(c, fd);
	}
	return (count);
}
