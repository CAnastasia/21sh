/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/05 14:54:11 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - Aim of the function :
*** - Collect terminal data
*/

int		terminal_data(t_term *term)
{
	char	*name_term;

	if ((name_term = getenv("TERM")) == NULL)
		name_term = "xterm-256color";
	if (tgetent(NULL, name_term) <= 0)
		return (-1);
	if (tcgetattr(0, term) == -1)
		return (-1);
	return (0);
}

/*
*** - Aim of the function :
*** - Modify the output options of the current terminal
*/

int		modify_terminos(t_term *term)
{
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_lflag &= ~(ISIG);
	term->c_cc[VMIN] = 0;
	term->c_cc[VTIME] = 1;
	if (tcsetattr(0, TCSADRAIN, term) == -1)
		return (-1);
	return (0);
}

/*
*** - Aim of the function :
*** - Reset termios to the original state
*/

int		reset_termios(t_term *term)
{
	term->c_lflag |= (ECHO | ICANON | ISIG);
	if (tcsetattr(0, TCSADRAIN, term) == -1)
		return (-1);
	return (0);
}
