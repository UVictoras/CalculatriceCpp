// ExerciceDessin.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "ExerciceDessin.h"
#include <windef.h>
#include <wingdi.h>
#include <stdio.h>
#include <iostream>
#include <random>

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale
HDC hWndDC;
HWND hWnd;
BOOL fDraw = FALSE;
POINT ptPrevious = { 0.0, 0.0 };
RECT testRect;
HBITMAP hBmp;
HBITMAP hBmpC;
HBITMAP hBmpP;
HDC tempDC;

BITMAPFILEHEADER bmpFileHeader;
BITMAPINFOHEADER bmpInfoHeader;
BITMAPFILEHEADER bmpFileHeaderC;
BITMAPINFOHEADER bmpInfoHeaderC;
BITMAPFILEHEADER bmpFileHeaderP;
BITMAPINFOHEADER bmpInfoHeaderP;

struct Pixel
{
    int x;
    int y;
    COLORREF color;
};

long int currentPixel = 0;
Pixel* pixels;

long int yJ = 0;
long int yC = -2576;
long int yP = 2576;

void ShufflePixels()
{
    std::default_random_engine rng(static_cast<unsigned int>(std::time(nullptr)));

    std::shuffle(pixels, pixels + bmpInfoHeader.biWidth * bmpInfoHeader.biHeight, rng);
}

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
    LoadStringW(hInstance, IDC_EXERCICEDESSIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXERCICEDESSIN));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXERCICEDESSIN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EXERCICEDESSIN);
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
    hInst = hInstance;

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 1920, 1080, nullptr, nullptr, hInstance, nullptr);

    hWndDC = GetDC(hWnd);

    testRect.left = 100;
    testRect.top = 100;
    testRect.right = 1000;
    testRect.bottom = 1000;

    FILE* file;
    errno_t err = fopen_s(&file, "Image/affiche_jaune.bmp", "rb");
    if (err != 0 || file == nullptr) {
        std::cerr << "Error opening image file: " << "Image/affiche_jaune.bmp" << std::endl;
        return FALSE;
    }

    fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);

    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    BYTE* bgr = new BYTE[bmpInfoHeader.biSizeImage];

    fseek(file, bmpFileHeader.bfOffBits, SEEK_SET); 
    fread(bgr, 1, bmpInfoHeader.biSizeImage, file); 

    fclose(file);

    hBmp = CreateDIBitmap(hWndDC, &bmpInfoHeader, CBM_INIT, bgr, (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS);



    err = fopen_s(&file, "Image/affiche_cyan.bmp", "rb");
    if (err != 0 || file == nullptr) {
        std::cerr << "Error opening image file: " << "Image/affiche_cyan.bmp" << std::endl;
        return FALSE;
    }

    fread(&bmpFileHeaderC, sizeof(BITMAPFILEHEADER), 1, file);

    fread(&bmpInfoHeaderC, sizeof(BITMAPINFOHEADER), 1, file);

    bgr = new BYTE[bmpInfoHeaderC.biSizeImage];

    fseek(file, bmpFileHeaderC.bfOffBits, SEEK_SET);
    fread(bgr, 1, bmpInfoHeaderC.biSizeImage, file);

    fclose(file);

    hBmpC = CreateDIBitmap(hWndDC, &bmpInfoHeaderC, CBM_INIT, bgr, (BITMAPINFO*)&bmpInfoHeaderC, DIB_RGB_COLORS);



    err = fopen_s(&file, "Image/affiche_violette.bmp", "rb");
    if (err != 0 || file == nullptr) {
        std::cerr << "Error opening image file: " << "Image/affiche_violette.bmp" << std::endl;
        return FALSE;
    }

    fread(&bmpFileHeaderP, sizeof(BITMAPFILEHEADER), 1, file);

    fread(&bmpInfoHeaderP, sizeof(BITMAPINFOHEADER), 1, file);

    bgr = new BYTE[bmpInfoHeaderP.biSizeImage];

    fseek(file, bmpFileHeaderP.bfOffBits, SEEK_SET);
    fread(bgr, 1, bmpInfoHeaderP.biSizeImage, file);

    fclose(file);

    hBmpP = CreateDIBitmap(hWndDC, &bmpInfoHeaderP, CBM_INIT, bgr, (BITMAPINFO*)&bmpInfoHeaderP, DIB_RGB_COLORS);

    tempDC = CreateCompatibleDC(hWndDC);
    SelectObject(tempDC, hBmp);

    pixels = new Pixel[bmpInfoHeader.biHeight * bmpInfoHeader.biWidth];

    for (int y = 0; y < bmpInfoHeader.biHeight; y++)
    {
        for (int x = 0; x < bmpInfoHeader.biWidth; x++)
        {
            pixels[y * bmpInfoHeader.biWidth + x].x = x;
            pixels[y * bmpInfoHeader.biWidth + x].y = y;
            pixels[y * bmpInfoHeader.biWidth + x].color = GetPixel(tempDC, x, y);
        }
    }

    ShufflePixels();

    if (!hBmp) {
        std::cerr << "Error creating the bitmap!" << std::endl;
        delete[] bgr;
        return FALSE;
    }

    delete[] bgr;

    if (!hWnd) {
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
    case WM_LBUTTONDOWN:
        fDraw = TRUE;
        ptPrevious.x = LOWORD(lParam);
        ptPrevious.y = HIWORD(lParam);
        return 0L;

    case WM_LBUTTONUP:
        if (fDraw)
        {
            hWndDC = GetDC(hWnd);
            MoveToEx(hWndDC, ptPrevious.x, ptPrevious.y, NULL);
            LineTo(hWndDC, LOWORD(lParam), HIWORD(lParam));
            ReleaseDC(hWnd, hWndDC);
        }
        fDraw = FALSE;
        return 0L;

    case WM_MOUSEMOVE:
        if (fDraw)
        {
            hWndDC = GetDC(hWnd);
            MoveToEx(hWndDC, ptPrevious.x, ptPrevious.y, NULL);
            LineTo(hWndDC, ptPrevious.x = LOWORD(lParam),
                ptPrevious.y = HIWORD(lParam));
            ReleaseDC(hWnd, hWndDC);
        }
        return 0L;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawText(hdc, L"JOHAAAAAAANN", 15, &testRect, DT_CENTER);
            // TODO: Ajoutez ici le code de dessin qui utilise hdc...

            //BitBlt(hWndDC, 0, 0, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight, tempDC, 0, 0, SRCCOPY); 

            /*for (int i = 0; i < bmpInfoHeader.biWidth * bmpInfoHeader.biHeight; i++)
            {
                SetPixel(hWndDC, pixels[i].x, pixels[i].y, pixels[i].color);
            }*/

            SetTimer(hWnd, NULL, 1, NULL);

            EndPaint(hWnd, &ps);


        }
        break;
    case WM_TIMER:
        /*if (currentPixel < bmpInfoHeader.biWidth * bmpInfoHeader.biHeight)
        {
            for (int i = 0; i < 1000; i++)
            {
                SetPixel(hWndDC, pixels[currentPixel].x, pixels[currentPixel].y, pixels[currentPixel].color);
                currentPixel++;
                if (currentPixel > bmpInfoHeader.biWidth * bmpInfoHeader.biHeight - 1) break;
            }
        }
        currentPixel++;*/

        SelectObject(tempDC, hBmp);
        BitBlt(hWndDC, 0, yJ, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight, tempDC, 0, 0, SRCCOPY);
        if (yJ > -5152)
            yJ -= 4;
        else
            yJ = 2576;

        SelectObject(tempDC, hBmpC);
        BitBlt(hWndDC, 0, yC, bmpInfoHeaderC.biWidth, bmpInfoHeaderC.biHeight, tempDC, 0, 0, SRCCOPY);
        if (yC > -5152)
            yC -= 4;
        else
            yC = 2576;

        SelectObject(tempDC, hBmpP);
        BitBlt(hWndDC, 0, yP, bmpInfoHeaderP.biWidth, bmpInfoHeaderP.biHeight, tempDC, 0, 0, SRCCOPY);
        if (yP > -5152)
            yP -= 4;
        else
            yP = 2576;

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
