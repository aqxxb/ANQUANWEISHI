#pragma once


// CMyToolBox 对话框

class CMyToolBox : public CDialogEx
{
	DECLARE_DYNAMIC(CMyToolBox)

public:
	CMyToolBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyToolBox();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();

	virtual BOOL OnInitDialog();
};
