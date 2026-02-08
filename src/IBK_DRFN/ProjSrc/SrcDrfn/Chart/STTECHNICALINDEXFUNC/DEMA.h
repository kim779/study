// DEMA.h: interface for the CDEMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMA_H__8D39EA55_0BD1_469C_B3CB_379715321227__INCLUDED_)
#define AFX_DEMA_H__8D39EA55_0BD1_469C_B3CB_379715321227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CDEMA : public CInnerFunction 
{
public:
	CDEMA();
	virtual ~CDEMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_DEMA_H__8D39EA55_0BD1_469C_B3CB_379715321227__INCLUDED_)
