
#ifndef __MACRO_H__
#define __MACRO_H__
/*
* 【macor.h】sudoku用到的宏定义，常数，及默认参数
*/
namespace sudoku {
	// PART1.题目要求
	#define DEFAULT_C 20                      // 默认生成20个终盘
	#define DEFAULT_PATH "game.txt"           // 默认生成数独到game.txt
	#define DEFAULT_SAVE_PATH "sudoku.txt"    // 默认求解数独结果保存到sudoku.txt
	#define DEFAULT_ANS_PATH "answer.txt"     // 默认生成数独的解保存到answer.txt
	#define DEFAULT_N 50                      // 默认生成50个游戏
	#define DEFAULT_M 2                       // 默认难度为2
	#define DEFAULT_RL 20                     // 默认挖最少20个空
	#define DEFAULT_RR 55                     // 默认挖最多55个空，RL和RR分别表示r的left和right
	#define DEFAULT_U true                    // 默认唯一解

	// PART2.自定义
	#define DEFAULT_SIZE 3                    // 即棋盘默认3*3=9个宫，还可以取2或4
	#define HELP_STRING "【sudoku.exe parameters helper】\n\
	-c: num of final boards [eg] -c 20\n\
	-s: path of boards unsolved [eg] -s game.txt\n\
	-n: num of games generated [eg] -n 1000\n\
	-m: difficulty of the games generated(1-esay, 2-normal, 3-hard) [eg] -n 1000 -m 1\n\
	-r: min and max num of blanks(split with ~) [eg] -n 20 -r 20~55\n\
	-u: unique solution boards [eg] -n 20 -u\n"

}
#endif