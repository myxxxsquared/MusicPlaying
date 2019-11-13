#include "stdafx.h"
#include "Changer.h"

FineTurning fineTurning;
ValueChanger valueChanger;

void ValueChanger::Change(int x)
{
	if (x < 300)
		x = 300;
	if (x > 1000)
		x = 1000;

	x -= 300;

	if (type == 0)
	{
		return;
	}
	else if (type == 1)
	{
		*value = 32767.0 / 700 * (double)x;
	}
	else if (type == 2)
	{
		double a = exp((double)x / 700);
		*value = 11627.90697674418604651*a - 11607.90697674418604651;
	}
}


int ValueChanger::calc_x(double value, int type)
{
	if (type == 0)
		return 0;
	else if (type == 1)
	{
		return (int)(value / 32767.0 * 700 + 300);
	}
	else if (type == 2)
	{
		return (int)(log((value + 11607.90697674418604651) / 11627.90697674418604651) * 700 + 300);
	}
	return 0;
}