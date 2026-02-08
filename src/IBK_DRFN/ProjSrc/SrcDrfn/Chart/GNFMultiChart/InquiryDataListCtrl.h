#if !defined(AFX_INQUIRYDATALISTCTRL_H__4363764A_68ED_4311_8B84_D24D46C82843__INCLUDED_)
#define AFX_INQUIRYDATALISTCTRL_H__4363764A_68ED_4311_8B84_D24D46C82843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InquiryDataListCtrl.h : header file
// 수치조회창에서 사용되는 리스트컨트롤로써 수치조회창(Window, Simple Window)타입이고
// '자동'이동형태 일 경우에 마우스포인터가 수치조회창에 위치하면 WM_MOUSEMOVE메세지가
// 수치조회창의 리스트컨트롤에서 발생한다. 이렇게 발생된 메세지를 다시 
// CToolBarManagerImplementation클래스에서 처리하도록 한다. 
// PS. 흘~ 복잡해. 복잡해.

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataListCtrl window

class CInquiryDataListCtrl : public CListCtrl
{
// Construction
public:
	CInquiryDataListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInquiryDataListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInquiryDataListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInquiryDataListCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2006/2/15 - Seung-Won, Bae) Manage Origianl Point of Drag
protected:
	CPoint	m_ptStart;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INQUIRYDATALISTCTRL_H__4363764A_68ED_4311_8B84_D24D46C82843__INCLUDED_)
