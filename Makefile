# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/18 10:30:11 by jibot             #+#    #+#              #
#    Updated: 2022/12/08 16:08:18 by mstojilj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC		= gcc -Wall -Wextra -Werror -g3

RM		= rm -f

SRCS	= Minishell.c jules.c utils.c \
			create_env_vars.c \
			fuse_quotes.c \
			get_env_vars.c \
			lex_utils.c \
			# builtin/env.c \
			# builtin/echo.c \
			# builtin/unset.c \
			# builtin/export.c \
			# builtin/directory.c \
			# builtin/export_utils.c \
			# builtin/builtin_utils.c \
			# exec/exec.c \

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
