// MyThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "MyThread.h"
#include "afxdialogex.h"
#include "MyListCtrl1.h"
#include <TlHelp32.h>


// CMyThread �Ի���

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


// CMyThread ��Ϣ�������


BOOL CMyThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	///////////////////////////////////////////
	//��ȡ��ǰ�ؼ�����չ���
	DWORD dwOldStyle = m_thread.GetExtendedStyle();          //��ȡԭ���
	//���õ�ǰ�ؼ�����չ���
	m_thread.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //������ͼ��ʽ
		LVS_EX_GRIDLINES          //������
		//������ֻ������report����listctrl
		);          
	//��ȡlist�Ŀ��
	CRect rc;
	m_thread.GetClientRect(rc);
	int nWidth = rc.Width();
	//ΪList��������Ϣ
	m_thread.InsertColumn(0,	        //��ǰ�б��
		L"����ID",       //Ҫ��������Ϣ
		0,	          //���
		nWidth / 3);//�еĿ��
	m_thread.InsertColumn(1, L"�߳�ID", 0, nWidth / 3);
	m_thread.InsertColumn(2, L"���ȼ�", 0, nWidth / 3);

	// �����߳̿���
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,
		dw1);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		m_thread.MessageBox(L"�����߳̿��ճ���");
	}
	// ��ʼ��ģ����Ϣ�ṹ��
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}
