// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "dxopsenscal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IOpSensCal properties

CString IOpSensCal::GetJCode()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetJCode(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString IOpSensCal::GetBedangJisu()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetBedangJisu(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString IOpSensCal::GetStrike()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetStrike(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

CString IOpSensCal::GetCurrValue()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetCurrValue(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

CString IOpSensCal::GetCd()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetCd(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

CString IOpSensCal::GetDelta()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

CString IOpSensCal::GetGamma()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

CString IOpSensCal::GetTheta()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}

CString IOpSensCal::GetRho()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

CString IOpSensCal::GetVegga()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

CString IOpSensCal::GetTheoryValue()
{
	CString result;
	GetProperty(0xb, VT_BSTR, (void*)&result);
	return result;
}

CString IOpSensCal::GetIv()
{
	CString result;
	GetProperty(0xc, VT_BSTR, (void*)&result);
	return result;
}

CString IOpSensCal::GetBzRemainDays()
{
	CString result;
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetBzRemainDays(LPCTSTR propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}

CString IOpSensCal::GetCalRemainDays()
{
	CString result;
	GetProperty(0xe, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetCalRemainDays(LPCTSTR propVal)
{
	SetProperty(0xe, VT_BSTR, propVal);
}

CString IOpSensCal::GetK200Cur()
{
	CString result;
	GetProperty(0xf, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetK200Cur(LPCTSTR propVal)
{
	SetProperty(0xf, VT_BSTR, propVal);
}

CString IOpSensCal::GetFutureCur()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetFutureCur(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

CString IOpSensCal::GetTime()
{
	CString result;
	GetProperty(0x11, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetTime(LPCTSTR propVal)
{
	SetProperty(0x11, VT_BSTR, propVal);
}

CString IOpSensCal::GetUserName()
{
	CString result;
	GetProperty(0x12, VT_BSTR, (void*)&result);
	return result;
}

void IOpSensCal::SetUserName(LPCTSTR propVal)
{
	SetProperty(0x12, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IOpSensCal operations

void IOpSensCal::Calc(long CalcItem)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CalcItem);
}

void IOpSensCal::ReadCfg()
{
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IOpSensCal::SetHv(LPCTSTR Hv90, LPCTSTR Hv60, LPCTSTR Hv30, LPCTSTR Hv21)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Hv90, Hv60, Hv30, Hv21);
}
