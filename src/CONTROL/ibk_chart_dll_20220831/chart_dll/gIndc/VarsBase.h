// VarsBase.h: interface for the CVarsBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARSBASE_H__333FF6A0_8DCD_4A0F_96DE_17DD37612EFF__INCLUDED_)
#define AFX_VARSBASE_H__333FF6A0_8DCD_4A0F_96DE_17DD37612EFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndcBase.h"
#include <afxtempl.h>

class CVarsBase : public CIndcBase  
{
public:
	CVarsBase(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay);
	virtual ~CVarsBase();

protected:
	CArray<int, int>		m_arPointX;
	CArray<COLORREF, COLORREF>	m_arColorX;

public:
//	void	SetUnion(bool bUnion) {	m_bUnion = bUnion; };
	
protected:
	CString	GetYposValue(CPoint pt, bool bGrid);
	virtual CString	GetXposValue(CPoint pt) { return _T(""); }

	virtual void DrawTickRight(CDC* pDC, double dMax, double dMin, bool bDrawMin = true);
	virtual void DrawTickLeft(CDC* pDC, double dMax, double dMin, bool bDrawMin = true);
	bool	DrawLineTickBottom(CDC* pDC, double MaxV, double MinV, int dispTICK, bool bTRN = true);
	bool	CalculateDisplay(double &div, double Max, double Min, double& drawMinValue, int dispTICK,
				double* dispValue, double* logValue, int& dispCount);

	//virtual void DrawTick(CDC* pDC, bool bBottom = false);
	virtual void DrawTick(CDC* pDC);

	virtual void SetUserDefined(CString sVal) {}
	virtual void GetUsedValue(CString &sVal) {}

	void DrawBottomStick(CDC* pDC);
};

#endif // !defined(AFX_VARSBASE_H__333FF6A0_8DCD_4A0F_96DE_17DD37612EFF__INCLUDED_)
