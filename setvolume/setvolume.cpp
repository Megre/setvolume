// setvolume.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "setvolume.h"

void printUsage() {
	string usage = "Usage:\n";
	usage += "setvolume [get | set] [volume] [mute]\n";
	usage += "    get: return current volume status\n";
	usage += "    volume: set volume value ([0, 100])\n";
	usage += "    mute: set mute status (0 to maintain, -1 to mute, 1 to speak)";
	cout << usage << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(!checkParam(argc, argv)) {
		return 1;
	}

	int vol;
	if(StrCmpW(L"get", argv[1]) == 0) {
		BOOL mute;
		if(!getVolume(&vol, &mute)) return 1;

		cout << vol << ", " << (mute?"mute":"speaking") << endl;
	}
	else { // set [volume] [mute]
		// volume
		vol = strToInt(argv[2]);
		if(vol < 0 || vol > 100) {
			printUsage();
			return 1;
		}

		// mute
		int mute = 0;
		if(argc > 3) mute = strToInt(argv[3]);
	
		if(!setVolume(vol, mute)) return 1;
	}

	return 0;
}

/**
 * outVolume: 0-100
 */
BOOL getVolume(int *outVolume, BOOL *bMute) {
	HRESULT hResult; 
	IAudioEndpointVolume* epVolume;
	IMMDevice *defaultDevice;

	if(!getDefaultDevice(&defaultDevice, &epVolume)) return FALSE;

	float currentVolume;
	hResult = epVolume->GetMasterVolumeLevelScalar(&currentVolume); 
	if (FAILED(hResult)) return FALSE;
	
	*outVolume = (int) (currentVolume * 100.0f);

	hResult = epVolume->GetMute(bMute);
	if (FAILED(hResult)) return FALSE;

	return TRUE;
}

/**
 * volume: 0-100
 * mute: 0 to maintain current status, -1 to mute, 1 to speak
 */
BOOL setVolume(int volume, int mute) {
	HRESULT hResult;
	IAudioEndpointVolume* epVolume;
	IMMDevice *defaultDevice;

	if(!getDefaultDevice(&defaultDevice, &epVolume)) return FALSE;

	hResult = epVolume->SetMasterVolumeLevelScalar(volume/100.0f, NULL);
	if(FAILED(hResult)) return FALSE;

	if(mute != 0) hResult = epVolume->SetMute(mute < 0, NULL);

	return !FAILED(hResult);
}

BOOL getDefaultDevice(IMMDevice **defaultDevice, IAudioEndpointVolume** epVolume) {
	HRESULT hResult = CoInitialize(NULL);
	if(FAILED(hResult)) return FALSE;

	// default audio endpoint
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hResult = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	hResult = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, defaultDevice);
	if(FAILED(hResult)) return hResult;

	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	hResult = (*defaultDevice)->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, reinterpret_cast<void **>(epVolume));

	return !FAILED(hResult);
}

BOOL checkParam(int argc, _TCHAR* argv[]) {
	if(argc < 2
		|| (StrCmpW(L"get", argv[1]) != 0 && StrCmpW(L"set", argv[1]) != 0)
		|| (StrCmpW(L"set", argv[1]) == 0 && argc < 3)) {
		printUsage();
		return FALSE;
	}

	return TRUE;
}

int strToInt(_TCHAR* str) {
	size_t cCharsConverted;
	char *strTmp = new char[2*(sizeof(str)+1)] ; 
	wcstombs_s(&cCharsConverted, strTmp, sizeof(strTmp), (const wchar_t *)str, sizeof(strTmp));
	int num = atoi(strTmp);
	delete strTmp;
	return num;
}