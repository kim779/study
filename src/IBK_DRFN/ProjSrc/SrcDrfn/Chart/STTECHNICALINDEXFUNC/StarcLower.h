// StarcLower.h: interface for the CStarcLower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STARCLOWER_H__9174AB0F_C1EF_47A1_9225_D3A8B54D6897__INCLUDED_)
#define AFX_STARCLOWER_H__9174AB0F_C1EF_47A1_9225_D3A8B54D6897__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CStarcLower  : public CInnerFunction
{
public:
	CStarcLower();
	virtual ~CStarcLower();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STARCLOWER_H__9174AB0F_C1EF_47A1_9225_D3A8B54D6897__INCLUDED_)
