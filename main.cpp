
#include "function.h"
#include "data.h"
#include "macro.h"

using namespace sudoku;

int main(int argc, char* argv[]) {

    if (!read_args(argc, argv)) {
        Log("读取参数错误，请重新执行！", 3);
        Log(HELP_STRING, 1);
        system("pause");
        return -1;
    };

    print_params();
    system("pause");

    // 生成不重复的board
    std::set<std::vector<std::vector<int>>> uniqueBoards;
    int uniqueCnt = 0;

    // 参数c
    if (has_args[0]) {
        //生成若干个数独终盘
        clear_file(DEFAULT_PATH);
        while (uniqueCnt < params.c) {
            generate_board(2);
            std::vector<std::vector<int>> copied_board = board;
            uniqueBoards.insert(copied_board);
            // 不重复才计数、打印、写文件
            if (uniqueCnt < uniqueBoards.size()) {
                uniqueCnt++;
                draw_board(board);
                write_board(DEFAULT_PATH, board);
            }
        }
    }
    // 参数s
    else if (has_args[1]) {
        //从文件中读取若干数独游戏,求解并输出到指定sudoku.txt文件
        clear_file(DEFAULT_SAVE_PATH);
        read_and_solve(DEFAULT_PATH, DEFAULT_SAVE_PATH);
    }
    // 参数 n
    else if (has_args[2]) {
        clear_file(DEFAULT_PATH);
        clear_file(DEFAULT_ANS_PATH);
        // 如果设置了难度m，首先更新r
        if (has_args[3]) {
            if (params.m == 1) {
                params.rl = 20;
                params.rr = 30;
            }
            else if (params.m == 2) {
                params.rl = 30;
                params.rr = 45;
            }
            else if (params.m == 3) {
                params.rl = 45;
                params.rr = 55;
            }
        }
        // 然后生成，注意区分是否有-u参数
        while (uniqueCnt < params.n) {
            if (has_args[6])
                generate_board(4);
            else
                generate_board(3);
            std::vector<std::vector<int>> copied_board = board_unsolved;
            std::vector<std::vector<int>> copied_board_ans = board;
            uniqueBoards.insert(copied_board);
            // 不重复才计数、打印、写文件
            if (uniqueCnt < uniqueBoards.size()) {
                uniqueCnt++;
                draw_board(copied_board);
                write_board(DEFAULT_PATH, copied_board);
                write_board(DEFAULT_ANS_PATH, copied_board_ans);
            }
        }

    }

    system("pause");
    return 0;
}