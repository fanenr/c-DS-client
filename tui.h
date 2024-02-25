#ifndef TUI_H
#define TUI_H

#include <stddef.h>

extern size_t width_of (const char *text);

extern void print_char (char ch, size_t times);
extern void print_empty_line (void);
extern void print_full_line (void);
extern void print_center_text (const char *text);
extern void print_left_text (const char *text);

#endif
