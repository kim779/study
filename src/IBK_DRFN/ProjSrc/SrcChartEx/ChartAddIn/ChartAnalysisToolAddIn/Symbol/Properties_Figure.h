#if !defined(AFX_PROPERTIES_FIGURE_H__161C3AFB_4BCB_4A3C_B86F_33E2904E2D0E__INCLUDED_)
#define AFX_PROPERTIES_FIGURE_H__161C3AFB_4BCB_4A3C_B86F_33E2904E2D0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Properties_Figure.h : header file
//

#include "resource.h"
#include "Coordinate.h"
#include "ColorButton.h"
#include "BitmapComboBox.h"
#include "SymbolBaseData.h"
#include "../Include_Analysis/ElementEnvironment.h"
#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

/////////////////////////////////////////////////////////////////////////////
// CProperties_Figure dialog
class CPropertiesData_text;
class CProperties_Figure : public CDialog
{
// Construction
public:

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	CProperties_Figure(CPropertiesData_text* pProperties,
// 						const CSymbolBaseData::TOOLTYPE &nElementType,
// 						const CString& strUserPath,
// 						const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
// 						const CCoordinate& coordStart,
// 						const CCoordinate& coordEnd,CWnd* pParent = NULL);   // standard constructor

	CProperties_Figure( IChartManager* pMainBlock,
						const CBlockIndex& blockIndex, 
						const CString & strFormatX,
						CPropertiesData_text* pProperties,
 						const CSymbolBaseData::TOOLTYPE &nElementType,
 						const CString& strUserPath,
 						const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
 						const CCoordinate& coordStart,
 						const CCoordinate& coordEnd,CWnd* pParent = NULL);   // standard constructor

	// 2011.02.08 by SYS <<


// Dialog Data
	//{{AFX_DATA(CProperties_Figure)
	enum { IDD = IDD_PROPERTIES_FIGURE };
	CButton	m_Fill_Bt;
	CColorButton	m_Color_Bt;
	CBitmapComboBox	m_ctlStyle;
	CBitmapComboBox	m_ctlWidth;

	CEdit			m_ctrlEditXValue1;
	CEdit			m_ctrlEditXValue2;
	
	CEdit			m_ctrlEditValue1;
	CEdit			m_ctrlEditValue2;
	
	CDateTimeCtrl	m_ctrlXDateTime1;
	CDateTimeCtrl	m_ctrlXDateTime2;

	int		m_nWidth;
	int		m_nStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperties_Figure)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProperties_Figure)
	virtual BOOL OnInitDialog();
	afx_msg void OnColor();
	afx_msg void OnCheck();
	afx_msg void OnSelchangeWidth();
	afx_msg void OnSelchangeStyle();
	afx_msg void OnDatetimechangeDatetimepickerXdate1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepickerXdate2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	IChartManager* m_pMainBlock;
	CBlockIndex m_blockIndex;
	CString		m_strFormatX;
	// 2011.02.08 by SYS <<

	CSymbolBaseData::TOOLTYPE m_nElementType;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_nDataPeriod;
	CPropertiesData_text* m_pProperties;
	CCoordinate m_coordStart;
	CCoordinate m_coordEnd;
	CString m_strUserPath;

	void InitializeData_Width();
	void InitializeData_Style();
	void SetDateValueLists();
	void SetYValue(double dYValue,int nIndex);
	void SetXDateTime(double dXDateTime,int nIndex);
	void SaveData();
public:
	CCoordinate GetStartCoord() const;
	CCoordinate GetEndCoord() const;

protected:
	HWND m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIES_FIGURE_H__161C3AFB_4BCB_4A3C_B86F_33E2904E2D0E__INCLUDED_)
