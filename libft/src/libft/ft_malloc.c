/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:46:53 by obentahi          #+#    #+#             */
/*   Updated: 2025/06/23 00:48:28 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_malloc(size_t size, int free_flag)
{
	static t_list	*list = 0;
	t_list			*node;
	void			*ptr;

	if (free_flag == 1)
		return (ft_lstclear(&list, free), NULL);
	if (free_flag == 2)
		return (&list);
	ptr = malloc(size);
	if (ptr)
	{
		node = ft_lstnew(ptr);
		if (node)
			return (ft_lstadd_back(&list, node), ptr);
		free(ptr);
	}
	perror("minishell");
	ft_lstclear(&list, free);
	exit(2);
}
