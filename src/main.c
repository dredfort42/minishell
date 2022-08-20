/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 09:14:40 by msimon            #+#    #+#             */
/*   Updated: 2021/10/31 14:36:37 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **arge)
{
	t_shell_data	sh_d;

	(void)argv;
	if (argc == 1)
	{
		init_sh_data(&sh_d, arge);
		while (parser_cmd(&sh_d))
			;
		free_all(&sh_d);
		return (EXIT_SUCCESS);
	}
	else
		write(2, "minishell: the program is launched without arguments\n", 53);
	return (EXIT_FAILURE);
}
