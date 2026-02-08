// FeeCalByPrice.cpp: implementation of the CFeeCalByPrice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "strategymanager.h"
#include "FeeCalByPrice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFeeCalByPrice::CFeeCalByPrice(double dFeeForEntry, double dFeeForExit)
{
	m_dFeeForEntry = dFeeForEntry;
	m_dFeeForExit = dFeeForExit;
}

CFeeCalByPrice::~CFeeCalByPrice()
{

}

void CFeeCalByPrice::Entry(double dPrice, long lVolume)
{
	m_dFee += ((m_dFeeForEntry)*lVolume);	
}

void CFeeCalByPrice::Init()
{
	m_dFee = 0.;
}

double CFeeCalByPrice::Exit(double dPrice, long lVolume)
{
	return (m_dFee + (m_dFeeForExit)*lVolume);
}
