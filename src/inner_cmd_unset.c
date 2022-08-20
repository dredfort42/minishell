/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 17:07:59 by msimon            #+#    #+#             */
/*   Updated: 2021/10/31 14:39:29 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_unset(t_shell_data *sh_d)
{
	int		i;
	int		ext;

	ext = 0;
	i = 0;
	while (sh_d->cmd->cmd_params[++i])
	{
		if (!chk_env_name(sh_d->cmd->cmd_params[i]))
			ext = write_err_export("unset", sh_d->cmd->cmd_params[i]);
	}
	exit(ext);
}

char	del_fnd_el(t_shell_data *sh_d, t_list *up, t_list *el, char *prm)
{
	t_env	*env_el;

	env_el = (t_env *)(el->content);
	if (ft_strncmp(prm, env_el->name, ft_strlen(prm) + 1))
		return (0);
	if (!up)
		sh_d->env = el->next;
	else
		up->next = el->next;
	if (env_el->val)
		free(env_el->val);
	free(env_el->name);
	free(env_el);
	free(el);
	return (1);
}

void	run_unset_main(t_shell_data *sh_d)
{
	int		i;
	t_list	*lst_el;
	t_list	*up_lst_el;

	if (sh_d->lst_cmd_size == 1)
	{
		i = 0;
		while (sh_d->cmd->cmd_params[++i])
		{
			if (chk_env_name(sh_d->cmd->cmd_params[i]))
			{
				lst_el = sh_d->env;
				up_lst_el = NULL;
				while (lst_el)
				{
					if (del_fnd_el(sh_d, up_lst_el, lst_el,
							sh_d->cmd->cmd_params[i]))
						break ;
					up_lst_el = lst_el;
					lst_el = lst_el->next;
				}
			}
		}
	}
}
