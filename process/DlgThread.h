#pragma once
#include "afxcmn.h"



// DlgThread 对话框

class DlgThread : public CDialogEx
{
	DECLARE_DYNAMIC(DlgThread)

public:
	DlgThread(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgThread();

// 对话框数据
	enum { IDD = IDD_DIALOG_THREAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnInsertThread(WPARAM, LPARAM);
	afx_msg LRESULT ListInfo(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListHeap;
	virtual BOOL OnInitDialog();

	static HWND dlgHwnd;
	static void WINAPI ListThread(PVOID);
};
