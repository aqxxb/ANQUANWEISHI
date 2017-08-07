// MyModule.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyModule.h"
#include "afxdialogex.h"
#include <locale.h>
#include <TlHelp32.h>
#include "MyTaskmgr.h"
#include "MyListCtrl1.h"
#include "MyListModule.h"
// CMyModule 对话框

IMPLEMENT_DYNAMIC(CMyModule, CDialogEx)

CMyModule::CMyModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyModule::IDD, pParent)
{

}

CMyModule::~CMyModule()
{
}

void CMyModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, m_module);
}


BEGIN_MESSAGE_MAP(CMyModule, CDialogEx)
END_MESSAGE_MAP()


// CMyModule 消息处理程序


BOOL CMyModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	///////////////////////////////////////////
	//获取当前控件的扩展风格
	DWORD dwOldStyle = m_module.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_module.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线
		//网格线只适用与report风格的listctrl
		);            //item前生成CheckBox控件
	//获取list的宽度
	CRect rc;
	m_module.GetClientRect(rc);
	int nWidth = rc.Width();
	//为List插入列消息
	m_module.InsertColumn(0,	        //当前列编号
		L"模块名",						//要求插入的信息
		0,								//风格
		nWidth / 4);					//列的宽度
	m_module.InsertColumn(1, L"模块大小", 0, nWidth / 4);
	m_module.InsertColumn(2, L"模块路径", 0, nWidth / 2);
	// 创建模块快照
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
		dw);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		m_module.MessageBox(L"创建模块快照出错！");
	}
	// 初始化模块信息结构体
	MODULEENTRY32 moduleInfo = {sizeof(MODULEENTRY32)};
	Module32First(hModuleSnap, &moduleInfo);
	do
	{
		
		if (dw == moduleInfo.th32ProcessID)
		{
			CString str, str1, str2;
			str.Format(_T("%s"), moduleInfo.szModule);
			m_module.InsertItem(0, str);
			str1.Format(_T("%d"), moduleInfo.modBaseSize);
			m_module.SetItemText(0, 1, str1);
			str2.Format(_T("%s"), moduleInfo.szExePath);
			m_module.SetItemText(0, 2, str2);
		}
	} while (Module32Next(hModuleSnap, &moduleInfo));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
