/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:59:49 by moel-hib          #+#    #+#             */
/*   Updated: 2025/07/14 19:34:14 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

int	*get_status(void)
{
	static int	flag;

	return (&flag);
}

int	handle_input(t_data *data)
{
	int		*is_running;

	is_running = get_status();
	*is_running = 0;
	data->str = readline("Megashell> ");
	if (data->str == NULL)
	{
		printf("exit\n");
		ft_exit(data->exit_code);
	}
	if (data->str[0] == '\0')
	{
		ft_free(data->str);
		return (0);
	}
	*is_running = 1;
	add_history(data->str);
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	(void) argc;
	(void) argv;
	data = get_data();
	data->env = ft_dupenv(env);
	handle_signals();
	while (1)
	{
		if (!handle_input(data))
			continue ;
		if (!parser(data))
			continue ;
		full_execution(data);
		free_cmd_list(&(data->cmd_list));
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
		exit(exit_code);
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
