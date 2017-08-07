#pragma once
#include <vector>
#include "MyWhiteList.h"
#include <list>
using std::vector;
using std::list;
// CMyVirusScan �Ի���


typedef struct _FILE_INFO
{
	WCHAR   szFilePath[MAX_PATH];  //�ļ�·��
	BOOL    bIsFolder; //�Ƿ�Ϊ�ļ���
	DWORD64 qWFileSize;			//�ļ���С
	DWORD   dwAttribute;		//�ļ�����
	WCHAR   szAttributeDescription[16];//�ļ���������
	SYSTEMTIME stcCreatTime;	//����ʱ��
	SYSTEMTIME stcLastAccessTime;//������ʱ��
	SYSTEMTIME stcLastWriteTime; //���д��ʱ��
}FILE_INFO, *PFILE_INFO;

class CMyVirusScan : public CDialogEx
{
	DECLARE_DYNAMIC(CMyVirusScan)

public:
	CMyVirusScan(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyVirusScan();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	
	void getFileList(const TCHAR* pszDir, int nDeep = 1);
	afx_msg void OnBnClickedButton4();
	void InCreaseVirusAndWhiteList();
	CString MD5value();
	//���没�����MD5ֵ������MD5��ɱ
	vector<CString> m_vecVirus;
	//������
	vector<CString> m_vecWhiteList;
	//�ļ����б�
	vector<CString> m_vecFolder;
	//�ļ��б�
	vector<CString> m_vecFile;
	CString m_MD5File;
	virtual BOOL OnInitDialog();
	CMyWhiteList m_WhiteListCtrl;
	CString m_Comspec;
	int Deep = 1;
	CString m_DeleteVirus;
};
