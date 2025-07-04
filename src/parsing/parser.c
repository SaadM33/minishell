/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:18:59 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/06/26 13:05:19 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	parser(t_data *data)
{
	int res;
	// t_token	*list;

	res = lexer(data);
	if (!syntax_checker(data, res))
		return ;
	// expand_and_join here
	list_to_cmds(data);
	handle_redirections(data, data->cmd_list);
	// print_cmd_list(data->cmd_list);

	// to complete
}
