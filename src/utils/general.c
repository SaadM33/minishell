/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 03:19:37 by omar              #+#    #+#             */
/*   Updated: 2025/06/25 03:37:29 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_table(char **table)
{
	int	i;

	i = 0;
	while (table[i])
	{
		ft_free(table[i]);
		i++;
	}
	ft_free(table);
}

char	*ft_strjoin3(char *str1, char *middle, char *str2)
{
	char	*tmp;
	char	*full_result;

	tmp = ft_strjoin(str1, middle);
	full_result = ft_strjoin(tmp, str2);
	ft_free(tmp);
	return (full_result);
}
