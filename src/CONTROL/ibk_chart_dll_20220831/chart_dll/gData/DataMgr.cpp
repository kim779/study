// DataMgr.cpp: implementation of the CDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataMgr.h"
#include <math.h>

#include "AppMA.h"
#include "AppNet.h"
#include "AppPsy.h"
#include "AppDisparity.h"
#include "AppSona.h"
#include "AppSonaM.h"
#include "AppMACD.h"
#include "AppStoch.h"
#include "AppObv.h"
#include "AppVR.h"
#include "AppRate.h"
#include "AppADR.h"
#include "AppADLine.h"
#include "AppDMI.h"
#include "AppCCI.h"
#include "AppParabolic.h"
#include "AppDemark.h"
#include "AppRSI.h"
#include "AppRSIWeight.h"		// 20090923 : ADD : RSI Weight 지표 추가
#include "AppTrix.h"
#include "AppWillR.h"
#include "AppMmt.h"
#include "AppABRatio.h"
#include "AppLFI.h"
#include "AppNCO.h"
#include "AppROC.h"
#include "AppSigma.h"
#include "AppMFI.h"
#include "AppNPsy.h"
#include "AppBol.h"
#include "AppEnv.h"
#include "AppPivot.h"
#include "AppEMV.h"
#include "AppCO.h"
#include "AppCV.h"
#include "AppMao.h"
#include "AppForeign.h"
#include "AppBandWidth.h"
#include "AppSonaM.h"
#include "AppMgjy.h"

#include "../../h/axisgenv.h"
#include "../../h/axisgwin.h"
#include "../../h/axisgobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataMgr::CDataMgr(CWnd *pParent)
{
	m_pParent = pParent;
	m_dDollar = 0;
}

CDataMgr::~CDataMgr()
{
	DetachData();
}

void CDataMgr::DetachData(int key)
{
	int	dataKey;
	class COrgData *pOrgData;
	class CAppData *pAppData;
	if (key == -1)
	{
		for (POSITION pos = m_mapOrgData.GetStartPosition(); pos; )
		{
			m_mapOrgData.GetNextAssoc(pos, dataKey, pOrgData);
			delete pOrgData;
			pOrgData = NULL;
		}
		m_mapOrgData.RemoveAll();
		
		for (POSITION pos = m_mapAppData.GetStartPosition(); pos; )
		{
			m_mapAppData.GetNextAssoc(pos, dataKey, pAppData);
			delete pAppData;
			pAppData = NULL;
		}
		m_mapAppData.RemoveAll();
	}
	else
	{
		if (!m_mapAppData.Lookup(key, pAppData))
			return;

		m_mapAppData.RemoveKey(key);
		delete pAppData;
		pAppData = NULL;
	}
}

int CDataMgr::CreateOrgData(char* pcData)
{
	struct _dataH* pdataH = (_dataH *)pcData;
	int iDtKey = pdataH->dkey;

	m_dDollar = 0;
	class COrgData* pOrgData = new COrgData(pcData, this);
	m_mapOrgData.SetAt(iDtKey, pOrgData);

	return iDtKey;
}

int CDataMgr::CreateAppData(class COrgData* pOrgData, int iKey, int iGKind, struct _chartinfo* pChartInfo, int iCOption)
{
	iKey = GetUnusedKey(false);

	class CAppData	*pAppData;
	switch (iGKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
	case GK_VOL:
	case GK_AMT:
		return -1;

	case GK_PMA:
	case GK_VMA:
	case GK_AMA:
		pAppData = new CAppMA(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_NET:
		pAppData = new CAppNet(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_PSY:
		pAppData = new CAppPsy(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_GAP:
		pAppData = new CAppDisparity(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_MACD:
	case GK_MACDOS:
		pAppData = new CAppMACD(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_PMAO:
	case GK_VMAO:
		pAppData = new CAppMAO(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_SONA:
		pAppData = new CAppSona(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_STOCHS:
	case GK_STOCHF:	
		pAppData = new CAppStoch(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_OBV:
		pAppData = new CAppOBV(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_VR:
		pAppData = new CAppVR(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_RRATE:
		pAppData = new CAppRate(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_ADR:
		pAppData = new CAppADR(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_ADLINE:
		pAppData = new CAppADLine(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_ADX:
	case GK_DMI:
		pAppData = new CAppDMI(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_CCI:
		pAppData = new CAppCCI(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_DEMARK:
		pAppData = new CAppDemark(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_RSI:
		pAppData = new CAppRSI(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_RSIWEIGHT: // 0090923 : CHANGE : RSI Weight 지표 추가
		pAppData = new CAppRSIWeight(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_TRIX:
		pAppData = new CAppTrix(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_WPR:
		pAppData = new CAppWillR(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_MOMENTUM:
		pAppData = new CAppMmt(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_ABRATIO:
		pAppData = new CAppABRatio(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_LFI:
		pAppData = new CAppLFI(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_NCO:
		pAppData = new CAppNCO(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_PROC:
	case GK_VROC:
		pAppData = new CAppROC(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_SIGMA:
		pAppData = new CAppSigma(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_MFI:
		pAppData = new CAppMFI(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_NPSY:
		pAppData = new CAppNPsy(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_BOLB:
		pAppData = new CAppBol(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;	
	case GK_PARB:
		pAppData = new CAppParabolic(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_ENVL:
		pAppData = new CAppEnv(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;	
	case GK_PIVOT:
		pAppData = new CAppPivot(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;	
	case GK_EMV:
		pAppData = new CAppEMV(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;	
	case GK_CO:
		pAppData = new CAppCO(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;	
	case GK_CV:
		pAppData = new CAppCV(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;	
	/** remarking by macho@2006.12.12 <begin>
	case GK_FEXRATE: 
	case GK_FOBRATE:
	case GK_FRGNVOL:
	case GK_ORGNVOL:
	case GK_FRGNAMT:
	case GK_ORGNAMT:
		pAppData =  new CAppForeign(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_MGJY:
		pAppData =  new CAppMgjy(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	remarking by macho@2006.12.12 <end> **/
	case GK_BANDWIDTH:
		pAppData =  new CAppBandWidth(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_SONAMOMENTUM:
		pAppData =  new CAppSonaM(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	case GK_FRGNHAVE: case GK_FRGNRATE: 
	case GK_PSNLPBUY: case GK_FRGNPBUY: case GK_ORGNPBUY:
	case GK_PSNLTBUY: case GK_FRGNTBUY: case GK_ORGNTBUY:
		pAppData = new CAppForeign(pOrgData, iKey, iGKind, pChartInfo, iCOption);
		break;
	default:
		return -1;
	}

	m_mapAppData.SetAt(iKey, pAppData);

	return iKey;
}

bool CDataMgr::AttachData(int iDtKey, int iCount, int iDummy, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData)
{
	if (iDtKey >= DKEY_APPDATA)
		return false;

	class COrgData* pOrgData;
	if (!GetData(iDtKey, pOrgData))
		return false;

	return pOrgData->AttachGraphData(iCount, iDummy, pcData, riDataL, pcCurDate, bTodayData);
}

bool CDataMgr::AttachData2(int dkey, int count, int dummy, char *pData, int &dataL, int id)
{
	if (dkey >= DKEY_APPDATA)
		return NULL;

	class COrgData	*pOrgData;
	if (!GetData(dkey, pOrgData))
		return NULL;

	return pOrgData->AttachGraphData2(count, dummy, pData, dataL, id);
}

bool CDataMgr::AttachMapData(int iDtKey, int iCount, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData)
{
	if (iDtKey >= DKEY_APPDATA)
		return false;

	class COrgData* pOrgData;
	if (!GetData(iDtKey, pOrgData))
		return false;

	return pOrgData->AttachMapData(iCount, pcData, riDataL, pcCurDate, bTodayData);
}

bool CDataMgr::GetData(int key, class COrgData*& pOrgData)
{
	if (key <= 0 || key >= DKEY_APPDATA)
		return false;

	if (m_mapOrgData.Lookup(key, pOrgData))
		return true;

	return false;
}

bool CDataMgr::GetData(int key, class CAppData *&pAppData)
{
	if (key <= 0 || key < DKEY_APPDATA)
		return false;

	if (m_mapAppData.Lookup(key, pAppData))
		return true;

	return false;
}

bool CDataMgr::GetOrgData(int key, class COrgData *&pOrgData)
{
	if (key <= 0 || key >= DKEY_APPDATA)
	{
		class CAppData *pAppData;

		if (m_mapAppData.Lookup(key, pAppData))
		{
			pAppData->GetOrgData(pOrgData);
			return true;
		}

		return false;
	}

	if (m_mapOrgData.Lookup(key, pOrgData))
		return true;

	return false;
}

int CDataMgr::GetUnusedKey(bool bOrgData)
{
	if (bOrgData)
	{
		class COrgData *pOrgData;
		for (int ii = DKEY_ORGDATA; ii < DKEY_APPDATA; ii++)
		{
			if (!m_mapOrgData.Lookup(ii, pOrgData))
				return ii;
		}
	}
	else
	{
		class CAppData *pAppData;
		for (int ii = DKEY_APPDATA; ii < DKEY_APPDATA + 200; ii++)
		{
			if (!m_mapAppData.Lookup(ii, pAppData))
				return ii;
		}
	}
	
	return -1;
}

int CDataMgr::ParseRealtime(CString sCode, CString strRTData, bool& rbIncrease)
{
	int iResponse = RTM_NO;

	int	iPos;
	bool	bSearch = false;
	class COrgData *pOrgData = nullptr;
	for (POSITION pos = m_mapOrgData.GetStartPosition(); pos; )
	{
		m_mapOrgData.GetNextAssoc(pos, iPos, pOrgData);
		// debug : 20090923
		CString strTemp = pOrgData->GetCode();
		if (pOrgData->GetCode() == sCode)
		{
			bSearch = true;
			break;
		}
	}
	
	if (!bSearch)
		return iResponse;

	CString	strSymbol;
	CString strVal;
	CMapStringToString mapRtmStore;
	mapRtmStore.RemoveAll();
	for (; !strRTData.IsEmpty(); )
	{
		iPos = strRTData.Find('\t');
		if (iPos < 0)
			break;

		strSymbol = strRTData.Left(iPos++);
		strRTData = strRTData.Mid(iPos);

		iPos = strRTData.Find('\t');
		if (iPos < 0)
			iPos = strRTData.GetLength();
		strVal = strRTData.Left(iPos++);

		mapRtmStore.SetAt(strSymbol, strVal);

		if (strRTData.GetLength() <= iPos)
			break;

		strRTData = strRTData.Mid(iPos);
	}
	

	CString	astrSymbol[10] = { R_CTIM, R_CURR, R_DIFF, R_UDYL, R_GVOL, R_MDGA, R_MSGA, R_SIGA, R_KOGA, R_JEGA};
	CString	astrBasicVal[10] = { "", "", "", "", "", "", "", "", "", ""};
	for (int ii = 0; ii < 10; ii++)
	{
		for (POSITION pos = mapRtmStore.GetStartPosition(); pos;)
		{
			mapRtmStore.GetNextAssoc(pos, strSymbol, strVal);
			if (strSymbol == astrSymbol[ii])
			{
				astrBasicVal[ii].Format(_T("%s"), strVal.GetString());
				break;
			}
		}
	}

	if (pOrgData->GetUnit() == GU_CODE && atoi(astrBasicVal[1]) && !atoi(astrBasicVal[8]))	// 장전/후 시간외 데이타 버림
		return iResponse;

	CString	strSiseVal;
	strSiseVal.Format(_T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"), 
		LPCTSTR(astrBasicVal[0]), LPCTSTR(astrBasicVal[1]), LPCTSTR(astrBasicVal[2]),
		LPCTSTR(astrBasicVal[3]), LPCTSTR(astrBasicVal[4]), LPCTSTR(astrBasicVal[5]),
		LPCTSTR(astrBasicVal[6]), LPCTSTR(astrBasicVal[7]), LPCTSTR(astrBasicVal[8]),
		LPCTSTR(astrBasicVal[9]));
	m_pParent->SendMessage(GEV_SISE, MAKEWPARAM(siseRTMData, 0), long(strSiseVal.operator LPCTSTR()));

	if (astrBasicVal[1].IsEmpty())
		return iResponse;

	CString	strUdyl = astrBasicVal[3];
	m_strCtim.Format(_T("%s"), LPCTSTR(astrBasicVal[0]));
	m_strCurr.Format(_T("%s"), LPCTSTR(astrBasicVal[1]));
	m_strGvol.Format(_T("%s"), LPCTSTR(astrBasicVal[4]));
	m_strSiga.Format(_T("%s"), LPCTSTR(astrBasicVal[7]));
	m_strKoga.Format(_T("%s"), LPCTSTR(astrBasicVal[8]));
	m_strJega.Format(_T("%s"), LPCTSTR(astrBasicVal[9]));

	astrSymbol[0].Format("%s", R_CVOL);
	astrSymbol[1].Format("%s", R_GAMT);
	astrBasicVal[0] = astrBasicVal[1] =  _T("");
	for (int ii = 0; ii < 2; ii++)
	{
		for (POSITION pos = mapRtmStore.GetStartPosition(); pos;)
		{
			mapRtmStore.GetNextAssoc(pos, strSymbol, strVal);
			if (strSymbol == astrSymbol[ii])
			{
				astrBasicVal[ii].Format(_T("%s"), strVal.GetString());
				break;
			}
		}
	}

	if (m_strCtim.GetLength() < 6 || astrBasicVal[0].IsEmpty())
		return iResponse;
	
	CString	strCurr = m_strCurr;
	if (sCode.GetLength() == 5)	// 업종은 체결부호 심볼이 안들어옴
		m_strCvol.Format(_T("%s"), LPCTSTR(astrBasicVal[0]));
	else
		m_strCvol.Format(_T("%s"), LPCTSTR(astrBasicVal[0].Mid(1)));
	m_strGamt.Format(_T("%s"), astrBasicVal[1].GetString());

	for (POSITION pos = m_mapOrgData.GetStartPosition(); pos; )
	{
		m_mapOrgData.GetNextAssoc(pos, iPos, pOrgData);
		if (pOrgData->GetCode() != sCode)
			continue;		
	
		iResponse = pOrgData->UpdateRTM(rbIncrease);		
		if (pOrgData->GetCurr() != atof(strCurr))
		{
			strVal.Format(_T("%s\t%s\t\t\t"), strCurr.GetString(), strUdyl.GetString());
			pOrgData->SetCurr(strVal);
			
			if (iResponse != RTM_SHIFT)
				iResponse = RTM_ALL;
		}
	}

	mapRtmStore.RemoveAll();

	return iResponse;
}

void CDataMgr::SetCurr(int dKey, CString sData)
{
	COrgData	*pOrgData;
	if (!GetData(dKey, pOrgData))
		return;

	pOrgData->SetCurr(sData);
}

void CDataMgr::SetStock(int dKey, CString sData)
{
	COrgData	*pOrgData;
	if (!GetData(dKey, pOrgData))
		return;

	pOrgData->SetStock(sData);
}

void CDataMgr::SetStockKind(int dKey, char cKind)
{
	COrgData* pOrgData;
	if (!GetData(dKey, pOrgData))
		return;

	pOrgData->SetStockKind(cKind);
}

void CDataMgr::GetRtsCode(CString &rcode)
{
	COrgData* pOrgData;
	if (!GetData(1, pOrgData))
		return;

	CString	code = pOrgData->GetCode();
	rcode.Format(_T("%s"), LPCTSTR(code));
}

//************** 이동평균(단순이동평균) ***************//
int CDataMgr::CalcSMA(double *source, double *dest, int count, int start, int size)
{
	int	ii = 0;
	double	total = 0.0;
	double	sum = 0;

	for (ii = start; ii < count; ii++)
	{
		if (source[ii] != GD_UNUSED)
			break;
	}

	start = ii;
	if (size <= 0 || (count - start) < size)
		return start + size - 1;
	
	for (ii = start ; ii < (start + size) ; ii++)
		sum += source[ii];

	dest[ii-1] = sum / (double)size;

	for (; ii < count; ii++)
	{	
		sum += source[ii] - source[ii - size];
		dest[ii] = sum / double(size);
	}

	return start + size - 1;
}

// Added by inho
double CDataMgr::CalcSMA(double *source, int size)
{
	int	ii = 0;
	double	sum = 0;

	if (size <= 0)
		return GD_UNUSED;

	for ( ii = 0 ; ii < size ; ii++ )
	{
		if (source[ii] == GD_UNUSED)
			return GD_UNUSED;
		
		sum += source[ii];	
	}

	return (sum / (double)size);
}


//************** 단순가중이동평균 ***************//
int CDataMgr::CalcSWMA(double *source, double *dest, int count, int start, int size)
{
	int ii = 0;
	double total = 0.0;
	for (ii = start; ii < count; ii++)
	{
		if (source[ii] != GD_UNUSED)
			break;
	}
	start = ii;
	if (count - start < size)
		return count;

	ii = start + size - 1;

	float	div = 0;
	for (int jj = 1; jj <= size; jj++)
		div += jj;

	int	kk = 0;
	for (; ii < count; ii++)
	{
		double	sum = 0;
		for (int jj = ii, kk = size; jj > ii - size; jj--, kk--)
			sum += source[jj]*kk;

		dest[ii] = sum / div;
	}

	return start + size - 1;
}

//************** 지수이동평균 ***************//
// M(1) = X(1)
// M(2) = R * X(2) + (1-R) * X(1)
// M(3) = R * X(3) + (1-R) * R * X(2) + (1-R)**2 * X(1)
// M(4) = R * X(4) + (1-R) * R * X(3) + (1-R)**2 * R * X(2) + (1-R)**3 * X(1)
// ....
// M(n) = Sum(2,n)[(1-R)**(n-ii) * R * X(ii)] + (1-R)**(n-1) * X(1)
// M(n) = R * X(n) + (1-R)*M(n-1)
int CDataMgr::CalcEMA(double *source, double *dest, int count, int start, int size)
{
	int	ii = 0;
	for (ii = start; ii < count; ii++)
	{
		if (source[ii] != GD_UNUSED)
			break;
	}
	start = ii;
	if (count - start < size)
		return count;

	double Ramda = 2.0 / (static_cast<double>(size) + 1);

	ii = start + size - 1;
	double	sum = pow(1-Ramda, (static_cast<double>(size)-1)) * source[start];
	for (int jj = ii ; jj > ii - size + 1; jj--)
		sum += pow(1-Ramda, static_cast<double>(ii)- static_cast<double>(jj)) * Ramda * source[jj];
	dest[ii] = sum;

	for (ii = ii+1; ii < count; ii++)
		dest[ii] = Ramda * source[ii] + (1-Ramda) * dest[ii-1];

	return (start + size - 1);
}

//************** 지수평활이평 ***************//
int CDataMgr::CalcESMA(double *source, double *dest, int count, int start, int size)
{
	int ii = 0;
	double SA = 2.0 / (static_cast<double>(size) + 1);
	double SAsum = 0.0;	

	for (ii = 0; ii < size; ii++)
		SAsum += pow(SA, double(ii));

	for (ii = start; ii < count; ii++)
	{
		if (source[ii] != GD_UNUSED)
			break;
	}
	start = ii;

	double total = 0.0;
	ii = start + size - 1;

	for (; ii >= start; ii--)
		total += source[ii] * pow(SA, static_cast<double>(start) + static_cast<double>(size) - 1 - static_cast<double>(ii));

	ii = start + size;
	dest[ii - 1]= (total / SAsum);
	
	for (; ii < count; ii++)
		dest[ii] = dest[ii - 1] + SA * (source[ii] - dest[ii - 1]);

	return start + size - 1;
}
/*
void CDataMgr::SetNoDeleteData(int iKey, bool bNoDeleteData)
{
	COrgData* pOrgData;
	GetOrgData(iKey, pOrgData);
	if (!pOrgData)
		return;

	pOrgData->SetNoDeleteData(bNoDeleteData);
}
*/