// MyTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTabCtrl 消息处理程序




void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	int nNum = GetCurSel();   //获取Tab控件当前选择的是第几项
	switch (nNum)         //通过获取的选项数，分别显示对应的对话框
	{
	case 0:
	{
		MyDialog[0]->ShowWindow(SW_SHOW);
		MyDialog[1]->ShowWindow(SW_HIDE);
		MyDialog[2]->ShowWindow(SW_HIDE);
		MyDialog[3]->ShowWindow(SW_HIDE);
	}
	break;
	case 1:
	{
		MyDialog[0]->ShowWindow(SW_HIDE);
		MyDialog[1]->ShowWindow(SW_SHOW);
		MyDialog[2]->ShowWindow(SW_HIDE);
		MyDialog[3]->ShowWindow(SW_HIDE);
	}
	break;
	case 2:
	{
		MyDialog[0]->ShowWindow(SW_HIDE);
		MyDialog[1]->ShowWindow(SW_HIDE);
		MyDialog[2]->ShowWindow(SW_SHOW);
		MyDialog[3]->ShowWindow(SW_HIDE);
	}
	break;
	case 3:
	{
		MyDialog[0]->ShowWindow(SW_HIDE);
		MyDialog[1]->ShowWindow(SW_HIDE);
		MyDialog[2]->ShowWindow(SW_HIDE);
		MyDialog[3]->ShowWindow(SW_SHOW);
	}
	break;
	default:break;
	}

}
