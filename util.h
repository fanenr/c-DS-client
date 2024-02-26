#ifndef UTIL_H
#define UTIL_H

#include <jansson.h>
#include <stdio.h>

extern FILE *file_menu;
extern FILE *file_student;
extern FILE *file_merchant;
extern FILE *file_evaluation;

extern json_t *table_menu;
extern json_t *table_student;
extern json_t *table_merchant;
extern json_t *table_evaluation;

/*
 * 遇到致命错误并退出
 */
extern void eprintf (const char *fmt, ...)
    __attribute__ ((noreturn, format (printf, 1, 2)));

#define error(FMT, ...) eprintf ("%s: " FMT "\n", __FUNCTION__, ##__VA_ARGS__)

/*
 * 检查信息表并初始化
 */
extern void init (void);

/*
 * 程序析构并释放资源
 */
extern void deinit (void);

/*
 * 数据表化并存储
 */
extern void save (json_t *from, FILE *to);

#endif
