#include "stdafx.h"
#include "SoundGenerator.h"
#include "Changer.h"

void OnPaint(HDC hdc, PAINTSTRUCT&);

void OnLButtonDonw(int x, int y)
{
	if (x <= 10)
	{
		int line = __calc_y_line(y);
		if (line == 0)
			generator->GetSound().push_back(Sound());
		else
		{
			int _index = (line - 2) / 4;
			int _item = line - 2 - _index * 4;
			if (_index >= generator->GetSound().size())
				return;
			if (_item == 0)
			{
				auto& gen = generator->GetSound();
				gen.erase(gen.begin() + _index);
			}
		}
	}
	else
	{
		int line = __calc_y_line(y);
		if (line == 0)
		{
			if (generator->IsPaused())
				generator->resume();
			else
				generator->pause();
		}
		else
		{
			int _index = (line - 2) / 4;
			int _item = line - 2 - _index * 4;

			if (_index >= generator->GetSound().size())
				return;

			auto& item = generator->GetSound()[_index];

			switch (_item)
			{
			case 0:
				item.type = (Sound::Type)((item.type + 1) % Sound::__Last);
				break;
			case 1:
				valueChanger.on(1, &item.amplitude);
				valueChanger.Change(x);
				break;
			case 2:
				valueChanger.on(2, &item.frequency);
				valueChanger.Change(x);
				break;
			}
		}
	}

	InvalidateRect(hWnd, NULL, FALSE);
}

void OnRButtonDonw(int x, int y)
{
	int line = __calc_y_line(y);
	if (line == 0)
	{
		return;
	}
	else
	{
		int _index = (line - 2) / 4;
		int _item = line - 2 - _index * 4;

		if (_index >= generator->GetSound().size())
			return;

		auto& item = generator->GetSound()[_index];

		switch (_item)
		{
		case 0:
			break;
		case 1:
			fineTurning.on(x, &item.amplitude);
			break;
		case 2:
			fineTurning.on(x, &item.frequency);
			break;
		}
	}

	InvalidateRect(hWnd, NULL, FALSE);
}

void OnRButtonUp(int x, int y)
{
	x; y;
	fineTurning.off();
}


void OnLButtonUp(int x, int y)
{
	x; y;
	valueChanger.off();
	InvalidateRect(hWnd, NULL, FALSE);
}

void OnMouseMove(int x, int y)
{
	y;
	valueChanger.Change(x);
	fineTurning.Change(x);
	InvalidateRect(hWnd, NULL, FALSE);
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			rect.right = rect.right - rect.left;
			rect.bottom = rect.bottom - rect.top;
			HBRUSH brush_bakcgournd = CreateSolidBrush(RGB(255, 255, 255));
			HDC hdcmem = CreateCompatibleDC(hdc);
			HBITMAP hbitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			HGDIOBJ oldbmp = SelectObject(hdcmem, hbitmap);
			FillRect(hdcmem, &rect, brush_bakcgournd);
			OnPaint(hdcmem, ps);
			BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcmem, 0, 0, SRCCOPY);
			SelectObject(hdcmem, oldbmp);
			DeleteObject(hbitmap);
			DeleteObject(hdcmem);
			DeleteObject(brush_bakcgournd);
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDonw(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDonw(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		OnRButtonUp(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(_hwnd, message, wParam, lParam);
	}
	return 0;
}
