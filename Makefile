# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jibot <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/18 10:30:11 by jibot             #+#    #+#              #
#    Updated: 2022/08/01 17:31:49 by jibot            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC		= gcc -Wall -Wextra -Werror

RM		= rm -f

SRCS	= Minishell.c

OBJS	= ${SRCS:.c=.o}

all:	${NAME}

%.o:	%.c
		${CC} -c $< -o $@

${NAME}:	${OBJS}
		make all -C ./ft_printf
		${CC} -o ${NAME} ${OBJS} -lreadline -lncurses libreadline.a ft_printf/libftprintf.a

clean:
		${RM} ${SRCS:.c=.o}

fclean:		clean
		${RM} ${NAME}

re:			fclean all

bonus_re:	fclean bonus

.PHONY:		all clean fclean re
