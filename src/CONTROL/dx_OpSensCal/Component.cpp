// Component.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "DX_OpSensCal.h"
#include "Component.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILE_CONFIG	"CFG.INI"
#define	MINGAM		"MINGAM"

/////////////////////////////////////////////////////////////////////////////
// CComponent

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CComponent, CCmdTarget)

CComponent::CComponent()
{
	m_sRoot = "";
	m_sUserName = "";
	m_sBzRemain = "0";
	m_sCalRemain = "0";
	EnableAutomation();
}

CComponent::~CComponent()
{
}


void CComponent::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CComponent, CCmdTarget)
	//{{AFX_MSG_MAP(CComponent)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CComponent, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CComponent)
	DISP_PROPERTY_EX(CComponent, "JCode", GetJCode, SetJCode, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "BedangJisu", GetBedangJisu, SetBedangJisu, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Strike", GetStrike, SetStrike, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "CurrValue", GetCurrValue, SetCurrValue, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "CD", GetCD, SetCD, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Delta", GetDelta, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Gamma", GetGamma, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Theta", GetTheta, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Rho", GetRho, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Vegga", GetVegga, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "TheoryValue", GetTheoryValue, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "IV", GetIV, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "BzRemainDays", GetBzRemainDays, SetBzRemainDays, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "CalRemainDays", GetCalRemainDays, SetCalRemainDays, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "K200Cur", GetK200Cur, SetK200Cur, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "FutureCur", GetFutureCur, SetFutureCur, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Time", GetTime, SetTime, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "UserName", GetUserName, SetUserName, VT_BSTR)
	DISP_FUNCTION(CComponent, "Calc", Calc, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CComponent, "ReadCfg", ReadCfg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CComponent, "SetHv", SetHv, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CComponent, "symIV", symIV, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CComponent, "symDELTA", symDELTA, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CComponent, "symGAMMA", symGAMMA, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CComponent, "symTHETA", symTHETA, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CComponent, "symVEGGA", symVEGGA, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CComponent, "symRHO", symRHO, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CComponent, "CalcEx", CalcEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IComponent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {884DF6AE-C71F-4d50-8AA1-F9118DD0BD8D}
static const IID IID_IComponent =
{ 0x884df6ae, 0xc71f, 0x4d50, { 0x8a, 0xa1, 0xf9, 0x11, 0x8d, 0xd0, 0xbd, 0x8d } };

BEGIN_INTERFACE_MAP(CComponent, CCmdTarget)
	INTERFACE_PART(CComponent, IID_IComponent, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComponent message handlers

BSTR CComponent::GetJCode() 
{
	CString strResult = m_sJCode;

	return strResult.AllocSysString();
}

void CComponent::SetJCode(LPCTSTR lpszNewValue) 
{
	m_sJCode = lpszNewValue;
}

BSTR CComponent::GetBedangJisu() 
{
	CString strResult = m_sBdjs;

	return strResult.AllocSysString();
}

void CComponent::SetBedangJisu(LPCTSTR lpszNewValue) 
{
	m_sBdjs = lpszNewValue;
}

BSTR CComponent::GetStrike() 
{
	CString strResult = m_sStrike;

	return strResult.AllocSysString();
}

void CComponent::SetStrike(LPCTSTR lpszNewValue) 
{
	m_sStrike = lpszNewValue;
}

BSTR CComponent::GetCurrValue() 
{
	CString strResult = m_sCurr;

	return strResult.AllocSysString();
}

void CComponent::SetCurrValue(LPCTSTR lpszNewValue) 
{
	m_sCurr = lpszNewValue;
	m_sCurr.Replace("-", "");
}

BSTR CComponent::GetCD() 
{
	CString strResult = m_sCD;
	return strResult.AllocSysString();
}

void CComponent::SetCD(LPCTSTR lpszNewValue) 
{
	m_sCD = lpszNewValue;
}

BSTR CComponent::GetDelta() 
{
	CString strResult = m_sDelta;
	return strResult.AllocSysString();
}

BSTR CComponent::GetGamma() 
{
	CString strResult = m_sGamma;
	return strResult.AllocSysString();
}

BSTR CComponent::GetTheta() 
{
	CString strResult = m_sTheta;
	return strResult.AllocSysString();
}

BSTR CComponent::GetRho() 
{
	CString strResult = m_sRho;
	return strResult.AllocSysString();
}

BSTR CComponent::GetVegga() 
{
	CString strResult = m_sVegga;
	return strResult.AllocSysString();
}

BSTR CComponent::GetIV() 
{
	CString strResult = m_sIV;
	return strResult.AllocSysString();
}

int CComponent::GetTimeMin(CString sTime)
{
	int	hh = 0, mm = 0;

	sTime.TrimRight();
	sTime.Replace(":", "");
	if (sTime.GetLength() < 4)
		return 0;

	hh = atoi(sTime.Left(2));
	mm = atoi(sTime.Mid(2, 2));
	return (hh * 60 + mm);
}

OPTION CComponent::getStOption(bool theoryType)
{
	CString val;
	OPTION	option{};
	double	dRemainDay = 0;

	CopyMemory(option.code, m_sJCode, min(sizeof(option.code), m_sJCode.GetLength()));
	option.cd          = atof(m_sCD) / 100;
	option.baedangJisu = atof(m_sBdjs);// / 1000; //lsh20060327
	option.currAmt     = atof(m_sCurr);

	if (atoi(m_sRDType) == 0)
		dRemainDay = atof(m_sBzRemain);	// 영업일수
	else
		dRemainDay = atof(m_sCalRemain);// 달력일수
	
	if (atoi(m_sTimeValue) == 1)
	{
		double	stime = 0, etime = 0, ctime = 0, timeloss = 0;

		if (m_sTime.Find("장마감") >= 0)
			m_sTime = "1500";
		if (m_sTime.Find("장종료") >= 0)
			m_sTime = "1600";


		stime = GetTimeMin(m_sTimeStart);
		etime = GetTimeMin(m_sTimeEnd);
		ctime = GetTimeMin(m_sTime);

		if (ctime > stime)
		{
			if (ctime  > etime)
				ctime = etime;
			if (stime < etime)
				timeloss = (ctime - stime) / (etime - stime);
			dRemainDay = dRemainDay - timeloss;
		}
	}
	if (atoi(m_sRDType) == 0)
		dRemainDay = dRemainDay / 250.0;
	else
		dRemainDay = dRemainDay / 365.0;
	option.remainDays = dRemainDay;

	switch (atoi(m_sBPType))
	{
	case 1:	// 선물
		option.underlying = fabs(atof(m_sFutureCur));
		break;
	case 2:	// K200 & 선물 비율적용
		option.underlying = (fabs(atof(m_sK200Cur)) - atof(m_sBdjs)) / 100 * atof(m_sK200Rate) + fabs(atof(m_sFutureCur)) * (100 - atof(m_sK200Rate)) / 100;
		break;
	case 3:	// 자동계산
		option.underlying = fabs(atof(m_sFutureCur)) * exp(-1 * atof(m_sCD) / 100 * dRemainDay);
		break;
	//case 0:
	default:// K200
		option.underlying  = fabs(atof(m_sK200Cur)) - option.baedangJisu;
		break;
	}
	option.strike = atof(m_sStrike);

	if (theoryType) // 이론가 계산용 변동성설정(2:사용자정의, 3: 역사적변동성 HVDay적용)
	{
		double	dval = 0;
		switch (atoi(m_sVolType))
		{
		case 1:
			Calc(CAL_IV);
			val= GetIV();
			dval = atof(val);
			break;
		case 2:
			// 20230125 파생상품 코드 개편  '1', 'A' : 선물
			//                              '2', 'B' : Call option
			//			        '3', 'C' : Put option
			//			        '4', 'D' : 스프레드
			if ((!m_sCD.IsEmpty()) && (m_sCD[0] == '2' || m_sCD[0] == 'B'))
				dval = atof(m_sUserCallIV);
			else
				dval = atof(m_sUserPutIV);
			break;
		//case 3:
		default:
			switch (atoi(m_sHVDay))
			{
			case 90:
				dval = m_dHV90;
				break;
			case 60:
				dval = m_dHV60;
				break;
			case 30:
				dval = m_dHV30;
				break;
			default:
				dval = m_dHV21;
				break;
			}
			break;
		}
		option.volatility  = dval / 100;
	}
	
	return option;
}

BSTR CComponent::GetBzRemainDays() 
{
	CString strResult;
	strResult = m_sBzRemain;

	return strResult.AllocSysString();
}

void CComponent::SetBzRemainDays(LPCTSTR lpszNewValue) 
{
	m_sBzRemain = lpszNewValue;
}

BSTR CComponent::GetCalRemainDays() 
{
	CString strResult;
	strResult = m_sCalRemain;

	return strResult.AllocSysString();
}

void CComponent::SetCalRemainDays(LPCTSTR lpszNewValue) 
{
	m_sCalRemain = lpszNewValue;
}

BSTR CComponent::GetK200Cur() 
{
	CString strResult;
	strResult = m_sK200Cur;

	return strResult.AllocSysString();
}

void CComponent::SetK200Cur(LPCTSTR lpszNewValue) 
{
	m_sK200Cur = lpszNewValue;
}

BSTR CComponent::GetFutureCur() 
{
	CString strResult;
	strResult = m_sFutureCur;

	return strResult.AllocSysString();
}

void CComponent::SetFutureCur(LPCTSTR lpszNewValue) 
{
	m_sFutureCur = lpszNewValue;
}

BSTR CComponent::GetTime() 
{
	CString strResult;
	strResult = m_sTime;

	return strResult.AllocSysString();
}

void CComponent::SetTime(LPCTSTR lpszNewValue) 
{
	m_sTime = lpszNewValue;
	m_sTime.Replace(":", "");
}

void CComponent::Calc(long CalcItem) 
{
	if (CalcItem == 0)
		CalcItem = CAL_IV | CAL_DELTA | CAL_GAMMA | CAL_THETA | CAL_VEGGA | CAL_RHO;

	OPTION option = getStOption(false);
	if (option.currAmt > 0 && option.strike > 0)
	{
		double iv, delta, gamma, theta, vegga, rho;
		m_optionCalc.Calc(CalcItem, &option, iv, delta, gamma, theta, vegga, rho);

		m_sIV.Format("%.02f", iv * 100);	if (m_sIV.Find("#") > -1) m_sIV = "0.00";
		m_sDelta.Format("%.04f", delta);	if (m_sDelta.Find("#") > -1) m_sDelta = "0.0000";
		m_sGamma.Format("%.04f", gamma);	if (m_sGamma.Find("#") > -1) m_sGamma = "0.0000";
		m_sTheta.Format("%.04f", theta);	if (m_sTheta.Find("#") > -1) m_sTheta = "0.0000";
		m_sVegga.Format("%.04f", vegga);	if (m_sVegga.Find("#") > -1) m_sVegga = "0.0000";
		m_sRho.Format("%.04f", rho);		if (m_sRho.Find("#") > -1) m_sRho = "0.0000";
	}
}

void CComponent::ReadCfg() 
{
/*
BPType=2	'기초자산설정(0:Kospi지수, 1:선물최근월물, 2:K200 & 선물 K200Rate, 3:자동계산)
VolType=0	'이론가 계산용 변동성설정(2:사용자정의, 3: 역사적변동성 HVDay적용)
RDType=1	'잔존일수설정(0:거래일수, 1:달력일수)
K200Rate=80	'K200과 선물비율(K200Rate이 80%이면 선물 20% 적용)
UserCallIV=1	'사용자설정CallIV
UserPutIV=1	'사용자설정PutIV
TimeValue=0	'장중 시간가치계산설정(0:미사용, 1:시간가치적용)
TimeStart=0900	'시간가치 시작시간
TimeEnd=1515	'시간가치 종료시간
HVDay=90	'역사적변동성일자
*/
	CString file;
	char tmps[256];
	file.Format("%s\\mtbl\\%s", m_sRoot, FILE_CONFIG);
	GetPrivateProfileString(MINGAM, "BPType", "3", tmps, sizeof(tmps), file);		m_sBPType.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "VolType", "0", tmps, sizeof(tmps), file);		m_sVolType.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "RDType", "1", tmps, sizeof(tmps), file);		m_sRDType.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "K200Rate", "80", tmps, sizeof(tmps), file);		m_sK200Rate.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "UserCallIV", "1", tmps, sizeof(tmps), file);		m_sUserCallIV.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "UserPutIV", "1", tmps, sizeof(tmps), file);		m_sUserPutIV.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "TimeValue", "0", tmps, sizeof(tmps), file);		m_sTimeValue.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "TimeStart", "0900", tmps, sizeof(tmps), file);		m_sTimeStart.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "TimeEnd", "1515", tmps, sizeof(tmps), file);		m_sTimeEnd.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "HVDay", "90", tmps, sizeof(tmps), file);		m_sHVDay.Format("%s", tmps);

	file.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, m_sUserName, FILE_CONFIG);
	GetPrivateProfileString(MINGAM, "BPType", m_sBPType, tmps, sizeof(tmps), file);		m_sBPType.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "VolType", m_sVolType, tmps, sizeof(tmps), file);	m_sVolType.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "RDType", m_sRDType, tmps, sizeof(tmps), file);		m_sRDType.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "K200Rate", m_sK200Rate, tmps, sizeof(tmps), file);	m_sK200Rate.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "UserCallIV", m_sUserCallIV, tmps, sizeof(tmps), file);	m_sUserCallIV.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "UserPutIV", m_sUserPutIV, tmps, sizeof(tmps), file);	m_sUserPutIV.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "TimeValue", m_sTimeValue, tmps, sizeof(tmps), file);	m_sTimeValue.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "TimeStart", m_sTimeStart, tmps, sizeof(tmps), file);	m_sTimeStart.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "TimeEnd", m_sTimeEnd, tmps, sizeof(tmps), file);	m_sTimeEnd.Format("%s", tmps);
	GetPrivateProfileString(MINGAM, "HVDay", m_sHVDay, tmps, sizeof(tmps), file);		m_sHVDay.Format("%s", tmps);	
}

BSTR CComponent::GetTheoryValue() 
{
	CString strResult = "";
	OPTION option = getStOption(true);
	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.02f", m_optionCalc.GetOptionTheoryAmt(&option));
		if (strResult.Find("#") > -1) strResult = "0.00";
	}
	return strResult.AllocSysString();
}

BSTR CComponent::GetUserName() 
{
	CString strResult;
	strResult = m_sUserName;

	return strResult.AllocSysString();
}

void CComponent::SetUserName(LPCTSTR lpszNewValue) 
{
	m_sUserName = lpszNewValue;
}

void CComponent::SetHv(LPCTSTR Hv90, LPCTSTR Hv60, LPCTSTR Hv30, LPCTSTR Hv21) 
{
	// TODO: Add your dispatch handler code here
	m_dHV90 = atof(Hv90);
	m_dHV60 = atof(Hv60);
	m_dHV30 = atof(Hv30);
	m_dHV21 = atof(Hv21);
}

void CComponent::symIV(LPCTSTR sym)	{	m_symIV= sym;		}
void CComponent::symDELTA(LPCTSTR sym)	{	m_symDelta = sym;	}
void CComponent::symGAMMA(LPCTSTR sym)	{	m_symGamma = sym;	}
void CComponent::symTHETA(LPCTSTR sym)	{	m_symTheta = sym;	}
void CComponent::symVEGGA(LPCTSTR sym)	{	m_symVegga = sym;	}
void CComponent::symRHO(LPCTSTR sym)	{	m_symRho = sym;		}

void CComponent::CalcEx(LPCTSTR cur, LPCTSTR k200cur, LPCTSTR fcur, LPCTSTR time) 
{
	m_sCurr		= cur;	m_sCurr.Replace("-", "");
	m_sTime		= time;	m_sTime.Replace(":", "");
	m_sK200Cur	= k200cur;
	m_sFutureCur	= fcur;

	Calc(0);
}

//void CComponent::SetData(CString sym, double value, bool dot2/* = false*/)
//{
//	if (bDot2)
//		sSym.Format("%s\t%.02f", sSym, dDat);
//	else
//		sSym.Format("%s\t%.04f", sSym, dDat);
//	m_pParent->SendMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)sSym.operator LPCTSTR());
//}
