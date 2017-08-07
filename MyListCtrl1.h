#pragma once


// CMyListCtrl1

class CMyListCtrl1 : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl1)

public:
	CMyListCtrl1();
	virtual ~CMyListCtrl1();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	void On32771();
	void On32772();
	DWORD func();
	afx_msg void On32773();
};


