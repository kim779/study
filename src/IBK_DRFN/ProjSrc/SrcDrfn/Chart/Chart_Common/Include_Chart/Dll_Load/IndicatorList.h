// IndicatorList.h: interface for the CIndicatorList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORLIST_H__B19E87F5_4EF5_4C17_938D_7B243D75730F__INCLUDED_)
#define AFX_INDICATORLIST_H__B19E87F5_4EF5_4C17_938D_7B243D75730F__INCLUDED_

#include <afxtempl.h>
#include <list>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndicatorListImplementation;
class CIndicatorInfo;
class CIndicatorGroupDataList;

class AFX_EXT_CLASS CIndicatorList  
{
public:
	CIndicatorList(const CString& strMainDefaultFilePathAndName = "", const CString& strUserDefaultFilePathAndName = "", const CString& strUserFilePathAndName = "");
	virtual ~CIndicatorList();

	// user 설정 모든 지표를 file에서 읽어오기.
	bool ReadUserFile(const CString& strFilePathAndName);
	// user 설정 모든 지표를 file에 저장하기.
	bool WriteAllUserIndicatores(const CString& strUserFilePathAndName);
	// user Default Indicator Info를 모두 저장하기.
	bool WriteAllUserDefaultIndicatores(const CString& strFilePathAndName);

	// map default 삭제한다.
	bool DeleteAllMapDefaultIndicatores();
	// user 설정 모든 지표를 삭제한다.
	bool DeleteAllUserIndicatores();
	// user info를 삭제한다.
	bool DeleteUserIndicatorInfo(CIndicatorInfo* pIndicatorInfo);
	bool DeleteUserIndicatorInfo(const CString& strIndicatorName);


	bool SetIndicatorInfo(const CIndicatorInfo* pIndicatorInfo, const bool bIsAutoMake = false);


	CIndicatorList* GetIndicatorList() const;
	double GetDefaultVersion() const;
	double GetUserFileVersion() const;
	// user file이 cfg file과 같은 version 인지의 여부.
	bool IsDefaultUserVersion() const;

	// Graph Type/Style/DrawStyle 에 관련된 내용
	bool GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, const CString& strDrawStyle, 
		int& nType, int& nStyle, int& nDrawStyle) const;
	bool GetGraphTypeAndStyle(const CString& strType, const CString& strStyle, int& nType, int& nStyle) const;
	bool GetGraphDrawStyle(const CString& strName, const CString& strStyle, int& nDrawStyle) const;
	bool GetGraphTypeAndStyle(const int nType, const int nStyle, CString& strType, CString& strStyle) const;
	bool GetAllGraphTypeList(CList<CString, CString>& graphTypeList) const;
	CList<CString, CString>* GetAllGraphStyleList(const CString& strType) const;
	CList<CString, CString>* GetAllGraphDrawStyleList(const CString& strType) const;
	// IndicatorInfo 에 관련된 내용
	list<CIndicatorInfo*>* GetMainDefaultIndicatorList_General() const;
	list<CIndicatorInfo*>* GetMainDefaultIndicatorList_Special() const;
	bool GetMainDefaultIndicatorNameList_General(CList<CString, CString>& indicatorNameList) const;
	bool GetMainDefaultIndicatorNameList_Special(CList<CString, CString>& indicatorNameList) const;
	bool GetMainDefaultIndicatorNameList_None(CList<CString, CString>& indicatorNameList) const;
	CIndicatorInfo* GetIndicatorInfo(const CString& strIndicatorName) const;
	CIndicatorInfo* GetDefaultIndicatorInfo(const CString& strIndicatorName) const;
	CIndicatorInfo* GetMainDefaultIndicatorInfo(const CString& strIndicatorName) const;
	CIndicatorInfo* GetMapDefaultIndicatorInfo(const CString& strIndicatorName, const bool bIsAutoMake = false);
	CIndicatorInfo* GetUserIndicatorInfo(const CString& strIndicatorName, const bool bIsAutoMake = true);
	CString GetGraphDataForMaking(const CString& strIndicatorName) const;
	// Indicator의 Group에 관련된 내용
	CIndicatorGroupDataList* GetAllIndicatorNameInGroupName();

private:
	CIndicatorListImplementation* m_pIndicatorListImpl;
};

#endif // !defined(AFX_INDICATORLIST_H__B19E87F5_4EF5_4C17_938D_7B243D75730F__INCLUDED_)
