/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:18:59 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/07/04 22:22:41 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	parser(t_data *data)
{
	int res;
	// t_token	*list;

	res = lexer(data);
	if (!syntax_checker(data, res))
		return (0);
	// expand_and_join here
	list_to_cmds(data);
	handle_redirections(data, data->cmd_list);
	// print_cmd_list(data->cmd_list);

	return (1);
}
