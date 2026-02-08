// ADX.h: interface for the CADX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADX_H__CC09A240_A23C_43F3_83AA_D39D39376336__INCLUDED_)
#define AFX_ADX_H__CC09A240_A23C_43F3_83AA_D39D39376336__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CADX  : public CInnerFunction
{
public:
	CADX();
	virtual ~CADX();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_ADX_H__CC09A240_A23C_43F3_83AA_D39D39376336__INCLUDED_)
