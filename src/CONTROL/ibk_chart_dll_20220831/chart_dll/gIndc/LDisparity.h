#if !defined(AFX_LDISPARITY_H__4F5DE21E_F402_47AF_856C_64FA34858B80__INCLUDED_)
#define AFX_LDISPARITY_H__4F5DE21E_F402_47AF_856C_64FA34858B80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLDisparity : public CLineBase  
{
public:
	CLDisparity(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLDisparity();
	
protected:
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);	
};

#endif // !defined(AFX_LDISPARITY_H__4F5DE21E_F402_47AF_856C_64FA34858B80__INCLUDED_)
