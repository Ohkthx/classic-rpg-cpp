#ifndef _UI_CAMERA_HPP
#define _UI_CAMERA_HPP

#include "../map/map.hpp"
#include "../pathfind/pathfind.hpp"
#include "../util/log.hpp"

class Camera {
public:
  void draw(const MapData &map, const Vec2i &center,
            const std::vector<LogEntry> &rhs_log,
            const std::vector<LogEntry> &bottom_log);

private:
  int width, height; // Dimensions of the camera / terminal.
  const double RHS_SPACE = 0.2, LHS_SPACE = 0.2;

  static void clearView();       // Clears the screen.
  void setViewSize();            // Sets the width/height of the view.
  int getMapHeight(std::size_t); // Gets the height based on text offset.
  int getMapWidth(std::size_t);  // Gets the width based on text offset.
  inline void resetCursor();     // Resets cursor to the top of the terminal.
};

#endif