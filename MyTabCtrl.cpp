// MyTabCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
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



// CMyTabCtrl ��Ϣ�������




void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	int nNum = GetCurSel();   //��ȡTab�ؼ���ǰѡ����ǵڼ���
	switch (nNum)         //ͨ����ȡ��ѡ�������ֱ���ʾ��Ӧ�ĶԻ���
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
