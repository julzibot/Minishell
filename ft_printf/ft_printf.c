/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 12:42:23 by jibot             #+#    #+#             */
/*   Updated: 2021/11/12 17:14:58 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static int	ft_putform(int fd, va_list ap, char flag)
{
	if (flag == 'c')
		return (ft_putchar_fd(va_arg(ap, int), fd));
	else if (flag == 's')
		return (ft_putstr_fd(va_arg(ap, char *), fd));
	else if (flag == 'i' || flag == 'd')
		return (ft_putnbr_fd(va_arg(ap, int), fd));
	else if (flag == 'p')
	{
		write(1, "0x", 2);
		return (ft_puthexp(fd, va_arg(ap, unsigned long), 'x'));
	}
	else if (flag == 'x' || flag == 'X')
		return (ft_puthex(fd, va_arg(ap, unsigned int), flag));
	else if (flag == 'u')
		return (ft_putnbru_fd(va_arg(ap, unsigned int), fd));
	else if (flag == '%')
		return (ft_putchar_fd('%', fd));
	else
		return (0);
}

int	ft_printf(int fd, const char *format, ...)
{
	int		i;
	int		count;
	va_list	ap;

	va_start(ap, format);
	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			count += ft_putform(fd, ap, format[i + 1]);
			i++;
		}
		else
			count += ft_putchar_fd(format[i], fd);
		i++;
	}
	va_end(ap);
	return (count);
}
