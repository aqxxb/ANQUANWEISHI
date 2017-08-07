#pragma once
#include <vector>
#include "MyWhiteList.h"
#include <list>
using std::vector;
using std::list;
// CMyVirusScan 对话框


typedef struct _FILE_INFO
{
	WCHAR   szFilePath[MAX_PATH];  //文件路径
	BOOL    bIsFolder; //是否为文件夹
	DWORD64 qWFileSize;			//文件大小
	DWORD   dwAttribute;		//文件属性
	WCHAR   szAttributeDescription[16];//文件属性描述
	SYSTEMTIME stcCreatTime;	//创建时间
	SYSTEMTIME stcLastAccessTime;//最后访问时间
	SYSTEMTIME stcLastWriteTime; //最后写入时间
}FILE_INFO, *PFILE_INFO;

class CMyVirusScan : public CDialogEx
{
	DECLARE_DYNAMIC(CMyVirusScan)

public:
	CMyVirusScan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyVirusScan();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	
	void getFileList(const TCHAR* pszDir, int nDeep = 1);
	afx_msg void OnBnClickedButton4();
	void InCreaseVirusAndWhiteList();
	CString MD5value();
	//保存病毒库的MD5值，用于MD5查杀
	vector<CString> m_vecVirus;
	//白名单
	vector<CString> m_vecWhiteList;
	//文件夹列表
	vector<CString> m_vecFolder;
	//文件列表
	vector<CString> m_vecFile;
	CString m_MD5File;
	virtual BOOL OnInitDialog();
	CMyWhiteList m_WhiteListCtrl;
	CString m_Comspec;
	int Deep = 1;
	CString m_DeleteVirus;
};
