/* BASS Multiple output example, copyright (c) 2001 Ian Luck.
========================================================================
Demonstrates how you can use LoadLibrary and GetProcAddress to import
multiple copies of BASS for multiple output devices.
Other source: multi.rc
Imports: kernel32.lib, user32.lib comdlg32.lib
*/

#include <windows.h>
#include <stdio.h>
#include "multi.h"

#define devices 2
#define BASSDEF(f) (WINAPI *f[devices])	// define the functions as pointers
#include "bass.h"

char bassfile[devices-1][MAX_PATH];	// temporary BASS.DLL files
HINSTANCE bass[devices]={0};		// bass handles

/* load BASS[n] and the required functions */
void LoadBASS(int n)
{
	void *data;
	HANDLE hfile;
	DWORD len,c;
	char temppath[MAX_PATH],tempfile[MAX_PATH];

	if (n) {
		if (!SearchPath(NULL,"bass.dll",NULL,MAX_PATH,tempfile,&c)
 		 || INVALID_HANDLE_VALUE==(hfile=CreateFile(tempfile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL))) {
			MessageBox(0,"Error: Can't open BASS.DLL\n","",0);
			ExitProcess(0);
		}
		len=SetFilePointer(hfile,0,NULL,FILE_END);
		data=malloc(len);
		SetFilePointer(hfile,0,NULL,FILE_BEGIN);
		ReadFile(hfile,data,len,&c,NULL);
		CloseHandle(hfile);
  
		/* get a temporary filename */
		GetTempPath(MAX_PATH,temppath);
		GetTempFileName(temppath,"bas",0,tempfile);
		/* write BASS.DLL to the temporary file */
		if (INVALID_HANDLE_VALUE==(hfile=CreateFile(tempfile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_TEMPORARY,NULL))) {
			MessageBox(0,"Error: Can't write BASS.DLL\n","",0);
			ExitProcess(0);
		}
		WriteFile(hfile,data,len,&c,NULL);
		CloseHandle(hfile);
		strcpy(bassfile[n-1],tempfile);
	} else
		strcpy(tempfile,"bass.dll");

	/* load the BASS.DLL library */
	if (!(bass[n]=LoadLibrary(tempfile))) {
		MessageBox(0,"Error: Can't load BASS.DLL\n","",0);
		ExitProcess(0);
	}
	/* load all the BASS functions that are to be used */
#ifdef __cplusplus
	#define LOADBASSFUNCTION(f) {\
		char *func=#f;\
		__asm {push func}\
		__asm {push bass[n]}\
		__asm {mov eax,GetProcAddress}\
		__asm {call eax}\
		__asm {mov f[n],eax}\
	}
#else
	#define LOADBASSFUNCTION(f) (FARPROC)f[n]=GetProcAddress(bass[n],#f)
#endif
	LOADBASSFUNCTION(BASS_GetVersion);
	LOADBASSFUNCTION(BASS_ErrorGetCode);
	LOADBASSFUNCTION(BASS_GetDeviceDescription);
	LOADBASSFUNCTION(BASS_Init);
	LOADBASSFUNCTION(BASS_Free);
	LOADBASSFUNCTION(BASS_Start);
	LOADBASSFUNCTION(BASS_StreamCreateFile);
	LOADBASSFUNCTION(BASS_StreamPlay);
	LOADBASSFUNCTION(BASS_StreamFree);
}

/* free the BASS library from memory and delete the temporary files */
void FreeBASS()
{
	int c;
	for (c=0;c<devices;c++) {
		if (bass[c]) {
			FreeLibrary(bass[c]);
			bass[c]=0;
			if (c) DeleteFile(bassfile[c-1]);
		}
	}
}


static HWND win=NULL;
static HINSTANCE inst;
static BOOL noogg=FALSE;

static int outdev[2];	// output devices
static HSTREAM chan[2];	// the streams (wav/mp3)

/* display error messages */
static void Error(int n,char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error %d code: %d)",es,n,BASS_ErrorGetCode[n]());
	MessageBox(win,mes,"Error",0);
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)w,(LPARAM)l)

BOOL CALLBACK dialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	static OPENFILENAME ofn;
	static char path[MAX_PATH];

	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case IDCANCEL:
					DestroyWindow(h);
					return 1;
				case ID_OPEN:
					{
						char file[MAX_PATH]="";
						ofn.lpstrFilter="streamable files\0*.mp3;*.wav\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							memcpy(path,file,ofn.nFileOffset);
							path[ofn.nFileOffset-1]=0;
							BASS_StreamFree[0](chan[0]);
							if (!(chan[0]=BASS_StreamCreateFile[0](FALSE,file,0,0,0))) {
								MESS(ID_OPEN,WM_SETTEXT,0,"click here to open a file...");
								Error(0,"Can't play the file");
								break;
							}
							MESS(ID_OPEN,WM_SETTEXT,0,file);
							BASS_StreamPlay[0](chan[0],0,BASS_SAMPLE_LOOP);
						}
					}
					return 1;
				case ID_OPEN2:
					{
						char file[MAX_PATH]="";
						if (noogg)
							ofn.lpstrFilter="Streamable files (wav/mp3/mp2/mp1)\0*.wav;*.mp3;*.mp2;*.mp1\0All files\0*.*\0\0";
						else
							ofn.lpstrFilter="Streamable files (wav/mp3/mp2/mp1/ogg)\0*.wav;*.mp3;*.mp2;*.mp1;*.ogg\0All files\0*.*\0\0";
						ofn.lpstrFile=file;
						if (GetOpenFileName(&ofn)) {
							memcpy(path,file,ofn.nFileOffset);
							path[ofn.nFileOffset-1]=0;
							BASS_StreamFree[1](chan[1]);
							if (!(chan[1]=BASS_StreamCreateFile[1](FALSE,file,0,0,0))) {
								MESS(ID_OPEN2,WM_SETTEXT,0,"click here to open a file...");
								Error(0,"Can't play the file");
								break;
							}
							MESS(ID_OPEN2,WM_SETTEXT,0,file);
							BASS_StreamPlay[1](chan[1],0,BASS_SAMPLE_LOOP);
						}
					}
					return 1;
			}
			break;

		case WM_INITDIALOG:
			win=h;
			GetCurrentDirectory(MAX_PATH,path);
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize=sizeof(ofn);
			ofn.hwndOwner=h;
			ofn.hInstance=inst;
			ofn.nMaxFile=MAX_PATH;
			ofn.lpstrInitialDir=path;
			ofn.Flags=OFN_HIDEREADONLY|OFN_EXPLORER;
			// setup output devices
			if (!BASS_Init[0](outdev[0],44100,BASS_DEVICE_OGG|BASS_DEVICE_NOSYNC,win)) {
				noogg=TRUE;	// no OGG support
				if (!BASS_Init[0](outdev[0],44100,BASS_DEVICE_NOSYNC,win)) {
					Error(0,"Can't initialize device");
					DestroyWindow(win);
				}
			}
			BASS_Start[0]();
			if (!BASS_Init[1](outdev[1],44100,BASS_DEVICE_OGG|BASS_DEVICE_NOSYNC,win)) {
				noogg=TRUE;	// no OGG support
				if (!BASS_Init[1](outdev[0],44100,BASS_DEVICE_NOSYNC,win)) {
					Error(1,"Can't initialize device");
					DestroyWindow(win);
				}
			}
			BASS_Start[1]();
			return 1;
	}
	return 0;
}


// Simple device selector dialog stuff begins here
int device;		// selected device

BOOL CALLBACK devicedialogproc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	switch (m) {
		case WM_COMMAND:
			switch (LOWORD(w)) {
				case ID_DEVLIST:
					if (HIWORD(w)==LBN_SELCHANGE)
						device=SendMessage(l,LB_GETCURSEL,0,0);
					else if (HIWORD(w)==LBN_DBLCLK)
						EndDialog(h,0);
					break;
				case IDOK:
					EndDialog(h,0);
					return 1;
			}
			break;

		case WM_INITDIALOG:
			{
				char *d;
				int c;
				for (c=0;BASS_GetDeviceDescription[0](c,&d);c++) {
					SendDlgItemMessage(h,ID_DEVLIST,LB_ADDSTRING,0,d);
				}
				SendDlgItemMessage(h,ID_DEVLIST,LB_SETCURSEL,0,0);
			}
			device=0;
			return 1;
	}
	return 0;
}
// Device selector stuff ends here

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	inst=hInstance;

	/* Load BASS */
	LoadBASS(0);
	LoadBASS(1);
	/* Check that BASS 1.2 was loaded */
	if (BASS_GetVersion[0]()!=MAKELONG(1,2)
		|| BASS_GetVersion[1]()!=MAKELONG(1,2)) {
		MessageBox(0,"BASS version 1.2 was not loaded","Incorrect BASS.DLL",0);
		FreeBASS();
		return 0;
	}

	/* Let the user choose an output device */
	MessageBox(0,"Select output device #1","",MB_OK|MB_ICONQUESTION);
	DialogBox(inst,2000,win,&devicedialogproc);
	outdev[0]=device;
	MessageBox(0,"Select output device #2","",MB_OK|MB_ICONQUESTION);
	DialogBox(inst,2000,win,&devicedialogproc);
	outdev[1]=device;

	/* main dialog */
	DialogBox(inst,1000,0,&dialogproc);

	BASS_Free[0]();
	BASS_Free[1]();
	FreeBASS();

	return 0;
}
