/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibot <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 16:02:09 by jibot             #+#    #+#             */
/*   Updated: 2021/11/12 17:24:52 by jibot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

//______________________STRUCTURES______________________//

/*typedef struct s_list
{
	char				*conv;
	char				*str;
	struct s_list		*next;
}	t_conv;*/

//______________________FUNCTIONS_______________________//

int	ft_printf(int fd, const char *format, ...);
int	ft_putchar_fd(char c, int fd);
int	ft_putstr_fd(char *s, int fd);
int	ft_putnbr_fd(int n, int fd);
int	ft_puthex(int fd, unsigned int n, char c);
int	ft_putnbru_fd(unsigned int n, int fd);
int	ft_puthexp(int fd, unsigned long n, char c);

#endif
