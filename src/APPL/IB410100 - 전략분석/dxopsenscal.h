// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IOpSensCal wrapper class
#pragma once
class IOpSensCal : public COleDispatchDriver
{
public:
	IOpSensCal() {}		// Calls COleDispatchDriver default constructor
	IOpSensCal(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IOpSensCal(const IOpSensCal& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetJCode();
	void SetJCode(LPCTSTR);
	CString GetBedangJisu();
	void SetBedangJisu(LPCTSTR);
	CString GetStrike();
	void SetStrike(LPCTSTR);
	CString GetCurrValue();
	void SetCurrValue(LPCTSTR);
	CString GetCd();
	void SetCd(LPCTSTR);
	CString GetDelta();
	CString GetGamma();
	CString GetTheta();
	CString GetRho();
	CString GetVegga();
	CString GetTheoryValue();
	CString GetIv();
	CString GetBzRemainDays();
	void SetBzRemainDays(LPCTSTR);
	CString GetCalRemainDays();
	void SetCalRemainDays(LPCTSTR);
	CString GetK200Cur();
	void SetK200Cur(LPCTSTR);
	CString GetFutureCur();
	void SetFutureCur(LPCTSTR);
	CString GetTime();
	void SetTime(LPCTSTR);
	CString GetUserName();
	void SetUserName(LPCTSTR);

// Operations
public:
	void Calc(long CalcItem);
	void ReadCfg();
	void SetHv(LPCTSTR Hv90, LPCTSTR Hv60, LPCTSTR Hv30, LPCTSTR Hv21);
};
