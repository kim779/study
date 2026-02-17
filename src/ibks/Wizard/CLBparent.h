#pragma once


// CLBparent
#include "CPopHistory.h"
class CLBparent : public CWnd
{
	DECLARE_DYNAMIC(CLBparent)

public:
	CLBparent();
	virtual ~CLBparent();


	BOOL CreatePopUpWindow(CWnd* pParent, CRect rec);
	CWnd* m_child{};

	CWnd* CreateListBox(class CClient* client, CString items);

	char* m_pbuf{};
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


