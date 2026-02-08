// BandWidth.h: interface for the CBandWidth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANDWIDTH_H__E6A073E5_0D64_4807_97F9_984971A108B3__INCLUDED_)
#define AFX_BANDWIDTH_H__E6A073E5_0D64_4807_97F9_984971A108B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CBandWidth  : public CInnerFunction
{
public:
	CBandWidth();
	virtual ~CBandWidth();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_BANDWIDTH_H__E6A073E5_0D64_4807_97F9_984971A108B3__INCLUDED_)
