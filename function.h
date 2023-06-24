#pragma once
#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <iostream>
#include <fstream>
#include <random>
/*
* sudoku用到的功能函数
*/

bool read_args(int argc, char* argv[]);     // 读取参数，初始化params和board两个全局变量
bool check_args();                          // 检查参数是否合法 
bool generate_board(int mod);               // 生成棋盘，mod模式：0，无效参数；1，随机；2，正常棋盘终局；3，唯一解棋盘终局
char get_num(int num);                      // 得到棋盘元素对应字符以打印，0转换为$，超过10的转换为字母
void draw_board();                          // 画棋盘
std::pair<int, int> find_empty_cell();      // 找到下一个空
bool is_valid(int row, int col, int num);   // 判断某个数字填入棋盘是否合法
bool solve_sudoku();                        // 使用回溯算法填充数独棋盘

/*
* 调试专用
*/
void Log(const char* buf, int type);            // 打印日志，type：1，正常；2，警告；3，错误

#endif