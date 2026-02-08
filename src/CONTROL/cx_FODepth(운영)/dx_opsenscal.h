class IOpSensCal : public COleDispatchDriver
{
public:
	IOpSensCal() {}		// Calls COleDispatchDriver default constructor
	IOpSensCal(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IOpSensCal(const IOpSensCal& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	void SetDelta(LPCTSTR);
	CString GetGamma();
	void SetGamma(LPCTSTR);
	CString GetTheta();
	void SetTheta(LPCTSTR);
	CString GetRho();
	void SetRho(LPCTSTR);
	CString GetVegga();
	void SetVegga(LPCTSTR);
	CString GetTheoryValue();
	void SetTheoryValue(LPCTSTR);
	CString GetIv();
	void SetIv(LPCTSTR);
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

public:
	void Calc(long CalcItem);
	void ReadCfg();
	void SetHv(LPCTSTR Hv90, LPCTSTR Hv60, LPCTSTR Hv30, LPCTSTR Hv21);
	void symIV(LPCTSTR sym);
	void symDELTA(LPCTSTR sym);
	void symGAMMA(LPCTSTR sym);
	void symTHETA(LPCTSTR sym);
	void symVEGGA(LPCTSTR sym);
	void symRHO(LPCTSTR sym);
	void CalcEx(LPCTSTR cur, LPCTSTR K200Cur, LPCTSTR fcur, LPCTSTR Time);
};
