// GraphMgr.cpp: implementation of the CGraphMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphMgr.h"

#include "BAmt.h"
#include "BAVol.h"
#include "BBalance.h"
#include "BBollinger.h"
#include "BDemark.h"
#include "BEnvelope.h"
#include "BJugaChart.h"
#include "BMA.h"
#include "BParabolic.h"
#include "BPivot.h"
#include "BVolume.h"
#include "BAVol.h"

#include "VSamsun.h"
#include "VKagi.h"
#include "VRClock.h"
#include "VPNF.h"
#include "VPCV.h"
#include "VRenko.h"

#include "LPsy.h"
#include "LDisparity.h"
#include "LMacd.h"
#include "Lsona.h"
#include "LSonaM.h"
#include "LSlowStoch.h"
#include "LFastStoch.h"
#include "LObv.h"
#include "LVr.h"		
#include "LRate.h"
#include "LADR.h"
#include "LADLine.h"
#include "LADX.h"
#include "LBandWidth.h"
#include "LDMI.h"
#include "LCCI.h"
#include "LMAO.h"
#include "LRSI.h"
#include "LRSIWeight.h"	// 20090923 : ADD : RSI weight 지표 추가
#include "LTRIX.h"
#include "LWilliamR.h"
#include "LMomentom.h"
#include "LABRatio.h"
#include "LLFI.h"
#include "LNCO.h"
#include "LROC.h"
#include "LSigma.h"
#include "LMFI.h"
#include "LNewPsy.h"
#include "LEMV.h"
#include "LCO.h"
#include "LCV.h"
#include "LForeignRate.h"
#include "LForeignMM.h"
#include "LMgjy.h"
#include "VGukki.h"
#include "VSwing.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndcBase* CGraphMgr::CreateGraph(CWnd* pwndView, CWnd* pwndPnChart, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay)
{
	struct _graph* pGraph = (struct _graph*)pcGraph;
	int iType = pGraph->btGType;
	int iKind = pGraph->wGKind;

	CIndcBase* pIndcBase = NULL;
	switch (iType)
	{
	case GT_BONG:
		switch (iKind)
		{
		case GK_JPN:
		case GK_BAR:
		case GK_LIN: 
		case GK_POLE: 
		case GK_EQV:
		case GK_CDV:
			pIndcBase = new CBJugaChart(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_NET: 
		case GK_PMA:
		case GK_VMA:
		case GK_AMA:
			pIndcBase = new CBMA(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_BOLB:
			pIndcBase = new CBBollinger(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_ENVL:
			pIndcBase = new CBEnvelope(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_PARB:
			pIndcBase = new CBParabolic(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_VOL:
		case GK_EQVV:
		case GK_CDVV:
			pIndcBase = new CBVolume(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_AMT:
			pIndcBase = new CBAmt(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_DEMARK:
			pIndcBase = new CBDemark(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_PIVOT:
			pIndcBase = new CBPivot(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_AVOL:
			pIndcBase = new CBAVol(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_BALANCE:
			pIndcBase = new CBBalance(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;

		default:
			TRACE("Graph kind is invalid[%d]\n", iKind);
			break;
		}
		break;
	case GT_VARS:
		switch (iKind)
		{
		case GK_THREE:
			pIndcBase = new CVSamsun(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_PNF:
			pIndcBase = new CVPNF(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_KAGI:
			pIndcBase = new CVKagi(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_RENKO:
			pIndcBase = new CVRenko(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_CLOCK:
			pIndcBase = new CVRClock(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_PV:
			pIndcBase = new CVPCV(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_GUKKI:
			pIndcBase = new CVGukki(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_SWING:
			pIndcBase = new CVSwing(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		default:
			TRACE("Graph kind is invalid[%d]\n", iKind);
			break;
		}
		break;
	case GT_LINE:
		switch (iKind)
		{
		case GK_PSY:
			pIndcBase = new CLPsy(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_GAP:
			pIndcBase = new CLDisparity(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_BANDWIDTH:
			pIndcBase = new CLBandWidth(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_MACD:
		case GK_MACDOS:
			pIndcBase = new CLMACD(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_SONA:
			pIndcBase = new CLSona(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_SONAMOMENTUM:
			pIndcBase = new CLSonaM(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_STOCHS:
			pIndcBase = new CLSlowStoch(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_STOCHF:	
			pIndcBase = new CLFastStoch(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_OBV:
			pIndcBase = new CLOBV(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_VR:
			pIndcBase = new CLVR(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_RRATE:
			pIndcBase = new CLRate(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_ADR:
			pIndcBase = new CLADR(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_ADLINE:
			pIndcBase = new CLADLine(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_ADX:
			pIndcBase = new CLADX(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_DMI:
			pIndcBase = new CLDMI(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_CCI:
			pIndcBase = new CLCCI(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_PMAO: case GK_VMAO:
			pIndcBase = new CLMAO(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_RSI:
			pIndcBase = new CLRSI(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;			
		case GK_RSIWEIGHT: // 20090923 : CHANGE : RSI Weight 지표 추가
			pIndcBase = new CLRSIWeight(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_TRIX:
			pIndcBase = new CLTrix(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_WPR:
			pIndcBase = new CLWilliamR(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_MOMENTUM:
			pIndcBase = new CLMomentom(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_ABRATIO:
			pIndcBase = new CLABRatio(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_LFI:
			pIndcBase = new CLLFI(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_NCO:
			pIndcBase = new CLNCO(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_PROC: case GK_VROC:
			pIndcBase = new CLROC(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_SIGMA:
			pIndcBase = new CLSigma(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_MFI:
			pIndcBase = new CLMFI(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_NPSY:
			pIndcBase = new CLNewPsy(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_EMV:
			pIndcBase = new CLEMV(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_CO:
			pIndcBase = new CLCO(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_CV:
			pIndcBase = new CLCV(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		/** remarking by macho@2006.12.12 <begin>
		case GK_FOBRATE: case GK_FEXRATE:
			pIndcBase = new CLForeignRate(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		//** 2005.11.08 add & modify by macho
		case GK_FRGNVOL:
		case GK_FRGNAMT:
		case GK_ORGNVOL:
		case GK_ORGNAMT:
		//** 
			pIndcBase = new CLForeignMM(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_MGJY:
			pIndcBase = new CLMgjy(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		remarking by macho@2006.12.12 <end> **/
		case GK_FRGNRATE:case GK_FRGNHAVE:						// 외국인보유(비율), 외국인보유(수량)
		case GK_PSNLTBUY: case GK_FRGNTBUY: case GK_ORGNTBUY:	// 누적순매수(수량) - 개인,외인,기관
			pIndcBase = new CLForeignRate(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		case GK_PSNLPBUY: case GK_FRGNPBUY: case GK_ORGNPBUY:	// 순매수(수량) - 개인,외인,기관
			pIndcBase = new CLForeignMM(pwndView, pwndPnChart, pDataMgr, pcInfo, pcGraph, iDispDay);
			break;
		default:
			TRACE("Graph kind is invalid[%d]\n", iKind);
			break;
		}
		break;
	default:
		TRACE("Graph type is invalid[%d]\n", iType);
		break;
	}

	return pIndcBase;
}
