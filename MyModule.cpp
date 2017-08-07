// MyModule.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "MyModule.h"
#include "afxdialogex.h"
#include <locale.h>
#include <TlHelp32.h>
#include "MyTaskmgr.h"
#include "MyListCtrl1.h"
#include "MyListModule.h"
// CMyModule �Ի���

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


// CMyModule ��Ϣ�������


BOOL CMyModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	///////////////////////////////////////////
	//��ȡ��ǰ�ؼ�����չ���
	DWORD dwOldStyle = m_module.GetExtendedStyle();          //��ȡԭ���
	//���õ�ǰ�ؼ�����չ���
	m_module.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //������ͼ��ʽ
		LVS_EX_GRIDLINES          //������
		//������ֻ������report����listctrl
		);            //itemǰ����CheckBox�ؼ�
	//��ȡlist�Ŀ��
	CRect rc;
	m_module.GetClientRect(rc);
	int nWidth = rc.Width();
	//ΪList��������Ϣ
	m_module.InsertColumn(0,	        //��ǰ�б��
		L"ģ����",						//Ҫ��������Ϣ
		0,								//���
		nWidth / 4);					//�еĿ��
	m_module.InsertColumn(1, L"ģ���С", 0, nWidth / 4);
	m_module.InsertColumn(2, L"ģ��·��", 0, nWidth / 2);
	// ����ģ�����
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
		dw);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		m_module.MessageBox(L"����ģ����ճ���");
	}
	// ��ʼ��ģ����Ϣ�ṹ��
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}
