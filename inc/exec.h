#ifndef EXEC_H
# define EXEC_H
# include  	"../libft/libft.h"
#include "parser.h"
# include <fcntl.h>

// struct for the execution of all commands
typedef struct      s_exec_redir
{
    // int         out;
	// // struct stat s;
	// int         saved_stdout;
	// char        **files;
	// int         j;
	// int         test;
    
    // save original states of the fds
    int				fd_orig[3];
    int             *fdoutred;

    char            ***heredoc;

    // function check_op
    // int 	    aux;
    // int         f_out;
}                   t_exec_redir;

/*
*** FILE : exec_all_cases.c
*/

int		ft_first_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);
int		ft_semi_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);
int		ft_and_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);
int		ft_or_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);
void	save_original_fd(t_exec_redir *t);
void	restore_original_fd(t_exec_redir *t);


/*
*** FILE : redirections.c
*/
int			process_redirections(t_exec_redir *t, t_simp_com cmd);

#endif