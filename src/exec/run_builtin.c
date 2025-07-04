/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:40:35 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/04 18:41:35 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_single_builtin(t_data *data, t_cmd *cmd)
{
	int	in_save;
	int	out_save;

	in_save = dup(0);
	out_save = dup(1);
	if (cmd->in_fd != 0)
		dup2(cmd->in_fd, 0);
	if (cmd->out_fd != 1)
		dup2(cmd->out_fd, 1);
	do_builtin(data, cmd->list, 0);
	dup2(in_save, 0);
	dup2(out_save, 1);
	close(in_save);
	close(out_save);
}

int	is_builtin(t_token *token)
{
	if (!token || !token->arg)
		return (0);
	if (!ft_strcmp(token->arg, "cd") || !ft_strcmp(token->arg, "export")
		|| !ft_strcmp(token->arg, "env") || !ft_strcmp(token->arg, "exit")
		|| !ft_strcmp(token->arg, "echo") || !ft_strcmp(token->arg, "pwd")
		|| !ft_strcmp(token->arg, "unset"))
		return (1);
	else
		return (0);
}

void	do_builtin(t_data *data, t_token *list, int exit_or_not)
{
	if (!data || !list || !list->arg)
		return ;
	if (ft_strcmp(list->arg, "cd") == 0)
		do_cd(data, list);
	else if (ft_strcmp(list->arg, "echo") == 0)
		do_echo(data, list);
	else if (ft_strcmp(list->arg, "env") == 0)
		do_env(data);
	else if (ft_strcmp(list->arg, "exit") == 0)
		do_exit(data, list);
	else if (ft_strcmp(list->arg, "export") == 0)
		do_export(data, list);
	else if (ft_strcmp(list->arg, "pwd") == 0)
		do_pwd(data);
	else if (ft_strcmp(list->arg, "unset") == 0)
		do_unset(data, list);
	if (exit_or_not)
		ft_exit(data->exit_code);
}
