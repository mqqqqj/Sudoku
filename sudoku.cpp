#include <iostream>
#include <fstream>


#include "data.h"
#include "macro.h"

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

    if (!check_args()) {
        return false;
    };

    board = new int*[DEFAULT_SIZE];
    for (int i = 0; i < DEFAULT_SIZE; i++) {
        board[i] = new int[DEFAULT_SIZE];
    }

    return true;
}

bool check_args() {
    return false;
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

int main(int argc, char* argv[]) {
    read_args(argc, argv);
    //write_file();

    return 0;
}