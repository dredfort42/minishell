/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logic.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 19:01:19 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 09:02:52 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	log_trim_del(char *str, int	*i, int *len)
{
	int		beg;
	int		end;
	char	fnd;

	fnd = 0;
	beg = *i;
	end = *len - 1;
	while (((str[beg] > 8 && str[beg] < 14) || str[beg] == 32) && beg < *len)
		beg++;
	if (str[beg] == '(')
	{
		beg++;
		fnd += 1;
	}
	while (((str[end] > 8 && str[end] < 14) || str[end] == 32) && end > 0)
		end--;
	if (str[end] == ')')
		fnd += 2;
	if (fnd == 3)
	{
		*i = beg;
		*len = end;
	}
}

static void	ignore_quotes(char *str, int *i, int len, char sym)
{
	while (++*i < len)
		if (str[*i] == sym)
			break ;
}

static int	get_next_op(char *str, int i, int len)
{
	int	br_count;

	i--;
	while (++i < (len - 1) && ft_memcmp(str + i, "&&", 2)
		&& ft_memcmp(str + i, "||", 2))
	{
		if (str[i] == '"' || str[i] == '\'')
			ignore_quotes(str, &i, len, str[i]);
		if (str[i] == '(')
		{
			br_count = 1;
			while (br_count > 0 && ++i < (len - 1))
			{
				if (str[i] == ')')
					br_count--;
				if (str[i] == '(')
					br_count++;
			}
		}
	}
	if (i < (len - 1))
		return (i);
	else
		return (len);
}

static void	chk_next_op(char *str, int res_int, t_pos *pos, int len)
{
	while ((res_int && str[pos->l] == '&')
		|| (!res_int && str[pos->l] == '|'))
	{
		pos->f = pos->l + 2;
		pos->l = get_next_op(str, pos->f, len);
		if (pos->l == len)
		{
			pos->f = pos->l;
			break ;
		}
	}
	if (pos->l < len)
	{
		pos->f = pos->l + 2;
		pos->l = get_next_op(str, pos->f, len);
	}
}

void	run_command(t_shell_data *sh_d, char *str, int len)
{
	t_pos	pos;
	char	*res_str;
	int		res_int;

	pos.f = 0;
	log_trim_del(str, &(pos.f), &len);
	pos.l = get_next_op(str, pos.f, len);
	if (pos.l == len)
		run_command2(sh_d, get_cmd(sh_d, str + pos.f, pos.l - pos.f));
	else
	{
		while (pos.l < len)
		{
			run_command(sh_d, str + pos.f, pos.l - pos.f);
			res_str = get_env_by_name(sh_d, "?");
			res_int = ft_atoi(res_str);
			free(res_str);
			chk_next_op(str, res_int, &pos, len);
		}
		if (pos.f != pos.l)
			run_command(sh_d, str + pos.f, pos.l - pos.f);
	}
}
