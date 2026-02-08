#if !defined(AFX_CODECTRL_H__8C6C1F37_A51A_4D7F_B461_FC7C57E521EF__INCLUDED_)
#define AFX_CODECTRL_H__8C6C1F37_A51A_4D7F_B461_FC7C57E521EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeCtrl.h : header file
//
//#include "AxisExt.h"

// IBK Unit Type ///////////////////////////////////
 #define GU_NONE		-1
 #define	GU_CODE		0	// 주식 
 #define	GU_FUTURE	1	// 선물 
 #define	GU_OPTION	2	// 옵션 
 #define	GU_INDEX	3	// 업종 
 #define	GU_FOCODE	4	// 선물  + 옵션
 #define	GU_POPTION	5	// 주식옵션 
 #define	GU_ELWCODE	6	// ELW 
 #define	GU_SCODE	7	// 신주인수권 
 #define	GU_BOND		8	// 채권(상품)
 #define	GU_SBOND	9	// 소매채권
 #define	GU_FCODE	10	// 주식선물 
 #define	GU_FOSTOCK	11	// 주식선물 + 옵션 
 #define	GU_FOREIGN	12	// 해외
/////////////////////////////////////////////////////

class CWinixCodeCtrl : public CWnd//, public CAxisExt
{
	// Construction
public:
	CWinixCodeCtrl(CWnd* Wnd);
	
	// Attributes
public:
	
private:
protected:
	UINT	m_nID;
	
	CWnd*	m_pwndCodeCtrl;
	CWnd*	m_pwndParent;
	CWnd*	m_pwndWizard;
	CWnd*	m_pCodeSendWnd;
	
	
	HINSTANCE	m_hiCodeCtrl;
	
	
	
	
public:
	BOOL	CreateCodeCtrl(CWnd* pwndParent, CWnd* pwndWizard,CWnd* pwndSendWnd,CRect rect, UINT nID);
	CString	GetCode();
	CString	GetName();
	bool	SetCode(CString strCode, bool bOnlySet = false, bool bSetHistory = false);
	bool	SetColor(COLORREF rgbFG, COLORREF rgbBG);
	bool	SetFont(CFont* pfont);
	bool	SetFocus();

//IBK 추가 함수 	
	BOOL		m_bIsComareChart;//KHD : 
	void	SetUnit_IBK(int unit);
	void	SetCtrlProperty( _bstr_t prop_name, _variant_t &var );
	bool	SetUnit(int nUnit);	// KHD : Int형으로 바꿈 "주식" : 주식 + ELW
	CString GetCtrlProperty( LPCSTR prop_name );
//

	// "업종" : 업종
	// "선물" : kospi 선물
	// "옵션" : kospi 옵션
	// "해외" : 해외
	// "ELW" : ELW
	// "선물옵션" : kospi 선물 + kospi 옵션 
	// "주식선물" : 주식선물
	// "주식옵션" : 주식옵션
	// "상품선물옵션" : 상품선물 + 상품옵션
	
private:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinixCodeCtrl)
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CWinixCodeCtrl();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CWinixCodeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnUser( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; 

/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl window
#include "../../SrcDrfn/Chart/Chart_Common/ILButton.h"

class CCodeCtrl : public CWinixCodeCtrl
{
// Construction
public:
	CCodeCtrl(CWnd* Wnd);

public:
	virtual void	UseCodeDlg(BOOL bValue) {m_bUseCodeDlg=bValue;}
	virtual void	SetRootPath(LPCSTR szRootDir) {m_szRootPath=szRootDir;}
	virtual bool	SetUnit(int nCodeType);
	virtual bool	SetUnit(CString strUnit);
	virtual bool	SetHistory(LPCSTR  szCode, LPCSTR szSymbol="1301");
	virtual bool	SetCode(CString strCode, bool bOnlySet=false);
	virtual CString	GetCode();

	// Operations
public:
	void	SetUseFutureOption(bool bValue) {m_bUseFutureOption=bValue;}
	bool	SetUnitEx(CString strUnit);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeCtrl)
protected:
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CCodeCtrl();

protected:
	bool	m_bFirstCalled;
	bool	m_bUseFutureOption;
	bool	m_bFOJipyo;
	CString m_szOldUnit;

	//-->검색버튼
	CImageList	m_btnImageList;
	CILButton	m_btnCodeDlg; 
	int			m_nCodeType;
	CString		m_szCodeUnit;
	BOOL		m_bUseCodeDlg;
	CString		m_szRootPath;
	CEdit		m_editCode;

	BOOL _DoCreateCodeDlgBtn();
	afx_msg void OnSelendokCodeCtrl();
	//<--검색버튼

	void OnBtnCodeDlg();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	

protected:
	//@100219-alzioyes.
	int		m_nFixUnit;
	bool	m_bFixUnit;

public:
	//@100219-alzioyes.
	void	SetFixUnit(CString strUnit);
	void	SetFixUnit(int	 nFixUnit);

};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // AFX_CODECTRL_H__8C6C1F37_A51A_4D7F_B461_FC7C57E521EF__INCLUDED_



/*--------------------------------------------------------------------------------
// 사용법
//--------------------------------------------------------------------------------

생성:
	m_pCodeCtrl = new CCodeCtrl;
	m_pCodeCtrl->CreateCodeCtrl(this, m_pwndWizard, CRect(1, 1, 1 + 70, 1 + 20), IDC_CODECTRL);
	m_pCodeCtrl->SetUnit("주식");


종목바뀔시 들어오는 이벤트:

.h 파일에 다음과 같이 선언
afx_msg void OnSelendokCodeCtrl();

.cpp 파일에 다음과 같이 선언 
ON_CBN_SELENDOK(IDC_CODECTRL, OnSelendokCodeCtrl)

void CMapWnd::OnSelendokCodeCtrl() 
{
	CString strCode = m_pCodeCtrl->GetCode();
}

*/
