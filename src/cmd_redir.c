/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 12:27:46 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 17:08:24 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_redir_len
{
	int	in_f;
	int	out_f;
}	t_redir_len;

static void	close_fd_pipe(int *fd_pipe, char nmb)
{
	if (nmb == 0)
	{
		if (fd_pipe[0] > -1)
			close(fd_pipe[0]);
		if (fd_pipe[1] > -1)
			close(fd_pipe[1]);
	}
	else
	{
		if (fd_pipe[2] > -1)
			close(fd_pipe[2]);
		if (fd_pipe[3] > -1)
			close(fd_pipe[3]);
	}
}

static char	init_dup2(int fd_out, int fd_in, int i, t_redir_len r_len)
{
	if (!(i & 4) && !r_len.in_f)
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			return (write_err_cmd("dup2", strerror(errno), NULL));
	if ((i && i != 4) && !r_len.out_f)
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (write_err_cmd("dup2", strerror(errno), NULL));
	return (1);
}

static char	redir_pipe(t_list *redir, int fd_out, int fd_in, int i)
{
	t_redir_len	r_len;
	t_list		*el;

	el = redir;
	r_len.in_f = 0;
	r_len.out_f = 0;
	while (el)
	{
		if (!((t_redir *)el->content)->dir)
			r_len.in_f++;
		else
			r_len.out_f++;
		el = el->next;
	}
	if (r_len.in_f || r_len.out_f)
	{
		if (!rw_file(redir))
			return (0);
	}
	if (!init_dup2(fd_out, fd_in, i, r_len))
		return (0);
	return (1);
}

char	set_redirect(t_command *cmd, int *fd_pipe, char i)
{
	if (!cmd->cmd_params[0] && !ft_lstsize(cmd->redir))
		exit(0);
	if ((i & 3) == 1 || (i & 3) == 0)
	{
		if (!redir_pipe(cmd->redir, fd_pipe[1], fd_pipe[2], i))
			return (0);
		if (i && i != 4)
			close_fd_pipe(fd_pipe, 1);
		if (!(i & 4))
			close_fd_pipe(fd_pipe, 0);
		return (1);
	}
	else
	{
		if (!redir_pipe(cmd->redir, fd_pipe[3], fd_pipe[0], i))
			return (0);
		close_fd_pipe(fd_pipe, 0);
		if (!(i & 4))
			close_fd_pipe(fd_pipe, 1);
	}
	return (1);
}

void	ft_lstadd_back_redir(t_list **lst, t_list *el)
{
	t_list	*f_el;
	t_redir	*lst_el;
	t_redir	*fun_el;

	f_el = *lst;
	fun_el = (t_redir *)el->content;
	while (f_el)
	{
		lst_el = (t_redir *)f_el->content;
		if (fun_el->dir == lst_el->dir)
			lst_el->last = 0;
		f_el = f_el->next;
	}
	ft_lstadd_back(lst, el);
}
