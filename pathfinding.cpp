#include "pathfinding.h"

array<Grid, 4> SquareGrid::DIRS =
  {Grid{1, 0}, Grid{0, -1}, Grid{-1, 0}, Grid{0, 1}};

bool operator == (Grid a, Grid b) {
  return a.x == b.x && a.y == b.y;
}

bool operator != (Grid a, Grid b) {
  return !(a == b);
}

bool operator < (Grid a, Grid b) {
  return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}
