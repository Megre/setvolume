
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <atlstr.h>
#include <iostream>
#include <string>

using namespace std;

BOOL getVolume(int *outVolume, BOOL *bMute);
BOOL setVolume(int volume, int mute);
BOOL getDefaultDevice(IMMDevice **defaultDevice, IAudioEndpointVolume** epVolume);

BOOL checkParam(int argc, _TCHAR* argv[]);
int strToInt(_TCHAR* str);