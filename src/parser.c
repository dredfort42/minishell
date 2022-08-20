/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 18:59:57 by msimon            #+#    #+#             */
/*   Updated: 2021/10/31 12:41:26 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_quo
{
	char	ch;
	int		len;
}	t_quo;

static void	skip_quo(char *str, int *i, int len)
{
	char	quo;

	quo = str[*i];
	while (++*i < len)
	{
		if (str[*i] == quo)
		{
			*i += 1;
			return ;
		}
	}
}

void	parser_semicolon(t_shell_data *sh_d, char *str, int len)
{
	int		i;
	int		b;

	i = -1;
	b = 0;
	while (++i < len)
	{
		if (str[i] == '\'' || str[i] == '"')
			skip_quo(str, &i, len);
		if (str[i] == ';')
		{
			if (b != i)
				run_command(sh_d, str + b, i - b);
			b = i + 1;
		}
	}
	if (b != len)
		run_command(sh_d, str + b, len - b);
}

int	parser_cmd(t_shell_data *sh_d)
{
	char	*str;

	str = readline(sh_d->prompt);
	if (!str)
	{
		free(str);
		printf("\x1bM\x1b[%d`exit\n", (int)ft_strlen(sh_d->prompt) + 1);
		return (0);
	}	
	if (str[0])
	{
		add_history_fd(str);
		if (!syntax_err(sh_d, str))
		{
			switch_echoctl(1);
			signal(SIGINT, handler3);
			signal(SIGQUIT, handler3);
			parser_semicolon(sh_d, str, ft_strlen(str));
			signal(SIGINT, handler);
			signal(SIGQUIT, handler);
			switch_echoctl(0);
		}
	}
	free(str);
	return (1);
}
