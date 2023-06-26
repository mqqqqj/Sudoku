#include "function.h"
#include "data.h"
#include <windows.h>

void Log(const char* buf, int type) {
	switch (type) {
	case 0:break;
	case 1:std::cout << "[LOG]:"; break;
	case 2:std::cout << "[WARN]:"; break;
	case 3:std::cout << "[ERROR]:"; break;
	default:return;
	}
	std::cout << buf << std::endl;
}

void draw_board(const std::vector<std::vector<int>>& board) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 字体颜色为红色
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);

    int block = params.l;
    int length = params.l * params.l;
    int char_length = 2 * (1 + block + length) - 1;
    for (int i = 0; i < char_length; i++) {
        std::cout << "*";
    }
    std::cout << std::endl;

    for (int i = 0; i < char_length; i++) {
        // i % (length -1)：每个宫需要length-2个横线，所以每隔length-1个横线就需要添加一个加号
        if (i == 0 || i % (2 * block + 2) == 0)
            std::cout << "+";
        else
            std::cout << "-";
    }
    std::cout << std::endl;


    for (int r = 0; r < length; r++) {

        std::cout << "| ";
        for (int c = 0; c < length; c++) {
            std::cout << get_num(board[r][c]) << " ";
            if ((c + 1) % block == 0)std::cout << "| ";
        }
        std::cout << std::endl;

        // 打印横线
        if ((r + 1) % block == 0) {
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