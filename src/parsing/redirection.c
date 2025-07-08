/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:36:26 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/08 23:01:24 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this is the very first redirection handler

int	is_redirection(t_token *token)
{
	if (ft_strcmp(token->arg, ">>") == 0 || ft_strcmp(token->arg, "<<") == 0 ||
		ft_strcmp(token->arg, "<") == 0 || ft_strcmp(token->arg, ">") == 0)
		return (1);
	return (0);
}

void	close_prev_fd(t_cmd *cmd, int in_or_out)
{
	if (in_or_out == 0 && cmd->in_fd != 0)
		close(cmd->in_fd);
	else if (in_or_out == 1 && cmd->out_fd != 1)
		close(cmd->out_fd);
}

void	open_files(t_cmd *cmd, t_token *curr)
{
	if (is_redirection(curr))
	{
		if (ft_strcmp(curr->arg, ">") == 0)
		{
			close_prev_fd(cmd, 1);
			cmd->out_fd = open(curr->next->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else if (ft_strcmp(curr->arg, ">>") == 0)
		{
			close_prev_fd(cmd, 1);
			cmd->out_fd = open(curr->next->arg, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else if (ft_strcmp(curr->arg, "<") == 0)
		{
			close_prev_fd(cmd, 0);
			cmd->in_fd = open(curr->next->arg, O_RDONLY);
		}
		// else if (ft_strcmp(curr->arg, "<<") == 0)
		// {
		// 	close_prev_fd(cmd, 0);
		// 	cmd->in_fd = handle_heredoc(curr->next->arg);
		// }
	}
}

int	handle_redirections(t_data *data, t_cmd *cmd)
{
	t_token	*curr;

	while (cmd)
	{
		curr = cmd->list;
		while (curr)
		{
			open_files(cmd, curr);
			if (cmd->in_fd == -1 || cmd->out_fd == -1)
			{
				perror(curr->next->arg);
				cmd->skip_cmd = 1;
				data->exit_code = 1;
				break ;
			}
			if (is_redirection(curr))
			{
				remove_token(&cmd->list, curr->next);
				remove_token(&cmd->list, curr);
			}
			curr = curr->next;
		}
		cmd = cmd->next;
	}
	return (1);
}
