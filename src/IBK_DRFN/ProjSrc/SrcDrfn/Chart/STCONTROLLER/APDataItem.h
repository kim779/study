// APDataItem.h: interface for the CAPDataItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APDATAITEM_H__909C4362_F0C1_47FC_AFBF_F70C62BCB2BE__INCLUDED_)
#define AFX_APDATAITEM_H__909C4362_F0C1_47FC_AFBF_F70C62BCB2BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/DataItem.h"
#include "../Common_ST/SDataItem.h"
#include "../Include_ST/CommentData.h"
#include "../../inc/ISTManager.h" // KHD시스템트레이딩 관련된 컨텐츠 인터페이스

//#include "../../COMMON_BASE/drds/DRDSDEF.h"
//#include "../../../DataMgr/DBMgr.h"

class CDrMLib_CRealData;
class CSTController;
class CApQurier;
class CCommentData;

class CAPDataItem  
{
public:
	CAPDataItem();
	virtual ~CAPDataItem();
	BOOL ReceiveAPData(ST_RECEIVEFROMSERVER* pStR);
	BOOL ReceiveAPData();//KHD
	CString GetDate(int nIndex);
	double GetOpenData(int nIndex);
	double GetHighData(int nIndex);
	double GetLowData(int nIndex);
	double GetCloseData(int nIndex);
	double GetVolumeData(int nIndex);	
	double GetAmountData(int nIndex);
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	double GetExtData(int nIndex);
//	double GetDayOpenData(int nIndex,int nCurrentPos);
//	double GetDayHighData(int nIndex,int nCurrentPos);
//	double GetDayLowData(int nIndex,int nCurrentPos);
//	double GetDayCloseData(int nIndex,int nCurrentPos);
	// (2006/1/10 - Seung-Won, Bae) Get the Data Count from the Close Data without virtual Data of Time Data.
	int  GetCntData() { return m_pDataItemClose->GetSize();} 
	CString GetCode() { return m_strCode; }
	//LONG OnDrdsData(UINT wParam, LPARAM lParam);
//	BOOL OnQuoteRealData(CRealData *pData);
//	BOOL OnFutureRealData(st_SC0* pStRealData);
//	BOOL OnOptionRealData(st_OC0* pStRealData);
	BOOL OnRealData(CDrMLib_CRealData *pData);

	CIDataItem* GetOpenData();
	CIDataItem* GetHighData();
	CIDataItem* GetLowData();
	CIDataItem* GetCloseData();
	CIDataItem* GetVolumeData();	
	CISDataItem* GetDateData();
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	CIDataItem* GetExtData();
	

	BOOL SetTempData();


	BOOL RequestDataToServer(CCommentData* pBasicalInputData, CSTController *pSTController,CApQurier* pApQurier);
	void AddReceiver(CSTController *pSTController);
	long RemoveReceiver(CSTController *pSTController);
	long GetVarInterval();
	char GetTimeType() { return m_chType;}
	CString GetTimeFormat() { return m_strTimeFormat; }
	void		SetTimeFormat(CString strTimeFormat){ m_strTimeFormat = strTimeFormat;	}//KHD: 추가 2006.12.12
	void SetDataItemSize(int nSize);
	//KHD
	void SetChartItem(long pChartItem){ m_pChartItem = pChartItem;}
	long GetChartItme()  {return m_pChartItem;}
	void SetHelpMessageData(CString stMessage);
	//
	double GetPrevOpenData(){return m_dPrevOpenData;}
	double GetPrevHighData(){return m_dPrevHighData;}
	double GetPrevLowData(){return m_dPrevLowData;}
	double GetPrevCloseData(){return m_dPrevCloseData;}

	//
private:
	BOOL SetRealData(LPCTSTR lpTime, double dClose, double dVolume);
	BOOL RemoveAllData();
	long GetHHMMSSInCeilingType(LPCTSTR lpOrgData);
	long GetHHMMInCeilingType(LPCTSTR lpOrgData);
	void SetTimeNext(LPCTSTR lpDateLast);
//	CString GetDayTimeData(int nIndex,int nCurrentPos);

	CApQurier* m_pApQurier;
	CSDataItem* m_pSdataItemDate;
	CSTDataItem* m_pDataItemOpen;
	CSTDataItem* m_pDataItemHigh;
	CSTDataItem* m_pDataItemLow;
	CSTDataItem* m_pDataItemClose;
	CSTDataItem* m_pDataItemVolume;
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	CSTDataItem* m_pDataItemExt;

	BOOL	m_bIsExternaRawData;
	BOOL	m_bIsMultiBong;

	CString m_strTimeFormat;
//	CString m_strDayTime;
//	double m_dDayOpen;
//	double m_dDayHigh;
//	double m_dDayLow;
//	double m_dDayClose;

	CString m_strBasicalData;
	CString m_strCode;
	
	char m_chType; // 틱'0'/분'1'/일'2'/주'3'/월'4'
	long m_lTickMinNumber;	//	
	long m_lCntOfDrdsForTickNumber;
	long m_lTimeNext;
	
	//KHD
	long m_pChartItem;
	double m_dPrevOpenData;
	double m_dPrevCloseData;
	double m_dPrevLowData;
	double m_dPrevHighData;

	//
	CList<CSTController*,CSTController*>m_listSTController;

};

#endif // !defined(AFX_APDATAITEM_H__909C4362_F0C1_47FC_AFBF_F70C62BCB2BE__INCLUDED_)
