#pragma once


// CRocket 对话框

class CRocket : public CDialogEx
{
	DECLARE_DYNAMIC(CRocket)

public:
	CRocket(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRocket();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
