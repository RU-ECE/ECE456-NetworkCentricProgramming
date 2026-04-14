#pragma once

#include <string>
#include <vector>

#include "TextBuffer.hpp"

using namespace std;

class Editor {
	TextBuffer buffer;
	Mark cursor;
	Mark view;

	using Action = void (Editor::*)();
	vector<Action> bindings;

	bool running = true;
	string filename;

public:
	explicit Editor(const string& file = "");

	void bind(int key, Action fn);
	void unbind(int key);
	void run();

private:
	void adjust_view();
	void draw();
	void move_up();
	void move_down();
	void move_left();
	void move_right();
	void delete_at_cursor();
	void backspace();
	void kill_line();
	void newline();
	void print();
	void quit();
	void save_file_interactive();
	void save_file(const string& path) const;
};
