#include "stdafx.h"
#include "SoundGenerator.h"
#include "Changer.h"

void OnPaint(HDC hdc, PAINTSTRUCT&)
{
	if (generator == nullptr)
		return;

	int _line_index = 0;

	SelectObject(hdc, hFontStatus);
	char text[1024];

	sprintf_s(text, "Running: %s", generator->IsPaused() ? "false" : "true");
	TextOutA(hdc, 10, __calc_line_y(_line_index), text, (int)strlen(text));

	for each (auto var in generator->GetSound())
	{
		_line_index++;
		switch (var.type)
		{
		case Sound::Cos:
			sprintf_s(text, "Wave Type:Cos");
			break;
		case Sound::Line:
			sprintf_s(text, "Wave Type:Line");
			break;
		case Sound::Sawtooth:
			sprintf_s(text, "Wave Type:Sawtooth");
			break;
		case Sound::Square:
			sprintf_s(text, "Wave Type:Square");
			break;
		}
		TextOutA(hdc, 10, __calc_line_y(_line_index), text, (int)strlen(text));

		int __y, __x;
		__x = ValueChanger::calc_x(var.amplitude, 1);
		__y = __calc_line_y(_line_index);
		sprintf_s(text, "Amplitude:%lf", var.amplitude);
		TextOutA(hdc, 10, __y, text, (int)strlen(text));
		MoveToEx(hdc, 300, __y + 12, NULL);
		LineTo(hdc, 1000, __y + 12);
		MoveToEx(hdc, __x, __y, NULL);
		LineTo(hdc, __x, __y + 24);

		__y = __calc_line_y(_line_index);
		__x = ValueChanger::calc_x(var.frequency, 2);
		sprintf_s(text, "Frequency:%lf", var.frequency);
		TextOutA(hdc, 10, __y, text, (int)strlen(text));
		MoveToEx(hdc, 300, __y + 12, NULL);
		LineTo(hdc, 1000, __y + 12);
		MoveToEx(hdc, __x, __y, NULL);
		LineTo(hdc, __x, __y + 24);
	}
}
