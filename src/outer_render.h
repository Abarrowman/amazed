#pragma once

#include <vector>

#include "cell.h"
#include "unicode_util.h"

struct outer_render {
  static void print_middle_row(std::vector<cell> const& higher, std::vector<cell> const& lower) {

    print_spot(true, false, higher[0].wall_below, true);

    for (size_t i = 0; i < (higher.size() - 1); ++i) {
      print_spot(higher[i].wall_right, higher[i].wall_below, higher[i + 1].wall_below, lower[i].wall_right);
    }


    print_spot(true, higher.back().wall_below, false, true);
    std::cout << "\n";


    //inner_print_row(lower);
  }

  static void print_last_row(std::vector<cell> const& penultimate, std::vector<cell> const& ultimate) {
    print_middle_row(penultimate, ultimate);

    print_spot(true, false, true, false);
    for (size_t i = 0; i < (ultimate.size() - 1); ++i) {
      print_spot(ultimate[i].wall_right, true, true, false);
    }
    print_spot(true, true, false, false);
    std::cout << "\n";
  }

  static void print_first_row(std::vector<cell> const& first) {
    print_spot(false, false, true, true);
    for (size_t i = 0; i < (first.size() - 1); ++i) {
      print_spot(false, true, true, first[i].wall_right);
    }
    print_spot(false, true, false, true);
    std::cout << "\n";
  }
};