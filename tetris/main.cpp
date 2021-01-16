// main.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "comctl32.lib")

#include "framework.h"
#include "tetris.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                      // current instance
WCHAR szTitle[MAX_LOADSTRING];        // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];  // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EndProc(HWND hwndDlg, UINT message, WPARAM wParam,
                          LPARAM lParam);
BOOL CALLBACK LeaderboardProc(HWND hwndDlg, UINT message, WPARAM wParam,
                          LPARAM lParam);
BOOL CALLBACK SettingsProc(HWND hwndDlg, UINT message, WPARAM wParam,
                          LPARAM lParam);
BOOL CALLBACK ChangeColorProc(HWND hwndDlg, UINT message, WPARAM wParam,
                           LPARAM lParam);
  INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

 HWND hMWnd;

HBRUSH hBrushes[8];

COLORREF DefaultColors[8];

CHOOSECOLOR cc;
static COLORREF a;
static COLORREF acrCustClr[16];

static GameTetris* game;

INT iAmountOfFigures = 0;
INT iLevel = 1;
INT iTime = 0;
INT iScore = 0;
double dTicksPerFigures;
WCHAR name[20]; 
bool is_standart;

static INT iTimerSpeed = 500;

static bool is_playing = true;
static bool is_pause;
static bool is_turn_end = false;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  InitCommonControls();

  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRIS);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // Store instance handle in our global variable

  hMWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hMWnd) {
    return FALSE;
  }

  ShowWindow(hMWnd, nCmdShow);
  UpdateWindow(hMWnd);

  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {

  switch (message) {
    case WM_CREATE: {
      SetTimer(hWnd, 1, iTimerSpeed, (TIMERPROC)NULL);

      DefaultColors[0] = RGB(0, 0, 0);
      DefaultColors[1] = RGB(100, 255, 100);
      DefaultColors[2] = RGB(150, 0, 200);
      DefaultColors[3] = RGB(255, 0, 0);
      DefaultColors[4] = RGB(255, 150, 0);
      DefaultColors[5] = RGB(255, 255, 0);
      DefaultColors[6] = RGB(255, 100, 255);
      DefaultColors[7] = RGB(100, 100, 255);

      //hBrushes[0] = CreateSolidBrush(RGB(0, 0, 0));
      //hBrushes[1] = CreateSolidBrush(RGB(100, 255, 100));
      //hBrushes[2] = CreateSolidBrush(RGB(150, 0, 200));
      //hBrushes[3] = CreateSolidBrush(RGB(255, 0, 0));
      //hBrushes[4] = CreateSolidBrush(RGB(255, 150, 0));
      //hBrushes[5] = CreateSolidBrush(RGB(255, 255, 0));
      //hBrushes[6] = CreateSolidBrush(RGB(255, 100, 255));
      //hBrushes[7] = CreateSolidBrush(RGB(100, 100, 255));
      for (int i = 0; i < 8; ++i) {
        hBrushes[i] = CreateSolidBrush(DefaultColors[i]);
      }

      game = new GameTetris();

      cc.lStructSize = sizeof(CHOOSECOLOR);
      cc.hwndOwner = hWnd;
      cc.Flags = CC_FULLOPEN | CC_RGBINIT;
      cc.rgbResult = RGB(255, 255, 255);
      cc.lpCustColors = acrCustClr;
      cc.lCustData = 0L;
    }

    break;

    case WM_CLOSE:
      if (is_playing) {
        is_playing = false;
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ENDGAME), hWnd, (DLGPROC)EndProc);
      }
      DestroyWindow(hWnd);
      return TRUE;
      break;
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      // Parse the menu selections:
      switch (wmId) {
        case IDM_ABOUT:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;

        case IDM_SETTINGS:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd,
                    (DLGPROC)SettingsProc);
          break;
        
        case IDCANCEL:
          //fallthrought
        case IDM_EXIT:
          if (is_playing) {
            is_playing = false;
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ENDGAME), hWnd,
                      (DLGPROC)EndProc);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_LEADERBOARD), hWnd,
                      (DLGPROC)LeaderboardProc);
          }
          DestroyWindow(hWnd);
          break;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    } break;

    case WM_TIMER:
      if (is_playing && !is_pause) {
        if (!game->move_down()) {
          is_turn_end = true;
          iScore += 15 * iLevel;
        }
        ++iTime;
        InvalidateRect(hWnd, NULL, FALSE);
      }
      break;

    case WM_KEYDOWN:
      switch (wParam) {
        case VK_SPACE: {
          int blocks_down = 0;
          if (is_playing && !is_pause) {
            while (game->move_down()) {
              ++blocks_down;
            }
            iScore += blocks_down * 5 * iLevel;
            is_turn_end = true;
            InvalidateRect(hWnd, NULL, FALSE);
          }
        } 
        break;

        case 0x41:
          //falltrough
        case VK_LEFT:
          if (is_playing && !is_pause) {
            game->move_left();
            InvalidateRect(hWnd, NULL, FALSE);
          }
          break;

        case 0x44:
          // falltrough
        case VK_RIGHT:
          if (is_playing && !is_pause) {
            game->move_right();
            InvalidateRect(hWnd, NULL, FALSE);
          }
          break;

        case 0x57:
          // falltrough
        case VK_UP:
          if (is_playing && !is_pause) {
            game->rotate();
            InvalidateRect(hWnd, NULL, FALSE);
          }
          break;

        case 0x53:
          // falltrough
        case VK_DOWN:
          if (is_playing && !is_pause) {
            game->move_down();
            iScore += 3 * iLevel;
            InvalidateRect(hWnd, NULL, FALSE);
          }
          break;

        case VK_ESCAPE:
          is_pause ? is_pause = false : is_pause = true;
          InvalidateRect(hWnd, NULL, TRUE);
          break;
      }
      break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      if (is_playing) {
        drawmatrix(hdc, *game, 10, 10, hBrushes);
        drawmatrix(hdc, *game, 510, 10, hBrushes);
        drawinfo(hdc, iTime, L"Время", 325, 250);
        if (is_turn_end) {
          checkNextLevel(hWnd, iAmountOfFigures, iLevel, iTimerSpeed, 1.1);
          dTicksPerFigures = (double)iTime / iAmountOfFigures;
          drawinfo(hdc, iLevel, L"Level", 325, 150);
          drawinfo(hdc, dTicksPerFigures, L"Ticks", 400, 150);
          int end_stats = game->endTurn();
          if (end_stats == -1) {
            HFONT hFont = CreateFont(100, 50, 0, 0, 50, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
                                     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                     FF_DONTCARE, NULL);
            KillTimer(hWnd, 1);

            HFONT oldFont = (HFONT)GetCurrentObject(hdc, OBJ_FONT);
            SelectObject(hdc, hFont);  //он будет иметь силу только когда мы его выберем
            SetTextColor(hdc, RGB(200, 0, 0));  //зададим цвет текста
            SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 10, 100, L"You lose!!!", 12);

            SetTextColor(hdc, RGB(0, 0, 0));  //зададим цвет текста
            DeleteObject(hFont);
            SelectObject(hdc, oldFont);
            drawinfo(hdc, iAmountOfFigures, L"Кол-во фигур", 425, 150);
            is_playing = false;
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ENDGAME), hWnd,
                      (DLGPROC)EndProc);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_LEADERBOARD), hWnd,
                      (DLGPROC)LeaderboardProc);
          } else {
            iScore += end_stats * end_stats * iLevel * 5;
          }
          drawnextfigure(hdc, *game, 300, 50, hBrushes);
          is_turn_end = false;
        }
        drawinfo(hdc, iScore, L"Счёт", 325, 350);
      } 
      
      if(is_pause){
        Rectangle(hdc, 0, 0, 750, 425);
      }
      EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}

BOOL CALLBACK EndProc(HWND hwndDlg, UINT message, WPARAM wParam,
                          LPARAM lParam) {
  static HWND hScore;
  static HWND hAmountOfFigures;
  static HWND hTicks;
  static HWND hLevel;
  static HWND hTicksPerFigure;
  static HWND hName;
  switch (message) {
    case WM_INITDIALOG:
      hScore = GetDlgItem(hwndDlg, IDC_SCORE);
      hAmountOfFigures = GetDlgItem(hwndDlg, IDC_NUMBER_OF_FIGURES);
      hTicks = GetDlgItem(hwndDlg, IDC_TICKS);
      hLevel = GetDlgItem(hwndDlg, IDC_LEVEL);
      hTicksPerFigure = GetDlgItem(hwndDlg, IDC_TICKS_PER_FIGURE);
      hName = GetDlgItem(hwndDlg, IDC_NAME);
      WCHAR s_tmp[7];
      swprintf(s_tmp, 7, L"%i", iScore);
      SendMessage(hScore, WM_SETTEXT, 0, (LPARAM)s_tmp); 
      swprintf(s_tmp, 7, L"%i", iAmountOfFigures);
      SendMessage(hAmountOfFigures, WM_SETTEXT, 0, (LPARAM)s_tmp);  
      swprintf(s_tmp, 7, L"%i", iTime);
      SendMessage(hTicks, WM_SETTEXT, 0, (LPARAM)s_tmp);  
      swprintf(s_tmp, 7, L"%i", iLevel);
      SendMessage(hLevel, WM_SETTEXT, 0, (LPARAM)s_tmp);  
      swprintf(s_tmp, 7, L"%.*f",3, dTicksPerFigures);
      SendMessage(hTicksPerFigure, WM_SETTEXT, 0, (LPARAM)s_tmp);

      SetFocus(hName);

      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {

        case IDOK: {
          INT text_length = GetWindowTextLength(hName);
          GetWindowText(hName, name, text_length);
          bool is_valid = true;
          if (text_length == 0) is_valid = false;
          for (int i = 0; i < text_length -1; ++i) {
            if (name[i] == ' ') {
              is_valid = false;
            }
          }

          if (is_valid) {
            std::ofstream file;
            file.open("leaderboard.txt", std::ios_base::out | std::ios_base::app);
            std::wstring ws_name(name, 10);
            std::string str;
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            str = converter.to_bytes(ws_name);
            file << str << " " << iScore << " " << iLevel << ' '
                 << iAmountOfFigures << " " << iTime << " "
                 << dTicksPerFigures <<' ';
            file << '\n';
            file.close();
            EndDialog(hwndDlg, wParam);
            return TRUE;
          }else{
            MessageBox(hwndDlg, L"Please enter name without spaces!",  L"Error", MB_ICONWARNING);
          }
        }

          break;


        case IDCANCEL:
          MessageBox(hwndDlg, L"Enter your name",L"Errrror", MB_ICONWARNING);
          break;
        default:
          break;
      }
  }
  return FALSE;
}


BOOL CALLBACK SettingsProc(HWND hwndDlg, UINT message, WPARAM wParam,
                      LPARAM lParam) {
  static HWND hHeight;
  static HWND hWidth;
  switch (message) {
    case WM_INITDIALOG:
      is_playing = false;

      hWidth = GetDlgItem(hwndDlg, IDC_WIDTH);
      hHeight = GetDlgItem(hwndDlg, IDC_HEIGHT);
      WCHAR wstr[4];
      for (int i = 15; i > 5; --i) {
        _itow_s(i, wstr, 10);
        SendMessageW(hWidth, CB_ADDSTRING, 1, (LPARAM)wstr);
      }
      SendMessage(hWidth, CB_SETCURSEL, (WPARAM)(15 - game->getWidth() + 5), 0L);
      for (int i = 25; i > 5; --i) {
        _itow_s(i, wstr, 10);
        SendMessageW(hHeight, CB_ADDSTRING, 1, (LPARAM)wstr);
      }
      SendMessage(hHeight, CB_SETCURSEL, (WPARAM)(25 - game -> getHeight() + 6), 0L);
      break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwndDlg, &ps);
      HBRUSH oldBrush = (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH);
      SelectObject(hdc, hBrushes[0]);
      drawbox(hdc, 250, 50);
      drawbox(hdc, 270, 50);
      drawbox(hdc, 290, 50);
      drawbox(hdc, 250, 70);
      drawbox(hdc, 250, 90);
      drawbox(hdc, 270, 70);
      drawbox(hdc, 350, 50);
      drawbox(hdc, 350, 70);
      
      SelectObject(hdc, hBrushes[1]);
      drawbox(hdc,290,70);
      drawbox(hdc,310,70);
      drawbox(hdc,290,90);
      drawbox(hdc,310,90);

      SelectObject(hdc, hBrushes[2]);
      drawbox(hdc, 350, 90);
      drawbox(hdc, 350, 110);
      drawbox(hdc, 350, 130);
      drawbox(hdc, 350, 150);

      SelectObject(hdc, hBrushes[4]);
      drawbox(hdc, 250, 110);
      drawbox(hdc, 270, 110);
      drawbox(hdc, 250, 130);
      drawbox(hdc, 270, 90);

      SelectObject(hdc, hBrushes[3]);
      drawbox(hdc, 250, 150);
      drawbox(hdc, 270, 150);
      drawbox(hdc, 270, 130);
      drawbox(hdc, 290, 130);

      SelectObject(hdc, hBrushes[5]);
      drawbox(hdc, 310, 50);
      drawbox(hdc, 330, 50);
      drawbox(hdc, 330, 70);
      drawbox(hdc, 330, 90);

      SelectObject(hdc, hBrushes[6]);
      drawbox(hdc, 290, 110);
      drawbox(hdc, 310, 110);
      drawbox(hdc, 330, 110);
      drawbox(hdc, 330, 130);

      SelectObject(hdc, hBrushes[7]);
      drawbox(hdc, 310, 130);
      drawbox(hdc, 290, 150);
      drawbox(hdc, 310, 150);
      drawbox(hdc, 330, 150);

 
      SelectObject(hdc, oldBrush);
      EndPaint(hwndDlg, &ps);

      
    }
      break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        
        case IDC_CHANGESIZE: {
          WCHAR* str = new WCHAR[4];
          int line_number = SendMessage(hWidth, CB_GETCURSEL, 0, 0L);
          SendMessage(hWidth, CB_GETLBTEXT, (WPARAM)line_number, (LPARAM)str);
          int field_width =  _wtoi(str);

          //int ItemIndex = SendMessage((HWND)hWidth, (UINT)CB_GETCURSEL,
          //                            (WPARAM)0, (LPARAM)0);
          //(TCHAR) SendMessage((HWND)hWidth, (UINT)CB_GETLBTEXT,
          //                    (WPARAM)ItemIndex, (LPARAM)str);

          
          line_number = SendMessage(hHeight, CB_GETCURSEL, 0, 0L);
          SendMessage(hHeight, CB_GETLBTEXT, (WPARAM)line_number, (LPARAM)str);
          int field_height =  _wtoi(str);
          game->~GameTetris();
          game = new GameTetris(field_width,field_height);
          InvalidateRect(NULL, NULL, TRUE);
        }
          break;
        case ID_CHANGECOLOR:
          DialogBox(hInst, MAKEINTRESOURCE(IDD_COLOR), hwndDlg,
                    (DLGPROC)ChangeColorProc);
          break;

        case IDCANCEL:
          is_playing = true;
          EndDialog(hwndDlg, wParam);
          return TRUE;
        default:
          break;
      }
  }
  return FALSE;
}

BOOL CALLBACK ChangeColorProc(HWND hwndDlg, UINT message, WPARAM wParam,
                           LPARAM lParam) {
  switch (message) {
    case WM_INITDIALOG:
      break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwndDlg, &ps);
      HBRUSH oldBrush = (HBRUSH)GetCurrentObject(hdc, OBJ_BRUSH);
      SelectObject(hdc, hBrushes[0]);
      Rectangle(hdc, 35, 35, 74, 74);


      SelectObject(hdc, hBrushes[1]);
      drawbox(hdc, 35, 115);
      drawbox(hdc, 55, 115);
      drawbox(hdc, 35, 95);
      drawbox(hdc, 55, 95);

      SelectObject(hdc, hBrushes[2]);
      drawbox(hdc, 15, 175);
      drawbox(hdc, 35, 175);
      drawbox(hdc, 55, 175);
      drawbox(hdc, 75, 175);

      SelectObject(hdc, hBrushes[4]);
      drawbox(hdc, 35, 235);
      drawbox(hdc, 35, 215);
      drawbox(hdc, 15, 215);
      drawbox(hdc, 55, 235);

      SelectObject(hdc, hBrushes[3]);
      drawbox(hdc, 250, 55);
      drawbox(hdc, 250, 35);
      drawbox(hdc, 270, 35);
      drawbox(hdc, 230, 55);

      SelectObject(hdc, hBrushes[5]);
      drawbox(hdc, 250, 115);
      drawbox(hdc, 230, 115);
      drawbox(hdc, 270, 115);
      drawbox(hdc, 270, 95);

      SelectObject(hdc, hBrushes[6]);
      drawbox(hdc, 250, 175);
      drawbox(hdc, 230, 175);
      drawbox(hdc, 270, 175);
      drawbox(hdc, 230, 155);

      SelectObject(hdc, hBrushes[7]);
      drawbox(hdc, 250, 235);
      drawbox(hdc, 230, 235);
      drawbox(hdc, 270, 235);
      drawbox(hdc, 250, 215);




      
      SelectObject(hdc, hBrushes[0]);
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
          drawbox(hdc, 430 + 20 * j, 80 + 20 * i);
        }
      }
      drawbox(hdc, 430, 120);
      drawbox(hdc, 450, 120);
      drawbox(hdc, 470, 120);
      drawbox(hdc, 430, 140);
      drawbox(hdc, 430, 160);
      drawbox(hdc, 450, 140);
      drawbox(hdc, 530, 120);
      drawbox(hdc, 530, 140);

      SelectObject(hdc, hBrushes[1]);
      drawbox(hdc, 470, 140);
      drawbox(hdc, 490, 140);
      drawbox(hdc, 470, 160);
      drawbox(hdc, 490, 160);

      SelectObject(hdc, hBrushes[2]);
      drawbox(hdc, 530, 160);
      drawbox(hdc, 530, 180);
      drawbox(hdc, 530, 200);
      drawbox(hdc, 530, 220);

      SelectObject(hdc, hBrushes[4]);
      drawbox(hdc, 430, 180);
      drawbox(hdc, 450, 180);
      drawbox(hdc, 430, 200);
      drawbox(hdc, 450, 160);

      SelectObject(hdc, hBrushes[3]);
      drawbox(hdc, 430, 220);
      drawbox(hdc, 450, 220);
      drawbox(hdc, 450, 200);
      drawbox(hdc, 470, 200);

      SelectObject(hdc, hBrushes[5]);
      drawbox(hdc, 490, 120);
      drawbox(hdc, 510, 120);
      drawbox(hdc, 510, 140);
      drawbox(hdc, 510, 160);

      SelectObject(hdc, hBrushes[6]);
      drawbox(hdc, 470, 180);
      drawbox(hdc, 490, 180);
      drawbox(hdc, 510, 180);
      drawbox(hdc, 510, 200);

      SelectObject(hdc, hBrushes[7]);
      drawbox(hdc, 490, 220);
      drawbox(hdc, 470, 220);
      drawbox(hdc, 490, 200);
      drawbox(hdc, 510, 220);

      SelectObject(hdc, oldBrush);
      EndPaint(hwndDlg, &ps);
    } break;

    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case IDC_CHANGE_0:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[0]);
            hBrushes[0] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_CHANGE_1:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[1]);
            hBrushes[1] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_CHANGE_2:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[2]);
            hBrushes[2] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_CHANGE_3:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[4]);
            hBrushes[4] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_CHANGE_4:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[3]);
            hBrushes[3] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_CHANGE_5:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[5]);
            hBrushes[5] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_CHANGE_6:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[6]);
            hBrushes[6] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_CHANGE_7:
          if (ChooseColorW(&cc)) {
            a = cc.rgbResult;
            DeleteObject(hBrushes[7]);
            hBrushes[7] = CreateSolidBrush(a);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDC_DEFAULT:
          for (int i = 0; i < 8; ++i) {
            DeleteObject(hBrushes[i]);
            hBrushes[i] = CreateSolidBrush(DefaultColors[i]);
          }
          InvalidateRect(NULL, NULL, TRUE);
          break;
        case IDCANCEL:
          EndDialog(hwndDlg, wParam);
          return TRUE;
        default:
          break;
      }
  }
  return FALSE;
}

BOOL CALLBACK LeaderboardProc(HWND hwndDlg, UINT message, WPARAM wParam,
                      LPARAM lParam) {
  std::ifstream ifile("leaderboard.txt",
                             std::ios_base::in | std::ios_base::app);
  static HWND hwndList;
  static HWND hOk;
  switch (message) {
    case WM_INITDIALOG: {
      hwndList = GetDlgItem(hwndDlg, IDC_LISTBOX);  
      hOk = GetDlgItem(hwndDlg, IDOK);
      SetFocus(hOk);
      
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
      std::vector<GameInfo> info;
      std::string str;
      std::wstring wstr;
      GameInfo game_info;

      ifile >> str;

      wstr = converter.from_bytes(str);

      wcscpy_s(game_info.name, wstr.c_str());

      ifile >> game_info.score >> game_info.level >>
          game_info.amount_of_figures >> game_info.time >>
          game_info.ticks_per_figure;
      info.push_back(game_info);

      
      do{

        ifile >> str;

        wstr = converter.from_bytes(str);
        
        wcscpy_s(game_info.name, wstr.c_str());
       
        
        ifile >> game_info.score >> game_info.level >>
            game_info.amount_of_figures >> game_info.time >> game_info.ticks_per_figure;
        info.push_back(game_info);
      } while (!ifile.eof()); 
      ifile.close();
      std::sort(info.begin(), info.end(),
                [&](const GameInfo a, const GameInfo b) { return (a.score > b.score); });
      WCHAR* addeble_string = new WCHAR[20];
      WCHAR* result_string = new WCHAR[100];
      for (auto k : info) {
        result_string[0] = '\0';

        lstrcpy(addeble_string, k.name);
        AddFlatString(result_string, k.name);
        AddFlatString(result_string, k.score);
        AddFlatString(result_string, k.level);
        AddFlatString(result_string, k.amount_of_figures);
        AddFlatString(result_string, k.time);
        AddFlatString(result_string, k.ticks_per_figure);
        SendMessage(hwndList, LB_ADDSTRING, NULL, (LPARAM)result_string);
       
      }
       delete[] result_string; 
      return TRUE;
      break;
    }
                    
      break;
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case IDOK: 
          iTimerSpeed = 500;
          SetTimer(hMWnd, 1, iTimerSpeed, (TIMERPROC)NULL);
          is_playing = true;
          is_turn_end = false;
          game->clear();
          iScore = 0; 
          iAmountOfFigures = 0; 
          iLevel = 1;
          dTicksPerFigures = 0;
          iTime = 0;
          InvalidateRect(hMWnd, NULL, TRUE);
          EndDialog(hwndDlg, wParam);
          return TRUE;
        break;

        case IDCANCEL:
          // SendMessage(hWndM, WM_LBUTTONDBLCLK, 0, 0);
          // InvalidateRect(hWndM, NULL, TRUE);
          EndDialog(hwndDlg, wParam);
          return TRUE;
        default:
          break;
      }
  }
  return FALSE;
}
