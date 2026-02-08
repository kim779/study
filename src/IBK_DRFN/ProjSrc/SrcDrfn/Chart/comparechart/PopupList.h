#if !defined(AFX_POPUPLIST_H__5A511268_124A_4B8B_8177_575E4AFAE751__INCLUDED_)
#define AFX_POPUPLIST_H__5A511268_124A_4B8B_8177_575E4AFAE751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupList.h : header file
//
#include "./control/RscDlg.h"
#include "resource.h"
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CPopupList dialog
#define CHistoryList CList<CHistory*, CHistory*> 
#define WM_POPUPLIST_SELITEM WM_USER + 77
#define MAX_ITMESIZE 10 //한번에 보여줄수 있는 아이템을 10개로 제한 - 10개가 넘으면 스크롤 생김

class CHistory
{
public:
	long _Type;
	CString _Code;
	CString _Name;
	CHistory(long nType, LPCTSTR szCode, LPCTSTR szName)
	{
		_Type = nType;
		_Code = szCode;
		_Name = szName;
	}
};


class CPopupList : public CRscDlg
{
// Construction
public:
	CPopupList(CHistoryList* HistoryInfo, int type = 0, CWnd* pParent = NULL);   // standard constructor

	char			m_Code[30];
	CWnd*			m_pParent;						// 부모 윈도우 핸들

	int				m_lstType;						// 0이면 코드, 1이면 계좌, 2이면 선택(주문종류)을 위한 것
	CHistoryList*	m_lstItemInfo;					// 리스트에 넣을 목록
	void			MakeListCtrlColumn();			// 리스트의 컬럼을 만듬	
	void			MakeInitHistoryIntoListCtrl();	// 리스트에 항목을 넣음
	void			AdjustListCtrl();				// 아이템의 갯수만큼 사이즈를 조절-스크롤 안생기도록

// Dialog Data
	//{{AFX_DATA(CPopupList)
	enum { IDD = IDD_POPUPLIST };
	CListCtrl	m_lstPopup;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopupList)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPLIST_H__5A511268_124A_4B8B_8177_575E4AFAE751__INCLUDED_)
