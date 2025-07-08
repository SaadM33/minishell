/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:33:00 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/08 22:56:22 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	full_execution(t_data *data)
{
	int		pid;
	t_cmd	*cmd;

	cmd = data->cmd_list;
	// if (!cmd->list && !cmd->next)
	// 	return ;
	if (is_builtin(cmd->list) && !cmd->next && !cmd->skip_cmd)
		exec_single_builtin(data, cmd);
	else
	{
		while (cmd)
		{
			if (cmd->skip_cmd)
			{
				cmd = cmd->next;
				continue ;
			}
			pid = fork();
			if (pid == 0)
			{
				dup2(cmd->in_fd, 0);
				dup2(cmd->out_fd, 1);
				if (is_builtin(cmd->list))
					do_builtin(data, cmd->list, 1);
				else
					execute_cmd(data, cmd->list);
			}
			if (pid > 0 && !cmd->next)
			{
				int status;
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					data->exit_code = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					data->exit_code = 128 + WTERMSIG(status);
			}
			cmd = cmd->next;
		}
		while (wait(NULL) > 0)
			;
	}
}
