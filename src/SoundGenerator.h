#pragma once

class WaveException : public std::exception {
	int code;
	std::string func;
public:
	WaveException(std::string func, int code)
	{
		this->code = code;
		this->func = func;
	}

	int GetCode()
	{
		return code;
	}

	std::string GetFunc()
	{
		return func;
	}
};

struct Sound
{
	enum Type {Cos, Square, Sawtooth, Line, __Last} type;
	double amplitude;
	double frequency;
	double phi;

	Sound()
	{
		type = Cos;
		frequency = 1000;
		phi = 0;
		amplitude = 32767;
	}

	Sound(Type _type, double _frequency, double _phi, double _amplitude)
	{
		this->type = _type;
		this->frequency = _frequency;
		this->phi = _phi;
		this->amplitude = _amplitude;
	}
};

class SoundGenerator
{
public:
	SoundGenerator(DWORD SamplesPerSec = 192000);
	~SoundGenerator();

	SoundGenerator(const SoundGenerator&) = delete;
	SoundGenerator& operator=(const SoundGenerator&) = delete;

	void Start();
	void Stop();

	void pause()
	{
		this->paused = true;
	}

	void resume()
	{
		this->paused = false;
	}

	WAVEFORMATEX GetFormat()
	{
		return *(WAVEFORMATEX*)&format;
	}

	void AddSound(Sound s)
	{
		sound.push_back(s);
	}


	std::vector<Sound> & GetSound()
	{
		return sound;
	}

	bool IsPaused() { return paused; }

private:
	HANDLE hThread;
	volatile WAVEFORMATEX format;
	volatile HWAVEOUT hWaveOut;
	volatile bool running;
	volatile bool stop_signal;
	volatile bool stoped_signal;
	volatile bool paused;

	std::vector<Sound> sound;
	
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	void RunningThread();
	void CalcSound(short* buffer, int count, const double _delta);
};

