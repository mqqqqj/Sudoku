#pragma once
#ifndef __FUNCTION_H__
#define __FUNCTION_H__
/*
* sudoku用到的功能函数
*/

bool read_args(int argc, char* argv[]);     // 读取参数，初始化params和board两个全局变量
bool check_args();                          // 检查参数是否合法 
bool generate_board(int mod);               // 生成棋盘，mod模式：0，无效参数；1，随机；2，正常棋盘终局；3，唯一解棋盘终局
void draw_board();                          // 画棋盘

#endif