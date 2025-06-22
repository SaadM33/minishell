/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obentahi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 22:16:17 by moel-hib          #+#    #+#             */
/*   Updated: 2025/06/22 23:39:17 by obentahi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>

/*     MACROS     */

# ifndef MINISHELL_MACRO
#  define MINISHELL_MACRO
#  define DF 0 // default
#  define SQ 1 // single quotes
#  define DQ 2 // double quotes
#  define OP 3 // opperators (<<, >>, <, >)
#  define PP 4 // pipe
# endif


typedef struct s_token
{
	char			*arg;
	int				type;
	int				append;
	
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	t_token			*list;

	int				in_fd;
	int				out_fd;

	int				skip_cmd;

	struct t_cmd	*next;
	struct t_cmd	*prev;
}	t_cmd;

typedef struct s_data
{
	char	**env;
	char	*str;


	t_token	*token_list;

	// t_cmd	*cmd_list;
	int		last_exit_code;

}	t_data;

/* Signal func */
void	handle_signals(void);
void	sigint_handler(int sig);

/* Parsing */
void	parser(t_data *data);
int		lexer(t_data *data);
void	syntax_checker(t_data *data);

/* built in functions */
int		is_builtin(t_data *data);
void	do_builtin(t_data *data);

int		do_cd(t_data *data);
int		do_echo(t_data *data);
int		do_env(t_data *data);
int		do_exit(t_data *data);
int		do_export(t_data *data);
int		do_pwd(t_data *data);
int		do_unset(t_data *data);

/* env functions */
char	**copy_env(char **env);
void	sort_env(char **env);
void	ft_setenv(char ***env, char *key, char *val);
char	*ft_getenv(char *key, t_data *data);

/* Execution! */
void	execute_cmd(t_data *data);

/* clean-up functions */
void	free_d_arr(char **arr);
void	free_token_list(t_token **head);

/* Linked list stuff*/
void	add_token_node(t_token **head, char *arg, int type, int append);

#endif
