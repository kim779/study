#if !defined(AFX_CHARTPROPERTYSHEET_H__0406C3D4_216A_4BFA_ADDC_CEDA1686A47B__INCLUDED_)
#define AFX_CHARTPROPERTYSHEET_H__0406C3D4_216A_4BFA_ADDC_CEDA1686A47B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartPropertySheet.h : header file
//

#include "Include_AddIn.h"							// for Ixxxxxx.h
#include "ViewDesignPage.h"
#include "ViewDesignPage2.h"
#include "ChartListPage.h"
#include "PacketPage.h"
#include "RealPacketPage.h"
#include "SharedDataPage.h"
#include "ToolandRButtonPage.h"
#include "AddInLoadInfoPage.h"		// for CAddInLoadInfoPage

interface IPropertyMap;
/////////////////////////////////////////////////////////////////////////////
// CChartPropertySheet
class CChartPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CChartPropertySheet)

// Construction
public:
//	CChartPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CChartPropertySheet(LPCTSTR pszCaption, HWND p_hOcxWnd, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	CChartListPage		m_ChartListPage;
	CViewDesignPage		m_ViewDesignPage;
	CViewDesignPage2	m_ViewDesignPage2;
	CPacketPage			m_PacketPage;
	CRealPacketPage		m_RealPacketPage;

//@solomon[sashia]	20090902 --- 기능 축소 --------->>
//	CSharedDataPage		m_SharedDataPage;
//@solomon[sashia]	20090902 --- 기능 축소 ---------<<

	CToolandRButtonPage	m_ToolandRButtonPage;
	// (2006/8/24 - Seung-Won, Bae) AddIn Loading Info Page
	CAddInLoadInfoPage	m_addInLoadInfoPage;


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartPropertySheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager);
	//export함수에서 접근. ocx에서 Row, Column받기위한 함수.
	void SetBlockProperty(const int nRow, const int nColumn);  
	//export함수에서 접근, ocx에서 받은 속성값 적용. 
	void SetProperty( IPropertyMap *p_pIPropertyMap);
	//각각의 속성페이지에서 접근.
	virtual ~CChartPropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartPropertySheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//sy 2006.02.20.
/*private:
	enum {
		ONELINE_COLOR = 1,
		TWOLINE_COLOR,
		THREELINE_COLOR,
		FOURLINE_COLOR,
		FIVELINE_COLOR,
		SIXLINE_COLOR
	};*/

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTPROPERTYSHEET_H__0406C3D4_216A_4BFA_ADDC_CEDA1686A47B__INCLUDED_)
