// MyVirusScan.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyVirusScan.h"
#include "afxdialogex.h"
#include "MD5.h"
#include <TlHelp32.h>
#include <Strsafe.h>
#include <minwinbase.h>
#include <string>
using std::wstring;
// CMyVirusScan 对话框

IMPLEMENT_DYNAMIC(CMyVirusScan, CDialogEx)

CMyVirusScan::CMyVirusScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyVirusScan::IDD, pParent)
	, m_MD5File(_T("C:\\Users\\北辰\\Desktop\\截图小兵.exe"))
	, m_Comspec(_T("C:\\Users\\北辰\\Desktop\\匹配病毒1文件夹"))
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


// CMyVirusScan 消息处理程序
BOOL CMyVirusScan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化病毒库
	InCreaseVirusAndWhiteList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//MD5查杀
void CMyVirusScan::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	DWORD v1, v2;
	UpdateData(TRUE);
	CString NUM2 =	MD5value();
	for (int i = 0; i < m_vecVirus.size(); i++)
	{
		if (m_vecVirus[i] == NUM2)
		{
			MessageBox(L"该文件是病毒！");
		}
	}
}

//全路径查杀
void CMyVirusScan::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	//查杀条件：在病毒库中存在且在指定的目录下
	UpdateData(TRUE);
	CMD5 M_CMD50, M_CMD5,M_CMD51;
	CStringA NUMCOM,NUMCOM1,NUMCOM2;
	CString STR,STR1,STR2;
	NUMCOM2 = m_Comspec;
	char *p1 = NUMCOM2.GetBuffer();
	//获取根文件夹MD5值
	STR2 = M_CMD50.md5Str(p1);
	//1.遍历输入编辑框中文件夹的路径及计算其MD5值
	//2.遍历文件夹中指定种类的文件，并得到其MD5值
	getFileList( m_Comspec,  5);
	
		 for (int i = 0; i < m_vecFolder.size(); i++)
		 {
			 NUMCOM = m_vecFolder[i];
			 char *p = NUMCOM.GetBuffer();
			 STR = M_CMD5.md5FileValue(p);
			 m_vecFile.push_back(STR);
		 }
	//3.对文件夹的MD5值与指定文件的MD5值匹配，判断是不是构成病毒的条件。 
		 for (int i = 0; i < m_vecFile.size(); i++)
		 {
			 //设定病毒文件夹1与病毒1匹配，病毒文件夹2与病毒2匹配
			 if (((m_vecFile[i] == L"d41d8cd98f00b204e9800998ecf8427e") && (STR2 == L"8fc4dad1bf56fc44b2ba9a6382f1ff2f")) || ((m_vecFile[i] == L"29d5fccd561eef2576960f9d5a7b4511")&&(STR2==L"c1f4ab3631d6d57b13b783f95dd2d9d5")))
			 {
				 m_DeleteVirus = m_vecFolder[i];
				 UpdateData(FALSE);
			 }
		 }
}


	// 获取文件列表(通过遍历一个文件夹来得到)
void CMyVirusScan::getFileList(const TCHAR* pszDir, int nDeep)
{
		if (nDeep == 0)
			return;

		wstring dir;
		// 1. 合成一个带有通配符的路径,否则查找不到
		//    文件.
		dir = pszDir + wstring(L"\\*");

		// 遍历文件: 
		// 2. 查找第一个文件.
		WIN32_FIND_DATA fData;
		HANDLE hFindHandle;
		hFindHandle = FindFirstFile(dir.c_str(),
			&fData);

		// 2.1 判断有没有查找到.
		if (hFindHandle == INVALID_HANDLE_VALUE)
			return;

		// 3. 查找后续的文件.
		do
		{
			// 过滤掉当前目录和上层目录
			if (_tcscmp(fData.cFileName, L".") == 0
				|| _tcscmp(fData.cFileName, L"..") == 0)
			{
				continue;
			}

			// 将文件名保存进变量
			wstring path = pszDir
				+ wstring(L"\\")
				+ fData.cFileName;

			m_vecFolder.push_back(path.c_str());

			// 判断文件是否是目录
			if (FILE_ATTRIBUTE_DIRECTORY&fData.dwFileAttributes)
			{
				getFileList(path.c_str(), nDeep - 1);
			}

		} while (FindNextFile(hFindHandle, &fData));

		// 4. 关闭查找句柄
		FindClose(hFindHandle);
	}


//白名单查杀
void CMyVirusScan::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	//对列表进行初始的操作
	//首先清空一遍列表
	m_WhiteListCtrl.DeleteAllItems();
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	m_WhiteListCtrl.DeleteColumn(0);
	//获取当前控件的扩展风格///////////////////初始化List控件
	DWORD dwOldStyle = m_WhiteListCtrl.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_WhiteListCtrl.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线,网格线只适用与report风格的listctrl
		);
	//获取list的宽度
	CRect rc;
	m_WhiteListCtrl.GetClientRect(rc);
	int nWidth = rc.Width();
	//为List插入列消息
	m_WhiteListCtrl.InsertColumn(0,	           //当前列编号               
		L"不在白名单的进程",       //要求插入的信息
		0,	           //风格
		nWidth / 3);   //列的宽度
	m_WhiteListCtrl.InsertColumn(1, L"进程ID", 0, nWidth / 5);
	m_WhiteListCtrl.InsertColumn(2, L"优先级", 0, nWidth / 5);
	m_WhiteListCtrl.InsertColumn(3, L"父进程ID", 0, nWidth / 5);

	//把初始化的和后来不在白名单中的进程视为病毒
	//1.遍历进程
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) {
		return;
	}
	PROCESSENTRY32 processInfo;
	// 初始化结构体的结构体字节字段
	processInfo.dwSize = sizeof(processInfo);
	// 获取第一个进程
	Process32First(hProcSnap, &processInfo);
	//进程信息与白名单进行匹配		
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
			//把不在白名单的进程信息输出到List中
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
//添加原始病毒MD5库以及白名单列表
void CMyVirusScan::InCreaseVirusAndWhiteList()
{
	//初始化原始病毒库
	m_vecVirus.push_back(L"d41d8cd98f00b204e9800998ecf8427e");
	m_vecVirus.push_back(L"29d5fccd561eef2576960f9d5a7b4511");
	m_vecVirus.push_back(L"4eeba26529346f197c60139028e8ded5");
	m_vecVirus.push_back(L"63ca6b1a893ac2e320b86a0e3d30d232");
	m_vecVirus.push_back(L"fab51e14c59c17a0fa742cb5320dd141");
	//添加白名单列表
	//遍历进程：
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) {
		return ;
	}
	PROCESSENTRY32 processInfo;
	// 初始化结构体的结构体字节字段
	processInfo.dwSize = sizeof(processInfo);
	// 获取第一个进程
	Process32First(hProcSnap, &processInfo);
	//导入进程信息									
	do {
		m_vecWhiteList.push_back(processInfo.szExeFile);
	} while (Process32Next(hProcSnap, &processInfo));

}

//计算MD5值
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



