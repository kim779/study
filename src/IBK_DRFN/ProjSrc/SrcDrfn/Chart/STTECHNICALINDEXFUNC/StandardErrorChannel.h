// StandardErrorChannel.h: interface for the CStandardErrorChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STANDARDERRORCHANNEL_H__4A6229AE_6428_4D67_AAAC_0AB5C42235E2__INCLUDED_)
#define AFX_STANDARDERRORCHANNEL_H__4A6229AE_6428_4D67_AAAC_0AB5C42235E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CStandardErrorChannel   : public CInnerFunction
{
public:
	CStandardErrorChannel();
	virtual ~CStandardErrorChannel();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTControllerD *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_STANDARDERRORCHANNEL_H__4A6229AE_6428_4D67_AAAC_0AB5C42235E2__INCLUDED_)
