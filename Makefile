# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/18 10:30:11 by jibot             #+#    #+#              #
#    Updated: 2023/01/25 12:23:23 by mstojilj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC		= gcc -Wall -Wextra -Werror -g3

RM		= rm -rf

SRCS	= Minishell.c jules.c utils.c \
			create_env_vars.c \
			fuse_quotes.c \
			lex_utils.c \
			signals.c \
			init.c \
			error.c \
			ft_itoa.c \
			get_env_vars.c \
			get_env_vars_utils.c \
			builtin/env.c \
			builtin/echo.c \
			builtin/exit.c \
			builtin/unset.c \
			builtin/export.c \
			builtin/directory.c \
			builtin/exit_utils.c \
			builtin/export_utils.c \
			builtin/export_check.c \
			builtin/exp_var_utils.c \
			builtin/builtin_utils.c \
			exec/exec.c \
			exec/exec_utils.c \
			exec/exec_cmd_check.c \

OBJS	= ${SRCS:.c=.o}

READLINE_PATH = vendor/readline/ 
  
COMPILEFLAGS = -L$(READLINE_PATH)/lib -lreadline 

all: libs ${NAME}
  
libs: readline

readline: $(READLINE_PATH) 
  
$(READLINE_PATH):
					sh ./install_readline.sh

%.o:	%.c
		${CC} -c $< -o $@

${NAME}:	${OBJS}
		make all -C ./ft_printf
		${CC} -o ${NAME} ${OBJS} -lreadline -lncurses libreadline.a ft_printf/libftprintf.a
#-lreadline -lncurses libreadline.a ft_printf/libftprintf.a

clean:
		${RM} ${SRCS:.c=.o}

fclean:		clean
		${RM} ${NAME}

re:			fclean all

bonus_re:	fclean bonus

.PHONY:		all clean fclean re
