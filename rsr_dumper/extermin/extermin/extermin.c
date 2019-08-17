#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <debugapi.h>
#include "addresses.h"
#include "extermin.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
}

// Directory to dump the files to.
static const char *RSR_DUMP_PATH = "C:/tmp/extermination-dump/%s";
static const char *LOG_FILE = "log.txt";

// We create this in our code and assign it back to extermination memory
static HINSTANCE main_hInstance;
static HWND main_hWndParent;


static int ShowMessageBox(const char *msg, const char *title)
{
	int msgboxId = MessageBoxA(NULL,
		msg,
		title,
		MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
	);

	return msgboxId;
}

static int DumpRsrFile()
{
	FILE *f = fopen("log.txt", "w");
	if (f)
	{
		const char *buf = "Starting RSR dump...\r\n";
		fwrite(buf, strlen(buf), 1, f);
	}

	// The game reads the .nam and then the .rsr, no need for an extension
	const char *fname = "extermin";

	EXTERM_CALL_PROC_CHAR(EXTERM_FUNC_READ_NAME_FILE, fname);

	int exterm_nam_filehnd = EXTERM_GLOBAL(EXTERM_MAIN_RSR_NAME_HND, int);
	int exterm_rsr_filehnd = EXTERM_GLOBAL(EXTERM_MAIN_RSR_FILE_HND, int);

	int file_count = EXTERM_GLOBAL(EXTERM_MAIN_RSR_READ_CT, int);

	if (f)
	{
		fprintf(f, "NAM FH: %d\r\n", exterm_nam_filehnd);
		fprintf(f, "RSR FH: %d\r\n", exterm_rsr_filehnd);
		fprintf(f, "Files in RSR: %d\r\n", file_count);
	}

	int ind_size = sizeof(struct rsr_nam_index);

	for (int i = 0; i < file_count; i++)
	{
		struct rsr_nam_index *entry = EXTERM_ADDRESS(EXTERM_MAIN_RSR_INDEX_LIST + i, struct rsr_nam_index);
		if (entry->filename[0] == NULL)
			continue;
		if (f)
			fprintf(f, "Entry = %s, Type = %s\r\n", entry->filename, entry->type);

		void *decompressed_data = EXTERM_CALL_FUNC_VOID_CHAR(EXTERM_FUNC_DECOMPRESS_RSR_FILE, entry->filename);

		char filename[256];
		sprintf(filename, RSR_DUMP_PATH, entry->filename);
		FILE *dumpfile = fopen(filename, "wb");
		fwrite(decompressed_data, entry->decompressed_size, 1, dumpfile);
		fflush(dumpfile);
		fclose(dumpfile);

		if (f)
			fflush(f);
	}

	if (f)
		fclose(f);
}

static int DisplaySplashScreen()
{
	WNDCLASSA wnd_class;
	wnd_class.style = 40;
	wnd_class.lpfnWndProc = EXTERM_ADDRESS(EXTERM_FUNC_UNK_4A6E26, WNDPROC);
	wnd_class.hInstance = main_hInstance;
	wnd_class.hIcon = 0;
	wnd_class.hCursor = LoadCursorA(0, 0x7F00);
	wnd_class.hbrBackground = 0;
	wnd_class.lpszMenuName = 0;
	wnd_class.lpszClassName = EXTERM_ADDRESS(EXTERM_SPLASH_CLASS_NAME, char);
	RegisterClassA(&wnd_class);
	HWND hWnd = CreateWindowExA(0,
		EXTERM_ADDRESS(EXTERM_SPLASH_CLASS_NAME, char),
		EXTERM_ADDRESS(EXTERM_WINDOW_NAME, char),
		WS_POPUP | WS_BORDER | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		128,
		64,
		0,
		0,
		main_hInstance,
		0);
	ShowWindow(hWnd, 1);
	ShowWindow(hWnd, 5);
	UpdateWindow(hWnd);
	Sleep(500);
	return DestroyWindow(hWnd);
}

static int CreateMainWindow(HINSTANCE hInstance)
{
	WNDCLASSA wnd_class;

	wnd_class.style = 32;
	// Assign the games main WndProc to our window
	wnd_class.lpfnWndProc = EXTERM_ADDRESS(EXTERM_FUNC_WND_PROC, WNDPROC);
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0;
	wnd_class.hInstance = hInstance;
	wnd_class.hIcon = 0;
	wnd_class.hCursor = 0;
	wnd_class.hbrBackground = 0;
	wnd_class.lpszMenuName = 0;
	wnd_class.lpszClassName = EXTERM_ADDRESS(EXTERM_SPLASH_CLASS_NAME, char);
	if (RegisterClassA(&wnd_class))
	{
		main_hWndParent = CreateWindowExA(8u,
			EXTERM_ADDRESS(EXTERM_SPLASH_CLASS_NAME, char),
			EXTERM_ADDRESS(EXTERM_WINDOW_NAME, char), 
			0x16000000u, 
			0, 
			0, 
			EXTERM_GLOBAL(EXTERM_WINDOW_NWIDTH, int),
			EXTERM_GLOBAL(EXTERM_WINDOW_NHEIGHT, int),
			0, 
			0, 
			hInstance, 
			0);
		EXTERM_GLOBAL(EXTERM_HWND_MAIN, HWND) = main_hWndParent;
	}

	if (main_hWndParent == 0)
		return 1;
	return 0;
}

static int WinMainCopy(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	main_hInstance = hInstance;
	char *mutex_name = EXTERM_ADDRESS(EXTERM_MUTEX_NAME, char);
	HANDLE mutexHnd = CreateMutexA(0, 1, mutex_name);
	MSG Msg;
	if (mutexHnd) {
		int unk1_ret = EXTERM_CALL_FUNC_INT_INT(EXTERM_FUNC_UNK_410012, 1);
		EXTERM_GLOBAL(EXTERM_HINSTANCE, HINSTANCE) = hInstance;
		unk1_ret = EXTERM_CALL_FUNC_INT_INT(EXTERM_FUNC_UNK_4A6D3B, hInstance);
		unk1_ret = EXTERM_CALL_FUNC_INT_CHAR(EXTERM_FUNC_UNK_45C69B, EXTERM_UNK_54F828);
		// Un-needed, DisplaySplashScreen does more or less the same thing
		// EXTERM_CALL_FUNC_INT_INT(EXTERM_FUNC_UNK_4A6F9E, unk1_ret);
		DisplaySplashScreen();
		EXTERM_GLOBAL(EXTERM_WINDOW_OPEN, int) = 1;
		
		if (!CreateMainWindow(hInstance))
		{
			ShowWindow(main_hWndParent, nCmdShow);
			UpdateWindow(main_hWndParent);
			SetCursor(0);
			while (1)
			{
				while (PeekMessageA(&Msg, 0, 0, 0, 1u))
				{
					if (Msg.message == WM_QUIT)
					{
						ReleaseMutex(mutexHnd);
						//CloseHandle(hHandle);
						ExitProcess(Msg.wParam);
					}
					TranslateMessage(&Msg);
					DispatchMessageA(&Msg);
				}
				if (EXTERM_GLOBAL(EXTERM_WINDOW_OPEN, int))
					RedrawWindow(main_hWndParent, 0, 0, 0x23u);
				WaitMessage();
			}
		}
	}
}

__declspec(dllexport) int StartExtermination(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// If we're only using this to dump the files then exit afterwards
	int msgBoxId = ShowMessageBox("Do you want to dump all files from exterm.rsr to disk?", "Dump?");
	if (msgBoxId == IDYES)
	{
		DumpRsrFile();
		return 0;
	}
	
	// Run our copy of the WinMain
	//WinMainCopy(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return 0;
}