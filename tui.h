#ifndef TUI_H
#define TUI_H

#include <stddef.h>

extern void print_char (char ch, size_t times);

extern void print_empty_line (void);
extern void print_split_line (void);
extern void print_full_line (void);

extern void print_center_text (const char *text);
extern void print_block_text (const char *text);
extern void print_left_text (const char *text);

extern char get_ascii (void);
extern char get_digit (void);
extern char get_alnum (void);

enum
{
  GET_ESC,
  GET_EMPTY,
  GET_SUCCESS,
};

extern int get_id (char *buff, size_t max);
extern int get_str (char *buff, size_t max);

#endif
