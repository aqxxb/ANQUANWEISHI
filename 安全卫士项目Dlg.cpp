
// 安全卫士项目Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "安全卫士项目Dlg.h"
#include "afxdialogex.h"
#include "MyTabCtrl.h"
#include "MyTaskmgr.h"
#include "MyCleanTrash.h"
#include "MyVirusScan.h"
#include "MyTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C安全卫士项目Dlg 对话框



C安全卫士项目Dlg::C安全卫士项目Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C安全卫士项目Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C安全卫士项目Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);
}

BEGIN_MESSAGE_MAP(C安全卫士项目Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// C安全卫士项目Dlg 消息处理程序

BOOL C安全卫士项目Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_tabctrl.InsertItem(0, L"任务管理器");   //用其类对象设置选项标题
	m_tabctrl.InsertItem(1, L"病毒查杀");
	m_tabctrl.InsertItem(2, L"垃圾清理");
	m_tabctrl.InsertItem(3, L"工具箱");
	//给子窗口指针赋值
	m_tabctrl.MyDialog[0] = new CMyTaskmgr();
	m_tabctrl.MyDialog[1] = new CMyVirusScan();
	m_tabctrl.MyDialog[2] = new CMyCleanTrash();
	m_tabctrl.MyDialog[3] = new CMyTool();
	//创建子窗口
	m_tabctrl.MyDialog[0]->Create(IDD_DIALOG1, &m_tabctrl);
	m_tabctrl.MyDialog[1]->Create(IDD_DIALOG2, &m_tabctrl);
	m_tabctrl.MyDialog[2]->Create(IDD_DIALOG3, &m_tabctrl);
	m_tabctrl.MyDialog[3]->Create(IDD_DIALOG4, &m_tabctrl);
	//控制两个子窗口
	CRect  rc;
	m_tabctrl.GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);    // 子对话框左下右上分别缩进的距离
	m_tabctrl.MyDialog[0]->MoveWindow(rc);
	m_tabctrl.MyDialog[1]->MoveWindow(rc);
	m_tabctrl.MyDialog[2]->MoveWindow(rc);
	m_tabctrl.MyDialog[3]->MoveWindow(rc);
	//默认显示第一个子窗口
	m_tabctrl.MyDialog[0]->ShowWindow(SW_SHOW);
	m_tabctrl.MyDialog[1]->ShowWindow(SW_HIDE);
	m_tabctrl.MyDialog[2]->ShowWindow(SW_HIDE);
	m_tabctrl.MyDialog[3]->ShowWindow(SW_HIDE);

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C安全卫士项目Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C安全卫士项目Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C安全卫士项目Dlg::BossKey()
{
	//1.注册全局热键
	::RegisterHotKey(this->GetSafeHwnd(),//获取当前窗口的句柄
					0Xa001,				 //自定义快捷键标识
					MOD_CONTROL | MOD_SHIFT,//同时按下Ctrl,shift
					'K'					  //设置为Ctrl+shift+K                    
					);
	//MSG* mMsg;
	//PreTranslateMessage(mMsg);
}


BOOL C安全卫士项目Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	//判断全局热键同时也是当前注册的键
	if ((pMsg->message==WM_HOTKEY)&&(pMsg->wParam==0xa001))
	{
		//隐藏窗口
		if (m_IsWindowHide==TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		//显示窗口的代码
		else
		{
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
