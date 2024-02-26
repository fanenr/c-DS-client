#include "tui.h"
#include "config.h"
#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define KEY_ESC 27
#define KEY_ENTER 10
#define KEY_BACKSPACE 127

static inline size_t
width_of (const char *text)
{
  size_t width = 0;
  const char *end = text + strlen (text);

  for (const char *ptr = text; ptr < end;)
    {
      size_t len = mblen (ptr, end - ptr);
      switch (len)
        {
        case (size_t)-1:
        case (size_t)-2:
          error ("字符串 %s 编码错误", text);

        case 0:
          /* NULL */
          return width;

        case 1:
          /* ASCII */
          width++;
          ptr += 1;
          break;

        default:
          /* UNICODE */
          width += TUI_NASCII_WIDTH;
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
print_str (const char *str, size_t times)
{
  for (size_t i = 0; i < times; i++)
    printf ("%s", str);
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
  if (width + (1 + TUI_TEXT_PADDING) * 2 > TUI_FRAME_WIDTH)
    error ("文本 %s 过长", text);

  if (strchr (text, '\n') != NULL)
    error ("文本 %s 含有换行符", text);

  putchar (TUI_FRAME_CHAR);
  print_char (' ', TUI_TEXT_PADDING);
  size_t left = TUI_FRAME_WIDTH / 2 - TUI_TEXT_PADDING - 1 - width / 2;
  print_char (' ', left);

  printf ("%s", text);

  size_t right = TUI_FRAME_WIDTH - (1 + TUI_TEXT_PADDING) * 2 - width - left;
  print_char (' ', right);
  print_char (' ', TUI_TEXT_PADDING);
  putchar (TUI_FRAME_CHAR);
  putchar ('\n');
}

void
print_block_text (const char *text)
{
  wchar_t wch;
  size_t width = 0;
  size_t width_wch = 1;
  const char *end = text + strlen (text);

  for (const char *ptr = text; ptr < end;)
    {
    newline:
      if (width < 1 + TUI_TEXT_PADDING)
        {
          putchar (TUI_FRAME_CHAR);
          print_char (' ', TUI_TEXT_PADDING);
          width += 1 + TUI_TEXT_PADDING;
        }

      size_t bytes = mbtowc (&wch, ptr, end - ptr);
      switch (bytes)
        {
        case (size_t)-2:
        case (size_t)-1:
          printf ("非法 UTF-8 字符串\n");
          abort ();

        case 0:
          goto end;

        case 1:
          if (wch == '\n')
            {
              print_char (' ', TUI_FRAME_WIDTH - width - 1);
              putchar (TUI_FRAME_CHAR);
              putchar ('\n');
              width = 0;
              ptr += 1;
              goto newline;
            }
          __attribute__ ((fallthrough));

        default:
          width_wch = (bytes == 1) ? 1 : TUI_NASCII_WIDTH;
          if (width + width_wch + TUI_TEXT_PADDING + 1 > TUI_FRAME_WIDTH)
            {
              print_char (' ', TUI_FRAME_WIDTH - width - 1);
              putchar (TUI_FRAME_CHAR);
              putchar ('\n');
              width = 0;
              goto newline;
            }
          printf ("%lc", wch);
          width += width_wch;
          ptr += bytes;
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
  if (width + (1 + TUI_TEXT_PADDING) * 2 > TUI_FRAME_WIDTH)
    error ("文本 %s 过长", text);

  if (strchr (text, '\n') != NULL)
    error ("文本 %s 含有换行符", text);

  putchar (TUI_FRAME_CHAR);
  print_char (' ', TUI_TEXT_PADDING);

  printf ("%s", text);

  size_t right = TUI_FRAME_WIDTH - width - (1 + TUI_TEXT_PADDING) * 2;
  print_char (' ', right);
  print_char (' ', TUI_TEXT_PADDING);
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
      char ch = get_ascii ();

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
        error ("无法从控制台读取数据");

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
              error ("字符串 %ls 重编码失败", temp);
            }

          if (wch == KEY_BACKSPACE)
            {
              if (num)
                {
                  size_t bytes = wctomb (mbstr, temp[--num]);
                  size_t width = (bytes == 1) ? 1 : TUI_NASCII_WIDTH;
                  print_str ("\b \b", width);
                  temp[num] = 0;
                  len -= bytes;
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
