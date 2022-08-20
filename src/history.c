/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 17:14:30 by msimon            #+#    #+#             */
/*   Updated: 2021/10/31 12:22:32 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_his_err(char *str1, char *str2)
{
	write(STDERR_FILENO, str1, ft_strlen(str1));
	write(STDERR_FILENO, str2, ft_strlen(str2));
	write(STDERR_FILENO, "\n", 1);
}

static void	read_history_fd(int fd, char **buf, ssize_t *b_s)
{
	struct stat	l_buf;

	*b_s = 0;
	if (fstat(fd, &l_buf) < 0)
	{
		write_his_err("minishell: ", strerror(errno));
		return ;
	}
	*b_s = l_buf.st_size;
	*buf = malloc(sizeof(char) * *b_s);
	if (!*buf)
		exit_err(MEM_ERR);
	if (read(fd, *buf, *b_s) < 0)
		*b_s = 0;
}

static int	get_fd(char **buf)
{
	int	fd;

	*buf = ft_strjoin(getenv("HOME"), "/.mini_shell_history_msimon");
	if (!*buf)
		*buf = ft_strjoin("/Users/Shared", "/.mini_shell_history_msimon");
	if (!*buf)
		exit_err(MEM_ERR);
	fd = open(*buf, O_CREAT | O_RDONLY, 0666);
	free(*buf);
	*buf = NULL;
	if (fd == -1)
		write_his_err("minishell: ", strerror(errno));
	return (fd);
}

void	load_history_fd(void)
{
	int		fd;
	char	*buf;
	ssize_t	buf_size;
	ssize_t	pos;
	ssize_t	i;

	rl_clear_history();
	fd = get_fd(&buf);
	if (fd == -1)
		return ;
	read_history_fd(fd, &buf, &buf_size);
	i = -1;
	pos = 0;
	while (++i < buf_size)
	{
		if (!buf[i])
		{
			add_history(buf + pos);
			pos = i + 1;
		}
	}
	free(buf);
	close(fd);
}

void	add_history_fd(char *str)
{
	int		fd;
	char	*buf;

	add_history(str);
	buf = ft_strjoin(getenv("HOME"), "/.mini_shell_history_msimon");
	if (!buf)
		buf = ft_strjoin("/Users/Shared", "/.mini_shell_history_msimon");
	if (!buf)
		exit_err(MEM_ERR);
	fd = open(buf, O_CREAT | O_WRONLY | O_APPEND, 0666);
	free(buf);
	if (fd == -1)
	{
		write_his_err("minishell: error save history to file: ",
			strerror(errno));
		return ;
	}
	if (write(fd, str, ft_strlen(str) + 1) < 0)
		write_his_err("minishell: ", strerror(errno));
	close(fd);
}
