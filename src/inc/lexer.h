/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 00:36:30 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/06 00:49:04 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "../libft/libft.h"
# include "termcap.h"

# define LEXER_INITIAL_CAPACITY 100

typedef enum		e_token_type
{
	T_EAT,
	T_NEG,
	T_DBLAND,
	T_DBL_SEMI,
	T_DBLOR,
	T_PIPE,
	T_SEMI,
	T_AND,
	T_OP_ARITHM,
	T_OP_BRA,
	T_CL_BRA,
	T_OP_PAR,
	T_CL_PAR,
	T_GREAT,
	T_DBL_GREAT,
	T_REDIR_LESSGREAT,
	T_LESS,
	T_DBL_LESS,
	T_DBL_LESS_DASH,
	T_TRL_LESS,
	T_REDIR,
	T_IO_NUMB,
	T_OP_CMD,
	T_OP_CURLY,
	T_CL_CURLY,
	T_REDIR_LESS,
	T_REDIR_GREAT,
	T_WORD,
}					t_token_type;

typedef struct			s_oplist
{
	const char			*op;
	int					size;
	t_token_type		type;
}						t_oplist;

typedef struct			s_lexer_token
{
	char				*content;
	int					size;
	t_token_type		type;
}						t_lexer_token;

typedef struct			s_lexer
{
	t_lexer_token		*tokens;
	int					used_size;
	int					capacity;
}						t_lexer;

/*
*** - struct only built for the norm
*/

typedef struct			s_norm
{
	t_oplist			current;
	int					quote_done;
	const char			*prev;
	char				type_quote;
	const char			*start;
	const char			*end;
}						t_norm;

static const t_oplist	g_existing_token[] = {
	{"$((", 3, T_OP_ARITHM},
	{"<<<", 3, T_TRL_LESS},
	{"<<-", 3, T_DBL_LESS_DASH},
	{"$(", 2, T_OP_CMD},
	{"${", 2, T_OP_CURLY},
	{">>", 2, T_DBL_GREAT},
	{"<<", 2, T_DBL_LESS},
	{"||", 2, T_DBLOR},
	{">|", 2, T_REDIR},
	{"<>", 2, T_REDIR_LESSGREAT},
	{"<&", 2, T_REDIR_LESS},
	{">&", 2, T_REDIR_GREAT},
	{"&&", 2, T_DBLAND},
	{";;", 2, T_DBL_SEMI},
	{"! ", 2, T_NEG},
	{"}", 1, T_CL_BRA},
	{"|", 1, T_PIPE},
	{"&", 1, T_AND},
	{"(", 1, T_OP_PAR},
	{")", 1, T_CL_PAR},
	{">", 1, T_GREAT},
	{"<", 1, T_LESS},
	{"(", 1, T_OP_PAR},
	{")", 1, T_CL_PAR},
	{"{", 1, T_OP_CURLY},
	{"}", 1, T_CL_CURLY},
	{";", 1, T_SEMI},
	{" ", 1, T_EAT},
	{"\n", 1, T_EAT},
	{"\v", 1, T_EAT},
	{"\t", 1, T_EAT},
	{"\r", 1, T_EAT},
	{"\f", 1, T_EAT},
	{NULL, 1, 0}
};

/*
*** - quote_management.c
*/

int						manage_back_quote(const char *s, const char *begin);
void					ft_find_closing_quote(const char **s, t_norm *nm);
void					ft_tokenize_quote_management(const char **s,
							t_lexer *lexer, t_norm *nm);
char					ft_count_quote(char *str);

/*
*** - collect_lines.c
*/

void					ft_get_entire_line(char **cmd, char *str,
							t_dlist **history);
void					ft_new_prompt(char **cmd, char type_quote,
							t_dlist **history, t_tcap *caps);
char					*ft_manage_prompt(char type_quote);

/*
*** - utils_lexer.c
*/

void					free_the_content_array_token(t_lexer *lexer);
void					lexer_init(t_lexer *lexer);

/*
*** - tokenization.c
*/

void					add_token_to_lexer(t_lexer *lexer, const char *text,
							int text_size, t_token_type type);
t_oplist				type_of_token(const char *s);
void					ft_string_to_lexer_advance(const char **s,
							t_lexer *lexer, t_norm *nm);
void					ft_initialize_nm(const char *s, t_norm *nm);
int						string_to_lexer(const char *s, t_lexer *lexer);

/*
*** - lexer.c
*/

t_lexer					final_tokens(t_dlist **history);
int						ft_manage_string_to_lexer(const char *s,
							t_lexer *lexer, t_dlist **history);
void					display_bash(char *str);

/*
*** - norm_utils.c
*/

int						norm_quote_end(const char **s, t_norm *nm);

#endif
