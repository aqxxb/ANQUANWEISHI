#pragma once
#include "MyListBox.h"
#include <vector>
using std::vector;

// CMyCleanTrash 对话框

class CMyCleanTrash : public CDialogEx
{
	DECLARE_DYNAMIC(CMyCleanTrash)

public:
	CMyCleanTrash(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyCleanTrash();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	void TraverseFile(TCHAR szPath[MAX_PATH], /* 要遍历的文件路径 */ const CString &strFilter,/* 过滤的文件后缀 */ vector<CString> &vecPath);// 保存遍历到的文件
	CString m_OpenFile;
	CString m_Suffix;
	CMyListBox m_PathList;
	CMyListBox m_FileList;
	// 保存要遍历的垃圾文件路径
	vector<CString> m_vecFileDelete;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton1();
};
