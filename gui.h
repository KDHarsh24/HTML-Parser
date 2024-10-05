#include<bits/stdc++.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
using namespace std;
int getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col; // Number of columns
}

void printFullWidth(const std::string& text) {
    int termWidth = getTerminalWidth();
    int textLen = text.length();

    if (textLen < termWidth) {
        // If the text is shorter than the terminal width, pad with spaces
        int padding = (termWidth - textLen) / 2; // Centering the text
        cout << string(padding, ' ') << text << endl;
    } else {
        // If the text is longer, just print it
        cout << text.substr(0, termWidth) << endl;
    }
}




//DialogueBox Maker
int selectDialogueBox(vector<string> options)
{
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    clear();
    mvprintw(0, 0, "Select an option:");
    // Print the selection prompt to the console
    refresh();

    // Create a window for the options
    WINDOW *win = newwin(options.size() + 2, 30, 2, 2);
    box(win, 0, 0);
    
    // Print the options
    for (int i = 0; i < options.size(); i++) {
        mvwprintw(win, i + 1, 2, "%s", options[i].c_str());
    }

    // Create a cursor for selection
    int cursor = 0;
    wrefresh(win);
    int c;
    do {
        for (int i = 0; i < options.size(); i++) {
            mvwprintw(win, i + 1, 1, " ");
            mvwprintw(win, i + 1, 2, "%s", options[i].c_str());
        }
        mvwprintw(win, cursor + 1, 1, ">");
        mvwprintw(win, cursor + 1, 2, "%s", options[cursor].c_str());
        wrefresh(win);
        c = getch();
        switch (c) {
            case KEY_UP:
                if (cursor > 0) {
                    cursor--;
                }
                break;
            case KEY_DOWN:
                if (cursor < options.size() - 1) {
                    cursor++;
                }
                break;
        }
    } while (c != '\n');

    // Clean up
    delwin(win);
    endwin();

    return cursor;
}
