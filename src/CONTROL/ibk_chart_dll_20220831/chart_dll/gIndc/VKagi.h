// VKagi.h: interface for the CVKagi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VKAGI_H__07E73D65_AA5B_4E38_B0C3_97969E2A8A33__INCLUDED_)
#define AFX_VKAGI_H__07E73D65_AA5B_4E38_B0C3_97969E2A8A33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarsBase.h"
#include <afxtempl.h>

class CVKagi : public CVarsBase  
{
public:	
	CVKagi(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CVKagi();

private:
	bool	m_bCalculated;
	bool	m_bCheck;
	int	m_iValue;
	int	m_iShow;
	int	m_iBlankGap;
	double	m_dUnit;

	struct _local_data
	{
		double	adVal[2];
		struct _gIndex	gIndex;
		double	dOpen;	//fpr;
		double	dHigh;	//hpr;
		double	dLow;	//lpr;
		double	dClose;	//epr;
		int	iGap;
		double	dSDay;	// 보조차트를위한 x축 unique값 yyyymmddhhmmss
		double	dEDay;	// 보조차트를위한 x축 unique값 yyyymmddhhmmss
	};

	CArray<struct _local_data*, struct _local_data*>	m_arData;

public:	
	CString	GetXposValue(CPoint pt);

	void	DrawGraph(CDC* pDC);
	void	GetUsedValue(CString &sVal);

	// kangmc추세선변경 
	virtual void	GetValueToPoint(CPoint& pt, double dXValue, double dYValue);
	virtual void	GetPtToPosValue(CPoint pt, double& dPos, double& dValue);
	virtual void	GetPtToEndvalPt(CPoint pt, CPoint &ept);
	virtual	CString GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr);
	double	GetOneDataSize();
	int	GetValueToXIdx(double dXValue);
	int	IsNoTime() { return 1;};

	

private:
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	void	SetUserDefined(CString sVal);
	


	void	DrawChart(CDC* pDC);
	void	DrawUnionChart(CDC* pDC);

	bool	CreateData();
	void	RemoveData();
	double	GetDefVal();
	int	GetYPos(double dVal);
	void	SetMember();
	
};

#endif // !defined(AFX_VKAGI_H__07E73D65_AA5B_4E38_B0C3_97969E2A8A33__INCLUDED_)
