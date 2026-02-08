// ChartFormulateData.h: interface for the CChartFormulateData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTFORMULATEDATA_H__E1DA3540_AD73_4CA2_AEA6_453BCD701392__INCLUDED_)
#define AFX_CHARTFORMULATEDATA_H__E1DA3540_AD73_4CA2_AEA6_453BCD701392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CPacketList;
class CIndicatorList;
class CIndicatorInfo;
class CGraphCalculateData;

class CChartFormulateData  
{
public:
	CChartFormulateData( const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);

public:
	CChartFormulateData& operator=(const CChartFormulateData& data);

protected:
	void SetDataInEndComma(CString& strOrgData, const CString& strNewData);


protected:
	CCalculateBaseData::CALCULATETYPE	m_eCalType;
public:
	void								SetCalculateType( const CCalculateBaseData::CALCULATETYPE eCalType)		{	m_eCalType = eCalType;	}
	CCalculateBaseData::CALCULATETYPE	GetCalculateType( void) const											{	return m_eCalType;		}


protected:
	CString		m_strGraphName;
public:
	void		SetGraphName( const CString& strGraphName)				{	m_strGraphName = strGraphName;	}
	CString		GetGraphName( void) const								{	return m_strGraphName;			}


// (2006/12/5 - Seung-Won, Bae) Support Indicator Name
protected:
	CString			m_strIndicatorName;
public:
	void			SetIndicatorName( const char *p_szIndicatorName)	{	m_strIndicatorName = p_szIndicatorName;	}
	const char *	GetIndicatorName( void) const						{	return m_strIndicatorName;				}

	
protected:
	CString		m_strPacketNames;
public:
	void		SetPacketNames( const CString& strPacketNames)			{	m_strPacketNames = strPacketNames;	}
	CString		GetPacketNames( void) const								{	return m_strPacketNames;			}

	
protected:
	CString		m_strSubGraphNames;
public:
	void		SetSubGraphNames( const CString& strSubGraphNames)		{	m_strSubGraphNames = strSubGraphNames;	}
	CString		GetSubGraphNames( void) const							{	return m_strSubGraphNames;				}

	
protected:
	CString		m_strViewableSubGraphNames;
public:
	void		SetViewableSubGraphNames( const CString& strViewableSubGraphNames)	{	m_strViewableSubGraphNames = strViewableSubGraphNames;	}
	CString		GetViewableSubGraphNames( void) const								{	return m_strViewableSubGraphNames;						}

	
protected:
	int		m_nDiaplayDataStartIndex;	// 현재 화면에 보이는 처음 data index
public:
	void	SetDisplayDataStartIndex( const int nDisplayDataStartIndex)		{	m_nDiaplayDataStartIndex = nDisplayDataStartIndex;	}
	int		GetDisplayDataStartIndex( void) const							{	return m_nDiaplayDataStartIndex;					}

	
protected:
	int		m_nDiaplayDataEndIndex;		// 현재 화면에 보이는 마지막 data index
public:
	void	SetDisplayDataEndIndex( const int nDisplayDataEndIndex)		{	m_nDiaplayDataEndIndex = nDisplayDataEndIndex;	}
	int		GetDisplayDataEndIndex( void) const							{	return m_nDiaplayDataEndIndex;					}

	
protected:
	CPacketList *	m_pPacketList;
public:
	void			SetPacketList( CPacketList* pPacketList)			{	m_pPacketList = pPacketList;	}
	CPacketList *	GetPacketList( void) const							{	return m_pPacketList;			}


protected:
	CIndicatorList *	m_pIndicatorList;
public:
	void				SetIndicatorList( CIndicatorList* pIndicatorList)	{	m_pIndicatorList = pIndicatorList;	}
	CIndicatorList *	GetIndicatorList( void) const						{	return m_pIndicatorList;			}

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;
public:
	void				SetOcxHwnd( HWND p_hOcxWnd)			{	m_hOcxWnd = p_hOcxWnd;	}
	HWND				GetOcxHwnd( void) const				{	return m_hOcxWnd;		}

// (2009/9/8 - Seung-Won, Bae) Chart Running Mode for the brick size of Renko on HTS.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
public:
	void					SetChartMode( CChartInfo::CHART_MODE p_eChartMode)		{	m_eChartMode = p_eChartMode;	}
	CChartInfo::CHART_MODE	GetChartMode( void) const								{	return m_eChartMode;			}
};

#endif // !defined(AFX_CHARTFORMULATEDATA_H__E1DA3540_AD73_4CA2_AEA6_453BCD701392__INCLUDED_)
