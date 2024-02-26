#include "page.h"
#include "tui.h"
#include <stdio.h>
#include <stdlib.h>

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
  print_block_text ("print_block_text 支持显示超长字符串，"
                    "且会自动处理换行，可以在字符串中包含换行符。");

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
      input = getchar ();
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
          exit (0);
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
page_student (void)
{
  for (;;)
    {
      char input;
      page_student_menu ();
      printf ("输入数字选择操作，输入 q 返回: ");

    input:
      input = getchar ();
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
      input = getchar ();
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
