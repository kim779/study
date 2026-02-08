// MDI.h: interface for the CMDI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDI_H__8DAC9C9B_F0B0_406D_9D01_AA4A7F5CF72B__INCLUDED_)
#define AFX_MDI_H__8DAC9C9B_F0B0_406D_9D01_AA4A7F5CF72B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CMDI  : public CInnerFunction
{
public:
	CMDI();
	virtual ~CMDI();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_MDI_H__8DAC9C9B_F0B0_406D_9D01_AA4A7F5CF72B__INCLUDED_)
