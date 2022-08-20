/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 20:13:12 by msimon            #+#    #+#             */
/*   Updated: 2021/10/25 10:07:07 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_err(char *str)
{
	switch_echoctl(1);
	write(STDERR_FILENO, str, ft_strlen(str));
	exit(EXIT_FAILURE);
}

int	write_err_cmd(char *str1, char *str2, char *str3)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, str1, ft_strlen(str1));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, str2, ft_strlen(str2));
	if (str3)
	{
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, str3, ft_strlen(str3));
	}
	write(STDERR_FILENO, "\n", 1);
	return (0);
}
