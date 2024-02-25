#ifndef CONFIG_H
#define CONFIG_H

/**
 * 信息表存储地址，默认为当前目录
 *
 * menu.json: 菜品信息表
 * student.json: 学生信息表
 * merchant.json: 商户信息表
 * evaluation.json: 评价信息表
 */

#define PATH_TABLE_MENU "./menu.json"
#define PATH_TABLE_STUDENT "./student.json"
#define PATH_TABLE_MERCHANT "./merchant.json"
#define PATH_TABLE_EVALUATION "./evaluation.json"

/* 
 * TUI 配置信息
 */

#define TUI_FRAME_CHAR '*'
#define TUI_FRAME_WIDTH 60
#define TUI_NASCII_WIDTH 2

#endif
