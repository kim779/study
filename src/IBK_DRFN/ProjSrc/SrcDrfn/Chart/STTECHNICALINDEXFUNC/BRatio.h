// BRatio.h: interface for the CBRatio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRATIO_H__45FA2339_853C_4E76_845F_71451EAE3D89__INCLUDED_)
#define AFX_BRATIO_H__45FA2339_853C_4E76_845F_71451EAE3D89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CBRatio : public CInnerFunction 
{
public:
	CBRatio();
	virtual ~CBRatio();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_BRATIO_H__45FA2339_853C_4E76_845F_71451EAE3D89__INCLUDED_)
