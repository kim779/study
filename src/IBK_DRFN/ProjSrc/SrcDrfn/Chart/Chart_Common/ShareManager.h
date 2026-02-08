// ShareManager.h: interface for the CShareManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREMANAGER_H__0CE61CA3_A08A_443D_BC3D_8A56C09CFFB2__INCLUDED_)
#define AFX_SHAREMANAGER_H__0CE61CA3_A08A_443D_BC3D_8A56C09CFFB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 공유 관련 추가
//#include "../../Common_Base/BaseInfo/baseinfo.h"
//#include "../../../DrMLib/DrMiddleLib.h"
class CSharedBase;
// end

// 공유 관련 정의 패킷 포함 
// {{
//#include "../../INCLUDE_BASE/RealSignalPacket.h"
// --------------------------------------------------------------------------------------
//#define SHARE_SIG_SAS_NORMAL	"SAS"			// System Alert		Signal - Normal
//#define SHARE_SIG_SAS_ALARM	"SAS_ALARM"		// System Alert		Signal - Alarm
//#define SHARE_SIG_SAS_TICKER	"SAS_TICKER"	// System Alert		Signal - Ticker
//#define SHARE_SIG_SAS_ORDER	"SAS_ORDER"		// System Alert		Signal - Order
// --------------------------------------------------------------------------------------
//#define SHARE_SIG_NKS_NORMAL	"NKS"			// News	  Keyword	Signal - Normal
//#define SHARE_SIG_NKS_ALARM	"NKS_ALARM"		// News	  Keyword	Signal - Alarm
//#define SHARE_SIG_NKS_TICKER	"NKS_TICKER"	// News	  Keyword	Signal - Ticker
//#define SHARE_SIG_NKS_ORDER	"NKS_ORDER"		// News	  Keyword	Signal - Order
// --------------------------------------------------------------------------------------
//#define SHARE_SIG_UAS_NORMAL	"UAS"			// User   Alert		Signal - Normal
//#define SHARE_SIG_UAS_ALARM	"UAS_ALARM"		// User   Alert		Signal - Alarm
//#define SHARE_SIG_UAS_TICKER	"UAS_TICKER"	// User   Alert		Signal - Ticker
//#define SHARE_SIG_UAS_ORDER	"UAS_ORDER"		// User   Alert		Signal - Order
// --------------------------------------------------------------------------------------
// }}

#define SHARE_ACCT_CHANGE		"계좌변경"

class CChartItem;
class CShareManager  :  public CSharedBase 
{
public:
	CShareManager();
	virtual ~CShareManager();


	//CBaseInfo			 m_BaseInfo;
	CDrMLib_CBaseInfo	 m_BaseInfo;

	CChartItem*		 m_pSignalOrderMain;



	void					InitBaseInfo(void* pDBMgr); 
	void					DestroyBaseInfo();
	CDrMLib_CBaseInfo*		GetBaseInfoPtr();
	
	
//#ifdef __INFO
//	void			SetParent(CSignalOrderMain_Eagle* pParent) { m_pSignalOrderMain = pParent; };
//#else
//	void			SetParent(CSignalOrderMain* pParent) { m_pSignalOrderMain = pParent; };
//#endif

	void			SetParent(CChartItem* pParent) {	m_pSignalOrderMain = pParent; }
	
	BOOL 			SetHandleInfoToShareBase();


	// 공유 등록 
	// Advise
	BOOL			AdviseSharedInfo();
	BOOL			UnAdviseSharedInfo();
	// Set
	BOOL			SetSharedData2(	CString sShareName, BYTE* pData, int nLen );

	// virtual class
	// Get / Change
	virtual	long	ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock=0);
	virtual long	GetSharedData(CString sShareName, BYTE* pData, int& nLen, int& nStock);	

	
	

};

#endif // !defined(AFX_SHAREMANAGER_H__0CE61CA3_A08A_443D_BC3D_8A56C09CFFB2__INCLUDED_)
