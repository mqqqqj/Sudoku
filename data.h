#pragma once
#ifndef __DATA_H__
#define __DATA_H__
/*
* sudoku用到的数据结构
*/

struct Param {
	int c;           // 终盘数量  [1, 1e6]
	char* s;         // 待求解棋盘的相对或绝对路径
	int n;           // 游戏数量
	int m;           // 游戏难度  [1, 3]
	int r;           // 挖空数量  [20, 55]
	bool u;          // 唯一解

	int l;           // 棋盘长度：2、3、4
};

Param params;
int** board;         // 棋盘

#endif