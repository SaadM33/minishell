/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirenment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:00:27 by omar              #+#    #+#             */
/*   Updated: 2025/06/25 00:28:17 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_dupenv(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = ft_calloc(i + 1, sizeof(char *));
	if (!new_env)
		(perror("minishell"), exit(errno));
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			(/*free_d_arr(new_env), */perror("minishell"), exit(errno));
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	ft_addvar(char *new, t_data *data)
{
	int		i;
	char	**new_env;

	i = 0;
	while (data->env[i])
		i++;
	new_env = ft_malloc(sizeof(char *) * (i + 2), 0);
	if (!new_env)
		return (errno);
	i = 0;
	while (data->env[i] && data->env[i + 1])
	{
		new_env[i] = data->env[i];
		i++;
	}
	new_env[i] = ft_strdup(new);
	if (!new_env[i])
		return (ft_free(new_env), errno);
	new_env[i + 1] = data->env[i];
	new_env[i + 2] = NULL;
	ft_free(data->env);
	data->env = new_env;
	return (0);
}

int	ft_expvar(char *var, t_data *data)
{
	int		i;
	int		len;
	char	*tmp;

	len = 0;
	while (var && var[len] && var[len] != '=')
		len++;
	len++;
	i = 0;
	while (data->env[i] && ft_strncmp(var, data->env[i], len))
		i++;
	if (!data->env[i])
		return (ft_addvar(var, data));
	tmp = ft_strdup(var);
	if (!tmp)
		return (errno);
	free(data->env[i]);
	data->env[i] = tmp;
	return (0);
}

char	*ft_getenv(char *var, t_data *data)
{
	int		i;
	char	*out;

	i = 0;
	out = NULL;
	if (!ft_strcmp(var, "?"))
		return (ft_itoa(data->exit_code));
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], var, ft_strlen(var)) &&
			data->env[i][ft_strlen(var)] == '=')
			out = ft_strdup(data->env[i] + ft_strlen(var) + 1);
		i++;
	}
	if (!out)
		out = ft_strdup("");
	return (out);
}

int	ft_remvar(char *var, t_data *data)
{
	int		i;
	// char	*out;

	i = 0;
	// out = NULL;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], var, ft_strlen(var)) &&
			data->env[i][ft_strlen(var)] == '=')
		{
			free(data->env[i]);
			i++;
			while (data->env[i])
			{
				data->env[i - 1] = data->env[i];
				i++;
			}
			data->env[i - 1] = data->env[i];
			return (0);
		}
		i++;
	}
	return (0);
}
