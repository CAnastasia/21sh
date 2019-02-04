/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:22 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/builtin.h"

/*
*** - Aim of the function :
*** - Print usage error and returns the proper int
*/

int		ft_usage_error(char *command, char *btwn, char *after, int i)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(btwn, 2);
	ft_putstr_fd(command, 2);
	ft_putendl_fd(after, 2);
	return (i);
}

/*
*** - Aim of the function :
*** - ft_putstr inside int function
*** - in order to use it inside if statement
*/

int		ft_int_error(char *str, int fd, int ret)
{
	ft_putstr_fd(str,fd);
	return (ret);
}

/*
*** - Aim of the function :
*** - Several ft_putstr inside int function
*** - in order to use it inside if statement
*/

int		ft_int_print_error(char *str, char *str2, char *str3, int fd)
{
	ft_putstr_fd(str, fd);
	ft_putstr_fd(str2, fd);
	ft_putstr_fd(str3, fd);
	return (0);
}

/*
*** - Aim of the function :
*** - Returns the len of the char **
*/

int		ft_len_array_char(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	return (i);
}

int		ft_usage_is_good(char *limitor, char *str)
{
	int		i;
	int		j;
	
	i = 0;
	j = -1;
	if (ft_strlen(str) > 1)
	{
		if (str[0] == '-' && (i = i + 1))
		{
				while (limitor[++j])
				{
					while (str[i])
					{
						if (limitor[j] == str[i])
							i++;
						else
							break ;
					}
				}
		}
		if (str[i])
			return (1);
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Function directs to the proper builtin
*** - and returns the proper int
*/

int		ft_exec_command(t_env_tools *env, char **cmd)
{
	if (cmd && cmd[0])
	{
		if (!ft_strcmp("echo", cmd[0])) // gérer le cas où VAR $HOME n'existe plus
			return (ft_builtin_echo(cmd));
		else if (!ft_strcmp("setenv", cmd[0]))
			return (ft_builtin_setenv(cmd, &(env->env_cpy), env));
		else if (!ft_strcmp("unsetenv", cmd[0]))
			return (ft_builtin_unsetenv(cmd, &(env->env_cpy), &(env->paths), env));
		else if (!ft_strcmp("cd", cmd[0]))
			return (ft_builtin_cd(cmd, &(env->env_cpy), env));
		else if (!ft_strcmp("env", cmd[0]))
			return (ft_builtin_env(cmd, &(env->env_cpy), &(env->paths), env));
		else if (!ft_strcmp("exit", cmd[0]))
			return (ft_builtin_exit(cmd));
		else
			return (error_exec_or_exec(env->paths, cmd, env->env_cpy, 0));
	}
	else
		return (error_exec_or_exec(env->paths, cmd, env->env_cpy, 0));
		// return (5);
	return (0);
}