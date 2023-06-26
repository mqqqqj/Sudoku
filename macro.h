#pragma once
#ifndef __MACRO_H__
#define __MACRO_H__
/*
* sudoku用到的宏定义，常数，及默认参数
*/

// PART1.题目要求
#define DEFAULT_C 20                      // 默认生成20个终盘
#define DEFAULT_PATH "game.txt"           // 默认求解game.txt下的数独
#define DEFAULT_N 50                      // 默认生成50个游戏
#define DEFAULT_M 2                       // 默认难度为2
#define DEFAULT_RL 20                     // 默认挖最少20个空
#define DEFAULT_RR 55                     // 默认挖最多55个空，RL和RR分别表示r的left和right
#define DEFAULT_U true                    // 默认唯一解

// PART2.自定义
#define DEFAULT_SIZE 3                    // 即棋盘默认3*3=9个宫，还可以取2或4


#endif