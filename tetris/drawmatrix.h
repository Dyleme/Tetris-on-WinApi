#pragma once

#include "framework.h"
#include "tetris.h"

struct GameInfo {
  wchar_t name[11];
  int score;
  int level;
  int amount_of_figures;
  int time;
  double ticks_per_figure;
};

void drawmatrix(HDC hdc, GameTetris& game, int pos_x, int pos_y,
                HBRUSH* hBrushes);

void redrawpart(HDC hdc, GameTetris& game, int pos_x, int pos_y,
                HBRUSH* hBrushes);

void drawnextfigure(HDC hdc, GameTetris& game, int pos_x, int pos_y,
                    HBRUSH* hBrushes);

void drawbox(HDC hdc, int pos_x, int pos_y);

void drawinfo(HDC hdc, int level, const WCHAR* name, int pos_x, int pos_y);
void drawinfo(HDC hdc, double level, const WCHAR* name, int pos_x, int pos_y);


bool checkNextLevel(HWND hwnd, int& amount_of_figures, int& level, int& timer_gap, double timer_change);

void FlatString(WCHAR* str);

void AddFlatString(WCHAR* dist, WCHAR* source);
void AddFlatString(WCHAR* dist, int value);
void AddFlatString(WCHAR* dist, double value);
