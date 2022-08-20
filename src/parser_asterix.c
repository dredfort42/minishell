/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_asterix.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/10 16:01:21 by msimon            #+#    #+#             */
/*   Updated: 2021/10/11 14:35:02 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_str_var(char **str, int len)
{
	*str = malloc(sizeof(char) * len);
	if (!*str)
		exit_err(MEM_ERR);
}

static void	get_name_lst(char ***arr, char **str, int *i)
{
	DIR				*p_dir;
	struct dirent	*dir_el;
	int				len;

	p_dir = opendir(".");
	dir_el = readdir(p_dir);
	while (dir_el)
	{
		if (dir_el->d_name[0] != '.')
		{
			if (!arr && *str)
				free(*str);
			len = ft_strlen(dir_el->d_name);
			init_str_var(str, len + 1);
			(*str)[len] = 0;
			ft_memcpy(*str, dir_el->d_name, len);
			if (arr)
				join_str_arr(arr, *str);
			*i += 1;
		}
		dir_el = readdir(p_dir);
	}
	closedir(p_dir);
}

/*Возвращает *(если список пуст) или имя файла или каталога если он один*/
/*в текущей директории. Если нет возвращает NULL*/
/*Если lst NULL список никуда не добавляется*/
char	*parser_asterix(char ***arr)
{
	char			*str;
	int				i;

	i = 0;
	str = NULL;
	get_name_lst(arr, &str, &i);
	if (i == 0)
	{
		init_str_var(&str, 2);
		str[0] = '*';
		str[1] = 0;
		if (arr)
			join_str_arr(arr, str);
	}
	else if (i > 1 && !arr)
	{
		free(str);
		str = NULL;
	}
	return (str);
}
