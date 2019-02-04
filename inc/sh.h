#ifndef SH_H
# define SH_H

// # include  	"../libft/libft.h"
# include	<unistd.h>
# include	<sys/wait.h>
# include	<sys/types.h>
# include	<stdlib.h>
# include	<stdio.h>
# include	<string.h>
# include <sys/stat.h>
// # include  	"../libft/libft.h"
// # include   "lexer.h"
# include   "parser.h"
# include	"termcap.h"

typedef struct		s_env_tools
{
	char			**env_cpy;
	char			**paths;
	char			*home;
	int				g_return_value;
}					t_env_tools;

int		size_str(char **environ);
// char	**copy_env(char **environ, int size);
char	**copy_env(char **environ, int size, t_env_tools *env);
void	set_path(char *str, char ***paths);
// void	cpy_home(t_env_tools *env);
void	path_str(char **envs, char ***paths);
int		exec(char *path, char **str, char **env);
// int		error_exec_or_exec(char **paths, char *path, char **str, char **env);
int		error_exec_or_exec(char **paths, char **str, char **env);
void	free_str(char **str);
// void	print(const t_lexer *lexer);

/*
*** - free_functions.c
*/

void	free_struct(t_command *cmd);//, t_lexer lex
void    free_the_op_content_array_token(t_lexer *lexer);

void	free_str_2(char **str, int size);
char	**expense_cmd(t_command cmd, t_env_tools env, int i);
char	*env_var(t_env_tools env, char *str, int size);
int		size_env_var(char *tmp, int i);
int		size_var(char *str);

/*
*** - heredoc.c
*/

int		ft_initialize_heredoc(t_lexer *lexer, char ***heredoc,
			int other_command, int previous_command);
int		ft_find_end_command_and_nb_kewyords(t_hdoc *h, t_lexer *lexer);
int		realloc_heredoc(t_hdoc *h, char ***heredoc);

/*
*** - heredoc.c
*/

int		ft_manage_heredoc(t_lexer *lexer, char ***heredoc, t_dlist **history);

#endif
