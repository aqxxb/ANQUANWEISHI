#pragma once


// CMyListModule

class CMyListModule : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListModule)

public:
	CMyListModule();
	virtual ~CMyListModule();

protected:
	DECLARE_MESSAGE_MAP()
};


