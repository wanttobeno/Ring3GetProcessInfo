// DlgPage.cpp : 实现文件
//

#include "stdafx.h"
#include "process.h"
#include "DlgPage.h"
#include "afxdialogex.h"

//#include <Psapi.h>
// CDlgPage 对话框

IMPLEMENT_DYNAMIC(CDlgPage, CDialogEx)

CDlgPage::CDlgPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPage::IDD, pParent)
{

}

CDlgPage::~CDlgPage()
{
}

void CDlgPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGES, m_ListPage);
}


BEGIN_MESSAGE_MAP(CDlgPage, CDialogEx)
	ON_MESSAGE(WM_INSERT_PAGE, &CDlgPage::OnInsertPage)
	ON_MESSAGE(WM_LIST_INFO, &CDlgPage::ListInfo)
END_MESSAGE_MAP()


// CDlgPage 消息处理程序


BOOL CDlgPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	dlgHwnd = m_hWnd;

	m_ListPage.InsertColumn(0, _T("标志"), LVCFMT_LEFT, 100, -1);
	m_ListPage.InsertColumn(1, _T("保护"), LVCFMT_LEFT, 100, -1);
	m_ListPage.InsertColumn(2, _T("共享数"), LVCFMT_LEFT, 100, -1);
	m_ListPage.InsertColumn(3, _T("是否共享"), LVCFMT_LEFT, 100, -1);
	m_ListPage.InsertColumn(4, _T("虚拟地址"), LVCFMT_LEFT, 100, -1);

	m_ListPage.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RECT rt;
	GetParent()->GetClientRect(&rt);
	m_ListPage.MoveWindow(&rt);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HWND CDlgPage::dlgHwnd = NULL;
void CDlgPage::ListPage(PVOID info)
{
	::SendMessage(dlgHwnd, WM_INSERT_PAGE, (WPARAM)info, 0);
}

/*
Value	Meaning
0
The page is not accessed.
1
Read-only.
2
Executable.
3
Executable and read-only.
4
Read/write.
5
Copy-on-write.
6
Executable and read/write.
7
Executable and copy-on-write.
8
The page is not accessed.
9
Non-cacheable and read-only.
10
Non-cacheable and executable.
11
Non-cacheable, executable, and read-only.
12
Non-cacheable and read/write.
13
Non-cacheable and copy-on-write.
14
Non-cacheable, executable, and read/write.
15
Non-cacheable, executable, and copy-on-write.
16
The page is not accessed.
17
Guard page and read-only.
18
Guard page and executable.
19
Guard page, executable, and read-only.
20
Guard page and read/write.
21
Guard page and copy-on-write.
22
Guard page, executable, and read/write.
23
Guard page, executable, and copy-on-write.
24
The page is not accessed.
25
Non-cacheable, guard page, and read-only.
26
Non-cacheable, guard page, and executable.
27
Non-cacheable, guard page, executable, and read-only.
28
Non-cacheable, guard page, and read/write.
29
Non-cacheable, guard page, and copy-on-write.
30
Non-cacheable, guard page, executable, and read/write.
31
Non-cacheable, guard page, executable, and copy-on-write.
*/
LRESULT CDlgPage::OnInsertPage(WPARAM wParam, LPARAM lParam)
{
	CString text;
	PPSAPI_WORKING_SET_INFORMATION pswsi = (PPSAPI_WORKING_SET_INFORMATION)wParam;

	for(DWORD i=0; i<pswsi->NumberOfEntries; i++) {
		text.Format(_T("%d"), pswsi->WorkingSetInfo[i].Flags);
		m_ListPage.InsertItem(0, text.GetBuffer());
		text.Format(_T("%d"), pswsi->WorkingSetInfo[i].Protection);
		m_ListPage.SetItemText(0, 1, text.GetBuffer());
		text.Format(_T("%d"), pswsi->WorkingSetInfo[i].ShareCount);
		m_ListPage.SetItemText(0, 2, text.GetBuffer());
		text.Format(_T("%d"), pswsi->WorkingSetInfo[i].Shared);
		m_ListPage.SetItemText(0, 3, text.GetBuffer());
		text.Format(_T("0x%08X"), pswsi->WorkingSetInfo[i].VirtualPage);
		m_ListPage.SetItemText(0, 4, text.GetBuffer());
	}

	return 0;
}

LRESULT CDlgPage::ListInfo(WPARAM wParam, LPARAM lParam)
{
	m_ListPage.DeleteAllItems();

	ProcAnalyze pa;
	pa.ListPages(ListPage, wParam);

	return 1;
}