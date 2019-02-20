/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/15 15:14:08 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include "../../inc/exec.h"

void	check_op_first_exec(t_command cmd, t_env_tools *env, t_exec_redir *t,
			int *i)
{
	save_original_fd(t);
	if (cmd.command[*i].tok == T_PIPE)
		ft_pipe_exec(env, cmd, i, t);
	else
	{
		ft_first_exec(env, cmd, 0, t);
		if (cmd.used_space > 1 && cmd.command[*i + 1].tok != T_PIPE)
			(*i)--;
	}
	restore_original_fd(t);
}
void	init_red_struct(t_exec_redir *t_red, char ***heredoc)
{
	t_red->i_hdoc = 0;
	t_red->file_name = NULL;
	t_red->fdoutred = 0;
	t_red->heredoc = heredoc;
}
void	check_op(t_command cmd, t_env_tools *env, char ***heredoc)
{
	t_exec_redir	t;
	int				i;

	i = 0;
	init_red_struct(&t, heredoc);
	check_op_first_exec(cmd, env, &t, &i);
	while (++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		if (i == cmd.used_space - 1 || cmd.command[i + 1].used_space == 0)
			break ;
		save_original_fd(&t);
		if (cmd.command[i].tok == T_DBLOR)
		{
			ft_or_exec(env, cmd, i + 1, &t);
			i++;
		}
		else if (cmd.command[i].tok == T_PIPE)
			ft_pipe_exec(env, cmd, &i, &t);
		else if (cmd.command[i].tok == T_DBLAND)
			ft_and_exec(env, cmd, i + 1, &t);
		else if (cmd.command[i].tok == T_SEMI &&
		cmd.command[i + 1].tok != T_PIPE)
			ft_semi_exec(env, cmd, i + 1, &t);
		restore_original_fd(&t);
	}
}

void	norm_all_exec(t_env_tools *env, t_command cmd, char ***heredoc)
{
	if (g_keeprun == 1 || g_keeprun == 2)
		env->g_return_value = 1;
	else if (g_keeprun == 5)
		env->g_return_value = 258;
	if (!g_keeprun || g_keeprun == 4)
		check_op(cmd, env, heredoc);
	if (*heredoc)
	{
		ft_free_av(*heredoc);
		*heredoc = NULL;
	}
}

void	all_exec(char **environ, char ***heredoc)
{
	t_dlist			*history;
	t_lexer			lex;
	t_command		cmd;
	t_env_tools		env;

	env.env_cpy = copy_env(environ, size_str(environ), &env);
	path_str(env.env_cpy, &env.paths);
	history = ft_dlstnew(NULL);
	g_keeprun = 0;
	while (42)
	{
		lex = final_tokens(&history);
		command_init(&cmd);
		add_simple_command(&cmd, lex, &history, heredoc);
		norm_all_exec(&env, cmd, heredoc);
		free_the_op_content_array_token(&lex);
		free_struct(&cmd);
	}
	if (env.env_cpy != NULL)
		free_str(env.env_cpy);
	ft_dlstdel(&history);
}

int		main(int argc, char **argv, char **environ)
{
	t_term		term;
	char		**heredoc;

	(void)argc;
	(void)argv;
	heredoc = NULL;
	if (terminal_data(&term) < 0)
	{
		ft_putstr_fd("envoie les bonnes infos ma gueule !!!\n", 2);
		exit(0);
	}
	all_exec(environ, &heredoc);
	return (0);
}
