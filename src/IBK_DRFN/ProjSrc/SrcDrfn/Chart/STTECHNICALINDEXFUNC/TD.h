// TD.h: interface for the CTD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TD_H__8188FA6B_6C1D_4A8C_A7D5_FAF24F3E2117__INCLUDED_)
#define AFX_TD_H__8188FA6B_6C1D_4A8C_A7D5_FAF24F3E2117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CTD  : public CInnerFunction
{
public:
	CTD();
	virtual ~CTD();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_TD_H__8188FA6B_6C1D_4A8C_A7D5_FAF24F3E2117__INCLUDED_)
