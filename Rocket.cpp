// Rocket.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "Rocket.h"
#include "afxdialogex.h"


// CRocket 对话框

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


// CRocket 消息处理程序
