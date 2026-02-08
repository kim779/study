// ScaleDataList.h: interface for the CScaleDataList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEDATALIST_H__7ADDDF88_70C2_4371_B1CB_06E8EA828792__INCLUDED_)
#define AFX_SCALEDATALIST_H__7ADDDF88_70C2_4371_B1CB_06E8EA828792__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ScaleBaseData.h"


// 삽입할 Block의 horzScale
class CInsertingBlockHorzScaleData
{
public:
	CInsertingBlockHorzScaleData& operator=(const CInsertingBlockHorzScaleData& data);

public:
	CInsertingBlockHorzScaleData();
	CInsertingBlockHorzScaleData(const bool bOnlyBottomBlockShow, const CHorizontalScale& horzScale);

	bool IsOnlyBottomBlockShow() const;
	CHorizontalScale GetHorizontalScale() const;
	CHorizontalScale& GetHorizontalScale();

	void SetOnlyBottomBlockShow(const bool bOnlyBottomBlockShow);
	void SetHorizontalScale(const CHorizontalScale& horzScale);

private:
	bool m_bOnlyBottomBlockShow; // 가장 하단에 있는 block만 가로 Scale 보이기 유무
	CHorizontalScale m_horzScale;
};


// 이동할 graph의 vertScale
class CMovingGraphVertScaleData
{
public:
	CMovingGraphVertScaleData();
	CMovingGraphVertScaleData(const int nVertScaleGroupIndex, const CVerticalScale& verticalScale);

	int GetVertScaleGroupIndex() const;
	CVerticalScale GetVerticalScale() const;

	void SetVertScaleGroupIndex(const int nVertScaleGroupIndex);
	void SetVerticalScale(const CVerticalScale& verticalScale);

private:
	int m_nVertScaleGroupIndex;
	CVerticalScale m_vertScale;
};


class CVerticalScaleGroupData
{
public:
	CVerticalScaleGroupData& operator=(const CVerticalScaleGroupData& vertScaleGroupData);

public:
	CVerticalScaleGroupData();
	CVerticalScaleGroupData(const CVerticalScale& vertScale, const CString& vertScaleTitle);

	CVerticalScale& GetVertScale();
	CVerticalScale GetVertScale() const;
	CString GetVertScaleTitle() const;

	void SetVertScale(const CVerticalScale& vertScale);
	void SetVertScaleTitle(const CString& vertScaleTitle);

private:
	CVerticalScale m_vertScale;
	CString m_strVertScaleTitle;
};


class CVerticalScaleGroupDataList
{
public:
	CVerticalScaleGroupDataList& operator=(const CVerticalScaleGroupDataList& vertScaleGroupDataList);

public:
	CVerticalScaleGroupDataList();
	CVerticalScaleGroupDataList(const CVerticalScaleGroupDataList& vertScaleGroupDataList);

	void AddTail(CVerticalScaleGroupData vertScaleGroupData);
	void RemoveAll();

	void SetFirstIndex(const int nFirstIndex);

	int GetFirstIndex() const;
	int GetCount() const;
	CVerticalScaleGroupData& GetAt(const int nIndex);
	CVerticalScaleGroupData GetAt(const int nIndex) const;

private:
	int m_nFirstIndex;
	CList<CVerticalScaleGroupData, CVerticalScaleGroupData&> m_vertScaleGroupDataList;
};


// ----------------------------------------------------------------------------
// 가로 scale의 data
class CHorizontalScalePtrData
{
public:
	CHorizontalScalePtrData& operator=(const CHorizontalScalePtrData& horzScalePtrData);

public:
	CHorizontalScalePtrData();
	CHorizontalScalePtrData(CHorizontalScale* pHorzScale, const CString& horzScaleTitle);

	CHorizontalScale* GetHorzScale();
	CHorizontalScale* GetHorzScale() const;
	CString GetHorzScaleTitle() const;

private:
	CHorizontalScale* m_pHorzScale;
	CString m_strHorzScaleTitle;
};


class CHorizontalScalePtrDataList
{
public:
	CHorizontalScalePtrDataList& operator=(const CHorizontalScalePtrDataList& horzScalePtrDataList);

public:
	CHorizontalScalePtrDataList();
	CHorizontalScalePtrDataList(const CHorizontalScalePtrDataList& horzScalePtrDataList);

	void AddTail(CHorizontalScalePtrData horzScalePtrData);
	void RemoveAll();

	void SetFirstIndex(const int nFirstIndex);
	void SetMarginCount(const int nMarginCount);
	void SetScaleColor(COLORREF* pGridColor, COLORREF* pTextColor);
	void SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor);
	void SetHorzScale(const int nIndex, const CHorizontalScale& horzScale);

	int GetFirstIndex() const;
	int GetMarginCount() const;
	COLORREF* GetScaleGridColor() const;
	COLORREF* GetScaleTextColor() const;
	int GetCount() const;
	CHorizontalScalePtrData& GetAt(const int nIndex);
	CHorizontalScalePtrData GetAt(const int nIndex) const;

private:
	int m_nFirstIndex;
	int m_nMarginCount;
	COLORREF* m_pScaleGridColor;
	COLORREF* m_pScaleTextColor;
	CList<CHorizontalScalePtrData, CHorizontalScalePtrData&> m_horzScalePtrDataList;
};


class CHorizontalScaleDataList
{
public:
	CHorizontalScaleDataList& operator=(const CHorizontalScaleDataList& horzScaleDataList);

public:
	CHorizontalScaleDataList();
	CHorizontalScaleDataList(const CHorizontalScaleDataList& horzScaleDataList);
	CHorizontalScaleDataList(const CHorizontalScalePtrDataList* pHorzScalePtrDataList);

	void AddTail(CHorizontalScale horzScale);
	void RemoveAll();

	COLORREF GetScaleGridColor() const;
	COLORREF GetScaleTextColor() const;
	int GetCount() const;
	CHorizontalScale& GetAt(const int nIndex);
	CHorizontalScale GetAt(const int nIndex) const;

private:
	COLORREF m_scaleGridColor;
	COLORREF m_scaleTextColor;
	CList<CHorizontalScale, CHorizontalScale&> m_horzScaleList;
};


// ----------------------------------------------------------------------------
// 세로 scale의 data
class CVerticalScalePtrData
{
public:
	CVerticalScalePtrData& operator=(const CVerticalScalePtrData& vertScalePtrData);

public:
	CVerticalScalePtrData();
	CVerticalScalePtrData(CVerticalScale* pVertScale, const CString& vertScaleTitle);

	CVerticalScale* GetVertScale();
	CVerticalScale* GetVertScale() const;
	CString GetVertScaleTitle() const;

private:
	CVerticalScale* m_pVertScale;
	CString m_strVertScaleTitle;
};


class CVerticalScalePtrDataList
{
public:
	CVerticalScalePtrDataList& operator=(const CVerticalScalePtrDataList& vertScalePtrDataList);

public:
	CVerticalScalePtrDataList();
	CVerticalScalePtrDataList(const CVerticalScalePtrDataList& vertScalePtrDataList);

	void AddTail(CVerticalScalePtrData vertScalePtrData);
	void RemoveAll();

	void SetFirstIndex(const int nFirstIndex);
	void SetScaleColor(COLORREF* pGridColor, COLORREF* pTextColor);
	void SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor);
	void SetEnableLogType(const bool bIsEnable);
	void SetEnableInvertType(const bool bIsEnable);
	void SetScaleType(bool* pIsLog, bool* pIsInvert);
	void SetScaleType(const bool bIsLog, const bool bIsInvert);
	void SetVertScale(const int nIndex, const CVerticalScale& vertScale);

	int GetFirstIndex() const;
	COLORREF* GetScaleGridColor() const;
	COLORREF* GetScaleTextColor() const;
	bool IsEnableLogType() const;
	bool IsEnableInvertType() const;
	bool* GetScaleLogType() const;
	bool* GetScaleInvertType() const;
	int GetCount() const;
	CVerticalScalePtrData& GetAt(const int nIndex);
	CVerticalScalePtrData GetAt(const int nIndex) const;

private:
	int m_nFirstIndex;
	COLORREF* m_pScaleGridColor;
	COLORREF* m_pScaleTextColor;
	bool m_bEnableLogType;
	bool m_bEnableInvertType;
	bool* m_pbLogType;
	bool* m_pbInvertType;
	CList<CVerticalScalePtrData, CVerticalScalePtrData&> m_vertScalePtrDataList;
};


class CVerticalScaleDataList
{
public:
	CVerticalScaleDataList& operator=(const CVerticalScaleDataList& vertScaleDataList);

public:
	CVerticalScaleDataList();
	CVerticalScaleDataList(const CVerticalScaleDataList& vertScaleDataList);
	CVerticalScaleDataList(const CVerticalScalePtrDataList* pVertScalePtrDataList);

	void AddTail(CVerticalScale vertScale);
	void RemoveAll();

	COLORREF GetScaleGridColor() const;
	COLORREF GetScaleTextColor() const;
	bool IsScaleLogType() const;
	bool IsScaleInvertType() const;
	int GetCount() const;
	CVerticalScale& GetAt(const int nIndex);
	CVerticalScale GetAt(const int nIndex) const;

private:
	COLORREF m_scaleGridColor;
	COLORREF m_scaleTextColor;
	bool m_bLogType;
	bool m_bInvertType;
	CList<CVerticalScale, CVerticalScale&> m_vertScaleList;
};

#endif // !defined(AFX_SCALEDATALIST_H__7ADDDF88_70C2_4371_B1CB_06E8EA828792__INCLUDED_)
