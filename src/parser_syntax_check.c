/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dredfort <dredfort@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/31 18:19:53 by                   #+#    #+#             */
/*   Updated: 2021/10/31 18:38:15 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

short	ft_syntax_1(t_symbol_flags sf, char *ws_str)
{
	if (!sf.quotes % 2 && ws_str[sf.i] == '\'')
		sf.apostrophe++;
	else if (!sf.apostrophe % 2 && ws_str[sf.i] == '"')
		sf.quotes++;
	if (!sf.quotes % 2 && !sf.apostrophe % 2)
		return (1);
	return (0);
}

static short	ft_syntax_2_0(t_symbol_flags *sf, char *ws_str)
{
	if (ws_str[sf->i + 1] && ws_str[sf->i] == ws_str[sf->i + 1]
		&& (ws_str[sf->i] == '>' || ws_str[sf->i] == '<'
			|| ws_str[sf->i] == '&' || ws_str[sf->i] == '|'))
	{
		sf->i++;
		if (ws_str[sf->i] == '>')
			sf->to_outpun_end++;
		else if (ws_str[sf->i] == '<')
			sf->from_input_delimiter++;
		else if (ws_str[sf->i] == '&')
			sf->logical_and++;
		else if (ws_str[sf->i] == '|')
			sf->logical_or++;
		return (0);
	}
	return (1);
}

static short	ft_syntax_2_1(t_symbol_flags *sf, char *ws_str)
{
	if (ws_str[sf->i + 1])
	{
		if (ws_str[sf->i + 1] == ')')
		{
			ft_print_error(")");
			sf->error = 1;
		}
		if (ws_str[sf->i + 2]
			&& ws_str[sf->i + 1] == ws_str[sf->i + 2])
		{
			if (ws_str[sf->i + 1] == '|')
			{
				ft_print_error("||");
				sf->error = 1;
			}
			else if (ws_str[sf->i + 1] == '&')
			{
				ft_print_error("&&");
				sf->error = 1;
			}
		}
	}
	return (sf->error);
}

short	ft_syntax_2(t_symbol_flags *sf, char *ws_str)
{
	if (ft_syntax_2_0(sf, ws_str))
	{
		if (ws_str[sf->i] == '(')
		{
			sf->breckets++;
			if (ft_syntax_2_1(sf, ws_str))
				return (1);
		}
		else if (ws_str[sf->i] == ')')
			sf->breckets--;
		else if (ws_str[sf->i] == ';')
			sf->semicolon++;
		else if (ws_str[sf->i] == '|')
			sf->pipe++;
		else if (ws_str[sf->i] == '>')
			sf->to_output++;
		else if (ws_str[sf->i] == '<')
			sf->from_input++;
	}
	return (0);
}

void	ft_syntax_3(t_symbol_flags *sf, char *ws_str)
{
	if (sf->semicolon || sf->pipe || sf->to_output || sf->from_input
		|| sf->to_outpun_end || sf->from_input_delimiter || sf->logical_and
		|| sf->logical_or)
		sf->flag_was_found++;
	if (sf->flag_was_found == 1)
		ft_reset_sf(sf);
	sf->i++;
	sf->error = ft_error(*sf, ws_str);
}
