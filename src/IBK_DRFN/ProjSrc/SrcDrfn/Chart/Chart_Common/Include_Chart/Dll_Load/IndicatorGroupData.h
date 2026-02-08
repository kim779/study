// IndicatorGroupData.h: interface for the CIndicatorGroupData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORGROUPDATA_H__836E6A48_60E7_4A77_AD3C_D1119C1EDE89__INCLUDED_)
#define AFX_INDICATORGROUPDATA_H__836E6A48_60E7_4A77_AD3C_D1119C1EDE89__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 해당 그룹과 그에 속하는 지표목록
class AFX_EXT_CLASS CIndicatorGroupData
{
public:
	// 생성&삭제
	static CIndicatorGroupData* Make(const CString& strGroupName = "");
	static void Delete(CIndicatorGroupData* pData);

public:
	CIndicatorGroupData& operator=(const CIndicatorGroupData& data);

public:
	CIndicatorGroupData(const CString& strGroupName = "");
	virtual ~CIndicatorGroupData();

	void Initialize();
	void RemoveAll();
	void AddTail(const CString& strIndicatorName);

	void SetGroupName(const CString& strGroupName);

	CString GetGroupName() const;
	int GetIndicatorNameCount() const;
	const CList<CString, CString>* GetIndicatorNameList() const;
	CList<CString, CString>* GetIndicatorNameList();

private:
	void CopySubGraphDataList(const CList<CString, CString>& strSourceIndiNameList);

private:
	CString m_strGroupName;
	CList<CString, CString> m_strIndicatorNameList;
};


class AFX_EXT_CLASS CIndicatorGroupDataList
{
public:
	virtual ~CIndicatorGroupDataList();

	void DeleteAll();
	void AddTail(CIndicatorGroupData* pIndiGroupData);
	bool AddTail_IndiName(const CString& strGroupName, const CString& strIndiName, const bool bIsAutoMake = true);

	int GetCount() const;
	POSITION GetHeadPosition() const;
	POSITION GetHeadPosition();
	POSITION GetTailPosition() const;
	POSITION GetTailPosition();
	const CIndicatorGroupData* GetPrev(POSITION& pos) const;
	CIndicatorGroupData* GetPrev(POSITION& pos);
	const CIndicatorGroupData* GetNext(POSITION& pos) const;
	CIndicatorGroupData* GetNext(POSITION& pos);

	const CList<CIndicatorGroupData*, CIndicatorGroupData*>* GetIndicatorGroupDataList() const;
	CList<CIndicatorGroupData*, CIndicatorGroupData*>* GetIndicatorGroupDataList();

	CIndicatorGroupData* FindIndicatorGroupData(const CString& strIndiGroupName);

private:
	CList<CIndicatorGroupData*, CIndicatorGroupData*> m_indicatorGroupDataList;
};

#endif // !defined(AFX_INDICATORGROUPDATA_H__836E6A48_60E7_4A77_AD3C_D1119C1EDE89__INCLUDED_)
