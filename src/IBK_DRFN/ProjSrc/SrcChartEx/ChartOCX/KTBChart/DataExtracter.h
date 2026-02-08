// DataExtracter.h: interface for the CDataExtracter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAEXTRACTER_H__5D112F44_F168_41C2_BFE5_B6E70F26D3E4__INCLUDED_)
#define AFX_DATAEXTRACTER_H__5D112F44_F168_41C2_BFE5_B6E70F26D3E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CPacketList;
class CMainBlock;
class CRowViewData;
class CSubGraphViewData;
class CDataExtracter  
{
public:
/*
	CString GetData(const int nStartIndex,
							 const int nEndIndex,
							 const int nDataStart,
							 const int nDataEnd,
							 CPacketList *pPacketList,
							 CMainBlock* pMainBlock,
							 int& nMaxLength);
*/

private:
	CString GetTitle(const CString& strSubGraphName,
					 CMainBlock* pMainBlock);
	CString GetSubGraphViewData(const CString &strSubGraphName,
								CSubGraphViewData* subgraphdata);
	CString GetRowViewData(const CString &strSubGraphName,
						   CRowViewData* rowdata);

	CString MakeDateString(const CString &strTitle, const CString& strValue);
	CString MakeString(const CString& strTitle,
					   const double dValue,
					   int& nMaxLength);

	void GetRealDataIndex(const int nStartIndex,
						  const int nEndIndex,
						  const int nDataStart,
						  const int nDataEnd,
						  int& nRealStartIndex,
						  int& nRealEndIndex);

	CList<double, double>* GetPacket(const CString& strSubGraphName,
									 CPacketList* pPacketList);

	POSITION FindPosition(const int nIndex,
						  const CList<double, double>& pSourceList) const;

	double GetHighestPrice(const int nStartIndex,
						const int nEndIndex,
						CPacketList *pPacketList);

	double GetLowestPrice(const int nStartIndex,
						  const int nEndIndex,
						  CPacketList *pPacketList);

	void GetHighestLowestVolume(const int nStartIndex,
									const int nEndIndex,
									CPacketList *pPacketList,
									double& nHighestVolume,
									double& nLowestVolume);

	double GetMA(const int nStartIndex,
				 const int nEndIndex,
				 CList<double, double>* pDataList);

//	CString GetDate(const int nIndex,
//				 CPacketList *pPacketList);
};

#endif // !defined(AFX_DATAEXTRACTER_H__5D112F44_F168_41C2_BFE5_B6E70F26D3E4__INCLUDED_)
