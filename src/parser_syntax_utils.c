/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dredfort <dredfort@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 11:13:51 by dredfort          #+#    #+#             */
/*   Updated: 2021/11/03 14:44:45 by dredfort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_string_without_spaces(char *str)
{
	char	*ws_str;
	short	i;
	short	j;

	i = 0;
	j = 0;
	ws_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	ws_str[ft_strlen(str)] = 0;
	if (!ws_str)
		exit(1);
	while (str[i])
	{
		if (str[i] != ' ')
		{
			ws_str[j] = str[i];
			j++;
		}
		i++;
	}
	ws_str[j] = 0;
	return (ws_str);
}

void	ft_reset_sf(t_symbol_flags *sf)
{
	sf->semicolon = 0;
	sf->pipe = 0;
	sf->to_output = 0;
	sf->from_input = 0;
	sf->to_outpun_end = 0;
	sf->from_input_delimiter = 0;
	sf->logical_and = 0;
	sf->logical_or = 0;
}

void	ft_sf_preset(t_symbol_flags *sf)
{
	sf->quotes = 0;
	sf->apostrophe = 0;
	sf->breckets = 0;
	ft_reset_sf(sf);
	sf->flag_was_found = 0;
	sf->i = 0;
	sf->error = 0;
}

void	ft_print_error(char *str)
{
	short	i;

	i = 0;
	write(1, "minishell: syntax error near unexpected token `", 47);
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	write(1, "'\n", 2);
}
