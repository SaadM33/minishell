/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omar <omar@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 20:59:49 by moel-hib          #+#    #+#             */
/*   Updated: 2025/06/26 12:55:00 by omar             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// just to see where we are
// char	*build_prompt()
// {
// 	char	cwd[1000];
// 	static char	prompt[1000];

// 	getcwd(cwd, sizeof(cwd));
// 	snprintf(prompt, 1000, "┌──(minishell)-[%s]\n└─❯ ", cwd);
// 	return (prompt);
// }

volatile sig_atomic_t	g_received_signal = 0;



int	is_builtin(t_token *token)
{
	if (!token || !token->arg)
		return (0);
	if (!ft_strcmp(token->arg, "cd") || !ft_strcmp(token->arg, "export")
		|| !ft_strcmp(token->arg, "env") || !ft_strcmp(token->arg, "exit")
		|| !ft_strcmp(token->arg, "echo") || !ft_strcmp(token->arg, "pwd")
		|| !ft_strcmp(token->arg, "unset"))
		return (1);
	else
		return (0);
}

void	do_builtin(t_data *data, t_token *list, int exit_or_not)
{
	if (!data || !list || !list->arg)
		return ;
	if (ft_strcmp(list->arg, "cd") == 0)
		do_cd(data, list);
	else if (ft_strcmp(list->arg, "echo") == 0)
		do_echo(data, list);
	else if (ft_strcmp(list->arg, "env") == 0)
		do_env(data);
	else if (ft_strcmp(list->arg, "exit") == 0)
		do_exit(data, list);
	else if (ft_strcmp(list->arg, "export") == 0)
		do_export(data, list);
	else if (ft_strcmp(list->arg, "pwd") == 0)
		do_pwd(data);
	else if (ft_strcmp(list->arg, "unset") == 0)
		do_unset(data, list);
	if (exit_or_not)
		ft_exit(data->exit_code);
}

void	exec_single_builtin(t_data *data, t_cmd *cmd)
{
	int	in_save;
	int	out_save;

	in_save = dup(0);
	out_save = dup(1);
	if (cmd->in_fd != 0)
		dup2(cmd->in_fd, 0);
	if (cmd->out_fd != 1)
		dup2(cmd->out_fd, 1);
	do_builtin(data, cmd->list, 0);
	dup2(in_save, 0);
	dup2(out_save, 1);
	close(in_save);
	close(out_save);
}

void	full_execution(t_data *data)
{
	int		last_pid;
	t_cmd	*cmd;

	cmd = data->cmd_list;
	if (is_builtin(cmd->list) && !cmd->next && !cmd->skip_cmd)
		exec_single_builtin(data, cmd);
	else
	{
		while (cmd)
		{
			if (cmd->skip_cmd)
			{
				cmd = cmd->next;
				continue ;
			}
			last_pid = fork();
			if (last_pid == 0)
			{
				dup2(cmd->in_fd, 0);
				dup2(cmd->out_fd, 1);
				if (is_builtin(cmd->list))
					do_builtin(data, cmd->list, 1);
				else
					execute_cmd(data, cmd->list);
			}
			if (last_pid > 0)
				wait(&data->exit_code);
			cmd = cmd->next;
		}
		while (wait(NULL) > 0)
			;
		// if (WIFEXITED(status))
		// 	data->last_exit_code = WEXITSTATUS(status);
		// else if (WIFSIGNALED(status))
		// 	data->last_exit_code = 128 + WTERMSIG(status);
	}
	free_cmd_list(&data->cmd_list);
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(tmp, cmd);
	ft_free(tmp);
	return (full_path);
}

char	*check_in_path(t_token *list, t_data *data)
{
	char	*cmd;
	char	*full_cmd;
	char	**dirs;
	int		i;

	cmd = list->arg;
	if (access(cmd, X_OK) == 0)
		return (cmd);
	dirs = ft_split(ft_getenv("PATH", data), ':');
	i = 0;
	while (dirs[i])
	{
		full_cmd = join_path(dirs[i], cmd);
		if (access(full_cmd, X_OK) == 0)
		{
			list->arg = full_cmd;
			return (ft_free(cmd), ft_free_table(dirs), full_cmd);
		}
		ft_free(full_cmd);
		i++;
	}
	ft_free_table(dirs);
	return (cmd);
}

char	**build_argv(t_data *data, t_token *list)
{
	char	**argv;
	int		count;
	int		i;
	t_token	*tmp;

	(void) data;
	count = 0;
	tmp = list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = ft_malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (list)
	{
		argv[i] = list->arg;
		list = list->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

void	execute_cmd(t_data *data, t_token *list)
{
	char	*cmd;
	char	**argv;		

	cmd = check_in_path(list, data);
	argv = build_argv(data, list);
	if (execve(cmd, argv, data->env) == -1)
	{
		if (ft_strchr(cmd, '/'))
			printf("%s: No such file or directory\n", cmd);
		else
			printf("%s: command not found\n", cmd);
		ft_exit(127);
	}	
}
void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_received_signal = sig;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
}

void	handle_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigint_handler;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		ft_exit(1);
	signal(SIGQUIT, SIG_IGN);
}


int	main(int argc, char **argv, char **env)
{
	t_data	data;
	// char	*prompt;

	(void) argc;
	(void) argv;
	ft_bzero(&data, sizeof(data));
	data.env = ft_dupenv(env);
	handle_signals();
	while (1)
	{
		// prompt = build_prompt();
		data.str = readline("Megashell> ");
		// shrink this
		if (g_received_signal == SIGINT)
		{
			data.exit_code = 130;
			g_received_signal = 0;
		}
		if (data.str == NULL)
		{
			printf("exit\n");
			ft_exit(data.exit_code);
		}
		if (data.str[0] == '\0')
			continue;
		add_history(data.str);
		parser(&data);
		full_execution(&data);
		free_token_list(&data.token_list);
		free_cmd_list(&data.cmd_list);
	}
	return (0);
}
