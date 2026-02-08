#if !defined(AFX_VPNF_H__0319E536_E6AF_4502_9C3B_4922B6FB7A48__INCLUDED_)
#define AFX_VPNF_H__0319E536_E6AF_4502_9C3B_4922B6FB7A48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarsBase.h"

class CVPNF : public CVarsBase  
{
public:
	CVPNF(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CVPNF();

private:
	struct _local_data
	{
		int	gap;
		double	fpr;
		double	hpr;
		double	lpr;
		double	epr;
		double	dSDay;	// 보조차트를위한 x축 unique값 yyyymmddhhmmss
		double	dEDay;	// 보조차트를위한 x축 unique값 yyyymmddhhmmss
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

	COLORREF	m_UpperColor;
	COLORREF	m_LowerColor;
	struct _local_data*	m_pLocal;

	double	m_fUnit;

public:	
	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	void	DrawUnionChart(CDC* pDC);
	void	DrawReversChart(CDC* pDC);
	CString	GetXposValue(CPoint pt);
	
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);

	bool	CreateData();
	void	SetMember();
	void	SetUserDefined(CString sVal);
	void	GetUsedValue(CString &sVal);
	void	CalXpos();

private:
	int	GetState(int PreVal, int CurVal);
	int	GetOnePrice();
	void	FindMinMax(int *pArray = NULL, int Size = 0);
	int	GetYPos(double val);
//	void	drawData(CDC* pDC);

// kangmc추세선변경 
	virtual void	GetValueToPoint(CPoint& pt, double dXValue, double dYValue);
	virtual void	GetPtToPosValue(CPoint pt, double& dPos, double& dValue);
	virtual void	GetPtToEndvalPt(CPoint pt, CPoint &ept);
	virtual	CString GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr);
	double		GetOneDataSize();
	int		GetValueToXIdx(double dXValue);
	int		IsNoTime() { return 1;};


};

#endif // !defined(AFX_VPNF_H__0319E536_E6AF_4502_9C3B_4922B6FB7A48__INCLUDED_)
