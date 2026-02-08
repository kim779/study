// SimpleGuideLine.h: interface for the CSimpleGuideLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEGUIDELINE_H__8B7AA4E8_BAE6_4A3D_B998_2254D4FA627A__INCLUDED_)
#define AFX_SIMPLEGUIDELINE_H__8B7AA4E8_BAE6_4A3D_B998_2254D4FA627A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/I133721/_ChartAddInBase.h"	// for CChartAddInBase

class CSimpleGuideLine : public CChartAddInBase  
{
public:
	CSimpleGuideLine( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CSimpleGuideLine();

	static CString m_strAddInItemName;

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2009/7/26 - Seung-Won, Bae) Manage property
protected:
	int			m_nBlockRow;
	int			m_nBlockColumn;
	short		m_nVScaleIndex;
	double		m_dYValue;
	short		m_nThickness;
	short		m_nLineStyle;
	COLORREF	m_clrColor;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND		m_hOcxWnd;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);

protected:
	virtual void OnDrawBeforeGraph( HDC p_hDC);
};

#endif // !defined(AFX_SIMPLEGUIDELINE_H__8B7AA4E8_BAE6_4A3D_B998_2254D4FA627A__INCLUDED_)
