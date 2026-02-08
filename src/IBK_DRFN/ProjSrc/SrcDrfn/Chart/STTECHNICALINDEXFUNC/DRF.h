// DRF.h: interface for the CDRF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRF_H__C5B10674_BDAD_46AE_9EF0_A0A5AB32AC3B__INCLUDED_)
#define AFX_DRF_H__C5B10674_BDAD_46AE_9EF0_A0A5AB32AC3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CDRF  : public CInnerFunction
{
public:
	CDRF();
	virtual ~CDRF();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_DRF_H__C5B10674_BDAD_46AE_9EF0_A0A5AB32AC3B__INCLUDED_)
