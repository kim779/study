// Std.h: interface for the CStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STD_H__EBC959D1_8541_41B8_9E4E_608348452A77__INCLUDED_)
#define AFX_STD_H__EBC959D1_8541_41B8_9E4E_608348452A77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CStd  : public CInnerFunction
{
public:
	CStd();
	virtual ~CStd();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_STD_H__EBC959D1_8541_41B8_9E4E_608348452A77__INCLUDED_)
