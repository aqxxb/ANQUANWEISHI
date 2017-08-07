// MyVirusScan.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "MyVirusScan.h"
#include "afxdialogex.h"
#include "MD5.h"
#include <TlHelp32.h>
#include <Strsafe.h>
#include <minwinbase.h>
#include <string>
using std::wstring;
// CMyVirusScan �Ի���

IMPLEMENT_DYNAMIC(CMyVirusScan, CDialogEx)

CMyVirusScan::CMyVirusScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyVirusScan::IDD, pParent)
	, m_MD5File(_T("C:\\Users\\����\\Desktop\\��ͼС��.exe"))
	, m_Comspec(_T("C:\\Users\\����\\Desktop\\ƥ�䲡��1�ļ���"))
	, m_DeleteVirus(_T(""))
{

}

CMyVirusScan::~CMyVirusScan()
{
}

void CMyVirusScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_MD5File);
	DDX_Control(pDX, IDC_LIST3, m_WhiteListCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_Comspec);
	DDX_Text(pDX, IDC_EDIT2, m_DeleteVirus);
}


BEGIN_MESSAGE_MAP(CMyVirusScan, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyVirusScan::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyVirusScan::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyVirusScan::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMyVirusScan ��Ϣ�������
BOOL CMyVirusScan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��������
	InCreaseVirusAndWhiteList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//MD5��ɱ
void CMyVirusScan::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD v1, v2;
	UpdateData(TRUE);
	CString NUM2 =	MD5value();
	for (int i = 0; i < m_vecVirus.size(); i++)
	{
		if (m_vecVirus[i] == NUM2)
		{
			MessageBox(L"���ļ��ǲ�����");
		}
	}
}

//ȫ·����ɱ
void CMyVirusScan::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ɱ�������ڲ������д�������ָ����Ŀ¼��
	UpdateData(TRUE);
	CMD5 M_CMD50, M_CMD5,M_CMD51;
	CStringA NUMCOM,NUMCOM1,NUMCOM2;
	CString STR,STR1,STR2;
	NUMCOM2 = m_Comspec;
	char *p1 = NUMCOM2.GetBuffer();
	//��ȡ���ļ���MD5ֵ
	STR2 = M_CMD50.md5Str(p1);
	//1.��������༭�����ļ��е�·����������MD5ֵ
	//2.�����ļ�����ָ��������ļ������õ���MD5ֵ
	getFileList( m_Comspec,  5);
	
		 for (int i = 0; i < m_vecFolder.size(); i++)
		 {
			 NUMCOM = m_vecFolder[i];
			 char *p = NUMCOM.GetBuffer();
			 STR = M_CMD5.md5FileValue(p);
			 m_vecFile.push_back(STR);
		 }
	//3.���ļ��е�MD5ֵ��ָ���ļ���MD5ֵƥ�䣬�ж��ǲ��ǹ��ɲ����������� 
		 for (int i = 0; i < m_vecFile.size(); i++)
		 {
			 //�趨�����ļ���1�벡��1ƥ�䣬�����ļ���2�벡��2ƥ��
			 if (((m_vecFile[i] == L"d41d8cd98f00b204e9800998ecf8427e") && (STR2 == L"8fc4dad1bf56fc44b2ba9a6382f1ff2f")) || ((m_vecFile[i] == L"29d5fccd561eef2576960f9d5a7b4511")&&(STR2==L"c1f4ab3631d6d57b13b783f95dd2d9d5")))
			 {
				 m_DeleteVirus = m_vecFolder[i];
				 UpdateData(FALSE);
			 }
		 }
}


	// ��ȡ�ļ��б�(ͨ������һ���ļ������õ�)
void CMyVirusScan::getFileList(const TCHAR* pszDir, int nDeep)
{
		if (nDeep == 0)
			return;

		wstring dir;
		// 1. �ϳ�һ������ͨ�����·��,������Ҳ���
		//    �ļ�.
		dir = pszDir + wstring(L"\\*");

		// �����ļ�: 
		// 2. ���ҵ�һ���ļ�.
		WIN32_FIND_DATA fData;
		HANDLE hFindHandle;
		hFindHandle = FindFirstFile(dir.c_str(),
			&fData);

		// 2.1 �ж���û�в��ҵ�.
		if (hFindHandle == INVALID_HANDLE_VALUE)
			return;

		// 3. ���Һ������ļ�.
		do
		{
			// ���˵���ǰĿ¼���ϲ�Ŀ¼
			if (_tcscmp(fData.cFileName, L".") == 0
				|| _tcscmp(fData.cFileName, L"..") == 0)
			{
				continue;
			}

			// ���ļ������������
			wstring path = pszDir
				+ wstring(L"\\")
				+ fData.cFileName;

			m_vecFolder.push_back(path.c_str());

			// �ж��ļ��Ƿ���Ŀ¼
			if (FILE_ATTRIBUTE_DIRECTORY&fData.dwFileAttributes)
			{
				getFileList(path.c_str(), nDeep - 1);
			}

		} while (FindNextFile(hFindHandle, &fData));

		// 4. �رղ��Ҿ��
		FindClose(hFindHandle);
	}


//��������ɱ
void CMyVirusScan::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���б���г�ʼ�Ĳ���
	//�������һ���б�
	m_WhiteListCtrl.DeleteAllItems();
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	//��ȡ��ǰ�ؼ�����չ���///////////////////��ʼ��List�ؼ�
	DWORD dwOldStyle = m_WhiteListCtrl.GetExtendedStyle();          //��ȡԭ���
	//���õ�ǰ�ؼ�����չ���
	m_WhiteListCtrl.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //������ͼ��ʽ
		LVS_EX_GRIDLINES          //������,������ֻ������report����listctrl
		);
	//��ȡlist�Ŀ��
	CRect rc;
	m_WhiteListCtrl.GetClientRect(rc);
	int nWidth = rc.Width();
	//ΪList��������Ϣ
	m_WhiteListCtrl.InsertColumn(0,	           //��ǰ�б��               
		L"���ڰ������Ľ���",       //Ҫ��������Ϣ
		0,	           //���
		nWidth / 3);   //�еĿ��
	m_WhiteListCtrl.InsertColumn(1, L"����ID", 0, nWidth / 5);
	m_WhiteListCtrl.InsertColumn(2, L"���ȼ�", 0, nWidth / 5);
	m_WhiteListCtrl.InsertColumn(3, L"������ID", 0, nWidth / 5);

	//�ѳ�ʼ���ĺͺ������ڰ������еĽ�����Ϊ����
	//1.��������
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) {
		return;
	}
	PROCESSENTRY32 processInfo;
	// ��ʼ���ṹ��Ľṹ���ֽ��ֶ�
	processInfo.dwSize = sizeof(processInfo);
	// ��ȡ��һ������
	Process32First(hProcSnap, &processInfo);
	//������Ϣ�����������ƥ��		
	CString str, str1, str2;
	int i = 0;
	do {
		if (i < m_vecWhiteList.size())
		{
			for (i; i < m_vecWhiteList.size(); i++)
			{
				if (processInfo.szExeFile == m_vecWhiteList[i])
				{
					break;
				}
				else  continue;
			}
		}
		else
		{
			//�Ѳ��ڰ������Ľ�����Ϣ�����List��
			m_WhiteListCtrl.InsertItem(0, processInfo.szExeFile);
			str.Format(_T("%d"), processInfo.th32ProcessID);
			m_WhiteListCtrl.SetItemText(0, 1, str);
			str1.Format(_T("%d"), processInfo.pcPriClassBase);
			m_WhiteListCtrl.SetItemText(0, 2, str1);
			str2.Format(_T("%d"), processInfo.th32ParentProcessID);
			m_WhiteListCtrl.SetItemText(0, 3, str2);

		}

	} while (Process32Next(hProcSnap, &processInfo));
}
//���ԭʼ����MD5���Լ��������б�
void CMyVirusScan::InCreaseVirusAndWhiteList()
{
	//��ʼ��ԭʼ������
	m_vecVirus.push_back(L"d41d8cd98f00b204e9800998ecf8427e");
	m_vecVirus.push_back(L"29d5fccd561eef2576960f9d5a7b4511");
	m_vecVirus.push_back(L"4eeba26529346f197c60139028e8ded5");
	m_vecVirus.push_back(L"63ca6b1a893ac2e320b86a0e3d30d232");
	m_vecVirus.push_back(L"fab51e14c59c17a0fa742cb5320dd141");
	//��Ӱ������б�
	//�������̣�
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) {
		return ;
	}
	PROCESSENTRY32 processInfo;
	// ��ʼ���ṹ��Ľṹ���ֽ��ֶ�
	processInfo.dwSize = sizeof(processInfo);
	// ��ȡ��һ������
	Process32First(hProcSnap, &processInfo);
	//���������Ϣ									
	do {
		m_vecWhiteList.push_back(processInfo.szExeFile);
	} while (Process32Next(hProcSnap, &processInfo));

}

//����MD5ֵ
CString CMyVirusScan::MD5value()
{
	CMD5 md5File;
	int len = WideCharToMultiByte(CP_ACP, 0, m_MD5File, -1, NULL, 0, NULL, NULL);
	char*MD5Temp = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_MD5File, -1, MD5Temp, len, NULL, NULL);

	CString NUM1 = md5File.md5FileValue(MD5Temp);
	//delete[]MD5Temp;	
	return NUM1;
}



