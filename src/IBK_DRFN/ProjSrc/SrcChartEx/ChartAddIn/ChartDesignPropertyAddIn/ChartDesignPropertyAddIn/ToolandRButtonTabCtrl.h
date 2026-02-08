#if !defined(AFX_TOOLANDRBUTTONTABCTRL_H__868CD342_C32E_491C_8841_E7119CE66D1D__INCLUDED_)
#define AFX_TOOLANDRBUTTONTABCTRL_H__868CD342_C32E_491C_8841_E7119CE66D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolandRButtonTabCtrl.h : header file
//

#include "ToolandRButtonItemMgr.h"
// ADD: 종찬(04/03/17) 수치조회관련
#include "NumericalInquiryOptionDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonTabCtrl window

class CToolandRButtonDlgwithGrid_ListCtrl;
class CToolandRButtonTabCtrl : public CTabCtrl
{
// Construction
public:
	CToolandRButtonTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolandRButtonTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolandRButtonTabCtrl();

	//부모로부터 저장되어 있던 Data를 받는다.
	void SetSavedDataFromParent(const CString& strSavedData);
	// ADD: 종찬(04/03/17) 수치조회관련(오버롸이뒹)
	void SetSavedDataFromParent(const int nOption);
	//멤버변수로 저장되어 있는 data를 해당 Dlg에 적용시킨다.
	void SetSavedDataToDlgwithListCtrl();
	//설정되어 있는 리스트컨트롤의 모든 값들을 얻는다.
	CString GetListItemInfo();
	// ADD: 종찬(04/03/17) 수치조회관련 옵션값을 얻는다.
	int GetNumericalInquiryOption();
	//DragandDrop을 위해서 초기화한다.
	void InitDropTarget();
	//Tab위에 위치할 리스트를 create한다.
	void InitTab();
	// ADD: 종찬(04/03/17) 수치조회관련. 내부툴바의 아이템 정보 문자열을 가져온다.
	CString GetMiniToolBarItem(void);

	void SetTabFocus();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolandRButtonTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//설정되어 있던 저장값을 부모로부터 받아서 현 class에 저장되어 있다.
	CString m_strSavedDataFromParent;
	//모든 Item정보들을 가지고 있고, 그 정보를 얻어올 수 있는 클래스이다.
	CToolandRButtonItemMgr m_ItemInfoManager;

	//여러개의 Tab 중에서 Active 되어 있는 tab의 번호이다.
	int m_tabCurrent;
	//몇개의 tab을 가지고 있는지 알려준다.
	int m_nNumberOfPages;
	//tab에 위치할 Dlg을 생성해서 저장한다.
	CToolandRButtonDlgwithGrid_ListCtrl *m_tabPages[3];

	// ADD: 종찬(04/03/17)수치조회관련옵션추가
	CNumericalInquiryOptionDlg m_DlgNumericalInquiryOption;

	//생성된 Dlg를 Tab위에 위치 시키기 위해서 위치를 계산한다.
	void SetRectangle();

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLANDRBUTTONTABCTRL_H__868CD342_C32E_491C_8841_E7119CE66D1D__INCLUDED_)
