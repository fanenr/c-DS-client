#include "util.h"
#include "config.h"
#include <jansson.h>
#include <locale.h>
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

FILE *file_menu;
FILE *file_student;
FILE *file_merchant;
FILE *file_evaluation;

static inline FILE *
check_and_init_file (const char *path)
{
  FILE *file = fopen (path, "r+");
  if (file)
    /* 打开成功 */
    return file;

  file = fopen (path, "w+");
  if (file)
    {
      /* 创建成功 */
      if (fputs ("{}", file) == EOF)
        goto error;
      if (fseek (file, 0, SEEK_SET) != 0)
        goto error;
      return file;
    }

error:
  printf ("数据表无法打开或创建失败\n");
  abort ();
}

json_t *table_menu;
json_t *table_student;
json_t *table_merchant;
json_t *table_evaluation;

static inline json_t *
ckeck_and_init_table (FILE *file)
{
  json_t *json;
  json_error_t jerr;

  json = json_loadf (file, 0, &jerr);
  if (json)
    {
      if (!json_is_object (json))
        goto error;
      if (fseek (file, 0, SEEK_SET) != 0)
        goto error;
      /* 加载成功 */
      return json;
    }

error:
  printf ("数据表加载失败或已损坏\n");
  abort ();
}

void
check_and_init (void)
{
  init_terminal ();

  file_menu = check_and_init_file (PATH_TABLE_MENU);
  file_student = check_and_init_file (PATH_TABLE_STUDENT);
  file_merchant = check_and_init_file (PATH_TABLE_MERCHANT);
  file_evaluation = check_and_init_file (PATH_TABLE_EVALUATION);

  table_menu = ckeck_and_init_table (file_menu);
  table_student = ckeck_and_init_table (file_student);
  table_merchant = ckeck_and_init_table (file_merchant);
  table_evaluation = ckeck_and_init_table (file_evaluation);
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
    {
      printf ("数据表化失败\n");
      abort ();
    }

  size_t len = strlen (str);
  if (fwrite (str, len, 1, to) != 1)
    {
      printf ("数据存储失败\n");
      abort ();
    }

  free (str);
}
