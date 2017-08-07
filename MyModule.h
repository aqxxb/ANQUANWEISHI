#pragma once
#include "MyListModule.h"


// CMyModule 对话框

class CMyModule : public CDialogEx
{
	DECLARE_DYNAMIC(CMyModule)

public:
	CMyModule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyModule();
	
// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMyListModule m_module;
	DWORD dw;
};
