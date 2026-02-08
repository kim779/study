// FeeCal.h: interface for the CFeeCal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEECAL_H__16322698_A31D_4BC6_967A_158670EFED95__INCLUDED_)
#define AFX_FEECAL_H__16322698_A31D_4BC6_967A_158670EFED95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFeeCal  
{
public:
	virtual ~CFeeCal() {};
	virtual void Init() = 0;
	virtual void Entry(double dPrice, long lVolume) = 0;
	virtual double Exit(double dPrice, long lVolume) = 0;
protected:
	CFeeCal() {};
};

#endif // !defined(AFX_FEECAL_H__16322698_A31D_4BC6_967A_158670EFED95__INCLUDED_)
