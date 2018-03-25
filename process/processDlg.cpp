
// processDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "process.h"
#include "processDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CprocessDlg 对话框



CprocessDlg::CprocessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CprocessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CprocessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_TabMain);
}

BEGIN_MESSAGE_MAP(CprocessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CprocessDlg::OnTcnSelchangeTabMain)
	ON_COMMAND(IDM_THREAD, &CprocessDlg::OnThread)
END_MESSAGE_MAP()


// CprocessDlg 消息处理程序
static UINT indicators[] = {
IDS_CONTENT,IDS_TOTAL,
};

BOOL CprocessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//m_TabMain.Create(TCS_TABS | TCS_FIXEDWIDTH | WS_CHILD | WS_VISIBLE, rtWnd, this, IDC_TAB_MAIN);
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(0, 0, x-100, y-100, TRUE);

	RECT rtTab ;
	this->GetClientRect(&rtTab);
	//rtTab.bottom -= 20;
	m_TabMain.MoveWindow(&rtTab);

	m_TabMain.InsertItem(0, _T("进程"));
	m_TabMain.InsertItem(1, _T("线程"));
	m_TabMain.InsertItem(2, _T("堆"));
	m_TabMain.InsertItem(3, _T("内存分页"));
	//m_TabMain.InsertItem(4, _T("作业"));
	//m_TabMain.InsertItem(5, _T("内核"));
	m_TabMain.SetMinTabWidth(100);

	//////////////////////////////////////////////////////////////////////////
	m_DlgProc.Create(IDD_DIALOG_PROC, &m_TabMain);	
	m_DlgThread.Create(IDD_DIALOG_THREAD, &m_TabMain);
	m_DlgHeap.Create(IDD_DIALOG_HEAP, &m_TabMain);
	m_DlgPage.Create(IDD_DIALOG_PAGES, &m_TabMain);
	m_pDlgTabs[0] = &m_DlgProc;
	m_pDlgTabs[1] = &m_DlgThread;
	m_pDlgTabs[2] = &m_DlgHeap;
	m_pDlgTabs[3] = &m_DlgPage;

	m_TabMain.GetClientRect(&rtTab);
	rtTab.top += 20;
	RECT rt = rtTab;
	int width = rt.right- rt.left;
	int height = rt.bottom - rt.top;
	for(int i=0; i<4; i++) {
		m_pDlgTabs[i]->MoveWindow(&rt, TRUE);
	}
	//m_DlgProc.MoveWindow(rt.left, rt.top, width, height, TRUE);

	m_DlgProc.ShowWindow(SW_SHOW);
	m_TabNumber = 0;

	//m_StatusBar.Create(this);
	//m_StatusBar.SetIndicators(indicators, 2);
	
	//CRect rect;
	//GetClientRect(rect);

	//int strPartDim[4]= {100, 280}; //分割数量
	//m_StatusBar.GetStatusBarCtrl().SetParts(2, strPartDim);
	//m_StatusBar.SetPaneInfo(0, IDS_CONTENT, SBPS_NORMAL, 100);
	//m_StatusBar.SetPaneInfo(1, IDS_TOTAL, SBPS_NORMAL, 100);
	//m_StatusBar.SetPaneText(0,_T("进程"));
	//m_StatusBar.SetPaneText(1,_T("数量:100"));

	//m_StatusBar.MoveWindow(0, rect.bottom-20, rect.right, 20);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CprocessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CprocessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	CRect rect;
	GetClientRect(rect);
	if (m_StatusBar)
		m_StatusBar.MoveWindow(0, rect.bottom-20, rect.right, 20);
	
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CprocessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CprocessDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_pDlgTabs[m_TabNumber]->ShowWindow(SW_HIDE);

	m_TabNumber = m_TabMain.GetCurSel();

	m_pDlgTabs[m_TabNumber]->ShowWindow(SW_SHOW);
	
	::SendMessage(m_pDlgTabs[m_TabNumber]->m_hWnd, WM_LIST_INFO, m_DlgProc.m_CurItemPID, 0);
	m_DlgProc.m_CurItemPID = 4;

	*pResult = 0;
}


void CprocessDlg::OnThread()
{
	// TODO: 在此添加命令处理程序代码
	m_TabMain.SetCurSel(1);
	//m_DlgThread
}
