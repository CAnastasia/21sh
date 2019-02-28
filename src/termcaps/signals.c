/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:54:56 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - Aim of the function : int function to use putchar_fd inside if
*/

static int		ft_putc_if(char c)
{
	ft_putchar_fd(c, 1);
	return (0);
}

static void		ctrl_c_norm(void)
{
	if (ft_strcmp(g_caps.prompt, "bash > ") && (g_caps.sz_str = 10))
	{
		free(g_caps.str[0]);
		g_caps.keeprun = 2;
		if (!ft_strcmp(g_caps.prompt, "dquote > "))
			g_caps.str[0] = ft_strdup("\"");
		else if (!ft_strcmp(g_caps.prompt, "squote > "))
			g_caps.str[0] = ft_strdup("\'");
		else if (!ft_strcmp(g_caps.prompt, "Missing arguments > "))
			g_caps.str[0] = ft_strdup("oui");
		else if (!ft_strcmp(g_caps.prompt, "\nHeredoc > "))
			g_caps.str[0] = ft_strdup("pppppp");
		else if (!ft_strcmp(g_caps.prompt, "Heredoc > "))
			g_caps.str[0] = ft_strdup("pppppp");
	}
}

/*
*** - Aim of the function : manage the ctrl_c signal
*** - Handles the resizing of the terminal window.
*** - do not forget to free the copy of the prompt when exit termcaps
*/

void			ctrl_c(int sig)
{
	char c;

	(void)sig;
	c = 10;
	if (g_caps.keeprun != 3 && !ft_putc_if('\n'))
		return ;
	g_caps.keeprun = 1;
	end_key(&g_caps);
	if (g_caps.str[0])
		free(g_caps.str[0]);
	g_caps.str[0] = ft_memalloc(1);
	g_caps.sz_str = ft_strlen(g_caps.prompt);
	while (g_caps.history[0]->prev)
		g_caps.history[0] = g_caps.history[0]->prev;
	ctrl_c_norm();
	ioctl(0, TIOCSTI, &c);
}

/*
*** Handles the resizing of the terminal window and the ctrl_c signal
*/

int				initialize_signals(void)
{
	signal(SIGWINCH, win_resize);
	signal(SIGINT, ctrl_c);
	return (1);
}
