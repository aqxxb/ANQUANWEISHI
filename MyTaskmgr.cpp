// MyTaskmgr.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyTaskmgr.h"
#include "afxdialogex.h"
#include <windows.h>
#include <TlHelp32.h>
#include <locale.h>
#include <Psapi.h>
#include <winsvc.h>
#include <libloaderapi.h>
// CMyTaskmgr 对话框

IMPLEMENT_DYNAMIC(CMyTaskmgr, CDialogEx)

CMyTaskmgr::CMyTaskmgr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTaskmgr::IDD, pParent)
{

}

CMyTaskmgr::~CMyTaskmgr()
{
}

void CMyTaskmgr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl1);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl2);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl3);
	DDX_Control(pDX, IDC_PROGRESS2, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CMyTaskmgr, CDialogEx)
	
END_MESSAGE_MAP()

BOOL CMyTaskmgr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	////////////////////MyListCtrl1//////////////////////////////

	//获取当前控件的扩展风格///////////////////初始化List控件
	DWORD dwOldStyle = m_ListCtrl1.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_ListCtrl1.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线,网格线只适用与report风格的listctrl
		);
	//获取list的宽度
	CRect rc;
	m_ListCtrl1.GetClientRect(rc);
	int nWidth = rc.Width();
	//为List插入列消息
	m_ListCtrl1.InsertColumn(0,	           //当前列编号               
		L"进程",       //要求插入的信息
		0,	           //风格
		nWidth / 4);   //列的宽度
	m_ListCtrl1.InsertColumn(1, L"进程ID", 0, nWidth / 4);
	m_ListCtrl1.InsertColumn(2, L"优先级", 0, nWidth / 4);
	m_ListCtrl1.InsertColumn(3, L"父进程ID", 0, nWidth / 4);
	//进程信息
	// 设置本地字符集
	setlocale(LC_ALL, "chs");
	// 1. 使用快照来遍历进程
	// 1.1 创建进程快照
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) {
		return 0;
	}
	PROCESSENTRY32 processInfo;
	// 初始化结构体的结构体字节字段
	processInfo.dwSize = sizeof(processInfo);
	// 获取第一个进程
	Process32First(hProcSnap, &processInfo);
	CString  str, str1, str2;
	//输出进程信息									
	do {
		m_ListCtrl1.InsertItem(0, processInfo.szExeFile);
		str.Format(_T("%d"), processInfo.th32ProcessID);
		m_ListCtrl1.SetItemText(0, 1, str);
		str1.Format(_T("%d"), processInfo.pcPriClassBase);
		m_ListCtrl1.SetItemText(0, 2, str1);
		str2.Format(_T("%d"), processInfo.th32ParentProcessID);
		m_ListCtrl1.SetItemText(0, 3, str2);
	} while (Process32Next(hProcSnap, &processInfo));

	///////////////////////MyListCtrl2/////////////////////////////
	//打开服务控制管理器
	SC_HANDLE hsev = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hsev,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,  //所有的服务状态
		NULL,				//缓冲区
		0,					//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中的服务个数
		NULL, NULL);
	//申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hsev, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL,
		(PBYTE)pEnumService,
		dwSize,
		&dwSize,
		&dwServiceNum,
		NULL, NULL);
	//
	DWORD dwOldStyle1 = m_ListCtrl2.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_ListCtrl2.SetExtendedStyle(dwOldStyle1 |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线,网格线只适用与report风格的listctrl
		);
	//获取list的宽度
	CRect rc1;
	m_ListCtrl2.GetClientRect(rc1);
	int nWidth1 = rc1.Width();
	//为List插入列消息
	m_ListCtrl2.InsertColumn(0, L"服务名称", 0, nWidth1 / 3);
	m_ListCtrl2.InsertColumn(1, L"服务类型", 0, nWidth1 / 3);
	m_ListCtrl2.InsertColumn(2, L"服务状态", 0, nWidth1 / 3);
	//进程信息
	// 设置本地字符集
	setlocale(LC_ALL, "chs");
	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//获取基础服务信息
		//服务名
		m_ListCtrl2.InsertItem(0, pEnumService[i].lpServiceName);
		//服务状态
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
		case 1:
			m_ListCtrl2.SetItemText(0, 2, L"服务停止");		break;
		case 2:
			m_ListCtrl2.SetItemText(0, 2, L"服务开始");		break;
		case 3:
			m_ListCtrl2.SetItemText(0, 2, L"服务正在停止");	break;
		case 4:
			m_ListCtrl2.SetItemText(0, 2, L"正在运行");		break;
		case 5:
			m_ListCtrl2.SetItemText(0, 2, L"服务即将继续");	break;
		case 6:
			m_ListCtrl2.SetItemText(0, 2, L"服务正在暂停");	break;
		case 7:
			m_ListCtrl2.SetItemText(0, 2, L"服务暂停");   	break;
		}
		//服务类型
		switch (pEnumService[i].ServiceStatusProcess.dwServiceType)
		{
		case 0x00000002:
			m_ListCtrl2.SetItemText(0, 1, L"文件系统驱动"); break;
		case 0x00000001:
			m_ListCtrl2.SetItemText(0, 1, L"设备驱动程序"); break;
		case 0x00000010:
			m_ListCtrl2.SetItemText(0, 1, L"独立进程");    break;
		case 0x00000020:
			m_ListCtrl2.SetItemText(0, 1, L"共享进程");    break;
		}
	};
	////////////////////////MyListCtrl3////////////////////////////
	//获取当前控件的扩展风格
	DWORD dwOldStyle2 = m_ListCtrl3.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_ListCtrl3.SetExtendedStyle(dwOldStyle2 |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES);        //网格线
	//网格线只适用与report风格的listctrl


	//获取list的宽度
	CRect rc2;
	m_ListCtrl3.GetClientRect(rc2);
	int nWidth3 = rc2.Width();
	//为List插入列消息
	m_ListCtrl3.InsertColumn(0,	  //当前列编号               
		L"窗口名称",				  //要求插入的信息
		0,					 	  //风格
		nWidth3 / 2);		      //列的宽度
	m_ListCtrl3.InsertColumn(1, L"窗口类名", 0, nWidth3 / 2);
	DeskTopWindows();
	CPUmessage();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
//显示桌面窗口
void CMyTaskmgr::DeskTopWindows() 
{
	EnumWindows(EnumWindowsProc, (LPARAM)this);
}
//枚举窗口的回调函数
BOOL CALLBACK CMyTaskmgr::EnumWindowsProc(HWND hwnd, LPARAM lpram)
{
	CMyTaskmgr* lpram1 = (CMyTaskmgr*)lpram;
	CString strTemp;
	WCHAR text[255] = {};
	WCHAR text2[255] = {};
	::GetWindowText(hwnd, text, 255);
	::GetClassName(hwnd, text2, 255);
	if (/*::IsWindowVisible(hwnd) == TRUE&&*/::GetWindow(hwnd, GW_OWNER) == NULL&&::GetWindow(hwnd, GW_CHILD) != NULL)//去掉注释部分则隐藏的窗口将不显示
	{
		strTemp.Format(L"%s", text);
		lpram1->m_ListCtrl3.InsertItem(0, strTemp);
		strTemp.Format(L"%s", text2);
		lpram1->m_ListCtrl3.SetItemText(0, 1, strTemp);
	}
	return TRUE;
}
//CPU使用率
void CMyTaskmgr::CPUmessage()
{
	double  idletime = 0.0, kerneltime = 0.0, usertime = 0.0, idletime1 = 0.0, kerneltime1 = 0.0, usertime1 = 0.0;
	//第一次获取系统时间
	FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	//创建内核对象，并等待1000ms
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);
	//第二次获取系统时间
	FILETIME preidleTime, prekernelTime, preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);
	idletime = (double)(idleTime.dwHighDateTime*4.294967296E9) + (double)idleTime.dwLowDateTime;
	kerneltime = (double)(kernelTime.dwHighDateTime*4.294967296E9) + (double)kernelTime.dwLowDateTime;
	usertime = (double)(userTime.dwHighDateTime*4.294967296E9) + (double)userTime.dwLowDateTime;
	idletime1 = (double)(preidleTime.dwHighDateTime*4.294967296E9) + (double)preidleTime.dwLowDateTime;
	kerneltime1 = (double)(prekernelTime.dwHighDateTime*4.294967296E9) + (double)prekernelTime.dwLowDateTime;
	usertime1 = (double)(preuserTime.dwHighDateTime*4.294967296E9) + (double)preuserTime.dwLowDateTime;
	int num = (int)(100 - (idletime1 - idletime) / (kerneltime1 - kerneltime + usertime1 - usertime) * 100) ;
	m_ProgressCtrl.OffsetPos(num);

}



