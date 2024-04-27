#include "terminal.hpp"
#include "../map/tile.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

void Terminal::setSize() {
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

void Terminal::clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  std::cout << "\033[2J\033[H";
#endif
}

void resetCursor() { std::cout << "\033[0;0H"; }

void Terminal::draw(const MapData &map) {
  clearScreen();
  setSize();

  // Stores main buffer.
  std::string output;

  // Draw the map.
  for (int y = 0; y < map.data.size(); ++y) {
    for (int x = 0; x < map.data[y].size(); ++x) {
      if (map.data[y][x] != nullptr) {
        output += map.data[y][x]->toString();
      } else {
        output += " ";
      }
    }
    output += "\n";
  }
  std::cout << output;
}