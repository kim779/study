#if !defined(AFX_DLGVERTSCALESET_H__53E7AE6F_1919_4244_9894_9474D261E3B4__INCLUDED_)
#define AFX_DLGVERTSCALESET_H__53E7AE6F_1919_4244_9894_9474D261E3B4__INCLUDED_

#include "../Include_Chart/ScaleDataList.h"
#include "ScaleSet.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVertScaleSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVertScaleSet dialog

class CVerticalScale;

class CDlgVertScaleSet : public CScaleSet
{
// Construction
public:
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	// 20081025 이주표 strType 추가
	CDlgVertScaleSet(CWnd* pParent, CVerticalScalePtrDataList* pVertScalePtrDataList, BOOL bHogaUnit);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVertScaleSet)
	enum { IDD = IDD_VERT_SCALE_SET };
	int		m_nScaleUnitType;
	BOOL	m_bIsInvert;
	BOOL	m_bIsLog;
	BOOL	m_bMinMaxLock;
	int		m_nMinMaxType;
	double	m_dHogaNumber;
	double	m_dUserNumber;
	int		m_nUserHogaType;
	int		m_nUpSpace;
	int		m_nDownSpace;
	//}}AFX_DATA

	// (2007/7/16 - Seung-Won, Bae) Do not show the INF Value in UI.
	double	m_dMax;
	double	m_dMin;

	// 20081015 이주표 >>
	CString m_strMin;
	CString m_strMax;
	// 20081015 이주표 <<

	// 호가단위, 기준가 사용 - ojtaso (20070917)
	BOOL	m_bHogaUnit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVertScaleSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVertScaleSet)
	virtual void OnCancel();
	//}}AFX_MSG
	void OnRdoMinmaxType( UINT nID);
	void OnRdoUserHogaType( UINT nID);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void InitializeData();
	virtual void ApplyInEtcData();
	virtual void SetAllScale();
	virtual void SetScale(const int nIndex);
	virtual void SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor);

	// 화면 변경
	virtual void ChangeScaleWindow(const int nGroupIndex);
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	BOOL SetEnableHogaUnit(const int nGroupIndex);

private:
	CVerticalScalePtrDataList* m_pVertScalePtrDataList;
	CVerticalScaleDataList m_baseVertScaleDataList;

	void InitializeData_ScaleGroup();
	void InitializeData_ScaleType();
	void InitializeData_Etc();
	void SetScaleType(const bool bIsLog);

	// 해당 index의 vertScale 가져오기.
	CVerticalScale* GetVertScale(const int nIndex);

// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
protected:
	virtual void EnableWindowOnAllScaleGroup( void);

// (2007/7/13 - Seung-Won, Bae) Set Activate.
protected:
	void	UpdateScale( const int nIndex);
	void	SetActiveVertcialScale( const int p_nIndex);

// (2007/7/16 - Seung-Won, Bae) Check Invalid Value.
protected:
	void CheckValue( int p_nPreSpace, int p_nNewSpace, int p_nID);
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVERTSCALESET_H__53E7AE6F_1919_4244_9894_9474D261E3B4__INCLUDED_)
