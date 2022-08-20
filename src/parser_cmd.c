/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 14:43:05 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 14:36:07 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_temp_ps
{
	int		i;
	char	*param;
}	t_temp_ps;

int	get_end_param(char *str, int b, int e)
{
	char	quo;

	quo = 0;
	if (str[b] == '"' || str[b] == '\'')
		quo = str[b];
	while ((!(str[b] == 32 || (str[b] > 8 && str[b] < 14)
				|| str[b] == '>' || str[b] == '<') || quo) && b < e)
	{
		b++;
		if (!quo && (str[b] == '"' || str[b] == '\'') && b < e)
		{
			quo = str[b];
			b++;
		}
		if (str[b] == quo)
			quo = 0;
	}
	return (b);
}

static void	add_redir(t_command	*res, char *param, char quo)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		exit_err(MEM_ERR);
	redir->fname = param;
	if ((quo & 1) == 0 && ((quo & 7) >> 1) != 3)
	{
		redir->f = quo >> 2;
		redir->dir = 1;
		redir->last = 1;
		ft_lstadd_back_redir(&(res->redir), ft_lstnew(redir));
	}
	else if ((quo & 1) != 0 && ((quo & 7) >> 1) != 2)
	{
		redir->f = quo >> 2;
		redir->dir = 0;
		redir->last = 1;
		ft_lstadd_back_redir(&(res->redir), ft_lstnew(redir));
	}
	else
	{
		free(param);
		free(redir);
	}
}

static char	set_this_val(char *str, int *i, int *b, int e)
{
	char	res;

	res = set_quo_val(str, i);
	*b = *i;
	*i = get_end_param(str, *b, e);
	return (res);
}

static t_command	*get_lst_cmd(t_shell_data *sh_d, char *s, int b, int e)
{
	t_command	*res;
	t_temp_ps	ps;
	char		quo;

	init_command(&res, &(ps.i), b);
	while (++ps.i < e)
	{
		if (!(s[ps.i] == 32 || (s[ps.i] > 8 && s[ps.i] < 14)
				|| s[ps.i] == '>' || s[ps.i] == '<'))
		{
			set_this_val(s, &(ps.i), &b, e);
			ps.param = get_str_param(sh_d, s, b, ps.i);
			if (!chk_variable(res, &(ps.param), sh_d))
				if (!chk_asterix(res, &(ps.param), s[b]))
					join_str_arr(&res->cmd_params, ps.param);
		}
		while (s[ps.i] == '>' || s[ps.i] == '<')
		{
			quo = set_this_val(s, &(ps.i), &b, e);
			ps.param = get_str_param(sh_d, s, b, ps.i);
			chk_asterix(NULL, &(ps.param), s[b]);
			add_redir(res, ps.param, quo);
		}
	}
	return (res);
}

t_list	*get_cmd(t_shell_data *sh_d, char *str, int len)
{
	t_list	*res;
	int		i;
	int		beg;
	char	quo;

	res = NULL;
	beg = 0;
	quo = 0;
	i = -1;
	while (++i < len)
	{
		if ((str[i] == '"' || str[i] == '\'') && !quo)
			quo = str[i++];
		if (str[i] == quo && str[i])
			quo = 0;
		if (str[i] == '|' && !quo)
		{
			ft_lstadd_back(&res, ft_lstnew(get_lst_cmd(sh_d, str, beg, i)));
			beg = i + 1;
		}
	}
	ft_lstadd_back(&res, ft_lstnew(get_lst_cmd(sh_d, str, beg, len)));
	return (res);
}
