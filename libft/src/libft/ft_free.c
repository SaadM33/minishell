/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 21:29:58 by obentahi          #+#    #+#             */
/*   Updated: 2025/06/22 21:55:33 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free(void *ptr)
{
	t_list	**list;
	t_list	*lst;
	t_list	*tmp;

	free(ptr);
	list = ft_malloc(0, 2);
	if (!list || !*list)
		return ;
	lst = *list;
	if (lst->content = ptr)
	{
		*list = lst->next;
		tmp = lst;
	}
	else
	{
		while (lst->next && lst->next->content != ptr)
			lst = lst->next;
		tmp = lst->next;
		if (tmp)
			lst->next = lst->next->next;
	}
	free(tmp);
}
