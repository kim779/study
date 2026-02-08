// GrpBasic.cpp: implementation of the CGrpBasic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GrpBasic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrpBasic::CGrpBasic()
{
	m_bDummy	= false;
	m_iOpen		= GD_UNUSED;
	m_iHigh		= GD_UNUSED;
	m_iLow		= GD_UNUSED;
	m_iClose	= GD_UNUSED;
	m_iMgjy		= GD_UNUSED;

	/** remarking by macho <begin>
	m_iOrgnVol	= 0;
	m_iOrgnAmt	= 0;
	m_iFrgnVol	= 0;
	m_iFrgnAmt	= 0;
	m_iFrgnObRate	= 0;
	m_iFrgnExRate	= 0;
	remarking by macho <end> **/

	//** macho added.
	m_frgnHave = m_frgnRate = m_psnlPBuy = m_psnlTBuy = m_frgnPBuy = m_frgnTBuy = m_orgnPBuy = m_orgnTBuy = 0;

	m_iUpCnt	= 0;
	m_iDnCnt	= 0;

	m_dVolume	= GD_UNUSED;
	m_dTrdMoney	= GD_UNUSED;

	m_dwCheck	= NULL;
	m_wRate		= NULL;

	ZeroMemory(&m_index, sizeof(struct _gIndex));
}

CGrpBasic::~CGrpBasic()
{

}
