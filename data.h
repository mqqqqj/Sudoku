
#ifndef __DATA_H__
#define __DATA_H__
#include <vector>
/*
* 【data.h】sudoku用到的数据结构
*/

namespace sudoku {

	struct Param {
		int c;           // 终盘数量  [1, 1e6]
		char s[100];     // 待求解棋盘的相对或绝对路径
		int n;           // 游戏数量
		int m;           // 游戏难度  [1, 3]
		int rl;          // 挖空数量  [20, 55]
		int rr;          // 挖空数量  [20, 55]
		bool u;          // 唯一解

		int l;           // 棋盘长度：2、3、4
		int r;           // 当前空的数量
	};

#define LL long long

extern char buf[50];                                    // DEBUG
extern std::vector<std::vector<int>> board;             // 棋盘终局
extern std::vector<std::vector<int>> board_unsolved;    // 棋盘，待求解
extern Param params;                                    // 参数数值
extern bool has_args[7];                                // 参数存在
}
#endif