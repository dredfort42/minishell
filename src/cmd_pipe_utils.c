/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pipe_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 10:24:11 by msimon            #+#    #+#             */
/*   Updated: 2021/10/19 10:27:01 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reinit_pipes_close(int *fd, char src)
{
	if ((src & 1) == 1)
	{
		if (fd[0] > -1)
			close(fd[0]);
		if (fd[1] > -1)
			close(fd[1]);
	}
	if ((src & 2) == 2)
	{
		if (fd[2] > -1)
			close(fd[2]);
		if (fd[3] > -1)
			close(fd[3]);
	}
}

/* Операция реинициализации пайпов 						*/
/* источник src - 1(пайп 1) - 2(пайп 2), 3 - оба		*/
/* тип операции type_op - 1(иниц.) - 2(закрыть) 3 - оба	*/
/* тип операции type_op - 4 заполнить -1	*/
void	reinit_pipes(int *fd, char src, char type_op)
{
	if (type_op == 4)
	{
		if ((src & 1) == 1)
		{
			fd[0] = -1;
			fd[1] = -1;
		}
		if ((src & 2) == 2)
		{
			fd[2] = -1;
			fd[3] = -1;
		}
	}
	if ((type_op & 2) == 2)
		reinit_pipes_close(fd, src);
	if ((type_op & 1) == 1)
	{
		if ((src & 1) == 1)
			if (pipe(fd))
				exit_err(PIPE_ERR);
		if ((src & 2) == 2)
			if (pipe(fd + 2))
				exit_err(PIPE_ERR);
	}
}
