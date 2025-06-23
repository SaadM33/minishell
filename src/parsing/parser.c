/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:18:59 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/06/23 20:01:38 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parser(t_data *data)
{
	int	tmp;

	tmp = lexer(data);   // works for now, need further testing later
	if (tmp == '\'')
		ft_error("TODO syntax error found '\n");
	else if (tmp == '"')
		ft_error("TODO syntax error found \"\n");
	printf("%p\n", data->token_list);
	print_token_list(data->token_list);
	// syntax_checker(data);
	// to complete
}