#include <iostream>
#include <ncurses.h>
#include <stack>
#include <string>
#include <unistd.h>

#define MAX_PATH_LEN 1024

using namespace std;

// 프로그램을 실행한 위치의 디렉토리의 주소를 알아냄
stack<string> currentDir();

// 디렉토리 스택 표시용
string showDir(stack<string> stc);

// 디렉토리 내 파일 표시용
void showFile(WINDOW *win);

int main() {
    // dirLocal : 현재 디렉토리 위치
    // fileIn : 파일 탐색기
    // command : 명령어 및 현재 상태 안내바
    // fileShow : 파일 미리 보기
    WINDOW *dirLocal, *fileIn1, *command, *fileIn2, *fileShow;

    char press;

    // 디렉토리 저장용 스택
    stack<string> dirLocation;

    dirLocation = currentDir();

    initscr();

    if (has_colors() == FALSE) {
        puts("Terminal does not support colors!");
        endwin();
        return 1;
    } else {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
    }

    refresh();

    dirLocal = newwin(1, 100, 0, 0);
    fileIn1 = newwin(30, 50, 1, 0);
    fileIn2 = newwin(30, 50, 1, 50);
    command = newwin(1, 100, 31, 0);

    wborder(fileIn1, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(fileIn2, '|', '|', '-', '-', '+', '+', '+', '+');

    wbkgd(dirLocal, COLOR_PAIR(2));
    wbkgd(fileIn1, COLOR_PAIR(1));
    wbkgd(fileIn2, COLOR_PAIR(1));
    wbkgd(command, COLOR_PAIR(2));

    wprintw(dirLocal, showDir(dirLocation).c_str());
    showFile(fileIn1);
    showFile(fileIn2);
    wprintw(command, "F1: Copy F2: Move F3: Delete F4: Rename F5: New File F6: "
                     "New Directory");

    wrefresh(dirLocal);
    wrefresh(fileIn1);
    wrefresh(fileIn2);
    wrefresh(command);

    while (true) {
        press = getchar();
        if (press == 'q') {
            break;
        } else if (press == 'r') {
            clear();
            fileShow = newwin(0, 100, 0, 0);
            wborder(fileShow, '|', '|', '-', '-', '+', '+', '+', '+');
            wbkgd(fileShow, COLOR_PAIR(1));
            mvwprintw(fileShow, 1, 1, "Hello, world!");
            wrefresh(fileShow);
        } else {
            continue;
        }
    }

    endwin();

    return 0;
}

void showFile(WINDOW *win) {
    mvwprintw(win, 1, 1, ".");
    mvwprintw(win, 2, 1, "..");
    mvwprintw(win, 3, 1, "dev");
    mvwprintw(win, 4, 1, "programming");
    mvwprintw(win, 5, 1, "test.txt");
    mvwprintw(win, 6, 1, "text.txt");
    mvwprintw(win, 7, 1, "project.c");
}

stack<string> currentDir() {
    stack<string> stc;

    char dirChar[MAX_PATH_LEN + 1] = {
        '\0',
    };

    if (getcwd(dirChar, MAX_PATH_LEN) == NULL) {
        perror("getcwd error!");
        exit(-1);
    }

    string dirStr(dirChar);

    while (true) {
        int cursor = 0;

        if (dirStr.empty())
            break;

        cursor = dirStr.find("/", 1);
        cout << cursor << endl;

        stc.push(dirStr.substr(1, cursor - 1));
        stc.top();

        dirStr.erase(0, cursor);
        cout << dirStr << endl;
    }

    return stc;
}

string showDir(stack<string> stc) {
    stack<string> tmpStr = stc;
    string dirStr;

    while (true) {
        if (tmpStr.empty())
            break;

        dirStr.insert(0, tmpStr.top());
        dirStr.insert(0, "/");
        tmpStr.pop();
    }

    return dirStr;
}
