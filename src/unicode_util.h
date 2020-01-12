#pragma once

static void print_spot(bool up, bool left, bool right, bool down) {
  if (up) {
    if (left) {
      if (right) {
        if (down) {
          std::cout << u8"┼"; // up left right down
        } else {
          std::cout << u8"┴"; // up left right
        }
      } else {
        if (down) {
          std::cout << u8"┤"; // up left down
        } else {
          std::cout << u8"┘"; // up left 
        }
      }
    } else {
      if (right) {
        if (down) {
          std::cout << u8"├"; // up right down
        } else {
          std::cout << u8"└"; // up right
        }
      } else {
        if (down) {
          std::cout << u8"│"; // up down 
        } else {
          std::cout << u8"╵"; // up
        }
      }
    }
  } else {
    if (left) {
      if (right) {
        if (down) {
          std::cout << u8"┬"; // left right down
        } else {
          std::cout << u8"─"; // left right
        }
      } else {
        if (down) {
          std::cout << u8"┐"; // left down
        } else {
          std::cout << u8"╴"; // left
        }
      }
    } else {
      if (right) {
        if (down) {
          std::cout << u8"┌"; // right down
        } else {
          std::cout << u8"╶"; // right
        }
      } else {
        if (down) {
          std::cout << u8"╷"; // down
        } else {
          std::cout << u8" "; // 
        }
      }
    }
  }
}
