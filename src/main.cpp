#include "Client.h"

#ifdef _WINDOWS
#define K_WIN32
#include <stdio.h>
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shellapi.h>
#endif
#ifdef K_WIN32
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	{
		int argc;
		char** argv;
		LPWSTR* lpArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
		argv = (char**)malloc(argc * sizeof(char*));
		int size, i = 0;
		for (; i < argc; ++i)
		{
			size = wcslen(lpArgv[i]) + 1;
			argv[i] = static_cast<char*>(malloc(size));
			wcstombs(argv[i], lpArgv[i], size);
		}
		Client client(argv[0]);
		if (client.isInit())
			client.deltaLoop();
		{
			LocalFree(lpArgv);
			int j = 0;
			for (; j < argc; ++j)
				free(argv[j]);
			free(argv);
		}
	}
	return 0;
}
#else
int main(int argc, char** argv)
{
	Client client(argv[0]);
	if(client.isInit())
		return client.deltaLoop();
    return 0;
}
#endif