#pragma once

#include "Resource.h"
#include <ctime>
#include <iostream>
#include <random>

class Figure;
class GameTetris;

enum type { square, stick, zet, rev_zet, l, rev_l, t};

enum color { transparent = 0, green, purple, red, ornage, yellow, pink, blue};

class  GameTetris {
 private:
  int** matrix; 
  size_t height;
  size_t width;
  
  type previous_figure;
  type current_figure;
  type next_figure;
  int state;
  int figure_x;
  int figure_y;

 public:
   GameTetris(int width_ = 9, int height_= 18);
   ~GameTetris();

   int** getMatrix();
   int getWidth();
   int getHeight();
   type getNextFigure();
   int get_figure_x();
   int get_figure_y();

   void clear();
 
   int endTurn();

   int deleteFull();


   bool move_down();
   bool move_right();
   bool move_left();

   bool rotate();

  private:
   void clearFigure();
   void drawFigure();

   bool checkFigure(int x, int y, int state_ = -1);

   void draw_square(color clr);
   void draw_stick(color clr);
   void draw_zet(color clr);
   void draw_rev_zet(color clr);
   void draw_l(color clr);
   void draw_rev_l(color clr);
   void draw_t(color clr);
 };
