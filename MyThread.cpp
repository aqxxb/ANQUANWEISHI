// MyThread.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyThread.h"
#include "afxdialogex.h"
#include "MyListCtrl1.h"
#include <TlHelp32.h>


// CMyThread 对话框

IMPLEMENT_DYNAMIC(CMyThread, CDialogEx)

CMyThread::CMyThread(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyThread::IDD, pParent)
{

}

CMyThread::~CMyThread()
{
}

void CMyThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, m_thread);
}


BEGIN_MESSAGE_MAP(CMyThread, CDialogEx)
END_MESSAGE_MAP()


// CMyThread 消息处理程序


BOOL CMyThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	///////////////////////////////////////////
	//获取当前控件的扩展风格
	DWORD dwOldStyle = m_thread.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_thread.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线
		//网格线只适用与report风格的listctrl
		);          
	//获取list的宽度
	CRect rc;
	m_thread.GetClientRect(rc);
	int nWidth = rc.Width();
	//为List插入列消息
	m_thread.InsertColumn(0,	        //当前列编号
		L"进程ID",       //要求插入的信息
		0,	          //风格
		nWidth / 3);//列的宽度
	m_thread.InsertColumn(1, L"线程ID", 0, nWidth / 3);
	m_thread.InsertColumn(2, L"优先级", 0, nWidth / 3);

	// 创建线程快照
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,
		dw1);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		m_thread.MessageBox(L"创建线程快照出错！");
	}
	// 初始化模块信息结构体
	THREADENTRY32 threadInfo = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &threadInfo);
	do
	{
		
		if (dw1 == threadInfo.th32OwnerProcessID)
		{
			CString str, str1, str2;
			str.Format(_T("%d"), threadInfo.th32OwnerProcessID );
			m_thread.InsertItem(0, str);
			str1.Format(_T("%d"), threadInfo.th32ThreadID);
			m_thread.SetItemText(0, 1, str1);
			str2.Format(_T("%d"), threadInfo.tpBasePri);
			m_thread.SetItemText(0, 2, str2);
		}
	} while (Thread32Next(hThreadSnap, &threadInfo));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
