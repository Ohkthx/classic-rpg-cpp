#ifndef _COLOR_H
#define _COLOR_H

#include <string>
#include <vector>

namespace color {

enum class Style {
  DEFAULT = 0,
  BOLD = 1,
  DIM = 2,
  ITALIC = 3,
  UNDERLINE = 4,
  BLINK = 5,
  REVERSE = 7,
  HIDDEN = 8,
  STRIKE = 9,
};

enum class Foreground {
  DEFAULT = 39,
  BLACK = 30,
  RED = 31,
  GREEN = 32,
  YELLOW = 33,
  BLUE = 34,
  MAGENTA = 35,
  CYAN = 36,
  WHITE = 37,
  BRIGHT_BLACK = 90,
  BRIGHT_RED = 91,
  BRIGHT_GREEN = 92,
  BRIGHT_YELLOW = 93,
  BRIGHT_BLUE = 94,
  BRIGHT_MAGENTA = 95,
  BRIGHT_CYAN = 96,
  BRIGHT_WHITE = 97
};

enum class Background {
  DEFAULT = 49,
  BLACK = 40,
  RED = 41,
  GREEN = 42,
  YELLOW = 43,
  BLUE = 44,
  MAGENTA = 45,
  CYAN = 46,
  WHITE = 47,
  BRIGHT_BLACK = 100,
  BRIGHT_RED = 101,
  BRIGHT_GREEN = 102,
  BRIGHT_YELLOW = 103,
  BRIGHT_BLUE = 104,
  BRIGHT_MAGENTA = 105,
  BRIGHT_CYAN = 106,
  BRIGHT_WHITE = 107
};

std::string stylize(std::string sym, Style style, Foreground fg, Background bg);
std::string stylize(std::string sym, Foreground fg, Background bg);
std::string stylize(std::string sym, Style style, Foreground fg);
std::string stylize(std::string sym, Foreground fg);
std::string stylize(std::string sym, const std::vector<Style> &styles,
                    Foreground fg, Background bg);

} // namespace color
#endif