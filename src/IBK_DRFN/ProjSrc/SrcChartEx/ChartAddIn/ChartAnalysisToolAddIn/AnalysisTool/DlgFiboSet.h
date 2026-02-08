#if !defined(AFX_DLGFIBOSET_H__991468B8_E73D_43C3_8EC4_93AD3F999F55__INCLUDED_)
#define AFX_DLGFIBOSET_H__991468B8_E73D_43C3_8EC4_93AD3F999F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFiboSet.h : header file
//
#include "resource.h"									// for IDD_ANALYSIS_TOOL_SET
#include "../Include_Analysis/ElementEnvironment.h"

#include "Coordinate.h"
#include "ColorButton.h"
#include "AnalysisData.h"
#include "BitmapComboBox.h"
#include "AnalysisGlobalDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFiboSet dialog

class CDlgFiboSet : public CDialog
{
// Construction
public:

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	CDlgFiboSet(CWnd* pParent,
// 				const CString& strUserPath,
// 				const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
// 				int nElementType,
// 				int *pEquiDivType,
// 				int *pPriceRangeDivType,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 				CTextPosition* pTextPosition,
// 				CPenEnvironment* pPenEnvironment,
// 				CElementPointList *pElementPointList,
// 				CLineExtEnvironment* pLineExtEnvironment,
// 				CDataTypeEnvironment* pDataEnvironment);   // standard constructor

	CDlgFiboSet(CWnd* pParent,
				const CString& strUserPath,
				const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
				int nElementType,
				int *pEquiDivType,
				int *pPriceRangeDivType,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
				CTextPosition* pTextPosition,
				CPenEnvironment* pPenEnvironment,
				CElementPointList *pElementPointList,
				CLineExtEnvironment* pLineExtEnvironment,
				CDataTypeEnvironment* pDataEnvironment,
				IChartOCX *p_pIChartOCX,
				CBlockIndex& blockIndex, 
				CString		strFormatX);		// X축 날짜시간 포맷 (YYYYMMDDHHMMSS)); 
	// 2011.02.08 by SYS <<
	
	enum EN_TEXTPOS_TYPE
	{
		G_TEXT_POS_NONE = -1,
		G_TEXT_POS_ONE =  0,
		G_TEXT_POS_HORZ = 1,
		G_TEXT_POS_VERT = 2,
		G_TEXT_POS_HV   = 3
	};

	enum EN_LINEEXT_TYPE
	{
		G_LINE_EXT_NONE = -1,
		G_LINE_EXT_LEFT =  0,
		G_LINE_EXT_RIGHT = 1,
		G_LINE_EXT_BOTH = 2,
	};

	enum EN_EQUIDIV_TYPE
	{
		G_EQUI_DIV_NONE = -1,
		G_EQUI_DIV_UNCHECK =  0,
		G_EQUI_DIV_CHECK = 1,
	};

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	enum EN_PRICE_RANGE_DIV_TYPE
	{
		G_PRICE_RANGE_DIV_NONE = -1,
		G_PRICE_RANGE_DIV_UNCHECK =  0,
		G_PRICE_RANGE_DIV_CHECK = 1,
	};

	enum EN_PERIOD_TYPE
	{
		G_PERIOD_TICK = 0,
		G_PERIOD_MIN =  1,
		G_PERIOD_DAY =  2,
		G_PERIOD_WEEK = 3,
		G_PERIOD_MON =  4,
	};

// Dialog Data
	//{{AFX_DATA(CDlgFiboSet)
	enum { IDD = IDD_ANALYSIS_FIBO_SET };
	CButton	m_chkPriceRangeDiv;
	CBitmapComboBox	m_ctlStyle;
	CBitmapComboBox	m_ctlWidth;
	CColorButton	m_ctlColor;
	CButton			m_ctrlLeftText;
	CButton			m_ctrlRightText;
	CButton			m_ctrlTopText;
	CButton			m_ctrlBottomText;
	CButton			m_ctrlLeftExt;
	CButton			m_ctrlRightExt;
	CButton			m_ctrlEquiDiv;
	CButton			m_ctrlCheckFill;
	CButton			m_ctrlCheckHL;
	CEdit			m_ctrlEditXValue1;
	CEdit			m_ctrlEditXValue2;
	CEdit			m_ctrlEditXValue3;
	CEdit			m_ctrlEditValue1;
	CEdit			m_ctrlEditValue2;
	CEdit			m_ctrlEditValue3;
	CDateTimeCtrl	m_ctrlXDateTime1;
	CDateTimeCtrl	m_ctrlXDateTime2;
	CDateTimeCtrl	m_ctrlXDateTime3;
	int		m_nWidth;
	int		m_nStyle;
	//}}AFX_DATA

public:
	CDataTypeEnvironment* m_pDataEnvironment;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFiboSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFiboSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnColor();
	afx_msg void OnApplyBt();
	afx_msg void OnSelchangeWidth();
	afx_msg void OnSelchangeStyle();
	afx_msg void OnDatetimechangeDatetimepickerXdate1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepickerXdate2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepickerXdate3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCancelBt();
	afx_msg void OnOkBt();
	//}}AFX_MSG
	afx_msg void OnCheck( UINT nID );
	DECLARE_MESSAGE_MAP()

protected:
	void InitializeData();
	void ApplyData();
	void ApplyOrgData();
	void SaveData();
	
	void SetInitTextPosition(CTextPosition* pTextPosition);
	void SetYValue(double dYValue,int nIndex);
	void SetXDateTime(double dXDateTime,int nIndex);
	CPenEnvironment* GetCurrentPenEnvironment() const;

private:

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	IChartOCX	*m_pIChartOCX;
	CBlockIndex m_blockIndex;
	CString		m_strFormatX;
	// 2011.02.08 by SYS <<

	CString m_strUserPath;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_nDataPeriod;
	ST_ANALYSIS_TOOL_INFO m_stAnalysisTool;

	CElementPointList m_baseElementPointList;
	CElementPointList *m_pElementPointList;
	int		m_nTextPosType;
	int		m_nLineExtType;
	int		m_nEquiDivLine;
	int		m_nPriceRangeDivLine;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.

	CWnd*	m_pParent;
	int		m_nElementType;

	CPenEnvironment* m_pPenEnvironment;
	CPenEnvironment m_basePenEnvironment;

	CTextShowPosition* m_pTextPosition;
	CTextHorzPosition* m_pTextHPosition;
	CTextVertPosition* m_pTextVPosition;
	CTextHorzVertPosition* m_pTextHVPosition;
	CTextShowPosition  m_baseTextPosition;
	CTextHorzPosition m_baseTextHPosition;
	CTextVertPosition m_baseTextVPosition;
	CTextHorzVertPosition m_baseTextHVPosition;
	
	CLineExtEnvironment* m_pLineExtEnvironment;
	CLineExtEnvironment m_baseLineExtEnvironment;

	int* m_pEquiDivEnvironment;
	int  m_baseEquiDivEnvironment;

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	int* m_pPriceRangeDivEnvironment;
	int  m_basePriceRangeDivEnvironment;

	void InitializeData_Width();
	void InitializeData_Style();

	void SetPenEnvironment();
	void SetColor(const COLORREF& color);

	void SetTextPosition();
	void SetTextPosition(const bool bShow);
	void SetTextPosition(const bool bLeft,const bool bRight);
	void SetTextPosition(const bool bLeft,const bool bRight,const bool bTop,const bool bBottom);

	void SetLineExtEnvironment();
	void SetLineExtEnvironment(const bool bLeft,const bool bRight);

	void SetDateValueLists();

	CTextPosition* GetTextPosition();
	CTextPosition* GetOrgTextPosition();
	
	CLineExtEnvironment* GetLineExtEnvironment();
	CLineExtEnvironment* GetOrgLineExtEnvironment();
	
	void ButtonMoveWindow(const int nID);
	CPoint GetMovingPosition_Button(const int nID, const CRect& dlgRect, const CRect& itemRect) const;
	int GetButtonPosition_X(const int nOrgPos, const int nWidth, const int nIndex) const;

protected:
	HWND m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFIBOSET_H__991468B8_E73D_43C3_8EC4_93AD3F999F55__INCLUDED_)
