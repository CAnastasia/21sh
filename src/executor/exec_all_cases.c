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
#include "../../inc/exec.h"

void	save_original_fd(t_exec_redir *t)
{
	int i;

	i = -1;
	while (++i < 3)
        // t->fd_orig[i] = i;
		// t->fd_orig[i] = dup(i);
        t->fd_orig[i] = dup2(i, 1500 + i);
}

void	restore_original_fd(t_exec_redir *t)
{
	int		i;
    
    i = -1;
	while (++i < 3)
	{
		dup2(t->fd_orig[i], i);
		close(t->fd_orig[i]);
	}
}

int		ft_or_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    cmd_expended = expense_cmd(cmd, *env, i + 1);
    if (env->g_return_value > 0)
    {
        if (cmd.command[i + 1].redirection.used_space > 0)
            process_redirections(t,cmd.command[i]);
        env->g_return_value = ft_exec_command(env, cmd_expended);
    }
    free_str(cmd_expended);
    return (0);
}

int		ft_and_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    cmd_expended = expense_cmd(cmd, *env, i + 1);
    if (env->g_return_value == 0)
    {
        if (cmd.command[i + 1].redirection.used_space > 0)
            process_redirections(t,cmd.command[i]);
        env->g_return_value = ft_exec_command(env, cmd_expended);
    }
    free_str(cmd_expended);
    return (0);
}

int		ft_semi_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    cmd_expended = expense_cmd(cmd, *env, i + 1);
    if (cmd.command[i + 1].redirection.used_space > 0)
        process_redirections(t,cmd.command[i]);
    env->g_return_value = ft_exec_command(env, cmd_expended);
    free_str(cmd_expended);
    return (0);
}

int		ft_first_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    if (cmd.used_space > 0)
	{
        cmd_expended = expense_cmd(cmd, *env, 0);
	    // premiere redirections
	    if (cmd.command[0].redirection.used_space > 0)
            process_redirections(t,cmd.command[i]);
	    // execution de la premiere commande
	    if (cmd_expended != NULL && cmd.used_space > 0)
		    env->g_return_value = ft_exec_command(env, cmd_expended);
	    free_str(cmd_expended);
    }
    return (0);
}