// AndrewPF.h: interface for the CAndrewPF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANDREWPF_H__DF73A80E_EA7D_4E5A_AFED_76E253600021__INCLUDED_)
#define AFX_ANDREWPF_H__DF73A80E_EA7D_4E5A_AFED_76E253600021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"
#define	maxPFLINE	3

class CAndrewPF : public CToolBase  
{
public:
	CAndrewPF(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname);
	virtual ~CAndrewPF();

private:
	CRect	m_cntRcHolder;
	CRect	m_cntRcHolder1;
	CRect	m_cntRcHolder2;

	CPoint	m_FPoint;
	CPoint	m_SPoint;
	CPoint	m_TPoint;
	bool	m_DrawLine;

	CPoint	m_RgnEndPoint;
	CPoint	m_RgnStartPoint;
	CPoint	m_RgnEndPoint1;
	CPoint	m_RgnStartPoint1;
	CPoint	m_RgnEndPoint2;
	CPoint	m_RgnStartPoint2;

	int	m_SelectPoint;

	CPoint	m_MaxPoint;
	CPoint	m_MiddlePoint;
	CPoint	m_MinPoint;

	CPoint	m_CenterPoint;
	CPoint	m_OffSetPointUp;
	CPoint	m_OffSetPointDown;

	double	m_Firstvalue;
	double	m_Secundvalue;
	double	m_Thirdvalue;

	double	m_Firstpos;
	double	m_Secundpos;
	double	m_Thirdpos;

	double	m_Maxvalue;
	double	m_Middlevalue;
	double	m_Minvalue;
	double	m_Centervalue;
	double	m_Upvalue;
	double	m_Downvalue;

	double	m_Maxpos;
	double	m_Middlepos;
	double	m_Minpos;
	double	m_Centerpos;
	double	m_uppos;
	double	m_Downpos;

public:

	void	DrawTool(CDC* pDC);
	void	HideTool();

	void	DrawHolder(CDC *pDC, bool showHolder);
	void	HideHolder();
	void	InflateHolder();

	void	CreateToolLBDN(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBMV(CPoint pt, bool &bMouseCapture);
	void	CreateToolLBUP(CPoint pt, bool &bMouseCapture);

	void	MoveToolLBDN(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBMV(CPoint pt, bool &bMouseCapture);
	void	MoveToolLBUP(CPoint pt, bool &bMouseCapture);

	void	ChangeToolLBDN(CPoint pt, bool &bMouseCapture);
	void	ChangeToolLBMV(CPoint pt, bool &bMouseCapture);
	void	ChangeToolLBUP(CPoint pt, bool &bMouseCapture);

	bool	PtInToolHolderRect(CPoint pt);
	bool	PtInToolRegion(CPoint pt);

	bool	CalculateDrawingPositionStart(CPoint& rpointStart, CPoint ipointStart, CPoint ipointEnd);
	bool	CalculateDrawingPositionEnd(CPoint& rpointEnd, CPoint ipointStart, CPoint ipointEnd);

	//int	XYPointCount();
	void	InitPoint(CPoint pt);
	void	CalMinMaxPoint();
	void	OffsetCal();

	CPoint	m_ptRegion;
};

#endif // !defined(AFX_ANDREWPF_H__DF73A80E_EA7D_4E5A_AFED_76E253600021__INCLUDED_)
