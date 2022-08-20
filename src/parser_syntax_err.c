/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_err.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dredfort <dredfort@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 18:26:07 by                   #+#    #+#             */
/*   Updated: 2021/10/31 18:36:12 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static short	ft_error_1_0(char *ws_str)
{
	if (ws_str[0] == ';')
	{
		ft_print_error(";");
		return (1);
	}
	else if (ws_str[0] == ')')
	{
		ft_print_error(")");
		return (1);
	}
	return (0);
}

static void	ft_error_1_1(char *ws_str)
{
	if (ws_str[1] && ws_str[1] == '|'
		&& (!ws_str[2] || ws_str[2] != '|'))
		ft_print_error("|");
	else if (ws_str[1] && ws_str[1] == '(')
		ft_print_error("(");
	else if (ws_str[1] && ws_str[1] == ')')
		ft_print_error(")");
	else if (ws_str[1] && ws_str[1] == ';')
		ft_print_error(";");
	else if (ws_str[2] && ws_str[1] == '&' && ws_str[1] == ws_str[2])
		ft_print_error("&&");
	else if (ws_str[2] && ws_str[1] == '|' && ws_str[1] == ws_str[2])
		ft_print_error("||");
	else
		ft_print_error("newline");
}

static short	ft_error_1_2(char *ws_str)
{
	if (!ws_str[1] || ws_str[0] != ws_str[1])
	{
		if (ws_str[0] == '|')
		{
			ft_print_error("|");
			return (1);
		}
		else if ((!ws_str[1] || !ft_isalnum(ws_str[1]))
			&& (ws_str[0] == '>' || ws_str[0] == '<'))
		{
			ft_error_1_1(ws_str);
			return (1);
		}
	}
	return (0);
}

static short	ft_error_1_3(const char *ws_str)
{
	if (ws_str[1] == '>' && ws_str[2] && ws_str[2] == '|'
		&& (!ws_str[3] || ws_str[3] != '|'))
		ft_print_error("|");
	else if (ws_str[1] == '>' && ws_str[2] && ws_str[2] == '(')
		ft_print_error("(");
	else if (ws_str[1] == '>' && ws_str[2] && ws_str[2] == ')')
		ft_print_error(")");
	else if (ws_str[1] == '>' && ws_str[2] && ws_str[2] == ';')
		ft_print_error(";");
	else if (ws_str[1] == '>'
		&& ws_str[3] && ws_str[2] == '&' && ws_str[2] == ws_str[3])
		ft_print_error("&&");
	else if (ws_str[1] == '>'
		&& ws_str[3] && ws_str[2] == '|' && ws_str[2] == ws_str[3])
		ft_print_error("||");
	else
		ft_print_error("newline");
	return (1);
}

short	ft_error_1(char *ws_str)
{
	if (ft_error_1_0(ws_str))
		return (1);
	if (ft_error_1_2(ws_str))
		return (1);
	if (ws_str[1] && ws_str[0] == ws_str[1])
	{
		if (ws_str[1] == '&')
		{
			ft_print_error("&&");
			return (1);
		}
		else if (ws_str[1] == '|')
		{
			ft_print_error("||");
			return (1);
		}
		else if ((!ws_str[2] || !ft_isalnum(ws_str[2]))
			&& (ws_str[1] == '>' || ws_str[1] == '<'))
			return (ft_error_1_3(ws_str));
	}
	return (0);
}
