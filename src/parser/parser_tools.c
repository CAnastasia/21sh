#include "../../inc/sh.h"

int		is_red(t_lexer lex, int i)
{
	if ((lex.tokens[i].type == T_GREAT || lex.tokens[i].type == T_LESS ||
		lex.tokens[i].type == T_DBL_GREAT || lex.tokens[i].type == T_DBL_LESS))
		return (1); 
	return (0);
}
int is_built_in(t_command cmd, int i)
{
	//dprintf(2, "AVANT-passe dans builtin: |%d| - |%s| - |%d|\n\n", cmd.command[i].used_space, cmd.command[i].cmd_simple[0], i);
	if (cmd.command[i].used_space > 0 && cmd.command[i].cmd_simple != NULL)
	{
//		dprintf(2, "passe dans builtin: |%d| - |%s| - |%d|\n", cmd.command[i].used_space, cmd.command[i].cmd_simple[0], i);
		if ((ft_strcmp("echo", cmd.command[i].cmd_simple[0]) == 0) ||
		!ft_strcmp("setenv", cmd.command[i].cmd_simple[0]) ||
		!ft_strcmp("unsetenv", cmd.command[i].cmd_simple[0]) ||
		!ft_strcmp("cd", cmd.command[i].cmd_simple[0]) ||
		!ft_strcmp("env", cmd.command[i].cmd_simple[0]))
			return (1);
	}
	return (0);
}

int		is_op(t_lexer lex, int i)
{
	if (lex.tokens[i].type >= T_DBLAND && lex.tokens[i].type <= T_AND)
		return (1);
	return(0);
}

int		parse_errors(t_lexer lex)
{
	int i;

	i = -1;
	while(++i < lex.used_size)
	{
		if ((is_op(lex, i) && i == 0) || lex.tokens[i].type == T_DBL_SEMI)
		{
			ft_putstr_fd("bash: syntax error near unexpected token ", 2);
			ft_putendl_fd(lex.tokens[i].content, 2);
			return (258);
		}
		if ((is_red(lex, i) && (is_red(lex, i + 1) || is_op(lex, i + 1))) ||
		(is_op(lex, i) && is_op(lex, i + 1)))
		{
			ft_putstr_fd("bash: syntax error near unexpected token ", 2);
			ft_putendl_fd(lex.tokens[i + 1].content, 2);
			return (258);
		}
		if (is_red(lex, i) && lex.tokens[i + 1].type != T_WORD)
		{
			ft_putendl_fd("bash: syntax error near unexpected token `newline'", 2);
			return (258);
		}
	}
	return (1);
}
