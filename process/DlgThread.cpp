// DlgThread.cpp : 实现文件
//

#include "stdafx.h"
#include "process.h"
#include "DlgThread.h"
#include "afxdialogex.h"


// DlgThread 对话框

IMPLEMENT_DYNAMIC(DlgThread, CDialogEx)

DlgThread::DlgThread(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgThread::IDD, pParent)
{

}

DlgThread::~DlgThread()
{
}

void DlgThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THREAD, m_ListHeap);
}


BEGIN_MESSAGE_MAP(DlgThread, CDialogEx)
	ON_MESSAGE(WM_INSERT_THREAD, &DlgThread::OnInsertThread)
	ON_MESSAGE(WM_LIST_INFO, &DlgThread::ListInfo)
END_MESSAGE_MAP()


// DlgThread 消息处理程序


BOOL DlgThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListHeap.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, 100, -1);
	m_ListHeap.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 100, -1);
	m_ListHeap.InsertColumn(2, _T("TID"), LVCFMT_LEFT, 100, -1);
	m_ListHeap.InsertColumn(3, _T("线程基本优先级"), LVCFMT_LEFT, 100, -1);
	m_ListHeap.InsertColumn(4, _T("优先级变化"), LVCFMT_LEFT, 100, -1);
	m_ListHeap.InsertColumn(5, _T("引用数"), LVCFMT_LEFT, 300, -1);

	DWORD style = m_ListHeap.GetStyle();
	style |= LVS_EX_FULLROWSELECT;
	m_ListHeap.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RECT rt;
	GetParent()->GetClientRect(&rt);
	m_ListHeap.MoveWindow(&rt);


	dlgHwnd = m_hWnd;
	
	//ListInfo(0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
HWND DlgThread::dlgHwnd = NULL;
void DlgThread::ListThread(PVOID info)
{
	::SendMessage(dlgHwnd, WM_INSERT_THREAD, (WPARAM)info, 0);
}

LRESULT DlgThread::OnInsertThread(WPARAM wParam, LPARAM lParam)
{
	PTHREADENTRY32 te32 = (PTHREADENTRY32)wParam;
	CString text;
	TCHAR tszExe[MAX_PATH] = {0};

	ProcAnalyze pa;
	pa.GetProcessName(te32->th32OwnerProcessID, tszExe);
	m_ListHeap.InsertItem(0, tszExe);
	text.Format(_T("%d"), te32->th32OwnerProcessID);
	m_ListHeap.SetItemText(0, 1, text.GetBuffer());
	text.Format(_T("%d"), te32->th32ThreadID);
	m_ListHeap.SetItemText(0, 2, text.GetBuffer());
	text.Format(_T("%d"), te32->tpBasePri);
	m_ListHeap.SetItemText(0, 3, text.GetBuffer());
	text.Format(_T("%d"), te32->tpDeltaPri);
	m_ListHeap.SetItemText(0, 4, text.GetBuffer());
	text.Format(_T("%d"), te32->cntUsage);
	m_ListHeap.SetItemText(0, 5,  text.GetBuffer());

	return 1;
}

LRESULT DlgThread::ListInfo(WPARAM wParam, LPARAM lParam)
{
	m_ListHeap.DeleteAllItems();

	ProcAnalyze pa;
	pa.ListThread(ListThread, wParam);

	return 1;
}