/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec_2.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/15 15:20:51 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:41:03 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/exec.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"

/*
***	- Aim of the function : to make an array of fds of output
*** - of the proper size (used_size of my redirections)
*** - I am doing so to close them easily afterwards
*** - t->outred, that I memset with the int -1
*** - For the while loop :
*** - I modify all the ionumber to the correct one, they were done,
*** - but not in a format easy enough to manage it properly ->
*** - the number that I shall redirect from
*** - if the command is : ls 5> oui => I shall redirect the fd 5
*** - to the fd of the file oui
*** - if the command is : ls > oui => I shall redirect the fd 1
*** - to the fd of the file oui
*** - if the command is : ls < oui => I shall redirect the fd 0
*** - to the fd of the file oui
*/

int			copy_fds(t_exec_redir *t, t_simp_com *cmd)
{
	int i;

	i = -1;
	if (!(t->fdoutred = (int*)malloc(sizeof(int)
		* cmd->redirection.used_space)))
		return (1);
	while (++i < cmd->redirection.used_space)
		t->fdoutred[i] = -1;
	i = -1;
	while (++i < cmd->redirection.used_space)
	{
		if (cmd->redirection.fd[i] == 1)
		{
			if (cmd->redirection.red[i] == T_LESS ||
				cmd->redirection.red[i] == T_DBL_LESS ||
					cmd->redirection.red[i] == T_DBL_LESS_DASH
						|| cmd->redirection.red[i] == T_REDIR_LESS)
				cmd->redirection.fd[i] = 0;
			else
				cmd->redirection.fd[i] = 1;
		}
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Clear all the used fds
*/

void		clear_fd(t_exec_redir *t, int end)
{
	int i;

	i = -1;
	while (++i < end)
	{
		if (t->fdoutred[i] > 2)
			close(t->fdoutred[i]);
	}
	free(t->fdoutred);
}

/*
*** - Aim of the function :
*** - Calculate the index corresponding to the last heredoc
*** - Example : cat << oui << non << plus
*** - will return : 3 which equals to "plus"
*/

void		process_redir_norm(t_exec_redir *t, int *i)
{
	if (t->file_name != NULL)
	{
		free(t->file_name);
		t->file_name = NULL;
	}
	(*i)++;
}

void		norm_for_exit(t_dlist **history, t_lexer *lex, t_command *cmd,
			t_env_tools *env)
{
	env->p.history = history;
	env->p.lex = lex;
	env->p.cmd = cmd;
}
