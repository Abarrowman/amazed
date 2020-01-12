#pragma once

#include <vector>
#include <sstream>

#include "cell.h"
#include "unicode_util.h"

static void print_block(
  bool up, bool left, bool right, bool down,
  std::stringstream& top, std::stringstream& middle, std::stringstream& bottom,
  bool first, bool last
  ) {

  if (first) {
    if (up) {
      top << u8"█ ";
    } else {
      top << u8"  ";
    }
  } else if (last) {
    if (up) {
      top << u8" █";
    } else {
      top << u8"  ";
    }
  } else {
    if (up) {
      top << u8" █ ";
    } else {
      top << u8"   ";
    }
  }

  if (first) {
    if (right) {
      middle << u8"██";
    } else {
      if (up || down) {
        middle << u8"█ ";
      } else {
        middle << u8"  "; // free
      }
    }
  } else if (last) {
    if (left) {
      middle << u8"██";
    } else {
      if (up || down) {
        middle << u8" █";
      } else {
        middle << u8"   "; // free
      }
    }
  } else {
    if (left) {
      if (right) {
        middle << u8"███";
      } else {
        middle << u8"██ ";
      }
    } else {
      if (right) {
        middle << u8" ██";
      } else {
        if (up || down) {
          middle << u8" █ ";
        } else {
          middle << u8"   "; // free
        }
      }
    }
  }
  
  if (first) {
    if (down) {
      bottom << u8"█ ";
    } else {
      bottom << u8"  ";
    }
  } else if (last) {
    if (down) {
      bottom << u8" █";
    } else {
      bottom << u8"  ";
    }
  } else {
    if (down) {
      bottom << u8" █ ";
    } else {
      bottom << u8"   ";
    }
  }
}

struct block_render {
  static void print_middle_row(std::vector<cell> const& higher, std::vector<cell> const& lower) {

    std::stringstream top;
    std::stringstream middle;
    std::stringstream bottom;

    print_block(true, false, higher[0].wall_below, true,
      top, middle, bottom,
      true, false);

    for (size_t i = 0; i < (higher.size() - 1); ++i) {
      print_block(higher[i].wall_right, higher[i].wall_below, higher[i + 1].wall_below, lower[i].wall_right,
        top, middle, bottom,
        false, false);
    }

    print_block(true, higher.back().wall_below, false, true,
      top, middle, bottom,
      false, true);

    std::cout << top.str() << "\n" << middle.str() << "\n" << bottom.str() << "\n";
  }

  static void print_last_row(std::vector<cell> const& penultimate, std::vector<cell> const& ultimate) {
    print_middle_row(penultimate, ultimate);

    std::stringstream top;
    std::stringstream middle;
    std::stringstream bottom;

    print_block(true, false, true, false,
      top, middle, bottom,
      true, false);
    for (size_t i = 0; i < (ultimate.size() - 1); ++i) {
      print_block(ultimate[i].wall_right, true, true, false,
        top, middle, bottom,
        false, false);
    }
    print_block(true, true, false, false,
      top, middle, bottom,
      false, true);
    
    std::cout << top.str() << "\n" << middle.str() << "\n";
  }

  static void print_first_row(std::vector<cell> const& first) {
    std::stringstream top;
    std::stringstream middle;
    std::stringstream bottom;

    print_block(false, false, true, true,
      top, middle, bottom,
      true, false);
    for (size_t i = 0; i < (first.size() - 1); ++i) {
      print_block(false, true, true, first[i].wall_right,
        top, middle, bottom,
        false, false);
    }
    print_block(false, true, false, true,
      top, middle, bottom,
      false, true);

    std::cout << middle.str() << "\n" << bottom.str() << "\n";
  }
};