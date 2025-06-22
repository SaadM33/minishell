/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:43:02 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/06/22 19:51:17 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token_node(t_token **head, char *arg, int type, int append)
{
	t_token	*new;
	t_token	*curr;

	if (!arg)
		return ;
	new = ft_malloc(sizeof(t_token), 0);
	new->arg = arg;
	new->type = type;
	new->append = append;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		curr = *head;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
}

void	free_token_list(t_token **head)
{
	t_token	*current;
	t_token	*next;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		next = current->next;
		free(current->arg);
		free(current);
		current = next;
	}
	*head = NULL;
}

// TODO	fix
// void	remove_token(t_token **head, t_token *node)
// {
// 	if (!node)
// 		return ;
// 	if (node->prev)
// 		node->prev->next = node->next;
// 	else
// 		*head = node->next;
// 	if (node->next)
// 		node->next->prev = node->prev;
// 	free(node->arg);
// 	free(node);
// }
