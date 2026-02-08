// IChartLogAddIn.h: interface for the IChartLogAddIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTLOGADDIN_H__AC809E8D_5E59_4206_AB03_DBAEB215B8FA__INCLUDED_)
#define AFX_ICHARTLOGADDIN_H__AC809E8D_5E59_4206_AB03_DBAEB215B8FA__INCLUDED_

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/8
// Comments		: Debug AddIn Custom Interface
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartLogAddIn : public IUnknown
{
protected:
	virtual ~IChartLogAddIn()	{}

// (2005/8/16 - Seung-Won, Bae) Real Advise/Unadivse Info Logging Interface
public:
	// (2005/11/21 - Seung-Won, Bae) General Interface
	virtual void	OnDrdsAdvise( long p_lRequestID, const char *p_szPacket, const char *p_szField, const char *p_szAdviseKey) = 0;
	virtual void	OffDrdsAdvise( long p_lRequestID, const char *p_szPacket, const char *p_szField, const char *p_szAdviseKey) = 0;
	// in Hanwha
	virtual void	AddRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;
	virtual void	ResetRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;

// (2005/9/29 - Seung-Won, Bae) Log the New Real Setting
public:
	virtual void	OnRealSettingChanged( const char *p_szRealSetting) = 0;

// (2005/11/7 - Seung-Won, Bae) Normalized Minute Time Rule Reset
public:
	virtual void	OnNormalizedMinuteTimeRuleReset( BOOL p_bNormalizedMinuteTimeRule) = 0;

// (2005/11/18 - Seung-Won, Bae) Log the GetAllProperties2
public:
	virtual void	OnGetAllProperties2( CStringList *p_pProperties) = 0;

// (2005/11/7 - Seung-Won, Bae) Write Log Temporary Message.
public:
	virtual void	WriteLogMessage( const char *p_szMsg) = 0;
};

#endif // !defined(AFX_ICHARTLOGADDIN_H__AC809E8D_5E59_4206_AB03_DBAEB215B8FA__INCLUDED_)
