#pragma once
#include "afxcmn.h"


// CDlgPage 对话框

class CDlgPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPage)

public:
	CDlgPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPage();

// 对话框数据
	enum { IDD = IDD_DIALOG_PAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnInsertPage(WPARAM, LPARAM);
	afx_msg LRESULT ListInfo(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListPage;

	static HWND dlgHwnd;
	static void WINAPI ListPage(PVOID);
};
