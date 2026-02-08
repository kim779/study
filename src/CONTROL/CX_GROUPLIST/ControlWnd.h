#pragma once
// ControlWnd.h : header file
//

#include "../../h/axisfire.h"

#include "GroupListCtrl.h"

typedef struct _item_
{
	CString brAccn;		// 관리지점번호
	CString accn;		// 계좌번호
	CString accnName;	// 계좌명
	CString code;		// 종목번호
	CString codeName;	// 종목명
	CString gubn;		// 매매구분 
	CString jucode;		// 주문유형코드
	CString juName;		// 주문유형명
	CString juNumber;	// 주문량
	CString chNumber;	// 체결량	
	CString noChNumber;	// 미체결량
	CString chPrice;	// 체결평균가
	CString chAmount;	// 체결금액
} sItem;

class CParam
{
public:
	int		key = 0;		// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point = 0;		// font point
	int		style = 0;		// font style
	DWORD		tRGB = 0;		// text color index
	DWORD		pRGB = 0;		// paint color index
	CString		options;		// properties
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(struct _param* pParam, CWnd* pWizard);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL
	
protected:
	std::unique_ptr<CGroupListCtrl> m_list;
	CWnd*	m_pWizard;
	CParam	m_param;

	void	GetString(CString str, sItem* p);
	void	AddList(sItem* p);
	void	SetList(sItem* p, int nItem);	
	void	UpdateGroup(sGLI* pg);

	CString MakeMoneyString(CString str);
	CString MakeMoneyString(__int64 dw);
	CString MakeAccnString(CString str);


// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnItemClick(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg void AddRow(LPCTSTR str);
	afx_msg void AddAlert(LPCTSTR str);
	afx_msg void MakeList();
	afx_msg void ReadyList();
	afx_msg void Visible(BOOL bShow);	
	afx_msg void RemoveAlert(LPCTSTR str);
	afx_msg void ExpandTree(BOOL bExpand);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};