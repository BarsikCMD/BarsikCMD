#pragma once
#include <string>
#include <vector>

// Инициализация readline с цветной подсветкой
void input_init();

// Считать строку с цветной подсветкой. Возвращает false при EOF.
bool input_readline(const std::string& prompt, std::string& out);

// Добавить строку в историю readline
void input_add_history(const std::string& line);

// Таблица цветов для команд (hex -> ANSI)
struct CommandColor {
    std::string command;
    std::string ansi_color; // например "\033[38;2;245;122;7m"
};

extern const std::vector<CommandColor> COMMAND_COLORS;