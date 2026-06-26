#pragma once


// CLBparent
#include "History.h"
#include "CCodeListCtrl.h"
#include "CHMenu.h"

class CLBparent : public CWnd
{
	DECLARE_DYNAMIC(CLBparent)

public:
	CLBparent();
	virtual ~CLBparent();


	BOOL CreatePopUpWindow(CWnd* pParent, CRect rec);
	CWnd* m_child{};

	BOOL CreateListBox(CString items);

	void InitMenu();
	CArray	< CHMenu*, CHMenu* > m_arMenuT{};
	BOOL _bRightPop = TRUE;
	BOOL MakePopMenu(CString sData, CPoint point);

	CCodeListCtrl* m_pCodelist{};
	CImageList m_imageList{};
	CString m_ClickCode{};
protected:
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);


public:
	CString m_slog{};
	CString m_sVal{};  //º¯Á¶
};


