#include "page.h"
#include "config.h"
#include "tui.h"
#include "util.h"
#include <jansson.h>
#include <stdio.h>

void
page_main_menu (void)
{
  print_char ('\n', 3);
  print_full_line ();

  print_empty_line ();
  print_center_text ("食堂菜品管理系统");
  print_empty_line ();

  print_split_line ();
  print_empty_line ();

  print_left_text ("1. 学生操作");
  print_left_text ("2. 商户操作");
  print_left_text ("3. 退出");

  print_empty_line ();
  print_full_line ();
}

void
page_main (void)
{
  for (;;)
    {
      char input;
      page_main_menu ();
      printf ("输入数字选择操作，输入 q 退出: ");

    input:
      input = get_alnum ();
      switch (input)
        {
        case '1':
          page_student ();
          break;
        case '2':
          page_merchant ();
          break;
        case '3':
        case 'q':
          return;
        default:
          goto input;
        }
    }
}

void
page_student_menu (void)
{
  print_char ('\n', 3);
  print_full_line ();

  print_empty_line ();
  print_center_text ("学生操作");
  print_empty_line ();

  print_split_line ();
  print_empty_line ();

  print_left_text ("1. 新增学生信息");
  print_left_text ("2. 查看学生信息");
  print_left_text ("3. 修改学生信息");
  print_left_text ("3. 删除学生信息");

  print_empty_line ();

  print_left_text ("4. 浏览菜品信息");
  print_left_text ("5. 浏览评价信息");
  print_left_text ("6. 浏览统计信息");

  print_empty_line ();

  print_left_text ("7. 评价菜品");
  print_left_text ("8. 返回主页");

  print_empty_line ();
  print_full_line ();
}

void
page_student_new (void)
{
  print_char ('\n', 3);
  printf ("请填写学生信息 (按 ESC 返回, ENTER 确认)\n");

  char id[MAX_STUDENT_ID + 1];
  char name[MAX_STUDENT_NAME + 1];
  char number[MAX_STUDENT_NUMBER + 1];

get_id:
  printf ("请输入学生学号 (不多于 %d 个数字): ", MAX_STUDENT_ID);
  for (;;)
    switch (get_id (id, MAX_STUDENT_ID))
      {
      case GET_ESC:
        return;
      case GET_EMPTY:
        continue;
      case GET_SUCCESS:
        putchar ('\n');
        goto get_name;
      }

get_name:
  printf ("请输入学生昵称 (不多于 %d 个字节): ", MAX_STUDENT_NAME);
  for (;;)
    switch (get_str (name, MAX_STUDENT_NAME))
      {
      case GET_ESC:
        return;
      case GET_EMPTY:
        continue;
      case GET_SUCCESS:
        putchar ('\n');
        goto get_number;
      }

get_number:
  printf ("请输入学生手机号 (不多于 %d 个数字): ", MAX_STUDENT_NUMBER);
  for (;;)
    switch (get_id (number, MAX_STUDENT_NUMBER))
      {
      case GET_ESC:
        return;
      case GET_EMPTY:
        continue;
      case GET_SUCCESS:
        putchar ('\n');
        goto build_info;
      }

build_info:
  if (json_object_get (table_student, id))
    {
      printf ("学号为: %s 的学生已经存在\n\n", id);
      goto get_id;
    }

  json_t *info = json_object ();
  if (!info)
    error ("json_object failed");

  if (0 != json_object_set_new (info, "name", json_string (name)))
    error ("json_object_set_new for name failed");
  if (0 != json_object_set_new (info, "number", json_string (number)))
    error ("json_object_set_new for number failed");
  if (0 != json_object_set_new (table_student, id, info))
    error ("json_object_set_new for %s failed", id);

  save (table_student, file_student);
}

void
page_student (void)
{
  for (;;)
    {
      char input;
      page_student_menu ();
      printf ("输入数字选择操作，输入 q 返回: ");

    input:
      input = get_alnum ();
      switch (input)
        {
        case '1':
          page_student_new ();
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
page_merchant_menu (void)
{
  print_char ('\n', 3);
  print_full_line ();

  print_empty_line ();
  print_center_text ("商户操作");
  print_empty_line ();

  print_split_line ();
  print_empty_line ();

  print_left_text ("1. 新增商户信息");
  print_left_text ("2. 查看商户信息");
  print_left_text ("3. 修改商户信息");
  print_left_text ("3. 删除商户信息");

  print_empty_line ();

  print_left_text ("4. 发布菜品");
  print_left_text ("5. 更新菜品");

  print_empty_line ();

  print_left_text ("6. 浏览评价");
  print_left_text ("7. 返回主页");

  print_empty_line ();
  print_full_line ();
}

void
page_merchant (void)
{
  for (;;)
    {
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
        case 'q':
          return;
        default:
          goto input;
        }
    }
}
