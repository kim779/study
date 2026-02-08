// DataOfChartListPage.h: interface for the CDataOfChartListPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAOFCHARTLISTPAGE_H__F60AEAB5_64E4_4A1F_B75E_9D019B0C53DC__INCLUDED_)
#define AFX_DATAOFCHARTLISTPAGE_H__F60AEAB5_64E4_4A1F_B75E_9D019B0C53DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"							// for Ixxxxxx.h

class CChartLink;
class CMyTreeCtrl;
class CGraphTypeInfo;
class CDataOfChartListPage  
{
public:
	CString GetBlockandGraphAllData(CChartLink* pRoot);
	void SetData(const CString& strChartListPageData, IGraphInfoManager *p_pIGraphInfoManager);

	CDataOfChartListPage(const int nRow, const int nColumn, CMyTreeCtrl* pTreeChart, HWND p_hOcxWnd);
	CDataOfChartListPage( HWND p_hOcxWnd);
	virtual ~CDataOfChartListPage();

private:
	CString GetGraphTypeWhenNoTypeInfo(const int nGraphType);
	CString GetGraphType(const CString& strGraphData, IGraphInfoManager *p_pIGraphInfoManager);
	int m_nColumn;
	int m_nRow;
	CMyTreeCtrl* m_pTreeChart;

	//Graph加己 Set 
	void SetItem(CChartLink* pVertiScale, const CString& data, IGraphInfoManager *p_pIGraphInfoManager);
	//vertical Scale 加己.
	CChartLink* SetVerticalScaleItem(const int nVerticalScale, CString strData);
	CChartLink* SetVerticalScaleItem_Temp(const int nVerticalScale, const CString strData);
	//Block加己 Set
	void SetBlockItem(const int nBlock, CString strData);
	void SetBlockItem_Temp(const int nBlock, CString strData);
	
	CString GetGraphAllData(CChartLink* pItem);
	CString GetVerticalAllData(CChartLink *pItem);
	CString GetBlockAllData(CChartLink* pItem);

//	bool IsCheckGraphType(const CString& strGraphType) const;

//	CString GetVertScaleGroup(CDataOfChartListPage::SCALEJOIN eScaleJoin, const bool bScaleJoin);

private:
	//sy 2006.04.17.
	void GetSignalData(const CString& strData, bool& bSignalUse, bool& bSignalShow);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

#endif // !defined(AFX_DATAOFCHARTLISTPAGE_H__F60AEAB5_64E4_4A1F_B75E_9D019B0C53DC__INCLUDED_)
