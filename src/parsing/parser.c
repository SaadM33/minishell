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

// void	test_lexer(t_data *data)
// {
// 	char *inputs[] = {
// 		"echo 'hello' > file.txt",
// 		"echo \"hello\" \"|\" file.txt",
// 		"echo 'hello\\'world'",
// 		"echo \"hello\\\"world\"",
// 		"echo 'hello\\' | grep       \"world\"",
// 		"echo \"hello\\\" | grep 'world'",
// 		"cat < \"file with spaces.txt\"",
// 		"cat << 'EOF' | grep \"world\"",
// 		"echo 'hello' | grep \"world\" > \"output file.txt\"",
// 		"echo \"hello\" | grep 'world' >> 'output file.txt'",
// 		"ls | grep 'minishell' | sort | wc -l > \"results file.txt\"",
// 		"echo 'hello' | grep 'world' | sort | wc -l >> 'results file.txt'",
// 		"echo \"hello\\\"world\" | grep \"escaped\\\"quote\"",
// 		"echo 'hello\\'world' | grep 'escaped\\'quote'",
// 		"cat << EOF | grep \"world\" | sort | wc -l > \"complex output.txt\"",
// 		"echo 'nested \"quotes\"' | grep \"nested 'quotes'\"",
// 		"echo \"nested 'quotes'\" | grep 'nested \"quotes\"'",
// 		"cat << 'EOF' | grep \"world\" | sort | wc -l > 'complex output.txt'",
// 		"echo 'hello\\'world' | grep \"world\\\"escaped\" | sort | wc -l",
// 		"echo \"hello\" >> file.txt",
// 		NULL
// 	};
// 	int i = 0;
// 	while (inputs[i])
// 	{
// 		data->str = ft_strdup(inputs[i]);
// 		printf("Testing input: %s\n", data->str);
// 		lexer(data);
// 		print_token_list(data->token_list);
// 		free_token_list(&data->token_list);
// 		ft_free(data->str);
// 		i++;
// 	}
// }

void	parser(t_data *data)
{
	// test_lexer(data);  // For testing purposes, remove later
	int res;

	res = lexer(data);   // works for now, need further testing later
	syntax_checker(data, res);
	print_token_list(data->token_list);
	// to complete
}
