#pragma once

class ValueChanger
{
public:
	ValueChanger()
	{
		value = nullptr;
	}

	void Change(int x);
	static int calc_x(double value, int type);

	void on(int _type, double* _value)
	{
		this->type = _type;
		this->value = _value;
	}

	void off()
	{
		this->type = 0;
		this->value = nullptr;
	}

	//0为关闭，1为振幅，2为频率
	int type;
	double * value;
};

class FineTurning
{
public:
	FineTurning()
	{
		value = nullptr;
	}

	void on(int x, double* _value)
	{
		last_x = x;
		this->value = _value;
	}

	void off()
	{
		value = nullptr;
	}

	void Change(int x)
	{
		if (value == nullptr)
			return;
		*value *= exp((x - last_x) / 100000.0);
		last_x = x;
	}

	int last_x;
	double * value;
};

extern FineTurning fineTurning;
extern ValueChanger valueChanger;
