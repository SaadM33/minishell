/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_all.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:19:27 by obentahi          #+#    #+#             */
/*   Updated: 2025/07/08 21:42:53 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_add_back(t_token **head, t_token *node)
{
	t_token	*tmp;

	if (!head || !node)
		return ;
	if (!*head)
		*head = node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	expand_all(t_data *data)
{
	t_cmd	*cmd;
	t_token	*new_list;
	t_token	*tmp1;
	t_token	*tmp2;
	
	cmd = data->cmd_list;
	while (cmd)
	{
		new_list = NULL;
		tmp1 = cmd->list;
		while (tmp1)
		{
			tmp2 = ft_expand_and_join(tmp1, data, 0, NULL);
			if (tmp2 && !tmp2->arg)
				free_token_list(&tmp2);
			else
				token_add_back(&new_list, tmp2);
			while (tmp1 && tmp1->append && tmp1->next)
				tmp1 = tmp1->next;
			tmp1 = tmp1->next;
		}
		free_token_list(&cmd->list);
		cmd->list = new_list;
		cmd = cmd->next;
	}
}
