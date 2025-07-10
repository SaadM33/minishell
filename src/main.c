/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:59:49 by moel-hib          #+#    #+#             */
/*   Updated: 2025/07/08 21:53:29 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_prompt(void)
{
	char		cwd[1000];
	static char	prompt[1000];

	getcwd(cwd, sizeof(cwd));
	snprintf(prompt, 1000, "┌──(minishell)-[%s]\n└─❯ ", cwd);
	return (prompt);
}

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

volatile sig_atomic_t	g_received_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	char	*prompt;

	(void) argc;
	(void) argv;
	data = get_data();
	ft_bzero(data, sizeof(t_data));
	data->env = ft_dupenv(env);
	handle_signals();
	while (1)
	{
		prompt = build_prompt();
		data->str = readline(prompt);
		// shrink this , put all input handling in a function
		if (g_received_signal == SIGINT)
		{
			data->exit_code = 130;
			g_received_signal = 0;
		}
		if (data->str == NULL)
		{
			printf("exit\n");
			ft_exit(data->exit_code);
		}
		if (data->str[0] == '\0')
			continue ;
		add_history(data->str);
		if (!parser(&data))
			continue ;
		full_execution(&data);
		free_cmd_list(&data->cmd_list);
		ft_free(data->str);
	}
	return (0);
}

void	ft_exit(int exit_code)
{
	t_list	**list;
	t_list	*curr;
	t_list	*next;

	free_cmd_list(&(get_data()->cmd_list));
	list = ft_get_list();
	if (!list || !*list)
		return ;
	curr = *list;
	while (curr)
	{
		next = curr->next;
		free(curr->content);
		free(curr);
		curr = next;
	}
	*list = NULL;
	exit(exit_code);
}
