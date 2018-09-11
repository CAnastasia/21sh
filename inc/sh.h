#ifndef SH_H
# define SH_H

# include   "lexer.h"
# include   "parser.h"
# include	"termcap.h"
# include	<unistd.h>
# include	<sys/wait.h>
# include	<sys/types.h>
# include	<stdlib.h>
# include	<stdio.h>
# include	<string.h>

typedef struct		s_env_tools
{
	char			**env_cpy;
	char			**paths;
}					t_env_tools;

int		size_str(char **environ);
char	**copy_env(char **environ, int size);
void	set_path(char *str, char ***paths);
void	path_str(char **envs, char ***paths);
int		exec(char *path, char **str, char **env);
int		error_exec_or_exec(char **paths, char *path, char **str, char **env);
void	free_str(char **str);
void	free_struct(t_command *cmd, t_lexer lex);
void	free_str_2(char **str, int size);
void      free_the_op_content_array_token(t_lexer *lexer);
char	**expense_cmd(t_command cmd, t_env_tools env, int i);
char	*env_var(t_env_tools env, char *str, int size);
int		size_env_var(char *tmp, int i);
int size_var(char *str);


#endif
