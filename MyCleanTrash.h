#pragma once
#include "MyListBox.h"
#include <vector>
using std::vector;

// CMyCleanTrash �Ի���

class CMyCleanTrash : public CDialogEx
{
	DECLARE_DYNAMIC(CMyCleanTrash)

public:
	CMyCleanTrash(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyCleanTrash();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	void TraverseFile(TCHAR szPath[MAX_PATH], /* Ҫ�������ļ�·�� */ const CString &strFilter,/* ���˵��ļ���׺ */ vector<CString> &vecPath);// ������������ļ�
	CString m_OpenFile;
	CString m_Suffix;
	CMyListBox m_PathList;
	CMyListBox m_FileList;
	// ����Ҫ�����������ļ�·��
	vector<CString> m_vecFileDelete;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton1();
};
