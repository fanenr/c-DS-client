#include "util.h"
#include "config.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

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
    /* 创建成功 */
    return file;

  /* 创建失败 */
  printf ("数据表无法打开或创建失败\n");
  abort ();
}

void
check_and_init (void)
{
  setlocale (LC_ALL, "en_US.utf8");

  file_menu = check_and_init_file (PATH_TABLE_MENU);
  file_student = check_and_init_file (PATH_TABLE_STUDENT);
  file_merchant = check_and_init_file (PATH_TABLE_MERCHANT);
  file_evaluation = check_and_init_file (PATH_TABLE_EVALUATION);
}
