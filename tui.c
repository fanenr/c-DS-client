#include "tui.h"
#include "config.h"
#include <bits/types/mbstate_t.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#define KEY_BACKSPACE 127
#define KEY_ENTER 10
#define KEY_ESC 27

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

      char unicode[5];
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

char
get_ascii (void)
{
  for (;;)
    {
      wint_t ch = getwchar ();
      if (isascii (ch))
        return ch;
    }
}

char
get_digit (void)
{
  for (;;)
    {
      char ch = get_ascii ();
      if (isdigit (ch))
        return ch;
    }
}

char
get_alnum (void)
{
  for (;;)
    {
      char ch = get_ascii ();
      if (isalnum (ch))
        return ch;
    }
}

int
get_id (char *buff, size_t max)
{
  size_t len = 0;
  for (;;)
    {
      int ch = get_ascii ();

      if (ch == KEY_ESC)
        return GET_ESC;

      if (ch == KEY_ENTER)
        {
          if (!len)
            return GET_EMPTY;
          return GET_SUCCESS;
        }

      if (ch == KEY_BACKSPACE)
        {
          if (len)
            {
              buff[--len] = '\0';
              printf ("\b \b");
            }
          continue;
        }

      if (!isdigit (ch) || len >= max)
        continue;

      buff[len++] = ch;
      buff[len] = '\0';
      putchar (ch);
    }
}

int
get_str (char *buff, size_t max)
{
  size_t len = 0;
  size_t num = 0;
  wchar_t temp[max + 1];
  char mbstr[MB_CUR_MAX + 1];

  for (;;)
    {
      wint_t wch = getwchar ();
      if (wch == WEOF)
        {
          printf ("非法 UTF-8 字符\n");
          abort ();
        }

      if (isascii (wch))
        {
          if (wch == KEY_ESC)
            return GET_ESC;

          if (wch == KEY_ENTER)
            {
              if (!num)
                return GET_EMPTY;
              if (wcstombs (buff, temp, max) != (size_t)-1)
                return GET_SUCCESS;
              printf ("字符串重编码失败\n");
              abort ();
            }

          if (wch == KEY_BACKSPACE)
            {
              if (num)
                {
                  size_t bytes = wctomb (mbstr, temp[--num]);
                  temp[num] = 0;
                  len -= bytes;
                  int width = (bytes == 1) ? 1 : TUI_NASCII_WIDTH;
                  for (int i = 0; i < width; i++)
                    printf ("\b \b");
                }
              continue;
            }
        }

      size_t bytes = wctomb (mbstr, wch);
      if (len >= max || len + bytes > max)
        continue;

      printf ("%lc", wch);
      temp[num++] = wch;
      temp[num] = 0;
      len += bytes;
    }
}
