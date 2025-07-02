/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:46:26 by obentahi          #+#    #+#             */
/*   Updated: 2025/06/22 19:13:10 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = ft_strlen(s1);
	while (ft_strchr(set, s1[i]))
		i++;
	if (i >= j)
	{
		str = ft_malloc(1);
		if (!str)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	while (ft_strchr(set, s1[j - 1]))
		j--;
	str = ft_malloc(j - i + 1);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1 + i, j - i + 1);
	return (str);
}
