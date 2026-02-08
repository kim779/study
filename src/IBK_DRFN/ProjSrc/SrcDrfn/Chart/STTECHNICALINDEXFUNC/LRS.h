// LRS.h: interface for the CLRS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LRS_H__3009351A_FDCB_4505_9961_61EC221A7E5B__INCLUDED_)
#define AFX_LRS_H__3009351A_FDCB_4505_9961_61EC221A7E5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CLRS  : public CInnerFunction
{
public:
	CLRS();
	virtual ~CLRS();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_LRS_H__3009351A_FDCB_4505_9961_61EC221A7E5B__INCLUDED_)
