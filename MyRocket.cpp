// MyRocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "MyRocket.h"
#include "afxdialogex.h"
#include <winbase.h>
#include <Psapi.h>


// CMyRocket �Ի���

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


// CMyRocket ��Ϣ�������


BOOL CMyRocket::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//����ڴ���Ϣ�ڻ����
	MemoryMessage();
	// ��ȡͼƬ��С��Ϣ
	BOOL h = m_bitmap.LoadBitmap(IDB_BITMAP1);
 	m_bitmap.GetBitmap(&m_bitinfo);
	/*HBITMAP hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), (TCHAR*)IDB_BITMAP1);*/
	//GetObject(hBitmap, sizeof(BITMAP), &m_bitinfo);
	// ���ô��ں�ͼƬһ����
	CRect rt;
	hDeskTopWnd = ::GetDesktopWindow();
	::GetWindowRect(hDeskTopWnd, &rt);
	rt.left = rt.right*0.5;
	rt.right = rt.left + m_bitinfo.bmWidth;
	rt.top = rt.bottom*0.6;
	rt.bottom = rt.top + m_bitinfo.bmHeight;
	MoveWindow(&rt);
	ShowWindow(SW_SHOWNORMAL);
	// ���ô��ڷ��Ϊ��͸��
	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// ����͸����
	SetLayeredWindowAttributes(RGB(255, 255, 255), 0, LWA_COLORKEY);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CMyRocket::OnPaint()
{
	// TODO:  �ڴ˴������Ϣ����������
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(m_bitmap);
	dc.BitBlt(0, 0, m_bitinfo.bmWidth, m_bitinfo.bmHeight, &dcMem, 0, 0, SRCCOPY);

	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CMyRocket::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
	
	//��������ڴ�
	CleanMemory();
	//�������
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
	//�ڴ�ʹ����
	MEMORYSTATUS *lpBuffer = new MEMORYSTATUS[];
	GlobalMemoryStatus(lpBuffer);
	m_MemoryEdit = (int)lpBuffer->dwMemoryLoad;
	UpdateData(FALSE);
}

void CMyRocket::CleanMemory()
{
	//��ȡ��ǰ���ڴ�״̬
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem =
		stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	//�����ڴ�
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	//��ȡ�������ڴ�״̬
	GlobalMemoryStatusEx(&stcMemStatusEx);
	//DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	m_MemoryEdit = (int)stcMemStatusEx.dwMemoryLoad;
	UpdateData(FALSE);
}