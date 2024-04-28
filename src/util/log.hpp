#ifndef _LOG_HPP
#define _LOG_HPP

#include "../ui/color.hpp"
#include "queue.hpp"
#include <string>
#include <vector>

struct LogEntry {
  color::Foreground color;
  color::Style style;
  std::string text;

  LogEntry(color::Foreground color, color::Style style, std::string text)
      : color(color), style(style), text(text){};

  // Draws the output to terminal.
  void draw() const { draw(text.length()); }
  void draw(int n) const { std::cout << toString(n); }

  // String representation of the log, including the color.
  std::string toString() const { return toString(text.length()); }
  std::string toString(int n) const {
    n = std::min(n, static_cast<int>(text.length()));
    return color::stylize(text.substr(0, n), style, color);
  }
};

class Log {
public:
  explicit Log(size_t length) : data(length) {}

  // Adds a new entry into the log.
  void add(std::string text) { add(color::Foreground::WHITE, text); }
  void add(color::Foreground color, std::string text) {
    add(color, color::Style::DEFAULT, text);
  }

  void add(color::Foreground color, color::Style style, std::string text) {
    data.push(LogEntry(color, style, text));
  }

  std::vector<LogEntry> getText() { return data.toVector(); }

private:
  FixedQueue<LogEntry> data;
};

#endif