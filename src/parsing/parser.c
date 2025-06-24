/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:18:59 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/06/23 01:24:30 by obentahi         ###   ########.fr       */
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
	print_token_list(data->token_list);
	if (!syntax_checker(data))
		return ;
	// to complete
}