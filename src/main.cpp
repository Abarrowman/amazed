#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <unordered_set>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "cell.h"
#include "inner_render.h"
#include "outer_render.h"
#include "block_render.h"


class maze_maker {

public:
  maze_maker(std::default_random_engine& e, size_t w, size_t h) : engine_(e), width_(w), height_(h), next_set_(1) {

    rows.reserve(h);
    rows.emplace_back(inital_row());
    for (size_t y = 1; y < (h - 1); y++) {
      rows.emplace_back(next_row(rows[y - 1]));
    }
    rows.emplace_back(final_row(rows[h - 2]));
  }

  template<typename R>
  void render() {
    R::print_first_row(rows[0]);
    for (size_t y = 1; y < (height_ - 1); y++) {
      R::print_middle_row(rows[y - 1], rows[y]);
    }
    R::print_last_row(rows[height_ - 2], rows[height_ - 1]);
  }

private:

  std::vector<cell> inital_row() {
    std::vector<cell> row;
    row.reserve(width_);

    for (size_t i = 0; i < width_; ++i) {
      row.emplace_back(next_set_ ++);
    }
    x_join_row(row);
    return row;
  }

  void join_sets(std::vector<cell>& row, size_t left_set, size_t right_set) {
    // O(n) reflect which cells are part of the same set
    // consider disjoint set data structure
    size_t min_set = std::min(left_set, right_set);
    size_t max_set = std::max(left_set, right_set);
    for (size_t j = 0; j < width_; ++j) {
      if (row[j].set == max_set) {
        row[j].set = min_set;
      }
    }
  }

  void x_join_row(std::vector<cell>& row) {
    for (size_t i = 0; i < (width_ - 1); ++i) {
      size_t left_set = row[i].set;
      size_t right_set = row[i + 1].set;
      if (left_set != right_set) {
        // unconnected cells
        if (next_double() > x_wall_chance) {
          row[i].wall_right = false;
          row[i + 1].wall_left = false;

          join_sets(row, left_set, right_set);
        }
      }
    }
  }

  std::vector<cell> next_row(std::vector<cell>& above) {
    std::vector<cell> row;
    row.reserve(width_);

    for (size_t i = 0; i < width_; ++i) {
      row.emplace_back(0);
    }

    std::unordered_set<size_t> joined_sets;


    // randomly vertically join some of the remaining cells
    for (size_t i = 0; i < width_; ++i) {
      if (next_double() > y_wall_chance) {
        row[i].wall_above = false;
        above[i].wall_below = false;
        size_t joining_set = above[i].set;
        row[i].set = joining_set;
        joined_sets.insert(joining_set);
      }
    }

    // join each set

    for (size_t n = 0; n < width_; ++n) {
      size_t set = above[n].set;

      if (!joined_sets.insert(set).second) {
        continue; //we've already joined this set
      }

      // std::cout << "Manually joining set " << set << "\n";

      std::vector<size_t> members;
      members.reserve(width_);

      members.push_back(n);
      for (size_t j = n + 1; j < width_; ++j) {
        if (above[j].set == set) {
          members.push_back(j);
        }
      }

      // randomly join one member of this set to the row below
      std::uniform_int_distribution<size_t> member_dist{0, members.size() - 1};
      size_t connected_idx = members[member_dist(engine_)];
      row[connected_idx].wall_above = false;
      above[connected_idx].wall_below = false;
      row[connected_idx].set = set;

      // std::cout << "Set " << set << " connects at idx " << connected_idx << "\n";
    }

    // assign sets to the cells not in sets
    for (size_t i = 0; i < width_; ++i) {
      if (row[i].set == 0) {
        row[i].set = next_set_++;
      }
    }

    //debug_row(row);
    x_join_row(row);
    

    return row;
  }

  std::vector<cell> final_row(std::vector<cell>& above) {
    std::vector<cell> row = next_row(above);

    // find all disconnected cells
    std::vector<size_t> disjoints;
    disjoints.reserve(width_);

    for (size_t i = 0; i < (width_ - 1); ++i) {
      size_t left_set = row[i].set;
      size_t right_set = row[i + 1].set;
      if (left_set != right_set) {
        disjoints.push_back(i);
      }
    }

    // start connecting them in a random order
    std::shuffle(disjoints.begin(), disjoints.end(), engine_);
    for (size_t i : disjoints) {
      size_t left_set = row[i].set;
      size_t right_set = row[i + 1].set;
      if (left_set != right_set) {
        row[i].wall_right = false;
        row[i + 1].wall_left = false;

        join_sets(row, left_set, right_set);
      } // else these sets were already joined
    }

    return row;
  }


  double next_double() {
    thread_local std::uniform_real_distribution<> double_dist{0.0, 1.0};
    return double_dist(engine_);
  }

  void debug_row(std::vector<cell> const& row) {
    std::cout << std::setw(3) << row[0].set;
    for (size_t i = 1; i < width_; ++i) {
      std::cout << ',' << std::setw(3) << row[i].set;
    }
    std::cout << '\n';

  }

  std::vector<std::vector<cell>> rows;
  std::default_random_engine& engine_;
  size_t const width_;
  size_t const height_;
  size_t next_set_;

  double x_wall_chance = 0.5;
  double y_wall_chance = 0.5;
};


#ifdef _WIN32
#include <Windows.h>
void os_specific_setup() {
  SetConsoleOutputCP(CP_UTF8);
  setvbuf(stdout, nullptr, _IOFBF, 1000);
}


#else
void os_specific_setup() {

}
#endif

int main() {
  os_specific_setup();

  std::random_device r;
  //std::default_random_engine engine{r()};
  std::default_random_engine engine{ 0xbaadf00d};

  maze_maker m{engine, 10, 10};

  m.render<inner_render>();
  m.render<outer_render>();
  m.render<block_render>();
}
