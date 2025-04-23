// ghost.cpp : Defines the entry point for the DLL application.
//

// dll 
#include <windows.h>
#include <ghost.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	// tutaj info np do pliku textowego
	
	switch (ul_reason_for_call) 
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

    return TRUE;
}

