// Grp_MA.h: interface for the CGrp_MA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRP_MA_H__C674EF42_F32A_4E28_AA58_E0C8FE58663C__INCLUDED_)
#define AFX_GRP_MA_H__C674EF42_F32A_4E28_AA58_E0C8FE58663C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Grp_Base.h"
#include <afxtempl.h>

#define	maxMA	4
struct	_maData
{
	double	nmav[maxMA];
};

class CGrp_MA : public CGrp_Base  
{
public:
	CGrp_MA(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_MA();

	void	DrawGraph(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax(bool bShift);
	CString	GetDisplayPosData(CPoint pt);
	CString	GetExcelData(int idx);

private:
	void	DrawMA(CDC* pDC);
	bool	DrawLineMA(CDC* pDC, int nDay, int style, int width, COLORREF color);
	int	CalculatePMA(int idx, int nDay);
	void	CalculateLastPMA();
	int	CalculateVMA(int idx, int nDay);
	void	CalculateLastVMA();

protected:
	CArray	<_maData *, _maData *> m_maQue;
	int	m_maCnt;
	int	m_pMA[maxMA];
	double	*m_pMAV;
};

#endif // !defined(AFX_GRP_MA_H__C674EF42_F32A_4E28_AA58_E0C8FE58663C__INCLUDED_)
