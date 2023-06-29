#include "pch.h"
#include "../function.h"
#include "../data.h"
#include "../macro.h"
#include "../debug.cpp"
#include "../sudoku.cpp"

using namespace sudoku;

/*
* ZYR
*/


std::vector<std::vector<int>> test_board =
{ {7, 9, 6, 0, 4, 1, 5, 8, 2},
    {3, 5, 1, 0, 2, 8, 0, 4, 0},
    {0, 0, 4, 9, 5, 0, 7, 1, 3},
    {0, 7, 0, 4, 9, 2, 0, 3, 0},
    {9, 4, 2, 0, 0, 3, 0, 0, 0},
    {6, 0, 0, 0, 0, 0, 2, 0, 4},
    {0, 0, 0, 6, 8, 0, 4, 2, 1},
    {0, 6, 8, 0, 3, 0, 9, 5, 0},
    {4, 2, 7, 5, 1, 9, 0, 0, 8},
};

std::vector < std::vector<int>> no_solution_board = {
    {5, 3, 3, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9},
};


std::vector < std::vector<int>> filled_board = {
    {9, 5, 3, 7, 4, 8, 1, 6, 2},
    {1, 7, 4, 2, 5, 6, 8, 3, 9},
    {2, 8, 6, 3, 9, 1, 5, 7, 4},
    {5, 6, 9, 4, 7, 3, 2, 1, 8},
    {4, 2, 8, 1, 6, 9, 7, 5, 3},
    {7, 3, 1, 5, 8, 2, 9, 4, 6},
    {3, 9, 2, 6, 1, 5, 4, 8, 7},
    {8, 4, 5, 9, 3, 7, 6, 2, 1},
    {6, 1, 7, 8, 2, 4, 3, 9, 5},
};



/*
* ZYR
*/

TEST(TestDebug, LOG) {
    testing::internal::CaptureStdout(); // 捕获标准输出
    Log("test log none", 0);
    std::string output1 = testing::internal::GetCapturedStdout(); // 获取捕获的标准输出

    testing::internal::CaptureStdout(); // 捕获标准输出
    Log("test log normal", 1);
    std::string output2 = testing::internal::GetCapturedStdout(); // 获取捕获的标准输出

    testing::internal::CaptureStdout(); // 捕获标准输出
    Log("test log warning", 2);
    std::string output3 = testing::internal::GetCapturedStdout(); // 获取捕获的标准输出

    testing::internal::CaptureStdout(); // 捕获标准输出
    Log("test log error", 3);
    std::string output4 = testing::internal::GetCapturedStdout(); // 获取捕获的标准输出


    // 检查输出是否符合预期
    EXPECT_EQ("test log none\n", output1);
    EXPECT_EQ("[LOG]:test log normal\n", output2);
    EXPECT_EQ("[WARN]:test log warning\n", output3);
    EXPECT_EQ("[ERROR]:test log error\n", output4);
}


TEST(TestDebug, GetNum) {
    // 输入整数，输出的是棋盘不同的符号，0、0-9、9-17、else
    int nums[5] = { 0,3,12,19,-2 };
    char chs[5];
    for (int i = 0; i < 5; i++) {
        chs[i] = get_num(nums[i]);
    }

    EXPECT_EQ(chs[0], '$');
    EXPECT_EQ(chs[1], '3');
    EXPECT_EQ(chs[2], 'C');
    EXPECT_EQ(chs[3], 'X');
    EXPECT_EQ(chs[4], 'X');
}

TEST(TestDebug, DrawBoard) {
    // 自拟棋盘，只设置了前5个
    std::vector<std::vector<int>> my_board(9, std::vector<int>(9, 0));
    my_board[0][0] = 0;
    my_board[0][1] = 3;
    my_board[0][2] = 12;
    my_board[0][3] = 19;
    my_board[0][4] = -2;
    params.l = 3;
    // 使用stringstream模拟输出到标准输出
    std::stringstream output;
    // 将输出重定向到stringstream
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
    // 输出函数调用
    draw_board(my_board);
    // 恢复标准输出
    std::cout.rdbuf(oldCoutBuffer);

    // 验证输出是否符合预期
    std::string expectedOutput = "\
*************************\n\
+-------+-------+-------+\n\
| $ 3 C | X X $ | $ $ $ | \n\
| $ $ $ | $ $ $ | $ $ $ | \n\
| $ $ $ | $ $ $ | $ $ $ | \n\
+-------+-------+-------+\n\
| $ $ $ | $ $ $ | $ $ $ | \n\
| $ $ $ | $ $ $ | $ $ $ | \n\
| $ $ $ | $ $ $ | $ $ $ | \n\
+-------+-------+-------+\n\
| $ $ $ | $ $ $ | $ $ $ | \n\
| $ $ $ | $ $ $ | $ $ $ | \n\
| $ $ $ | $ $ $ | $ $ $ | \n\
+-------+-------+-------+\n\
*************************\n";

    EXPECT_EQ(output.str(), expectedOutput);
}


TEST(TestIO, WriteBoard) {
    // 测试write_board，1.检查是否正常打印  2.检查写入文件的board是否正确
    std::string outputPath = "D:/testdata/test_writeboard_output.txt";
    std::string expectedOutputPath = "D:/testdata/test_writeboard_expected_output.txt";
    std::vector<std::vector<int>> my_board(9, std::vector<int>(9, 0));
    my_board[0][0] = 0;
    my_board[0][1] = 3;
    my_board[0][2] = 12;
    my_board[0][3] = 19;
    my_board[0][4] = -2;
    params.l = 3;
    params.m = 2;
    params.r = 33;
    // 清空文件
    std::ofstream outputFile(outputPath, std::ios::trunc);
    ASSERT_TRUE(outputFile.is_open());
    outputFile.close();
    // 1.1创建一个输出流用于捕获 cout 的输出
    std::ostringstream oss;
    // 1.2重定向 cout 到输出流
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    write_board(outputPath.c_str(), my_board);
    // 1.3恢复 cout 到原来的缓冲区
    std::cout.rdbuf(coutBuffer);
    // 1.4检查输出是否符合预期
    std::string output = oss.str();
    EXPECT_EQ("[LOG]:board written\n", output);
    // 2.1读取期望的输出文件
    std::string line;
    std::ifstream expectedOutputFile(expectedOutputPath);
    ASSERT_TRUE(expectedOutputFile.is_open());
    std::string expectedOutputData;
    while (std::getline(expectedOutputFile, line)) {
        expectedOutputData += line;
    }
    expectedOutputFile.close();
    // 2.2读取生成的输出文件
    std::ifstream generatedOutputFile(outputPath);
    ASSERT_TRUE(generatedOutputFile.is_open());
    std::string generatedOutputData;
    while (std::getline(generatedOutputFile, line)) {
        generatedOutputData += line;
    }
    generatedOutputFile.close();
    // 2.3比较生成的输出与期望的输出
    EXPECT_EQ(expectedOutputData, generatedOutputData);
}

TEST(TestIO, ReadBoard) {
    // 测试std::vector<std::vector<std::vector<int>>> read_boards(const char* file_name)
    std::string inputFilePath = "D:/testdata/test_readboard_input.txt";
    // 调用函数读取
    std::vector<std::vector<std::vector<int>>> boards = read_boards(inputFilePath.c_str());
    // 使用stringstream模拟输出到标准输出
    std::stringstream output;
    // 将输出重定向到stringstream
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
    // 输出函数调用
    draw_board(boards[0]);
    draw_board(boards[1]);
    // 恢复标准输出
    std::cout.rdbuf(oldCoutBuffer);

    // 验证输出是否符合预期
    std::string expectedOutput = "\
*************************\n\
+-------+-------+-------+\n\
| 2 8 $ | $ 1 6 | 7 4 5 | \n\
| $ 4 9 | 2 $ $ | $ $ $ | \n\
| 6 $ 1 | 5 4 3 | 9 8 $ | \n\
+-------+-------+-------+\n\
| $ 9 $ | $ 2 5 | $ 1 3 | \n\
| 1 $ 6 | $ 3 $ | $ 2 7 | \n\
| 3 2 4 | 7 6 1 | 8 5 9 | \n\
+-------+-------+-------+\n\
| $ 6 7 | $ $ 8 | 2 3 4 | \n\
| 8 $ 2 | 3 7 $ | 5 $ $ | \n\
| 4 $ 5 | $ 9 2 | $ $ $ | \n\
+-------+-------+-------+\n\
*************************\n\
*************************\n\
+-------+-------+-------+\n\
| $ $ $ | $ 1 $ | 7 $ $ | \n\
| 5 4 9 | $ 8 7 | 3 6 $ | \n\
| 6 7 1 | $ 4 $ | 9 $ 2 | \n\
+-------+-------+-------+\n\
| $ 9 8 | 4 $ 5 | 6 $ $ | \n\
| 1 $ $ | $ 3 9 | 4 2 7 | \n\
| $ $ $ | $ $ 1 | 8 $ 9 | \n\
+-------+-------+-------+\n\
| $ $ 7 | 1 $ 8 | 2 3 $ | \n\
| 8 $ $ | $ $ 4 | 5 9 6 | \n\
| 4 3 $ | 6 9 2 | 1 7 8 | \n\
+-------+-------+-------+\n\
*************************\n";
    // 比较生成的输出与期望的输出
    EXPECT_EQ(output.str(), expectedOutput);

}

TEST(TestIO, ClearFile) {
    // 测试void clear_file(const char* file_name)
    std::string file_path = "D:/testdata/test_clearfile.txt";
    // 把内容先读出来
    std::ifstream file(file_path);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::string file_data;
    // 清空文件
    clear_file(file_path.c_str());
    // 再次读取
    std::string file_data_new;
    while (std::getline(file, line)) {
        file_data_new = file_data + "\n" + line;
    }
    file.close();
    // 验证为空
    EXPECT_EQ(file_data_new, "");

}

TEST(TestIO, ReadAndSolve) {
    // 测试bool read_and_solve(const char* in_file, const char* out_file);
    std::string in_path = "D:/testdata/test_readsolve_input.txt";
    std::string out_path = "D:/testdata/test_readsolve_output.txt";
    std::string expect_path = "D:/testdata/test_readsolve_expected_output.txt";
    // 清空输出文件
    clear_file(out_path.c_str());
    // 调用函数
    read_and_solve(in_path.c_str(), out_path.c_str());

    // 读取输出文件
    std::string line;
    std::ifstream outputFile(out_path);
    ASSERT_TRUE(outputFile.is_open());
    std::string outputData;
    while (std::getline(outputFile, line)) {
        outputData += line;
    }
    outputFile.close();
    // 读取期望文件
    std::ifstream expectFile(out_path);
    ASSERT_TRUE(expectFile.is_open());
    std::string expectData;
    while (std::getline(expectFile, line)) {
        expectData += line;
    }
    outputFile.close();
    // 比较
    EXPECT_EQ(expectData, outputData);

}

TEST(TestGameRelated, SolveSudoku) {
    // 测试bool solve_sudoku(std::vector<std::vector<int>>& board)
    // 待求解数独
    std::vector<std::vector<int>> unsolved_board =
    {
        {2, 8, 0, 0, 1, 6, 7, 4, 5},
        {0, 4, 9, 2, 0, 0, 0, 0, 0},
        {6, 0, 1, 5, 4, 3, 9, 8, 0},
        {0, 9, 0, 0, 2, 5, 0, 1, 3},
        {1, 0, 6, 0, 3, 0, 0, 2, 7},
        {3, 2, 4, 7, 6, 1, 8, 5, 9},
        {0, 6, 7, 0, 0, 8, 2, 3, 4},
        {8, 0, 2, 3, 7, 0, 5, 0, 0},
        {4, 0, 5, 0, 9, 2 ,0, 0, 0},
    };
    // 答案
    std::vector<std::vector<int>> solved_board =
    {
        {2, 8, 3, 9, 1, 6, 7, 4, 5},
        {5, 4, 9, 2, 8, 7, 3, 6, 1},
        {6, 7, 1, 5, 4, 3, 9, 8, 2},
        {7, 9, 8, 4, 2, 5, 6, 1, 3},
        {1, 5, 6, 8, 3, 9, 4, 2, 7},
        {3, 2, 4, 7, 6, 1, 8, 5, 9},
        {9, 6, 7, 1, 5, 8, 2, 3, 4},
        {8, 1, 2, 3, 7, 4, 5, 9, 6},
        {4, 3, 5, 6, 9, 2 ,1, 7, 8},
    };
    // 无解数独
    std::vector<std::vector<int>> nosolution_board =
    {
        {2, 8, 0, 0, 1, 6, 7, 4, 5},
        {0, 4, 9, 2, 0, 0, 0, 0, 0},
        {6, 0, 8, 5, 4, 3, 9, 8, 0},
        {0, 9, 0, 0, 2, 5, 0, 1, 3},
        {1, 0, 6, 0, 3, 0, 0, 2, 7},
        {3, 2, 4, 7, 6, 1, 8, 5, 9},
        {0, 6, 7, 0, 0, 8, 2, 3, 4},
        {8, 0, 2, 3, 7, 0, 5, 0, 0},
        {4, 0, 5, 0, 9, 2 ,0, 0, 0},
    };

    bool ret = solve_sudoku(unsolved_board);
    EXPECT_EQ(ret, true);
    bool eq = true;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (unsolved_board[i][j] != solved_board[i][j]) {
                eq = false;
            }
        }
    }
    EXPECT_EQ(eq, true);
    // 无解数独
    ret = solve_sudoku(nosolution_board);
    EXPECT_EQ(ret, false);
}

TEST(TestPrintParams, AllParams) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = true;
    }
    std::ostringstream oss;
    // 重定向 cout 到输出流
    std::streambuf* coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    params.c = DEFAULT_C;
    strcpy(params.s, DEFAULT_PATH);
    params.n = DEFAULT_N;
    params.m = DEFAULT_M;
    params.rl = DEFAULT_RL;
    params.rr = DEFAULT_RR;
    params.u = DEFAULT_U;
    // 在函数或代码块中执行你要测试的输出
    print_params();

    // 恢复 cout 到原来的缓冲区
    std::cout.rdbuf(coutBuffer);

    // 检查输出是否符合预期
    std::string output = oss.str();
    std::string expected_str = "终盘数量  [1, 1e6]: 20\n待求解棋盘的相对或绝对路径: game.txt\n游戏数量: 50\n游戏难度  [1, 3]: 2\n挖空数量下界  [20, 55]: 20\n挖空数量上界  [20, 55]: 55\n生成唯一解: 1\n";

    EXPECT_EQ(expected_str, output);

}

TEST(TestREADARGS, WrongParams) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 12;
    char* c_args[] = { "**.exe", "-c", "2", "-s", "game.txt", "-n", "1", "-m", "2", "-r", "20~30", "-u" };
    bool res = read_args(n_args, c_args);
    EXPECT_EQ(false, res);
}


TEST(TestREADARGS, RightParams) {
    for (int i = 0; i < 7; i++) {
        has_args[i] = false;
    }
    int n_args = 3;
    char* c_args[] = { "**.exe", "-c", "2" };
    bool res = read_args(n_args, c_args);
    EXPECT_EQ(true, res);
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


TEST(TestGenboard, mode_0) {
    EXPECT_EQ(generate_board(0), false);
}

TEST(TestGenboard, mode_1) {
    params.l = 3;
    EXPECT_EQ(generate_board(1), true);
}

TEST(TestGenboard, mode_2) {
    EXPECT_EQ(generate_board(2), true);
}

TEST(TestGenboard, mode_3) {
    EXPECT_EQ(generate_board(3), true);
}

TEST(TestGenboard, mode_4) {
    EXPECT_EQ(generate_board(4), true);
}

TEST(TestGenboard, mode_invalid) {
    EXPECT_EQ(generate_board(5), false);
}

TEST(TestGameRelated, FindEmptyCell) {
    params.l = 3;
    std::pair<int, int> p = find_empty_cell(test_board);
    EXPECT_EQ(0, p.first);
    EXPECT_EQ(3, p.second);
}

TEST(TestGameRelated, IsValid) {
    bool res = is_valid(test_board, 0, 3, 6);
    EXPECT_EQ(res, false);
    res = is_valid(test_board, 0, 3, 3);
    EXPECT_EQ(res, true);
}

TEST(TestGameRelated, HasSolution) {
    bool res = has_solution(test_board);
    EXPECT_EQ(res, true);
    res = has_solution(no_solution_board);
    EXPECT_EQ(res, false);
}

TEST(TestGameRelated, CheckUnique) {
    int cnt = check_unique(test_board, 0, 0, 0);
    EXPECT_EQ(cnt, 1);
}

TEST(TestGameRelated, DigHole) {
    bool res = dig_hole(filled_board);
    EXPECT_EQ(res, true);
}


//:: testing :: InitGoogleTest（）函数解析Google测试标志的命令行，
//并删除所有已识别的标志。 这允许用户通过各种标志控制测试程序的行为.
// 在调用RUN_ALL_TESTS（）之前必须调用此函数，否则标志将无法正确初始化。
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

