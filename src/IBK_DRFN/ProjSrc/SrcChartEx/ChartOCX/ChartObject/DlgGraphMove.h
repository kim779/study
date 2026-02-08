#if !defined(AFX_DLGGRAPHMOVE_H__E5EDFC51_8F44_4E2F_BCD5_0183550337A3__INCLUDED_)
#define AFX_DLGGRAPHMOVE_H__E5EDFC51_8F44_4E2F_BCD5_0183550337A3__INCLUDED_

#include "resource.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphMove.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphMove dialog

class CVerticalScale;
class CVerticalScaleGroupDataList;
class CMovingGraphVertScaleData;
class CGraphImp;
interface IChartCtrl;

class CDlgGraphMove : public CDialog
{
// Construction
public:
	CDlgGraphMove( CWnd* pParent, CGraphImp* pSourceGraph, const CVerticalScaleGroupDataList& vertScaleGroupDataList, IChartCtrl *p_pIChartCtrl);

// Dialog Data
	//{{AFX_DATA(CDlgGraphMove)
	enum { IDD = IDD_GRAPH_MOVE };
	CComboBox	m_cbxStyleList;
	CComboBox	m_ctlScaleGroup;
	int		m_nScaleGroup;
	int		m_nScalePosition;
	int		m_nScaleGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphMove)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphMove)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeScalegroup();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeStylecombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CMovingGraphVertScaleData GetMovingGraphVertScaleData() const;

private:
	const CVerticalScaleGroupDataList* m_pVertScaleGroupDataList;

	void InitializeData();
	void SetScalePositionAndGrid(const CVerticalScale& verticalScale);

	CVerticalScale GetVerticalScale(const int nIndex) const;
	CVerticalScale GetCurrentVerticalScale() const;

// (2009/9/21 - Seung-Won, Bae) Show Style Type on Price, Volume and MAs.
protected:
	CGraphImp *		m_pSourceGraph;
	IChartCtrl *	m_pIChartCtrl;
	CFont *			m_pTitleFont;
protected:
	BOOL	IsStyledGraph( void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHMOVE_H__E5EDFC51_8F44_4E2F_BCD5_0183550337A3__INCLUDED_)
