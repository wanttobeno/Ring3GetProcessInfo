#pragma once
#include "afxcmn.h"


// CDlgModule 对话框

class CDlgModule : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModule)

public:
	CDlgModule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModule();

// 对话框数据
	enum { IDD = IDD_DIALOG_MODULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnInsertModule(WPARAM, LPARAM);
	afx_msg LRESULT ListInfo(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListModule;

	static HWND dlgHwnd;
	static void WINAPI ListModule(PVOID);
};
