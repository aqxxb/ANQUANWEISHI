
// 安全卫士项目Dlg.h : 头文件
//

#pragma once
#include "MyTabCtrl.h"


// C安全卫士项目Dlg 对话框
class C安全卫士项目Dlg : public CDialogEx
{
// 构造
public:
	C安全卫士项目Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void BossKey();
public:
	CMyTabCtrl m_tabctrl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_IsWindowHide = TRUE;
};
