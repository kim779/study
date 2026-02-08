// UserString.h: interface for the CUserString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERSTRING_H__58B9B56D_296C_476A_BDA9_E1CCD88D4591__INCLUDED_)
#define AFX_USERSTRING_H__58B9B56D_296C_476A_BDA9_E1CCD88D4591__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

class CUserString : public CToolBase  
{
public:
	CUserString(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CUserString();

private:
	CRect	m_dispRECT;
	CRect	m_saveRECT;

	CString	m_dispSTR;

public:

	void	DrawTool(CDC* pDCzz);
	void	HideTool();

	void	DrawHolder(CDC *pDC, bool showHolder);
	void	HideHolder();
	void	InflateHolder();

	void	CreateToolLBUP(CPoint pt, bool &bMouseCapture);

	void	ChangeToolLBDN(CPoint pt, bool &bMouseCapture);

	void	MoveToolLBDN(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBMV(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBUP(CPoint pt, bool &bMouseCapture);

	bool	PtInToolRegion(CPoint pt);
	bool	PtInHolderRect(CPoint pt);

	void	SetDisplayString(CString str) { m_dispSTR = str; }
	void	GetFontSize();

private:
	int	m_fWidth;
	int	m_fHeight;

};

#endif // !defined(AFX_USERSTRING_H__58B9B56D_296C_476A_BDA9_E1CCD88D4591__INCLUDED_)
