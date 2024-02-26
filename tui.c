#include "tui.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static inline size_t
width_of (const char *text)
{
  mbstate_t state;
  memset (&state, 0, sizeof (mbstate_t));

  size_t width = 0;
  const char *end = text + strlen (text);

  for (const char *ptr = text; ptr < end;)
    {
      size_t len = mbrlen (ptr, end - ptr, &state);
      switch (len)
        {
        case (size_t)-1:
        case (size_t)-2:
          printf ("非法 UTF-8 字符串\n");
          abort ();
        case 0:
          return width;
        case 1:
          width++;
          ptr += 1;
          break;
        default:
          width += 2;
          ptr += len;
          break;
        }
    }

  return width;
}

void
print_char (char ch, size_t times)
{
  for (size_t i = 0; i < times; i++)
    putchar (ch);
}

void
print_empty_line (void)
{
  putchar (TUI_FRAME_CHAR);
  print_char (' ', TUI_FRAME_WIDTH - 2);
  putchar (TUI_FRAME_CHAR);
  putchar ('\n');
}

void
print_split_line (void)
{
  printf ("%c ", TUI_FRAME_CHAR);
  print_char (TUI_FRAME_CHAR, TUI_FRAME_WIDTH - 4);
  printf (" %c", TUI_FRAME_CHAR);
  putchar ('\n');
}

void
print_full_line (void)
{
  print_char (TUI_FRAME_CHAR, TUI_FRAME_WIDTH);
  putchar ('\n');
}

void
print_center_text (const char *text)
{
  size_t width = width_of (text);
  putchar (TUI_FRAME_CHAR);
  size_t left = (TUI_FRAME_WIDTH - width - 2) / 2;
  print_char (' ', left);

  printf ("%s", text);

  size_t right = TUI_FRAME_WIDTH - left - width - 2;
  print_char (' ', right);
  putchar (TUI_FRAME_CHAR);
  putchar ('\n');
}

void
print_block_text (const char *text)
{
  size_t width = 0;
  const char *end = text + strlen (text);

  mbstate_t state;
  memset (&state, 0, sizeof (mbstate_t));

  for (const char *ptr = text; ptr < end;)
    {
    newline:
      if (width < 3)
        {
          printf ("%c  ", TUI_FRAME_CHAR);
          width += 3;
        }

      if (width + 2 + 3 > TUI_FRAME_WIDTH)
        {
          print_char (' ', TUI_FRAME_WIDTH - width - 1);
          putchar (TUI_FRAME_CHAR);
          putchar ('\n');
          width = 0;
          goto newline;
        }

      char unicode[6];
      size_t len = mbrlen (ptr, end - ptr, &state);
      switch (len)
        {
        case (size_t)-1:
        case (size_t)-2:
          printf ("非法 UTF-8 字符串\n");
          abort ();
        case 0:
          goto end;
        case 1:
          if (*ptr == '\n')
            {
              print_char (' ', TUI_FRAME_WIDTH - width - 1);
              putchar (TUI_FRAME_CHAR);
              putchar ('\n');
              width = 0;
              ptr += 1;
              goto newline;
            }
          putchar (*ptr);
          width++;
          ptr += 1;
          break;
        default:
          strncpy (unicode, ptr, len);
          unicode[len] = '\0';
          printf ("%s", unicode);
          width += TUI_NASCII_WIDTH;
          ptr += len;
          break;
        }
    }

end:
  print_char (' ', TUI_FRAME_WIDTH - width - 1);
  putchar (TUI_FRAME_CHAR);
  putchar ('\n');
}

void
print_left_text (const char *text)
{
  size_t width = width_of (text);
  putchar (TUI_FRAME_CHAR);
  printf ("  %s", text);

  size_t right = TUI_FRAME_WIDTH - width - 4;
  print_char (' ', right);
  putchar (TUI_FRAME_CHAR);
  putchar ('\n');
}
