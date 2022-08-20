/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dredfort <dredfort@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 11:13:21 by dredfort          #+#    #+#             */
/*   Updated: 2021/11/03 14:44:45 by dredfort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static short	ft_error_2(char *ws_str)
{
	short	len;

	len = ft_strlen(ws_str) - 1;
	if (ws_str[len] == '>')
	{
		ft_print_error("newline");
		return (1);
	}
	else if (ws_str[len] == '<')
	{
		ft_print_error("newline");
		return (1);
	}
	else if (ws_str[len - 1]
		&& ((ws_str[len] == '|' && ws_str[len - 1] == '|')
			|| (ws_str[len] == '&' && ws_str[len - 1] == '&')))
		return (1);
	return (0);
}

static short	ft_error_3_0(t_symbol_flags *sf)
{
	if ((*sf).semicolon)
		ft_print_error(";");
	else if ((*sf).pipe)
		ft_print_error("|");
	else if ((*sf).to_output)
		ft_print_error(">");
	else if ((*sf).from_input)
		ft_print_error("<");
	else if ((*sf).to_outpun_end)
		ft_print_error(">>");
	else if ((*sf).from_input_delimiter)
		ft_print_error("<<");
	else if ((*sf).logical_and)
		ft_print_error("&&");
	else if ((*sf).logical_or)
		ft_print_error("||");
	else
		ft_print_error(" ");
	return (1);
}

static short	ft_error_3(t_symbol_flags sf, char *ws_str)
{
	if ((sf.semicolon + sf.pipe + sf.to_output + sf.from_input
			+ sf.to_outpun_end + sf.from_input_delimiter + sf.logical_and
			+ sf.logical_or)
		&& (ws_str[1] && ws_str[0] != ws_str[1] && ws_str[0] != '<')
		&& sf.flag_was_found)
		return (ft_error_3_0(&sf));
	return (0);
}

short	ft_error(t_symbol_flags sf, char *ws_str)
{
	if (ft_error_1(ws_str))
		return (1);
	if (ft_error_2(ws_str))
		return (1);
	if (ft_error_3(sf, ws_str))
		return (1);
	return (0);
}

char	syntax_err(t_shell_data *sh_d, char *str)
{
	t_symbol_flags	sf;
	char			*ws_str;

	ws_str = ft_string_without_spaces(str);
	ft_sf_preset(&sf);
	while (ws_str[sf.i] && !(sf.error))
	{
		if (ft_isalnum(ws_str[sf.i])
			|| ws_str[sf.i] == '"' || ws_str[sf.i] == '\'')
			sf.flag_was_found = 0;
		if (ft_syntax_1(sf, ws_str))
			if (ft_syntax_2(&sf, ws_str))
				break ;
		ft_syntax_3(&sf, ws_str);
	}
	if (!sf.error && sf.breckets)
	{
		write(1, "minishell: syntax error expected token\n", 39);
		sf.error = 1;
	}
	if (sf.error)
		set_env_by_name(sh_d, get_env_name("?"), ft_itoa(258), 1);
	free(ws_str);
	return (sf.error);
}
