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

char	**build_argv(t_token *list);

int	is_directory(const char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return (1);
	return (0);
}

char	*check_in_path(t_token *list, t_data *data)
{
	int		i;
	char	*cmd;
	char	*full_cmd;
	char	**dirs;

	cmd = list->arg;
	dirs = ft_split(ft_getenv("PATH", data), ':');
	i = 0;
	while (dirs[i])
	{
		full_cmd = ft_strjoin3(dirs[i], "/", cmd);
		if (!access(full_cmd, X_OK) && !is_directory(full_cmd))
		{
			list->arg = full_cmd;
			return (ft_free(cmd), ft_free_table(dirs), full_cmd);
		}
		ft_free(full_cmd);
		i++;
	}
	ft_error2(cmd, ": command not found\n");
	ft_exit(127);
	return (NULL);
}

char	*lookup_cmd(t_data *data, t_token *list)
{
	if (!list || !list->arg)
		return(ft_exit(0), NULL);
	else if (!ft_strchr(list->arg, '/'))
		return (check_in_path(list, data));
	else
	{
		if (access(list->arg, F_OK) == -1)	
		{
			ft_error3("minishell: ", list->arg, ": N"
				"o such file or directory\n");
			ft_exit(127);
		}
		if (is_directory(list->arg))
		{
			ft_error3("minishell: ", list->arg, ": is a directory\n");
			ft_exit(126);
		}
		return (list->arg);
	}
}

void	execute_cmd(t_data *data, t_token *list)
{
	char	*cmd;
	char	**argv;		

	cmd = lookup_cmd(data, list);
	argv = build_argv(list);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(cmd, argv, data->env) == -1)
	{
		ft_error("minishell: ");
		perror("execve");
		ft_exit(126);
	}
}

char	**build_argv(t_token *list)
{
	char	**argv;
	int		count;
	int		i;
	t_token	*tmp;

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
