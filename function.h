
#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
/*
* 【function.h】sudoku用到的功能函数
*/

/*
* 参数相关
*/
bool read_args(int argc, char* argv[]);                                                 // 读取参数，初始化params和board两个全局变量
void print_params();                                                                    // 打印参数
bool check_args();                                                                      // 检查参数是否合法 

/*
* 游戏相关
*/
bool generate_board(int mod);                                                           // 生成棋盘，mod模式：0，无效参数；1，随机填充；2，棋盘终局；3，有解棋盘；4，唯一解棋盘
std::pair<int, int> find_empty_cell(const std::vector<std::vector<int>>& board);        // 找到下一个空
bool is_valid(const std::vector<std::vector<int>>& board, int row, int col, int num);   // 判断某个数字填入棋盘是否合法
bool solve_sudoku(std::vector<std::vector<int>>& board);                                // 使用回溯算法填充数独棋盘
bool has_solution(const std::vector<std::vector<int>>& board);                          // 判断是否有解
int check_unique(std::vector<std::vector<int>>& board, int x, int y, int count);        // 检查是否唯一解(返回解的个数)
bool dig_hole(std::vector<std::vector<int>>& board);                                    // 挖空                        

/*
* IO读写文件
*/
std::vector<std::vector<std::vector<int>>> read_file(const char* file_name);            // 读数独文件到棋盘
void write_file(const char* file_name, std::vector<std::vector<int>> board);            // 写数独棋盘到文件
void clear_file(const char* file_name);                                                 // 清空文件

/*
* 调试相关
*/
char get_num(int num);                                                                  // 得到棋盘元素对应字符以打印，0转换为$，超过10的转换为字母
void draw_board(const std::vector<std::vector<int>>& board);                            // 画棋盘
void Log(const char* buf, int type);                                                    // 打印日志，type：1，正常；2，警告；3，错误

#endif