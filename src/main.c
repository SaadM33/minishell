/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:06:27 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/17 21:06:32 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

int	init_cwd(t_data *data)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		ft_error2("shell-init: error retrieving current directory: getcwd: ",
			"cannot access parent directories: No such file or directory\n");
		data->cwd = NULL;
		return (0);
	}
	data->cwd = ft_strdup(cwd);
	return (1);
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
	init_cwd(data);
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
	}
	return (0);
}

void	ft_exit(int exit_code)
{
	t_list	**list;
	t_list	*curr;
	t_list	*next;

	free_cmd_list(&(get_data()->cmd_list));
	ft_free(get_data()->str);
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
