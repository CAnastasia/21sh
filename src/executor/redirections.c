/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/28 14:00:12 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/exec.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"

int			fnorm_print_error(t_simp_com cmd, int i)
{
	return (ft_print_error_directory("bash: ",
			cmd.redirection.file[i], ": No such file or directory", 2));
}

/*
*** - Aim of the function :
*** - Process the fds redirections in order to process the manage file function
*/

static int	manage_file_norm(t_simp_com cmd, int i, t_exec_redir *t,
				struct stat *buf)
{
	if (cmd.redirection.red[i] == T_GREAT)
	{
		if (S_ISDIR((*buf).st_mode))
			return (ft_perror_norm_dir(cmd.redirection.file[i], 2));
		t->fdoutred[i] = open(t->file_name,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (t->fdoutred[i] < 0)
			return (fnorm_print_error(cmd, i));
	}
	else if (cmd.redirection.red[i] == T_DBL_GREAT)
	{
		if (S_ISDIR((*buf).st_mode))
			return (ft_perror_norm_dir(cmd.redirection.file[i], 2));
		t->fdoutred[i] = open(t->file_name,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (cmd.redirection.red[i] == T_LESS)
	{
		t->fdoutred[i] = open(t->file_name, O_RDONLY);
		if (t->fdoutred[i] < 0)
			return (fnorm_print_error(cmd, i));
	}
	else if (cmd.redirection.red[i] == T_REDIR_LESSGREAT)
		t->fdoutred[i] = open(t->file_name, O_RDWR);
	return (0);
}

void		norm_manage_file(char **tmp, char **tmp2, t_exec_redir *t,
				struct stat	*buf)
{
	char		pbuf[1024];

	getcwd(pbuf, sizeof(pbuf));
	*tmp = ft_strjoin(pbuf, "/");
	*tmp2 = ft_strjoin(*tmp, t->file_name);
	free(*tmp);
	lstat(*tmp2, buf);
}

/*
*** - Aim of the function :
*** - Handle [n]>[word], [n]>>[word], [n]<[word], [n]<>[word]
*/

static int	manage_file(t_simp_com cmd, int i, t_exec_redir *t,
				t_env_tools *env)
{
	struct stat	buf;
	char		*tmp;
	char		*tmp2;

	buf.st_mode = 0;
	if (expand_filename(cmd, t, i, env))
		return (1);
	norm_manage_file(&tmp, &tmp2, t, &buf);
	if (manage_file_norm(cmd, i, t, &buf) && !ft_free(tmp2))
		return (1);
	if (t->fdoutred[i] < 0)
	{
		if (!S_ISDIR(buf.st_mode) && !ft_free(tmp2))
			return (ft_print_error_directory("bash: ",
				cmd.redirection.file[i], ": Permission Denied", 2));
	}
	if (dup2(t->fdoutred[i], cmd.redirection.fd[i]) < 0 && !ft_free(tmp2))
	{
		return (ft_print_error_directory("bash: ",
		cmd.redirection.file[i], ": Is a directory", 2));
	}
	free(tmp2);
	return (0);
}

/*
*** - Aim of the function :
*** - Redirect to the proper fd
*/

int			process_redirections(t_exec_redir *t, t_simp_com cmd,
				t_env_tools *env)
{
	int		i;
	int		ret;
	int		pos_heredoc;
	int		count_red;

	i = 0;
	count_red = 0;
	ret = 0;
	pos_heredoc = ft_calcul_pos_last_heredoc(t, &cmd);
	int j = -1;
	while (j++ < cmd.redirection.used_space)
	{
		dprintf(1, " cmd.redirection.red[j]: %d|", cmd.redirection.red[j]);
	}
	while (i < cmd.redirection.used_space && ret == 0)
	{
		dprintf(1, "index_here : %d\n", pos_heredoc );
		dprintf(1, "i : %d\n", i);
		if ((cmd.redirection.red[i] == T_REDIR_LESS
			|| cmd.redirection.red[i] == T_REDIR_GREAT))
		{
			count_red++;
			ret = manage_aggreg(cmd, i, t);
		}
		else if ((cmd.redirection.red[i] == T_DBL_LESS
			|| cmd.redirection.red[i] == T_DBL_LESS_DASH))
		{

			if (i == pos_heredoc + count_red)
			{
				ret = manage_here_doc(cmd, i, t);
				
			}
		}
		else
		{
			count_red++;
			ret = manage_file(cmd, i, t, env);	
		}
		process_redir_norm(t, &i);
	}
	clear_fd(t, cmd.redirection.used_space);
	return (ret);
}
