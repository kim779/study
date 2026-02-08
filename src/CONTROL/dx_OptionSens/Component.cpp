// Component.cpp : implementation file
//

#include "stdafx.h"
#include "DX_OptionSens.h"
#include "Component.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComponent

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CComponent, CCmdTarget)

CComponent::CComponent()
{
	m_sRemain = "0";
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
	DISP_PROPERTY_EX(CComponent, "Underlying", GetUnderlying, SetUnderlying, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "CurrValue", GetCurrValue, SetCurrValue, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "CD", GetCD, SetCD, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "RemainDays", GetRemainDays, SetRemainDays, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Delta", GetDelta, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Gamma", GetGamma, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Theta", GetTheta, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Rho", GetRho, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Vegga", GetVegga, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "TheoryValue", GetTheoryValue, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "IV", GetIV, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CComponent, "Volatility", GetVolatility, SetVolatility, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IComponent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {6AA3219B-F740-4DCA-A9FA-41E472AC7385}
static const IID IID_IComponent =
{ 0x6aa3219b, 0xf740, 0x4dca, { 0xa9, 0xfa, 0x41, 0xe4, 0x72, 0xac, 0x73, 0x85 } };

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

BSTR CComponent::GetUnderlying() 
{
	CString strResult = m_sUnder;
	return strResult.AllocSysString();
}

void CComponent::SetUnderlying(LPCTSTR lpszNewValue) 
{
	m_sUnder = lpszNewValue;
	m_sUnder.Replace("-", "");
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

BSTR CComponent::GetRemainDays() 
{
	CString strResult = m_sRemain;
	return strResult.AllocSysString();
}

void CComponent::SetRemainDays(LPCTSTR lpszNewValue) 
{
	m_sRemain = lpszNewValue;
}

BSTR CComponent::GetDelta() 
{
	CString strResult;
	OPTION	option = getStOption();

	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.04f", m_optionCalc.GetOptionDelta(&option));
		if (strResult.Find("#") > -1)
			strResult = "0.0000";
	}
	return strResult.AllocSysString();
}

BSTR CComponent::GetGamma() 
{
	CString strResult;
	OPTION	option = getStOption();

	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.04f", m_optionCalc.GetOptionGamma(&option));
		if (strResult.Find("#") > -1)
			strResult = "0.0000";
	}
	return strResult.AllocSysString();
}

BSTR CComponent::GetTheta() 
{
	CString strResult;
	OPTION	option = getStOption();

	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.04f", m_optionCalc.GetOptionTheta(&option));
		if (strResult.Find("#") > -1)
			strResult = "0.0000";
	}
	return strResult.AllocSysString();
}

BSTR CComponent::GetRho() 
{
	CString strResult;
	OPTION	option = getStOption();

	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.04f", m_optionCalc.GetOptionRho(&option));
		if (strResult.Find("#") > -1)
			strResult = "0.0000";
	}
	return strResult.AllocSysString();
}

BSTR CComponent::GetVegga() 
{
	CString strResult;
	OPTION	option = getStOption();

	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.04f", m_optionCalc.GetOptionVegga(&option));
		if (strResult.Find("#") > -1)
			strResult = "0.0000";
	}
	return strResult.AllocSysString();
}

BSTR CComponent::GetTheoryValue() 
{
	CString strResult;
	OPTION	option = getStOption();

	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.02f", m_optionCalc.GetOptionTheoryAmt(&option));
		if (strResult.Find("#") > -1)
			strResult = "0.00";
	}
	return strResult.AllocSysString();
}

BSTR CComponent::GetIV() 
{
	CString strResult;
	OPTION	option = getStOption();

	if (option.currAmt > 0 && option.strike > 0)
	{
		strResult.Format("%.02f", m_optionCalc.GetOptionInsideVolatility(&option) * 100);
		if (strResult.Find("#") > -1)
			strResult = "0.00";
	}
	return strResult.AllocSysString();
}

OPTION CComponent::getStOption()
{
	OPTION	option{};
	
	strcpy(option.code, m_sJCode);
	option.cd          = atof(m_sCD) / 100;
	option.baedangJisu = atof(m_sBdjs) / 10000;
	option.currAmt     = atof(m_sCurr);
	option.remainDays  = atof(m_sRemain) / 365;
	option.underlying  = atof(m_sUnder);
	option.volatility  = atof(m_sVola) / 100;
	option.strike      = atof(m_sStrike);

	return option;
}

BSTR CComponent::GetVolatility() 
{
	CString strResult = m_sVola;
	return strResult.AllocSysString();
}

void CComponent::SetVolatility(LPCTSTR lpszNewValue) 
{
	m_sVola = lpszNewValue;
}
