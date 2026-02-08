#if !defined(AFX_DLGHORZSCALESET_H__50DDDAE5_1A0C_4A51_95C7_2FC266E136FF__INCLUDED_)
#define AFX_DLGHORZSCALESET_H__50DDDAE5_1A0C_4A51_95C7_2FC266E136FF__INCLUDED_

#include "../Include_Chart/ScaleDataList.h"
#include "ScaleSet.h"
#include "ColorButton.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHorzScaleSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHorzScaleSet dialog

class CHorizontalScale;
// xScaleManager 추가 : xScaleManager - ojtaso (20070525)
class CXScaleManager;

class CDlgHorzScaleSet : public CScaleSet
{
// Construction
public:
	// xScaleManager 추가 : xScaleManager - ojtaso (20070525)
	CDlgHorzScaleSet( CWnd* pParent, CMainBlock *p_pMainBlock, CHorizontalScalePtrDataList* pHorzScalePtrDataList, CXScaleManager* lpxScaleManager);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHorzScaleSet)
	enum { IDD = IDD_HORZ_SCALE_SET };
	CString	m_strCompart_Date;
	CString	m_strCompart_Time;
	int		m_nMarginCount;
	BOOL	m_bReverse;
	//}}AFX_DATA
	int		m_nOrgMarginCount;
	BOOL	m_bOrgReverse;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHorzScaleSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHorzScaleSet)
	virtual void OnCancel();
	afx_msg void OnChangeMargin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void InitializeData();
	virtual void ApplyInEtcData();
	virtual void SetAllScale();
	virtual void SetScale(const int nIndex);
	virtual void SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor);

	// 화면 변경
	virtual void ChangeScaleWindow(const int nGroupIndex);
	virtual void EnableWindowOnAllScaleGroup();

private:
	CHorizontalScalePtrDataList* m_pHorzScalePtrDataList;
	CHorizontalScaleDataList m_baseHorzScaleDataList;

	// xScaleManager 추가 : xScaleManager - ojtaso (20070525)
	CXScaleManager* m_lpxScaleManager;

	// data 초기화
	void InitializeData_ScaleGroup();
	void InitializeData_Date();
	void InitializeData_Time();
	void InitializeData_Compart(CComboBox* pComboBox, const CString& currentComboBoxString);
	void InitializeData_Etc();

	// ocx에 message주기
	void SendMessageToParent(const int nMarginCount);

	// 해당 index의 horzScale 가져오기.
	CHorizontalScale* GetScale(const int nIndex);

// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
public:
	CMainBlock *	m_pMainBlock;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHORZSCALESET_H__50DDDAE5_1A0C_4A51_95C7_2FC266E136FF__INCLUDED_)
