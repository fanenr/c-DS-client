#ifndef TUI_H
#define TUI_H

#include <stddef.h>

extern void gotoxy (int x, int y);
extern void tui_init (void);
extern void clear (void);

extern size_t width_of (const char *text);

extern void print_empty_line (void);
extern void print_split_line (void);
extern void print_full_line (void);

extern void print_char (char ch, size_t times);

extern void print_center_text (const char *text);
extern void print_block_text (const char *text);
extern void print_left_text (const char *text);

extern char get_ascii (void);
extern char get_digit (void);
extern char get_alnum (void);

enum
{
  GET_INT,
  GET_NUM,
  GET_ALNUM,
  GET_STRING,

  GOT_ESC,
  GOT_EMPTY,
  GOT_SUCCESS,
};

extern int get_more (int typ, char *buff, size_t max);

#endif
