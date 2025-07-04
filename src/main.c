/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:59:49 by moel-hib          #+#    #+#             */
/*   Updated: 2025/07/04 22:33:15 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_prompt()
{
	char	cwd[1000];
	static char	prompt[1000];

	getcwd(cwd, sizeof(cwd));
	snprintf(prompt, 1000, "┌──(minishell)-[%s]\n└─❯ ", cwd);
	return (prompt);
}

volatile sig_atomic_t	g_received_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*prompt;

	(void) argc;
	(void) argv;
	ft_bzero(&data, sizeof(data));
	data.env = ft_dupenv(env);
	handle_signals();
	while (1)
	{
		prompt = build_prompt();
		data.str = readline(prompt);
		// shrink this , put all input handling in a function
		if (g_received_signal == SIGINT)
		{
			data.exit_code = 130;
			g_received_signal = 0;
		}
		if (data.str == NULL)
		{
			printf("exit\n");
			ft_exit(data.exit_code);
		}
		if (data.str[0] == '\0')
			continue;
		add_history(data.str);
		if (!parser(&data))
			continue;
		full_execution(&data);
		free_token_list(&data.token_list);
		free_cmd_list(&data.cmd_list);
	}
	return (0);
}
