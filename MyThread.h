#pragma once
#include "MyListThread.h"


// CMyThread �Ի���

class CMyThread : public CDialogEx
{
	DECLARE_DYNAMIC(CMyThread)

public:
	CMyThread(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyThread();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMyListThread m_thread;
	virtual BOOL OnInitDialog();
	DWORD dw1;
};
