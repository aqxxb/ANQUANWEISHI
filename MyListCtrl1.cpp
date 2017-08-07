// MyListCtrl1.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyListCtrl1.h"
#include <afxmsg_.h>
#include "MyModule.h"
#include <afxcmn.h>
#include <afxwin.h>
#include "MyListModule.h"
#include "MyThread.h"
#include "MyTaskmgr.h"
// CMyListCtrl1

IMPLEMENT_DYNAMIC(CMyListCtrl1, CListCtrl)

CMyListCtrl1::CMyListCtrl1()
{

}

CMyListCtrl1::~CMyListCtrl1()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl1, CListCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMyListCtrl1::OnNMRClick)
	ON_COMMAND(ID_32771, &CMyListCtrl1::On32771)
	ON_COMMAND(ID_32772, &CMyListCtrl1::On32772)
END_MESSAGE_MAP()



// CMyListCtrl1 消息处理程序


void CMyListCtrl1::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	// TODO:  在此添加控件通知处理程序代码
	CMenu  menu; //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_MENU1); //装载自定义的右键菜单
	CMenu  *pContextMenu = menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单
	CPoint point1;//定义一个用于确定光标位置的位置  
	GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标 
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //在指定位置显示弹出菜单*/
	func();
}
//模块信息
void CMyListCtrl1::On32771()
{
	CMyModule m_module;
	m_module.dw = func();
	m_module.DoModal();
}
//线程信息
void CMyListCtrl1::On32772()
{
	CMyThread m_thread;
	m_thread.dw1 = func();
	m_thread.DoModal();
}
//获取List控件被点击行的指定位置的内容
DWORD CMyListCtrl1:: func()
{
	int nSel = GetSelectionMark();
	if (nSel == -1)
	{
		return -1;
	}
	CString str;
	str = GetItemText(nSel , 1);
	DWORD num = (DWORD)_ttoi64(str);
	return num;
};


