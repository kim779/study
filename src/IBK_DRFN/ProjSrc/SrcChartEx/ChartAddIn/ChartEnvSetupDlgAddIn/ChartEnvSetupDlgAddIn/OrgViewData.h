// OrgViewData.h: interface for the COrgViewData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORGVIEWDATA_H__DCBD417D_6148_4171_B09C_2D25FB7736A2__INCLUDED_)
#define AFX_ORGVIEWDATA_H__DCBD417D_6148_4171_B09C_2D25FB7736A2__INCLUDED_

#include "Include_AddIn.h"		// for IGraphInfo

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiInfoData
{
public:
	static CIndiInfoData* Make(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning);
	static void Delete(CIndiInfoData* pIndiInfoData);

public:
	CIndiInfoData(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning = false);
	virtual ~CIndiInfoData();

	void SetTheBeginning(const bool bTheBeginning);
	void SetIndicatorInfo(IGraphInfo* p_pIGraphInfo);

	bool IsTheBeginning() const;
	IGraphInfo& GetIndicatorInfo();
	IGraphInfo* GetIGraphInfo();

	CString GetGraphName() const;
	CString GetIndicatorName() const;

private:
	bool m_bTheBeginning; // 처음 화면부터 존재 여부
	IGraphInfo *m_pIGraphInfo;
};


class CIndiInfoDataList
{
public:
	CIndiInfoDataList();
	virtual ~CIndiInfoDataList();

	bool AddTail(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning);
	void DeleteAll();

	//멤버로 있는 IGraphInfo 값을 pTargetIndiList에 적용하기.
	bool ChangeAllIndicatorList( IGraphInfoManager* p_pIGraphInfoManager);

	POSITION GetHeadPosition() const;
	POSITION GetHeadPosition();
	POSITION GetTailPosition() const;
	POSITION GetTailPosition();
	CIndiInfoData* GetPrev(POSITION& pos) const;
	CIndiInfoData* GetPrev(POSITION& pos);
	CIndiInfoData* GetNext(POSITION& pos) const;
	CIndiInfoData* GetNext(POSITION& pos);
	CIndiInfoData* GetIndiInfoData(const CString& strIndicatorName);
	
	//처음 화면에 road 되어 있는 IndiInfo 값만 가져오기.
	bool GetIndiInfoInTheBeginning(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const;
	bool GetIndiInfoIntheSave(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const;
	
	bool IsIndiInfoData( const char *p_szGraphName) const;

private:
	CList<CIndiInfoData*, CIndiInfoData*> m_indiInfoDataList;
};

class CGraphEtcData
{
public:
	CGraphEtcData();
	CGraphEtcData(const CString& strFunctionName, const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType);

	void SetFunctionName(const CString& strFunctionName);
	void SetVertScaleLogType(const bool bIsVertScaleLogType);
	void SetVertScaleInvertType(const bool bIsVertScaleInvertType);

	CString GetFunctionName() const;
	bool IsVertScaleLogType() const;
	bool IsVertScaleInvertType() const;

private:
	CString m_strFunctionName;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
};



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Explain		: 
// Using way	: 초기 IndicatorInfo를 저장한다.
//-----------------------------------------------------------------------------
class COrgViewData  
{
public:
	COrgViewData();
	virtual ~COrgViewData();

	// 멤버로 있는 IGraphInfo 값을 IGraphInfoManager에 적용하기.
	bool ChangeAllIndicatorList( IGraphInfoManager* p_pIGraphInfoManager);
	
	// 처음 화면에 road 되어 있는 IndiInfo 값만 가져오기.
	bool GetIndiInfoInTheBeginning( CList< IGraphInfo *, IGraphInfo * >&indiInfoList ) const;
	bool GetIndiInfoIntheSave( CList< IGraphInfo *, IGraphInfo * >&indiInfoList ) const;
	
	bool IsIndicatorInfo( IGraphInfo* p_pIGraphInfo);				// IndicatorInfo가 있는지 확인한다.

	CGraphEtcData& GetGraphEtcData();
	CIndiInfoDataList& GetIndiInfoDataList();

	bool AddTail( IGraphInfo *p_pIGraphInfo, const bool bTheBeginning = false);
	void Initialize();

private:
	CIndiInfoDataList m_indiInfoDataList;
	CGraphEtcData m_graphEtcData;
	
	//bool m_bAllHiding_PriceMA;
};

#endif // !defined(AFX_ORGVIEWDATA_H__DCBD417D_6148_4171_B09C_2D25FB7736A2__INCLUDED_)
