/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/07 17:58:18 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"

void	path_str(char **envs, char ***paths)
{
	*paths = ft_find_path_and_split(envs);
}


void	check_path_loop(char **path_env, char **path, char **str, int *count)
{
	int		j;
	char	*join_slash;
	char	*join_cmd;
	
	j = 0;
	while (path_env[j])
	{
		join_slash = ft_strjoin(path_env[j], "/");
		join_cmd = ft_strjoin(join_slash, *str);
		free(join_slash);
		if (access(join_cmd, F_OK) == 0)
		{
			if (*count >= 1)
				free(*path);
			*path = ft_strdup(join_cmd);
			(*count)++;
		}
		j++;
		free(join_cmd);
	}
	if (!(*path))
		*path = NULL;
}

int		check_path(char **path_env, char **path, char **str)
{
	int	count;

	count = 0;
	if (path_env != NULL)
		check_path_loop(path_env, path, str, &count);
	return (count);
}

int	error_command(char *part, char **str, char *str2, int ret)
{
	ft_putstr_fd(part, 2);
	ft_putstr_fd(*str, 2);
	ft_putendl_fd(str2, 2);
	return (ret);
}

int		exec(char *path, char **str, char **env)
{
	pid_t	pid;
	int 	status;
	int		res;

	res = 0;
	if ((pid = fork()))
	{
		if (pid == -1)
			return (-1);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			res = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			res = manage_sig_term_ret_1(WTERMSIG(status));
		if (res > 0)
			return(res);	
	}
	else
	{
		execve(path, str, env);
		exit(EXIT_FAILURE);
	}
	return (0);
}


// function tells if the word after the >& && <& is a redir number

int		ft_isnumber_redir(char *str)
{
	dprintf(3, "rentre dans ft_isnumber_redir\n");
	int i;

	i = -1;
	if (!ft_strcmp(str, "-"))
		return (0);
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (1);
	return (0);
}
// Place it somewhere else for the norm

int		check_errors_exec(char *path, char **str)
{
	int res;
	struct stat buf;
	
	res = 0;
	if (access(path, F_OK)) // no such file or directory
		res = error_command("bash: ", str,
			": No such file or directory", 127);
	// return (msh_nosuchfile(cmd));
	else if (!(stat(path, &buf) == -1)) // À tester => Check les directory
	{
		if ((buf.st_mode & S_IFMT) == S_IFDIR) // À tester
			res = error_command("bash: ", str, ": is a directory", 126);
	}
	// realloc la commande
	else if ((stat(path, &buf) == 0 && buf.st_mode & S_IXUSR) == 0) // permission denied
		res = error_command("bash: ", str, ": permission denied", 126);
	return (res);
}

int		error_exec_or_exec(char **paths, char **str,
		char **env) // insérer booléen précisant si dans env ou non
{
	int			res;
	char		*path;

	path = NULL;
	// Find the proper path
	if (str[0] && ft_strchr(str[0], '/'))
		path = ft_strdup(str[0]);
	else
		check_path(paths, &path, str);
	if (path)
	{
		if ((res = check_errors_exec(path, str)))
			;
		else
			res = exec(path, str, env);
	}
	else
		res = error_command("bash: ", str,
			": command not found", 127);
	if (path != NULL)
		free(path);
	dprintf(3, "return res: %d\n", res);
	return (res);
}
