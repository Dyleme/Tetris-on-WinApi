#include "tetris.h"

std::mt19937 rng;
std::uniform_int_distribution<int> dist(0, 6);
std::uniform_int_distribution<int> dist05(0, 5);

GameTetris::GameTetris(int width_, int height_) {
  width = width_;
  height = height_ + 1;
  matrix = new int*[width];
  for (size_t i = 0; i < width; i++) {
    matrix[i] = new int[height];
    for (size_t j = 0; j < height - 1; ++j) {
      matrix[i][j] = 0;
    }
  }


  rng.seed(time(NULL));
  current_figure = static_cast<type>(dist(rng));
  next_figure = static_cast<type>(dist(rng));
  endTurn();
  drawFigure();
}

GameTetris::~GameTetris() {
  for (size_t i = 0; i < width; i++) {
    delete[] matrix[i];
  }
  delete[] matrix;
}

int** GameTetris::getMatrix() { return matrix; }

int GameTetris::getWidth() { return width; }

int GameTetris::getHeight() { return height; }

type GameTetris::getNextFigure() { return next_figure; }

int GameTetris::get_figure_x() { return figure_x; }
int GameTetris::get_figure_y() { return figure_y; }

void GameTetris::clear() {
  for (size_t i = 0; i < width; i++) {
    for (size_t j = 0; j < height; j++) {
      matrix[i][j] = 0;
    }
  }
}

int GameTetris::endTurn() {
  int amount_of_lines = deleteFull();
  previous_figure = current_figure;
  current_figure = next_figure;
  if (previous_figure != current_figure) {
    next_figure = static_cast<type>(dist(rng));
  }else{
    int next = dist05(rng);
    if (next >= current_figure) ++next;
    next_figure = static_cast<type>(next);
  } 
  figure_x = width / 2 ;
  figure_y = 1;
  state = 0;
  if (!checkFigure(figure_x, figure_y)) {
    return -1;
  };
  drawFigure();
  return amount_of_lines;
}

int GameTetris::deleteFull() {
  int amount_of_full_lines = 0;
  for (int i = height - 1; i >= 0; --i) {
    bool is_line_full = true;
    for (int j = 0; j < width; j++) {
      if (matrix[j][i] == 0) {
        is_line_full = false;
        break;
      }
    }
    if (is_line_full) {
      ++amount_of_full_lines;
      for (int a = i; a > 0; --a) {
        for (int b = 0; b < width; ++b) {
          matrix[b][a] = matrix[b][a - 1];
        }
      }
      ++i;
    }
  }
  return amount_of_full_lines;
}


bool GameTetris::move_down() {
  clearFigure();
  if (checkFigure(figure_x, figure_y + 1)) {
    ++figure_y;
    drawFigure();
    return true;
  } else {
    drawFigure();
    return false;
  }
}

bool GameTetris::move_right() {
  clearFigure();
  if (checkFigure(figure_x + 1, figure_y)) {
    ++figure_x;
    drawFigure();
    return true;
  } else {
    drawFigure();
    return false;
  }
}

bool GameTetris::move_left() {
  clearFigure();
  if (checkFigure(figure_x - 1, figure_y)) {
    --figure_x;
    drawFigure();
    return true;
  } else {
    drawFigure();
    return false;
  }
}

bool GameTetris::rotate() { 
  int next_state = state - 1;
  int x = figure_x;
  int y = figure_y;
  switch (current_figure) {
    case square:
      next_state = 0;
      break;
    case stick:
      if (next_state == -1) next_state = 1; 
      if (next_state == 0) {
        if (x == 0) ++x;
        if (x == width - 1) x -= 2;
      }
      break;
      case zet:
      if (next_state == -1) next_state = 1; 
      if (next_state == 0) {
        if (x == width -1) --x;
      }
      break;
    case rev_zet:
      if (next_state == -1) next_state = 1; 
      if (next_state == 0) {
        if (x == width - 1) --x;
      }
      break;
    case l:
      if (next_state == -1) next_state = 3; 
      if (next_state == 2) {
        if (x == 0) ++x;
      }
      if (next_state == 0) {
        if (x == width - 1) --x;
      }
      break;
    case rev_l:
      if (next_state == -1) next_state = 3; 
      if (next_state == 2) {
        if (x == 0) ++x;
      }
      if (next_state == 0) {
        if (x == width - 1) --x;
      }
      break;
    case t:
      if (next_state == -1) next_state = 3; 
      if (next_state == 2) {
        if (x == 0) ++x;
      }
      if (next_state == 0) {
        if (x == width - 1) --x;
      }
      break;
    default:
      break;
  }
  clearFigure();
  if (checkFigure(x, y, next_state)) {
    state = next_state;
    figure_x = x;
    figure_y = y;
    drawFigure();
    return true;
  } else {
    drawFigure();
    return false;
  }
}

void GameTetris::clearFigure() {
  switch (current_figure) {
    case square:
      draw_square(transparent);
      break;
    case stick:
      draw_stick(transparent);
      break;
    case zet:
      draw_zet(transparent);
      break;
    case rev_zet:
      draw_rev_zet(transparent);
      break;
    case l:
      draw_l(transparent);
      break;
    case rev_l:
      draw_rev_l(transparent);
      break;
    case t:
      draw_t(transparent);
      break;
    default:
      break;
  }
}

void GameTetris::drawFigure() {
  if (current_figure == square) {
    draw_square(green);
  } else if (current_figure == stick) {
    draw_stick(purple);
  } else if (current_figure == zet) {
    draw_zet(ornage);
  } else if (current_figure == rev_zet) {
    draw_rev_zet(red);
  } else if (current_figure == l) {
    draw_l(yellow);
  } else if (current_figure == rev_l) {
    draw_rev_l(pink);
  } else if (current_figure == t) {
    draw_t(blue);
  }
}

bool GameTetris::checkFigure(int x, int y, int state_) {
  if (state_ == -1) {
    state_ = state;  // function without arguiments
  }
  if (current_figure == square) {
    if (x >= 0 && x <= width - 2 && figure_y <= height - 1) {
      if (matrix[x][y] == 0 && matrix[x + 1][y] == 0 &&
          matrix[x + 1][y - 1] == 0 && matrix[x][y - 1] == 0) {
        return true;
      }
    }
  } else if (current_figure == stick) {
    switch (state_) {
      case 0:
        if (x >= 1 && x <= width - 3 && figure_y <= height - 1) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x + 1][y] == 0 && matrix[x + 2][y] == 0) {
            return true;
          }
        }
        break;
      case 1:
        if (x >= 0 && x <= width - 1 && figure_y <= height - 3) {
          if (matrix[x][y - 1] == 0 && matrix[x][y] == 0 &&
              matrix[x][y + 1] == 0 && matrix[x][y + 2] == 0) {
            return true;
          }
        }
        break;
      default:
        break;
    }
  } else if (current_figure == zet) {
    switch (state_) {
      case 0:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 1) {
          if (matrix[x + 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x][y - 1] == 0 && matrix[x - 1][y - 1] == 0) {
            return true;
          }
        }
        break;
      case 1:
        if (x >= 1 && x <= width - 1 && figure_y <= height - 2) {
          if (matrix[x][y - 1] == 0 && matrix[x - 1][y] == 0 &&
              matrix[x][y] == 0 && matrix[x - 1][y + 1] == 0) {
            return true;
          }
        }
        break;
      default:
        break;
    }
  } else if (current_figure == rev_zet) {
    switch (state_) {
      case 0:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 1) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x][y - 1] == 0 && matrix[x + 1][y - 1] == 0) {
            return true;
          }
        }
        break;
      case 1:
        if (x >= 1 && x <= width - 1 && figure_y <= height - 2) {
          if (matrix[x - 1][y - 1] == 0 && matrix[x - 1][y] == 0 &&
              matrix[x][y] == 0 && matrix[x][y + 1] == 0) {
            return true;
          }
        }
        break;
      default:
        break;
    }
  } else if (current_figure == l) {
    switch (state_) {
      case 0:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 1) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x + 1][y] == 0 && matrix[x + 1][y - 1] == 0) {
            return true;
          }
        }
        break;
      case 1:
        if (x >= 1 && x <= width - 1 && figure_y <= height - 2) {
          if (matrix[x - 1][y - 1] == 0 && matrix[x][y - 1] == 0 &&
              matrix[x][y] == 0 && matrix[x][y + 1] == 0) {
            return true;
          }
        }
        break;
      case 2:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 2) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x + 1][y] == 0 && matrix[x - 1][y + 1] == 0) {
            return true;
          }
        }
        break;
      case 3:
        if (x >= 0 && x <= width - 2 && figure_y <= height - 2) {
          if (matrix[x][y - 1] == 0 && matrix[x][y] == 0 &&
              matrix[x][y + 1] == 0 && matrix[x + 1][y + 1] == 0) {
            return true;
          }
        }
        break;
      default:
        break;
    }
  } else if (current_figure == rev_l) {
    switch (state_) {
      case 0:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 1) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x + 1][y] == 0 && matrix[x - 1][y - 1] == 0) {
            return true;
          }
        }
        break;
      case 1:
        if (x >= 1 && x <= width - 1 && figure_y <= height - 2) {
          if (matrix[x - 1][y + 1] == 0 && matrix[x][y - 1] == 0 &&
              matrix[x][y] == 0 && matrix[x][y + 1] == 0) {
            return true;
          }
        }
        break;
      case 2:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 2) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x + 1][y] == 0 && matrix[x + 1][y + 1] == 0) {
            return true;
          }
        }
        break;
      case 3:
        if (x >= 0 && x <= width - 2 && figure_y <= height - 2) {
          if (matrix[x][y - 1] == 0 && matrix[x][y] == 0 &&
              matrix[x][y + 1] == 0 && matrix[x + 1][y - 1] == 0) {
            return true;
          }
        }
        break;
      default:
        break;
    }
  } else if (current_figure == t) {
    switch (state_) {
      case 0:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 1) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x + 1][y] == 0 && matrix[x][y - 1] == 0) {
            return true;
          }
        }
        break;
      case 1:
        if (x >= 1 && x <= width - 1 && figure_y <= height - 2) {
          if (matrix[x - 1][y] == 0 && matrix[x][y - 1] == 0 &&
              matrix[x][y] == 0 && matrix[x][y + 1] == 0) {
            return true;
          }
        }
        break;
      case 2:
        if (x >= 1 && x <= width - 2 && figure_y <= height - 2) {
          if (matrix[x - 1][y] == 0 && matrix[x][y] == 0 &&
              matrix[x + 1][y] == 0 && matrix[x][y + 1] == 0) {
            return true;
          }
        }
        break;
      case 3:
        if (x >= 0 && x <= width - 2 && figure_y <= height - 2) {
          if (matrix[x][y - 1] == 0 && matrix[x][y] == 0 &&
              matrix[x][y + 1] == 0 && matrix[x + 1][y] == 0) {
            return true;
          }
        }
        break;
      default:
        break;
    }
  }
  return false;
}

void GameTetris::draw_square(color clr) {
  matrix[figure_x][figure_y] = clr;
  matrix[figure_x][figure_y - 1] = clr;
  matrix[figure_x + 1][figure_y] = clr;
  matrix[figure_x + 1][figure_y - 1] = clr;
}

void GameTetris::draw_stick(color clr) {
  switch (state) {
    case 0:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      matrix[figure_x + 2][figure_y] = clr;
      break;
    case 1:
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      matrix[figure_x][figure_y + 2] = clr;
      break;
    default:
      break;
  }
}

void GameTetris::draw_zet(color clr) {
  switch (state) {
    case 0:
      matrix[figure_x - 1][figure_y - 1] = clr;
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      break;
    case 1:
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x - 1][figure_y + 1] = clr;
      break;
    default:
      break;
  }
}

void GameTetris::draw_rev_zet(color clr) {
  switch (state) {
    case 0:
      matrix[figure_x + 1][figure_y - 1] = clr;
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x - 1][figure_y] = clr;
      break;
    case 1:
      matrix[figure_x][figure_y + 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x - 1][figure_y - 1] = clr;
      break;
    default:
      break;
  }
}

void GameTetris::draw_l(color clr) {
  switch (state) {
    case 0:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      matrix[figure_x + 1][figure_y - 1] = clr;
      break;
    case 1:
      matrix[figure_x - 1][figure_y - 1] = clr;
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      break;
    case 2:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      matrix[figure_x - 1][figure_y + 1] = clr;
      break;
    case 3:
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      matrix[figure_x + 1][figure_y + 1] = clr;
      break;
    default:
      break;
  }
}

void GameTetris::draw_rev_l(color clr) {
  switch (state) {
    case 0:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      matrix[figure_x - 1][figure_y - 1] = clr;
      break;
    case 1:
      matrix[figure_x - 1][figure_y + 1] = clr;
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      break;
    case 2:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      matrix[figure_x + 1][figure_y + 1] = clr;
      break;
    case 3:
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      matrix[figure_x + 1][figure_y - 1] = clr;
      break;
    default:
      break;
  }
}

void GameTetris::draw_t(color clr) {
  switch (state) {
    case 0:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      matrix[figure_x][figure_y - 1] = clr;
      break;
    case 1:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      break;
    case 2:
      matrix[figure_x - 1][figure_y] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      break;
    case 3:
      matrix[figure_x][figure_y - 1] = clr;
      matrix[figure_x][figure_y] = clr;
      matrix[figure_x][figure_y + 1] = clr;
      matrix[figure_x + 1][figure_y] = clr;
      break;
    default:
      break;
  }
}
