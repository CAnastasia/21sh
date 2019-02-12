/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/12 22:31:58 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

static int	norm_alt_keys(t_tcap *caps)
{
	size_windows(caps);
	return (-1);
}

static void	alt_up_key_norm(t_tcap *caps, int curs_pos[2])
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	tputs(tgetstr("up", NULL), 1, ft_outc);
	caps->cursor = caps->cursor - (curs_pos[0] +
		(caps->x_lines[0] - curs_pos[0]) + 1);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
}

int			alt_up_key(t_tcap *caps)
{
	int		curs_pos[2];
	int		i;

	i = norm_alt_keys(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps, curs_pos);
	if (curs_pos[1] > caps->y_prompt && curs_pos[1] > 0)
	{
		if (curs_pos[0] <= caps->size_prompt &&
			curs_pos[1] == (caps->y_prompt == 0 ?
				caps->y_prompt + 1 : caps->y_prompt) + 1)
			home_key(caps);
		else if (curs_pos[0] > caps->x_lines[0])
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			while (++i < curs_pos[0])
				left_key(caps);
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else
			alt_up_key_norm(caps, curs_pos);
	}
	return (0);
}

int			alt_down_key(t_tcap *caps)
{
	int		curs_pos[2];
	int		i;

	i = norm_alt_keys(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps, curs_pos);
	pos_char_in_window_in_str(caps->cursor, caps, caps->sz_str, 1);
	if (curs_pos[1] == caps->char_pos[1] - 1
			&& curs_pos[0] > caps->x_lines[2] && caps->x_lines[2] != -1)
		end_key(caps);
	else if (curs_pos[0] > caps->x_lines[2] && caps->x_lines[2] != -1)
	{
		tputs(tgetstr("vi", NULL), 1, ft_outc);
		while (++i < (caps->x_lines[1] - curs_pos[0]) + caps->x_lines[2] + 2)
			right_key(caps);
		tputs(tgetstr("ve", NULL), 1, ft_outc);
	}
	else if (curs_pos[1] < caps->char_pos[1])
	{
		tputs(tgetstr("vi", NULL), 1, ft_outc);
		while (++i < (caps->x_lines[1] - curs_pos[0]) + curs_pos[0] + 1)
			right_key(caps);
		tputs(tgetstr("ve", NULL), 1, ft_outc);
	}
	return (0);
}
