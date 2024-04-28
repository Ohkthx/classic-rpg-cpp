#include "camera.hpp"
#include "../map/tile.hpp"
#include "../util/log.hpp"
#include "color.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

void Camera::setViewSize() {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  }
#else
  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
    width = size.ws_col;
    height = size.ws_row;
  }
#endif
}

void Camera::clearView() {
#ifdef _WIN32
  system("cls");
#else
  std::cout << "\033[2J\033[H";
#endif
}

void resetCursor() { std::cout << "\033[0;0H"; }

int Camera::getMapWidth(std::size_t line_count) {
  return line_count == 0 ? width : static_cast<int>(width * (1.0 - RHS_SPACE));
}

int Camera::getMapHeight(std::size_t line_count) {
  return height - (line_count == 0 ? 0 : line_count + 1);
}

void Camera::draw(const MapData &map, const Vec2i &center,
                  const std::vector<LogEntry> &rhs_log,
                  const std::vector<LogEntry> &bottom_log) {
  clearView();
  setViewSize();
  resetCursor();

  std::size_t rhs_size = rhs_log.size();
  int map_width = getMapWidth(rhs_log.size());
  int map_height = getMapHeight(bottom_log.size()) - 1; // -1 for input.

  // Gets starting position for the map based on position offset.
  int start_x = center.x - map_width / 2;
  int start_y = center.y - map_height / 2;

  // Stores main buffer and RHS text to be printed.
  std::string output;
  output.reserve(width * (map_height + bottom_log.size() + 2));

  // Draw the map and optionally the RHS text area.
  for (int y = 0; y < map_height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (rhs_size > 0 && x >= map_width) {
        // Handle RHS text area.
        if (x == map_width) {
          output += " | ";
        }

        // Calculate the RHS text line to potentially display at this row.
        std::size_t line =
            y + (rhs_size > map_height ? (rhs_size - map_height) : 0);

        // Print RHS text if this row corresponds to a line in the vector.
        if (line < rhs_size) {
          if (x == map_width + 3) {
            // Position text after separator with correct spacing
            output += rhs_log[line].toString(width - map_width - 3);
          }
        }
      } else if (x < map_width) {
        // Within map bounds.
        int map_x = start_x + x;
        int map_y = start_y + y;

        if (map.inBounds({map_x, map_y})) {
          if (map_x == center.x && map_y == center.y) {
            // Draws the central symbol.
            output += color::stylize("@", color::Foreground::YELLOW);
          } else if (map.data[map_y][map_x] != nullptr) {
            output += map.data[map_y][map_x]->toString();
          } else {
            output += " ";
          }
        } else {
          // Space for out-of-bounds areas
          output += " ";
        }
      }
    }

    output += "\n";
  }

  std::cout << output;
  output = "";

  // Used to draw the bottom-text.
  if (!bottom_log.empty()) {
    // Create the separator.
    for (int x = 0; x < width; ++x) {
      output += "=";
    }
    output += "\n";

    // Write the text to the line.
    for (const auto &line : bottom_log) {
      line.draw();
      output += "\n";
    }
  }

  std::cout << output;
}