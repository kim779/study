// GraphTypeData.h: interface for the CGraphTypeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHTYPEDATA_H__BE46D27D_252D_4DEA_8F0D_FA3E1C48A954__INCLUDED_)
#define AFX_GRAPHTYPEDATA_H__BE46D27D_252D_4DEA_8F0D_FA3E1C48A954__INCLUDED_

#include "IndicatorConditionDefineData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// graph tyle 이나 drawStyle tyle 관련 부분
class CGraphTypeData  
{
public:
	static CGraphTypeData* Make();
	static void Delete(CGraphTypeData* pGraphTypeData);

public:
	CGraphTypeData();
	virtual ~CGraphTypeData();

	CString		GetName() const;
	void		SetName(const CString& strName);

	int			GetStyleCount() const;
	CString		GetStyle(const int nIndex) const;

private:
	CString			m_strName;
	CStringDataList	m_styleDataList;

// (2006/11/2 - Seung-Won, Bae) Encapsulate for Old Price Chart Type Manage
friend class CGraphTypeOrStyleDataBuilder;
friend class CGraphTypeDataList;
protected:
	CStringDataList *	GetStyleDataList();
	const				CStringDataList* GetStyleDataList() const;
	int					GetStyleIndex(const CString& strStyle) const;
};

class CGraphTypeDataList
{
public:
	virtual ~CGraphTypeDataList();

	void AddTail(CGraphTypeData* pGraphTypeData);
	void DeleteAll();

	bool IsEmpty() const;
	int GetCount() const;
	CGraphTypeData* GetGraphTypeData(const CString& strName) const;
	bool GetDataIndex(const CString& strName, const CString& strStyle, 
		int& nTypeIndex, int& nStyleIndex) const;
	int GetStyleDataIndex(const CString& strStyle) const;
	bool GetData(const int nTypeIndex, const int nStyleIndex, 
		CString& strName, CString& strStyle) const;
	bool GetAllNamesList(CStringDataList& namesList) const;
	CStringDataList* GetAllStyleList(const CString& strType) const;

private:
	bool IsExisting(const int nIndex) const;

private:
	CGraphTypeDataDeque m_graphTypeDataList;
};


class AFX_EXT_CLASS CGraphTypeAndStyleData
{
public:
	enum DELETETYPE
	{
		ALL = 0,
		TYPE,
		STYLE
	};

	enum STYLETYPE
	{
		TYPE_STYLE = 0,
		DRAW_STYLE
	};

public:
	virtual ~CGraphTypeAndStyleData();

	bool FileDataBuild( char *&p_szDataString);

	void DeleteAll(const CGraphTypeAndStyleData::DELETETYPE eDeleteType);

	CGraphTypeDataList* GetGraphTypeList();
	CGraphTypeDataList* GetStyleTypeList();

	bool IsEmpty() const;
	bool GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, 
		const CString& strDrawStyleName, const CString& strDrawStyle, 
		int& nType, int& nStyle, int& nDrawStyle) const;
	bool GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, const CString& strDrawStyle, 
		int& nType, int& nStyle, int& nDrawStyle) const;
	bool GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, int& nType, int& nStyle) const;
	bool GetGraphTypeAndStyle(const int nType, const int nStyle, CString& strType, CString& strStyle) const;
	bool GetGraphDrawStyle(const CString& strName, const CString& strStyle, int& nDrawStyle) const;
	bool GetAllGraphTypeList(CStringDataList& graphTypeList) const;
	CStringDataList* GetAllGraphStyleList(const CGraphTypeAndStyleData::STYLETYPE eStyleType, const CString& strType) const;

private:
	CGraphTypeDataList m_graphTypeList;
	CGraphTypeDataList m_graphStyleList;
};

#endif // !defined(AFX_GRAPHTYPEDATA_H__BE46D27D_252D_4DEA_8F0D_FA3E1C48A954__INCLUDED_)
