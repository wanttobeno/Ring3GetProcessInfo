#pragma once
#include "afxcmn.h"


// CDlgProc 对话框

class CDlgProc : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProc)

public:
	CDlgProc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProc();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT InsertProcItem(WPARAM, LPARAM);
	afx_msg LRESULT ListInfo(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListProc;
	virtual BOOL OnInitDialog();


public:
	static HWND dlgHwnd;
	static void WINAPI ListProc(PVOID);
	afx_msg void OnLvnColumnclickListProc(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CMenu m_ProcMenu;
	ProcAnalyze   m_pa;
public:
	static int	  m_CurItemPID;
	afx_msg void OnNMRClickListProc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnThread();
	afx_msg void OnHeap();
	afx_msg void OnPage();
	afx_msg void OnModule();
	afx_msg void OnKillproc();
};
