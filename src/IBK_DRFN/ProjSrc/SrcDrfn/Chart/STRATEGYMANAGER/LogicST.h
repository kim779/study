// LogicST.h: interface for the CLogicST class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICST_H__D73A1CD7_08A4_4E28_B5E2_25713B2E8361__INCLUDED_)
#define AFX_LOGICST_H__D73A1CD7_08A4_4E28_B5E2_25713B2E8361__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/DllSTLoader.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/DealData.h"

#include "LogicItem.h"
#include <afxtempl.h>

class CDealDataItem;

// 20060705 클래스선언 제거 : class CDBMgr;
//class CDBMgr;

class CLogicST  : public CLogicItem
{
public:
	CLogicST();
	virtual ~CLogicST();
	virtual long Run(LPCTSTR lpDataCode, long lDataCnt, LPCTSTR lpEndDate, LPCTSTR lpOption);
	virtual BOOL Stop();
	virtual BOOL Pause();
	virtual long Update(long lPos, BOOL bIsUpdateData);	//@060922전략Real관련수정
	virtual BOOL IsFilled();
	virtual void InitFilledSet();
	virtual CISDataItem *GetDateItem() { return m_pIDateItem;}
	virtual CPtrArray* GetPArrayHistoryData();
	virtual CDealData* GetValue(LPCTSTR lpTime, char chType, LPCTSTR lpCurrentTime,int nPos, LPCTSTR lpCurrentTimeType);
	virtual long GetCount() {return 1;}
	virtual CLogicST *GetLogicST(long lIndex);
 	virtual CString GetTime() { return m_strTimeTypeNum;}
	virtual long GetTimeTypeNum();

// 20060705 함수인자 제거 : ,HWND hSocket/*,long pDBMgr*/
//	BOOL Init(LPCTSTR lpSDNamePath, long lPos, int nVlaue, LPCTSTR lpTime, LPCTSTR lpSTFilePath,HWND hParent, HWND hSocket/*,long pDBMgr*/);
	BOOL Init(LPCTSTR lpSDNamePath, long lPos, int nVlaue, LPCTSTR lpTime, LPCTSTR lpSTFilePath,HWND hParent,long pChartItem=0);

	void ReceiveFromSTDataManager(LPARAM lParam);
	long GetDataCount();
	long GetPtrST();
	CDealData* GetDataValue(long lIndex);
	CString GetDateValue(long lIndex);
	CString GetInputs(long lIndex);
	CString GetPath();
	void SetPosition(POSITION pos){
		m_posPassedDeal = pos;
	}
	
private:
	CArray <CIDataItem*,CIDataItem*> m_pArrayIDataItem;
	CDealDataItem* m_pDealDataItem;
	CISDataItem* m_pIDateItem;
	BOOL m_bInitDataItem;
//	CDllSTLoader m_loaderOfSTLoader; //SD 전략 파일을 로드한 로드 포인터
	CDllSTLoader* m_pLoaderOfSTLoader; //SD 전략 파일을 로드한 로드 포인터
	CString m_strSDName;
	CString m_strSTFilePath;
	BOOL m_bTrue;	// true or not
	long m_lPos;	// position In STFile
	long m_lType;	// User or System
	CString m_strTimeTypeNum;
	char m_chTimeType;
	CString m_strTimeNum;
	BOOL m_bIsFilled;


	double m_dStartTimeInLastPeriod;
	double m_dEndTimeInLastPeriod;
	POSITION m_posStartDealInLastPeriod;
	POSITION m_posEndDealInLastPeriod;
	CDealData m_DealData;
	POSITION m_posPassedDeal;
	ST_DRAWINFO* m_pDrawInfo;
	
public:
	// 이전, 이내, 연속 체크를 위한 배열
	CArray<double, double> m_arrayDeal_Src;
	CArray<double, double> m_arrayDeal_Des;
	int			m_nUseExt;
	int			m_nType;
	int			m_nBongCount;

	void		SetConfig(int nUseExt, int nType, int nBongCount);
	void		CheckStrategySignal();
	int			GetStrategySignal();
	void		SetStrategySignal(int nSignalCount);
	
	void		UnloadSTLoader();
};

#endif // !defined(AFX_LOGICST_H__D73A1CD7_08A4_4E28_B5E2_25713B2E8361__INCLUDED_)
