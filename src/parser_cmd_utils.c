/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:52:30 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 13:16:22 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_arr(char ***arr, int len)
{
	*arr = malloc(sizeof(char *) * len);
	if (!*arr)
		exit_err(MEM_ERR);
}

void	join_str_arr(char ***arr, char *str)
{
	char	**buf;
	int		i;

	if (*arr == NULL)
	{
		init_arr(arr, 2);
		(*arr)[0] = str;
		(*arr)[1] = NULL;
	}
	else
	{
		i = 0;
		while ((*arr)[i])
			i++;
		init_arr(&buf, i + 2);
		buf[i + 1] = NULL;
		buf[i] = str;
		i = -1;
		while ((*arr)[++i])
			buf[i] = (*arr)[i];
		free(*arr);
		*arr = buf;
	}
}

char	set_quo_val(char *str, int *i)
{
	char	res;

	if (str[*i] == '>' || str[*i] == '<')
	{
		res = (str[*i] & 3) >> 1;
		*i += 1;
		if (str[*i] == '>' || str[*i] == '<')
			res = (str[*i] & 7) | res;
		if (res > 1)
			*i += 1;
		while (str[*i] == 32 || (str[*i] > 8 && str[*i] < 14))
			*i += 1;
		return (res);
	}
	else
		return (0);
}

char	chk_asterix(t_command *res, char **param, char ch)
{
	int	i;

	i = -1;
	while ((*param)[++i])
		if ((*param)[i] != '*')
			return (0);
	if (!(ch == '"' || ch == '\''))
	{
		free(*param);
		if (res)
			parser_asterix(&res->cmd_params);
		else
			*param = parser_asterix(NULL);
		return (1);
	}
	else
		return (0);
}

void	init_command(t_command **res, int *i, int b)
{
	*res = malloc(sizeof(t_command));
	if (!*res)
		exit_err(MEM_ERR);
	(*res)->cmd_params = malloc(sizeof(char *));
	(*res)->cmd_params[0] = NULL;
	(*res)->cmd_env = NULL;
	(*res)->redir = NULL;
	*i = b - 1;
}
