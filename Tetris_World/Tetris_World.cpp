// Tetris_World.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Tetris_World.h"
#include "Render.h"
#include "sdk.h"
#include "Interface.h"
#include <conio.h>
#include "Web_Connect.h"
#include "Tetris_Logic.h"

sdk sdk_game;
c_render* render;
web_connect_t* webconnect;

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND TEXTBOX;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TETRISWORLD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRISWORLD));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRISWORLD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TETRISWORLD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP | WS_VISIBLE,
       (GetSystemMetrics(SM_CXSCREEN) >> 1) - (520 >> 1), (GetSystemMetrics(SM_CYSCREEN) >> 1) - (780 >> 1), 520, 780, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: {
        SetTimer(hWnd, 1, 1, 0);
        }
        break;
    case WM_TIMER: {

        InvalidateRect(hWnd, NULL, FALSE);

        interfaces->check_already_name();

        //webconnect->init();
        logic->sort_table_leaders();

        if (sdk_game.start_game) {
            render->OnUpdate();

            sdk_game.reset_controls();
        }
        else {
            sdk_game.server.write_score = 0;
            sdk_game.server.write_player = "";
            sdk_game.server.write = false;
        }

        if (sdk_game.control_set.m_flmouse_reset) {
            sdk_game.control_set.m_flcontrol_mouse_x = 0;
            sdk_game.control_set.m_flcontrol_mouse_y = 0;
            sdk_game.control_set.m_flmouse_reset = false;
        }

        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_DOWN:
            if (!(HIWORD(lParam) & KF_REPEAT)) {
                if (!sdk_game.m_flrotate_block)
                    sdk_game.control_set.m_flcontrol_down = true;
            }
            break;
        case VK_SPACE:
            sdk_game.control_set.m_flcontrol_space = true;
            break;
        case VK_RIGHT:
            if (!(HIWORD(lParam) & KF_REPEAT)) {
                if (!sdk_game.m_flmoveright_block)
                     sdk_game.control_set.m_flcontrol_right = true;
            }
            break;
        case VK_RETURN:
            if (!sdk_game.m_flreturn_block)
                sdk_game.m_flfallboost = true;
            break;
        case VK_LEFT:
            if (!(HIWORD(lParam) & KF_REPEAT)) {
               if (!sdk_game.m_flmoveleft_block)
                    sdk_game.control_set.m_flcontrol_left = true;
            }
            break;
        }
        break;
    case WM_LBUTTONDOWN: {
        sdk_game.control_set.m_flcontrol_mouse_x = LOWORD(lParam);
        sdk_game.control_set.m_flcontrol_mouse_y = HIWORD(lParam);
        }
        break;
    case WM_PAINT:
        render->fixed_wmpaint(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
