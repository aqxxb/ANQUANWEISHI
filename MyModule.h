#pragma once
#include "MyListModule.h"


// CMyModule �Ի���

class CMyModule : public CDialogEx
{
	DECLARE_DYNAMIC(CMyModule)

public:
	CMyModule(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyModule();
	
// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMyListModule m_module;
	DWORD dw;
};
