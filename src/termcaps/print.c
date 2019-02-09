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

void print_end_line(t_tcap *caps, char *string, char *tmp)
{
    tmp = caps->str[0];
    caps->str[0] = ft_strjoin(tmp, string);
    free(tmp);
    free(string);
    caps->sz_str++;
    caps->cursor++;
    cursor_position(caps->curs_pos);
    write(1, caps->buf, 3);
    if (caps->curs_pos[0] == caps->window_size[1])
    {
        position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str, 0);
        if (caps->char_pos[0] + 1 == (caps->window_size[1]) && caps->char_pos[1] == caps->window_size[0])
        {
            tputs(tgetstr("sf", NULL), 1, ft_outc);
            //diminues the value of the y_prompt, to keep correct track of the y.position of the origin
            caps->y_prompt--;
        }
        tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1]), 1, ft_outc);
    }
    else if (caps->curs_pos[1] == caps->window_size[0] && caps->buf[0] == '\n')
        caps->y_prompt--;
}

int check_if_scroll(t_tcap *caps, char *str)
{
    int curs[2]; // [0] = x
    int i = -1;
    int x;
    int y;
    int count_up;
    int keeptrack;

    cursor_position(curs);
    size_windows(caps); // row in [0], col in [1]
    x = curs[0];
    y = curs[1];
    count_up = 0;
    while (str && str[++i])
    {
        if (x == caps->window_size[1] || str[i] == '\n')
        {
            y++;
            x = 1;
        }
        else
            x++;
        if (y > caps->window_size[0] && keeptrack != str[i] && (keeptrack = str[i]))
            count_up++;
    }
    // je regarde si, dans ma str, à partir de maintenant, au premier \n, dès qu'il touche la window_size, si en y, il touche aussi la taille de la fenêtre. Si c'est le cas, alors j'up d'un cran
    dprintf(3, "count_up: %d\n", count_up);
    // dprintf(3, "check if scroll: |x: %d|y: %d|row: %d|col: %d|\n", curs[0], curs[1], caps->window_size[0], caps->window_size[1]);
    // en horizontal, à gauche, x commence à 2, et termine à caps->window_size + 1, dès que j'écris en window_size, je vais à index 2 de la ligne du dessous
    // en vertical, curs[y] (1) = window_size[y] (0)
    return (count_up > 0 ? 1 : 0);
}

// ------- FONCTIONNE PAS MAIS BEAU -------

static void     print_middle_line(t_tcap *caps, char *string, char *tmp, char *tmp2)
{
    int tst[2];
    int boul = 0;
    // saves cursor position
    tputs(tgetstr("sc", NULL), 1, ft_outc);
    // manages substrings
    tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt));
    tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
    free(caps->str[0]);
    caps->str[0] = ft_strjoin(tmp, string);
    free(tmp);
    free(string);
    tmp = caps->str[0];
    caps->str[0] = ft_strjoin(tmp, tmp2);
    free(tmp);
    // efface le reste
    tputs(tgetstr("cd", NULL), 1, ft_outc);
    // usleep(50000);
    //saves cursor position again, but not in the system tmp
    cursor_position(caps->curs_pos);
    if (check_if_scroll(caps, tmp2))
    {
            //   saves position
        cursor_position(tst);
        // if cursor.y is not at the end of the window.y, go to the line at the bottom
        if (tst[1] != caps->window_size[0])
            tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, caps->window_size[0]), 1, ft_outc);
        // sleep(2);
        // scroll up one time
        tputs(tgetstr("sf", NULL), 1, ft_outc);
        // sleep(4);
        //diminues the value of the y_prompt, to keep correct track of the y.position of the origin
        caps->y_prompt--;
        // replace the cursor at the previous position
        tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);
        boul = 1;
        // sleep(4);
    
    }
    // write the new char
    write(1, caps->buf, 3);// write(1, caps->buf, 4); ??
    if (caps->buf[0] == '\n' && check_if_scroll(caps, tmp2)) //  && boul == 1 // caps->copy_str
    {
        dprintf(3, "je suis passe dans cette merde\n");
        caps->y_prompt--;
    }
    // write the new char

    // prints the rest (the tmp)
    write(1, tmp2, caps->sz_str - caps->cursor);
    // Incrémente le compteur
    caps->sz_str++;
    //replace le curseur
    if (boul == 0)
        tputs(tgetstr("rc", NULL), 1, ft_outc);
    else if (boul == 1)
        tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);

    //manages char end of line, at x == window_size
    // print_middle_line_manage_char_end_line(caps);

    // Déplacer le curseur à droite, et incrémente en même temps
    right_key(caps);
    free(tmp2);
    // tputs(tgetstr("ve", NULL), 1, ft_outc);
}

// le probleme est que, quand on envoie un \n dans la str, il faut parfois scroll-up
// 

static void print_normal_char_initialization_first_passage(t_tcap *caps, char *string)
{
    // if (caps->str[0])
    // free(caps->str[0]);
    caps->str[0] = ft_strdup(string);
    free(string);
    ++(caps->i);
    caps->sz_str++;
    caps->cursor++;
    write(1, caps->buf, 3);
}

/*
*** - NEED TO NORM THE FUNCTION, BUT WORKS PROPERLY
*/

int print_normal_char(t_tcap *caps)
{
    char *tmp;
    char *tmp2;
    char *string;

    tmp = NULL;
    tmp2 = NULL;
    string = NULL; //fonctionne sans
    size_windows(caps);
    position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str, 0);

     string = ft_strndup(caps->buf, 1);
     tmp2 = ft_strjoin(string, caps->str[0] + (caps->cursor - caps->size_prompt));
    // régler ici le problème de l'affichage de char quand la str est pleine et que ça déborde
    // je pense que le pb vient du y-1 et y => les 2 seront justes quand il y a un \n dans la str
    if (caps->str && caps->str[0])
        dprintf(3, "print_normal_char: %d| %d\n", caps->y_prompt, check_if_scroll(caps, caps->str[0] + (caps->cursor - caps->size_prompt)));
    if (((caps->buf[0] >= 32 && caps->buf[0] <= 127) || caps->buf[0] == 10) 
        && caps->buf[1] == 0 &&
        // && !(((caps->y_prompt - 1) == 0 || (caps->y_prompt) == 0) && (caps->char_pos[0] + 1) == caps->window_size[1] && caps->char_pos[1] == caps->window_size[0]) && caps->buf[0] != 9)
        !(caps->y_prompt == 1 && check_if_scroll(caps, tmp2))) // caps->str[0] + (caps->cursor - caps->size_prompt) ====  && caps->char_pos[1] == caps->window_size[0]
    {
        // string = ft_strndup(caps->buf, 1);
        free(tmp2);
        tmp2 = NULL;
        if (caps->i == 0) // Initialization of the str, the first time
            print_normal_char_initialization_first_passage(caps, string);
        else if (caps->cursor < caps->sz_str) // Manages when prints char in the middle of string
            print_middle_line(caps, string, tmp, tmp2);
        else
            print_end_line(caps, string, tmp);
        // if (caps->i != 0)
        // free(string);
    }
    else if (caps->buf[0] != 9) // escape the tab char
        tputs(tgetstr("bl", NULL), 1, ft_outc);
    return (0);
}