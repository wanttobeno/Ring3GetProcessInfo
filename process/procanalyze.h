#include "stdafx.h"
#include <Psapi.h>

#pragma once

typedef void (WINAPI * PFN_Callback) (PVOID);

class ProcAnalyze
{
private:
	int EnablePrivilege(LPTSTR ptszPrivilegeName, BOOL bEnable=TRUE);
	int GetOSVer();
public:
	ProcAnalyze();
	~ProcAnalyze();

	//process
	int ListProcess(PFN_Callback _func);
	int GetProcFullPath(DWORD dwPID, TCHAR *tszPath);
	int KillProcess(DWORD dwPID);
	//thread
	int ListThread(PFN_Callback _func, DWORD dwPID = 0);
	int GetProcessName(DWORD dwPID, TCHAR *tszName);
	//heap
	int ListHeap(PFN_Callback _func, DWORD dwPID = 0);
	//int ListMemory(PFN_Callback _func, DWORD dwPID = 0);
	int ListMemory(PPROCESS_MEMORY_COUNTERS ppsmem, DWORD dwPID = 0);
	//modules
	int ListModule(PFN_Callback _func, DWORD dwPID );
	//pages
	int ListPages(PFN_Callback _func, DWORD dwPID = 0);
};
