#pragma once

#if __has_include(<ncurses.h>)
#include <ncurses.h>
#elif __has_include(<curses.h>)
#include <curses.h>
#else

struct WINDOW;

extern WINDOW* stdscr;

int raw(void);
int noecho(void);
int echo(void);
int erase(void);
int endwin(void);
int refresh(void);
int clrtoeol(void);
int def_prog_mode(void);
int reset_prog_mode(void);
int timeout(int delay);

WINDOW* initscr(void);

int keypad(WINDOW* win, int bf);
int wgetch(WINDOW* win);
int getch(void);
int wmove(WINDOW* win, int y, int x);
int move(int y, int x);
int wrefresh(WINDOW* win);

int getmaxx(const WINDOW* win);
int getmaxy(const WINDOW* win);

int mvaddnstr(int y, int x, const char* str, int n);
int mvprintw(int y, int x, const char* fmt, ...);
int getnstr(char* str, int n);

#define getmaxyx(win, y, x)                                                                                            \
	do {                                                                                                               \
		(y) = getmaxy(win);                                                                                            \
		(x) = getmaxx(win);                                                                                            \
	} while (false)

#ifndef TRUE
#define TRUE 1
#endif

#ifndef ERR
#define ERR (-1)
#endif

#ifndef KEY_UP
#define KEY_UP 259
#endif

#ifndef KEY_DOWN
#define KEY_DOWN 258
#endif

#ifndef KEY_LEFT
#define KEY_LEFT 260
#endif

#ifndef KEY_RIGHT
#define KEY_RIGHT 261
#endif

#ifndef KEY_BACKSPACE
#define KEY_BACKSPACE 263
#endif

#ifndef KEY_DC
#define KEY_DC 330
#endif

#ifndef KEY_ENTER
#define KEY_ENTER 343
#endif

#ifndef KEY_MAX
#define KEY_MAX 511
#endif

#endif
