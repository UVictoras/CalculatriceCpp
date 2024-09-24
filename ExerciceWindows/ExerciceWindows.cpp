// ExerciceWindows.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include <iostream>
#include "ExerciceWindows.h"
#include <vector>
#include <string>
#include <strsafe.h>

#define MAX_LOADSTRING 100

#define ID_BUTTON_0 0
#define ID_BUTTON_1 1
#define ID_BUTTON_2 2
#define ID_BUTTON_3 3
#define ID_BUTTON_4 4
#define ID_BUTTON_5 5
#define ID_BUTTON_6 6
#define ID_BUTTON_7 7
#define ID_BUTTON_8 8
#define ID_BUTTON_9 9

#define ID_BUTTON_PLUS 10
#define ID_BUTTON_MINUS 11
#define ID_BUTTON_DIVIDE 12
#define ID_BUTTON_MULTIPLY 13
#define ID_BUTTON_EQUAL 14
#define ID_BUTTON_INVERT 15
#define ID_BUTTON_SQUARE 16
#define ID_BUTTON_ROOT 17
#define ID_BUTTON_DECIMAL 18
#define ID_BUTTON_RESET 19

// Variables globales :
std::vector<HWND*> hWndButton;
HWND hWndInput;
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

float currentInput;
float lastInput;
float resultInput;
char chosenOperation;
bool hasChosenOperation;
bool isDecimal;
int currentDecimal;

// Déclarations anticipées des fonctions incluses dans ce module de code :
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

    // TODO: Placez le code ici.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EXERCICEWINDOWS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXERCICEWINDOWS));

    MSG msg;

    // Boucle de messages principale :
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

void CreateButton(HWND hwnd, LPCWSTR name, int xPos, int yPos, int idButton)
{
    HWND tempHWnd = CreateWindow( 
        L"BUTTON",  // Predefined class; Unicode assumed 
        name,      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles  
        xPos,         // x position 
        yPos,         // y position 
        100,        // Button width
        100,        // Button height
        hwnd,     // Parent window
        (HMENU)idButton,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    hWndButton.push_back(&tempHWnd); 
}

void CreateInput(HWND hwnd)
{
    hWndInput = CreateWindow(
        L"STATIC",  // Predefined class; Unicode assumed 
        L"CALCULATOR PEOPLE ARE GOOD",      // Button text 
        WS_VISIBLE | WS_CHILD | ES_RIGHT,  // Styles    
        10,         // x position 
        10,         // y position 
        400,        // Button width
        100,        // Button height
        hwnd,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.
}

//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXERCICEWINDOWS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EXERCICEWINDOWS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    currentInput = 0.0;
    lastInput = 0.0;
    resultInput = 0.0;
    chosenOperation = ' ';
    hasChosenOperation = false;
    isDecimal = false;
    currentDecimal = 1;

   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 440, 680, nullptr, nullptr, hInstance, nullptr);

   CreateInput(hWnd);

   CreateButton(hWnd, L"1/x", 10, 110, ID_BUTTON_INVERT); 
   CreateButton(hWnd, L"x²", 110, 110, ID_BUTTON_SQUARE); 
   CreateButton(hWnd, L"sqrt(x)", 210, 110, ID_BUTTON_ROOT); 
   CreateButton(hWnd, L"/", 310, 110, ID_BUTTON_DIVIDE);
   CreateButton(hWnd, L"*", 310, 210, ID_BUTTON_MULTIPLY); 
   CreateButton(hWnd, L"-", 310, 310, ID_BUTTON_MINUS); 
   CreateButton(hWnd, L"+", 310, 410, ID_BUTTON_PLUS); 
   CreateButton(hWnd, L"CE", 10, 510, ID_BUTTON_RESET); 
   CreateButton(hWnd, L",", 210, 510, ID_BUTTON_DECIMAL); 
   CreateButton(hWnd, L"=", 310, 510, ID_BUTTON_EQUAL);

   CreateButton(hWnd, L"7", 10, 210, ID_BUTTON_7); 
   CreateButton(hWnd, L"8", 110, 210, ID_BUTTON_8); 
   CreateButton(hWnd, L"9", 210, 210, ID_BUTTON_9); 
   CreateButton(hWnd, L"4", 10, 310, ID_BUTTON_4); 
   CreateButton(hWnd, L"5", 110, 310, ID_BUTTON_5); 
   CreateButton(hWnd, L"6", 210, 310, ID_BUTTON_6); 
   CreateButton(hWnd, L"1", 10, 410, ID_BUTTON_1); 
   CreateButton(hWnd, L"2", 110, 410, ID_BUTTON_2); 
   CreateButton(hWnd, L"3", 210, 410, ID_BUTTON_3); 
   CreateButton(hWnd, L"0", 110, 510, ID_BUTTON_0); 

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            std::wstring wstr = std::to_wstring(chosenOperation);
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case ID_BUTTON_0: 
            case ID_BUTTON_1:
            case ID_BUTTON_2:
            case ID_BUTTON_3:
            case ID_BUTTON_4:
            case ID_BUTTON_5:
            case ID_BUTTON_6:
            case ID_BUTTON_7:
            case ID_BUTTON_8:
            case ID_BUTTON_9:
                if (wmEvent == BN_CLICKED)
                {
                    if (isDecimal == false)
                    {
                        currentInput = currentInput * 10 + wmId;
                    }
                    else
                    {
                        currentInput = currentInput + wmId * powf(10.0,-currentDecimal);
                        currentDecimal++;
                    }
                    wstr = std::to_wstring(currentInput);
                    SetWindowText(hWndInput, wstr.c_str());
                }
                break;
            case ID_BUTTON_PLUS:
                if (hasChosenOperation == false)
                {
                    lastInput = currentInput;
                    currentInput = 0.0;
                }
                chosenOperation = '+';

                hasChosenOperation = true;
                isDecimal = false;
                currentDecimal = 1;
                break;
            case ID_BUTTON_MINUS: 
                if (hasChosenOperation == false)
                {
                    lastInput = currentInput;
                    currentInput = 0.0;
                }
                chosenOperation = '-';

                hasChosenOperation = true;
                isDecimal = false;
                currentDecimal = 1;
                break;
            case ID_BUTTON_MULTIPLY: 
                if (hasChosenOperation == false)
                {
                    lastInput = currentInput;
                    currentInput = 0.0;
                }
                chosenOperation = '*';

                hasChosenOperation = true;
                isDecimal = false;
                currentDecimal = 1;
                break;
            case ID_BUTTON_DIVIDE: 
                if (hasChosenOperation == false)
                {
                    lastInput = currentInput;
                    currentInput = 0.0;
                }
                chosenOperation = '/'; 

                hasChosenOperation = true;
                isDecimal = false;
                currentDecimal = 1;
                break;
            case ID_BUTTON_INVERT:
                if (hasChosenOperation == false)
                {
                    lastInput = currentInput;
                    currentInput = 0.0;
                }
                chosenOperation = 'i';

                hasChosenOperation = true;
                isDecimal = false;
                currentDecimal = 1;
                break;
            case ID_BUTTON_SQUARE:
                if (hasChosenOperation == false)
                {
                    lastInput = currentInput;
                    currentInput = 0.0;
                }
                chosenOperation = '^';

                hasChosenOperation = true;
                isDecimal = false;
                currentDecimal = 1;
                break;
            case ID_BUTTON_ROOT:
                if (hasChosenOperation == false)
                {
                    lastInput = currentInput;
                    currentInput = 0.0;
                }
                chosenOperation = 'r';

                hasChosenOperation = true;
                isDecimal = false;
                currentDecimal = 1;
                break;
            case ID_BUTTON_DECIMAL:
                isDecimal = true;
                break;
            case ID_BUTTON_RESET:
                currentInput = 0.0;
                lastInput = 0.0;
                chosenOperation = ' ';
                resultInput = 0.0;
                isDecimal = false;
                currentDecimal = 1;
                hasChosenOperation = false;

                wstr = std::to_wstring(currentInput);
                SetWindowText(hWndInput, wstr.c_str());
                break;
            case ID_BUTTON_EQUAL:
                switch (chosenOperation)
                {
                case '+':
                    resultInput = lastInput + currentInput;
                    break;
                case '-':
                    resultInput = lastInput - currentInput;
                    break;
                case '*':
                    resultInput = lastInput * currentInput;
                    break;
                case '/':
                    resultInput = lastInput / currentInput;
                    break;
                case 'i':
                    if (currentInput != 0.0)
                        resultInput = 1.0 / currentInput;
                    else
                        resultInput = 1.0 / lastInput;
                    break;
                case '^':
                    resultInput = lastInput * lastInput;
                    break;
                case 'r':
                    if (lastInput != 0.0)
                        resultInput = sqrtf(lastInput);
                    else
                        resultInput = sqrtf(currentInput);
                    break;
                }
                isDecimal = false;
                currentDecimal = 1;
                currentInput = 0.0;
                lastInput = 0.0;
                chosenOperation = ' ';
                hasChosenOperation = false;
                wstr = std::to_wstring(resultInput);
                SetWindowText(hWndInput, wstr.c_str());
                break;
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Ajoutez ici le code de dessin qui utilise hdc...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
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
