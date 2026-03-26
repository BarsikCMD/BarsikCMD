#include "file.h"
#include "colors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <string>
#include <ncursesw/ncurses.h>
#include <cstring>
#include <locale.h>
#include <wchar.h>

namespace fs = std::filesystem;

// ─── UTF-8 утилиты ────────────────────────────────────────────────────────

// Количество байт в UTF-8 символе по первому байту
static int utf8CharLen(unsigned char c) {
    if (c < 0x80) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

// Количество экранных символов (не байт) в UTF-8 строке
static int utf8DisplayLen(const std::string& s) {
    int len = 0;
    for (size_t i = 0; i < s.size(); ) {
        i += utf8CharLen((unsigned char)s[i]);
        len++;
    }
    return len;
}

// Экранная позиция байтового индекса в строке
static int utf8ByteToDisplay(const std::string& s, int bytePos) {
    int display = 0;
    for (int i = 0; i < bytePos && i < (int)s.size(); ) {
        int cl = utf8CharLen((unsigned char)s[i]);
        i += cl;
        display++;
    }
    return display;
}

// Байтовый индекс по экранной позиции
static int utf8DisplayToByte(const std::string& s, int displayPos) {
    int byte = 0;
    for (int d = 0; d < displayPos && byte < (int)s.size(); d++) {
        byte += utf8CharLen((unsigned char)s[byte]);
    }
    return byte;
}

// ─── Вспомогательные функции ──────────────────────────────────────────────

static void printUsage() {
    std::cout << YELLOW << "Использование: " << BOLD << "file <аргумент> [путь] [...]" << RESET << "\n";
    std::cout << "Доступные аргументы:\n";
    std::cout << "  " << GREEN << BOLD << "new <путь>" << RESET    << "                   — создать новый файл\n";
    std::cout << "  " << GREEN << BOLD << "del <путь>" << RESET    << "                   — удалить файл\n";
    std::cout << "  " << GREEN << BOLD << "set <путь> <текст>" << RESET << "             — задать текст в файле\n";
    std::cout << "  " << GREEN << BOLD << "edit <путь>" << RESET   << "                  — открыть редактор файла\n";
    std::cout << "  " << GREEN << BOLD << "move <путь> <новый-путь>" << RESET << "       — переместить файл\n";
}

// Проверка что файл не защищён (системные пути)
static bool isProtected(const fs::path& p) {
    std::string s = fs::absolute(p).string();
    // Простая проверка системных путей
    static const std::vector<std::string> systemPaths = {
        "/etc", "/bin", "/sbin", "/usr/bin", "/usr/sbin",
        "/boot", "/sys", "/proc", "/dev"
    };
    for (const auto& sp : systemPaths)
        if (s.rfind(sp, 0) == 0) return true;
    return false;
}

// ─── file new ─────────────────────────────────────────────────────────────

static void fileNew(const std::string& path) {
    fs::path p(path);
    if (fs::exists(p)) {
        std::cout << RED << "Ошибка: файл уже существует: " << path << RESET << "\n";
        return;
    }
    // Создаём родительские директории если нужно
    if (p.has_parent_path() && !fs::exists(p.parent_path())) {
        std::error_code ec;
        fs::create_directories(p.parent_path(), ec);
        if (ec) {
            std::cout << RED << "Ошибка: не удалось создать директории: " << ec.message() << RESET << "\n";
            return;
        }
    }
    std::ofstream f(path);
    if (!f) {
        std::cout << RED << "Ошибка: не удалось создать файл: " << path << RESET << "\n";
        return;
    }
    std::cout << GREEN << "Файл создан: " << path << RESET << "\n";
}

// ─── file del ─────────────────────────────────────────────────────────────

static void fileDel(const std::string& path) {
    fs::path p(path);
    if (!fs::exists(p)) {
        std::cout << RED << "Ошибка: файл не найден: " << path << RESET << "\n";
        return;
    }
    if (isProtected(p)) {
        std::cout << RED << "Ошибка: файл защищён и не может быть удалён." << RESET << "\n";
        return;
    }
    std::error_code ec;
    fs::remove(p, ec);
    if (ec) {
        std::cout << RED << "Ошибка при удалении: " << ec.message() << RESET << "\n";
        return;
    }
    std::cout << GREEN << "Файл удалён: " << path << RESET << "\n";
}

// ─── file set ─────────────────────────────────────────────────────────────

static void fileSet(const std::string& path, const std::string& text) {
    // Заменяем ":;" на перевод строки
    std::string content;
    for (size_t i = 0; i < text.size(); ++i) {
        if (i + 1 < text.size() && text[i] == ':' && text[i+1] == ';') {
            content += '\n';
            ++i;
        } else {
            content += text[i];
        }
    }

    std::ofstream f(path);
    if (!f) {
        std::cout << RED << "Ошибка: не удалось открыть файл для записи: " << path << RESET << "\n";
        return;
    }
    f << content;
    std::cout << GREEN << "Текст записан в файл: " << path << RESET << "\n";
}

// ─── file move ────────────────────────────────────────────────────────────

static void fileMove(const std::string& from, const std::string& to) {
    fs::path src(from);
    if (!fs::exists(src)) {
        std::cout << RED << "Ошибка: файл не найден: " << from << RESET << "\n";
        return;
    }
    if (isProtected(src)) {
        std::cout << RED << "Ошибка: файл защищён и не может быть перемещён." << RESET << "\n";
        return;
    }
    std::error_code ec;
    fs::rename(src, fs::path(to), ec);
    if (ec) {
        std::cout << RED << "Ошибка при перемещении: " << ec.message() << RESET << "\n";
        return;
    }
    std::cout << GREEN << "Файл перемещён: " << from << " → " << to << RESET << "\n";
}

// ─── file edit (ncurses TUI) ──────────────────────────────────────────────

struct EditorState {
    std::vector<std::string> lines;
    int cursorRow = 0;
    int cursorCol = 0;    // экранная колонка (не байты!)
    int scrollRow = 0;
    bool modified = false;
    std::string filename;
    std::string statusMsg;

    bool searchMode = false;
    bool replaceMode = false;
    std::string searchQuery;
    std::string replaceQuery;
    int searchResultRow = -1;
    int searchResultCol = -1; // байтовая позиция

    // Текущая байтовая позиция курсора в строке
    int cursorByte() const {
        if (cursorRow >= (int)lines.size()) return 0;
        const std::string& line = lines[cursorRow];
        int byte = utf8DisplayToByte(line, cursorCol);
        // Никогда не выходим за пределы строки
        if (byte > (int)line.size()) byte = (int)line.size();
        return byte;
    }

    // Ограничить cursorCol пределами текущей строки
    void clampCursor() {
        if (cursorRow >= (int)lines.size()) cursorRow = (int)lines.size() - 1;
        if (cursorRow < 0) cursorRow = 0;
        int maxCol = utf8DisplayLen(lines[cursorRow]);
        if (cursorCol > maxCol) cursorCol = maxCol;
        if (cursorCol < 0) cursorCol = 0;
    }
};

// Загрузить файл в lines
static void editorLoad(EditorState& e) {
    e.lines.clear();
    std::ifstream f(e.filename);
    if (!f) {
        e.lines.push_back("");
        return;
    }
    std::string line;
    while (std::getline(f, line)) e.lines.push_back(line);
    if (e.lines.empty()) e.lines.push_back("");
}

// Сохранить файл
static bool editorSave(EditorState& e) {
    std::ofstream f(e.filename);
    if (!f) return false;
    for (size_t i = 0; i < e.lines.size(); ++i) {
        f << e.lines[i];
        if (i + 1 < e.lines.size()) f << '\n';
    }
    e.modified = false;
    return true;
}

// Нарисовать редактор
static void editorDraw(EditorState& e) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int textRows = rows - 2; // строка заголовка + строка статуса

    clear();

    // ── Заголовок ──
    attron(A_REVERSE);
    std::string title = " BarsikCMD File Editor v1  |  " + e.filename;
    if (e.modified) title += " [изменён]";
    while ((int)title.size() < cols) title += ' ';
    mvprintw(0, 0, "%s", title.substr(0, cols).c_str());
    attroff(A_REVERSE);

    // ── Текст ──
    for (int r = 0; r < textRows; ++r) {
        int lineIdx = e.scrollRow + r;
        move(r + 1, 0);
        clrtoeol();
        if (lineIdx < (int)e.lines.size()) {
            // Подсветка найденного
            if (e.searchResultRow == lineIdx && e.searchResultCol >= 0) {
                const std::string& ln = e.lines[lineIdx];
                int qlen = (int)e.searchQuery.size();
                printw("%s", ln.substr(0, e.searchResultCol).c_str());
                attron(A_REVERSE);
                printw("%s", ln.substr(e.searchResultCol, qlen).c_str());
                attroff(A_REVERSE);
                printw("%s", ln.substr(e.searchResultCol + qlen).c_str());
            } else {
                printw("%s", e.lines[lineIdx].c_str());
            }
        }
    }

    // ── Строка статуса ──
    std::string status;
    if (e.searchMode) {
        status = " Поиск: " + e.searchQuery + "  (Enter — найти, Esc — отмена)";
    } else if (e.replaceMode) {
        status = " Замена: " + e.searchQuery + " → " + e.replaceQuery + "  (Enter — заменить, Esc — отмена)";
    } else if (!e.statusMsg.empty()) {
        status = " " + e.statusMsg;
    } else {
        status = " Строка " + std::to_string(e.cursorRow + 1) +
                 ":" + std::to_string(e.cursorCol + 1) +
                 "  |  Ctrl+W — выйти  Ctrl+S — сохранить  Ctrl+F — поиск";
    }
    attron(A_REVERSE);
    while ((int)status.size() < cols) status += ' ';
    mvprintw(rows - 1, 0, "%s", status.substr(0, cols).c_str());
    attroff(A_REVERSE);

    // ── Курсор ──
    int screenRow = e.cursorRow - e.scrollRow + 1;
    int screenCol = utf8ByteToDisplay(
        (e.cursorRow < (int)e.lines.size() ? e.lines[e.cursorRow] : ""),
        e.cursorByte()
    );
    if (screenRow >= 1 && screenRow <= textRows)
        move(screenRow, screenCol);

    refresh();
}

// Поиск вперёд
static bool editorSearch(EditorState& e) {
    if (e.searchQuery.empty()) return false;
    int startRow = e.cursorRow;
    int startCol = e.cursorCol + 1;
    for (int r = startRow; r < (int)e.lines.size(); ++r) {
        size_t pos = e.lines[r].find(e.searchQuery, r == startRow ? startCol : 0);
        if (pos != std::string::npos) {
            e.cursorRow = r;
            e.cursorCol = (int)pos;
            e.searchResultRow = r;
            e.searchResultCol = (int)pos;
            // Скроллим если нужно
            int rows, cols; getmaxyx(stdscr, rows, cols); (void)cols;
            int textRows = rows - 2;
            if (e.cursorRow < e.scrollRow) e.scrollRow = e.cursorRow;
            if (e.cursorRow >= e.scrollRow + textRows) e.scrollRow = e.cursorRow - textRows + 1;
            return true;
        }
    }
    // Не нашли — сбрасываем подсветку
    e.searchResultRow = -1;
    e.searchResultCol = -1;
    return false;
}

static void editorRun(EditorState& e) {
    setlocale(LC_ALL, "");   // UTF-8 локаль
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    std::vector<std::vector<std::string>> undoStack;
    std::vector<std::vector<std::string>> redoStack;

    auto saveUndo = [&]() {
        undoStack.push_back(e.lines);
        redoStack.clear();
    };

    while (true) {
        editorDraw(e);
        int ch = getch();

        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        int textRows = rows - 2;
        (void)cols;

        e.statusMsg = "";

        // ── Режим поиска ──
        if (e.searchMode) {
            if (ch == 27) { // Esc
                e.searchMode = false;
                e.searchResultRow = -1;
                e.searchResultCol = -1;
            } else if (ch == '\n' || ch == KEY_ENTER) {
                if (!editorSearch(e))
                    e.statusMsg = "Не найдено: " + e.searchQuery;
                e.searchMode = false;
            } else if (ch == KEY_BACKSPACE || ch == 127) {
                if (!e.searchQuery.empty()) e.searchQuery.pop_back();
            } else if (ch >= 32 && ch < 256) {
                e.searchQuery += (char)ch;
            }
            continue;
        }

        // ── Режим замены ──
        if (e.replaceMode) {
            if (ch == 27) {
                e.replaceMode = false;
                e.searchResultRow = -1;
                e.searchResultCol = -1;
            } else if (ch == '\n' || ch == KEY_ENTER) {
                // Заменяем все вхождения
                saveUndo();
                int count = 0;
                for (auto& line : e.lines) {
                    size_t pos = 0;
                    while ((pos = line.find(e.searchQuery, pos)) != std::string::npos) {
                        line.replace(pos, e.searchQuery.size(), e.replaceQuery);
                        pos += e.replaceQuery.size();
                        count++;
                    }
                }
                e.modified = true;
                e.replaceMode = false;
                e.searchResultRow = -1;
                e.statusMsg = "Заменено: " + std::to_string(count) + " вхождений";
            } else if (ch == KEY_BACKSPACE || ch == 127) {
                if (!e.replaceQuery.empty()) e.replaceQuery.pop_back();
            } else if (ch >= 32 && ch < 256) {
                e.replaceQuery += (char)ch;
            }
            continue;
        }

        // ── Основной режим ──

        // Ctrl+W — выход
        if (ch == 23) {
            if (e.modified) {
                e.statusMsg = "Есть несохранённые изменения! Нажми Ctrl+W ещё раз для выхода.";
                editorDraw(e);
                int ch2 = getch();
                if (ch2 != 23) continue;
            }
            break;
        }

        // Ctrl+S — сохранить
        if (ch == 19) {
            if (editorSave(e))
                e.statusMsg = "Сохранено: " + e.filename;
            else
                e.statusMsg = "Ошибка сохранения!";
            continue;
        }

        // Ctrl+F — поиск
        if (ch == 6) {
            e.searchMode = true;
            e.searchQuery = "";
            e.searchResultRow = -1;
            e.searchResultCol = -1;
            continue;
        }

        // Ctrl+Alt+F — поиск с заменой (терминалы часто шлют его как ESC+6 или 198)
        if (ch == 198 || ch == 30) {
            e.replaceMode = true;
            e.searchQuery = "";
            e.replaceQuery = "";
            continue;
        }

        // Ctrl+1 — отменить (undo)
        if (ch == '1' - 64 + 64) { // Ctrl+1 обычно не работает, используем F5
        }
        if (ch == KEY_F(5)) { // Undo через F5
            if (!undoStack.empty()) {
                redoStack.push_back(e.lines);
                e.lines = undoStack.back();
                undoStack.pop_back();
                e.modified = true;
                e.statusMsg = "Отменено";
            }
            continue;
        }

        // Ctrl+2 — redo через F6
        if (ch == KEY_F(6)) {
            if (!redoStack.empty()) {
                undoStack.push_back(e.lines);
                e.lines = redoStack.back();
                redoStack.pop_back();
                e.modified = true;
                e.statusMsg = "Повторено";
            }
            continue;
        }

        // ── Навигация ──
        if (ch == KEY_UP) {
            if (e.cursorRow > 0) {
                e.cursorRow--;
                int maxCol = utf8DisplayLen(e.lines[e.cursorRow]);
                e.cursorCol = std::min(e.cursorCol, maxCol);
            }
            e.clampCursor();
        } else if (ch == KEY_DOWN) {
            if (e.cursorRow < (int)e.lines.size() - 1) {
                e.cursorRow++;
                int maxCol = utf8DisplayLen(e.lines[e.cursorRow]);
                e.cursorCol = std::min(e.cursorCol, maxCol);
            }
            e.clampCursor();
        } else if (ch == KEY_LEFT) {
            if (e.cursorCol > 0) {
                e.cursorCol--;
            } else if (e.cursorRow > 0) {
                e.cursorRow--;
                e.cursorCol = utf8DisplayLen(e.lines[e.cursorRow]);
            }
            e.clampCursor();
        } else if (ch == KEY_RIGHT) {
            int maxCol = utf8DisplayLen(e.lines[e.cursorRow]);
            if (e.cursorCol < maxCol) {
                e.cursorCol++;
            } else if (e.cursorRow < (int)e.lines.size() - 1) {
                e.cursorRow++;
                e.cursorCol = 0;
            }
            e.clampCursor();
        } else if (ch == KEY_HOME) {
            e.cursorCol = 0;
        } else if (ch == KEY_END) {
            e.cursorCol = utf8DisplayLen(e.lines[e.cursorRow]);
        } else if (ch == KEY_PPAGE) {
            e.cursorRow = std::max(0, e.cursorRow - textRows);
            e.clampCursor();
        } else if (ch == KEY_NPAGE) {
            e.cursorRow = std::min((int)e.lines.size() - 1, e.cursorRow + textRows);
            e.clampCursor();
        }

        // ── Редактирование ──
        else if (ch == '\n' || ch == KEY_ENTER) {
            saveUndo();
            std::string& cur = e.lines[e.cursorRow];
            int bytePos = e.cursorByte();
            std::string newLine = cur.substr(bytePos);
            cur = cur.substr(0, bytePos);
            e.lines.insert(e.lines.begin() + e.cursorRow + 1, newLine);
            e.cursorRow++;
            e.cursorCol = 0;
            e.modified = true;
            e.clampCursor();
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            saveUndo();
            if (e.cursorCol > 0) {
                std::string& cur = e.lines[e.cursorRow];
                int bytePos = e.cursorByte();
                int prevByte = bytePos - 1;
                while (prevByte > 0 && (((unsigned char)cur[prevByte]) & 0xC0) == 0x80) prevByte--;
                cur.erase(prevByte, bytePos - prevByte);
                e.cursorCol--;
                e.modified = true;
            } else if (e.cursorRow > 0) {
                int prevLen = utf8DisplayLen(e.lines[e.cursorRow - 1]);
                e.lines[e.cursorRow - 1] += e.lines[e.cursorRow];
                e.lines.erase(e.lines.begin() + e.cursorRow);
                e.cursorRow--;
                e.cursorCol = prevLen;
                e.modified = true;
            }
            e.clampCursor();
        } else if (ch == KEY_DC) {
            saveUndo();
            std::string& cur = e.lines[e.cursorRow];
            int bytePos = e.cursorByte();
            if (bytePos < (int)cur.size()) {
                int charLen = utf8CharLen((unsigned char)cur[bytePos]);
                cur.erase(bytePos, charLen);
                e.modified = true;
            } else if (e.cursorRow + 1 < (int)e.lines.size()) {
                cur += e.lines[e.cursorRow + 1];
                e.lines.erase(e.lines.begin() + e.cursorRow + 1);
                e.modified = true;
            }
            e.clampCursor();
        } else if (ch >= 32 && ch < 256) {
            saveUndo();
            std::string& cur = e.lines[e.cursorRow];
            int bytePos = e.cursorByte();
            if (bytePos <= (int)cur.size()) {
                cur.insert(bytePos, 1, (char)ch);
                e.cursorCol++;
                e.modified = true;
            }
            e.clampCursor();
        }

        // ── Скролл ──
        if (e.cursorRow < e.scrollRow) e.scrollRow = e.cursorRow;
        if (e.cursorRow >= e.scrollRow + textRows) e.scrollRow = e.cursorRow - textRows + 1;
    }

    endwin();
}

// ─── Точка входа ─────────────────────────────────────────────────────────

void cmd_file(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        printUsage();
        return;
    }

    const std::string& sub = args[1];

    if (sub == "new") {
        if (args.size() < 3) {
            std::cout << RED << "Укажи путь: file new <путь>" << RESET << "\n";
            return;
        }
        fileNew(args[2]);

    } else if (sub == "del") {
        if (args.size() < 3) {
            std::cout << RED << "Укажи путь: file del <путь>" << RESET << "\n";
            return;
        }
        fileDel(args[2]);

    } else if (sub == "set") {
        if (args.size() < 4) {
            std::cout << RED << "Использование: file set <путь> <текст>" << RESET << "\n";
            std::cout << "Для новой строки используй :; (например: строка1:;строка2)\n";
            return;
        }
        // Собираем весь текст после пути
        std::string text;
        for (size_t i = 3; i < args.size(); ++i) {
            if (i > 3) text += " ";
            text += args[i];
        }
        fileSet(args[2], text);

    } else if (sub == "move") {
        if (args.size() < 4) {
            std::cout << RED << "Использование: file move <путь> <новый-путь>" << RESET << "\n";
            return;
        }
        fileMove(args[2], args[3]);

    } else if (sub == "edit") {
        if (args.size() < 3) {
            std::cout << RED << "Укажи путь: file edit <путь>" << RESET << "\n";
            return;
        }
        EditorState e;
        e.filename = args[2];
        editorLoad(e);
        editorRun(e);

    } else {
        std::cout << RED << "Неизвестный аргумент: " << sub << RESET << "\n";
        printUsage();
    }
}