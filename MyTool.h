#pragma once
#include "MyQdMessage.h"
#include "MyMlMessage.h"
#include "afxwin.h"
#include "MyToolTableMessage.h"
#include <vector>
#include "MyRocket.h"
using std:: vector;
// CMyTool 对话框

class CMyTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTool)

public:
	CMyTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyTool();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton42();
	afx_msg void OnBnClickedButton41();
	afx_msg void OnBnClickedButton43();
	afx_msg void OnBnClickedButton44();
	afx_msg void OnBnClickedButton45();
	afx_msg void OnBnClickedButton46();
	afx_msg void OnBnClickedButton47();
	CString m_ToolOpenFileEdit;
	CString m_ToolRuKouEdit;
	CString m_ToolshijianEdit;
	CString m_ToolJxEdit;
	CString m_ToolQdEdit;
	CString m_ToolTzEdit;
	CString m_ToolSbEdit;
	virtual BOOL OnInitDialog();
	VOID MessageOftable();
	BOOL GetToken();
	CMyQdMessage m_ToolQdMessage;
	CMyMlMessage m_ToolMlMessage;

	CMyToolTableMessage m_ToolTableMessage;
	// 保存函数表
	vector<CString> m_vecTableMessage;
	
	void OnBnClickedButton1();
	void OnBnClickedButton3();
	void OnBnClickedButton4();
	void OnBnClickedButton5();

	CMyRocket *m_Rock;
};
