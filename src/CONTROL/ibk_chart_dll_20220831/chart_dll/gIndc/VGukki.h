#if !defined(AFX_VGUKKI_H__EF0F565F_C3BD_4350_AB07_5BAF749A34BC__INCLUDED_)
#define AFX_VGUKKI_H__EF0F565F_C3BD_4350_AB07_5BAF749A34BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VGukki.h : header file
//

#include "VarsBase.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CVGukki window

class CVGukki : public CVarsBase 
{
// Construction
public:
	CVGukki(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);

private:
	struct _local_data
	{
		int	orgindex;
		double	fpr;
		double	hpr;
		double	lpr;
		double	epr;
		struct _gIndex	date;		
	};

	struct PFTICK
	{
		CString	Context;
		CPoint	xyPoint;
		COLORREF	PenColor;
	};

	enum { EMPTY = 0, INC = 1, DEC = 2};

	bool	m_bCheck;
	bool	m_bCalculated;

	int*	m_piGukkiData;
	double*	m_piXpos;	
	int	m_iCurveN;
	int	m_iDrawHeight;
	int	m_iDrawWidth;
	int	m_iOnePrice;
	int	m_iJWPnt;
	int	m_iStartInx;
	int	m_iUserJWPnt;
	double	m_dOnePoint;
	bool	m_union;
	int	m_BlkDataCnt;
	int	m_btnindex;

	COLORREF	m_UpperColor;
	COLORREF	m_LowerColor;
	struct _local_data*	m_pLocal;
	CArray	<double, double>	m_jjumData;
	CArray	<double, double>	m_gjumData;


public:	
	void	DrawGraph(CDC* pDC);
	void	DrawUnionChart(CDC* pDC);
	CString	GetXposValue(CPoint pt);
	
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);

	bool	CreateData();
	void	SetMember();
	void	SetUserDefined(CString sVal);
	void	GetUsedValue(CString &sVal);

private:
	int	GetState(int PreVal, int CurVal);
	int	GetOnePrice();
	void	FindMinMax(int *pArray = NULL, int Size = 0);
	int	GetYPos(double val);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVGukki)
	//}}AFX_VIRTUAL

// Implementation	
public:
	virtual ~CVGukki();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VGUKKI_H__EF0F565F_C3BD_4350_AB07_5BAF749A34BC__INCLUDED_)
