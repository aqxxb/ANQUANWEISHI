#pragma once


// CMyToolBox �Ի���

class CMyToolBox : public CDialogEx
{
	DECLARE_DYNAMIC(CMyToolBox)

public:
	CMyToolBox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyToolBox();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();

	virtual BOOL OnInitDialog();
};
