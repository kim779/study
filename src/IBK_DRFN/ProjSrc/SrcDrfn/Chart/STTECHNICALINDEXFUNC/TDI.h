// TDI.h: interface for the CTDI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDI_H__BD6CB1DA_E314_4049_9C33_AA44C8E04F76__INCLUDED_)
#define AFX_TDI_H__BD6CB1DA_E314_4049_9C33_AA44C8E04F76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CTDI  : public CInnerFunction
{
public:
	CTDI();
	virtual ~CTDI();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_TDI_H__BD6CB1DA_E314_4049_9C33_AA44C8E04F76__INCLUDED_)
