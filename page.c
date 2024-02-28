#include "page.h"
#include "config.h"
#include "table.h"
#include "tui.h"
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
      clear ();
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
  print_left_text ("4. 删除学生信息");

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
page_student_new (int mode)
{
  print_char ('\n', 3);
  printf ("请填写学生信息 (按 ESC 返回, ENTER 确认)\n");

  json_t *info = NULL;

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
  info = find_by (table_student, NULL, "id", TYP_STR, id);
  if (mode == 1 && info)
    {
      printf ("学号为: %s 的学生已经存在\n\n", id);
      goto get_id;
    }
  if (mode == 3 && !info)
    {
      printf ("学号为: %s 的学生不存在\n\n", id);
      goto get_id;
    }

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
  printf ("请输入学生电话 (不多于 %d 个数字): ", MAX_STUDENT_NUMBER);
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
  if (mode == 1)
    info = object_new ();

  object_set (info, "name", TYP_STR, name);
  object_set (info, "number", TYP_STR, number);

  if (mode == 1)
    {
      object_set (info, "id", TYP_STR, id);
      array_add (table_student, info);
    }

  save (table_student, PATH_TABLE_STUDENT);
}

void
page_student_look (int mode)
{
  print_char ('\n', 3);

  size_t idx;
  json_t *info = NULL;
  char id[MAX_STUDENT_ID + 1];

get_id:
  printf ("请输入学生学号 (按 ESC 返回, ENTER 确认): ");
  for (;;)
    switch (get_id (id, MAX_STUDENT_ID))
      {
      case GET_ESC:
        return;
      case GET_EMPTY:
        continue;
      case GET_SUCCESS:
        putchar ('\n');
        goto handle_info;
      }

handle_info:
  info = find_by (table_student, &idx, "id", TYP_STR, id);
  if (!info)
    {
      printf ("学号为: %s 的学生不存在\n\n", id);
      goto get_id;
    }

  json_t *name = object_get (info, "name");
  json_t *number = object_get (info, "number");

  putchar ('\n');
  printf ("学生学号: %s\n", id);
  printf ("学生昵称: %s\n", json_string_value (name));
  printf ("学生电话: %s\n\n", json_string_value (number));

  if (mode == 2)
    printf ("是否继续查看其他信息? (输入 y 确定, n 返回): ");
  if (mode == 4)
    printf ("是否确定删除该信息? (输入 y 确定, n 取消): ");

  for (char ch;;)
    switch ((ch = get_ascii ()))
      {
      case 'y':
        if (mode == 2)
          {
            print_char ('\n', 2);
            goto get_id;
          }
        if (mode == 4)
          {
            array_del (table_student, idx);
            save (table_student, PATH_TABLE_STUDENT);
          }
        return;
      case 'n':
      default:
        return;
      }
}

void
page_student (void)
{
  for (;;)
    {
      clear ();
      char input;
      page_student_menu ();
      printf ("输入数字选择操作，输入 q 返回: ");

    input:
      input = get_alnum ();
      switch (input)
        {
        case '1':
          page_student_new (1);
          break;
        case '2':
          page_student_look (2);
          break;
        case '3':
          page_student_new (3);
          break;
        case '4':
          page_student_look (4);
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
  print_left_text ("4. 删除商户信息");

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
page_merchant_new (int mode)
{
  print_char ('\n', 3);
  printf ("请填写商户信息 (按 ESC 返回, ENTER 确认)\n");

  json_t *info = NULL;

  char name[MAX_MERCHANT_NAME + 1];
  char number[MAX_MERCHANT_NUMBER + 1];
  char position[MAX_MERCHANT_POSITION + 1];

get_name:
  printf ("请输入商户名称 (不多于 %d 个字节): ", MAX_MERCHANT_NAME);
  for (;;)
    switch (get_str (name, MAX_MERCHANT_NAME))
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
  info = find_by (table_merchant, NULL, "name", TYP_STR, name);
  if (mode == 1 && info)
    {
      printf ("商户 %s 已经存在\n\n", name);
      goto get_name;
    }
  if (mode == 3 && !info)
    {
      printf ("商户 %s 不存在\n\n", name);
      goto get_name;
    }

  printf ("请输入商户电话 (不多于 %d 个数字): ", MAX_MERCHANT_NUMBER);
  for (;;)
    switch (get_id (number, MAX_MERCHANT_NUMBER))
      {
      case GET_ESC:
        return;
      case GET_EMPTY:
        continue;
      case GET_SUCCESS:
        putchar ('\n');
        goto get_position;
      }

get_position:
  printf ("请输入窗口位置 (不多于 %d 个字节): ", MAX_MERCHANT_POSITION);
  for (;;)
    switch (get_str (position, MAX_MERCHANT_POSITION))
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
  if (mode == 1)
    info = object_new ();

  object_set (info, "number", TYP_STR, number);
  object_set (info, "position", TYP_STR, position);

  if (mode == 1)
    {
      object_set (info, "name", TYP_STR, name);
      array_add (table_merchant, info);
    }

  save (table_merchant, PATH_TABLE_MERCHANT);
}

void
page_merchant_look (int mode)
{
  print_char ('\n', 3);

  size_t idx;
  json_t *info = NULL;
  char name[MAX_MERCHANT_NAME + 1];

get_name:
  printf ("请输入商户名称 (按 ESC 返回, ENTER 确认): ");
  for (;;)
    switch (get_str (name, MAX_MERCHANT_NAME))
      {
      case GET_ESC:
        return;
      case GET_EMPTY:
        continue;
      case GET_SUCCESS:
        putchar ('\n');
        goto handle_info;
      }

handle_info:
  info = find_by (table_merchant, &idx, "name", TYP_STR, name);
  if (!info)
    {
      printf ("商户 %s 不存在\n\n", name);
      goto get_name;
    }

  json_t *number = object_get (info, "number");
  json_t *position = object_get (info, "position");

  putchar ('\n');
  printf ("商户名称: %s\n", name);
  printf ("商户电话: %s\n", json_string_value (number));
  printf ("窗口位置: %s\n\n", json_string_value (position));

  if (mode == 2)
    printf ("是否继续查看其他信息? (输入 y 确定, n 退出): ");
  if (mode == 4)
    printf ("是否确定删除该信息? (输入 y 确定, n 取消): ");

  for (char ch;;)
    switch ((ch = get_ascii ()))
      {
      case 'y':
        if (mode == 2)
          {
            print_char ('\n', 2);
            goto get_name;
          }
        if (mode == 4)
          {
            array_del (table_merchant, idx);
            save (table_merchant, PATH_TABLE_MERCHANT);
          }
        return;
      case 'n':
      default:
        return;
      }
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
          page_merchant_new (1);
          break;
        case '2':
          page_merchant_look (2);
          break;
        case '3':
          page_merchant_new (3);
          break;
        case '4':
          page_merchant_look (4);
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
