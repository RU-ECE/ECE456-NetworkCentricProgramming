#include "Editor.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "NcursesCompat.hpp"

using namespace std;

Editor::Editor(const string& file) : bindings(KEY_MAX + 1, nullptr), filename(file) {
	if (!filename.empty())
		buffer.load(filename);

	bind(KEY_UP, &Editor::move_up);
	bind(KEY_DOWN, &Editor::move_down);
	bind(KEY_LEFT, &Editor::move_left);
	bind(KEY_RIGHT, &Editor::move_right);
	bind(KEY_DC, &Editor::delete_at_cursor);
	bind(KEY_BACKSPACE, &Editor::backspace);
	bind(KEY_ENTER, &Editor::newline);
	bind(127, &Editor::backspace);
	bind(11, &Editor::kill_line); // Ctrl-K
	bind('\n', &Editor::newline);
	bind(16, &Editor::print); // Ctrl-P
	bind(17, &Editor::quit); // Ctrl-Q
	bind(19, &Editor::save_file_interactive); // Ctrl-S
}

void Editor::bind(const int key, const Action fn) {
	if (key >= 0 && key < static_cast<int>(bindings.size()))
		bindings[key] = fn;
}

void Editor::unbind(const int key) {
	if (key >= 0 && key < static_cast<int>(bindings.size()))
		bindings[key] = nullptr;
}

void Editor::run() {
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	while (running) {
		adjust_view();
		draw();
		const int ch = wgetch(stdscr);

		if (ch >= 0 && ch < static_cast<int>(bindings.size()) && bindings[ch]) {
			(this->*bindings[ch])();
		} else if (ch >= 0 && ch <= 255 && isprint(static_cast<unsigned char>(ch))) {
			buffer.insert_char(cursor, static_cast<char>(ch));
			cursor.col++;
		}
	}

	endwin();
}

void Editor::adjust_view() {
	int rows = 0;
	int cols = 0;
	getmaxyx(stdscr, rows, cols);
	rows = max(rows, 1);
	cols = max(cols, 1);

	if (cursor.line < view.line)
		view.line = cursor.line;
	if (cursor.line >= view.line + static_cast<size_t>(rows))
		view.line = cursor.line - rows + 1;

	if (cursor.col < view.col)
		view.col = cursor.col;
	if (cursor.col >= view.col + static_cast<size_t>(cols))
		view.col = cursor.col - cols + 1;
}

void Editor::draw() {
	erase();
	int rows = 0;
	int cols = 0;
	getmaxyx(stdscr, rows, cols);

	for (int r = 0; r < rows; ++r) {
		const size_t idx = view.line + static_cast<size_t>(r);
		if (idx >= buffer.lines.size())
			break;

		const string& s = buffer.lines[idx];
		if (view.col < s.size())
			mvaddnstr(r, 0, s.c_str() + view.col, cols);
	}

	wmove(stdscr, static_cast<int>(cursor.line - view.line), static_cast<int>(cursor.col - view.col));
	wrefresh(stdscr);
}

void Editor::move_up() {
	if (cursor.line > 0) {
		cursor.line--;
		cursor.col = min(cursor.col, buffer.lines[cursor.line].size());
	}
}

void Editor::move_down() {
	if (cursor.line + 1 < buffer.lines.size()) {
		cursor.line++;
		cursor.col = min(cursor.col, buffer.lines[cursor.line].size());
	}
}

void Editor::move_left() {
	if (cursor.col > 0) {
		cursor.col--;
	} else if (cursor.line > 0) {
		cursor.line--;
		cursor.col = buffer.lines[cursor.line].size();
	}
}

void Editor::move_right() {
	if (cursor.col < buffer.lines[cursor.line].size()) {
		cursor.col++;
	} else if (cursor.line + 1 < buffer.lines.size()) {
		cursor.line++;
		cursor.col = 0;
	}
}

void Editor::delete_at_cursor() { buffer.delete_char_at(cursor); }

void Editor::backspace() {
	if (cursor.col > 0) {
		buffer.delete_char_before(cursor);
		cursor.col--;
	} else if (cursor.line > 0) {
		const size_t prev_len = buffer.lines[cursor.line - 1].size();
		buffer.delete_char_before(cursor);
		cursor.line--;
		cursor.col = prev_len;
	}
}

void Editor::kill_line() { buffer.delete_to_eol(cursor); }

void Editor::newline() {
	buffer.insert_newline(cursor);
	cursor.line++;
	cursor.col = 0;
}

void Editor::print() {
	def_prog_mode();
	endwin();

	const int pid = fork();
	if (pid == 0) {
		const string text = buffer.get_text();
		system(text.c_str());
		_exit(0);
	}

	if (pid > 0) {
		int status = 0;
		waitpid(pid, &status, 0);
	}

	reset_prog_mode();
	wrefresh(stdscr);
}

void Editor::quit() { running = false; }

void Editor::save_file_interactive() {
	int rows = 0;
	int cols = 0;
	getmaxyx(stdscr, rows, cols);

	wmove(stdscr, rows - 1, 0);
	clrtoeol();

	mvprintw(rows - 1, 0, "Save as: ");
	echo();

	char buf[256];
	getnstr(buf, 255);
	noecho();

	filename = string(buf);
	if (!filename.empty())
		if (buffer.save(filename))
			mvprintw(rows - 1, 0, "Saved to %s", filename.c_str());
		else
			mvprintw(rows - 1, 0, "Failed to save %s", filename.c_str());
	else
		mvprintw(rows - 1, 0, "Save cancelled");

	clrtoeol();
	wmove(stdscr, static_cast<int>(cursor.line - view.line), static_cast<int>(cursor.col - view.col));
	wrefresh(stdscr);
}

void Editor::save_file(const string& path) const { buffer.save(path); }

int main(const int argc, char** argv) {
	string file;
	if (argc > 1)
		file = argv[1];

	Editor ed(file);
	ed.run();
	return 0;
}
