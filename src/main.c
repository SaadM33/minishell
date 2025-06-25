/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:59:49 by moel-hib          #+#    #+#             */
/*   Updated: 2025/06/23 09:57:33 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// just to see where we are
// char	*build_prompt()
// {
// 	char	cwd[1000];
// 	static char	prompt[1000];

// 	getcwd(cwd, sizeof(cwd));
// 	snprintf(prompt, 1000, "┌──(minishell)-[%s]\n└─❯ ", cwd);
// 	return (prompt);
// }

volatile sig_atomic_t	g_received_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	// char	*prompt;

	(void) argc;
	(void) argv;
	(void) env;
	ft_bzero(&data, sizeof(data));
	// data.env = copy_env(env);
	// handle_signals();
	while (1)
	{
		// prompt = build_prompt();
		data.str = readline("Megashell> ");
		// shrink this
		if (g_received_signal == SIGINT)
		{
			data.exit_code = 130;
			g_received_signal = 0;
		}
		if (data.str == NULL)
		{
			printf("exit\n");
			// free_d_arr(data.env);
			exit(data.exit_code);
		}
		add_history(data.str);
		parser(&data);
		free_token_list(&data.token_list);
		// future execution here
	}
	return (0);
}


