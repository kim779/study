#if !defined(AFX_DLGINTERFACE_H__D6CE8BBB_8A0E_42A3_B276_4A0B6B44F8B5__INCLUDED_)
#define AFX_DLGINTERFACE_H__D6CE8BBB_8A0E_42A3_B276_4A0B6B44F8B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInterface.h : header file
//

#include <afxtempl.h>
///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CDlgInterface dialog
#define CLR_BACKGROUND RGB(255,255,255)
class CInputLeft;
class CDlgAnalysis;
class CDlgInterface : public CEscDialog
{
// Construction
public:
	CDlgInterface(UINT nIDTemplate, CWnd* pParentWnd = NULL);    // standard constructor
// Dialog Data
	//{{AFX_DATA(CDlgInterface)
	enum { IDD = NULL};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInterface)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInterface)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
		CInputLeft *m_pInput; // KHD : Left Dlg Pointer!!
		CDlgAnalysis* m_pDlgAnalysis;
public:
		//Input Pointer
		//콤마 루틴 필요 !! 
		BOOL m_bIsJusik; // TRUE: 주식 옵션 , FALSE: 선물 옵션 
		CString ParsingComma(CString strData); //콤마 루틴 
		CInputLeft* GetCInputLeft(){ return m_pInput;}
		void SetCInputLeft(CInputLeft* pInput){m_pInput = pInput;}
		void SetParentDlg(CDlgAnalysis *pAnalysis){m_pDlgAnalysis = pAnalysis;}
		CDlgAnalysis* GetParentDlg(){ return m_pDlgAnalysis;}
		//StrategyItem Interface
		
		virtual void UpdateDlg() {}
		virtual void SetChartFocus(int nFocusIndex) {}
		virtual void GuganBtnUpdate(){}
		static CString GetComma(LPCSTR pData);
		static CString GetNumber( LPCSTR pData );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINTERFACE_H__D6CE8BBB_8A0E_42A3_B276_4A0B6B44F8B5__INCLUDED_)
