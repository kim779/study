// RSI.h: interface for the CRSI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSI_H__D07A00A0_8B4B_46A9_AE5C_F28AB04F6EDB__INCLUDED_)
#define AFX_RSI_H__D07A00A0_8B4B_46A9_AE5C_F28AB04F6EDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CRSI   : public CInnerFunction
{
public:
	CRSI();
	virtual ~CRSI();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_RSI_H__D07A00A0_8B4B_46A9_AE5C_F28AB04F6EDB__INCLUDED_)
