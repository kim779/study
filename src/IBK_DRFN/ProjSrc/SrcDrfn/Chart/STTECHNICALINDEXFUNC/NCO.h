// NCO.h: interface for the CNCO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCO_H__7603DF97_9D87_4E28_9F15_38253F1B4FC7__INCLUDED_)
#define AFX_NCO_H__7603DF97_9D87_4E28_9F15_38253F1B4FC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CNCO  : public CInnerFunction
{
public:
	CNCO();
	virtual ~CNCO();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_NCO_H__7603DF97_9D87_4E28_9F15_38253F1B4FC7__INCLUDED_)
