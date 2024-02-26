#include "util.h"
#include "config.h"
#include <jansson.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined(__linux__)
#include <termios.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

static inline void
init_terminal (void)
{
  setlocale (LC_ALL, "en_US.utf8");
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
eprintf (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  va_end (ap);
  exit (1);
}

FILE *file_menu;
FILE *file_student;
FILE *file_merchant;
FILE *file_evaluation;

static inline FILE *
init_file (const char *path)
{
  FILE *file = fopen (path, "r+");
  if (file)
    /* 打开成功 */
    return file;

  file = fopen (path, "w+");
  if (!file)
    error ("数据表 %s 创建失败", path);

  if (fputs ("{}", file) == EOF)
    error ("数据表 %s 初始化失败", path);

  return file;
}

json_t *table_menu;
json_t *table_student;
json_t *table_merchant;
json_t *table_evaluation;

static inline json_t *
init_table (FILE *file)
{
  if (fseek (file, 0, SEEK_SET) != 0)
    error ("文件流重定位失败");

  json_t *json;
  json_error_t jerr;

  json = json_loadf (file, 0, &jerr);
  if (!json)
    error ("json 解析失败");

  if (!json_is_object (json))
    error ("json 格式错误");

  return json;
}

void
init (void)
{
  init_terminal ();

  file_menu = init_file (PATH_TABLE_MENU);
  file_student = init_file (PATH_TABLE_STUDENT);
  file_merchant = init_file (PATH_TABLE_MERCHANT);
  file_evaluation = init_file (PATH_TABLE_EVALUATION);

  table_menu = init_table (file_menu);
  table_student = init_table (file_student);
  table_merchant = init_table (file_merchant);
  table_evaluation = init_table (file_evaluation);
}

void
deinit (void)
{
  fclose (file_menu);
  fclose (file_student);
  fclose (file_merchant);
  fclose (file_evaluation);

  json_decref (table_menu);
  json_decref (table_student);
  json_decref (table_merchant);
  json_decref (table_evaluation);
}

void
save (json_t *from, FILE *to)
{
  char *str = json_dumps (from, 0);
  if (!str)
    error ("json 编码失败");

  if (fseek (to, 0, SEEK_SET) != 0)
    error ("文件流重定位失败");

  size_t len = strlen (str);
  if (fwrite (str, len, 1, to) != 1)
    error ("数据写入失败");

  free (str);
}
