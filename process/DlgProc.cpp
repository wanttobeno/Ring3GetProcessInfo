// DlgProc.cpp : 实现文件
//

#include "stdafx.h"
#include "process.h"
#include "DlgProc.h"
#include "afxdialogex.h"
#include "DlgModule.h"

#include <Psapi.h>
// CDlgProc 对话框

IMPLEMENT_DYNAMIC(CDlgProc, CDialogEx)

CDlgProc::CDlgProc(CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgProc::IDD, pParent)
{
}

CDlgProc::~CDlgProc()
{
}

void CDlgProc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROC, m_ListProc);
}


BEGIN_MESSAGE_MAP(CDlgProc, CDialogEx)
	ON_MESSAGE(WM_INSERT_PROC, &CDlgProc::InsertProcItem)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PROC, &CDlgProc::OnLvnColumnclickListProc)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROC, &CDlgProc::OnNMRClickListProc)
	ON_MESSAGE(WM_LIST_INFO, &CDlgProc::ListInfo)
	ON_COMMAND(IDM_THREAD, &CDlgProc::OnThread)
	ON_COMMAND(IDM_HEAP, &CDlgProc::OnHeap)
	ON_COMMAND(IDM_PAGE, &CDlgProc::OnPage)
	ON_COMMAND(IDM_MODULE, &CDlgProc::OnModule)
	ON_COMMAND(IDM_KILLPROC, &CDlgProc::OnKillproc)
END_MESSAGE_MAP()


// CDlgProc 消息处理程序


BOOL CDlgProc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ListProc.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, 100, -1);
	m_ListProc.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 40, -1);
	m_ListProc.InsertColumn(2, _T("父进程"), LVCFMT_LEFT, 50, -1);
	m_ListProc.InsertColumn(3, _T("线程数"), LVCFMT_LEFT, 50, -1);
	m_ListProc.InsertColumn(4, _T("优先级"), LVCFMT_LEFT, 50, -1);
	m_ListProc.InsertColumn(5, _T("路径"), LVCFMT_LEFT, 300, -1);
	
	m_ListProc.InsertColumn(6, _T("缺页中断次数"), LVCFMT_LEFT, 80, -1);
	m_ListProc.InsertColumn(7, _T("内存高峰"), LVCFMT_LEFT, 80, -1);
	m_ListProc.InsertColumn(8, _T("使用内存"), LVCFMT_LEFT, 70, -1);
	m_ListProc.InsertColumn(9, _T("页面缓存池高峰"), LVCFMT_LEFT, 100, -1);
	m_ListProc.InsertColumn(10, _T("页面缓存池"), LVCFMT_LEFT, 100, -1);
	m_ListProc.InsertColumn(11, _T("非分页缓存池高峰"), LVCFMT_LEFT, 100, -1);
	m_ListProc.InsertColumn(12, _T("非分页缓存池"), LVCFMT_LEFT, 100, -1);
	m_ListProc.InsertColumn(13, _T("使用分页文件"), LVCFMT_LEFT, 100, -1);
	m_ListProc.InsertColumn(14, _T("使用分页文件高峰"), LVCFMT_LEFT, 100, -1);


	DWORD style = m_ListProc.GetStyle();
	style |= LVS_EX_FULLROWSELECT;
	m_ListProc.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	RECT rt;
	GetParent()->GetClientRect(&rt);
	m_ListProc.MoveWindow(&rt);


	dlgHwnd = m_hWnd;
	
	ListInfo(0, 0);

	m_ProcMenu.LoadMenu(IDR_MENU_PROC);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
HWND CDlgProc::dlgHwnd = NULL;
void CDlgProc::ListProc(PVOID info)
{
	::SendMessage(dlgHwnd, WM_INSERT_PROC, (WPARAM)info, 0);
}

LRESULT CDlgProc::InsertProcItem(WPARAM wParam,  LPARAM lParam)
{
	PPROCESSENTRY32 pe32 = (PPROCESSENTRY32)wParam;
	CString text;
	TCHAR tszExe[MAX_PATH] = {0};

	//text.Format(_T("%ws"), pe32->szExeFile);
	m_ListProc.InsertItem(0, pe32->szExeFile);
	text.Format(_T("%d"), pe32->th32ProcessID);
	m_ListProc.SetItemText(0, 1, text.GetBuffer());
	text.Format(_T("%d"), pe32->th32ParentProcessID);
	m_ListProc.SetItemText(0, 2, text.GetBuffer());
	text.Format(_T("%d"), pe32->cntThreads);
	m_ListProc.SetItemText(0, 3, text.GetBuffer());
	text.Format(_T("%d"), pe32->pcPriClassBase);
	m_ListProc.SetItemText(0, 4, text.GetBuffer());

	m_pa.GetProcFullPath(pe32->th32ProcessID, tszExe);
	m_ListProc.SetItemText(0, 5, tszExe);

	PROCESS_MEMORY_COUNTERS psmem = {sizeof(psmem)};
	m_pa.ListMemory(&psmem, pe32->th32ProcessID);

	text.Format(_T("%d"), psmem.PageFaultCount);
	m_ListProc.SetItemText(0, 6, text.GetBuffer());
	text.Format(_T("%d"), psmem.PeakWorkingSetSize);
	m_ListProc.SetItemText(0, 7, text.GetBuffer());
	text.Format(_T("%d"), psmem.WorkingSetSize);
	m_ListProc.SetItemText(0, 8, text.GetBuffer());
	text.Format(_T("%d"), psmem.QuotaPeakPagedPoolUsage);
	m_ListProc.SetItemText(0, 9, text.GetBuffer());
	text.Format(_T("%d"), psmem.QuotaPagedPoolUsage);
	m_ListProc.SetItemText(0, 10, text.GetBuffer());
	text.Format(_T("%d"), psmem.QuotaPeakNonPagedPoolUsage);
	m_ListProc.SetItemText(0, 11, text.GetBuffer());
	text.Format(_T("%d"), psmem.QuotaNonPagedPoolUsage);
	m_ListProc.SetItemText(0, 12, text.GetBuffer());
	text.Format(_T("%d"), psmem.PagefileUsage);
	m_ListProc.SetItemText(0, 13, text.GetBuffer());
	text.Format(_T("%d"), psmem.PeakPagefileUsage);
	m_ListProc.SetItemText(0, 14, text.GetBuffer());

	m_CurItemPID = 4;

	return 1;
}
typedef struct _Item
{
	CListCtrl *mlist;
	int Isub;     //排序列
}Item;
static int nSortOrder = 1;      // 排序的方式,1升序，-1降序。

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 得到排序方式
	Item*   pV=(Item*)lParamSort;   
	// 得到两个列的排序信息
	CString sz1 = pV->mlist->GetItemText(lParam1,pV->Isub);
	CString sz2 = pV->mlist->GetItemText(lParam2,pV->Isub);
	if(nSortOrder == 1)
	{
		return sz1.Compare(sz2);
	}	else	{
		return -sz1.Compare(sz2);
	}
}

void CDlgProc::OnLvnColumnclickListProc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	static int ncurSortCol = -1;      // 保存当前的排序列
	// 一开始表示为-1，表示尚未按任何列排序。
	/* 若当前排序列与点击列相同，则更改增、减序的排序方式。
	若点击列与当前排序列不同的列，则改变排序序，并将排序方式改为增序。*/
	if (ncurSortCol == pNMListView->iSubItem)
	{
		if (nSortOrder == 1)		{
			nSortOrder = -1;
		}
		else
		{
			nSortOrder = 1;
		}
	}	else{
		ncurSortCol = pNMListView->iSubItem;
		nSortOrder = 1;
	}
	Item mItem;

	mItem.mlist = &m_ListProc;
	mItem.Isub = pNMListView->iSubItem;
	for(int i=0;i < m_ListProc.GetItemCount();i++)	{  
		m_ListProc.SetItemData(i,i);
	}
	m_ListProc.SortItems(CompareFunc,(LPARAM)&mItem);

	*pResult = 0;
}

int CDlgProc::m_CurItemPID = 4;
void CDlgProc::OnNMRClickListProc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(pNMItemActivate->iItem != -1) {

		TCHAR tszPID[20] = {0};
		m_ListProc.GetItemText(pNMItemActivate->iItem, 1, tszPID, 20);
		m_CurItemPID = _ttoi(tszPID);

		CMenu *SubMenu = m_ProcMenu.GetSubMenu(0);
		DWORD dwPos = GetMessagePos();                
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );         
		
		SubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);

	}

	*pResult = 0;
}

LRESULT CDlgProc::ListInfo(WPARAM wParam, LPARAM lParam)
{
	m_ListProc.DeleteAllItems();
	m_pa.ListProcess(ListProc);
	return 1;
}

void CDlgProc::OnThread()
{
	// TODO: 在此添加命令处理程序代码
	CTabCtrl* _this = ((CTabCtrl*)GetParent());
	_this->SetCurSel(1);
	//this->ShowWindow(SW_HIDE);
	NMHDR *pNMHDR = new NMHDR;
	pNMHDR->hwndFrom = _this->m_hWnd;
	pNMHDR->idFrom = IDC_TAB_MAIN;
	pNMHDR->code = TCN_SELCHANGE;
	::SendMessage(_this->GetParent()->m_hWnd, /**/WM_NOTIFY, (WPARAM)IDC_TAB_MAIN, (LPARAM)pNMHDR);
	delete pNMHDR;
}

void CDlgProc::OnHeap()
{
	// TODO: 在此添加命令处理程序代码
	CTabCtrl* _this = ((CTabCtrl*)GetParent());
	_this->SetCurSel(2);
	//this->ShowWindow(SW_HIDE);
	NMHDR *pNMHDR = new NMHDR;
	pNMHDR->hwndFrom = _this->m_hWnd;
	pNMHDR->idFrom = IDC_TAB_MAIN;
	pNMHDR->code = TCN_SELCHANGE;
	::SendMessage(_this->GetParent()->m_hWnd, /**/WM_NOTIFY, (WPARAM)IDC_TAB_MAIN, (LPARAM)pNMHDR);
	delete pNMHDR;
}

void CDlgProc::OnPage()
{
	// TODO: 在此添加命令处理程序代码
	CTabCtrl* _this = ((CTabCtrl*)GetParent());
	_this->SetCurSel(3);
	//this->ShowWindow(SW_HIDE);
	NMHDR *pNMHDR = new NMHDR;
	pNMHDR->hwndFrom = _this->m_hWnd;//tab的父窗口
	pNMHDR->idFrom = IDC_TAB_MAIN;//tab id
	pNMHDR->code = TCN_SELCHANGE;//tab notify code
	::SendMessage(_this->GetParent()->m_hWnd, /**/WM_NOTIFY, (WPARAM)IDC_TAB_MAIN, (LPARAM)pNMHDR);

	delete pNMHDR;
}

void CDlgProc::OnModule()
{
	// TODO: 在此添加命令处理程序代码
	CDlgModule dlg;
	dlg.DoModal();
}


void CDlgProc::OnKillproc()
{
	if(!m_pa.KillProcess(m_CurItemPID)){
		MessageBox(_T("kill failed！"), _T("Warnning"), MB_ICONWARNING);
	}else{
		ListInfo(0, 0);
	}
	m_CurItemPID = 4;
}
