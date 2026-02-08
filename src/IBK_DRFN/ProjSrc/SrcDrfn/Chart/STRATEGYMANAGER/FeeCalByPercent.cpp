// FeeCalByPercent.cpp: implementation of the CFeeCalByPercent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "strategymanager.h"
#include "FeeCalByPercent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFeeCalByPercent::CFeeCalByPercent(double dFeeForEntry, double dFeeForExit)
{
	m_dFeeForEntry = dFeeForEntry;
	m_dFeeForExit = dFeeForExit;
}

CFeeCalByPercent::~CFeeCalByPercent()
{

}

void CFeeCalByPercent::Entry(double dPrice, long lVolume)
{
	m_dFee += (dPrice*0.01*(m_dFeeForEntry)*lVolume);
}


void CFeeCalByPercent::Init()
{
	m_dFee = 0.;
}

double CFeeCalByPercent::Exit(double dPrice, long lVolume)
{
	return (m_dFee + dPrice*0.01*(m_dFeeForExit)*lVolume);
}


