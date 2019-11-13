#include "stdafx.h"
#include "resource.h"
#include "SoundGenerator.h"

BOOL InitInstance();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int)
{
	hFontStatus = CreateFont(24, NULL, NULL, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, L"Î¢ÈíÑÅºÚ");

	hInst = hInstance;

	if (!InitInstance())
	{
		MessageBox(NULL, TEXT("ÎÞ·¨³õÊ¼»¯³ÌÐò"), NULL, NULL);
		return 1;
	}

	generator = new SoundGenerator();
	generator->Start();

	InvalidateRect(hWnd, NULL, FALSE);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete generator;

	return (int)msg.wParam;
	return 0;
}

BOOL InitInstance()
{
	WNDCLASSEXW wcex;

	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = TEXT("MusicPlaying_yV392FP9");
	wcex.hIconSm = NULL;
	wcex.lpszMenuName = NULL;

	ATOM result = RegisterClassExW(&wcex);
	if (!result)
	{
		return FALSE;
	}

	hWnd = CreateWindowExW(0L, TEXT("MusicPlaying_yV392FP9"), TEXT("ÉùÒô²¥·ÅÆ÷"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	return TRUE;
}




