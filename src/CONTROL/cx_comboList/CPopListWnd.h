#pragma once


// CLBparent

#include "CCodeListCtrl.h"
#include "CHMenu.h"
#include "fxImgButton.h"




class CPopListWnd : public CWnd
{
	DECLARE_DYNAMIC(CPopListWnd)

public:
	CPopListWnd();
	virtual ~CPopListWnd();

	int m_nType = VS1_TYPE;  // 기본값 VS1_TYPE
	void SetType(int nType) { m_nType = nType; }
	int  GetType() { return m_nType; }

	BOOL CreatePopUpWindow(CWnd* pParent, CRect rec);
	CWnd* m_child{};
	class CControlWnd* m_pParent{};

	BOOL CreateListBox(CString items);

	void InitMenu();
	CArray	< CHMenu*, CHMenu* > m_arMenuT{};
	BOOL _bRightPop = TRUE;
	BOOL MakePopMenu(CString sData, CPoint point);

	std::unique_ptr<CCodeListCtrl> m_pCodelist{};
	CImageList m_imageList{};
	std::unique_ptr<CfxImgButton> m_pBtnClear{};


	CString m_ClickCode{};



protected:
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnClear();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);


public:
	CString m_slog{};
	CString m_sVal{};  //변조
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);

};


