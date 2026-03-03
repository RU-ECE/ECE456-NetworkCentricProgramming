#pragma once
#include <string>
#include <vector>
#include <ncurses.h>
#include "TextBuffer.hpp"

class Editor {
    TextBuffer buffer;
    Mark cursor;
    Mark view;

    using Action = void (Editor::*)();
    vector<Action> bindings;   // index = key code, nullptr = unbound

    bool running = true;
    string filename;

public:
    Editor(const string& file = "")
        : bindings(KEY_MAX + 1, nullptr), filename(file)
    {
        if (!filename.empty()) buffer.load(filename);

        bind(KEY_UP,    &Editor::move_up);
        bind(KEY_DOWN,  &Editor::move_down);
        bind(KEY_LEFT,  &Editor::move_left);
        bind(KEY_RIGHT, &Editor::move_right);
        bind(KEY_DC,    &Editor::delete_at_cursor);
        bind(KEY_BACKSPACE, &Editor::backspace);
        bind(127, &Editor::backspace);
        bind(11,  &Editor::kill_line);   // Ctrl-K
        bind('\n', &Editor::newline);
        bind(17,  &Editor::quit);        // Ctrl-Q
        bind(19,  &Editor::save_file_interactive);   // Ctrl-S
    }

    void bind(int key, Action fn) {
        if (key >= 0 && key < (int)bindings.size())
            bindings[key] = fn;
    }

    void unbind(int key) {
        if (key >= 0 && key < (int)bindings.size())
            bindings[key] = nullptr;
    }

    void run() {
        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();

        while (running) {
            adjust_view();
            draw();
            int ch = getch();

            if (ch >= 0 && ch < (int)bindings.size() && bindings[ch]) {
                (this->*bindings[ch])();
            } else if (isprint(ch)) {
                buffer.insert_char(cursor, ch);
                cursor.col++;
            }
        }

        endwin();
    }

private:
    void adjust_view() {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        if (cursor.line < view.line) view.line = cursor.line;
        if (cursor.line >= view.line + (size_t)rows)
            view.line = cursor.line - rows + 1;

        if (cursor.col < view.col) view.col = cursor.col;
        if (cursor.col >= view.col + (size_t)cols)
            view.col = cursor.col - cols + 1;
    }

    void draw() {
        erase();
        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        for (int r = 0; r < rows; ++r) {
            size_t idx = view.line + r;
            if (idx >= buffer.lines.size()) break;
            const string& s = buffer.lines[idx];
            if (view.col < s.size())
                mvaddnstr(r, 0, s.c_str() + view.col, cols);
        }

        move(cursor.line - view.line, cursor.col - view.col);
        refresh();
    }

    void move_up() {
        if (cursor.line > 0) {
            cursor.line--;
            cursor.col = min(cursor.col, buffer.lines[cursor.line].size());
        }
    }

    void move_down() {
        if (cursor.line + 1 < buffer.lines.size()) {
            cursor.line++;
            cursor.col = min(cursor.col, buffer.lines[cursor.line].size());
        }
    }

    void move_left() {
        if (cursor.col > 0) cursor.col--;
        else if (cursor.line > 0) {
            cursor.line--;
            cursor.col = buffer.lines[cursor.line].size();
        }
    }

    void move_right() {
        if (cursor.col < buffer.lines[cursor.line].size()) cursor.col++;
        else if (cursor.line + 1 < buffer.lines.size()) {
            cursor.line++;
            cursor.col = 0;
        }
    }

    void delete_at_cursor() { buffer.delete_char_at(cursor); }
    void backspace()        {
        if (cursor.col > 0) { buffer.delete_char_before(cursor); cursor.col--; }
        else if (cursor.line > 0) {
            size_t prev_len = buffer.lines[cursor.line - 1].size();
            buffer.delete_char_before(cursor);
            cursor.line--;
            cursor.col = prev_len;
        }
    }
    void kill_line()        { buffer.delete_to_eol(cursor); }
    void newline() {
        buffer.insert_newline(cursor);
        // Move cursor to the beginning of the new line
        cursor.line++;
        cursor.col = 0;
    }
    void quit()             { running = false; }
    void save_file_interactive() {
        // Temporarily move cursor to bottom
        int rows, cols;
        getmaxyx(stdscr, rows, cols);
    
        // Clear the bottom line
        move(rows - 1, 0);
        clrtoeol();
    
        // Prompt
        mvprintw(rows - 1, 0, "Save as: ");
        echo();          // enable user input
        char buf[256];   // temporary buffer for filename
        getnstr(buf, 255);
        noecho();        // turn off echo
    
        filename = string(buf);
        if (!filename.empty()) {
            if (buffer.save(filename))
                mvprintw(rows - 1, 0, "Saved to %s", filename.c_str());
            else
                mvprintw(rows - 1, 0, "Failed to save %s", filename.c_str());
        } else {
            mvprintw(rows - 1, 0, "Save cancelled");
        }
    
        clrtoeol();       // clear any leftover input
        move(cursor.line - view.line, cursor.col - view.col); // restore cursor
        refresh();
    }
    void save_file(const string& filename) {
        buffer.save(filename);
    }
};
