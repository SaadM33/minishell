/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:43:01 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/14 19:05:24 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

void	sigint_handler(int sig)
{
	t_data	*data;

	data = get_data();
	if (sig == SIGINT)
	{
		g_received_signal = sig;
		write(1, "\n", 1);
		if (!*get_status())
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			data->exit_code = 130;
		}
		return ;
	}
}

void	handle_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
