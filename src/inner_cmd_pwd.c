/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 00:56:16 by msimon            #+#    #+#             */
/*   Updated: 2021/10/20 01:03:43 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_pwd(t_shell_data *sh_d)
{
	char	*pwd;

	(void)sh_d;
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	exit(0);
}
