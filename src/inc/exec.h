/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 00:12:59 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/12 22:03:27 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "../libft/libft.h"
# include "parser.h"
# include "builtin.h"
# include <fcntl.h>

typedef struct	s_pipe
{
	int			fds[2];
	int			fd_in;
	int			aux;
	pid_t		pid;
	int			len_pipe;

}				t_pipe;

/*
*** - struct for the execution of all commands
*/

typedef struct	s_exec_redir
{
	int			fd_orig[3];
	int			*fdoutred;
	char		***heredoc;
	int			i_hdoc;
	t_pipe		pipe_tools;
	char		*file_name;
	int			count_here;
}				t_exec_redir;

/*
*** - FILE : exec_all_cases.c
*/

int				ft_first_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
int				ft_semi_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
int				ft_and_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
int				ft_or_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
void			save_original_fd(t_exec_redir *t);
void			restore_original_fd(t_exec_redir *t);
int				manage_sig_term_ret_1(int ret);
int				right_return(int status);

/*
*** - FILE : redirections.c
*/

int				process_redirections(t_exec_redir *t, t_simp_com cmd,
					t_env_tools *env, int k);
void			clear_fd(t_exec_redir *t, int end);
int				copy_fds(t_exec_redir *t, t_simp_com *cmd);

/*
*** - pipe
*/

int				ft_pipe_exec(t_env_tools *env, t_command cmd, int *i,
					t_exec_redir *t);

/*
*** - exec_utils.c
*/

int				error_command(char *part, char **str, char *str2, int ret);

/*
*** - exec_2.c
*/

int				manage_aggreg(t_simp_com cmd, int i, t_exec_redir *t);
int				ft_perror_norm_dir(char *str1, int fd);
int				ft_print_error_directory(char *str, char *str1, char *str2,
					int fd);
int				manage_here_doc(t_simp_com cmd, int i, t_exec_redir *t, int k);

/*
*** - utils_fd.c
*/

int				ft_calcul_pos_last_heredoc(t_exec_redir *t, t_simp_com *cmd);

/*
*** - exec_norm.c
*/
void			init_red_struct(t_exec_redir *t_red, char ***heredoc);
int				expand_filename(t_simp_com cmd, t_exec_redir *t, int i,
					t_env_tools *env);
void			process_redir_norm(t_exec_redir *t, int *i);
void			norm_for_exit(t_dlist **history, t_lexer *lex, t_command *cmd,
				t_env_tools *env);

/*
*** - env_from_file.c
*/

void			check_op_norm(t_exec_redir *t, int i, t_command cmd);
int				is_here(t_token_type *red, int size);

#endif
