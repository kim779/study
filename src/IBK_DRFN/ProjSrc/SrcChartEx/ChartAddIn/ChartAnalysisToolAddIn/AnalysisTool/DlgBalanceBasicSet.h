#pragma once

#include "resource.h"									// for IDD_ANALYSIS_TOOL_SET
#include "../Include_Analysis/ElementEnvironment.h"

#include "Coordinate.h"
#include "ColorButton.h"
#include "AnalysisData.h"
#include "BitmapComboBox.h"
#include "AnalysisGlobalDefine.h"

// CDlgBalanceBasicSet dialog

class CDlgBalanceBasicSet : public CDialog
{
// Construction
public:
	CDlgBalanceBasicSet(CWnd* pParent,
				const CString& strUserPath,
				const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
				int nElementType,
				CTextPosition* pTextPosition,
				CPenEnvironment* pPenEnvironment,
				CElementPointList *pElementPointList,
				CLineExtEnvironment* pLineExtEnvironment,
				CDataTypeEnvironment* pDataEnvironment);   // standard constructor

	enum EN_TEXTPOS_TYPE
	{
		G_TEXT_POS_NONE = -1,
		G_TEXT_POS_ONE =  0,
		G_TEXT_POS_HORZ = 1,
		G_TEXT_POS_VERT = 2,
		G_TEXT_POS_HV   = 3
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
	//{{AFX_DATA(CDlgBalanceBasicSet)
	enum { IDD = IDD_ANALYSIS_BALANCE_BASIC_SET };
	CDateTimeCtrl	m_ctrlXDateTime1;
	CEdit			m_ctrlEditValue1;
	CColorButton	m_ctlColor;
	CBitmapComboBox	m_ctlWidth;
	CBitmapComboBox	m_ctlStyle;
	CButton			m_chkShowText;
	CButton			m_ctrlLeftExt;
	CButton			m_ctrlRightExt;

	int		m_nWidth;
	int		m_nStyle;
	//}}AFX_DATA

public:
	CDataTypeEnvironment* m_pDataEnvironment;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBalanceBasicSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBalanceBasicSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnColor();
	afx_msg void OnApplyBt();
	afx_msg void OnSelchangeWidth();
	afx_msg void OnSelchangeStyle();
	afx_msg void OnDatetimechangeDatetimepickerXdate1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCancelBt();
	afx_msg void OnOkBt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitializeData();
	void ApplyData();
	void ApplyOrgData();
	void SaveData();

	void SetYValue(double dYValue,int nIndex);
	void SetXDateTime(double dXDateTime,int nIndex);
	CPenEnvironment* GetCurrentPenEnvironment() const;

private:
	CString m_strUserPath;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_nDataPeriod;
	ST_ANALYSIS_TOOL_INFO m_stAnalysisTool;

	CElementPointList m_baseElementPointList;
	CElementPointList *m_pElementPointList;
	int		m_nTextPosType;

	CWnd*	m_pParent;
	int		m_nElementType;

	CPenEnvironment* m_pPenEnvironment;
	CPenEnvironment m_basePenEnvironment;

	CTextShowPosition* m_pTextPosition;
	CTextShowPosition  m_baseTextPosition;

	CLineExtEnvironment* m_pLineExtEnvironment;
	CLineExtEnvironment m_baseLineExtEnvironment;

	void InitializeData_Width();
	void InitializeData_Style();
	void InitializeData_Coefficient();

	void SetPenEnvironment();
	void SetColor(const COLORREF& color);

	void SetTextPosition();
	void SetTextPosition(const bool bShow);

	void SetLineExtEnvironment();
	void SetLineExtEnvironment(const bool bLeft,const bool bRight);

	void SetDateValueLists();

	void SetCoefficientValueLists();

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
