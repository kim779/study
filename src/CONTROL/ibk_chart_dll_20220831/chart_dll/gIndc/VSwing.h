// VSwing.h: interface for the CVSwing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSWING_H__8A7175D6_2A1B_4B9F_9D2C_D52E667906CC__INCLUDED_)
#define AFX_VSWING_H__8A7175D6_2A1B_4B9F_9D2C_D52E667906CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarsBase.h"

class CVSwing : public CVarsBase  
{
public:
	CVSwing(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CVSwing();
	
private:
	struct _local_data
	{
		double	fpr;
		double	hpr;
		double	lpr;
		double	epr;
		int		iGap;
		int		iSOrgidx;//OrgData start idx
		int		iEOrgidx;//OrgData end idx
		int		UpDn;
		int		sval;
		int		HLidx;
	
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
	
	int*	m_piPnFData;
	int*	m_piXpos;	
	int	m_iDrawHeight;
	int	m_iDrawWidth;
	int	m_iOnePrice;
	int	m_iJWPnt;
	int	m_iStartInx;
	int	m_iUserJWPnt;
	double	m_dOnePoint;
	double	m_fUnit;
	int			m_iViweOrgDataCnt;		//화면상에보이는 org봉의 갯수
	int			m_OrgInDataStart;		//화면상에보이는 org봉data 시작idx
	
	int			m_iCurveN;			//삼선 Data 전체갯수
	int			m_iViweDataCnt;		//화면상에 보이는 삼선 차트 갯수
	int			m_iViweSidx;		//화면상에 보이는 삼선 차트 start idx
	int			m_iViweEidx;		//화면상에 보이는 삼선 차트 end idx
	

	COLORREF	m_UpperColor;
	COLORREF	m_LowerColor;
	struct _local_data*	m_pLocal;
	
	
	public:	
		void	DrawGraph(CDC* pDC);
		void	DrawUnionChart(CDC* pDC);
		CString	GetXposValue(CPoint pt);
		
		bool	CalculateMinMax(bool bForce = false);
		bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
		CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
		CString GetDisplayPosData(CPoint pt, bool bGrid = true);
		int		GetYPos(double val);			
		bool	CheckMinMax();
		bool	CreateData();
		void	SetMember();
		void	SetUserDefined(CString sVal);
		void	GetUsedValue(CString &sVal);
	
	private:
		int	GetState(int PreVal, int CurVal);
		int	GetOnePrice();
		void	FindMinMax(int *pArray = NULL, int Size = 0);
		//	void	drawData(CDC* pDC);
		
		
};

#endif // !defined(AFX_VSWING_H__8A7175D6_2A1B_4B9F_9D2C_D52E667906CC__INCLUDED_)
