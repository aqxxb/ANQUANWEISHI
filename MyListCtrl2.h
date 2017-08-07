#pragma once


// CMyListCtrl2

class CMyListCtrl2 : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl2)

public:
	CMyListCtrl2();
	virtual ~CMyListCtrl2();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void On32774();
	afx_msg void On32775();
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateService();
};


