// CCI.h: interface for the CCCI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCI_H__C003BFCA_80CB_4AE1_AEFA_B71B17A15BBF__INCLUDED_)
#define AFX_CCI_H__C003BFCA_80CB_4AE1_AEFA_B71B17A15BBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CCCI  : public CInnerFunction
{
public:
	CCCI();
	virtual ~CCCI();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_CCI_H__C003BFCA_80CB_4AE1_AEFA_B71B17A15BBF__INCLUDED_)
