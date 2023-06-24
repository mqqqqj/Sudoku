
#include "data.h"
#include "macro.h"
#include "function.h"


std::random_device rd;
std::mt19937 gen(rd());

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
        int max = params.l * params.l;
        std::uniform_int_distribution<> dis(1, max); // 假设生成 1 到 max 之间的随机数

        int length = params.l * params.l;
        // 为数组赋随机值
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < length; j++) {
                board[i][j] = dis(gen);
            }
        }
        return true;
    }
    else if (mod == 2) {
        // 记得先初始化为0，否则无法填充
        int length = params.l * params.l;
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < length; j++) {
                board[i][j] = 0;
            }
        }
        solve_sudoku();
    }
    else
        return false;
}

char get_num(int num) {
    if (num == 0)
        return '$';
    else if (num > 0 && num < 10)
        return num + '0';
    else if (num > 9 && num < 17)
        return num - 10 + 'A';
    else {
        std::cout <<"【" << num<<"】";
        return 'X';
    }
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
            std::cout << get_num(board[r][c]) << " ";
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


// 检查某个数字是否能填入指定的行、列和小九宫格
bool is_valid(int row, int col, int num) {
    int block = params.l;
    int length = params.l * params.l;

    // 检查行和列
    for (int i = 0; i < length; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }

    // 检查宫，首先获取当前宫的起始x、y
    int startRow = (row / block) * block;
    int startCol = (col / block) * block;

    for (int i = 0; i < block; i++) {
        for (int j = 0; j < block; j++) {
            if (board[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }

    return true;
}


// 找到下一个空单元格
std::pair<int, int> find_empty_cell() {
    int length = params.l * params.l;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (board[i][j] == 0) {
                return std::make_pair(i, j);
            }
        }
    }

    return std::make_pair(-1, -1); // 返回-1表示找不到空单元格
}


// 使用回溯算法填充数独棋盘
bool solve_sudoku() {
    // 找到下一个空单元格
    std::pair<int, int> cell = find_empty_cell();
    int row = cell.first;
    int col = cell.second;

    // 如果没有空单元格，即填充完所有的单元格
    if (row == -1 && col == -1) {
        //Log("There is no empty blanks!", 2);
        return true; // 返回true表示找到一个可行解
    }

    // 生成随机数字序列 1 到 9
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::shuffle(numbers.begin(), numbers.end(), gen);

    for (int num : numbers) {
        if (is_valid(row, col, num)) {
            board[row][col] = num;

            // 递归填充下一个单元格
            if (solve_sudoku()) {
                return true;
            }

            // 如果无法找到解，重置当前单元格的值
            board[row][col] = 0;
        }
    }

    // 无解
    return false;
}


int main(int argc, char* argv[]) {
    if (!read_args(argc, argv)) {
        std::cout << "读取参数错误，请重新执行！" << std::endl;
        return -1;
    };
    //write_file();

    generate_board(2);
    draw_board();
    return 0;
}