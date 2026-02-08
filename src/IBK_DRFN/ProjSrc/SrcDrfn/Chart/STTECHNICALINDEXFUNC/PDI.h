// PDI.h: interface for the CPDI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDI_H__53FD33FB_E963_4F87_ADCA_A0C1C35EF0E0__INCLUDED_)
#define AFX_PDI_H__53FD33FB_E963_4F87_ADCA_A0C1C35EF0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CPDI  : public CInnerFunction
{
public:
	CPDI();
	virtual ~CPDI();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_PDI_H__53FD33FB_E963_4F87_ADCA_A0C1C35EF0E0__INCLUDED_)
