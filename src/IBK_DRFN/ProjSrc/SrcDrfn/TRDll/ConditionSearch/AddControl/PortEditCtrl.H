#if !defined(AFX_PORTEDITCTRL_H__8B7696D5_2478_4769_B5F1_6D7CF96F8D73__INCLUDED_)
#define AFX_PORTEDITCTRL_H__8B7696D5_2478_4769_B5F1_6D7CF96F8D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PotoEdit.h : header file
//


/**********************************************************************/
/*																																		*/
/*											PortPolio 조합 Edit Control										*/
/*													(Mouse Control)														*/
/*																																		*/
/*												 작성일 : 2002. 5~													*/
/*							       작성자 : 우동우 (Doori)												*/
/*																																		*/
/**********************************************************************/

#include <afxtempl.h>   //<= CList


#include "../DefineFiles/client_typedef.h"
/*
typedef struct tagPortPolio
{
	CString   strOperation;
	int				nType;				//# 0 : PortPolio, 1 : Account
	CString   strPortNumber;
	CString   strPortName;
} PortPolio;
*/


/////////////////////////////////////////////////////////////////////////////
// CPortEditCtrl window

class CPortEditCtrl : public CRichEditCtrl
{
	// Construction
public:
	CPortEditCtrl();
	virtual ~CPortEditCtrl();
	
protected:
	//## Cursor ##
	HCURSOR m_hOperationSelectCursor;
	
	int m_noldStartOperation;
	int m_noldEndOperation;
	BOOL m_bMouseOver;
	
  CFont m_fontDefault;

public:
	CList<PortPolio, PortPolio> m_listPort;
	CString    m_strAndOr;
	
	// Operations
public:
	BOOL AddPortData(PortPolio  stPortPolio);    
	BOOL DeletePortData(CString strPortNumber);    
	int GetPortDataCount();
	
	void ChangeAllOpt();
	void ChangeAllOpt(BOOL bAnd);
	
	void DeleteAll();
	void ReLoadAll();   
	
	void SetPortPolioColor();
  CHARFORMAT GetFontFormat(COLORREF clrColor);
	
	BOOL IsExistPortCode(CString strPortNumber);
	
	int PointToStrPosition(CPoint point);
	CSize GetFontSize();

	
	BOOL GetPortGroupList(CStringArray &saPortGroup, BOOL &bAnd);


	
	BOOL GetPortInsertCheck(CStringArray &saPort, CString strJongCode);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortEditCtrl)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CPortEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTEDITCTRL_H__8B7696D5_2478_4769_B5F1_6D7CF96F8D73__INCLUDED_)
