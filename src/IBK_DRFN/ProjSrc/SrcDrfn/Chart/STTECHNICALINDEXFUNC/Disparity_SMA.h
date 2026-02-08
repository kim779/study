// Disparity_SMA.h: interface for the CDisparity_SMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPARITY_SMA_H__B511D2C2_BEB9_4DAD_A227_840745AC41D0__INCLUDED_)
#define AFX_DISPARITY_SMA_H__B511D2C2_BEB9_4DAD_A227_840745AC41D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDisparity_SMA   : public CInnerFunction
{
public:
	CDisparity_SMA();
	virtual ~CDisparity_SMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
protected:
	CString m_strName;
};

#endif // !defined(AFX_DISPARITY_SMA_H__B511D2C2_BEB9_4DAD_A227_840745AC41D0__INCLUDED_)
