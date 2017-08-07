#pragma once
#include "MyListCtrl1.h"
#include "MyListCtrl2.h"
#include "MyListCtrl3.h"
#include "afxcmn.h"
// CMyTaskmgr 对话框

class CMyTaskmgr : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTaskmgr)

public:
	CMyTaskmgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyTaskmgr();

	// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl1 m_ListCtrl1;
	virtual BOOL OnInitDialog();
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lpram);


	void CPUmessage();
	void DeskTopWindows();
	CMyListCtrl2 m_ListCtrl2;
	CMyListCtrl3 m_ListCtrl3;
	CProgressCtrl m_ProgressCtrl;
};

	