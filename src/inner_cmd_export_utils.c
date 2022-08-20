/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_export_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 17:22:45 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 15:41:32 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fnd_el(t_list *src, t_list **lst_fnd_el, t_list **lst_up_fnd_el)
{
	t_list	*lst_el;
	t_list	*lst_up_el;

	lst_el = src;
	*lst_fnd_el = src;
	*lst_up_fnd_el = NULL;
	while (lst_el)
	{
		if (ft_strncmp(((t_env *)(lst_el->content))->name,
			((t_env *)((*lst_fnd_el)->content))->name,
			ft_strlen(((t_env *)(lst_el->content))->name) + 1) < 0)
		{
			*lst_fnd_el = lst_el;
			*lst_up_fnd_el = lst_up_el;
		}
		lst_up_el = lst_el;
		lst_el = lst_el->next;
	}
}

static t_list	*ft_sort_lst(t_list *src)
{
	t_list	*res;
	t_list	*lst_fnd_el;
	t_list	*lst_up_fnd_el;

	res = NULL;
	while (src)
	{
		fnd_el(src, &lst_fnd_el, &lst_up_fnd_el);
		ft_lstadd_back(&res, ft_lstnew(lst_fnd_el->content));
		if (!lst_up_fnd_el)
			src = lst_fnd_el->next;
		else
			lst_up_fnd_el->next = lst_fnd_el->next;
		free(lst_fnd_el);
	}
	return (res);
}

static t_list	*copy_lst(t_list *src)
{
	t_list	*res;
	t_list	*lst_el;

	res = NULL;
	lst_el = src;
	while (lst_el)
	{
		ft_lstadd_back(&res, ft_lstnew(lst_el->content));
		lst_el = lst_el->next;
	}
	return (res);
}

static char	*add_scr(char *str)
{
	char	*res;
	int		b;
	int		i;

	res = NULL;
	b = 0;
	i = -1;
	while (str[++i])
	{
		if (ft_strchr("\\$\"", str[i]))
		{
			str_join_mem(&res, str, b, i);
			str_join_mem(&res, "\\", 0, 1);
			str_join_mem(&res, str, i, i + 1);
			b = i + 1;
		}
	}
	str_join_mem(&res, str, b, i + 1);
	return (res);
}

void	write_export(t_shell_data *sh_d)
{
	t_list	*lst_el;
	t_list	*f_lst_el;
	t_env	*el;
	char	*new_str;

	lst_el = ft_sort_lst(copy_lst(sh_d->env));
	f_lst_el = lst_el;
	while (lst_el)
	{
		el = (t_env *)lst_el->content;
		if (el->f_gl && !(el->name[0] == '_' && el->name[1] == 0))
		{
			if (el->val)
			{
				new_str = add_scr(el->val);
				printf("declare -x %s=\"%s\"\n", el->name, new_str);
				free(new_str);
			}
			else
				printf("declare -x %s\n", el->name);
		}
		lst_el = lst_el->next;
	}
	ft_lstclear(&f_lst_el, NULL);
}
