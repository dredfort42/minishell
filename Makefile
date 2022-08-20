#Programm
NAME = 				minishell
NAME_BONUS = 		minishell_bonus

#Libft
LIBFT_PATH =		libft
LIBFT = 			ft

#Readline
RL_PATH =			readline
RL = 				readline history

#Compiler 
CC = 				gcc

#Flags
CFLAGS = 			-Wall -Wextra -Werror

#Include
INC_PATH = 			inc
INC = 				-I${INC_PATH} -I${LIBFT_PATH} -I${RL_PATH}

#Headers
HEA_NAME =			minishell.h
HEA = 				${addprefix ${INC_PATH}/,${HEA_NAME}}

#Sources
SRC_PATH =			src
SRC_NAME_MAIN =		main.c init.c exit.c env_utils.c history.c parser.c \
					parser_logic.c parser_pipe.c env_utils2.c \
					parser_cmd.c parser_param.c parser_asterix.c \
					parser_cmd_utils.c cmd_redir.c cmd_run.c free_utils.c \
					signals.c cmd_redir_utils.c cmd_pipe_utils.c parser_var.c\
					parser_syntax.c inner_cmd_utils.c inner_cmd_exit.c inner_cmd_echo.c \
					inner_cmd_cd.c inner_cmd_pwd.c inner_cmd_env.c inner_cmd_export.c inner_cmd_unset.c inner_cmd_export_utils.c \
					parser_syntax_utils.c parser_syntax_err.c parser_syntax_check.c
SRC_NAME_BONUS =	main.c init.c exit.c env_utils.c history.c parser.c \
					parser_pipe.c env_utils2.c \
					parser_asterix_bonus.c parser_logic_bonus.c \
					parser_cmd.c parser_param.c\
					parser_cmd_utils.c cmd_redir.c cmd_run.c free_utils.c \
					signals.c cmd_redir_utils.c cmd_pipe_utils.c parser_var.c\
					parser_syntax.c inner_cmd_utils.c inner_cmd_exit.c inner_cmd_echo.c \
					inner_cmd_cd.c inner_cmd_pwd.c inner_cmd_env.c inner_cmd_export.c inner_cmd_unset.c inner_cmd_export_utils.c \
					parser_syntax_utils.c parser_syntax_err.c parser_syntax_check.c

#Objects
OBJ_PATH = 			obj
OBJ_MAIN = 			${addprefix ${OBJ_PATH}/,${SRC_NAME_MAIN:.c=.o}}
OBJ_BONUS = 		${addprefix ${OBJ_PATH}/,${SRC_NAME_BONUS:.c=.o}}

${OBJ_PATH}/%.o:	${SRC_PATH}/%.c ${HEA} ${LIBFT_PATH}/*.c ${LIBFT_PATH}/*.h ${LIBFT_PATH}/Makefile ${RL_PATH}/*.c ${RL_PATH}/*.h ${RL_PATH}/Makefile Makefile
					@mkdir -p $(dir $@)
					${CC} ${CFLAGS} -c $< -o $@ ${INC}

${NAME}:			${OBJ_MAIN}
					@$(MAKE) bonus -C ${LIBFT_PATH}
					@$(MAKE) static -C ${RL_PATH}
					${CC} ${CFLAGS} -o ${NAME} ${OBJ_MAIN} -L${LIBFT_PATH} -l${LIBFT} -L${RL_PATH} ${addprefix -l,${RL}} -ltermcap

${NAME_BONUS}:		${OBJ_BONUS}
					@$(MAKE) bonus -C ${LIBFT_PATH}
					@$(MAKE) static -C ${RL_PATH}
					${CC} ${CFLAGS} -o ${NAME_BONUS} ${OBJ_BONUS} -L${LIBFT_PATH} -l${LIBFT} -L${RL_PATH} ${addprefix -l,${RL}} -ltermcap

#Rules

all: 				${NAME}

bonus:				${NAME_BONUS}

clean:
					rm -rf ${OBJ_PATH}					
					@make clean -C ${LIBFT_PATH}
					@make clean -C ${RL_PATH}

fclean: 			clean
					rm -rf ${NAME}
					rm -rf ${NAME_BONUS}
					@make fclean -C ${LIBFT_PATH}

re: 				fclean all

norm:				fclean
					norminette ./src
					norminette ./inc
					norminette ./libft

.PHONY: 			all clean fclean re bonus norm