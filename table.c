#include "table.h"
#include "config.h"
#include "util.h"
#include <jansson.h>
#include <stdarg.h>
#include <string.h>

json_t *table_menu;
json_t *table_student;
json_t *table_merchant;
json_t *table_evaluation;

static inline FILE *
load_file (const char *path)
{
  FILE *file = fopen (path, "r+");
  if (file)
    /* 打开成功 */
    return file;

  file = fopen (path, "w+");
  if (!file)
    error ("数据表 %s 创建失败", path);

  if (fputs ("[]", file) == EOF)
    error ("数据表 %s 初始化失败", path);

  return file;
}

static inline json_t *
load_table (FILE *file)
{
  if (fseek (file, 0, SEEK_SET) != 0)
    error ("文件流重定位失败");

  json_t *json;
  json_error_t jerr;

  json = json_loadf (file, 0, &jerr);
  if (!json)
    error ("json 解析失败");

  if (!json_is_array (json))
    error ("json 格式错误");

  fclose (file);
  return json;
}

void
table_init ()
{
  table_menu = load_table (load_file (PATH_TABLE_MENU));
  table_student = load_table (load_file (PATH_TABLE_STUDENT));
  table_merchant = load_table (load_file (PATH_TABLE_MERCHANT));
  table_evaluation = load_table (load_file (PATH_TABLE_EVALUATION));
}

json_t *
find_by (json_t *table, size_t *idx, const char *key, int typ, ...)
{
  size_t size = json_array_size (table);
  if (!size)
    return NULL;

  int ival;
  char *sval;
  json_t *obj = NULL;

  va_list ap;
  va_start (ap, typ);

  switch (typ)
    {
    case TYP_INT:
      ival = va_arg (ap, int);
      break;
    case TYP_STR:
      sval = va_arg (ap, char *);
      break;
    default:
      error ("未知数据类型 %d", typ);
    }

  for (size_t i = 0; i < size; i++)
    {
      json_t *item = json_array_get (table, i);
      json_t *jval = json_object_get (item, key);
      if (!jval)
        error ("json 数据格式错误: 缺少 %s 键值", key);

      switch (typ)
        {
        case TYP_INT:
          if (ival != json_integer_value (jval))
            continue;
          break;

        case TYP_STR:
          if (strcmp (sval, json_string_value (jval)) != 0)
            continue;
          break;
        }

      if (idx)
        *idx = i;
      obj = item;
      goto ret;
    }

ret:
  va_end (ap);
  return obj;
}

json_t *
object_new (void)
{
  json_t *obj = json_object ();
  if (!obj)
    error ("json 对象创建失败");
  return obj;
}

void
object_set (json_t *obj, const char *key, int typ, ...)
{
  int ival;
  char *sval;
  json_t *jval = NULL;

  va_list ap;
  va_start (ap, typ);

  switch (typ)
    {
    case TYP_INT:
      ival = va_arg (ap, int);
      jval = json_integer (ival);
      break;
    case TYP_STR:
      sval = va_arg (ap, char *);
      jval = json_string (sval);
      break;
    default:
      error ("未知数据类型 %d", typ);
    }

  if (!jval)
    error ("json 数据节点创建失败");

  if (0 != json_object_set_new (obj, key, jval))
    error ("json 对象成员插入失败");

  va_end (ap);
}

json_t *
object_get (json_t *obj, const char *key)
{
  json_t *data = json_object_get (obj, key);
  if (!data)
    error ("json 数据格式错误: 缺少 %s 键值", key);
  return data;
}

void
object_del (json_t *obj, const char *key)
{
  if (0 != json_object_del (obj, key))
    error ("json 对象成员删除失败");
}

void
array_add (json_t *arr, json_t *ele)
{
  if (0 != json_array_append_new (arr, ele))
    error ("json 数组成员插入失败");
}

void
array_del (json_t *obj, size_t idx)
{
  if (0 != json_array_remove (obj, idx))
    error ("json 数组成员删除失败");
}

void
save (json_t *from, const char *to)
{
  char *str = json_dumps (from, JSON_INDENT (2));
  if (!str)
    error ("json 编码失败");

  FILE *file = fopen (to, "w+");
  if (!file)
    error ("文件 %s 打开失败", to);

  size_t len = strlen (str);
  if (fwrite (str, len, 1, file) != 1)
    error ("数据写入失败");

  fclose (file);
  free (str);
}
