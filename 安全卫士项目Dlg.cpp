
// ��ȫ��ʿ��ĿDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "��ȫ��ʿ��ĿDlg.h"
#include "afxdialogex.h"
#include "MyTabCtrl.h"
#include "MyTaskmgr.h"
#include "MyCleanTrash.h"
#include "MyVirusScan.h"
#include "MyTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C��ȫ��ʿ��ĿDlg �Ի���



C��ȫ��ʿ��ĿDlg::C��ȫ��ʿ��ĿDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C��ȫ��ʿ��ĿDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C��ȫ��ʿ��ĿDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);
}

BEGIN_MESSAGE_MAP(C��ȫ��ʿ��ĿDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// C��ȫ��ʿ��ĿDlg ��Ϣ�������

BOOL C��ȫ��ʿ��ĿDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_tabctrl.InsertItem(0, L"���������");   //�������������ѡ�����
	m_tabctrl.InsertItem(1, L"������ɱ");
	m_tabctrl.InsertItem(2, L"��������");
	m_tabctrl.InsertItem(3, L"������");
	//���Ӵ���ָ�븳ֵ
	m_tabctrl.MyDialog[0] = new CMyTaskmgr();
	m_tabctrl.MyDialog[1] = new CMyVirusScan();
	m_tabctrl.MyDialog[2] = new CMyCleanTrash();
	m_tabctrl.MyDialog[3] = new CMyTool();
	//�����Ӵ���
	m_tabctrl.MyDialog[0]->Create(IDD_DIALOG1, &m_tabctrl);
	m_tabctrl.MyDialog[1]->Create(IDD_DIALOG2, &m_tabctrl);
	m_tabctrl.MyDialog[2]->Create(IDD_DIALOG3, &m_tabctrl);
	m_tabctrl.MyDialog[3]->Create(IDD_DIALOG4, &m_tabctrl);
	//���������Ӵ���
	CRect  rc;
	m_tabctrl.GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);    // �ӶԻ����������Ϸֱ������ľ���
	m_tabctrl.MyDialog[0]->MoveWindow(rc);
	m_tabctrl.MyDialog[1]->MoveWindow(rc);
	m_tabctrl.MyDialog[2]->MoveWindow(rc);
	m_tabctrl.MyDialog[3]->MoveWindow(rc);
	//Ĭ����ʾ��һ���Ӵ���
	m_tabctrl.MyDialog[0]->ShowWindow(SW_SHOW);
	m_tabctrl.MyDialog[1]->ShowWindow(SW_HIDE);
	m_tabctrl.MyDialog[2]->ShowWindow(SW_HIDE);
	m_tabctrl.MyDialog[3]->ShowWindow(SW_HIDE);

	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C��ȫ��ʿ��ĿDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C��ȫ��ʿ��ĿDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void C��ȫ��ʿ��ĿDlg::BossKey()
{
	//1.ע��ȫ���ȼ�
	::RegisterHotKey(this->GetSafeHwnd(),//��ȡ��ǰ���ڵľ��
					0Xa001,				 //�Զ����ݼ���ʶ
					MOD_CONTROL | MOD_SHIFT,//ͬʱ����Ctrl,shift
					'K'					  //����ΪCtrl+shift+K                    
					);
	//MSG* mMsg;
	//PreTranslateMessage(mMsg);
}


BOOL C��ȫ��ʿ��ĿDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	//�ж�ȫ���ȼ�ͬʱҲ�ǵ�ǰע��ļ�
	if ((pMsg->message==WM_HOTKEY)&&(pMsg->wParam==0xa001))
	{
		//���ش���
		if (m_IsWindowHide==TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		//��ʾ���ڵĴ���
		else
		{
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
