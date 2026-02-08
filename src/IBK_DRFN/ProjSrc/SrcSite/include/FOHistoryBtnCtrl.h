#if !defined(AFX_FOHISTORYBTNCTRL_H__E77DAF17_A061_4047_A587_6E09D104FD74__INCLUDED_)
#define AFX_FOHISTORYBTNCTRL_H__E77DAF17_A061_4047_A587_6E09D104FD74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CFOHistoryBtnCtrl.h : header file
//


#include <atlbase.h>
extern CComModule _Module;
#include <atlwin.h>
#include <atlcom.h>

#include "./AxisFire.h"

#define DISPID_CODE		0x00000001
#define DISPID_NAME		0x00000002
#define DISPID_CODENAME		0x00000003

#define KIND_ALL	0	//전체
#define KIND_FUTURE	1	//선물
#define KIND_OPTION	2	//옵션

#define KIND_FUTURE_ALL			0
#define KIND_FUTURE_KOPI200		1
#define KIND_FUTURE_STAR		2
#define KIND_FUTURE_STOCK		3
#define KIND_FUTURE_COMMDITY	4

#define KIND_OPTION_ALL			0
#define KIND_OPTION_KOPI200		1
#define KIND_OPTION_STOCK		2
#define KIND_OPTION_COMMDITY	3

/////////////////////////////////////////////////////////////////////////////
// CFOHistoryBtnCtrl window

class CFOHistoryBtnCtrl : public CWnd
{
// Construction
public:
	CFOHistoryBtnCtrl();

// Attributes
public:

private:
	UINT		m_nID;
	DWORD		m_dwType;

	
	CWnd*		m_pwndWizard;
	CWnd*		m_pwndParent;
	CWnd*		m_pWndFOHistoryBtn;

	HINSTANCE	m_hiFOHistoryBtnDll;
	
	CString		m_strHome;


public:
	bool	CreateFOHistoryBtn(CWnd* pwndParent, CWnd* pwndWizard, CRect rect, UINT nID, int nKind, int nFutureKind=KIND_FUTURE_KOPI200, int nOptionKind=KIND_OPTION_KOPI200);
	CString	GetCode();
	CString GetName();
	CString GetCodeName(CString strCode);
	int		GetCodeType(CString strCode);
private:
	


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFOHistoryBtnCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFOHistoryBtnCtrl();
	// Generated message map functions
protected:
	//{{AFX_MSG(CFOHistoryBtnCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BALANCECTRL_H__CB12213D_855E_448D_A42A_75BE1CCC5266__INCLUDED_)




/*--------------------------------------------------------------------------------
// 사용법
//--------------------------------------------------------------------------------

생성:
	m_pFOHistoryBtn = new CFOHistoryBtnCtrl();
	m_pFOHistoryBtn->CreateFOHistoryBtn(this, m_pwndWizard, CRect(1, 22, 1 + 140, 22 + 20), IDC_FOHISTORYBTN, 
										KIND_ALL, KIND_FUTURE_KOPI200, KIND_OPTION_ALL);


*	nKind속성지정
		KIND_ALL	0	//전체
		KIND_FUTURE	1	//선물
		KIND_OPTION	2	//옵션

*	nFutureKind 속성지정
		KIND_FUTURE_ALL			0
		KIND_FUTURE_KOPI200		1
		KIND_FUTURE_STAR		2
		KIND_FUTURE_STOCK		3
		KIND_FUTURE_COMMDITY	4

*	nOptionKind 속성지정
		KIND_OPTION_ALL			0
		KIND_OPTION_KOPI200		1
		KIND_OPTION_STOCK		2
		KIND_OPTION_COMMDITY	3


종목선택시 들어오는 이벤트:

.h 파일에 다음과 같이 선언
afx_msg void OnSelendokFOHistoryBtn();

.cpp 파일에 다음과 같이 선언 
ON_EN_CHANGE(IDC_FOHISTORYBTN, OnSelendokFOHistoryBtn)

void CMapWnd::OnSelendokFOHistoryBtn() 
{
	CString strCode = m_pFOHistoryBtn->GetCode();
}

코드네임
CString strName = GetCodeName(CString strCode);

코드타입
int nType = GetCodeType(CString strCode);

*/