/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <obentahi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:24:56 by obentahi          #+#    #+#             */
/*   Updated: 2025/07/16 21:30:42 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_strict_num(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (str[0] == '+' || str[0] == '-')
		str++;
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (i > 0);
}

static int	parse_exit_code(char *str, long long *result)
{
	int			sign;
	long long	num;

	sign = 1;
	num = 0;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		num = num * 10 + (*str - '0');
		if (num < 0)
			return (0);
		str++;
	}
	*result = num * sign;
	return (1);
}

int	do_exit(t_data *data, t_token *list)
{
	t_token		*arg;
	long long	exit_status;

	if (data->cmd_list && !data->cmd_list->next)
		printf("exit\n");
	arg = list->next;
	if (arg)
	{
		if (!is_strict_num(arg->arg)
			|| !parse_exit_code(arg->arg, &exit_status))
		{
			ft_error3("exit: ", arg->arg, ": numeric argument required\n");
			ft_exit(2);
		}
		if (arg->next)
			return (ft_error("exit: too many arguments\n"),
				data->exit_code = 1, 1);
	}
	else
		exit_status = data->exit_code;
	ft_exit((unsigned char)exit_status);
	return (1);
}
