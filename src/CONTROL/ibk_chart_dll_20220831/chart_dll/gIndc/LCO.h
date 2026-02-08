// LCO.h: interface for the CLCO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LCO_H__AD1E1156_884D_424B_84AB_3D82C975638B__INCLUDED_)
#define AFX_LCO_H__AD1E1156_884D_424B_84AB_3D82C975638B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLCO : public CLineBase  
{
public:
	CLCO(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLCO();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LCO_H__AD1E1156_884D_424B_84AB_3D82C975638B__INCLUDED_)
