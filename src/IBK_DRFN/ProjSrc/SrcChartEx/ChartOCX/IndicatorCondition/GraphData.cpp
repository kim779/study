// GraphData.cpp: implementation of the CGraphData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphData.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTOM2()
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CPenThickness

CPenThickness& CPenThickness::operator=(const CPenThickness& data)
{
	if(this == &data)
		return *this;

	m_nThickness1 = data.GetThickness1();
	m_nThickness2 = data.GetThickness2();
	m_nThickness3 = data.GetThickness3();
	m_bIsHightlight = data.IsHightlight();

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	m_nBaseLineThickness = data.GetBaseLineThickness();

	return *this;
}

bool CPenThickness::operator==(const CPenThickness& data) const
{
	if(m_nThickness1 != data.GetThickness1())
		return false;
	if(m_nThickness2 != data.GetThickness2())
		return false;
	if(m_nThickness3 != data.GetThickness3())
		return false;
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	if(m_nBaseLineThickness != data.GetBaseLineThickness())
		return false;
	
	return (m_bIsHightlight == data.IsHightlight());
}

CPenThickness::CPenThickness():
	m_nThickness1(1),
	m_nThickness2(1),
	m_nThickness3(1),
	m_bIsHightlight(false),
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	m_nBaseLineThickness(1)
{
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
CPenThickness::CPenThickness(const int nThickness1, const int nThickness2, const int nThickness3, const int nBaseLineThickness, const bool bIsHightlight):
	m_nThickness1(nThickness1),
	m_nThickness2(nThickness2),
	m_nThickness3(nThickness3),
	m_bIsHightlight(bIsHightlight),
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	m_nBaseLineThickness(nBaseLineThickness)
{
}

CPenThickness::CPenThickness(const CPenThickness& data):
	m_nThickness1(data.GetThickness1()),
	m_nThickness2(data.GetThickness2()),
	m_nThickness3(data.GetThickness3()),
	m_bIsHightlight(data.IsHightlight()),
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	m_nBaseLineThickness(data.GetBaseLineThickness())
{
}

// public =====================================================================
void CPenThickness::SetData( const char *p_szPenThickness)
{
	m_nThickness1	= 1;
	m_nThickness2	= 1;
	m_nThickness3	= 1;
	m_bIsHightlight	= false;
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	m_nBaseLineThickness = 1;

	if( !p_szPenThickness) return;
	if( !*p_szPenThickness) return;

	CString strPenThickness;
	int nPenThicknessLen = strlen( p_szPenThickness);
	char *szPenThickness = strPenThickness.GetBufferSetLength( nPenThicknessLen + 1);
	strcpy( szPenThickness, p_szPenThickness);
	if( szPenThickness[ nPenThicknessLen - 1] != ';')
	{
		szPenThickness[ nPenThicknessLen] = ';';
		szPenThickness[ nPenThicknessLen + 1] = '\0';
	}

	char *szItem = szPenThickness;
	szPenThickness = strchr( szPenThickness, ';');
	if( szPenThickness) *szPenThickness++ = '\0';
	m_nThickness1 = atoi( szItem);
	if( !szPenThickness) return;
	if( !*szPenThickness) return;

	szItem = szPenThickness;
	szPenThickness = strchr( szPenThickness, ';');
	if( szPenThickness) *szPenThickness++ = '\0';
	m_nThickness2 = atoi( szItem);
	if( !szPenThickness) return;
	if( !*szPenThickness) return;

	szItem = szPenThickness;
	szPenThickness = strchr( szPenThickness, ';');
	if( szPenThickness) *szPenThickness++ = '\0';
	m_nThickness3 = atoi( szItem);
	if( !szPenThickness) return;
	if( !*szPenThickness) return;

	szItem = szPenThickness;
	szPenThickness = strchr( szPenThickness, ';');
	if( szPenThickness) *szPenThickness++ = '\0';
	m_bIsHightlight = ( atoi( szItem) != 0);
	if( !szPenThickness) return;
	if( !*szPenThickness) return;

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	szItem = szPenThickness;
	szPenThickness = strchr( szPenThickness, ';');
	if( szPenThickness) *szPenThickness++ = '\0';
	m_nBaseLineThickness = atoi( szItem);
	if( !szPenThickness) return;
	if( !*szPenThickness) return;
}

void CPenThickness::SetHightlight(const bool bIsHightlight)
{
	m_bIsHightlight = bIsHightlight;
}

void CPenThickness::SetThickness(const int nThickness1, const int nThickness2, const int nThickness3)
{
	m_nThickness1 = nThickness1;
	m_nThickness2 = nThickness2;
	m_nThickness3 = nThickness3;
}

void CPenThickness::SetThickness1(const int nThickness)
{
	m_nThickness1 = nThickness;
}

void CPenThickness::SetThickness2(const int nThickness)
{
	m_nThickness2 = nThickness;
}

void CPenThickness::SetThickness3(const int nThickness)
{
	m_nThickness3 = nThickness;
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CPenThickness::SetBaseLineThickness(const int nThickness)
{
	m_nBaseLineThickness = nThickness;
}

void CPenThickness::ReversedHightlight()
{
	m_bIsHightlight = !m_bIsHightlight;
}

// ----------------------------------------------------------------------------
bool CPenThickness::IsHightlight() const
{
	return m_bIsHightlight;
}

int CPenThickness::GetThickness1() const
{
	return m_nThickness1;
}

int CPenThickness::GetThickness2() const
{
	return m_nThickness2;
}

int CPenThickness::GetThickness3() const
{
	return m_nThickness3;
}

int CPenThickness::GetDrawingThickness1() const
{
	return GetDrawingThickness(m_nThickness1);
}

int CPenThickness::GetDrawingThickness2() const
{
	return GetDrawingThickness(m_nThickness2);
}

int CPenThickness::GetDrawingThickness3() const
{
	return GetDrawingThickness(m_nThickness3);
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
int CPenThickness::GetBaseLineThickness() const
{
	return m_nBaseLineThickness;
}

void CPenThickness::GetStringData( CString &p_strPenThickness) const
{
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	char szThickness1[ 20], szThickness2[ 20], szThickness3[ 20], szBaseLineThickness[20];
	itoa( m_nThickness1, szThickness1, 10);
	itoa( m_nThickness2, szThickness2, 10);
	itoa( m_nThickness3, szThickness3, 10);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	itoa( m_nBaseLineThickness, szBaseLineThickness, 10);

	int nThicknessLen1 = strlen( szThickness1); 
	int nThicknessLen2 = strlen( szThickness2); 
	int nThicknessLen3 = strlen( szThickness3); 
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	int nBaseLineThicknessLen = strlen( szBaseLineThickness); 

	int nPenThickness = 0;
	nPenThickness += nThicknessLen1;
	nPenThickness++;
	nPenThickness += nThicknessLen2;
	nPenThickness++;
	nPenThickness += nThicknessLen3;
	nPenThickness += 3;								// ;?;
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	nPenThickness += nBaseLineThicknessLen;
	nPenThickness++;

	char *szPenThickness = p_strPenThickness.GetBufferSetLength( nPenThickness);

	strcpy( szPenThickness, szThickness1);
	szPenThickness += nThicknessLen1;
	*szPenThickness++ = ';';

	strcpy( szPenThickness, szThickness2);
	szPenThickness += nThicknessLen2;
	*szPenThickness++ = ';';

	strcpy( szPenThickness, szThickness3);
	szPenThickness += nThicknessLen3;
	*szPenThickness++ = ';';

	*szPenThickness++ = ( m_bIsHightlight ? '1' : '0');
	*szPenThickness++ = ';';

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	strcpy( szPenThickness, szBaseLineThickness);
	szPenThickness += nBaseLineThicknessLen;
	*szPenThickness++ = ';';
}


// private ====================================================================
int CPenThickness::GetDrawingThickness(const int orgPenThickness) const
{
	if(!m_bIsHightlight)
		return orgPenThickness;

	if(orgPenThickness < 3)
		return orgPenThickness *2;
	return orgPenThickness +2;
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphDataDrawingData

CGraphDataDrawingData& CGraphDataDrawingData::operator=(const CGraphDataDrawingData& data)
{
	if(this == &data)
		return *this;

	m_bCalcRiseFallRate_LowHigh = data.DoesCalcRiseFallRate_LowHigh();
	m_bCalcRiseFallRate_LowCur = data.DoesCalcRiseFallRate_LowCur();
	m_bCalcRiseFallRate_HighCur = data.DoesCalcRiseFallRate_HighCur();
	m_bCalcRate_PrevCur = data.DoesCalcRate_PrevCur();
	m_nCalcRiseFallRate_CurData = data.GetCalcRiseFallRate_CurData();
	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	m_bShowOHLC = data.GetShowOHLC();

	return *this;
}

bool CGraphDataDrawingData::operator==(const CGraphDataDrawingData& data)
{
	if(this == &data)
		return true;

	return (m_bCalcRiseFallRate_LowHigh == data.DoesCalcRiseFallRate_LowHigh() && 
		m_bCalcRiseFallRate_LowCur == data.DoesCalcRiseFallRate_LowCur() && 
		m_bCalcRiseFallRate_HighCur == data.DoesCalcRiseFallRate_HighCur() && 
		m_bCalcRate_PrevCur == data.DoesCalcRate_PrevCur() && 
		m_nCalcRiseFallRate_CurData == data.GetCalcRiseFallRate_CurData() &&
		// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
		m_bShowOHLC == data.GetShowOHLC());
}

CGraphDataDrawingData::CGraphDataDrawingData():
	m_bCalcRiseFallRate_LowHigh(false),
	m_bCalcRiseFallRate_LowCur(false),
	m_bCalcRiseFallRate_HighCur(false),
	m_bCalcRate_PrevCur(false),
	m_nCalcRiseFallRate_CurData(0),
	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	m_bShowOHLC( FALSE)
{
}

CGraphDataDrawingData::CGraphDataDrawingData( const char *p_szGraphDataDrawingData):
	m_bCalcRiseFallRate_LowHigh(false),
	m_bCalcRiseFallRate_LowCur(false),
	m_bCalcRiseFallRate_HighCur(false),
	m_bCalcRate_PrevCur(false),
	m_nCalcRiseFallRate_CurData(0),
	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	m_bShowOHLC( FALSE)
{
	SetData( p_szGraphDataDrawingData);
}

CGraphDataDrawingData::CGraphDataDrawingData(const bool bCalcRiseFallRate_LowHigh, const bool bCalcRiseFallRate_LowCur, 
	const bool bCalcRiseFallRate_HighCur, const bool bCalcRate_PrevCur, const int nCalcRiseFallRate_CurData, const BOOL p_bShowOHLC):
	m_bCalcRiseFallRate_LowHigh(bCalcRiseFallRate_LowHigh),
	m_bCalcRiseFallRate_LowCur(bCalcRiseFallRate_LowCur),
	m_bCalcRiseFallRate_HighCur(bCalcRiseFallRate_HighCur),
	m_bCalcRate_PrevCur(bCalcRate_PrevCur),
	m_nCalcRiseFallRate_CurData(nCalcRiseFallRate_CurData),
	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	m_bShowOHLC( p_bShowOHLC)
{
}

CGraphDataDrawingData::CGraphDataDrawingData(const CGraphDataDrawingData& data):
	m_bCalcRiseFallRate_LowHigh(data.DoesCalcRiseFallRate_LowHigh()),
	m_bCalcRiseFallRate_LowCur(data.DoesCalcRiseFallRate_LowCur()),
	m_bCalcRiseFallRate_HighCur(data.DoesCalcRiseFallRate_HighCur()),
	m_bCalcRate_PrevCur(data.DoesCalcRate_PrevCur()),
	m_nCalcRiseFallRate_CurData(data.GetCalcRiseFallRate_CurData()),
	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	m_bShowOHLC( data.GetShowOHLC())
{
}

// (2006/5/9 - Seung-Won, Bae) Clear Data
void CGraphDataDrawingData::ClearData( void)
{
	m_bCalcRiseFallRate_LowHigh	= false;
	m_bCalcRiseFallRate_LowCur	= false;
	m_bCalcRiseFallRate_HighCur	= false;
	m_bCalcRate_PrevCur			= false;
	m_nCalcRiseFallRate_CurData	= 0;
	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	m_bShowOHLC					= FALSE;
}

// public =====================================================================
void CGraphDataDrawingData::SetDoesCalcRiseFallRate_LowHigh(const bool bCalcData)
{
	m_bCalcRiseFallRate_LowHigh = bCalcData;
}

void CGraphDataDrawingData::SetDoesCalcRiseFallRate_LowCur(const bool bCalcData)
{
	m_bCalcRiseFallRate_LowCur = bCalcData;
}

void CGraphDataDrawingData::SetDoesCalcRiseFallRate_HighCur(const bool bCalcData)
{
	m_bCalcRiseFallRate_HighCur = bCalcData;
}

void CGraphDataDrawingData::SetDoesCalcRate_PrevCur(const bool bCalcData)
{
	m_bCalcRate_PrevCur = bCalcData;
}

void CGraphDataDrawingData::SetCalcRiseFallRate_CurData(const int nCalcData)
{
	m_nCalcRiseFallRate_CurData = nCalcData;
}

// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
void CGraphDataDrawingData::SetShowOHLC( const BOOL p_bShowOHLC)
{
	m_bShowOHLC	= p_bShowOHLC;
}

// ----------------------------------------------------------------------------
bool CGraphDataDrawingData::DoesAllDefaultData() const
{
	return (m_bCalcRiseFallRate_LowHigh == false && 
		m_bCalcRiseFallRate_LowCur == false && 
		m_bCalcRiseFallRate_HighCur == false && 
		m_bCalcRate_PrevCur == false && 
		m_nCalcRiseFallRate_CurData == 0);
}

bool CGraphDataDrawingData::DoesCalcRiseFallRate_LowHigh() const
{
	return m_bCalcRiseFallRate_LowHigh;
}

bool CGraphDataDrawingData::DoesCalcRiseFallRate_LowCur() const
{
	return m_bCalcRiseFallRate_LowCur;
}

bool CGraphDataDrawingData::DoesCalcRiseFallRate_HighCur() const
{
	return m_bCalcRiseFallRate_HighCur;
}

bool CGraphDataDrawingData::DoesCalcRate_PrevCur() const
{
	return m_bCalcRate_PrevCur;
}

int CGraphDataDrawingData::GetCalcRiseFallRate_CurData() const
{
	return m_nCalcRiseFallRate_CurData;
}

// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
BOOL CGraphDataDrawingData::GetShowOHLC( void) const
{
	return m_bShowOHLC;
}

// graph data 가져오기
void CGraphDataDrawingData::GetData( CString &p_strGraphDataDrawingData) const
{
	char *szGraphDataDrawingData = p_strGraphDataDrawingData.GetBufferSetLength( 12);	// Do not set 13, It has default NULL Terminator.
	szGraphDataDrawingData[  0] = ( m_bCalcRiseFallRate_LowHigh			? '1' : '0');
	szGraphDataDrawingData[  1] = ';';
	szGraphDataDrawingData[  2] = ( m_bCalcRiseFallRate_LowCur			? '1' : '0');
	szGraphDataDrawingData[  3] = ';';
	szGraphDataDrawingData[  4] = ( m_bCalcRiseFallRate_HighCur			? '1' : '0');
	szGraphDataDrawingData[  5] = ';';
	szGraphDataDrawingData[  6] = ( m_bCalcRate_PrevCur					? '1' : '0');
	szGraphDataDrawingData[  7] = ';';
	szGraphDataDrawingData[  8] = '0' + m_nCalcRiseFallRate_CurData;
	szGraphDataDrawingData[  9] = ';';
	szGraphDataDrawingData[ 10] = '0' + m_bShowOHLC;	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	szGraphDataDrawingData[ 11] = ';';
	szGraphDataDrawingData[ 12] = '\0';
}


// private ====================================================================
void CGraphDataDrawingData::SetData( const char *p_szGraphDataDrawingData)
{
	if( !p_szGraphDataDrawingData) return;
	if( !*p_szGraphDataDrawingData) return;

	CString strGraphDataDrawingData;
	int nGraphDataDrawingDataLen = strlen( p_szGraphDataDrawingData);
	char *szGraphDataDrawingData = strGraphDataDrawingData.GetBufferSetLength( nGraphDataDrawingDataLen + 1);
	strcpy( szGraphDataDrawingData, p_szGraphDataDrawingData);
	if( szGraphDataDrawingData[ nGraphDataDrawingDataLen - 1] != ';')
	{
		szGraphDataDrawingData[ nGraphDataDrawingDataLen] = ';';
		szGraphDataDrawingData[ nGraphDataDrawingDataLen + 1] = '\0';
	}

	char *szItem = szGraphDataDrawingData;
	szGraphDataDrawingData = strchr( szGraphDataDrawingData, ';');
	if( szGraphDataDrawingData) *szGraphDataDrawingData++ = '\0';
	m_bCalcRiseFallRate_LowHigh = ( atoi( szItem) != 0);
	if( !szGraphDataDrawingData) return;
	if( !*szGraphDataDrawingData) return;

	szItem = szGraphDataDrawingData;
	szGraphDataDrawingData = strchr( szGraphDataDrawingData, ';');
	if( szGraphDataDrawingData) *szGraphDataDrawingData++ = '\0';
	m_bCalcRiseFallRate_LowCur = ( atoi( szItem) != 0);
	if( !szGraphDataDrawingData) return;
	if( !*szGraphDataDrawingData) return;

	szItem = szGraphDataDrawingData;
	szGraphDataDrawingData = strchr( szGraphDataDrawingData, ';');
	if( szGraphDataDrawingData) *szGraphDataDrawingData++ = '\0';
	m_bCalcRiseFallRate_HighCur = ( atoi( szItem) != 0);
	if( !szGraphDataDrawingData) return;
	if( !*szGraphDataDrawingData) return;

	szItem = szGraphDataDrawingData;
	szGraphDataDrawingData = strchr( szGraphDataDrawingData, ';');
	if( szGraphDataDrawingData) *szGraphDataDrawingData++ = '\0';
	m_bCalcRate_PrevCur = ( atoi( szItem) != 0);
	if( !szGraphDataDrawingData) return;
	if( !*szGraphDataDrawingData) return;

	szItem = szGraphDataDrawingData;
	szGraphDataDrawingData = strchr( szGraphDataDrawingData, ';');
	if( szGraphDataDrawingData) *szGraphDataDrawingData++ = '\0';
	m_nCalcRiseFallRate_CurData = atoi( szItem);
	if( !szGraphDataDrawingData) return;
	if( !*szGraphDataDrawingData) return;

	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data
	szItem = szGraphDataDrawingData;
	szGraphDataDrawingData = strchr( szGraphDataDrawingData, ';');
	if( szGraphDataDrawingData) *szGraphDataDrawingData++ = '\0';
	m_bShowOHLC = atoi( szItem);
	if( !szGraphDataDrawingData) return;
	if( !*szGraphDataDrawingData) return;
}

///////////////////////////////////////////////////////////////////////////////
// class CSignalData

CSignalData& CSignalData::operator=(const CSignalData& data)
{
	if(this == &data)
		return *this;

	m_bIsUse = data.IsUse();
	m_bIsShow = data.IsShow();
	return *this;
}

bool CSignalData::operator==(const CSignalData& data)
{
	if(this == &data)
		return true;

	return (m_bIsUse == data.IsUse() && m_bIsShow == data.IsShow());
}

CSignalData::CSignalData(const bool bIsUse, const bool bIsShow):
	m_bIsUse(bIsUse),
	m_bIsShow(bIsShow)
{
	// (2009/10/7 - Seung-Won, Bae) for Signal Arrow Drawing Cache.
	m_pSignalCache = NULL;
}

CSignalData::CSignalData( const char *p_szSignalData)
{
	SetData( p_szSignalData);

	// (2009/10/7 - Seung-Won, Bae) for Signal Arrow Drawing Cache.
	m_pSignalCache = NULL;
}

CSignalData::CSignalData(const CSignalData& data) : 
	m_bIsUse(data.IsUse()),
	m_bIsShow(data.IsShow())
{
	// (2009/10/7 - Seung-Won, Bae) for Signal Arrow Drawing Cache.
	m_pSignalCache = NULL;
}

// (2009/10/7 - Seung-Won, Bae) for Signal Arrow Drawing Cache.
CSignalData::~CSignalData()
{
	ClearCalcDataForDraw();
}

// (2006/5/9 - Seung-Won, Bae) Clear Data
void CSignalData::ClearData( void)
{
	m_bIsUse	= false;
	m_bIsShow	= false;
}

// public =====================================================================
void CSignalData::SetUse(const bool bIsUse)
{
	m_bIsUse = bIsUse;
}

void CSignalData::SetShow(const bool bIsShow)
{
	m_bIsShow = bIsShow;
}

// ----------------------------------------------------------------------------
bool CSignalData::IsUse() const
{
	return m_bIsUse;
}

bool CSignalData::IsShow() const
{
	return m_bIsShow;
}

bool CSignalData::IsDraw() const
{
	if(!m_bIsUse)
		return false;
	return m_bIsShow;
}


// ----------------------------------------------------------------------------
void CSignalData::GetData( CString &p_strSignalData) const
{
	char *szSignalData = p_strSignalData.GetBufferSetLength( 4);	// Do not set 5, It has default NULL Terminator.
	szSignalData[ 0] = ( m_bIsUse	? '1' : '0');
	szSignalData[ 1] = ':';
	szSignalData[ 2] = ( m_bIsShow	? '1' : '0');
	szSignalData[ 3] = ';';
	szSignalData[ 4] = '\0';
}


// private ====================================================================
void CSignalData::SetData( const char *p_szSignalData)
{
	if( !p_szSignalData) return;
	if( !*p_szSignalData) return;

	CString strSignalData;
	int nSignalDataLen = strlen( p_szSignalData);
	char *szSignalData = strSignalData.GetBufferSetLength( nSignalDataLen + 1);
	strcpy( szSignalData, p_szSignalData);
	if( szSignalData[ nSignalDataLen - 1] != ';')
	{
		szSignalData[ nSignalDataLen] = ';';
		szSignalData[ nSignalDataLen + 1] = '\0';
	}

	// "signal사용여부:signal사용;"
	// signal사용여부
	char *szItem = szSignalData;
	szSignalData = strchr( szSignalData, ':');
	if( szSignalData) *szSignalData++ = '\0';
	m_bIsUse = ( atoi( szItem) != 0);
	if( !szSignalData) return;
	if( !*szSignalData) return;
	// signal사용
	szItem = szSignalData;
	szSignalData = strchr( szSignalData, ';');
	if( szSignalData) *szSignalData++ = '\0';
	m_bIsShow = ( atoi( szItem) != 0);
	if( !szSignalData) return;
	if( !*szSignalData) return;
}

// (2009/10/7 - Seung-Won, Bae) for Signal Arrow Drawing Cache.
void CSignalData::ClearCalcDataForDraw( void)
{
	if( m_pSignalCache)
	{
		delete m_pSignalCache;
		m_pSignalCache = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphData

CGraphData& CGraphData::operator=(const CGraphData& data)
{
	if(this == &data) return *this;

	m_strIndicatorName		= data.m_strIndicatorName;
	m_strGraphName			= data.m_strGraphName;
	m_strPacketName			= data.m_strPacketName;
	m_signalData			= data.m_signalData;
	m_graphDataDrawingData	= data.m_graphDataDrawingData;

	return *this;
}

// (2006/5/9 - Seung-Won, Bae) Clear Data
void CGraphData::ClearData( void)
{
	m_strIndicatorName.Empty();
	m_strGraphName.Empty();
	m_strPacketName.Empty();
	m_signalData.ClearData();
	m_graphDataDrawingData.ClearData();
}

void CGraphData::SetPacketNames(const CString& packetNames)
{
	if(m_strPacketName == packetNames)
		return;

	m_strPacketName = packetNames;
}

void CGraphData::SetSignalData(const CSignalData& signalData)
{
	m_signalData = signalData;
}

void CGraphData::SetSignalData(const bool bIsUse, const bool bIsShow)
{
	m_signalData.SetUse(bIsUse);
	m_signalData.SetShow(bIsShow);
}

void CGraphData::SetGraphDataDrawingData(const CGraphDataDrawingData& drawingData)
{
	m_graphDataDrawingData = drawingData;
}

CString CGraphData::GetPacketNames() const
{
	return m_strPacketName;
}

CSignalData CGraphData::GetSignalData() const
{
	return m_signalData;
}

CSignalData& CGraphData::GetSignalData()
{
	return m_signalData;
}

bool CGraphData::IsSignalUse() const
{
	return m_signalData.IsUse();
}

bool CGraphData::IsSignalShow() const
{
	return m_signalData.IsShow();
}

CGraphDataDrawingData CGraphData::GetGraphDataDrawingData() const
{
	return m_graphDataDrawingData;
}

CGraphDataDrawingData& CGraphData::GetGraphDataDrawingData()
{
	return m_graphDataDrawingData;
}

// (2006/9/6 - Seung-Won, Bae) Map Design Info.
CGraphData::CGraphData( const char *p_szGraphData, HWND p_hOcxWnd)
{
	if( !p_szGraphData) return;
	if( !*p_szGraphData) return;

	// "index;가격차트;시가,고가,저자,종가;signal여부(사용여부:보임);
	//  최저가최고가등락률;최저가현재가등락률;최고가현재가등락률;전일비;현재가;
	//  최고값보기;최저값보기;부호표시;색표현방법(0:봉색,1:빨/파,2:양(빨),음(파));\r\n"
	CString strGraphData;
	int nGraphDataLen = strlen( p_szGraphData);
	char *szGraphData = strGraphData.GetBufferSetLength( nGraphDataLen);
	strcpy( szGraphData, p_szGraphData);
	// (2006/10/26 - Seung-Won, Bae) Skip Function Index;
	szGraphData = strchr( szGraphData, ';');	// m_nFunctionIndex = atoi(CDataConversion::GetStringData(strRealData, ";")); // function index
	if( !szGraphData) return;
	szGraphData++;
	// graph function name
	char *szItem = szGraphData;
	szGraphData = strchr( szGraphData, ';');	// m_nFunctionIndex = atoi(CDataConversion::GetStringData(strRealData, ";")); // function index
	if( szGraphData) *szGraphData++ = '\0';
	m_strIndicatorName = _MTOM2( szItem);
	if( !szGraphData) return;
	if( !*szGraphData) return;
	// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
	if( strncmp( szGraphData, "G:", 2)) m_strGraphName = m_strIndicatorName;
	else
	{
		szItem = szGraphData + 2;
		szGraphData = strchr( szGraphData, ';');
		if( szGraphData) *szGraphData++ = '\0';
		m_strGraphName = _MTOM2( szItem);		// (2008/2/22 - Seung-Won, Bae) The first default Graph Name is Indicator Name.
		if( !szGraphData) return;
		if( !*szGraphData) return;
	}
	// packet name
	szItem = szGraphData;
	szGraphData = strchr( szGraphData, ';');
	if( szGraphData) *szGraphData++ = '\0';
	_MHWND( p_hOcxWnd);
	if( strchr( szItem, '('))	m_strPacketName = _STOMS( szItem, ",(+)/23", 0);
	else						m_strPacketName = _STOMS( szItem, ",", 0);
	if( !szGraphData) return;
	if( !*szGraphData) return;
	// Signal
	szItem = szGraphData;
	szGraphData = strchr( szGraphData, ';');
	if( szGraphData) *szGraphData++ = '\0';
	m_signalData = CSignalData( szItem);
	if( !szGraphData) return;
	if( !*szGraphData) return;
	// Graph Drawing Data
	szItem = szGraphData;
	int i = 0;
	do
	{
		szGraphData = strchr( szGraphData, ';');
		if( !szGraphData) continue;
		i++;
		szGraphData++;
	} while( szGraphData && i < 6);
	if( szGraphData) *szGraphData = '\0';
	m_graphDataDrawingData = CGraphDataDrawingData( szItem);
}

// 복수종목 저장 : 복수종목 - ojtaso (20070507)
void CGraphData::GetData( CString &p_strGraphData, LPCTSTR lpszRQ/* = NULL*/) const
{
	CString strSingalData;
	m_signalData.GetData( strSingalData);

	CString strGraphDataDrawingData;
	m_graphDataDrawingData.GetData( strGraphDataDrawingData);
	
	int nGraphDataLen = 0;									//
	nGraphDataLen++;										// ;
	nGraphDataLen += m_strIndicatorName.GetLength();		// Indicator Name
	nGraphDataLen += 3;										// ;G:
	nGraphDataLen += m_strGraphName.GetLength();			// Graph Name
	nGraphDataLen++;										// ;
	nGraphDataLen += m_strPacketName.GetLength();			// Packet Name
	nGraphDataLen++;										// ;
	nGraphDataLen += strSingalData.GetLength();				// Singal Data
	nGraphDataLen += strGraphDataDrawingData.GetLength();	// Graph Data Drawing Data
	// 복수종목 저장 : 복수종목 - ojtaso (20070507)
	if(lpszRQ)
	{
		nGraphDataLen += 3;										// RQ:
		nGraphDataLen += ::lstrlen(lpszRQ);
	}
	nGraphDataLen += 2;										// \r\n (Do not add NULL Terminator size. It has that with default)
	char *szGraphData = p_strGraphData.GetBufferSetLength( nGraphDataLen);

	*szGraphData++ = ';';

	strcpy( szGraphData, m_strIndicatorName);
	szGraphData += m_strIndicatorName.GetLength();

	*szGraphData++ = ';';
	*szGraphData++ = 'G';
	*szGraphData++ = ':';

	strcpy( szGraphData, m_strGraphName);
	szGraphData += m_strGraphName.GetLength();

	*szGraphData++ = ';';

	strcpy( szGraphData, m_strPacketName);
	szGraphData += m_strPacketName.GetLength();

	*szGraphData++ = ';';

	strcpy( szGraphData, strSingalData);
	szGraphData += strSingalData.GetLength();

	strcpy( szGraphData, strGraphDataDrawingData);
	szGraphData += strGraphDataDrawingData.GetLength();

	// 복수종목 저장 : 복수종목 - ojtaso (20070507)
	if(lpszRQ)
	{
		strcpy( szGraphData, _T("RQ:"));
		szGraphData += 3;
		strcpy( szGraphData, lpszRQ);
		szGraphData += ::lstrlen(lpszRQ);
	}

	*szGraphData++ = '\r';
	*szGraphData++ = '\n';
	*szGraphData++ = '\0';
}

///////////////////////////////////////////////////////////////////////////////
// class CSubGraphDataBase

CSubGraphDataBase& CSubGraphDataBase::operator=(const CSubGraphDataBase& data)
{
	if(this == &data)
		return *this;

	m_nTitleEndPosition = data.GetTitleEndPosition();
	m_nTitleEndLine = data.GetTitleEndLine();
	m_TYPEType = data.GetType();
	m_nStyle = data.GetStyle();
	m_nDrawStyle = data.GetDrawStyle();
	m_strName = data.GetName();
	m_strTitle = data.GetTitle();
	m_bHiding = data.m_bHiding;
	m_clrColor = data.GetColor();
	m_penThickness = data.GetPenThickness();
	// 종목명 표시 : 복수종목 - ojtaso (20070625)
	m_strItemName = data.GetItemName();
	m_nSubGraphIndex = data.GetSubGraphIndex();	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	m_nPaintStyle = data.GetPaintStyle();		// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	m_nBaseLineStyle = data.GetBaseLineDrawStyle();		// 기준선 두께, 모양 설정 - ojtaso (20071213)
	
	// KTB 체결잔고표시 - ojtaso (20091019)
	const CArray<ST_HORZLINE*, ST_HORZLINE*>& arrInfo = data.GetHorizontalLine();
	m_arrHorzLine.Copy(arrInfo);	
	return *this;
}

CSubGraphDataBase::CSubGraphDataBase():
	m_nTitleEndPosition(0),
	m_nTitleEndLine(1), 	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	m_TYPEType(CGraphBaseData::GTNONE),
	m_bHiding(true)
{
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
// 기준선 두께, 모양 설정 - ojtaso (20071213)
CSubGraphDataBase::CSubGraphDataBase(const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle):
	m_nTitleEndPosition(0),
	m_nTitleEndLine(1), 	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	m_TYPEType(type),
	m_nStyle(style),
	m_nDrawStyle(drawStyle),
	m_strName(name),
	m_strTitle(title),
	m_bHiding(hiding),
	m_clrColor(color),
	m_penThickness(penThickness),
	m_nSubGraphIndex(-1),	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	m_nPaintStyle(paintStyle),		// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	m_nBaseLineStyle(baseLineStyle)		// 기준선 두께, 모양 설정 - ojtaso (20071213)
{
	// 종목명 표시 : 복수종목 - ojtaso (20070625)
	m_strItemName.Empty();
}

CSubGraphDataBase::CSubGraphDataBase(const CSubGraphDataBase& data):
	m_nTitleEndPosition(data.GetTitleEndPosition()),
	m_nTitleEndLine(data.GetTitleEndLine()), 	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	m_TYPEType(data.GetType()),
	m_nStyle(data.GetStyle()),
	m_nDrawStyle(data.GetDrawStyle()),
	m_strName(data.GetName()),
	m_strTitle(data.GetTitle()),
	m_bHiding( data.m_bHiding),
	m_clrColor(data.GetColor()),
	m_penThickness(data.GetPenThickness()),
	// 종목명 표시 : 복수종목 - ojtaso (20070625)
	m_strItemName(data.GetItemName()),
	m_nSubGraphIndex(data.GetSubGraphIndex()),	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	m_nPaintStyle(data.GetPaintStyle()),		// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	m_nBaseLineStyle(data.GetBaseLineDrawStyle())	// 기준선 두께, 모양 설정 - ojtaso (20071213)
{
	// KTB 체결잔고표시 - ojtaso (20091019)
	const CArray<ST_HORZLINE*, ST_HORZLINE*>& arrInfo = data.GetHorizontalLine();
	m_arrHorzLine.Copy(arrInfo);	
}

CSubGraphDataBase::CSubGraphDataBase( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName):
	m_nTitleEndPosition(0),
	m_nTitleEndLine(1), 	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	m_TYPEType(CGraphBaseData::GTNONE),
	m_bHiding(true),
	m_nSubGraphIndex(-1)	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
{
	SetData( p_szSubGraphData, p_hOcxWnd, p_szIndicatorName);
}

CSubGraphDataBase::~CSubGraphDataBase()
{
	DeleteAllHorizontalLine();
}

// ----------------------------------------------------------------------------
// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
void CSubGraphDataBase::SetTitleEndPosition(const int nTitleEndPosition, const int nTitleEndLine/* = 1*/)
{
	m_nTitleEndPosition = nTitleEndPosition;
	m_nTitleEndLine = nTitleEndLine;
}

void CSubGraphDataBase::SetData( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName)
{
	m_TYPEType					= CGraphBaseData::GTNONE;
	m_nStyle.SetGraphStyle(		0);
	m_nDrawStyle.SetDrawStyle(	0);
	m_strName.Empty();
	m_strTitle.Empty();
	m_bHiding					= false;
	m_clrColor.SetData(			NULL);
	m_penThickness.SetData(		NULL);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	m_nPaintStyle.SetPaintStyle(1);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	m_nBaseLineStyle.SetBaseLineStyle(0);

	if( !p_szSubGraphData) return;
	if( !*p_szSubGraphData) return;

	// "그래프타입;그래프스타일;그리는스타일;이름;title;숨김;색1;색2;색3;색4;색5;색6;
	//  상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n"
	CString strSubGraphData;
	int nSubGraphDataLen = strlen( p_szSubGraphData);
	char *szSubGraphData = strSubGraphData.GetBufferSetLength( nSubGraphDataLen);
	strcpy( szSubGraphData, p_szSubGraphData);

	char *szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	m_TYPEType = ( CGraphBaseData::GRAPHTYPE)atoi( szItem);
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	m_nStyle = ( CGraphStyle)atoi( szItem);
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	m_nDrawStyle = ( CGraphDrawStyle)atoi( szItem);
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	if( m_TYPEType == CGraphBaseData::Bong_Type && m_nStyle.GetGraphStyle() != m_nDrawStyle.GetDrawStyle())
		m_nDrawStyle.SetDrawStyle( m_nStyle.GetGraphStyle());

	szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	m_strName = _MTOM3( szItem);
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	if( g_iMetaTable.IsTitleIncludeCondition( p_szIndicatorName)) m_strTitle = szItem;
	else
	{
		_MHWND( p_hOcxWnd);
		m_strTitle = _STOM1( szItem);
	}
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	m_bHiding = ( atoi( szItem) != 0);
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	int i = 0;
	do
	{
		szSubGraphData = strchr( szSubGraphData, ';');
		if( !szSubGraphData) continue;
		i++;
		szSubGraphData++;
	//} while( szSubGraphData && i < 8);
	} while( szSubGraphData && i < 14);		//@Solomon-091222
	if( szSubGraphData) *( szSubGraphData - 1) = '\0';
	m_clrColor.SetData( szItem);
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	m_penThickness.SetData( szItem);

	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	i = 0;
	do
	{
		szSubGraphData = strchr( szSubGraphData, ';');
		if( !szSubGraphData) continue;
		i++;
		szSubGraphData++;
	//@Solomon} while( szSubGraphData && i < 5);
	//@Solomon 상승, 양봉, 하락, 음봉 색상 분리 - ojtaso (20080707)
	} while( szSubGraphData && i < 14);

	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	m_nPaintStyle = ( CGraphPaintStyle)atoi( szItem);

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	if( !szSubGraphData) return;
	if( !*szSubGraphData) return;

	szItem = szSubGraphData;
	szSubGraphData = strchr( szSubGraphData, ';');
	if( szSubGraphData) *szSubGraphData++ = '\0';
	m_nBaseLineStyle = ( CGraphBaseLineStyle)atoi( szItem);
}

void CSubGraphDataBase::SetType(const CGraphBaseData::GRAPHTYPE type)
{
	m_TYPEType = type;
}

void CSubGraphDataBase::SetStyle(const CGraphStyle& style)
{
	m_nStyle = style;
}

void CSubGraphDataBase::SetDrawStyle(const CGraphDrawStyle& drawStyle)
{
	m_nDrawStyle = drawStyle;
}

void CSubGraphDataBase::SetName(const CString& name)
{
	m_strName = name;
}

void CSubGraphDataBase::SetTitle(const CString& title)
{
	m_strTitle = title;
}

// 종목명 표시 : 복수종목 - ojtaso (20070625)
void CSubGraphDataBase::SetItemName(const CString& strItemName)
{
	m_strItemName = strItemName;
}

// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
void CSubGraphDataBase::SetSubGraphIndex(const int nSubGraphIndex)
{
	m_nSubGraphIndex = nSubGraphIndex;
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
void CSubGraphDataBase::SetPaintStyle(const CGraphPaintStyle& nPaintStyle)
{
	m_nPaintStyle = nPaintStyle;
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
void CSubGraphDataBase::SetBaseLineStyle(const CGraphBaseLineStyle& nBaseLineStyle)
{
	m_nBaseLineStyle = nBaseLineStyle;
}

void CSubGraphDataBase::SetHiding(const bool hiding)
{
	m_bHiding = hiding;
}

void CSubGraphDataBase::SetColor(const CGraphColor& color)
{
	m_clrColor = color;
}

void CSubGraphDataBase::SetPenThickness(const CPenThickness& penThickness)
{
	m_penThickness = penThickness;
}

// ----------------------------------------------------------------------------
void CSubGraphDataBase::ChangeKeyPart(const CSubGraphDataBase& data)
{
	m_TYPEType = data.GetType();		// graph type
	m_nStyle = data.GetStyle();			// graph style
	m_nDrawStyle = data.GetDrawStyle();	// graph draw style
	m_strName = data.GetName();			// graph name
}

// ----------------------------------------------------------------------------
int CSubGraphDataBase::GetTitleEndPosition() const
{
	return m_nTitleEndPosition;
}

// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
int CSubGraphDataBase::GetTitleEndLine() const
{
	return m_nTitleEndLine;
}

CGraphBaseData::GRAPHTYPE CSubGraphDataBase::GetType() const
{
	return m_TYPEType;
}

CGraphStyle CSubGraphDataBase::GetStyle() const
{
	return m_nStyle;
}

CGraphDrawStyle CSubGraphDataBase::GetDrawStyle() const
{
	return m_nDrawStyle;
}

// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
CGraphPaintStyle CSubGraphDataBase::GetPaintStyle() const
{
	return m_nPaintStyle;
}

// 기준선 두께, 모양 설정 - ojtaso (20071213)
CGraphBaseLineStyle CSubGraphDataBase::GetBaseLineDrawStyle() const
{
	return m_nBaseLineStyle;
}

CString CSubGraphDataBase::GetName() const
{
	return m_strName;
}

CString CSubGraphDataBase::GetTitle() const
{
	return m_strTitle;
}

// 종목명 표시 : 복수종목 - ojtaso (20070625)
CString CSubGraphDataBase::GetItemName() const
{
	return m_strItemName;
}

// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
int CSubGraphDataBase::GetSubGraphIndex() const
{
	return m_nSubGraphIndex;
}

CGraphColor CSubGraphDataBase::GetColor() const
{
	return m_clrColor;
}

CGraphColor* CSubGraphDataBase::GetColor_PointerType()
{
	return &m_clrColor;
}

CGraphColor& CSubGraphDataBase::GetColor()
{
	return m_clrColor;
}

CPenThickness CSubGraphDataBase::GetPenThickness() const
{
	return m_penThickness;
}

CPenThickness& CSubGraphDataBase::GetPenThickness()
{
	return m_penThickness;
}

void CSubGraphDataBase::GetStringData( CString &p_strSubGraphData) const
{
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	char szType[ 20], szGraphStyle[ 20], szDrawStyle[ 20], szPaintStyle[20], szBaseLineStyle[20];

	itoa( m_TYPEType,					szType,			10);
	itoa( m_nStyle.GetGraphStyle(),		szGraphStyle,	10);
	itoa( m_nDrawStyle.GetDrawStyle(),	szDrawStyle,	10);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	itoa( m_nPaintStyle.GetPaintStyle(),	szPaintStyle,	10);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	itoa( m_nBaseLineStyle.GetBaseLineStyle(),	szBaseLineStyle,	10);

	int nTypeLen		= strlen( szType);
	int nGraphStyleLen	= strlen( szGraphStyle);
	int nDrawStyleLen	= strlen( szDrawStyle);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	int nPaintStyleLen	= strlen( szPaintStyle);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	int nBaseLineStyleLen	= strlen( szBaseLineStyle);

	CString strGraphColor;
	m_clrColor.GetStringData( strGraphColor);

	CString strPenThickness;
	m_penThickness.GetStringData( strPenThickness);

	int nSubGraphDataLen = 0;
	nSubGraphDataLen += nTypeLen;
	nSubGraphDataLen++;
	nSubGraphDataLen += nGraphStyleLen;
	nSubGraphDataLen++;
	nSubGraphDataLen += nDrawStyleLen;
	nSubGraphDataLen++;
	nSubGraphDataLen += m_strName.GetLength();
	nSubGraphDataLen++;
	nSubGraphDataLen += m_strTitle.GetLength();
	nSubGraphDataLen++;
	nSubGraphDataLen += 2;	// ?;
	nSubGraphDataLen += strGraphColor.GetLength();
	nSubGraphDataLen += strPenThickness.GetLength();
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	nSubGraphDataLen++;
	nSubGraphDataLen += nPaintStyleLen;
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	nSubGraphDataLen++;
	nSubGraphDataLen += nBaseLineStyleLen;
	nSubGraphDataLen += 2;	// \r\n

	char *szSubGraphData = p_strSubGraphData.GetBufferSetLength( nSubGraphDataLen);

	strcpy( szSubGraphData, szType);
	szSubGraphData += nTypeLen;
	*szSubGraphData++ = ';';

	strcpy( szSubGraphData, szGraphStyle);
	szSubGraphData += nGraphStyleLen;
	*szSubGraphData++ = ';';

	strcpy( szSubGraphData, szDrawStyle);
	szSubGraphData += nDrawStyleLen;
	*szSubGraphData++ = ';';

	strcpy( szSubGraphData, m_strName);
	szSubGraphData += m_strName.GetLength();
	*szSubGraphData++ = ';';

	strcpy( szSubGraphData, m_strTitle);
	szSubGraphData += m_strTitle.GetLength();
	*szSubGraphData++ = ';';

	*szSubGraphData++ = ( m_bHiding ? '1' : '0');
	*szSubGraphData++ = ';';

	strcpy( szSubGraphData, strGraphColor);
	szSubGraphData += strGraphColor.GetLength();

	strcpy( szSubGraphData, strPenThickness);
	szSubGraphData += strPenThickness.GetLength();

	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	strcpy( szSubGraphData, szPaintStyle);
	szSubGraphData += nPaintStyleLen;
	*szSubGraphData++ = ';';

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	strcpy( szSubGraphData, szBaseLineStyle);
	szSubGraphData += nBaseLineStyleLen;
	*szSubGraphData++ = ';';

	*szSubGraphData++ = '\r';
	*szSubGraphData++ = '\n';
	*szSubGraphData++ = '\0';
}

// KTB 체결잔고표시 - ojtaso (20091019)
long CSubGraphDataBase::AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	ST_HORZLINE* pInfo = new ST_HORZLINE;
	pInfo->m_strTitle = lpszTitle;
	pInfo->m_clrLine = clrLine;
	pInfo->m_nLine = nLine;
	pInfo->m_dPrice = dPrice;

	int nSize = m_arrHorzLine.GetSize();
	if(nSize > 0)
		pInfo->m_nIndex = m_arrHorzLine.GetAt(nSize - 1)->m_nIndex + 1;
	else
		pInfo->m_nIndex = 1;

	m_arrHorzLine.Add(pInfo);

	return pInfo->m_nIndex;
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CSubGraphDataBase::UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	for(int i = 0; i < m_arrHorzLine.GetSize(); i++)
	{
		ST_HORZLINE* pInfo = m_arrHorzLine.GetAt(i);
		if(pInfo->m_nIndex == (UINT)nIndex)
		{
			pInfo->m_strTitle = lpszTitle;
			pInfo->m_clrLine = clrLine;
			pInfo->m_nLine = nLine;
			pInfo->m_dPrice = dPrice;

			break;
		}
	}
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CSubGraphDataBase::DeleteHorizontalLine(long nIndex)
{
	for(int i = 0; i < m_arrHorzLine.GetSize(); i++)
	{
		ST_HORZLINE* pInfo = m_arrHorzLine.GetAt(i);
		if(pInfo->m_nIndex == (UINT)nIndex)
		{
			delete pInfo;
			m_arrHorzLine.RemoveAt(i);

			break;
		}
	}
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CSubGraphDataBase::DeleteAllHorizontalLine()
{
	for(int i = 0; i < m_arrHorzLine.GetSize(); i++)
	{
		ST_HORZLINE* pInfo = m_arrHorzLine.GetAt(i);
		delete pInfo;
	}

	m_arrHorzLine.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
// class CSubGraphDataBase

CSubGraphData::CSubGraphData() : CSubGraphDataBase()
{
	m_pGraphCache = NULL;
}


// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
// 기준선 두께, 모양 설정 - ojtaso (20071213)
CSubGraphData::CSubGraphData( const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle)
	: CSubGraphDataBase( type, style, drawStyle, name, title, hiding, color, penThickness, paintStyle, baseLineStyle)
{
	m_pGraphCache = NULL;
}
CSubGraphData::CSubGraphData( const CSubGraphData& data)
	: CSubGraphDataBase( data)
{
	m_pGraphCache = NULL;
}
CSubGraphData::CSubGraphData( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName)
	: CSubGraphDataBase( p_szSubGraphData, p_hOcxWnd, p_szIndicatorName)
{
	m_pGraphCache = NULL;
}

CSubGraphData::~CSubGraphData()
{
	ClearCalcDataForDraw();
}

void CSubGraphData::ClearCalcDataForDraw( void)
{
	if( m_pGraphCache)
	{
		delete m_pGraphCache;
		m_pGraphCache = NULL;
	}
}

bool CSubGraphData::GetHiding() const
{
	return m_bHiding;
}

CSubGraphData& CSubGraphData::operator=(const CSubGraphData& data)
{
	if( this == &data) return *this;
	*( CSubGraphDataBase *)this = *( CSubGraphDataBase *)&data;
	return *this;
}
