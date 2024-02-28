#include "tui.h"
#include "config.h"
#include "util.h"
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#if defined(__linux__)
#include <termios.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#define KEY_ESC 27
#define KEY_ENTER 10
#define KEY_BACKSPACE 127

void
tui_init (void)
{
  setlocale (LC_ALL, "en_US.utf8");

  fwide (stdout, -1);
  fwide (stderr, -1);
  fwide (stdin, 1);

#if defined(__linux__)
  struct termios termios;
  tcgetattr (STDIN_FILENO, &termios);
  termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr (STDIN_FILENO, TCSANOW, &termios);
#elif defined(_WIN32)
  DWORD mode;
  HANDLE handle = GetStdHandle (STD_INPUT_HANDLE);
  GetConsoleMode (handle, &mode);
  mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
  SetConsoleMode (handle, mode);
#endif
}

void
clear (void)
{
  int sys = 0;
#if defined(__linux__)
  sys = system ("clear");
#elif defined(_WIN32)
  sys = system ("cls");
#endif
  if (sys != 0)
    error ("屏幕清空失败");
}

void
gotoxy (int x, int y)
{
#if defined(__linux__)
  printf ("\033[%d;%dH", y, x);
#elif defined(_WIN32)
  COORD coord = { x, y };
  HANDLE handle = GetStdHandle (STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition (handle, coord);
#endif
}

size_t
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
          error ("字符串 %s 编码错误", text);

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
get_more (int typ, char *buff, size_t max)
{
  size_t len = 0;
  size_t num = 0;
  bool is_real = false;
  wchar_t temp[max + 1];
  char mbstr[MB_CUR_MAX + 1];

  for (;;)
    {
      wint_t wch = getwchar ();
      if (wch == WEOF)
        error ("无法从控制台读取数据");

      switch (wch)
        {
        case KEY_ESC:
          return GOT_ESC;

        case KEY_ENTER:
          if (!num)
            return GOT_EMPTY;
          if (wcstombs (buff, temp, max + 1) == (size_t)-1)
            error ("字符串 %ls 重编码失败", temp);
          return GOT_SUCCESS;

        case KEY_BACKSPACE:
          if (!num)
            continue;
          size_t bytes = wctomb (mbstr, temp[--num]);
          size_t width = (bytes == 1) ? 1 : TUI_NASCII_WIDTH;
          print_str ("\b \b", width);
          temp[num] = 0;
          len -= bytes;
          continue;
        }

      switch (typ)
        {
        case GET_INT:
          if (!isdigit (wch))
            continue;
          break;

        case GET_NUM:
          if (!isdigit (wch) && wch != '.')
            continue;
          if (wch == '.')
            if (!num || is_real)
              continue;
          break;

        case GET_ALNUM:
          if (!isalnum (wch))
            continue;
          break;

        case GET_STRING:
          if (isascii (wch) && !isprint (wch))
            continue;
          break;
        }

      size_t bytes = wctomb (mbstr, wch);
      if (len >= max || len + bytes > max)
        continue;

      if (typ == GET_NUM && wch == '.')
        is_real = true;

      printf ("%lc", wch);
      temp[num++] = wch;
      temp[num] = 0;
      len += bytes;
    }
}
