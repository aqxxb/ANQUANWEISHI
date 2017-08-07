// MyListCtrl2.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyListCtrl2.h"
#include <winsvc.h>
#include <locale.h>


// CMyListCtrl2

IMPLEMENT_DYNAMIC(CMyListCtrl2, CListCtrl)

CMyListCtrl2::CMyListCtrl2()
{

}

CMyListCtrl2::~CMyListCtrl2()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl2, CListCtrl)
	ON_COMMAND(ID_32774, &CMyListCtrl2::On32774)
	ON_COMMAND(ID_32775, &CMyListCtrl2::On32775)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMyListCtrl2::OnNMRClick)
END_MESSAGE_MAP()



// CMyListCtrl2 消息处理程序



//开启服务
void CMyListCtrl2::On32774()
{
	// TODO:  在此添加命令处理程序代码
	int nSel = GetSelectionMark();
	if (nSel == -1)
	{
		return;
	}
	CString str;
	str = GetItemText(nSel, 0);
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		MessageBox(L"打开错误！");
		return;
	}
	// 打开服务。
	SC_HANDLE hSvc = ::OpenService(hSC, str,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		MessageBox(L"打开错误！");
		::CloseServiceHandle(hSC);
		return;
	}
	if ((::StartService(hSvc, NULL, NULL) == FALSE))
	{
		MessageBox(L"该服务不支持操作！");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
		DeleteAllItems();
		UpdateService();
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);

}
//停止服务
void CMyListCtrl2::On32775()
{
	// TODO:  在此添加命令处理程序代码
	int nSel = GetSelectionMark();
	if (nSel == -1)
	{
		return ;
	}
	CString str;
	str = GetItemText(nSel, 0);
	SC_HANDLE scm;
	SC_HANDLE service;
	if ((scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		CloseServiceHandle(scm);
		return;
	}
	service = OpenService(scm, str, SERVICE_ALL_ACCESS);
	if (!service)
	{
	    //该服务不支持修改
		MessageBox(L"该服务不支持操作！");
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return;
	}

	SERVICE_STATUS  status;
	if (QueryServiceStatus(service, &status))
	{
		if (status.dwCurrentState != SERVICE_STOPPED)
		{
			ControlService(service, SERVICE_CONTROL_STOP, &status);
			SC_LOCK sclLockA;
			sclLockA= LockServiceDatabase(scm);
			if (sclLockA!= NULL){
				if (!ChangeServiceConfig(
					service,
					SERVICE_NO_CHANGE,
					SERVICE_DISABLED,
					SERVICE_NO_CHANGE,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL))
				{
				}
			}

			UnlockServiceDatabase(sclLockA);
		}
		else
		{
			SC_LOCK sclLockB;
			sclLockB= LockServiceDatabase(scm);
			if (sclLockB!= NULL){
				if (!ChangeServiceConfig(
					service,
					SERVICE_NO_CHANGE,
					SERVICE_DISABLED,
					SERVICE_NO_CHANGE,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL))
				{

				}
			}

			UnlockServiceDatabase(sclLockB);
		}
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
	}
	DeleteAllItems();
	UpdateService();

}

void CMyListCtrl2::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	// TODO:  在此添加控件通知处理程序代码
	CMenu  menu; //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_MENU2); //装载自定义的右键菜单
	CMenu  *pContextMenu = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
	CPoint point1;//定义一个用于确定光标位置的位置??
	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标?
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //在指定位置显示弹出菜单*/
}

void CMyListCtrl2::UpdateService()
{
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
	DWORD dwOldStyle1 = GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	SetExtendedStyle(dwOldStyle1 |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线,网格线只适用与report风格的listctrl
		);
	//获取list的宽度
	CRect rc1;
	GetClientRect(rc1);
	int nWidth1 = rc1.Width();
	//为List插入列消息
	InsertColumn(0, L"服务名称", 0, nWidth1 / 3);
	InsertColumn(1, L"服务类型", 0, nWidth1 / 3);
	InsertColumn(2, L"服务状态", 0, nWidth1 / 3);
	//进程信息
	// 设置本地字符集
	setlocale(LC_ALL, "chs");
	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//获取基础服务信息
		//服务名
		InsertItem(0, pEnumService[i].lpServiceName);
		//服务状态
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
		case 1:
			SetItemText(0, 2, L"服务停止");		break;
		case 2:
			SetItemText(0, 2, L"服务开始");		break;
		case 3:
			SetItemText(0, 2, L"服务正在停止");	break;
		case 4:
			SetItemText(0, 2, L"正在运行");		break;
		case 5:
			SetItemText(0, 2, L"服务即将继续");	break;
		case 6:
			SetItemText(0, 2, L"服务正在暂停");	break;
		case 7:
			SetItemText(0, 2, L"服务暂停");   	break;
		}
		//服务类型
		switch (pEnumService[i].ServiceStatusProcess.dwServiceType)
		{
		case 0x00000002:
			SetItemText(0, 1, L"文件系统驱动"); break;
		case 0x00000001:
			SetItemText(0, 1, L"设备驱动程序"); break;
		case 0x00000010:
			SetItemText(0, 1, L"独立进程");    break;
		case 0x00000020:
			SetItemText(0, 1, L"共享进程");    break;
		}
	};
}