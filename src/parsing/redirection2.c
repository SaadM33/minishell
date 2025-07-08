/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:36:26 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/08 21:54:06 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_heredoc(t_token *node, t_cmd *cmd, t_data *data);

void	close_prev_fd(t_cmd *cmd, int in_or_out)
{
	if (in_or_out == 0 && cmd->in_fd != 0 && cmd->in_fd != -1)
		close(cmd->in_fd);
	else if (in_or_out == 1 && cmd->out_fd != 1 && cmd->out_fd != -1)
		close(cmd->out_fd);
}

int	open_files(t_cmd *cmd, t_token *curr, t_data *data, char *filename)
{
	if (ft_strcmp(curr->arg, ">") == 0)
	{
		close_prev_fd(cmd, 1);
		cmd->out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (ft_strcmp(curr->arg, ">>") == 0)
	{
		close_prev_fd(cmd, 1);
		cmd->out_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (ft_strcmp(curr->arg, "<") == 0)
	{
		close_prev_fd(cmd, 0);
		cmd->in_fd = open(filename, O_RDONLY);
	}
	if (cmd->in_fd == -1 || cmd->out_fd == -1)
	{
		close_prev_fd(cmd, 0);
		close_prev_fd(cmd, 1);
		data->exit_code = 1;
		cmd->skip_cmd = 1;
		return (-1);
	}
	return (0);
}

void	free_appended_nodes(t_token **head, t_token *node)
{
	t_token	*tmp;

	if (node->type == 3)
	{
		tmp = node->next;
		remove_token(head, node);
		node = tmp;
	}
	tmp = node;
	while (tmp && tmp->next && tmp->next && tmp->append)
	{
		node = tmp;
		tmp = tmp->next;
		remove_token(head, node);
	}
	remove_token(head, tmp);
}

int	redirect(t_token *curr, t_cmd *cmd, t_data *data)
{
	t_token	*tmp;

	if (cmd->in_fd != -1 && cmd->out_fd != -1 &&
		curr->type == 3 && ft_strcmp(curr->arg, "<<")) // handle other operators
	{
		tmp = ft_expand_and_join(curr->next, data, 0, NULL);
		if (!tmp || !tmp->arg || tmp->next)
		{
			ft_error("bash: *tmp*: ambiguous redirect\n"); // TODO fix name
			free_token_list(&tmp);
			return (data->exit_code = 1, cmd->skip_cmd = 1, 1);
		}
		if (open_files(cmd, curr, data, tmp->arg))
		{
			ft_error4("minishell: ", tmp->arg, ": ", strerror(errno));
			return (ft_error("\n"), free_token_list(&tmp), data->exit_code = 1, cmd->skip_cmd = 1, 1);
		}
		free_token_list(&tmp);
	}
	return (0);
}

int	handle_redirections(t_data *data, t_cmd *cmd)
{
	t_token	*curr;
	int		check;

	while (cmd)
	{
		curr = cmd->list;
		while (curr)
		{
			check = redirect(curr, cmd, data);
			if (curr->type == 3)
			{
				free_appended_nodes(&cmd->list, curr);
				curr = cmd->list;
			}
			else
				curr = curr->next;
		}
		cmd = cmd->next;
	}
	return (1);
}
