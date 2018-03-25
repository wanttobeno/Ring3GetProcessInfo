#include "stdafx.h"
#include "procanalyze.h"

#include <strsafe.h>
#include <TlHelp32.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

ProcAnalyze::ProcAnalyze()
{
	EnablePrivilege(SE_DEBUG_NAME);
}

ProcAnalyze::~ProcAnalyze()
{

}

int ProcAnalyze::ListProcess(PFN_Callback _func)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = {sizeof(pe32)};

	if(hSnap != INVALID_HANDLE_VALUE) {
		if(Process32First(hSnap, &pe32)) {
			do{
				_func((PVOID)&pe32);
			}while(Process32Next(hSnap, &pe32));
		}
		CloseHandle(hSnap);
	}
	return 0;
}

int ProcAnalyze::GetOSVer()
{
	DWORD ver = GetVersion();
	if(LOWORD(ver) <= 5) 
	{
		return 0;
	}else{
		return 1;
	}
}

int ProcAnalyze::KillProcess(DWORD dwPID)
{
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if(hProc == NULL) {
		Log(_T("KillProcess->OpenProcess error:%d"), GetLastError());
		return 0;
	} 

	if(!TerminateProcess(hProc, 0)){
		CloseHandle(hProc);
		return 0;
	}
	CloseHandle(hProc);
	return 1;
}

int ProcAnalyze::GetProcFullPath(DWORD dwPID, TCHAR *tszPath)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 me32 = {sizeof(me32)};
	TCHAR tszExe[5] = {0};
	DWORD dwSize =MAX_PATH;
	HANDLE hProc = NULL;
	
	if(hSnap == INVALID_HANDLE_VALUE) {
		Log(_T("CreateToolhelp32Snapshot %d error:%d"), dwPID,  GetLastError());

		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		if(!hProc) {
			Log(_T("OpenProcess %d error:%d"), dwPID,  GetLastError());
			return 0;
		}
		if(GetOSVer() == 0){//xp
			if(!GetProcessImageFileName(hProc, tszPath, MAX_PATH)){
				Log(_T("GetProcessImageFileName %d error:%d"), dwPID,  GetLastError());
				CloseHandle(hProc);
				return 0;
			}
		}else{
			//if(!QueryFullProcessImageName(hProc, /*PROCESS_NAME_NATIVE*/0, tszPath, &dwSize)){
			//	Log(_T("QueryFullProcessImageName %d error:%d"), dwPID,  GetLastError());
			//	return 0;
			//}
		}
		CloseHandle(hProc);
		return 0;
	}
	if(Module32First(hSnap, &me32)) {
		do{
			_tcsncpy(tszExe, me32.szModule+_tcslen(me32.szModule)-4, 4);
			if(!_tcsicmp(tszExe, _T(".exe"))) {
				StringCbCopy(tszPath, MAX_PATH, me32.szExePath);
				CloseHandle(hSnap);
				return 1;
			}
		}while(Module32Next(hSnap, &me32));
	}
	CloseHandle(hSnap);
	return 1;
}

int ProcAnalyze::EnablePrivilege(LPTSTR ptszPrivilegeName, BOOL bEnable/* =TRUE */)
{
	HANDLE hToken = NULL;
	LUID   luid = {0};
	TOKEN_PRIVILEGES tp = {0};

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)){
		Log(_T("OpenProcessToken error:%d"), GetLastError());
		return 0;
	}

	if(!LookupPrivilegeValue(NULL, ptszPrivilegeName, &luid )){
		Log(_T("LookupPrivilegeValue error:%d"), GetLastError());
		CloseHandle(hToken);
		return 0;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges->Luid = luid;
	if(bEnable)
		tp.Privileges->Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges->Attributes = 0;
	// Enable the privilege or disable all privileges.
	if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL)){
		Log(_T("AdjustTokenPrivileges error:%d"), GetLastError());
		CloseHandle(hToken);
		return 0;
	}
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		Log(_T("The token does not have the specified privilege. \n"));
		CloseHandle(hToken);
		return FALSE;
	}
	CloseHandle(hToken);
	return 1;
}

int ProcAnalyze::ListThread(PFN_Callback _func, DWORD dwPID)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 te32 = {sizeof(te32)};

	if(hSnap == INVALID_HANDLE_VALUE) {
		Log(_T("ListThread->CreateToolhelp32Snapshot error:%d"), GetLastError());
		return 0;
	}
	if(Thread32First(hSnap, &te32)) {
		do{
			if( (dwPID != 0) && (te32.th32OwnerProcessID != dwPID) ) {
				continue;
			}
			_func((PVOID)&te32);
		}while(Thread32Next(hSnap, &te32));
	}
	CloseHandle(hSnap);
	return 1;
}

int ProcAnalyze::GetProcessName(DWORD dwPID, TCHAR *tszName)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = {sizeof(pe32)};

	if(hSnap != INVALID_HANDLE_VALUE) {
		if(Process32First(hSnap, &pe32)) {
			do{
				if(pe32.th32ProcessID == dwPID) {
					StringCbCopy(tszName, MAX_PATH, pe32.szExeFile);
					CloseHandle(hSnap);
					return 1;
				}
			}while(Process32Next(hSnap, &pe32));
		}
		CloseHandle(hSnap);
	}
	return 0;
}

int ProcAnalyze::ListHeap(PFN_Callback _func, DWORD dwPID)
{
	if(dwPID <= 0 )
		return 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPID);
	HEAPLIST32 hl32 = {sizeof(hl32)};
	HEAPENTRY32 he32 = {sizeof(he32)};

	if(hSnap == INVALID_HANDLE_VALUE){
		Log(_T("ListHeap->CreateToolhelp32Snapshot error:%d"), GetLastError());
		return 0;
	}

	if(Heap32ListFirst(hSnap, &hl32)) {
		do{
			he32.dwSize = sizeof(he32);
			if(Heap32First(&he32, hl32.th32ProcessID, hl32.th32HeapID)) {
				do{
					_func((PVOID)&he32);
				}while(Heap32Next(&he32));
			}
		}while(Heap32ListNext(hSnap, &hl32));	
	}
	CloseHandle(hSnap);
	return 1;
}

int ProcAnalyze::ListMemory(PPROCESS_MEMORY_COUNTERS psmem, DWORD dwPID /* = 0 */)
{
	if(dwPID <= 0) 
		return 0;

	//PROCESS_MEMORY_COUNTERS psmem = {sizeof(psmem)};

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if(hProc == 0) {
		Log(_T("ListMemory->OpenProcess error:%d"), GetLastError());
		return 0;
	}

	if(!GetProcessMemoryInfo(hProc, psmem, psmem->cb)){
		Log(_T("ListMemory->GetProcessMemoryInfo error:%d"), GetLastError());
		CloseHandle(hProc);
		return 0;
	}

	//_func((PVOID)&psmem);
	CloseHandle(hProc);
	return 1;
}

int ProcAnalyze::ListModule(PFN_Callback _func, DWORD dwPID )
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	MODULEENTRY32 me32 = {sizeof(me32)};

	if(hSnap == INVALID_HANDLE_VALUE) {
		Log(_T("ListModule->CreateToolhelp32Snapshot error:%d"), GetLastError());
		return 0;
	}

	if(Module32First(hSnap, &me32)) {
		do{
			_func((PVOID)&me32);
		}while(Module32Next(hSnap, &me32));
	}
	CloseHandle(hSnap);
	return 1;
}

int ProcAnalyze::ListPages(PFN_Callback _func, DWORD dwPID /* = 0 */)
{
	if(dwPID <= 0 ) 
		return 0;

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hProc == INVALID_HANDLE_VALUE)
		return 0;

	PPSAPI_WORKING_SET_INFORMATION ppswsi = new PSAPI_WORKING_SET_INFORMATION;

	memset(ppswsi, 0, sizeof(PSAPI_WORKING_SET_INFORMATION));
	if(!QueryWorkingSet(hProc, ppswsi, sizeof(PSAPI_WORKING_SET_INFORMATION))){
		if(ERROR_BAD_LENGTH == GetLastError()){
			int size = ppswsi->NumberOfEntries*sizeof(PSAPI_WORKING_SET_BLOCK) + sizeof(ULONG_PTR);
			ppswsi = (PPSAPI_WORKING_SET_INFORMATION)new BYTE[size];
			if(ppswsi != 0) {
				if(QueryWorkingSet(hProc, ppswsi, size)){
					//Log(_T("ListMemory->QueryWorkingSet error:%d"), GetLastError());
					_func((PVOID)ppswsi);
				}else{
					Log(_T("ListMemory->QueryWorkingSet error:%d"), GetLastError());
				}				
			}
		}
	}
	delete[] ppswsi;
	CloseHandle(hProc);
	return 1;
}