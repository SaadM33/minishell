/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:07:00 by obentahi          #+#    #+#             */
/*   Updated: 2025/01/20 14:10:12 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lst2;
	t_list	*tmp;

	if (!lst || !f || !del)
		return (NULL);
	lst2 = ft_lstnew(f(lst->content));
	tmp = lst2;
	while (lst && lst->next)
	{
		if (!tmp)
			return (NULL);
		tmp->next = ft_lstnew(f(lst->next->content));
		tmp = tmp->next;
		lst = lst->next;
	}
	return (lst2);
}
