#include "pch.h"
#include "../function.h"
#include "../data.h"
#include "../macro.h"
#include "../debug.cpp"
#include "../sudoku.cpp"

using namespace sudoku;



TEST(TESTLOG, TestLOG) {
    // 创建一个输出流用于捕获 cout 的输出
    std::ostringstream oss;
    // 重定向 cout 到输出流
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    // 在函数或代码块中执行你要测试的输出
    Log("test log", 1);
    // 恢复 cout 到原来的缓冲区
    std::cout.rdbuf(coutBuffer);

    // 检查输出是否符合预期
    std::string output = oss.str();
    EXPECT_EQ("[LOG]:test log\n", output);
}
/*
TEST(TestREADARGS, WrongParams) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 12;
    char* c_args[] = { "**.exe", "-c", "2", "-s", "game.txt", "-n", "1", "-m", "2", "-r", "20~30", "-u" };
    bool res = read_args(n_args, c_args);
    EXPECT_EQ(false, res);
}*/


TEST(TestREADARGS, RightParams) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 3;
    char* c_args[] = { "**.exe", "-c", "2" };
    bool res = read_args(n_args, c_args);
    EXPECT_EQ(true, res);
        // char* c_args2[] = { "**.exe", "-n", "2" };
    // bool res2 = read_args(n_args, c_args);
    // EXPECT_EQ(true, res2);                     // true
        // char* c_args3[] = { "**.exe", "-m", "1" };
    // bool res3 = read_args(n_args, c_args);
    // EXPECT_EQ(false, res3);                    // false
        // char* c_args2[] = { "**.exe", "-n", "2", "-m", "1" };
    // bool res4 = read_args(n_args, c_args);
    // EXPECT_EQ(true, res4);                     // true
}

TEST(TestCheckArgs, CheckM) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 5;
    char* c_args[] = { "**.exe", "-n", "2", "-m", "1" };
    bool res = read_args(n_args, c_args);
    EXPECT_EQ(true, res);
}

TEST(TestCheckArgs, CheckU) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 4;
    char* c_args[] = { "**.exe", "-n", "2", "-u" };
    bool res = read_args(n_args, c_args);
    EXPECT_EQ(true, res);
}

TEST(TestCheckArgs, CheckR) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 5;
    char* c_args[] = { "**.exe", "-n", "2", "-r" , "20~55" };
    bool res = read_args(n_args, c_args);
    EXPECT_EQ(true, res);
}
TEST(TestIO, WriteBoard) {
    // 创建一个输出流用于捕获 cout 的输出
    std::ostringstream oss;
    // 重定向 cout 到输出流
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    write_board("test.txt", board);

    // 恢复 cout 到原来的缓冲区
    std::cout.rdbuf(coutBuffer);

    // 检查输出是否符合预期
    std::string output = oss.str();
    EXPECT_EQ("[LOG]:board written\n", output);

}

TEST(TestGENBOARD, TESTGENBOARD_0) {
    EXPECT_EQ(generate_board(0), false);
}

TEST(TestPlay, TestNMU) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 6;
    char* c_args[] = { "**.exe", "-n", "2", "-m" , "2" , "-u" };
    read_args(n_args, c_args);
    print_params();
    std::set<std::vector<std::vector<int>>> uniqueBoards;
    int uniqueCnt = 0;
    clear_file(DEFAULT_PATH);
    clear_file(DEFAULT_ANS_PATH);
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

TEST(TestPlay, TESTC) {
    extern std::vector<std::vector<int>> board;
    extern std::vector<std::vector<int>> board_unsolved;
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 3;
    char* c_args[] = { "**.exe", "-c", "2" };
    read_args(n_args, c_args);
    print_params();

    // 生成不重复的board
    std::set<std::vector<std::vector<int>>> uniqueBoards;
    int uniqueCnt = 0;
    //生成若干个数独终盘
    clear_file(DEFAULT_PATH);
    while (uniqueBoards.size() < params.c) {
        generate_board(2);
        std::vector<std::vector<int>> copied_board = board;
        copied_board.resize(board.size()); // 调整外层向量大小
        for (size_t i = 0; i < board.size(); i++) {
            copied_board[i] = board[i]; // 对内层向量进行深拷贝
        }
        if (std::find(uniqueBoards.begin(), uniqueBoards.end(), copied_board) == uniqueBoards.end()) {
            // 不存在，才插入
            uniqueBoards.insert(copied_board);
            //draw_board(board);
            //write_board(DEFAULT_PATH, board);
        }
    }
}

//:: testing :: InitGoogleTest（）函数解析Google测试标志的命令行，
//并删除所有已识别的标志。 这允许用户通过各种标志控制测试程序的行为.
// 在调用RUN_ALL_TESTS（）之前必须调用此函数，否则标志将无法正确初始化。
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
