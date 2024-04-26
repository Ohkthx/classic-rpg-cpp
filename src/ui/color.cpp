#include "color.hpp"
#include <sstream>

template <typename Enum> std::string ansiToString(Enum code) {
  return std::to_string(static_cast<int>(code));
}

std::string color::stylize(std::string sym, Style style, Foreground fg,
                           Background bg) {
  return "\033[" + ansiToString(style) + ';' + ansiToString(fg) + ';' +
         ansiToString(bg) + "m" + sym + "\033[0m";
}

std::string color::stylize(std::string sym, Foreground fg, Background bg) {
  return "\033[" + ansiToString(fg) + ';' + ansiToString(bg) + "m" + sym +
         "\033[0m";
}

std::string color::stylize(std::string sym, Style style, Foreground fg) {
  return "\033[" + ansiToString(style) + ';' + ansiToString(fg) + "m" + sym +
         "\033[0m";
}

std::string color::stylize(std::string sym, Foreground fg) {
  return "\033[" + ansiToString(fg) + "m" + sym + "\033[0m";
}

std::string color::stylize(std::string sym, const std::vector<Style> &styles,
                           Foreground fg, Background bg) {
  std::ostringstream builder;
  builder << "\033[";

  for (size_t i = 0; i < styles.size(); ++i) {
    builder << ansiToString(styles[i]);
    if (i < styles.size() - 1) {
      builder << ';';
    }
  }

  if (!styles.empty()) {
    builder << ';';
  }

  builder << ansiToString(fg) << ';' << ansiToString(bg) << "m" << sym
          << "\033[0m";

  return builder.str();
}