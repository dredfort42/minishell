/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 09:13:38 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 15:47:31 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MSH_PROMPT "minishell$ "
# define MEM_ERR "Memory was allocated incorrectly\n"
# define PIPE_ERR "Pipe was init incorrectly\n"
# define FORK_ERR "Fork was run incorrectly\n"
# define HEREDOC "> "

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <termios.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <signal.h>
# include "readline.h"
# include "history.h"
# include "libft.h"

typedef struct s_pos
{
	int		f;
	int		l;
}	t_pos;

typedef struct s_env
{
	char	*name;
	char	*val;
	char	f_gl;
}	t_env;

/*f - флаг 0 если (> или <), 1 если (>> или <<) */
/*если dir == 0 in (в файл) == 1 out (из файла)*/
typedef struct s_redir
{
	char	*fname;
	char	f;
	char	dir;
	char	last;
}	t_redir;

typedef struct s_command
{
	char	**cmd_params;
	char	**cmd_env;
	t_list	*redir;
}	t_command;

typedef struct s_shell_data
{
	t_list		*env;
	int			std_in;
	int			std_out;
	int			std_err;
	t_command	*cmd;
	char		*prompt;
	char		*inner_cmd[8];
	char		*inner_cmd_main[4];
	int			fl_main_op;
	int			errno_main_op;
	int			lst_cmd_size;
	char		c_flag;
	char		money_flag;
}	t_shell_data;

typedef struct s_symbol_flags
{
	short	quotes;
	short	apostrophe;
	short	breckets;
	short	semicolon;
	short	pipe;
	short	to_output;
	short	from_input;
	short	to_outpun_end;
	short	from_input_delimiter;
	short	logical_and;
	short	logical_or;
	short	flag_was_found;
	short	i;
	short	error;
}	t_symbol_flags;

/*init*/
void	init_sh_data(t_shell_data *sh_d, char **arge);

/*env_utils*/
t_env	*get_env_el(char *str, char f_gl);
char	*get_env_by_name(t_shell_data *sh_d, char *str);
char	*get_env_name(char *str);

/*env_utils2.c*/
void	set_env_by_name(t_shell_data *sh_d, char *n_env,
			char *v_env, char f_gl);
char	**get_global_env(t_shell_data *sh_d);
t_list	*lst_el_found(t_list *f_el, char *n_env);

/*exit*/
void	exit_err(char *str);
int		write_err_cmd(char *str1, char *str2, char *str3);

/*history*/
void	load_history_fd(void);
void	add_history_fd(char *str);

/*parser*/
int		parser_cmd(t_shell_data *sh_d);
void	parser_semicolon(t_shell_data *sh_d, char *str, int len);

/*parser_syntax*/
char	syntax_err(t_shell_data *sh_d, char *str);
short	ft_error(t_symbol_flags sf, char *ws_str);

/*parser_syntax_utils.c*/
char	*ft_string_without_spaces(char *str);
void	ft_reset_sf(t_symbol_flags *sf);
void	ft_sf_preset(t_symbol_flags *sf);
void	ft_print_error(char *str);

/*parser_syntax_err.c*/
short	ft_error_1(char *ws_str);

/*parser_syntax_check.c*/
short	ft_syntax_1(t_symbol_flags sf, char *ws_str);
short	ft_syntax_2(t_symbol_flags *sf, char *ws_str);
void	ft_syntax_3(t_symbol_flags *sf, char *ws_str);

/*parser_logic*/
void	run_command(t_shell_data *sh_d, char *str, int len);

/*parser_pipe*/
void	run_command2(t_shell_data *sh_d, t_list *cmd_first);

/*parser_cmd_utils.c*/
void	join_str_arr(char ***arr, char *str);
char	set_quo_val(char *str, int *i);
char	chk_asterix(t_command *res, char **param, char ch);
void	init_command(t_command **res, int *i, int b);

/*cmd_run.c*/
void	run_cmd(t_shell_data *sh_d, int *fd_pipe, char i);
char	*found_cmd(char *str_path, char *str_com);

/*parser_cmd*/
t_list	*get_cmd(t_shell_data *sh_d, char *str, int len);
int		get_end_param(char *str, int b, int e);

/*parser_param.c*/
char	*get_str_param(t_shell_data *sh_d, char *str, int b, int e);
void	str_join_mem(char **des, char *src, int b, int e);
char	*get_val_env(t_shell_data *sh_d, char *str, int *b, int e);

/*parser_asterix.c*/
char	*parser_asterix(char ***arr);

/*parser_var.c*/
char	chk_variable(t_command *res, char **str, t_shell_data *sh_d);

/*cmd_redir.c*/
char	set_redirect(t_command *cmd, int *fd_pipe, char i);
void	ft_lstadd_back_redir(t_list **lst, t_list *el);

/*cmd_pipe_utils.c*/
void	reinit_pipes(int *fd, char src, char type_op);

/*cmd_redir_utils.c*/
char	rw_file(t_list *lst_el);

/*free_utils.c*/
void	free_all(t_shell_data *sh_d);
void	free_arr_or_lst(char **a_str, t_list *lst);
void	del_cmd_el(void *el);

/*signals.c*/
void	handler(int st);
void	handler2(int st);
void	handler3(int st);
void	handler4(int st);
void	switch_echoctl(char on);

/*inner_cmd_utils.c*/
int		ft_arr_size(char **arr);
char	chk_inner_cmd(t_shell_data *sh_d, char **arr);
void	run_inner_cmd(t_shell_data *sh_d);
void	run_inner_cmd_main(t_shell_data *sh_d);

/*inner_cmd_exit.c*/
char	chk_exit(t_shell_data *sh_d, t_list *lst_cmd);
void	cmd_exit(t_shell_data *sh_d, t_command *cmd);
void	run_exit(t_shell_data *sh_d);

/*inner_cmd_echo.c*/
void	run_echo(t_shell_data *sh_d);

/*inner_cmd_cd.c*/
void	run_cd(t_shell_data *sh_d);
void	run_cd_main(t_shell_data *sh_d);

/*inner_cmd_pwd.c */
void	run_pwd(t_shell_data *sh_d);

/*inner_cmd_env.c*/
void	run_env(t_shell_data *sh_d);
void	change_cmd_this(t_shell_data *sh_d);

/*inner_cmd_export.c*/
void	run_export(t_shell_data *sh_d);
void	run_export_main(t_shell_data *sh_d);
char	chk_env_name(char *n);
int		write_err_export(char *op, char *str);

/*inner_cmd_export_utils.c*/
void	write_export(t_shell_data *sh_d);

/*inner_cmd_unset.c*/
void	run_unset(t_shell_data *sh_d);
void	run_unset_main(t_shell_data *sh_d);
char	del_fnd_el(t_shell_data *sh_d, t_list *up, t_list *el, char *prm);

#endif
