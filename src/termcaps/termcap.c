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

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

/*
*** - Aim of the function :
*** - Initialize the array of pointers function
*/
t_tab		*tab_termcaps(void)
{
	static t_tab ttab[17] = {
		{&left_key, 27, 91, 68, 0, 0, "le"},
		{&right_key, 27, 91, 67, 0, 0, "nd"},
		{&del_key, 127, 0, 0, 0, 0, "del"},
		{&home_key, 27, 91, 72, 0, 0, "home"}, // verify if keyboard at school has the same numbers for home
		{&end_key, 27, 91, 70, 0, 0, "end"},// verify if keyboard at school has the same numbers for end
		{&alt_up_key, 27, 27, 91, 65, 0, "alt_up"},
		{&alt_down_key, 27, 27, 91, 66, 0, "alt_down"},
		{&up_key, 27, 91, 65, 0, 0, "up"},
		{&down_key, 27, 91, 66, 0, 0, "down"},
		{&alt_right_key, 27, 27, 91, 67, 0, "alt_right"},
		{&alt_left_key, 27, 27, 91, 68, 0, "alt_left_key"},
		{&ctrl_l, 12, 0, 0, 0, 0, "ctrl_l"},
		// {&ctrl_d, 4, 0, 0, 0, 0, "ctrl_d"},
		{&alt_x, -30, -119, -120, 0, 0, "alt_x"},
		{&alt_s, -61, -110, 0, 0, 0, "alt_s"},
		{&alt_w, -30, -128, -71, 0, 0, "alt_w"},
		{&alt_p, -49, -128, 0, 0, 0, "alt_p"},
		{NULL, 0, 0, 0, 0, 0, NULL}
	};

	return ((t_tab*)ttab);
}

int		ft_clean(void *s, size_t n)
{
	ft_bzero(s, n);
	return (0);
}

void		ft_initialize_get_line(t_tab **ttab, char *str, t_term *term, t_dlist **history)
{
	// terminal_data(term);
	// modify_terminos(term);
// Initialisation du tableau de pointeurs sur fonction
	*ttab = tab_termcaps();
// Initialisation de la struct caps
	initialize_caps(&caps, str);
	initialize_signals();
//inclure un printf de prompt pour voir
	caps.history = history;
}

int 		get_line_term(char **res, char *str, t_dlist **history)
{
	t_tab		*ttab;
	t_tab		*tmp_tab;
	t_term		term;

	terminal_data(&term);
	modify_terminos(&term);
	ft_initialize_get_line(&ttab, str, &term, history);
	////
// Itérer sur infini
	while ((tmp_tab = (ttab - 1)) && !ft_clean(caps.buf, 2048) && (read(0, caps.buf, 2047) >= 0))
	{
		// dprintf(2, "LA: %d %d %d %d %d\n", caps.buf[0], caps.buf[1], caps.buf[2], caps.buf[3], caps.buf[4]);
		if (ENTER_KEY && !end_key(&caps)) //|| CTRL_D_KEY
		{
			// to avoid segfault when empty
			if (((caps.sz_str - caps.size_prompt) == 0) && (*res = ft_memalloc(2)))
			{
				if (caps.str[0])
					free(caps.str[0]);
				free(caps.str);
			// returns 2 when normal \n but empty str
				return (2);
			}
			// if (ENTER_KEY)
				break ;
		}
		if (CTRL_D_KEY)
		{
					// dprintf(2, "passe dans le grand [%d - %d]\n", caps.sz_str, caps.size_prompt);	
				if (((caps.sz_str - caps.size_prompt) == 0))
			{
				// dprintf(2, "passe\n");
				free(caps.str[0]);
				caps.str[0] = ft_strdup("exit");
				break ;
			}
			else
				tputs(tgetstr("bl", NULL), 1, ft_outc);
		}
		// dprintf(2, "buf: |%s|\n", caps.buf);
		while ((++tmp_tab)->cmd)
			if (BUF_EQUALS_ARRAY && !(tmp_tab->ptr(&caps)))
				break;
		if (!tmp_tab->cmd)
			print_buf(&caps, caps.buf);
	}
	*res = caps.str[0];
	// dprintf(2, "tmp_str: %s\n", caps.tmp_str);
	// FIRST FREES
	if (caps.tmp_str && caps.tmp_str[0])
		free(caps.tmp_str);
	if (caps.copy_str)
		free(caps.copy_str);
	if (caps.tmp_str)
		free(caps.tmp_str);
	free(caps.str);
	// if (caps.prompt)
	// 	free(caps.prompt);
	///
	reset_termios(&term);
return (0);
}