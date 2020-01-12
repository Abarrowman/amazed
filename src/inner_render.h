#pragma once

#include <vector>

#include "cell.h"
#include "unicode_util.h"

struct inner_render {

  static void inner_print_row(std::vector<cell> const& row) {
    for (cell const& c : row) {
      print_spot(!c.wall_above, !c.wall_left, !c.wall_right, !c.wall_below);
    }
    std::cout << "\n";
  }

  static void print_middle_row(std::vector<cell> const& higher, std::vector<cell> const& lower) {
    inner_print_row(lower);
  }

  static void print_last_row(std::vector<cell> const& penultimate, std::vector<cell> const& ultimate) {
    inner_print_row(ultimate);
  }

  static void print_first_row(std::vector<cell> const& first) {
    inner_print_row(first);
  }
};
