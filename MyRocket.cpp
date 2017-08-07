// MyRocket.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyRocket.h"
#include "afxdialogex.h"
#include <winbase.h>
#include <Psapi.h>


// CMyRocket 对话框

IMPLEMENT_DYNAMIC(CMyRocket, CDialogEx)

CMyRocket::CMyRocket(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyRocket::IDD, pParent)
	, m_MemoryEdit(0)
{
	ZeroMemory(&m_bitinfo, sizeof(m_bitinfo));
}

CMyRocket::~CMyRocket()
{
}

void CMyRocket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MemoryEdit);
}


BEGIN_MESSAGE_MAP(CMyRocket, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMyRocket 消息处理程序


BOOL CMyRocket::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	//输出内存信息在火箭上
	MemoryMessage();
	// 获取图片大小信息
	BOOL h = m_bitmap.LoadBitmap(IDB_BITMAP1);
 	m_bitmap.GetBitmap(&m_bitinfo);
	/*HBITMAP hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), (TCHAR*)IDB_BITMAP1);*/
	//GetObject(hBitmap, sizeof(BITMAP), &m_bitinfo);
	// 设置窗口和图片一样大
	CRect rt;
	hDeskTopWnd = ::GetDesktopWindow();
	::GetWindowRect(hDeskTopWnd, &rt);
	rt.left = rt.right*0.5;
	rt.right = rt.left + m_bitinfo.bmWidth;
	rt.top = rt.bottom*0.6;
	rt.bottom = rt.top + m_bitinfo.bmHeight;
	MoveWindow(&rt);
	ShowWindow(SW_SHOWNORMAL);
	// 设置窗口风格为可透明
	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// 设置透明度
	SetLayeredWindowAttributes(RGB(255, 255, 255), 0, LWA_COLORKEY);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CMyRocket::OnPaint()
{
	// TODO:  在此处添加消息处理程序代码
	CPaintDC dc(this); // 用于绘制的设备上下文
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(m_bitmap);
	dc.BitBlt(0, 0, m_bitinfo.bmWidth, m_bitinfo.bmHeight, &dcMem, 0, 0, SRCCOPY);

	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void CMyRocket::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
	
	//点击清理内存
	CleanMemory();
	//火箭运行
	CRect tempRect, oldRect;
	GetWindowRect(&oldRect);
	tempRect = oldRect;
	for (int i = tempRect.bottom; i > (tempRect.bottom - tempRect.top); i--)
	{
		MoveWindow(&tempRect);
		tempRect.top -= 10;
		tempRect.bottom -= 10;
		Sleep(5);
	}
	MoveWindow(&oldRect);





	/////////////////////////////////////////////
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMyRocket::MemoryMessage()
{
	//内存使用率
	MEMORYSTATUS *lpBuffer = new MEMORYSTATUS[];
	GlobalMemoryStatus(lpBuffer);
	m_MemoryEdit = (int)lpBuffer->dwMemoryLoad;
	UpdateData(FALSE);
}

void CMyRocket::CleanMemory()
{
	//获取当前的内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem =
		stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	//清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	//获取清理后的内存状态
	GlobalMemoryStatusEx(&stcMemStatusEx);
	//DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	m_MemoryEdit = (int)stcMemStatusEx.dwMemoryLoad;
	UpdateData(FALSE);
}