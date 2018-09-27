#include "../../inc/sh.h"
// #include "../../inc/termcap.h"

/*
*** - Aim of the function :
*** - Putstr from index i to j
*/
void	ft_putstr_i_to_j(char *str, int i, int j, int fd)
{
	--i;
	while (++i < j)
		ft_putchar_fd(str[i], fd);
}

/*
*** - Aim of the function :
*** - Obligatory to use tputs, as ft_putchar is (void*) type
*** - And tputs requires an int function (for the pointer function)
*/
int			ft_outc(int c)
{
  ft_putchar(c);
  return (0);
}

/*
*** - Aim of the function :
*** - Collect terminal data
*/
int     terminal_data (t_term *term)
{
  char   *name_term;
 
  if ((name_term = getenv("TERM")) == NULL)
     return (-1);
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
int     modify_terminos(t_term *term)
{
  term->c_lflag &= ~(ICANON);
  term->c_lflag &= ~(ECHO);
	// term->c_lflag &= ~(ECHO | ICANON);
  term->c_cc[VMIN] = 1;
  term->c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSADRAIN, term) == -1)
    return (-1);
//   tputs(tgetstr("sc", NULL), 0, ft_outc);
  return (0);
}

int		reset_termios(t_term *term)
{
	term->c_lflag = (ICANON | ECHO);
	if (tcsetattr(0, TCSADRAIN, term) == -1)
    	return (-1);
	return (0);
}

void		initialize_caps(t_tcap *caps, char *prompt)
{
	int 	tmp[2];
	caps->size_prompt = ft_strlen(prompt);
	caps->sz_str = caps->size_prompt;
	caps->cursor =  caps->size_prompt;
	cursor_position(tmp);
	caps->y_prompt = tmp[1];
	// dprintf(2, "YEEEEAAAH - %d\n", caps->y_prompt);
	if (!(caps->str = (char**)malloc(sizeof(char*))))
		return ;
}

/*
*** - Aim of the function : Collect at a given moment the
***	- position of the cursor - I use it to manage the size of 
*** - the cursor at any given time + at the beginning to 
*** - manage the history (after)
*/

void		cursor_position(int curs_pos[2])
{
	char	ret[9];
	int		i;
	
	i = 0;
	ft_bzero(ret, 9);
	write(0, "\033[6n", 4);
	//ip
	// tputs(tgetstr("ip", NULL), 1, ft_outc);
	read(0, ret, 9);
	curs_pos[1] = ft_atoi(ret + 2);
	while (ret[i] && ret[i] != 59)
		i++;
	curs_pos[0] = ft_atoi(ret + i + 1);
}

/*
*** - Aim of the function : Collect the size of the
***	- window when asked
*/
void		size_windows(t_tcap *caps)
{
	struct winsize *w;

	if (!(w = (struct winsize *)malloc(sizeof(struct winsize))))
		return ;
	ioctl(STDOUT_FILENO,  TIOCGWINSZ, w);
	caps->window_size[0] = w->ws_row;
	caps->window_size[1] = w->ws_col;
	free(w);
}

int 		left_key(t_tcap *caps)
{
	int  curs_pos[2];
	if (caps->cursor > caps->size_prompt)
	{
		cursor_position(curs_pos);
		size_windows(caps);
		//
		// tputs(tgetstr("vi", NULL), 1, ft_outc);
		dprintf(2, "gauche fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, curs_pos[0], curs_pos[1], caps->window_size[1], caps->sz_str);

		if (curs_pos[0] == 1)
		{
			dprintf(2, "passe ici %d", curs_pos[1]);
			// tputs(tgetstr("sf", NULL), 1, ft_outc);
			tputs(tgoto(tgetstr("cm", NULL), caps->window_size[1] - 1, curs_pos[1] - 2), 1, ft_outc);
		}
		else
		{
			tputs(tgetstr("le", NULL), 1, ft_outc);
		}
		caps->cursor--;
		// tputs(tgetstr("ve", NULL), 1, ft_outc);
		//
		// cursor_position(caps->curs_pos);
		dprintf(2, "gauche fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, curs_pos[0], curs_pos[1], caps->window_size[1], caps->sz_str);
	}
	return (0);
}

int 		right_key(t_tcap *caps)
{
	int  curs_pos[2];
	if (caps->cursor < caps->sz_str)
	{
		cursor_position(curs_pos);
		size_windows(caps);
		//
		// tputs(tgetstr("vi", NULL), 1, ft_outc);
		dprintf(2, "droite fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, curs_pos[0], curs_pos[1], caps->window_size[1], caps->sz_str);
		if (curs_pos[0] == caps->window_size[1])
		{
			dprintf(2, "passe par la porte\n");
			tputs(tgoto(tgetstr("cm", NULL), 0, curs_pos[1]), 1, ft_outc);
		}
		else
		{
			tputs(tgetstr("nd", NULL), 1, ft_outc);
		}
		caps->cursor++;
		// tputs(tgetstr("ve", NULL), 1, ft_outc);
		//
		// cursor_position(caps->curs_pos);
		dprintf(2, "droite fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, caps->curs_pos[0], caps->curs_pos[1], caps->window_size[1], caps->sz_str);
	}
	return (0);
}

/*
*** - NEED TO NORM THE FUNCTION, BUT WORKS PROPERLY
*/

int			print_normal_char(t_tcap *caps)
{
	char		*tmp;
	char		*tmp2;
	char		*string;
	static int	i = 0;

	if (caps->buf[0] >= 0 && caps->buf[0] <= 127 && caps->buf[1] == 0)
	{
		string = ft_strndup(caps->buf, 1);
		size_windows(caps);

		if (i == 0) // Initialization of the str, the first time
		{
			caps->str[0] = string;
			++i;
			caps->sz_str++;
			caps->cursor++;
			ft_putstr_i_to_j(caps->buf, 0, 3, 1);
		}
		else if (caps->cursor < caps->sz_str) // Manages when prints char in the middle of string
		{
			// tputs(tgetstr("vi", NULL), 1, ft_outc);
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
			// write the new char
			write(1, caps->buf, 4);
			// prints the rest (the tmp)
			write(1, tmp2, caps->sz_str - caps->cursor);
			
			// Incrémente le compteur
			caps->sz_str++;
			
			//replace le curseur
			tputs(tgetstr("rc", NULL), 1, ft_outc);

			// gère la ligne en plus en fin, quand la string touche le bas de la fenêtre, au bon moment
			// et replace le curseur au bon endroit
			if (((caps->sz_str ) % (caps->window_size[1])) == 0 && ((caps->y_prompt + ((caps->sz_str ) / (caps->window_size[1])) - 1) == caps->window_size[0])) 
			/////////// AJOUTER CONDITION --> NE FAIRE que quand string est en bas de fenêtre
			{
				int tst[2];
				// saves position
				cursor_position(tst);
				// if cursor.y is not at the end of the window.y, go to the line at the bottom
				if (tst[1] != caps->window_size[0])
					tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, caps->window_size[0]), 1, ft_outc);
				// scroll up one time
				tputs(tgetstr("sf", NULL), 1, ft_outc);
				//diminues the value of the y_prompt, to keep correct track of the y.position of the origin
				caps->y_prompt--;
				// replace the cursor at the previous position
				tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);
			}

			// Déplacer le curseur à droite, et incrémente en même temps
			right_key(caps);
			free(tmp2);
			// tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else // prints and manages when end of string
		{
			tmp = caps->str[0];
			caps->str[0] = ft_strjoin(tmp, string);
			free(tmp);
			free(string);
			caps->sz_str++;
			caps->cursor++;
			cursor_position(caps->curs_pos);
			// tputs(tgetstr("vi", NULL), 1, ft_outc);
			write(1, caps->buf, 4);
			// ft_putstr_i_to_j(caps->buf, 0, 3, 1);
			// dprintf(2, "fin ligne: [%d - %d]\n", caps->curs_pos[0], caps->window_size[1]);
			if (caps->curs_pos[0] == caps->window_size[1])
			/////////// AJOUTER CONDITION --> NE FAIRE que quand string est en bas de fenêtre
			{
				dprintf(2, "DEBUGGY: | %d - %d - %d - %d |", caps->y_prompt, caps->sz_str, (caps->window_size[1]), caps->window_size[0]);
				if ((caps->y_prompt + ((caps->sz_str ) / (caps->window_size[1])) - 1) == caps->window_size[0])
				{
					dprintf(2, "oh non pas la\n");
					tputs(tgetstr("sf", NULL), 1, ft_outc);
					//diminues the value of the y_prompt, to keep correct track of the y.position of the origin
					caps->y_prompt--;
				}
				tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1]), 1, ft_outc);
			}
			// tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
	}
	return (0);
}

/*
*** - Aim of the function : Managing of the del key
*** - Plus realloc properly the string
*/

int			del_key(t_tcap *caps)
{
		char *tmp2;
		char *tmp;
	if (caps->cursor > caps->size_prompt)
	{
		// tputs(tgetstr("vi", NULL), 1, ft_outc);
		left_key(caps);
		// saves cursor position
		tputs(tgetstr("sc", NULL), 1, ft_outc);
		// manages substrings
		tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt + 1));
		tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
		free(caps->str[0]);
		caps->str[0] = ft_strjoin(tmp, tmp2);
		free(tmp);
		// efface le reste
		tputs(tgetstr("cd", NULL), 1, ft_outc);
		// prints the rest (the tmp)
		write(1, tmp2, caps->sz_str - caps->cursor);
		//replace le curseur
		tputs(tgetstr("rc", NULL), 1, ft_outc);
		free(tmp2);
		//keeps counter correct
		caps->sz_str--;
		// prints the cursor back
		// tputs(tgetstr("ve", NULL), 1, ft_outc);
	}
	return (0);
}

int			home_key(t_tcap *caps)
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	while (caps->cursor > caps->size_prompt)
		left_key(caps);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
	return (0);
}

int			end_key(t_tcap *caps)
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	while (caps->cursor < caps->sz_str)
		right_key(caps);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
	return (0);
}

int			alt_up_key(t_tcap *caps)
{
	int		curs_pos[2];
	
	size_windows(caps);
	cursor_position(curs_pos);
	if (curs_pos[1] > caps->y_prompt) // && curs_pos[1] < ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1)))
	{
		if (curs_pos[0] <= caps->size_prompt && curs_pos[1] == caps->y_prompt + 1)
			home_key(caps);
		else
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			tputs(tgetstr("up", NULL), 1, ft_outc);
			caps->cursor = caps->cursor - caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
	}
	return (0);
}


int			alt_down_key(t_tcap *caps)
{
	int		curs_pos[2];
	
	size_windows(caps);
	cursor_position(curs_pos);
	if (curs_pos[1] < ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1])))) // && curs_pos[1] < ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1)))
	{
		dprintf(2, "curs_pos[1]: %d, calc_complet_y: [%d], calc_alt_down: [%d]\n", curs_pos[1], ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1))), (caps->sz_str % (caps->window_size[1])));
		if (curs_pos[1] == ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1])) - 1) && curs_pos[0] > (caps->sz_str % (caps->window_size[1]))) //curs_pos[0] >= (caps->sz_str % (caps->window_size[1])) && 
		{
			dprintf(2, "ici mon char");
			end_key(caps);
		}
		else
		{
			dprintf(2, "ici ma poule");
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			tputs(tgoto(tgetstr("cm", NULL), curs_pos[0] - 1, curs_pos[1]), 1, ft_outc);
			caps->cursor = caps->cursor + caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		// dprintf(2, "ma biche");
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Initialize the array of pointers function
*/
t_tab		*tab_termcaps(void)
{
	static t_tab ttab[8] = {
		{&left_key, 27, 91, 68, 0, "le"},
		{&right_key, 27, 91, 67, 0, "nd"},
		{&del_key, 127, 0, 0, 0, "del"},
		{&home_key, 27, 91, 72, 0, "home"}, // verify if keyboard at school has the same numbers for home
		{&end_key, 27, 91, 70, 0, "end"},// verify if keyboard at school has the same numbers for end
		{&alt_up_key, 27, 27, 91, 65, "alt_up"},
		{&alt_down_key, 27, 27, 91, 66, "alt_down"},
		// {&alt_right_key, 27, 27, 91, 67, "alt_right"},
		// {&alt_left_key, 27, 27, 91, 68, "alt_left_key"},
		{NULL, 0, 0, 0, 0, NULL}
	};

	return ((t_tab*)ttab);
}

int			main(void)
{
  	t_term		term;
	t_tcap		caps;
	t_tab		*ttab;
	t_tab		*tmp_tab;

	//TEST
	// char		buf_backup[3];

//Initialisation du termios
	terminal_data(&term);
  	modify_terminos(&term);

// Initialisation du tableau de pointeurs sur fonction
	ttab = tab_termcaps();
// Initialisation de la struct caps
	initialize_caps(&caps, " bash >");
//inclure un printf de prompt pour voir
	dprintf(1, "bash > ");
// Itérer sur infini
	// ft_bzero(buf_backup, 3);
	while (42)
	{
		tmp_tab = (ttab - 1);
		ft_bzero(caps.buf, 4);
		// tputs(tgetstr("ip", NULL), 1, ft_outc);
		read(0, caps.buf, 4);
		while ((++tmp_tab)->cmd)
		{
			if (caps.buf[0] == tmp_tab->key0 &&
				caps.buf[1] == tmp_tab->key1
				&& caps.buf[2] == tmp_tab->key2 && caps.buf[3] == tmp_tab->key3)
			{
				tmp_tab->ptr(&caps);
				break;
			}
		}
		dprintf(2, "yo: caps.buf[0]: %d, 1: %d, 2: %d\n", caps.buf[0], caps.buf[1], caps.buf[2]);
		// if (!tmp_tab->cmd && (!(caps.buf[0] == 82 && caps.buf[1] == 0 && caps.buf[2] == 0 && ((buf_backup[0] == 53 && buf_backup[1] == 59 && buf_backup[2] == 56) || (buf_backup[0] == 53 && buf_backup[1] == 82 && buf_backup[2] == 0)))))
		if (!tmp_tab->cmd)
		{
			// dprintf(2, "why: caps.buf[0]: %d, 1: %d, 2: %d\n", caps.buf[0], caps.buf[1], caps.buf[2]);
			print_normal_char(&caps);
		}
		// buf_backup[0] = caps.buf[0];
		// buf_backup[1] = caps.buf[1];
		// buf_backup[2] = caps.buf[2];
		// dprintf(2, "[pos_curs: %d, sz_str: %d]\n", caps.cursor, caps.sz_str);
	}
	reset_termios(&term);



// tester prompt avec printf, puis clear screen pour voir ce qui apparaît
// free mallocs non free








// 	char *res; // dans fonction du poointeur sur fonction
//   char buf[3];
// 	// char test[1000];
// 	// char *travail;
// 	// travail = test;
 
//  // INITIALISATION DU TERMINAL ET DU TERMINOS
//   terminal_data(&term);
//   modify_terminos(&term);
//   while(1)
//   {
//   	  ft_bzero(buf, 3);
//   	  // ft_bzero(res, ft_strlen(res));
//       read(0, buf, 3);
//       // printf("%s", buf);
//       if (buf[0] != 127 && buf[0] != 64 && buf[0] != 27 && buf[0] != 38)//buf[0] != 27 && 
//       {
// 				ft_putstr_i_to_j(buf, 0, 3, 1);
// 				// *travail++ = buf[0];
// 				// dprintf(1, "debug : %s", test);
// 			}
//       // ft_putstr_i_to_j(buf, 0, 3, 1);
//       //left arrow
//       if (buf[0] == 27 && buf[1] == 91 && buf[2] == 68)
// 	  {
// 	  		if ((res = tgetstr("le", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 			// ft_bzero(res, ft_strlen(res));
// 	  }//saves cursor position - key : del -> code : sc
// 	  else if (buf[0] == 127 && buf[1] == 0 && buf[2] == 0)
// 	  {//sc
// 	  		if ((res = tgetstr("cd", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 			// ft_bzero(res, ft_strlen(res));
// 	  }//right arrow
// 	  else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 67)
// 	  {
// 	  		if ((res = tgetstr("nd", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 			// ft_bzero(res, ft_strlen(res));
// 	  }//return to the saved cursor position key : @
// 	  else if (buf[0] == 64 && buf[1] == 0 && buf[2] == 0)
// 	  {
// 	  		if ((res = tgetstr("rc", NULL)) == NULL)
// 				return (-1);
// 			// ft_putendl(res);
// 			tputs(res, 1, ft_outc);
// 	  }//launches insert mode, to add new things// key : &
// 	  else if (buf[0] == 38 && buf[1] == 0 && buf[2] == 0)
// 	  {
// 	  		if ((res = tgetstr("im", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 	  }
// 	  else if (buf[0] == 34 && buf[1] == 0 && buf[2] == 0)
// 	  {
// 	  		if ((res = tgetstr("ei", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 	  }


// ///////////INFO REFERENCE : http://web.mit.edu/~jik/sipbsrc/sun4m_53/micrognu/tty.c
//       //ft_putnstr(buf, 3);
//       // printf("%s\n", buf);
//       // printf(" %d %d %d \n", buf[0], buf[1], buf[2]);
//   }
//   //if (tcgetattr(0, &term) == -1)
//     //return (-1);
//   term.c_lflag = (ICANON | ECHO);
//   if (tcsetattr(0, TCSADRAIN, &term) == -1)
//     return (-1);
return (0);
}