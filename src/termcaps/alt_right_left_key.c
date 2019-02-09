/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:29:51 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int			alt_right_key(t_tcap *caps)
{
	int i;

	i = caps->cursor - caps->size_prompt;
	if ((i > 0 && !ft_isclear(caps->str[0][i - 1])) ||
		(i + caps->size_prompt < caps->sz_str && !ft_isclear(caps->str[0][i])))
	{
		while (i + caps->size_prompt < caps->sz_str
			&& !ft_isclear(caps->str[0][i]) && !right_key(caps))
			i++;
		while (i + caps->size_prompt < caps->sz_str
			&& ft_isclear(caps->str[0][i]) && !right_key(caps))
			i++;
	}
	else
		while ((i + caps->size_prompt < caps->sz_str)
				&& ft_isclear(caps->str[0][i]) && !right_key(caps))
			i++;
	return (0);
}

int			alt_left_key(t_tcap *caps)
{
	int i;

	i = caps->cursor - caps->size_prompt;
	if (i > 0 && !ft_isclear(caps->str[0][i - 1]))
		while (i > 0 && !ft_isclear(caps->str[0][i - 1])
				&& !left_key(caps))
			i--;
	else
	{
		while (i > 0 && ft_isclear(caps->str[0][i - 1])
			&& !left_key(caps))
			i--;
		while (i > 0 && !ft_isclear(caps->str[0][i - 1])
			&& !left_key(caps))
			i--;
	}
	return (0);
}
