#pragma once

#pragma warning(disable : 4731)

#define EXTERM_ADDRESS(address, type)				((type*)address)
#define EXTERM_GLOBAL(address, type)				(*((type*)address))
#define EXTERM_CALLPROC(address)					(((void(*)())address)())
#define EXTERM_CALLFUNC(address, returnType)		(((returnType(*)())address)())

#define EXTERM_MUTEX_NAME							0x00501619
#define EXTERM_SPLASH_CLASS_NAME					0x0050160E
#define EXTERM_WINDOW_NAME							0x005015F0
#define EXTERM_HINSTANCE							0x007C8320
#define EXTERM_HWND_MAIN							0x007C8338
#define EXTERM_WINDOW_OPEN							0x0054F834
#define EXTERM_WINDOW_NWIDTH						0x0051E1AC
#define EXTERM_WINDOW_NHEIGHT						0x0051E1B0

// After calling EXTERM_FUNC_READ_NAME_FILE, if successful then
// then opened FDs are stored here
#define EXTERM_MAIN_RSR_FILE_HND					0x0056F210
#define EXTERM_MAIN_RSR_NAME_HND					0x0056F20C

// List of up to 0x800 rsr_nam_index structs, use EXTERM_MAIN_RSR_READ_CT
// to know how many are populated
#define EXTERM_MAIN_RSR_INDEX_LIST					0x0056F214
// How many of the above rsr_nam_index structs are populated
#define EXTERM_MAIN_RSR_READ_CT						0x0058F214

// List of available ingame errors (see error_msg struct)
#define EXTERM_ERROR_LIST							0x0058F260

// Location init cfg is loaded
#define EXTERM_INIT_CFG								0x0054FE90

// 
// Functions
//

#define EXTERM_FUNC_OPEN_RSR_FILES					0x004C8538
// Decompress a file and return a pointer to it
#define EXTERM_FUNC_DECOMPRESS_RSR_FILE				0x004D3257

// Close nam and rsr files
#define EXTERM_FUNC_CLOSE_RSR_FILES					0x004D3217

#define EXTERM_FUNC_READ_NAME_FILE					0x004D3094

//
// Funcs & variables accessed from original WinMain
//

// These are called from the original WinMain
#define EXTERM_FUNC_UNK_410012						0x00410012
#define EXTERM_FUNC_UNK_4A6D3B						0x004A6D3B
#define EXTERM_FUNC_UNK_45C69B						0x0045C69B
// We will hijack this one, this is where CreateWindow is called
#define EXTERM_FUNC_UNK_4A6F9E						0x004A6F9E
// WndProc splash
#define EXTERM_FUNC_UNK_4A6E26						0x004A6E26

#define EXTERM_FUNC_WND_PROC						0x004A6A76

#define EXTERM_UNK_54F828							0x0054F828


static void EXTERM_CALL_PROC_CHAR(int address, char *_eax)
{
	_asm {
		mov eax, _eax
		call [address]
	}
}

static int EXTERM_CALL_FUNC_INT_INT(int address, int _eax)
{
	int ret;
	_asm {
		mov eax, _eax
		call [address]
		mov ret, eax
	}
	return ret;
}


static int EXTERM_CALL_FUNC_INT_CHAR(int address, char *_eax)
{
	void *ret;
	_asm {
		mov eax, _eax
		call[address]
		mov ret, eax
	}
	return ret;
}


static void * EXTERM_CALL_FUNC_VOID_CHAR(int address, char *_eax)
{
	void *ret;
	_asm {
		mov eax, _eax
		call [address]
		mov ret, eax
	}
	return ret;
}