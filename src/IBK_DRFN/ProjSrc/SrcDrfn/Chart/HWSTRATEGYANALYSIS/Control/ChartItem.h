// ChartItem.h: interface for the CChartItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_)
#define AFX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ezpchart.h"

typedef struct ChartData
{
	char    date[8];			/* 날짜     		*/
	char    open[10];			/* 시가     		*/
	char    high[10];			/* 고가     		*/
	char    low[10];			/* 저가     		*/
	char    close[10];			/* 종가   			*/
	char    volume[10];     	/* 거래량   		*/
} ST_CHARTDATA;

typedef struct ChartHeader
{
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[90];    	/* Chart Bojo Data  */
} ST_CHARTHEADER;

class CChartItem : public CEzpChart
{
public:
	CChartItem();	
	BOOL Create(CWnd *pWnd,LPRECT lpRect, int nID);
	void SetpBaseDesk(long lKey,HWND hWndDll);
//	void SetData(CDataMng *pDataMng,BOOL bAll, BOOL bKospi, BOOL bKosdaq, BOOL bKospiIndex, BOOL bKosdaqIndex, CString strJong ="");
//	BOOL AddPacketAndGraph(BOOL bAll, BOOL bKospi, BOOL bKosdaq, BOOL bKospiIndex, BOOL bKosdaqIndex, CString strJong);
	CString MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal);
	CString MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle = 0,
									COLORREF clrGrp1 = RGB(218,46,46),COLORREF clrGrp2 = RGB(218,46,46),
									COLORREF clrGrp3 = RGB(46,46,218),COLORREF clrGrp4 = RGB(46,46,218),
									COLORREF clrGrp5 = RGB(218,46,46),COLORREF clrGrp6 = RGB(218,46,46),
									BOOL bUpFilled = 0, BOOL bDownFilled = 1,
									long lUpLineWidth = 1, long lDownLineWidth = 1, BOOL bHideGrp = 0);
	CString GetPacketName();
	long GetDataSize();
	BOOL AddPacketAndGraph();
	void AddRealItem(CString strCode);
	void RemoveRealItem();

private:
	BOOL SetAllProperties();
	long m_lKey;
	CStringArray m_strArrayPacketName;
	CStringArray m_strArrayGraphTitle;
	CStringArray m_strArrayRealItemRegistered;
	BOOL m_bInitalChart;
//	CHARTYIELDDATA m_ChartYieldData;
// Operations
public:

// Overrides
// Implementation
public:
	virtual ~CChartItem();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_CHARTITEM_H__11999AEF_8200_4DA5_B205_33C1402F54D5__INCLUDED_)
