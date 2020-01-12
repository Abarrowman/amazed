#pragma once

struct cell {
  cell(size_t s) : set(s) {}

  size_t set;
  bool wall_right = true;
  bool wall_above = true;
  bool wall_left = true;
  bool wall_below = true;
};