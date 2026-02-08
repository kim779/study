// GetIndicatorInfo.h: interface for the CGetIndicatorInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETINDICATORINFO_H__8AE160FC_10B9_4EEA_A5DB_53F7D66550DE__INCLUDED_)
#define AFX_GETINDICATORINFO_H__8AE160FC_10B9_4EEA_A5DB_53F7D66550DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "CalculateData.h"

class CChartFormulateData;
class CIndicatorInfo;
class CIndicatorList;
class CGetIndicatorInfo  
{
public:
//	static void GetIndicatorInfoFromIndicatorConditionDLL(const CString& strPacketName, 
//														  const CString& strFuncName, 
//														  CIndicatorList* pIndicatorList, 
//														  CList<double, double>& ConditionList, 
//														  CList<CString, CString>& PacketNameList, 
//														  CList<CString, CString>& SubGraphNameList);

	//매매레포트, 시스템트레이더에서 SUbGraphName만 가져가야 하는 때가 있음.
//	static void GetSubGraphNameList(const CString& strGraphName, CIndicatorList* pIndicatorList, CList<CString, CString>& subGraphNameList);
	static int GetSubGraphIndex(CIndicatorInfo* pIndicatorinfo, const CString& subgraphname);


private:
	//strOriginal의 맨마지막 부분에 strCompare가 있는지 체크하고, 없으면 strOriginal에 strCompare를 Add.	
	static void AddCompareStringInOriginalString(CString& strOriginal, const CString& strCompare);
	static void GetSubGraphNameList(CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& subGraphNameList);
	
	static void ChangeTypeFromCStringToCList(const CString& strPacketName, CList<CString, CString>& PacketNameList);
	static void GetConditionList(CIndicatorInfo* pIndicatorInfo, CList<double, double>& ConditionList);
	static void GetPacketNameList(CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& PacketNameList);


};

#endif // !defined(AFX_GETINDICATORINFO_H__8AE160FC_10B9_4EEA_A5DB_53F7D66550DE__INCLUDED_)
