#if !defined(AFX_VIEWWRAP_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_)
#define AFX_VIEWWRAP_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewWrap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewWrap window
#include "../../../SrcSite/include/axisfire.h"
#include "../../../SrcSite/include/axisgenv.h"
#include "../../inc/IAUPartnerMng.h"

class CSendItem
{
public:
	CString m_szTR;
	long	m_pCallback;
	int		m_nKey,
			m_nLen;
	LPBYTE	m_pData;

public:
	void Copy(CSendItem* pItem)
	{
		m_szTR = pItem->m_szTR;
		m_pCallback = pItem->m_pCallback;
		m_nKey = pItem->m_nKey;
		m_nLen = pItem->m_nLen;
		m_pData = pItem->m_pData;
	}
};

typedef CList<CSendItem*, CSendItem*> List_CSendItem;

// View를 감싸는 껍데기
// View를 생성, 삭제, 외부와의 인터페이스 기능
class CViewWrap : public CWnd
{
// Construction
public:
	CViewWrap();

// Attributes
public:
	CView*			m_pView;
 	BOOL CreateView( CRuntimeClass * pViewClass );

	//100530:동일화면 다르게 저장/읽기
	void SetStartInfo(void* pData);
// 	void AddViewForSingle( );
// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewWrap)
	//}}AFX_VIRTUAL

//	void OnSetFormLinkEvent();
// Implementation
public:
	virtual ~CViewWrap();

protected:
	IPartnerSvrManager	*m_pPtSvrMng;
	IPartnerManager		*m_pPtMng;
	void _DoGetPartnerManger(BOOL _bAdvise=TRUE);

	void	CloseScreen();

//@Solomon관심-->
protected:
	List_CSendItem m_SIList;
	//CSendItem	m_SendItem;
	
	int _DoParsingSendData(CString &szData, CSendItem &Info);
	BOOL _DoDirectSendTR(CString strTR, char *szData, int nLen, int nKey);
	
	int _DoListSendItem(BOOL bAdd, CSendItem* _pItem);
	void _DoClearSendItem();
	CSendItem* FindSendItem(int nKey);
	LONG OnRmsgSendMessage(WPARAM wParam, LPARAM lParam);
//@Solomon관심<--

	//100530:동일화면 다르게 저장/읽기
	CString m_szStartInfo, m_szFileKey;

	// Generated message map functions
protected:
		
	//{{AFX_MSG(CViewWrap)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnWinixDllData(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//실시간 처리할 종목인지 체크.
	BOOL _DoCheckAllertCode(CString szCode);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWWRAP_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_)
