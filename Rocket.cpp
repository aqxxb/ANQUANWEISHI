// Rocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "Rocket.h"
#include "afxdialogex.h"


// CRocket �Ի���

IMPLEMENT_DYNAMIC(CRocket, CDialogEx)

CRocket::CRocket(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRocket::IDD, pParent)
{

}

CRocket::~CRocket()
{
}

void CRocket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRocket, CDialogEx)
END_MESSAGE_MAP()


// CRocket ��Ϣ�������
