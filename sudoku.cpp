
#include <windows.h>
#include <algorithm>

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
    params.c = DEFAULT_C;
    strcpy(params.s, DEFAULT_PATH);
    params.n = DEFAULT_N;
    params.m = DEFAULT_M;
    params.rl = DEFAULT_RL;
    params.rr = DEFAULT_RR;
    params.u = DEFAULT_U;
    params.l = DEFAULT_SIZE;

    for (int i = 1; i < argc; i++)
    {
        // c, s, n, m, r, u
        if (!strcmp(argv[i], "-c")) {
            params.c = atoi(argv[i + 1]);
            has_args[0] = true;
            i++;
        }
        else if (!strcmp(argv[i], "-s")) {
            sprintf(params.s, "%s", argv[i + 1]);
            has_args[1] = true;
            i++;
        }
        else if (!strcmp(argv[i], "-n")) {
            params.n = atoi(argv[i + 1]);
            has_args[2] = true;
            i++;
        }
        else if (!strcmp(argv[i], "-m")) {
            params.m = atoi(argv[i + 1]);
            has_args[3] = true;
            i++;
        }
        else if (!strcmp(argv[i], "-r")) {
            std::stringstream ss(argv[i+1]);
            std::string token;
            getline(ss, token, '~');
            params.rl = std::stoi(token);
            getline(ss, token, '~');
            params.rr = std::stoi(token);
            has_args[4] = true;
            has_args[5] = true;
            i++;
        }
        else if (!strcmp(argv[i], "-u")) {
            params.u = true;
            has_args[6] = true;
        }
    }
    

    if (!check_args()) {
        return false;
    };

    int n = params.l * params.l;
    board.resize(n);
    board_unsolved.resize(n);
    for (int i = 0; i < n; i++) {
        board[i].resize(n);
        board_unsolved[i].resize(n);
    }

    return true;
}

bool check_args() {
    // TODO，检查参数搭配使用是否正确,参数赋默认值在read_args函数中一开始就应完成.
    for (int i = 0; i < 7; ++i) {
        std::cout << has_args[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 2; ++i) {//对于-c或者-s,都要求其他全为false
        if (has_args[i]) {
            has_args[i] = false;
            bool result = std::all_of(has_args, has_args + sizeof(has_args), [](bool element) { return !element; });
            has_args[i] = true;
            return result;
        }
    }
    if (has_args[2]) {
        // n , n && m, n && rl && rr, n && u这四种情况是合法的,其他均不合法
        int count = std::count(has_args + 3, has_args + sizeof(has_args), true);
        if (count == 2) {
            return has_args[4] && has_args[5];
        }
        else if (count == 1) {
            return has_args[3] || has_args[6];
        }
        else return count == 0;
    }
    return false;
}

//将一个数独游戏写到文件中
void write_file(const char* file_name, std::vector<std::vector<int>> board) {
    // 打开文件并指定路径,模式为追加写入
    std::ofstream file(file_name, std::ios::out | std::ios::app);

    // 检查文件是否成功打开
    if (file.is_open()) {
        // 向文件写入数据
        for (const auto& row : board) {
            for (const auto& element : row) {
                file << element << " ";
            }
            file << std::endl;
        }
        file << "-------END-------" << std::endl;;
        // 关闭文件
        file.close();
        std::cout << "文件写入成功" << std::endl;
    }
    else {
        std::cout << "无法打开文件" << std::endl;
    }
}

//从文件中读若干个数独游戏
std::vector<std::vector<std::vector<int>>> read_file(const char* file_name) {
    std::vector<std::vector<std::vector<int>>> boards;
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::vector<std::vector<int>> one_board;
        std::string line;
        while (std::getline(file, line)) {
            if (line ==  "-------END-------") {
                //读取一个新的数独
                std::vector<std::vector<int>> copied_board(one_board);//深拷贝
                boards.push_back(copied_board);//加入到已读取的数独中
                for (auto& row : one_board) {//清空原数独
                    row.clear();
                }
                one_board.clear();
                one_board.resize(0);
                continue;
            }
            std::vector<int> row;
            std::istringstream iss(line);
            int num;

            while (iss >> num) {
                row.push_back(num);
            }

            one_board.push_back(row);
        }
        file.close();
        std::cout << "文件读取成功" << std::endl;
    }
    else {
        std::cout << "无法打开文件" << std::endl;
    }
    return boards;
}

void clear_file(const char* file_name) {
    std::ofstream file(file_name, std::ios::trunc);
    if (file.is_open()) {
        file.close();
        std::cout << "文件已被清空。" << std::endl;
    }
    else {
        std::cout << "无法打开文件。" << std::endl;
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
        board_unsolved = board;
        return true;
    }
    else if (mod == 2) {
        // 直接填充
        solve_sudoku(board);
        board_unsolved = board;
        return true;
    }
    else if (mod == 3) {
        // 先填好
        solve_sudoku(board);
        // 再copy一份，用来挖空
        board_unsolved = board;
        // 挖空(一般都能true，否则设置的r不合理)
        return dig_hole(board_unsolved);
    }
    else if (mod == 4) {
        // 先填好
        solve_sudoku(board);
        // 再copy一份，用来挖空
        int loop = 0;
        do {
            board_unsolved = board;    // 一定要在循环内copy一份，否则最后都是0了
            loop++;
            // 挖空，若不是唯一解就继续挖
            if (!dig_hole(board_unsolved)) {
                return false;
            }
        } while (check_unique(board_unsolved, 0, 0, 0) != 1);

        sprintf(buf, "Unique generated! Tried %d times...", loop);
        Log(buf, 2);
    }
    else {
        return false;
    }
}

char get_num(int num) {
    if (num == 0)
        return '$';
    else if (num > 0 && num < 10)
        return num + '0';
    else if (num > 9 && num < 17)
        return num - 10 + 'A';
    else {
        //std::cout <<"【" << num<<"】";
        return 'X';
    }
}

void draw_board(const std::vector<std::vector<int>>& board) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 字体颜色为红色
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_GREEN);

    int block = params.l;
    int length = params.l * params.l;
    int char_length = 2 * (1 + block + length) - 1;
    for (int i = 0; i < char_length; i++) {
        std::cout << "*";
    }
    std::cout << std::endl;

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
    for (int i = 0; i < char_length; i++) {
        std::cout << "*";
    }
    std::cout << std::endl;
    // 恢复默认的字体颜色
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


// 检查某个数字是否能填入指定的行、列和小九宫格
bool is_valid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
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
std::pair<int, int> find_empty_cell(const std::vector<std::vector<int>>& board) {
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
bool solve_sudoku(std::vector<std::vector<int>>& board) {
    // 找到下一个空单元格
    std::pair<int, int> cell = find_empty_cell(board);
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
        if (is_valid(board, row, col, num)) {
            board[row][col] = num;

            // 递归填充下一个单元格
            if (solve_sudoku(board)) {
                return true;
            }

            // 如果无法找到解，重置当前单元格的值
            board[row][col] = 0;
        }
    }

    // 无解
    return false;
}


// 检查数独是否有解
bool has_solution(const std::vector<std::vector<int>>& board) {
    std::vector<std::vector<int>> board_copy = board;
    return solve_sudoku(board_copy);
}

bool dig_hole(std::vector<std::vector<int>>& board) {
    int max = params.l * params.l;
    std::uniform_int_distribution<> dis(1, max); // 假设生成 1 到 max 之间的随机数，用来选取x和y作为挖空的下标
    int rl = params.rl, rr = params.rr;
    int hole_cnt = 0; // 空计数
    int x, y, z;
    while (hole_cnt <= rr){
        x = dis(gen);
        y = dis(gen);
        // 原先不是个空，现在变成空
        if (board[x - 1][y - 1] != 0) {
            board[x - 1][y - 1] = 0;
            hole_cnt++;
        }
        int z = dis(gen);
        if (hole_cnt >= rl && z < (max / params.l)) {
            break;   // 如果达到了r的最小值要求，则按照俄罗斯轮盘赌规则有一定概率break出去
        }
    }
    sprintf(buf, "Totally %d hole digged!", hole_cnt);
    Log(buf, 2);
    return true;
}

int check_unique(std::vector<std::vector<int>>& board, int x, int y, int count) {
    int size = params.l * params.l;
    int num;
    if (x == size) {
        x = 0;
        if (++y == size) return (1 + count);
    }
    // Skip opened cell.
    if (board[x][y] != 0)  return check_unique(board, x + 1, y, count);
    // skip if the puzzle has more than one solution
    for (num = 1; (num <= size) && (count < 2); num++) {
        if (is_valid(board, x, y, num)) {
            board[x][y] = num;
            // Goto next.
            count = check_unique(board, x + 1, y, count);
        }
    }
    // Reset cell on go back.
    board[x][y] = 0;
    return count;
}

void print_params() {
    if(has_args[0])
        std::cout << "终盘数量  [1, 1e6]: " << params.c << std::endl;
    if(has_args[1])
        std::cout << "待求解棋盘的相对或绝对路径: " << params.s << std::endl;
    if(has_args[2])
        std::cout << "游戏数量: " << params.n << std::endl;
    if(has_args[3])
        std::cout << "游戏难度  [1, 3]: " << params.m << std::endl;
    if(has_args[4])
        std::cout << "挖空数量下界  [20, 55]: " << params.rl << std::endl;
    if(has_args[5])
        std::cout << "挖空数量上界  [20, 55]: " << params.rr << std::endl;
    if(has_args[6])
        std::cout << "生成唯一解: " << params.u << std::endl;
}

int main(int argc, char* argv[]) {

    if (!read_args(argc, argv)) {
        std::cout << "读取参数错误，请重新执行！" << std::endl;
        system("pause");
        return -1;
    };
    //write_file();
    print_params();
    system("pause");

    if (has_args[0]) {
        //生成若干个数独终盘
        for (int i = 0; i < params.c; ++i) {
            generate_board(2);
            draw_board(board);
            for (int i = 0; i < board.size(); ++i) {
                std::fill(board_unsolved[i].begin(), board_unsolved[i].end(), 0);
                std::fill(board[i].begin(), board[i].end(), 0);
            }
        }
    }
    else if (has_args[1]) {
        //从文件中读取若干数独游戏,求解并输出到指定sudoku.txt文件
        clear_file("sudoku.txt");
        std::vector<std::vector<std::vector<int>>> boards = read_file(params.s);
        for (int i = 0; i < boards.size(); ++i) {
            draw_board(boards[i]);
            board_unsolved = boards[i];
            solve_sudoku(board_unsolved);
            write_file("sudoku.txt", board_unsolved);
        }
    }
    else if (has_args[2]) {
        //生成指定难度的数独游戏
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
            for (int i = 0; i < params.n; ++i) {
                generate_board(3);
                Log("New game:", 1);
                draw_board(board_unsolved);
                std::cout << std::endl;
                // 没保存到任何地方,只是输出到屏幕上
            }
        }
        else if (has_args[4] && has_args[5]) {
            //生成给定挖空范围的数独游戏
            for (int i = 0; i < params.n; ++i) {
                generate_board(3);
                Log("New game:", 1);
                draw_board(board_unsolved);
                std::cout << std::endl;
                // 没保存到任何地方,只是输出到屏幕上
            }
        }
        else if (has_args[6]) {
            //生成具有唯一解的数独游戏
            for (int i = 0; i < params.n; ++i) {
                generate_board(4);
                Log("New game:", 1);
                draw_board(board_unsolved);
                //Log("Standard Answer:", 1);
                //draw_board(board);
                //solve_sudoku(board_unsolved);
                //Log("Solved Answer:", 1);
                //draw_board(board_unsolved);
                std::cout << std::endl;
                // 没保存到任何地方,只是输出到屏幕上
            }
        }
        else {
            //生成若干个数独游戏,使用默认参数
            for (int i = 0; i < params.n; ++i) {
                generate_board(3);
                Log("New game:", 1);
                draw_board(board_unsolved);
                std::cout << std::endl;
                // 没保存到任何地方,只是输出到屏幕上
            }
        }
    }

    system("pause");
    return 0;
}