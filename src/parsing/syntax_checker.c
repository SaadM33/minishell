/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:26:25 by sel-maaq          #+#    #+#             */
/*   Updated: 2025/06/22 18:26:35 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_checker(t_data *data)
{
	t_token *current;

	current = data->token_list;
	while (current)
	{	
		if (current->type == 3 && (current->next->type == 3 || current->next->type == 4))
			printf("syntax error near unexpected token `%s'", current->next->arg);
		// else if (current->type == 3 && !current->next)
			
	// to complete
	}
}