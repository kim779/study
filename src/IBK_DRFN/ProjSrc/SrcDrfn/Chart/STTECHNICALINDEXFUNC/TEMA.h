// TEMA.h: interface for the CTEMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMA_H__D485238E_0830_485B_9D28_CBE66ED01A9D__INCLUDED_)
#define AFX_TEMA_H__D485238E_0830_485B_9D28_CBE66ED01A9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CTEMA : public CInnerFunction 
{
public:
	CTEMA();
	virtual ~CTEMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_TEMA_H__D485238E_0830_485B_9D28_CBE66ED01A9D__INCLUDED_)
