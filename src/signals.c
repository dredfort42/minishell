/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 16:37:54 by msimon            #+#    #+#             */
/*   Updated: 2021/11/02 21:19:26 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int st)
{
	if (st == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	else if (st == SIGINT)
	{		
		rl_on_new_line();
		write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handler2(int st)
{
	if (st == SIGQUIT)
		printf("Quit: %d\n", st);
	else if (st == SIGINT)
		printf("\n");
}

void	handler3(int st)
{
	if (st == SIGQUIT)
		;
	else if (st == SIGINT)
		;
}

void	switch_echoctl(char on)
{
	struct termios	ts;

	tcgetattr(STDIN_FILENO, &ts);
	if (on)
		ts.c_lflag |= ECHOCTL;
	else
		ts.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &ts);
}

void	handler4(int st)
{
	if (st == SIGQUIT)
	{
		printf("\x1bM       \b\b\b\b\b\b\b");
		rl_on_new_line();
		rl_redisplay();
	}
	else if (st == SIGINT)
	{
		switch_echoctl(1);
		load_history_fd();
		exit(1);
	}
}
