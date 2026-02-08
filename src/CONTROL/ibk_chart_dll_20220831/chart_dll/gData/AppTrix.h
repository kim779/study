#if !defined(AFX_APPTRIX_H__783F408F_B352_4DFC_AC31_EF8E20C1600B__INCLUDED_)
#define AFX_APPTRIX_H__783F408F_B352_4DFC_AC31_EF8E20C1600B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppTrix : public CLineData  
{
public:
	CAppTrix(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppTrix();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
	double	m_prevVal[6];
};

#endif // !defined(AFX_APPTRIX_H__783F408F_B352_4DFC_AC31_EF8E20C1600B__INCLUDED_)
