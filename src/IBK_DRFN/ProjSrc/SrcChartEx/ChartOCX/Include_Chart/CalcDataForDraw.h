// CalcDataForDraw.h: interface for the CCalcDataForDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCDATAFORDRAW_H__2C95B996_76A7_433B_8F95_22AB39871F82__INCLUDED_)
#define AFX_CALCDATAFORDRAW_H__2C95B996_76A7_433B_8F95_22AB39871F82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CDFD_DECLARE( Type, Member)										\
	protected:																\
		Type *		Member;													\
		int			nSize_##Member;											\
	public:																	\
		void		Alloc_##Member( int p_nSize);							\
		Type *		Get_##Member( void)		{	return Member;		}		\
		int			GetSize_##Member( void)	{	return nSize_##Member;	}

class CCalcDataForDraw  
{
public:
	CCalcDataForDraw();

public:
	double m_dMax, m_dMin;
	int    m_nS_High, m_nH_High, m_nL_High, m_nE_High;
	int	   m_nS_Low, m_nH_Low, m_nL_Low, m_nE_Low;
	int	   m_nMerCnt_High, m_nMerCnt_Low;
	int    m_nCurXHigh, m_nCurXLow;
	bool   m_bLog, m_bInvert;
	CRect  m_graphRegion;
	CPoint m_Prept;
	int	   m_nHighLow;
	
public:
	int		 nFlag;
	int		 nDTI_Start, nDTI_End; 
	int		 nDTI_Start_LastPixel;
	int		 nIndex_LastPixel;
	int		 nMaxTop, nPreLeft;
	int		 nLastPixel_high, nLastPixel_Low, nLastPixel_Bohap;
	int		 nLastPolyCnt_high, nLastPolyCnt_Low, nLastPolyCnt_Bohap;
	int		 nArrowIndex;
	int		 m_nPoints;
		
	
	double	 dPreClose;
	double   dPreVolume;
	
	int		    nCount_High,	  nCount_Low,		nCount_Bohap,  nPtCnt;      //Bar °³¼ö	
	int			nCount_High2,	  nCount_Low2,		nCount_Bohap2;

	// (2009/2/27 - Seung-Won, Bae) Temporary Pointer (no memory)
	void *		m_pTemp;

public:
	bool	CheckYValue(double dMaxValue, double dMinValue, bool bLog, bool bInvert, const CRect& graphRegion, int nCntPoint);
	void    SetYValue(double dMaxValue, double dMinValue, bool bLog, bool bInvert, const CRect& graphRegion, int nCntPoint);

// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
friend class CSignalData;
friend class CSubGraphData;
protected:
	virtual ~CCalcDataForDraw();
public:
	void		ResetValue( void);
public:
	_CDFD_DECLARE( POINT, pptLine);
	_CDFD_DECLARE( double, dCurOpen);
	_CDFD_DECLARE( double, dCurClose);
	_CDFD_DECLARE( double, dCurVolume);
	_CDFD_DECLARE( CRect, rtGraph);
	_CDFD_DECLARE( POINT, lpPoint_High);
	_CDFD_DECLARE( INT, lpPolyCount_High);
	_CDFD_DECLARE( DWORD, lpdwcntPoly_High);
	_CDFD_DECLARE( POINT, lpPoint_Low);
	_CDFD_DECLARE( INT, lpPolyCount_Low);
	_CDFD_DECLARE( DWORD, lpdwcntPoly_Low);
	_CDFD_DECLARE( POINT, lpPoint_Bohap);
	_CDFD_DECLARE( INT, lpPolyCount_Bohap);
	_CDFD_DECLARE( DWORD, lpdwcntPoly_Bohap);
	int						nPolyCount_High;
	int						nPointCount_High;
	int						nLastPolyCount_High;
	int						nLastPointCount_High;
	int						nPolyCount_Low;
	int						nPointCount_Low;
	int						nLastPolyCount_Low;
	int						nLastPointCount_Low;

// (2010/2/25 - Seung-Won, Bae) Temporary Variable Buffer.
public:
	_CDFD_DECLARE( POINT,	lpPointBuffer);

protected:
	void	Allocated( int p_nSize);
};

#endif // !defined(AFX_CALCDATAFORDRAW_H__2C95B996_76A7_433B_8F95_22AB39871F82__INCLUDED_)
