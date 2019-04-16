#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <map>
#include <set>
#include <array>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

struct Grid {
  int x, y;
};
bool operator == (Grid a, Grid b);
bool operator != (Grid a, Grid b);
bool operator < (Grid a, Grid b);

struct SquareGrid {
  static array<Grid, 4> DIRS;

  int width, height;
  set<Grid> walls;

  SquareGrid(int width_, int height_)
     : width(width_), height(height_) {}

  bool in_bounds(Grid id) const {
    return 0 <= id.x && id.x < width
        && 0 <= id.y && id.y < height;
  }

  bool passable(Grid id) const {
    return walls.find(id) == walls.end();
  }

  void add_wall(int x, int y) {
    walls.insert(Grid{x, y});
  }

  vector<Grid> neighbors(Grid id) const {
    vector<Grid> results;

    for (Grid dir : DIRS) {
      Grid next{id.x + dir.x, id.y + dir.y};
      if (in_bounds(next) && passable(next)) {
        results.push_back(next);
      }
    }

    if ((id.x + id.y) % 2 == 0) {
      // aesthetic improvement on square grids
      std::reverse(results.begin(), results.end());
    }

    return results;
  }
};

template<typename T, typename priority_t>
struct PriorityQueue {
  typedef pair<priority_t, T> PQElement;
  priority_queue<PQElement, vector<PQElement>,
                 greater<PQElement>> elements;

  inline bool empty() const {
     return elements.empty();
  }

  inline void put(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

inline double heuristic1(Grid a, Grid b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}

inline double heuristic2(Grid a, Grid b) {
  return (abs(a.x - b.x)+1) * (abs(a.y - b.y)+1);
}

template<typename Location, typename Graph>
bool a_star_search
  (Graph graph,
   Location start,
   Location goal,
   map<Location, Location>& came_from,
   map<Location, double>& cost_so_far)
{
  PriorityQueue<Location, double> frontier;
  frontier.put(start, 0);
  Location current;

  came_from[start] = start;
  cost_so_far[start] = 0;

  while (!frontier.empty()) {
    current = frontier.get();

    if (current == goal) {
      break;
    }

    for (Location next : graph.neighbors(current)) {
      if (cost_so_far.find(next) == cost_so_far.end()
          || cost_so_far[current] < cost_so_far[next]) {
        cost_so_far[next] = cost_so_far[current];
        double priority = cost_so_far[next] + heuristic1(next, goal) + heuristic2(start, next);
        frontier.put(next, priority);
        came_from[next] = current;
      }
    }
  }
  if(current != goal) { return false; }
  else { return true; }
}

template<typename Location>
vector<Location> reconstruct_path(
   Location start, Location goal,
   map<Location, Location> came_from
) {
  vector<Location> path;
  Location current = goal;
  while (current != start) {
    path.push_back(current);
    current = came_from[current];
  }
  //path.push_back(start); // optional
  //reverse(path.begin(), path.end());
  return path;
}


#endif // PATHFINDING_H
