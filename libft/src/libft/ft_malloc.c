/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:46:53 by obentahi          #+#    #+#             */
/*   Updated: 2025/06/22 19:08:05 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_malloc(size_t size, int free_flag)
{
	static t_list	*list;
	t_list			*node;
	void			*ptr;

	if (free_flag)
	{
		ft_lstclear(&list, free);
		return (NULL);
	}
	ptr = malloc(size);
	if (ptr)
	{
		node = ft_lstnew(ptr);
		if (node)
		{
			ft_lstadd_back(list, node);
			return (ptr);
		}
		free(ptr);
	}
	perror("minishell");
	ft_lstclear(&list, free);
	exit(2);
}