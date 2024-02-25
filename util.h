#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

extern FILE *file_menu;
extern FILE *file_student;
extern FILE *file_merchant;
extern FILE *file_evaluation;

/*
 * 检查信息表文件并初始化
 */
extern void check_and_init (void);

#endif
