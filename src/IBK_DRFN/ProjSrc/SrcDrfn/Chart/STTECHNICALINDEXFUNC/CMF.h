// CMF.h: interface for the CCMF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMF_H__5E19B5BA_D86F_4A38_A9E1_E4A0C129019F__INCLUDED_)
#define AFX_CMF_H__5E19B5BA_D86F_4A38_A9E1_E4A0C129019F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CCMF  : public CInnerFunction
{
public:
	CCMF();
	virtual ~CCMF();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_CMF_H__5E19B5BA_D86F_4A38_A9E1_E4A0C129019F__INCLUDED_)
