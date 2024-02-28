#ifndef TABLE_H
#define TABLE_H

#include <jansson.h>

extern json_t *info;
extern json_t *table_menu;
extern json_t *table_student;
extern json_t *table_merchant;
extern json_t *table_evaluation;

/*
 * 初始化数据表
 */
extern void table_init (void);

enum
{
  TYP_INT,
  TYP_STR,
};

/*
 * 根据键值查找条目
 */
extern json_t *find_by (json_t *table, size_t *idx, const char *key, int typ,
                        ...);

/*
 * 生成一个空对象
 */
extern json_t *object_new (void);

/*
 * 添加或覆写对象成员
 */
extern void object_set (json_t *obj, const char *key, int typ, ...);

/*
 * 读取对象成员
 */
extern json_t *object_get (json_t *obj, const char *key);

/*
 * 删除对象成员
 */
extern void object_del (json_t *obj, const char *key);

/*
 * 向数组中追加成员
 */
extern void array_add (json_t *arr, json_t *ele);

/*
 * 删除数组成员
 */
extern void array_del (json_t *arr, size_t idx);

/*
 * 数据表化并存储
 */
extern void save (json_t *from, const char *to);

#endif
