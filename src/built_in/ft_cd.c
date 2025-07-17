/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-maaq <sel-maaq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:59:36 by obentahi          #+#    #+#             */
/*   Updated: 2025/07/17 21:06:56 by sel-maaq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_args(t_token *token_list)
{
	int	i;

	i = 0;
	while (token_list)
	{
		i++;
		token_list = token_list->next;
	}
	return (i - 1);
}

static char	*get_cd_target(t_data *data, t_token *list)
{
	char	*arg;

	if (list->next)
		arg = list->next->arg;
	else
		arg = NULL;
	if (!arg)
	{
		arg = ft_getenv("HOME", data);
		if (!arg)
			ft_error("cd: HOME not set\n");
		data->exit_code = 1;
	}
	else if (!ft_strcmp(arg, "-"))
	{
		arg = ft_getenv("OLDPWD", data);
		if (!arg)
			ft_error("cd: OLDPWD not set\n");
		data->exit_code = 1;
	}
	return (arg);
}

static void	update_pwd(t_data *data, char *old_pwd, char *cd_arg)
{
	char	new_pwd[4096];
	char	*tmp;

	ft_setenv(&data->env, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, sizeof(new_pwd)))
	{
		data->cwd = ft_strdup(new_pwd);
		ft_setenv(&data->env, "PWD", data->cwd);
	}	
	else
	{
		tmp = ft_strjoin3(data->cwd, "/", cd_arg);
		ft_setenv(&data->env, "PWD", tmp);
		data->cwd = ft_strdup(tmp);
		ft_free(tmp);
		perror("cd: error retrieving current directory: "
			"getcwd: cannot access parent directories");
	}
}

int	do_cd(t_data *data, t_token *list)
{
	char	old_pwd[4096];
	char	*cd_arg;

	if (count_args(list) > 1)
		return (ft_error("cd: too many arguments\n"),
			data->exit_code = 1, 1);
	cd_arg = get_cd_target(data, list);
	if (!cd_arg)
		return (data->exit_code = 1, 1);
	ft_strlcpy(old_pwd, data->cwd, 4096);
	if (chdir(cd_arg) == -1)
	{
		data->exit_code = 1;
		perror("chdir");
		return (1);
	}
	update_pwd(data, old_pwd, cd_arg);
	return (1);
}
