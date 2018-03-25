#pragma once
#include "afxcmn.h"


// CDlgHeap 对话框

class CDlgHeap : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHeap)

public:
	CDlgHeap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHeap();

// 对话框数据
	enum { IDD = IDD_DIALOG_HEAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnInsertHeap(WPARAM, LPARAM);
	afx_msg LRESULT ListInfo(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListHeap;
	virtual BOOL OnInitDialog();

	static HWND dlgHwnd;
	static void WINAPI ListHeap(PVOID);
};
