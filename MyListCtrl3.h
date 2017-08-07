#pragma once


// CMyLiatCtrl3

class CMyListCtrl3 : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl3)

public:
	CMyListCtrl3();
	virtual ~CMyListCtrl3();
	
protected:
	DECLARE_MESSAGE_MAP()
	void OnNMRClickList(UINT nFlags, CPoint point);

};


