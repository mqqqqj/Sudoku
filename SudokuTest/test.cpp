#include "pch.h"
#include "../function.h"
#include "../data.h"
#include "../macro.h"
#include "../debug.cpp"
#include "../sudoku.cpp"

using namespace sudoku;

/*

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


*/


/*
* ZYR
*/

TEST(TESTDEBUG, LOG) {
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


TEST(TESTDEBUG, GETNUM) {
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

TEST(TESTDEBUG, DRAWBOARD) {
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


TEST(TESTIO, WRITEBOARD) {
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

TEST(TESTIO, READBOARD) {
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

TEST(TESTIO, CLEARFILE) {
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

TEST(TESTIO, READSOLVE) {
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


//:: testing :: InitGoogleTest（）函数解析Google测试标志的命令行，
//并删除所有已识别的标志。 这允许用户通过各种标志控制测试程序的行为.
// 在调用RUN_ALL_TESTS（）之前必须调用此函数，否则标志将无法正确初始化。
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
