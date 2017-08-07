// MyToolBox.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyToolBox.h"
#include "afxdialogex.h"
#include "MyRocket.h"
#include "Rocket.h"
// CMyToolBox 对话框

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


// CMyToolBox 消息处理程序

//小火箭按钮
void CMyToolBox::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	CMyRocket  m_Rocket;
 	m_Rocket.MessageBox(L"123");
}


BOOL CMyToolBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
