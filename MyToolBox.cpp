// MyToolBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "MyToolBox.h"
#include "afxdialogex.h"
#include "MyRocket.h"
#include "Rocket.h"
// CMyToolBox �Ի���

IMPLEMENT_DYNAMIC(CMyToolBox, CDialogEx)

CMyToolBox::CMyToolBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyToolBox::IDD, pParent)
{

}

CMyToolBox::~CMyToolBox()
{
}

void CMyToolBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyToolBox, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyToolBox::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMyToolBox ��Ϣ�������

//С�����ť
void CMyToolBox::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMyRocket  m_Rocket;
 	m_Rocket.MessageBox(L"123");
}


BOOL CMyToolBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
