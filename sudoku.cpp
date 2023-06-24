#include <iostream>
#include <fstream>
#include <random>

#include "data.h"
#include "macro.h"
#include "function.h"


bool read_args(int argc, char* argv[]) {
    std::cout << "nums of args:" << argc << std::endl;

    // DEBUG
    for (int i = 0; i < argc; ++i) {
        std::cout << "param[" << i << "]: " << argv[i] << std::endl;
    }

    for (int i = 1; i < argc; i++)
    {
        // c, s, n, m, r, u
        if (!strcmp(argv[i], "-c"))   
            params.c = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-s")) 
            sprintf(params.s, "%s", argv[i + 1]);
        else if (!strcmp(argv[i], "-n"))
            params.n = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-m")) 
            params.m = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-r"))
            params.r = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-u"))
            params.u = true;
    }

    params.l = DEFAULT_SIZE;

    if (!check_args()) {
        return false;
    };

    int n = params.l * params.l;
    board = new int*[n];
    for (int i = 0; i < n; i++) {
        board[i] = new int[n];
    }

    return true;
}

bool check_args() {
    return true;
}

// TODO
void write_file() {
    // 打开文件并指定路径
    std::ofstream file("output.txt");

    // 检查文件是否成功打开
    if (file.is_open()) {
        // 向文件写入数据
        file << "这是要写入文件的内容" << std::endl;
        file << "另一行数据" << std::endl;

        // 关闭文件
        file.close();
        std::cout << "文件写入成功" << std::endl;
    }
    else {
        std::cout << "无法打开文件" << std::endl;
    }
}

bool generate_board(int mod) {
    if (mod == 0) {
        return false;
    }
    // 随机棋盘
    else if (mod == 1) {
        // 创建随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 9); // 假设生成 1 到 100 之间的随机数

        int length = params.l * params.l;
        // 为数组赋随机值
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < length; j++) {
                board[i][j] = dis(gen);
            }
        }
        return true;
    }
    else
        return false;
}

void draw_board() {
    int block = params.l;
    int length = params.l * params.l;
    int char_length = 2 * (1 + block + length) - 1;

    for (int i = 0; i < char_length; i++) {
        // i % (length -1)：每个宫需要length-2个横线，所以每隔length-1个横线就需要添加一个加号
        if (i == 0 || i % ( 2 * block + 2)==0) 
            std::cout << "+";
        else
            std::cout << "-";
    }
    std::cout << std::endl;


    for (int r = 0; r < length; r++) {

        std::cout << "| ";
        for (int c = 0; c < length; c++) {
            if(board[r][c]==0)
                std::cout << "$";
            else
                std::cout << board[r][c];
            std::cout << " ";
            if ((c+1)%block==0)std::cout << "| ";
        }
        std::cout << std::endl;

        // 打印横线
        if ((r+1) % block == 0) {
            for (int i = 0; i < char_length; i++) {
                if (i == 0 || i % (2 * block + 2) == 0)
                    std::cout << "+";
                else
                    std::cout << "-";
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (!read_args(argc, argv)) {
        std::cout << "读取参数错误，请重新执行！" << std::endl;
        return -1;
    };
    //write_file();

    generate_board(1);
    draw_board();
    return 0;
}