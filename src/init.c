/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 19:08:41 by msimon            #+#    #+#             */
/*   Updated: 2021/10/31 12:03:08 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_env(t_list	**env, char **arge)
{
	int		i;
	t_list	*el;
	t_env	*el_env;

	*env = NULL;
	el = ft_lstnew((void *)get_env_el("?=0", 0));
	if (!el)
		exit_err(MEM_ERR);
	ft_lstadd_back(env, el);
	if (arge[0])
	{
		i = -1;
		while (arge[++i])
		{
			el_env = get_env_el(arge[i], 1);
			el = ft_lstnew((void *)el_env);
			if (!el)
				exit_err(MEM_ERR);
			ft_lstadd_back(env, el);
		}
	}
}

static void	prompt_init(t_shell_data *sh_d)
{
	int	len;

	len = ft_strlen(MSH_PROMPT);
	sh_d->prompt = malloc(len + 1);
	sh_d->prompt[len] = 0;
	ft_memcpy(sh_d->prompt, MSH_PROMPT, len);
}

static void	init_inner_cmd(t_shell_data *sh_d)
{
	sh_d->inner_cmd[0] = "echo";
	sh_d->inner_cmd[1] = "cd";
	sh_d->inner_cmd[2] = "pwd";
	sh_d->inner_cmd[3] = "export";
	sh_d->inner_cmd[4] = "unset";
	sh_d->inner_cmd[5] = "env";
	sh_d->inner_cmd[6] = "exit";
	sh_d->inner_cmd[7] = NULL;
	sh_d->inner_cmd_main[0] = "cd";
	sh_d->inner_cmd_main[1] = "export";
	sh_d->inner_cmd_main[2] = "unset";
	sh_d->inner_cmd_main[3] = NULL;
}

static void	set_shell_val(t_shell_data *sh_d)
{
	char	*shell_val;
	char	*shell_val2;
	char	*pwd;

	shell_val = get_env_by_name(sh_d, "_");
	if (shell_val[0] == '.')
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			exit_err(MEM_ERR);
		if (shell_val[1] == '.')
			shell_val2 = ft_strjoin(pwd, shell_val + 2);
		else
			shell_val2 = ft_strjoin(pwd, shell_val + 1);
		free(pwd);
		if (!shell_val2)
			exit_err(MEM_ERR);
		set_env_by_name(sh_d, get_env_name("SHELL"), shell_val2, 1);
	}
	free(shell_val);
}

void	init_sh_data(t_shell_data *sh_d, char **arge)
{
	char	*shlvl_val;

	sh_d->std_in = dup(0);
	sh_d->std_out = dup(1);
	sh_d->std_err = dup(2);
	sh_d->c_flag = 0;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	switch_echoctl(0);
	init_env(&(sh_d->env), arge);
	set_shell_val(sh_d);
	set_env_by_name(sh_d, get_env_name("OLDPWD"), NULL, 1);
	shlvl_val = get_env_by_name(sh_d, "SHLVL");
	set_env_by_name(sh_d, get_env_name("SHLVL"),
		ft_itoa(ft_atoi(shlvl_val) + 1), 1);
	set_env_by_name(sh_d, get_env_name("PWD"), getcwd(NULL, 0), 1);
	free(shlvl_val);
	prompt_init(sh_d);
	init_inner_cmd(sh_d);
	load_history_fd();
}
