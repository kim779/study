// AMA.h: interface for the CAMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMA_H__4AE3F18C_2E72_4C9C_B5C6_3C0369ACDC88__INCLUDED_)
#define AFX_AMA_H__4AE3F18C_2E72_4C9C_B5C6_3C0369ACDC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CAMA   : public CInnerFunction
{
public:
	CAMA();
	virtual ~CAMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_AMA_H__4AE3F18C_2E72_4C9C_B5C6_3C0369ACDC88__INCLUDED_)
