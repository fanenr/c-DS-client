#include "page.h"
#include "config.h"
#include "table.h"
#include "tui.h"
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void
page_main_menu (void)
{
  print_full_line ();

  print_empty_line ();
  print_center_text ("食堂菜品管理系统");
  print_empty_line ();

  print_split_line ();
  print_empty_line ();

  print_left_text ("1. 学生注册");
  print_left_text ("2. 学生登录");

  print_empty_line ();

  print_left_text ("3. 商户注册");
  print_left_text ("4. 商户登录");

  print_empty_line ();

  print_left_text ("5. 退出");

  print_empty_line ();
  print_empty_line (); /* 15 */
  print_empty_line ();
  print_full_line ();
}

void
page_main (void)
{
  int y = 15;
  int x = 2 + TUI_TEXT_PADDING;

  for (;;)
    {
      clear ();
      char input;
      page_main_menu ();
      gotoxy (x, y);
      printf ("输入数字选择操作，输入 q 退出: ");

    get:
      input = get_alnum ();
      switch (input)
        {
        case '1':
          page_student_reg ();
          break;
        case '2':
          page_student_log ();
          break;
        case '3':
          page_merchant_reg ();
          break;
        case '4':
          page_merchant_log ();
          break;
        case '5':
        case 'q':
          clear ();
          exit (0);
        default:
          goto get;
        }
    }
}

void
page_student_menu (void)
{
  print_full_line ();

  print_empty_line ();
  print_center_text ("学生主页");
  print_empty_line ();

  print_split_line ();
  print_empty_line ();

  print_left_text ("1. 修改信息");
  print_left_text ("2. 删除账户");

  print_empty_line ();

  print_left_text ("3. 浏览菜品");
  print_left_text ("4. 评价菜品");

  print_empty_line ();

  print_left_text ("5. 退出");

  print_empty_line ();
  print_empty_line (); /* 15 */
  print_empty_line ();
  print_full_line ();
}

void
page_student (void)
{
  int y = 15;
  int x = 2 + TUI_TEXT_PADDING;

  for (;;)
    {
      clear ();
      char input;
      page_student_menu ();
      gotoxy (x, y);
      printf ("输入数字选择操作，输入 q 退出: ");

    get:
      input = get_alnum ();
      switch (input)
        {
        case '1':
          break;
        case '2':
          break;
        case '3':
          break;
        case '4':
          break;
        case '5':
        case 'q':
          clear ();
          exit (0);
        default:
          goto get;
        }
    }
}

void
page_merchant_menu (void)
{
  print_char ('\n', 3);
  print_full_line ();

  print_empty_line ();
  print_center_text ("商户主页");
  print_empty_line ();

  print_split_line ();
  print_empty_line ();

  print_left_text ("1. 新增商户信息");
  print_left_text ("2. 查看商户信息");
  print_left_text ("3. 修改商户信息");
  print_left_text ("4. 删除商户信息");

  print_empty_line ();

  print_left_text ("4. 发布菜品");
  print_left_text ("5. 更新菜品");
  print_left_text ("6. 删除菜品");

  print_empty_line ();

  print_left_text ("7. 浏览评价");
  print_left_text ("8. 返回主页");

  print_empty_line ();
  print_full_line ();
}

void
page_dish_new (void)
{
}

void
page_merchant (void)
{
  for (;;)
    {
      clear ();
      char input;
      page_merchant_menu ();
      printf ("输入数字选择操作，输入 q 返回: ");

    input:
      input = get_alnum ();
      switch (input)
        {
        case '1':
          break;
        case '2':
          break;
        case '3':
          break;
        case '4':
          break;
        case '5':
          break;
        case '6':
          break;
        case '7':
          break;
        case '8':
        case 'q':
          return;
        default:
          goto input;
        }
    }
}

void
page_student_reg (void)
{
  clear ();
  print_full_line ();

  print_empty_line ();
  print_center_text ("学生注册");
  print_empty_line ();

  print_split_line ();

  const char *text1 = "学生学号: ";
  const char *text2 = "学生昵称: ";
  const char *text3 = "学生电话: ";
  const char *text4 = "登录密码: ";
  print_empty_line ();
  print_left_text (text1); /* 7 */
  print_empty_line ();
  print_left_text (text2); /* 9 */
  print_empty_line ();
  print_left_text (text3); /* 11 */
  print_empty_line ();
  print_left_text (text4); /* 13 */
  print_empty_line ();
  print_left_text ("(按 ENTER 确认信息，按 ESC 取消注册)");

  int y1 = 7, y2 = 9, y3 = 11, y4 = 13, y5 = 18;
  int x = 2 + TUI_TEXT_PADDING + width_of (text1);

  print_empty_line ();
  print_full_line ();

  char id[MAX_STUDENT_ID + 1];
  char name[MAX_STUDENT_NAME + 1];
  char number[MAX_STUDENT_NUMBER + 1];
  char password[MAX_STUDENT_PASSWORD + 1];

get_id:
  gotoxy (x, y1);
  for (;;)
    for (;;)
      switch (get_more (GET_INT, id, MAX_STUDENT_ID))
        {
        case GOT_ESC:
          return;
        case GOT_EMPTY:
          continue;
        case GOT_SUCCESS:
          goto get_name;
        }

get_name:
  if (find_by (table_student, NULL, "id", TYP_STR, id))
    {
      gotoxy (0, 18);
      printf ("学号为: %s 的学生已经存在", id);
      gotoxy (x, y1);
      print_char (' ', MAX_STUDENT_ID);
      goto get_id;
    }

  gotoxy (x, y2);
  for (;;)
    switch (get_more (GET_STRING, name, MAX_STUDENT_NAME))
      {
      case GOT_ESC:
        return;
      case GOT_EMPTY:
        continue;
      case GOT_SUCCESS:
        goto get_number;
      }

get_number:
  gotoxy (x, y3);
  for (;;)
    switch (get_more (GET_INT, number, MAX_STUDENT_NUMBER))
      {
      case GOT_ESC:
        return;
      case GOT_EMPTY:
        continue;
      case GOT_SUCCESS:
        goto get_password;
      }

get_password:
  gotoxy (x, y4);
  for (;;)
    switch (get_more (GET_ALNUM, password, MAX_STUDENT_NUMBER))
      {
      case GOT_ESC:
        return;
      case GOT_EMPTY:
        continue;
      case GOT_SUCCESS:
        goto build_info;
      }

build_info:
  info = object_new ();
  object_set (info, "id", TYP_STR, id);
  object_set (info, "name", TYP_STR, name);
  object_set (info, "number", TYP_STR, number);
  object_set (info, "password", TYP_STR, password);

  array_add (table_student, info);
  save (table_student, PATH_TABLE_STUDENT);
}

void
page_student_log (void)
{
  clear ();
  print_full_line ();

  print_empty_line ();
  print_center_text ("学生登录");
  print_empty_line ();

  print_split_line ();

  const char *text1 = "学生学号: ";
  const char *text2 = "登录密码: ";
  print_empty_line ();
  print_left_text (text1); /* 7 */
  print_empty_line ();
  print_left_text (text2); /* 9 */
  print_empty_line ();
  print_left_text ("(按 ENTER 确认信息，按 ESC 取消登录)");

  int y1 = 7, y2 = 9, y3 = 14;
  int x = 2 + TUI_TEXT_PADDING + width_of (text1);

  print_empty_line ();
  print_full_line ();

  json_t *temp = NULL;
  const char *passwd = NULL;

  char id[MAX_STUDENT_ID + 1];
  char name[MAX_STUDENT_NAME + 1];
  char number[MAX_STUDENT_NUMBER + 1];
  char password[MAX_STUDENT_PASSWORD + 1];

get_id:
  gotoxy (x, y1);
  for (;;)
    for (;;)
      switch (get_more (GET_INT, id, MAX_STUDENT_ID))
        {
        case GOT_ESC:
          return;
        case GOT_EMPTY:
          continue;
        case GOT_SUCCESS:
          goto get_password;
        }

get_password:
  gotoxy (x, y2);
  for (;;)
    switch (get_more (GET_ALNUM, password, MAX_STUDENT_NUMBER))
      {
      case GOT_ESC:
        return;
      case GOT_EMPTY:
        continue;
      case GOT_SUCCESS:
        goto check_info;
      }

check_info:
  temp = find_by (table_student, NULL, "id", TYP_STR, id);
  passwd = temp ? json_string_value (object_get (temp, "password")) : NULL;
  if (!temp || strcmp (passwd, password) != 0)
    {
      gotoxy (0, y3);
      printf ("学生不存在或密码错误");
      goto retry;
    }
  page_student ();

retry:
  gotoxy (x, y1);
  print_char (' ', MAX_STUDENT_ID);
  gotoxy (x, y2);
  print_char (' ', MAX_STUDENT_PASSWORD);
  goto get_id;
}

void
page_merchant_reg (void)
{
}

void
page_merchant_log (void)
{
}
