// ChartDataSimulatorAddInImp.h: interface for the CChartDataSimulatorAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTDATASIMULATORRADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_)
#define AFX_ChartDataSimulatorADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin_134221/I134221/_ChartAddInBase.h"	// for CChartAddInBase

/////////////////////////////////////////////////////
//일 , 주, 월, 분, 틱
#define	DAY_TYPE			0
#define	WEEK_TYPE			1
#define	MONTH_TYPE			2
#define	MIN_TYPE			3
#define	TICK_TYPE			4


/////////////////////////////////////////////////////
#define	DATE_TIME			0
#define	OPEN				1
#define	HIGH				2
#define	LOW					3
#define	CLOSE				4
#define	VOLLUME				5

#define	COL_SIZE			6

class CChartRecordSet
{
public:
	
	CString		m_Data[COL_SIZE];	

public:

	CChartRecordSet & operator=(CChartRecordSet & Record)
	{
		for(int i = 0 ; i < COL_SIZE ; i++)
			m_Data[i] = Record.m_Data[i];
		return *this;
	}
};


typedef CList < CChartRecordSet * , CChartRecordSet * > CChartDataList;

class CChartDataGridHeaderInfo
{
public:
	CString		m_strHeader;
	CString		m_strTitle;
	CString		m_strPacketType;
};
typedef CArray < CChartDataGridHeaderInfo * , CChartDataGridHeaderInfo * > CChartDataGridHeaderInfoArray;

class CChartDataGridManager
{
public:

	CString							m_strItemCode;//종목코드

	CString							m_strItemName;//종목명

	CString							m_strDateTime;//(EX:DDHHMMSS 등)

	int								m_nDataType;//일(DAY_TYPE:0), 주(WEEK_TYPE:1), 월(MONTH_TYPE:2) 등등	

	int								m_nDataInterval;//주기

	CString							m_strHelpMessage;//보조메세지

	CChartDataGridHeaderInfoArray	m_ChartDataGridHeaderInfoArray;

	CChartDataList					m_ChartDataList;	

public:

	CChartDataGridManager()
	{
		m_nDataType = DAY_TYPE;
		m_nDataInterval = 1;
	}

	~CChartDataGridManager()
	{
		for(int i = 0 ; i < m_ChartDataGridHeaderInfoArray.GetSize(); i++) delete m_ChartDataGridHeaderInfoArray.GetAt(i);
		while(!m_ChartDataList.IsEmpty()) delete m_ChartDataList.RemoveHead();
	}
};

//class CDlgDataListViewer;
class CChartDataSimulatorAddInImp : public CChartAddInBase  
{
public:
	CChartDataSimulatorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartDataSimulatorAddInImp();

	// Declare AddIn Event Map
//	DECLARE_ADDIN_EVENT_MAP()
protected:																										
	static const EAddInEventType	m_eRegisteredEventMap[ EAE_END + 1];										
protected:																										
	virtual const EAddInEventType *	GetRegisteredEventMap( void) const	{	return m_eRegisteredEventMap;	}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	BOOL				m_bUpdateRealData;
	CString				m_strSimulateRQ;
	double				m_dLastSimulateDateTime;
	int					m_nDataType;//분, 틱, 일, 주, 월
	static CString		m_strAddInItemName;

protected:
	CString GetDataWindowRealPacketNames(const char *szTitle, const char *szPacketNames) const;
	BOOL	GetDataFromPacketList(int nColIndex, LPCTSTR szRQ, LPCTSTR szPacketName, 
														CChartDataGridManager * pOutChartDataGridManager, BOOL bAdd);
	BOOL	SetDataToChartOCX(IPacketManager * pCurPacketManager, CChartDataGridManager * pChartDataGridManager,CString strLastDateTime);


// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	virtual BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL);
	virtual BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
};

#endif // !defined(AFX_ChartDataSimulatorADDINIMP_H__87872DD8_9073_43F7_AE8D_BFBAF155C985__INCLUDED_)
