// Disparity_EMA.h: interface for the CDisparity_EMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPARITY_EMA_H__1AE1ED67_44BE_4AC3_8FCD_83997919F322__INCLUDED_)
#define AFX_DISPARITY_EMA_H__1AE1ED67_44BE_4AC3_8FCD_83997919F322__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CDisparity_EMA  : public CInnerFunction
{
public:
	CDisparity_EMA();
	virtual ~CDisparity_EMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_DISPARITY_EMA_H__1AE1ED67_44BE_4AC3_8FCD_83997919F322__INCLUDED_)
