/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 20:09:04 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 08:52:17 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipes(int *fd_pipe, char *i, char f_last)
{
	if (*i == 0 || *i == 1)
	{
		if (fd_pipe[1] > -1 && !f_last)
			close(fd_pipe[1]);
		if (fd_pipe[2] > -1)
			close(fd_pipe[2]);
		*i = 2;
	}
	else
	{
		if (fd_pipe[0] > -1)
			close(fd_pipe[0]);
		if (fd_pipe[3] > -1 && !f_last)
			close(fd_pipe[3]);
		*i = 1;
	}
}

static void	ms_exec_cmd(t_shell_data *sh_d, int *fd_pipe, char *i)
{
	int	pid;

	if (*i == 0 || *i == 1)
		reinit_pipes(fd_pipe, 1, 1);
	else
		reinit_pipes(fd_pipe, 2, 1);
	if (chk_inner_cmd(sh_d, sh_d->inner_cmd_main) != -1)
		run_inner_cmd_main(sh_d);
	pid = fork();
	if (pid == -1)
		exit_err(FORK_ERR);
	else if (pid == 0)
		run_cmd(sh_d, fd_pipe, *i);
	close_pipes(fd_pipe, i, 0);
}

static void	ms_exec_cmd_last(int *exit_stat, t_shell_data *sh_d,
								int *fd_pipe, char i)
{
	int	pid;
	int	pid2;
	int	stat;

	pipe(exit_stat);
	if (chk_inner_cmd(sh_d, sh_d->inner_cmd_main) != -1)
		run_inner_cmd_main(sh_d);
	pid = fork();
	if (pid == -1)
		exit_err(FORK_ERR);
	else if (pid == 0)
	{
		pid2 = fork();
		if (pid2 == 0)
			run_cmd(sh_d, fd_pipe, (i | 4));
		else if (pid2 > 0)
		{
			wait(&stat);
			close(exit_stat[0]);
			write(exit_stat[1], &stat, sizeof(int));
			close(exit_stat[1]);
		}
		exit(0);
	}
	close_pipes(fd_pipe, &i, 1);
}

static void	get_status(t_shell_data *sh_d, t_list *cmd, int *exit_stat)
{
	int		stat;
	t_list	*cmd_first;

	cmd_first = cmd;
	signal(SIGINT, handler2);
	signal(SIGQUIT, handler2);
	while (cmd)
	{
		wait(NULL);
		cmd = cmd->next;
	}
	close(exit_stat[1]);
	read(exit_stat[0], &stat, sizeof(int));
	close(exit_stat[0]);
	set_env_by_name(sh_d, get_env_name("?"), ft_itoa(stat >> 8), 0);
	ft_lstclear(&cmd_first, del_cmd_el);
}

void	run_command2(t_shell_data *sh_d, t_list *cmd_first)
{
	t_list	*cmd;
	int		fd_pipe[4];
	int		exit_stat[2];
	char	i;

	cmd = cmd_first;
	i = 0;
	reinit_pipes(fd_pipe, 3, 4);
	if (cmd)
	{
		if (chk_exit(sh_d, cmd_first))
			cmd_exit(sh_d, (t_command *)cmd->content);
		while (cmd->next)
		{
			sh_d->cmd = (t_command *)cmd->content;
			((t_command *)cmd->content)->cmd_env = get_global_env(sh_d);
			ms_exec_cmd(sh_d, fd_pipe, &i);
			cmd = cmd->next;
		}
		sh_d->cmd = (t_command *)cmd->content;
		((t_command *)cmd->content)->cmd_env = get_global_env(sh_d);
		ms_exec_cmd_last(exit_stat, sh_d, fd_pipe, i);
		get_status(sh_d, cmd_first, exit_stat);
	}
	reinit_pipes(fd_pipe, 3, 2);
}
