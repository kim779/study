// PacketListModule.h: interface for the CPacketListModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLISTMODULE_H__3D5B4B26_3ADC_43FB_81DF_D419F380923E__INCLUDED_)
#define AFX_PACKETLISTMODULE_H__3D5B4B26_3ADC_43FB_81DF_D419F380923E__INCLUDED_

#include "PacketBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseRealDataModule;

// (2009/10/20 - Seung-Won, Bae) Comment.
//		실시간 수신값의 가공을 처리하는 Module.
//		당일 거래량을 받아 처리하는 기본거래량과 거래대금 기능도 이것으로하면 좋을 듯한데...
//		Type을 두어 Logic을 교체하도록 되어 있는데, Type을 두개 이상 둘 수 있는 구조도 아니다.
//			(Packet 별로 실시간 수신시마다 바꿀 수 있는 구조 아님)
//		따라서, 항시 기본적으로 CRealDataProcessor를 두고, 내부적으로 Packet별로 연산을 위한 Logic Class를 연결시켜 처리하는 것이 좋겠다.
//			당연 Logic Class가 등록되지 않은 경우, 원값 그대로를 제공하는 것으로.
//		특히, 해당 Logic 검색은 Lookup을 쓰도록 개편이 필요하다.
//		보다 다양한 Data 가공 Logic의 확장성을 위해 개편 필요하나 다음 기회에...
class CRealDataModule  
{
public:
	enum REALDATAMODULETYPE
	{
		BASE_TYPE = 0,
		RATE_TYPE
	};

public:
	CRealDataModule( HWND p_hOcxWnd, const REALDATAMODULETYPE eRealDataModuleType = BASE_TYPE);
	virtual ~CRealDataModule();


// (2009/10/20 - Seung-Won, Bae) Comment.
protected:
											//	비율 계산하는 설정값이 1이거나 0인지 확인한다. (비율 계산의 의미가 없는 경우)
											//	비율 계산만을 위한 것인가? 그리고 첫 RDC와 ER의 설정만 확인하면 그 뒤는 무조건 무시?
	CRealDataModule::REALDATAMODULETYPE		GetMakeRealModuleType( const REALDATAMODULETYPE eRealDataModuleType, const CString& strData) const;







public:
	void SetRealDataModuleType( const REALDATAMODULETYPE eRealDataModuleType, const CString& strModuleHelpMsg);
	bool SetRealDataModuleType( const REALDATAMODULETYPE eRealDataModuleType);

	void ChangeExchangeRateData_Percent();
	bool ChangeRealDataModuleAndData(const CString& strModuleHelpMsg);

	double GetRealData(const CString& strPacketName, const double& dData) const;
	double GetRealData(const CString& strPacketName, const CString& strData) const;
	// 2008.10.24 JS.Kim	Packet의 데이타를 원 데이타로 복구
	double GetOrgData(const CString& strPacketName, const double& dData) const;
	
private:
	void MakeRealDataModule(const REALDATAMODULETYPE eRealDataModuleType);
	void DeleteRealDataModule();


private:
	CBaseRealDataModule *m_pRealDataModule;

// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
protected:
	HWND		m_hOcxWnd;
};


// real 관련 Module
class CBaseRealDataModule
{
public:
	virtual ~CBaseRealDataModule()	{};
public:
	virtual CRealDataModule::REALDATAMODULETYPE GetRealDataModuleType() const = 0;
	virtual double GetRealData(const CString& strPacketName, const double& dData) const = 0;
	// 2008.10.24 JS.Kim	Packet의 데이타를 원 데이타로 복구
	virtual double GetOrgData(const CString& strPacketName, const double& dData) const = 0;

	virtual void SetHelpMessage(const CString& strData, const bool bIsRemove = true) = 0;

	virtual void ChangeExchangeRateData_Percent() = 0;
};


// org real 관련 Module
class COrgRealDataModule : public CBaseRealDataModule
{
public:
	virtual CRealDataModule::REALDATAMODULETYPE GetRealDataModuleType() const;
	virtual double GetRealData(const CString& strPacketName, const double& dData) const;
	// 2008.10.24 JS.Kim	Packet의 데이타를 원 데이타로 복구
	virtual double GetOrgData(const CString& strPacketName, const double& dData) const;

	virtual void SetHelpMessage(const CString& strData, const bool bIsRemove = true);

	virtual void ChangeExchangeRateData_Percent();
};


// 비율 real 관련 Module
class CRateRealDataModule : public CBaseRealDataModule
{
public:
	CRateRealDataModule( HWND p_hOcxWnd);
	virtual ~CRateRealDataModule();

public:
	virtual CRealDataModule::REALDATAMODULETYPE GetRealDataModuleType() const;
	virtual double GetRealData(const CString& strPacketName, const double& dData) const;
	// 2008.10.24 JS.Kim	Packet의 데이타를 원 데이타로 복구
	virtual double GetOrgData(const CString& strPacketName, const double& dData) const;

	virtual void SetHelpMessage(const CString& strData, const bool bIsRemove = true);

	virtual void ChangeExchangeRateData_Percent();

private:
	void MakeRealRateData(const CString& strData);
	void DeleteAllRealRateData();

	// (2006/6/20 - Seung-Won, Bae) Kim Sun-Young Update! for Real data with RDC Help Message
	void SetRealRateData(const CString& strData);
	void SetRealRateData_PartChange(const CString& strData);

	bool DoesOnlyOriginalRealData(const CString& strPacketName) const;
	CString GetRealRateData(CString& strAllData, const CString& strCompart) const;

private:
	CList<CRealRateData*, CRealRateData*> m_realReteDataList;

// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
protected:
	HWND		m_hOcxWnd;
};

#endif // !defined(AFX_PACKETLISTMODULE_H__3D5B4B26_3ADC_43FB_81DF_D419F380923E__INCLUDED_)

