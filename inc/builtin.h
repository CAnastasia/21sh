#ifndef BUILTIN_H
# define BUILTIN_H
#include "sh.h"


# include "../libft/libft.h"
# include <sys/stat.h>
# include <sys/types.h>

// # include "sh.h"

typedef struct		s_norm_cd
{
    int         begin;
    char        c;
    int         p;
    int         argc;
    // char        **av;
    int         dash;
}                   t_norm_cd;

typedef struct      s_nom_pwd
{
    int         dash;
    char	    *tmp;
    char        *tmp2;
	char	    buf[1024];
    struct stat buf2;
    int         i;
    char        *s1;
    char        *s2;
    int         p;
}                   t_norm_pwd;

// typedef struct		s_env_tools
// {
// 	char			**env_cpy;
// 	char			**paths;
// 	int				g_return_value;
// }					t_env_tools;

int		ft_len_array_char(char **av);
int		ft_exec_builtin(t_env_tools *env, char **cmd);
int	    ft_builtin_echo(char **av, char ***c_env);
int		ft_int_error(char *str, int fd, int ret);
int		ft_int_print_error(char *str, char *str2, char *str3, int fd);
void	ft_exchange_chars(char **av, char delimitor, char to_replace);
int		ft_str_is_alpha_setenv(char *str);
int     ft_free_av(char **av);
void	ft_swap_string(char **ret, char **tmp, char **c_env);
void	ft_print_env(char ***c_env);
int		ft_builtin_setenv(char **av, char ***c_env, char ***paths, t_env_tools *env);
char	*ft_strdup_without_quotes(char *av);
int		ft_builtin_unsetenv(char **av, char ***c_env, char ***paths, t_env_tools *env);
char	*ft_array_char_to_str(char **c_env, int avoid);
int		ft_free(char *av);
int		ft_usage_error(char *command, char *btwn, char *after, int i);

/*
*** - setenv.c
*/

void		ft_builtin_setenv_2(char *av, char ***c_env, char ***paths, t_env_tools *env);

/*
*** - cd2.c
*/

int	    	ft_change_dir_and_pwds(char *av, char ***c_env, t_env_tools *env, t_norm_cd *n);
// int         ft_manage_dot(char *av, char ***c_env, t_env_tools *env);


/*
*** - cd.c
*/

int			ft_builtin_cd(char **av, char ***c_env, t_env_tools *env);

#endif