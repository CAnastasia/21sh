/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:57:36 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

/*
*** - Aim of the function :
*** - Initialize the array of pointers function
*/

t_tab		*tab_termcaps(void)
{
	static t_tab ttab[18] = {
		{&left_key, 27, 91, 68, 0, 0, "le"},
		{&right_key, 27, 91, 67, 0, 0, "nd"},
		{&del_key, 127, 0, 0, 0, 0, "del"},
		{&home_key, 27, 91, 72, 0, 0, "home"},
		{&end_key, 27, 91, 70, 0, 0, "end"},
		{&alt_up_key, 27, 27, 91, 65, 0, "alt_up"},
		{&alt_down_key, 27, 27, 91, 66, 0, "alt_down"},
		{&up_key, 27, 91, 65, 0, 0, "up"},
		{&down_key, 27, 91, 66, 0, 0, "down"},
		{&alt_right_key, 27, 27, 91, 67, 0, "alt_right"},
		{&alt_left_key, 27, 27, 91, 68, 0, "alt_left_key"},
		{&ctrl_l, 12, 0, 0, 0, 0, "ctrl_l"},
		{&alt_x, -30, -119, -120, 0, 0, "alt_x"},
		{&alt_s, -61, -110, 0, 0, 0, "alt_s"},
		{&alt_w, -30, -128, -71, 0, 0, "alt_w"},
		{&alt_p, -49, -128, 0, 0, 0, "alt_p"},
		{&ctrl_dnorm, 4, 0, 0, 0, 0, "ctrl_dnorm"},
		{NULL, 0, 0, 0, 0, 0, NULL}
	};

	return ((t_tab*)ttab);
}

int			ft_clean(void *s, size_t n)
{
	ft_bzero(s, n);
	return (0);
}

void		ft_initialize_get_line(t_tab **ttab, char *str, t_dlist **history)
{
	*ttab = tab_termcaps();
	initialize_caps(&g_caps, str);
	initialize_signals();
	g_caps.history = history;
}

int			get_line_term_termcaps(char **res, char *str, t_dlist **history)
{
	t_tab		*ttab;
	t_tab		*tmp_tab;

	ft_initialize_get_line(&ttab, str, history);
	while ((tmp_tab = (ttab - 1)) && !ft_clean(g_caps.buf, 2048)
		&& (read(0, g_caps.buf, 2047) >= 0))
	{
		if (EN_K1 && EN_K2 && !end_key(&g_caps)
			&& ((g_caps.sz_str - g_caps.size_prompt) == 0)
				&& (*res = NULL) && (g_caps.str[0] ?
					!ft_free(g_caps.str[0]) : 1) && (g_keeprun == 3 ?
						0 : g_keeprun) && !ft_free_char_char(g_caps.str))
			return (2);
		else if (EN_K1 && EN_K2 && !end_key(&g_caps))
			break ;
		if (CD_K1 && CD_K2 && ctrl_d_management(&g_caps))
			break ;
		while ((++tmp_tab)->cmd)
			if (EQ1 && EQ2 && EQ3 && !(tmp_tab->ptr(&g_caps)))
				break ;
		if (!tmp_tab->cmd)
			print_buf(&g_caps, g_caps.buf);
	}
	return (0);
}

int			get_line_term(char **res, char *str, t_dlist **history)
{
	t_term		term;
	int			ret;

	terminal_data(&term);
	modify_terminos(&term);
	g_keeprun = 3;
	if ((ret = get_line_term_termcaps(res, str, history)))
		return (ret);
	*res = g_caps.str[0];
	if (g_caps.tmp_str)
		free(g_caps.tmp_str);
	g_caps.tmp_str = NULL;
	if (g_caps.prompt)
		free(g_caps.prompt);
	g_caps.prompt = NULL;
	if (g_caps.copy_str)
		free(g_caps.copy_str);
	g_caps.copy_str = NULL;
	free(g_caps.str);
	g_caps.str = NULL;
	if (g_keeprun == 3)
		g_keeprun = 0;
	reset_termios(&term);
	return (0);
}
