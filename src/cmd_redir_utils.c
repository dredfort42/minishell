/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 13:12:25 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 14:55:50 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	wr_file(t_redir	*redir_el)
{
	int	fd;

	if (redir_el->f)
		fd = open(redir_el->fname, O_CREAT | O_APPEND | O_WRONLY, 0666);
	else
		fd = open(redir_el->fname, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
		return (write_err_cmd(redir_el->fname, strerror(errno), NULL));
	if (redir_el->last)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

static void	console_rd(int fd, t_redir *redir_el)
{
	char	*str;

	signal(SIGINT, handler4);
	signal(SIGQUIT, handler4);
	rl_clear_history();
	switch_echoctl(0);
	while (1)
	{		
		str = readline(HEREDOC);
		if (!str || !ft_strncmp(str, redir_el->fname, ft_strlen(str) + 1))
		{
			free(str);
			if (!str)
				write(0, "\x1bM\x1b[3`", 6);
			break ;
		}
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
	switch_echoctl(1);
	load_history_fd();
}

static int	doc_here(t_redir *redir_el)
{
	char	*buf;
	int		fd;

	buf = ft_strjoin(getenv("HOME"), "/.mini_shell_consol");
	if (!buf)
		buf = ft_strjoin("/Users/Shared", "/.mini_shell_consol");
	if (!buf)
	{
		write_err_cmd("malloc", "Memory was allocated incorrectly", NULL);
		return (-1);
	}
	fd = open(buf, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1)
	{
		free(buf);
		return (fd);
	}
	console_rd(fd, redir_el);
	close(fd);
	fd = open(buf, O_RDONLY);
	free(buf);
	return (fd);
}

static char	rd_file(t_redir	*redir_el)
{
	int	fd;

	if (redir_el->f)
	{
		fd = doc_here(redir_el);
		if (fd == -1)
			return (write_err_cmd(redir_el->fname, strerror(errno), NULL));
		if (redir_el->last)
			dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
		fd = open(redir_el->fname, O_RDONLY);
	if (fd == -1)
		return (write_err_cmd(redir_el->fname, strerror(errno), NULL));
	if (redir_el->last)
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

char	rw_file(t_list *lst_el)
{
	t_redir	*redir_el;

	while (lst_el)
	{
		redir_el = (t_redir *)(lst_el->content);
		if (!redir_el->dir)
		{
			if (!wr_file(redir_el))
				return (0);
		}
		else
		{
			if (!rd_file(redir_el))
				return (0);
		}
		lst_el = lst_el->next;
	}
	return (1);
}
