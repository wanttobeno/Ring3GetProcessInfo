#include "stdafx.h"
#include "Log.h"

#define OUTPUT_LEN MAX_PATH*5

#define SIZEOF(ar) sizeof(ar)/sizeof(TCHAR)

int StrLength(LPTSTR s)
{
	int i =0;
	while(*(TCHAR*)(s+i) != _T('\0')) {
		i++;
	}
	return i;
}

void Log(TCHAR *tszFormat, ...)
{
	va_list arg;
	va_start(arg, tszFormat);

	TCHAR tszBuffer[OUTPUT_LEN] = {0};
	StringCbVPrintf(tszBuffer, OUTPUT_LEN, tszFormat, arg);
	int len = _tcslen/*StrLength*/(tszBuffer);
	if(tszBuffer[len-1] != _T('\n')) {
		if(len < OUTPUT_LEN-1){
			tszBuffer[len] = _T('\n');
			tszBuffer[len+1] = _T('\0');
		}
	}
	va_end(arg);

	OutputDebugString(tszBuffer);
};