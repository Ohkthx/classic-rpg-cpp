#include "input.hpp"
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void setTerminalMode(bool enable) {
  static struct termios oldt, newt;
  if (enable) {
    tcgetattr(STDIN_FILENO, &oldt); // Save the old settings.
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo.
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  } else {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings.
  }
}

int kbhit(void) {
  setTerminalMode(true);
  int bytesWaiting;
  ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
  setTerminalMode(false);
  return bytesWaiting > 0;
}

char getch() {
  setTerminalMode(true);
  char ch = getchar();
  setTerminalMode(false);
  return ch;
}
#endif

Input Input::check(bool wait) {
  Input input;
  if (wait || kbhit()) {
    char ch = getch();
    // Check for quit command.
    if (ch == 'q' || ch == 'Q') {
      input.quit = true;
    } else {
      input.movement_offset = getMovementOffset(ch);
    }
  }
  return input;
}

Vec2i Input::getMovementOffset(char ch) {
  Vec2i offset = Vec2i::ORIGIN();
  switch (ch) {
  case 'w':
  case 'W':
    // Move north.
    offset.y -= 1;
    break;
  case 'd':
  case 'D':
    // Move east.
    offset.x += 1;
    break;
  case 's':
  case 'S':
    // Move south.
    offset.y += 1;
    break;
  case 'a':
  case 'A':
    // Move west.
    offset.x -= 1;
    break;
  }
  return offset;
}
