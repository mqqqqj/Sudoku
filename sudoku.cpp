
#include "data.h"
#include "macro.h"
#include "function.h"


namespace sudoku {

    char buf[50];                                    // DEBUG
    std::vector<std::vector<int>> board;             // 棋盘终局
    std::vector<std::vector<int>> board_unsolved;    // 棋盘，待求解
    Param params;                                    // 参数数值
    bool has_args[7];                                // 参数存在
    // 随机数相关
    std::random_device rd;
    std::mt19937 gen(rd());

    bool read_args(int argc, char* argv[]) {
        sprintf_s(buf, sizeof(buf), "nums of args: %d", argc);
        Log(buf, 1);

        // DEBUG
        for (int i = 0; i < argc; ++i) {
            sprintf_s(buf, sizeof(buf), "param[%d]: %s", argc, argv[i]);
            Log(buf, 1);
        }
        params.c = DEFAULT_C;
        strcpy_s(params.s, sizeof(DEFAULT_PATH), DEFAULT_PATH);
        params.n = DEFAULT_N;
        params.m = DEFAULT_M;
        params.rl = DEFAULT_RL;
        params.rr = DEFAULT_RR;
        params.u = DEFAULT_U;
        params.l = DEFAULT_SIZE;
        params.r = params.rl;

        for (int i = 1; i < argc; i++)
        {
            // c, s, n, m, r, u
            if (!strcmp(argv[i], "-c")) {
                params.c = atoi(argv[i + 1]);
                has_args[0] = true;
                i++;
            }
            else if (!strcmp(argv[i], "-s")) {
                sprintf_s(params.s, sizeof(argv[i + 1])+1, "%s", argv[i + 1]);
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
                std::stringstream ss(argv[i + 1]);
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
        Log("params used:", 1);
        for (int i = 0; i < 7; ++i) {
            std::cout << has_args[i] << " ";
        }
        Log("", 0);
        std::cout << std::endl;
        for (int i = 0; i < 2; ++i) {//对于-c或者-s,都要求其他全为false
            if (has_args[i]) {
                has_args[i] = false;
                bool result = std::all_of(has_args, has_args + sizeof(has_args), [](bool element) { return !element; });
                has_args[i] = true;
                return result;
            }
        }
        bool result = false;
        if (has_args[3] || has_args[6]) {
            // m和u的前提是有n
            result = has_args[2];
        }
        if (has_args[4] || has_args[5]) {
            // r的前提是rl和rr同时有，且有n
            result = has_args[4] && has_args[5] && has_args[2];
        }
        return result;
        /*if (has_args[2]) {
            // n , n && m, n && rl && rr, n && u这四种情况是合法的,其他均不合法
            LL count = std::count(has_args + 3, has_args + sizeof(has_args), true);
            if (count == 2) {
                return has_args[4] && has_args[5];
            }
            else if (count == 1) {
                return has_args[3] || has_args[6];
            }
            else return count == 0;
        }
        return false;*/
    }

    //将一个数独游戏写到文件中
    void write_board(const char* file_name, std::vector<std::vector<int>> board) {
        // 打开文件并指定路径,模式为追加写入
        std::ofstream file(file_name, std::ios::out | std::ios::app);

        // 检查文件是否成功打开
        if (file.is_open()) {
            // 向文件写入数据：长度、难度、空数量
            file << "$ " << params.l << " " << params.m << " " << params.r << std::endl;
            for (const auto& row : board) {
                for (const auto& element : row) {
                    file << element << " ";
                }
                file << std::endl;
            }
            // 关闭文件
            file.close();
            Log("board written", 1);
        }
        else {
            std::cout << "无法打开文件" << std::endl;
        }
    }
    //从文件中读若干个数独游戏
    bool read_and_solve(const char* in_path, const char* out_path) {
        std::ifstream in_file(in_path);
        if (!in_file.is_open()) {
            Log("File not opened!", 3);
            return false;
        }
        clear_file(out_path);
        bool flag = false;
        std::vector<std::vector<int>> one_board;
        std::string line;
        while (std::getline(in_file, line)) {
            if (line.substr(0, 1) == "$") {
                // 以$开头，读取一个新的board
                if (flag) {
                    solve_sudoku(one_board);
                    write_board(out_path, one_board);
                    draw_board(one_board);
                    //清空原数独
                    for (auto& row : one_board) {
                        row.clear();
                    }
                    one_board.clear();
                    one_board.resize(0);
                }
                flag = true;
                // 读取一些参数：l、m、r
                std::istringstream iss(line);
                std::string tmp;
                iss >> tmp;
                iss >> params.l;
                iss >> params.m;
                iss >> params.r;
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
        solve_sudoku(one_board);
        write_board(out_path, one_board);
        draw_board(one_board);
        in_file.close();
        return true;
    }

    //从文件中读若干个数独游戏
    std::vector<std::vector<std::vector<int>>> read_boards(const char* file_name) {
        std::vector<std::vector<std::vector<int>>> boards;
        std::ifstream file(file_name);
        if (!file.is_open()) {
            Log("File not opened!", 3);
            return boards;
        }
        bool flag = false;
        std::vector<std::vector<int>> one_board;
        std::string line;
        while (std::getline(file, line)) {

            if (line.substr(0, 1) == "$") {
                // 以$开头，读取一个新的board
                if (flag) {
                    std::vector<std::vector<int>> copied_board(one_board);//深拷贝
                    boards.push_back(copied_board);//加入到已读取的数独中
                    //清空原数独
                    for (auto& row : one_board) {
                        row.clear();
                    }
                    one_board.clear();
                    one_board.resize(0);
                }
                flag = true;
                // 读取一些参数：l、m、r
                std::istringstream iss(line);
                std::string tmp;
                iss >> tmp;
                iss >> params.l;
                iss >> params.m;
                iss >> params.r;
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
        std::vector<std::vector<int>> copied_board(one_board);//深拷贝
        boards.push_back(copied_board);//加入到已读取的数独中
        file.close();
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

            sprintf_s(buf, sizeof(buf), "Unique generated! Tried %d times...", loop);
            Log(buf, 2);
            return true;
        }
        else {
            return false;
        }
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
        LL startRow = (LL)(row / block) * (LL)block;
        LL startCol = (LL)(col / block) * (LL)block;

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

        while (hole_cnt <= rr) {
            LL x = dis(gen);
            LL y = dis(gen);
            // 原先不是个空，现在变成空
            if (board[x - 1][y - 1] != 0) {
                board[x - 1][y - 1] = 0;
                hole_cnt++;
            }
            LL z = dis(gen);
            if (hole_cnt >= rl && z < (max / params.l)) {
                break;   // 如果达到了r的最小值要求，则按照俄罗斯轮盘赌规则有一定概率break出去
            }
        }
        sprintf_s(buf,sizeof(buf), "Totally %d hole digged!", hole_cnt);
        params.r = hole_cnt;
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
        if (has_args[0])
            std::cout << "终盘数量  [1, 1e6]: " << params.c << std::endl;
        if (has_args[1])
            std::cout << "待求解棋盘的相对或绝对路径: " << params.s << std::endl;
        if (has_args[2])
            std::cout << "游戏数量: " << params.n << std::endl;
        if (has_args[3])
            std::cout << "游戏难度  [1, 3]: " << params.m << std::endl;
        if (has_args[4])
            std::cout << "挖空数量下界  [20, 55]: " << params.rl << std::endl;
        if (has_args[5])
            std::cout << "挖空数量上界  [20, 55]: " << params.rr << std::endl;
        if (has_args[6])
            std::cout << "生成唯一解: " << params.u << std::endl;
    }

    int add(int x, int y) {
        return x + y;
    }
}
