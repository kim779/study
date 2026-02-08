// CSar.h: interface for the CCSar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSAR_H__474FE662_FF48_4B74_806D_AD7094684D69__INCLUDED_)
#define AFX_CSAR_H__474FE662_FF48_4B74_806D_AD7094684D69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CCSar   : public CInnerFunction
{
public:
	CCSar();
	virtual ~CCSar();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_CSAR_H__474FE662_FF48_4B74_806D_AD7094684D69__INCLUDED_)
