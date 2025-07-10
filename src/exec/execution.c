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

void	child_exec(t_data *data, t_cmd *cmd, int prev_fd, int pipe_fd[2])
{
	if (cmd->in_fd != 0)
		dup2(cmd->in_fd, 0);
	else if (prev_fd != -1)
		dup2(prev_fd, 0);
	if (cmd->out_fd != 1)
		dup2(cmd->out_fd, 1);
	else if (cmd->next)
		dup2(pipe_fd[1], 1);
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next)
		close(pipe_fd[0]);
	if (cmd->next)
		close(pipe_fd[1]);
	if (is_builtin(cmd->list))
		do_builtin(data, cmd->list, 1);
	else
		execute_cmd(data, cmd->list);
	exit(1);
}

void	full_execution(t_data *data)
{
	t_cmd	*cmd;
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;

	cmd = data->cmd_list;
	if (is_builtin(cmd->list) && !cmd->next && !cmd->skip_cmd)
		return (exec_single_builtin(data, cmd));
	prev_fd = -1;
	while (cmd)
	{
		if (cmd->skip_cmd)
			cmd = cmd->next;
		else
		{
			if (cmd->next && pipe(pipe_fd) == -1)
				return (perror("pipe"), (void)0);
			pid = fork();
			if (pid == 0)
				child_exec(data, cmd, prev_fd, pipe_fd);
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->next)
				close(pipe_fd[1]);
			prev_fd = (cmd->next) ? pipe_fd[0] : -1;
			if (!cmd->next)
				waitpid(pid, &status, 0);
			cmd = cmd->next;
		}
	}
	data->exit_code = (WIFEXITED(status)) ? WEXITSTATUS(status)
		: 128 + WTERMSIG(status);
	while (wait(NULL) > 0)
		;
}

// void	full_execution(t_data *data)
// {
// 	int		pid;
// 	int		status;
// 	t_cmd	*cmd;

// 	cmd = data->cmd_list;
// 	// if (!cmd->list && !cmd->next)
// 	// 	return ;
// 	if (is_builtin(cmd->list) && !cmd->next && !cmd->skip_cmd)
// 		exec_single_builtin(data, cmd);
// 	else
// 	{
// 		while (cmd)
// 		{
// 			if (cmd->skip_cmd || !cmd->list)
// 			{
// 				cmd = cmd->next;
// 				continue ;
// 			}
// 			pid = fork();
// 			if (pid == 0)
// 			{
// 				dup2(cmd->in_fd, 0);
// 				dup2(cmd->out_fd, 1);
// 				if (is_builtin(cmd->list))
// 					do_builtin(data, cmd->list, 1);
// 				else
// 					execute_cmd(data, cmd->list);
// 			}
// 			if (pid > 0 && !cmd->next)
// 			{
// 				waitpid(pid, &status, 0);
// 				if (WIFEXITED(status))
// 					data->exit_code = WEXITSTATUS(status);
// 				else if (WIFSIGNALED(status))
// 					data->exit_code = 128 + WTERMSIG(status);
// 			}
// 			cmd = cmd->next;
// 		}
// 		while (wait(NULL) > 0)
// 			;
// 	}
// }
