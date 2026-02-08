// AppData.h: interface for the CAppData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPDATA_H__CB9C204F_7BFC_4381_A40C_890A3AE5D182__INCLUDED_)
#define AFX_APPDATA_H__CB9C204F_7BFC_4381_A40C_890A3AE5D182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "GrpBasic.h"

class AFX_EXT_CLASS CAppData  
{
public:

protected:
	bool	m_bCalc;
	int	m_iCOption;
	int	m_iDataSize;	
	int	m_iDtKey;		// data key - 데이타 구분자
	int	m_iGKind;
	int	m_iNLine;
	int	m_iNSignal;
	int	m_aiPIndex[12];
	double	m_dMax;
	double	m_dMin;
	double*	m_apdVal[12];		// 계산된값

	CString		m_strName;	// 데이타명

	class COrgData*		m_pOrgData;
	struct	_chartinfo*	m_pCInfo;	// calculate info

	
	
public:
	CAppData(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppData();

	int	GetDataKey()	{ return m_iDtKey; }
	int	GetDataCount();
	void	GetOrgData(class COrgData *&pOrgData)	{ pOrgData = m_pOrgData; }
	void	SetName(CString name) { m_strName = name; }
	virtual void	SetVariable(struct _chartinfo *pCInfo = NULL, int option = -1);
	virtual double GetVal(int lindex, int vindex);
	virtual bool Calculate(int start, int end, bool bForce = true)	{ return false; }
	virtual bool CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce = false);
	virtual bool CalculateLastIndex(bool bShift, bool bIncrease)	{ return false; }

protected:


};

#endif // !defined(AFX_APPDATA_H__CB9C204F_7BFC_4381_A40C_890A3AE5D182__INCLUDED_)
