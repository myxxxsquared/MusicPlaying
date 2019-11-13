#pragma once

class SoundGenerator;

extern HINSTANCE hInst;
extern HWND hWnd;
extern SoundGenerator* generator;
extern HFONT hFontStatus;

inline int __calc_line_y(int& _line_index)
{
	return 10 + (_line_index++) * 24;
}

inline int __calc_y_line(int _y)
{
	return (_y - 10) / 24;
}