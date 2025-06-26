/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 19:43:02 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/06/26 13:00:24 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token_node(t_token **head, char *arg, int type, int append)
{
	t_token	*new;
	t_token	*curr;

	// if (!arg) // need abilty to add null strings to the list
	// 	return ;
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
		ft_free(current->arg);
		ft_free(current);
		current = next;
	}
	*head = NULL;
}

void	print_token_list(t_token *lst)
{
	while (lst)
	{
		printf("str: %s	type: %i	append: %i\n", lst->arg, lst->type, lst->append);
		lst = lst->next;
	}
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
// 	ft_free(node->arg);
// 	ft_free(node);
// }

void	remove_token(t_token **head, t_token *node)
{
	t_token	*tmp;

	if (!node || !head || !*head)
		return ;
	if (*head == node)
	{
		*head = node->next;
		ft_free(node->arg);
		ft_free(node);
	}
	else
	{
		tmp = *head;
		while (tmp->next && tmp->next != node)
			tmp = tmp->next;
		if (tmp->next)
			tmp->next = tmp->next->next;
		ft_free(node->arg);
		ft_free(node);
	}
}
