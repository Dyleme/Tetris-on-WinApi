
#include "drawmatrix.h"

#define square_size 20

void drawmatrix(HDC hdc, GameTetris& game, int pos_x, int pos_y,
                HBRUSH* hBrushes) {
  int** matrix = game.getMatrix();
  int width = game.getWidth();
  int height = game.getHeight();
  HBRUSH oldBrush = (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH);

  POINT pt;
  for (int i = 0; i <= width; ++i) {
    MoveToEx(hdc, pos_x + i * square_size, pos_y + square_size, &pt);
    LineTo(hdc, pos_x + i * square_size, pos_y + (height - 1) * square_size);
  }
  for (int i = 1; i <= height; ++i) {
    MoveToEx(hdc, pos_x, pos_y + i * square_size, &pt);
    LineTo(hdc, pos_x + (width - 2) * square_size, pos_y + i * square_size);
  }
  for (int i = 0; i < width; ++i) {
    for (int j = 1; j < height; ++j) {
      SelectObject(hdc, hBrushes[matrix[i][j]]);
      drawbox(hdc, pos_x + i * square_size, pos_y + j * square_size);
    }
  }
  SelectObject(hdc, oldBrush);
}

void redrawpart(HDC hdc, GameTetris& game, int pos_x, int pos_y,
                HBRUSH* hBrushes) {
  int** matrix = game.getMatrix();
  int width = game.getWidth();
  int height = game.getHeight();
  HBRUSH oldBrush = (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH);

  int x = game.get_figure_x();
  int y = game.get_figure_y();
  for (int i = x - 1; i < x + 4; ++i) {
    for (int j = y - 1; j < y + 4; ++j) {
      SelectObject(hdc, hBrushes[matrix[i][j]]);
      drawbox(hdc, pos_x + (i - 1) * square_size,
              pos_y + (j - 1) * square_size);
    }
  }
  SelectObject(hdc, oldBrush);
}

void drawnextfigure(HDC hdc, GameTetris& game, int pos_x, int pos_y,
                    HBRUSH* hBrushes) {
  type next_figure = game.getNextFigure();
  Rectangle(hdc, pos_x, pos_y, pos_x + 6 * square_size,
            pos_y + 4 * square_size);
  HBRUSH oldBrush = (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH);
  switch (game.getNextFigure()) {
    case square:
      SelectObject(hdc, hBrushes[1]);
      drawbox(hdc, pos_x + square_size, pos_y + square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + square_size);
      drawbox(hdc, pos_x + square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + 2 * square_size);
      break;
    case stick:
      SelectObject(hdc, hBrushes[2]);
      drawbox(hdc, pos_x + square_size, pos_y + square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + square_size);
      drawbox(hdc, pos_x + 3 * square_size, pos_y + square_size);
      drawbox(hdc, pos_x + 4 * square_size, pos_y + square_size);
      break;
    case zet:
      SelectObject(hdc, hBrushes[4]);
      drawbox(hdc, pos_x + square_size, pos_y + square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + square_size);
      drawbox(hdc, pos_x + 3 * square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + 2 * square_size);
      break;
    case rev_zet:
      SelectObject(hdc, hBrushes[3]);
      drawbox(hdc, pos_x + 3 * square_size, pos_y + square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + square_size);
      drawbox(hdc, pos_x + square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + 2 * square_size);
      break;
    case l:
      SelectObject(hdc, hBrushes[5]);
      drawbox(hdc, pos_x + square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 3 * square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 3 * square_size, pos_y + square_size);
      break;
    case rev_l:
      SelectObject(hdc, hBrushes[6]);
      drawbox(hdc, pos_x + square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 3 * square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + square_size, pos_y + square_size);
      break;
    case t:
      SelectObject(hdc, hBrushes[7]);
      drawbox(hdc, pos_x + square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 3 * square_size, pos_y + 2 * square_size);
      drawbox(hdc, pos_x + 2 * square_size, pos_y + square_size);
      break;
    default:
      break;
  }
  SelectObject(hdc, oldBrush);
}

void drawbox(HDC hdc, int pos_x, int pos_y) {
  Rectangle(hdc, pos_x, pos_y, pos_x + square_size + 1,
            pos_y + square_size + 1);
}

void drawinfo(HDC hdc, int value, const WCHAR* info_name, int pos_x,
              int pos_y) {
  wchar_t sInfo[10] = {'\0', '\0', '\0', '\0'};
  _itow_s(value, sInfo, 10);
  RECT rect2{pos_x, pos_y, pos_x + 75, pos_y + 20};
  RECT rect{pos_x, pos_y, pos_x + 75, pos_y + 75};
  Rectangle(hdc, pos_x, pos_y, pos_x + 75, pos_y + 75);
  DrawText(hdc, info_name, -1, &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  DrawText(hdc, sInfo, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void drawinfo(HDC hdc, double value, const WCHAR* info_name, int pos_x,
              int pos_y) {
  wchar_t sInfo[10] = {'\0', '\0', '\0', '\0'};
  swprintf(sInfo, 10, L"%.*f", 3, value);
  RECT rect2{pos_x, pos_y, pos_x + 75, pos_y + 20};
  RECT rect{pos_x, pos_y, pos_x + 75, pos_y + 75};
  Rectangle(hdc, pos_x, pos_y, pos_x + 75, pos_y + 75);
  DrawText(hdc, info_name, -1, &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  DrawText(hdc, sInfo, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool checkNextLevel(HWND hWnd, int& amount_of_figures, int& level,
                    int& timer_gap, double timer_change) {
  ++amount_of_figures;
  if (amount_of_figures % 10 == 0) {
    KillTimer(hWnd, 1);
    timer_gap /= timer_change;
    ++level;
    SetTimer(hWnd, 1, timer_gap, (TIMERPROC)NULL);
  }
  return false;
}


void AddFlatString(WCHAR* dist, int value) {
  WCHAR* addeble_string = new WCHAR[15];
  wsprintf(addeble_string, L"%i", value);
  FlatString(addeble_string);
  lstrcat(dist, addeble_string);
  //delete addeble_string;
}

void AddFlatString(WCHAR* dist, double value) {
  WCHAR* addeble_string = new WCHAR[15];
  wsprintf(addeble_string, L"%f", value);
  FlatString(addeble_string);
  lstrcat(dist, addeble_string);
}

void AddFlatString(WCHAR* dist, WCHAR* source) {
  WCHAR* temp = new WCHAR[15];
  lstrcpy(temp, source);
  FlatString(temp);
  lstrcat(dist, temp);
}
void FlatString(WCHAR* str) {
  int length = wcslen(str);
  for (int i = length; i <= 15; ++i) {
    lstrcat(str, L" ");
  }
}
