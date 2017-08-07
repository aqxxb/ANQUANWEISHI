#pragma once
#include "Rocket.h"


// CMyRocket �Ի���

class CMyRocket : public CDialogEx
{
	DECLARE_DYNAMIC(CMyRocket)


public:
	CMyRocket(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyRocket();

	CBitmap m_bitmap;
	BITMAP m_bitinfo ;
// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void MemoryMessage();
	void CleanMemory();
	HWND hDeskTopWnd;
	typedef struct _MEMORYSTATUS
	{
		DWORD dwLength;
		DWORD dwMemoryLoad;
		SIZE_T dwTotalPhy;
		SIZE_T dwAvailPhy;
		SIZE_T dwToPageFile;
		SIZE_T dwAvailPageFile;
		SIZE_T dwTotalVirtual;
		SIZE_T dwAvaiVirtual;
	}MEMMORYSTATUS, *LPMEMORYSTATUS;

	//
	int  m_MemoryEdit;
	
};
