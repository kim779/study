// ExceptionHandler.h: interface for the CExceptionHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCEPTIONHANDLER_H__BDA43461_004D_45DF_93A0_4875811FBA6A__INCLUDED_)
#define AFX_EXCEPTIONHANDLER_H__BDA43461_004D_45DF_93A0_4875811FBA6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 에러가 났을때 호출할 interface 를 정의합니다.
class IExceptionHandler
{
public:
	IExceptionHandler() {};
	virtual ~IExceptionHandler() {};

public:
	virtual void ExceptionInformation(CString strInfo) = 0;
};

class CExceptionHandler  
{
public:
	CExceptionHandler();
	virtual ~CExceptionHandler();

public:
	static IExceptionHandler* m_pOuter;

	void SetConnectionPoint(IExceptionHandler *pPoint) { m_pOuter = pPoint; }

};

#endif // !defined(AFX_EXCEPTIONHANDLER_H__BDA43461_004D_45DF_93A0_4875811FBA6A__INCLUDED_)
