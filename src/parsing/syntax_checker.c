/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:26:25 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/06/23 00:11:00 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// maybe add this to lexer
static	int	syn_error(t_data *data, char *arg)
{
	if (!arg)
		printf("syntax error near unexpected token `newline'\n");
	else
		printf("syntax error near unexpected token `%s'\n", arg);
	data->exit_code = 2;
	return (0);
}

int	syntax_checker(t_data *data)
{
	t_token	*current;
	t_token	*next;
	int		status;

	status = 1;
	current = data->token_list;
	next = current->next;
	if (data->token_list->type == PP && !next)
		status = syn_error(data, data->token_list->arg);
	while (current)
	{	
		if ((current->type == OP || current->type == PP) && !next)
			status = syn_error(data, NULL);
		else if (current->type == OP && (next->type == OP || next->type == PP))
			status = syn_error(data, next->arg);
		else if (current->type == PP && next->type == PP)
			status = syn_error(data, next->arg);
		if (!status)
			return (0);
		current = current->next;
		next = current->next;
	}
	return (1);
}
