#include "stdafx.h"
#include "SoundGenerator.h"

#define __PI 3.14159265358979323846

class StopRunningException: public std::exception {
	int code;
public:
	StopRunningException(int code)
	{
		this->code = code;
	}

	int GetCode()
	{
		return code;
	}
};

SoundGenerator::SoundGenerator(DWORD SamplesPerSec)
{
	format.nSamplesPerSec = SamplesPerSec;
	format.nChannels = 1;
	format.wBitsPerSample = 16;

	format.nBlockAlign = 2;
	format.nAvgBytesPerSec = format.nBlockAlign * SamplesPerSec;

	format.cbSize = 0;
	format.wFormatTag = WAVE_FORMAT_PCM;

	MMRESULT result;
	result = waveOutOpen((LPHWAVEOUT)&hWaveOut, WAVE_MAPPER, (LPCWAVEFORMATEX)&format, 0, 0, CALLBACK_NULL);
	if (result != MMSYSERR_NOERROR)
		throw WaveException(std::string("waveOutOpen"), result);

	hThread = CreateThread(NULL, NULL, ThreadProc, this, CREATE_SUSPENDED, NULL);
	if (NULL == hThread)
	{
		waveOutClose(hWaveOut);
		throw WaveException(std::string("CreateThread"), GetLastError());
	}

	running = false;
	stop_signal = false;
	stoped_signal = false;
}

SoundGenerator::~SoundGenerator()
{
	if (running)
	{
		Stop();
	}
	waveOutClose(hWaveOut);
	CloseHandle(hThread);
}

void SoundGenerator::Start()
{
	if (stop_signal)
		throw std::logic_error("无法启动已停止或正在停止的播放器。");

	if (running)
		return;

	if (-1 == ResumeThread(hThread))
		throw WaveException(std::string("ResumeThread"), GetLastError());

	running = true;
}

void SoundGenerator::Stop()
{
	if (running == false)
		return;

	stop_signal = true;
	while (!stoped_signal)
		Sleep(0);
	running = false;
}

DWORD SoundGenerator::ThreadProc(LPVOID lpParameter)
{
	auto obj = ((SoundGenerator*)lpParameter);
	try
	{
		obj->RunningThread();
	}
	catch(StopRunningException e)
	{
		obj->stoped_signal = true;
		return e.GetCode();
	}
	return 0;
}

#define SAMPLE_PER_OUTPUT	10000
void SoundGenerator::RunningThread()
{
	const int totalLength = 2 * SAMPLE_PER_OUTPUT;
	const double _delta = 2 * __PI / format.nSamplesPerSec;

	WAVEHDR header1, header2;
	ZeroMemory(&header1, sizeof(WAVEHDR));
	ZeroMemory(&header2, sizeof(WAVEHDR));

	short* block1((short*)malloc(totalLength));
	short* block2((short*)malloc(totalLength));

	header1.dwBufferLength = totalLength;
	header1.lpData = (LPSTR)(short*)block1;
	header2.dwBufferLength = totalLength;
	header2.lpData = (LPSTR)(short*)block2;

	MMRESULT result;
	for (;;)
	{
		while (!stop_signal && paused)
			Sleep(0);

		if (stop_signal)
		{
			while (waveOutUnprepareHeader(hWaveOut, &header1, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
			while (waveOutUnprepareHeader(hWaveOut, &header2, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
			free(block1);
			free(block2);
			throw StopRunningException(0);
		}

		CalcSound(block1, SAMPLE_PER_OUTPUT, _delta);
		while (waveOutUnprepareHeader(hWaveOut, &header1, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
		result = waveOutPrepareHeader(hWaveOut, &header1, sizeof(WAVEHDR));
		if (MMSYSERR_NOERROR != result)
			throw WaveException(std::string("waveOutPrepareHeader"), result);
		result = waveOutWrite(hWaveOut, &header1, sizeof(WAVEHDR));
		if (MMSYSERR_NOERROR != result)
			throw WaveException(std::string("waveOutWrite"), result);

		CalcSound(block2, SAMPLE_PER_OUTPUT, _delta);
		while (waveOutUnprepareHeader(hWaveOut, &header2, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
		result = waveOutPrepareHeader(hWaveOut, &header2, sizeof(WAVEHDR));
		if (MMSYSERR_NOERROR != result)
			throw WaveException(std::string("waveOutPrepareHeader"), result);
		result = waveOutWrite(hWaveOut, &header2, sizeof(WAVEHDR));
		if (MMSYSERR_NOERROR != result)
			throw WaveException(std::string("waveOutWrite"), result);
	}
}

void SoundGenerator::CalcSound(short * buffer, int count, const double _delta)
{
	for (int i = 0; i < count; i++)
	{
		double complex = 0.0;
		try
		{
			for (auto item = sound.begin(); item != sound.end(); item++)
			{
				item->phi += _delta*item->frequency;
				if (item->phi > 2 * __PI)
					item->phi -= 2 * __PI;
				switch (item->type)
				{
				case Sound::Cos:
					complex += item->amplitude * cos(item->phi);
					break;
				case Sound::Square:
					if (item->phi < __PI)
						complex += item->amplitude;
					else
						complex -= item->amplitude;
					break;
				case Sound::Sawtooth:
					complex += item->amplitude * item->phi / (2 * __PI);
					break;
				case Sound::Line:
					complex += item->amplitude;
				}
			}
		}
		catch (...) {}

		buffer[i] = (short)complex;
	}
}
