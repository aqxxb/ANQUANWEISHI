
// ��ȫ��ʿ��ĿDlg.h : ͷ�ļ�
//

#pragma once
#include "MyTabCtrl.h"


// C��ȫ��ʿ��ĿDlg �Ի���
class C��ȫ��ʿ��ĿDlg : public CDialogEx
{
// ����
public:
	C��ȫ��ʿ��ĿDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
