/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:36:26 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/08 23:02:43 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this is old redirection with both redirection and heredoc, kept just incase something in it is needed

int	ft_heredoc(t_token *node, t_cmd *cmd, t_data *data);

// int	is_redirection(t_token *token)
// {
// 	if (ft_strcmp(token->arg, ">>") == 0 || ft_strcmp(token->arg, "<<") == 0 ||
// 		ft_strcmp(token->arg, "<") == 0 || ft_strcmp(token->arg, ">") == 0)
// 		return (1);
// 	return (0);
// }

void	close_prev_fd(t_cmd *cmd, int in_or_out)
{
	if (in_or_out == 0 && cmd->in_fd != 0 && cmd->in_fd != -1)
		close(cmd->in_fd);
	else if (in_or_out == 1 && cmd->out_fd != 1 && cmd->out_fd != -1)
		close(cmd->out_fd);
}

int	open_files(t_cmd *cmd, t_token *curr, char *filename)
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

	if (curr->type == 3 && !ft_strcmp(curr->arg, "<<"))
	{
		if (ft_heredoc(curr->next, cmd, data))
			return (2);
	}
	else if (curr->type == 3) // handle other operators
	{
		tmp = ft_expand_and_join(curr->next, data, 0, NULL);
		if (!tmp || !tmp->arg || tmp->next)
		{
			ft_error("bash: *tmp*: ambiguous redirect\n"); // TODO fix name
			free_token_list(&tmp);
			return (data->exit_code = 1, cmd->skip_cmd = 1, 1);
		}
		if (open_files(cmd, curr, tmp->arg))
		{
			ft_error4("minishell: ", tmp->arg, ": ", strerror(errno));
			return (ft_error("\n"), free_token_list(&tmp), data->exit_code = 1, cmd->skip_cmd = 1	, 1);
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
			if (check == 1)
				break ;
			else if (check == 2)
				return (0);
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

char	*ft_heredoc_name(int c)
{
	char	*char_buff;
	char	*name;
	char	*path;
	int		fd;
	int		i;

	name = ft_calloc(HEREDOC_SIZE + 1, sizeof(char));
	fd = open("/dev/urandom", O_RDONLY);
	char_buff = "2>ND{e:~mn7G`<kW6h^_IuAOw&#\\}y"
			"aszM]btp?8@3%L$9J*4Kd!j[g5lfrRH0Y(ox;SE\"cXU1CQv|)F=q+T'-VZBP,i";
	if (fd == -1)
		return (perror("minishell: open"), ft_free(name), NULL);
	i = 0;
	while (i < HEREDOC_SIZE)
	{
		if (read(fd, &c, 1) == -1)
			return (perror("minishell: read"), ft_free(name), close(fd), NULL);
		name[i] = char_buff[c % 92];
		i++;
	}
	close(fd);
	name[i] = '\0';
	path = ft_strjoin("/tmp/", name);
	return (ft_free(name), path);
}

int	ft_should_expand(t_token *node)
{
	int should_expand;

	should_expand = 1;
	while (node && node->append && node->next)
	{
		if (node->type)
		should_expand = 0;
		node = node->next;
	}
	if (node->type)
		should_expand = 0;
	return (should_expand);
}

void	heredoc_read_loop(t_token *node, int fd, int should_expand,
		t_data *data)
{
	char *str;
	char *str2;

	str = readline("> ");
	if (!str)
	{
		close(fd);
		ft_error3("minishell: warning: here-document delimited by "
				"end-of-file (wanted `", node->arg, "')\n");
		ft_exit(0);
	}
	if (!ft_strcmp(str, node->arg))
		(close(fd), ft_exit(0));
	if (should_expand)
	{
		str2 = mini_expand(str, data);
		ft_free(str);
	}
	else
		str2 = str;
	write(fd, str2, ft_strlen(str2));
	write(fd, "\n", 1);
	ft_free(str2);
}

int	ft_fork_heredoc(t_token *node, int fd, int should_expand, t_data *data)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), data->exit_code = 1, 1); // handle error here
	status = 0;
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		while (69)
			heredoc_read_loop(node, fd, should_expand, data);
	}
	close(fd);
	wait(&status);
	if (WIFSIGNALED(status))
		return (data->exit_code = WTERMSIG(status) + 128, 1);
	else
		data->exit_code = WEXITSTATUS(status);
	return (0);
}

//HEREDOC
// - use expand_and_join to just join since the hereoc doesnt expand the delimiter
// - the int "should_join" is calculated to be 0 or 1 depending on if it has a quote present
//  can later make it a second function for cleanness
int	ft_heredoc(t_token *node, t_cmd *cmd, t_data *data)
{
	char	*heredoc_name;
	int		should_expand;
	int		rd_fd;
	int		wr_fd;

	should_expand = ft_should_expand(node);
	node = ft_expand_and_join(node, data, 1, NULL);
	heredoc_name = ft_heredoc_name(0);
	if (!heredoc_name)
		return (data->exit_code = 1, free_token_list(&node), -1);
	printf("delimitter: %s, %i, heredoc name: %s\n", node->arg, should_expand, heredoc_name); /// delete when done
	wr_fd = open(heredoc_name, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (wr_fd == -1)
		return (perror("minishell: open"), data->exit_code = 1,
			ft_free(heredoc_name), free_token_list(&node), -1);
	rd_fd = open(heredoc_name, O_RDONLY, 0600);
	if (rd_fd == -1)
		return (perror("minishell: open"), data->exit_code = 1,
			ft_free(heredoc_name), free_token_list(&node), close(wr_fd), -1);
	(unlink(heredoc_name), ft_free(heredoc_name));
	cmd->in_fd = rd_fd;
	if (ft_fork_heredoc(node, wr_fd, should_expand, data))
		return (free_token_list(&node), -1);
	return (free_token_list(&node), 0);
}
