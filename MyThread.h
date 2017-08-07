#pragma once
#include "MyListThread.h"


// CMyThread 对话框

class CMyThread : public CDialogEx
{
	DECLARE_DYNAMIC(CMyThread)

public:
	CMyThread(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyThread();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListThread m_thread;
	virtual BOOL OnInitDialog();
	DWORD dw1;
};
