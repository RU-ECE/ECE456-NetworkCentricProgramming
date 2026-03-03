#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

using namespace std;

struct Mark {
    size_t line = 0;
    size_t col  = 0;
};

enum class ChangeType : uint8_t {
    InsertChar      = 1,
    DeleteChar      = 2,
    InsertNewline   = 3,
    DeleteToEol     = 4
};

struct Change {
    ChangeType type;
    uint32_t line;
    uint32_t col;
    string payload;   // used for InsertChar
};

class TextBuffer {
public:
    vector<string> lines;

    TextBuffer() { lines.push_back(""); }

    void apply(const Change& c) {
        if (c.line >= lines.size()) return;

        switch (c.type) {
        case ChangeType::InsertChar:
            lines[c.line].insert(c.col, c.payload);
            break;

        case ChangeType::DeleteChar:
            if (c.col < lines[c.line].size())
                lines[c.line].erase(c.col, 1);
            break;

        case ChangeType::InsertNewline: {
            string tail = lines[c.line].substr(c.col);
            lines[c.line].erase(c.col);
            lines.insert(lines.begin() + c.line + 1, tail);
            break;
        }

        case ChangeType::DeleteToEol:
            lines[c.line].erase(c.col);
            break;
        }
    }

    void load(const string& path) {
        ifstream f(path);
        lines.clear();
        string line;
        while (getline(f, line)) lines.push_back(line);
        if (lines.empty()) lines.push_back("");
    }

    bool save(const string& path) {
        ofstream f(path);
        if (!f.is_open()) return false;
        for (size_t i = 0; i < lines.size(); ++i) {
            if (i) f << '\n';
            f << lines[i];
        }
        return true;
    }

    void insert_char(const Mark& m, char ch) {
        apply(Change{ChangeType::InsertChar, (uint32_t)m.line, (uint32_t)m.col, string(1, ch)});
    }

    void delete_char_at(const Mark& m) {
        if (m.line < lines.size() && m.col < lines[m.line].size())
            apply(Change{ChangeType::DeleteChar, (uint32_t)m.line, (uint32_t)m.col, ""});
    }

    void delete_char_before(const Mark& m) {
        if (m.col > 0 && m.line < lines.size())
            apply(Change{ChangeType::DeleteChar, (uint32_t)m.line, (uint32_t)(m.col - 1), ""});
        else if (m.line > 0 && !lines.empty()) {
            lines[m.line - 1] += lines[m.line];
            lines.erase(lines.begin() + (ptrdiff_t)m.line);
        }
    }

    void delete_to_eol(const Mark& m) {
        if (m.line < lines.size())
            apply(Change{ChangeType::DeleteToEol, (uint32_t)m.line, (uint32_t)m.col, ""});
    }

    void insert_newline(const Mark& m) {
        if (m.line < lines.size())
            apply(Change{ChangeType::InsertNewline, (uint32_t)m.line, (uint32_t)m.col, ""});
    }
};