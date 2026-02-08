// EnvelopeUp.h: interface for the CEnvelopeUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVELOPEUP_H__92F5D6BC_320E_4FF5_841D_233B745DBF6D__INCLUDED_)
#define AFX_ENVELOPEUP_H__92F5D6BC_320E_4FF5_841D_233B745DBF6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CEnvelopeUp      : public CInnerFunction
{
public:
	CEnvelopeUp();
	virtual ~CEnvelopeUp();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_ENVELOPEUP_H__92F5D6BC_320E_4FF5_841D_233B745DBF6D__INCLUDED_)
