// editor.cpp
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include "TextBuffer.hpp"
#include "Editor.hpp"

int main(int argc, char** argv) {
    string file;
    if (argc > 1) file = argv[1];
    Editor ed(file);
    ed.run();
    return 0;
}