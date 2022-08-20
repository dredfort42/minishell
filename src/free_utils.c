/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 13:58:23 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 13:16:23 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr_or_lst(char **a_str, t_list *lst)
{
	int		i;
	t_list	*f_el;

	if (a_str)
	{
		i = -1;
		while (a_str[++i])
			free(a_str[i]);
		free(a_str);
	}
	else if (lst)
	{
		f_el = lst;
		while (f_el)
		{
			free(((t_redir *)f_el->content)->fname);
			free(f_el->content);
			f_el = f_el->next;
		}
		ft_lstclear(&lst, NULL);
	}
}

void	del_cmd_el(void *el)
{
	t_command	*cmd;

	cmd = (t_command *)el;
	free_arr_or_lst(cmd->cmd_env, NULL);
	free_arr_or_lst(cmd->cmd_params, NULL);
	free_arr_or_lst(NULL, cmd->redir);
	free(cmd);
}

/*очистка всего;*/
void	free_all(t_shell_data *sh_d)
{
	t_list	*el;
	t_list	*el2;

	el = sh_d->env;
	while (el)
	{
		el2 = el->next;
		del_fnd_el(sh_d, NULL, el, ((t_env *)(el->content))->name);
		el = el2;
	}
	switch_echoctl(1);
	free(sh_d->prompt);
}
