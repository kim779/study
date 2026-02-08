#if !defined(AFX_LCV_H__C6AE936A_2B4B_49C8_BBE9_EA6DC356963C__INCLUDED_)
#define AFX_LCV_H__C6AE936A_2B4B_49C8_BBE9_EA6DC356963C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLCV : public CLineBase  
{
public:
	CLCV(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLCV();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LCV_H__C6AE936A_2B4B_49C8_BBE9_EA6DC356963C__INCLUDED_)
