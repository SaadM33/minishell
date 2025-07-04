/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:11:08 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/04 19:44:38 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_path(const char *dir, const char *cmd);
char	*check_in_path(t_token *list, t_data *data);
char	**build_argv(t_data *data, t_token *list);

void	execute_cmd(t_data *data, t_token *list)
{
	char	*cmd;
	char	**argv;		

	cmd = check_in_path(list, data);
	argv = build_argv(data, list);
	if (execve(cmd, argv, data->env) == -1)
	{
		if (ft_strchr(cmd, '/'))
			printf("%s: No such file or directory\n", cmd);
		else
			printf("%s: command not found\n", cmd);
		ft_exit(127);
	}	
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(tmp, cmd);
	ft_free(tmp);
	return (full_path);
}

char	*check_in_path(t_token *list, t_data *data)
{
	char	*cmd;
	char	*full_cmd;
	char	**dirs;
	int		i;

	cmd = list->arg;
	if (access(cmd, X_OK) == 0)
		return (cmd);
	dirs = ft_split(ft_getenv("PATH", data), ':');
	i = 0;
	while (dirs[i])
	{
		full_cmd = join_path(dirs[i], cmd);
		if (access(full_cmd, X_OK) == 0)
		{
			list->arg = full_cmd;
			return (ft_free(cmd), ft_free_table(dirs), full_cmd);
		}
		ft_free(full_cmd);
		i++;
	}
	ft_free_table(dirs);
	return (cmd);
}

char	**build_argv(t_data *data, t_token *list)
{
	char	**argv;
	int		count;
	int		i;
	t_token	*tmp;

	(void) data;
	count = 0;
	tmp = list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = ft_malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (list)
	{
		argv[i] = list->arg;
		list = list->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}
