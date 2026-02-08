// CoordinateCalculatorData.cpp: implementation of the CCoordinateCalculatorData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoordinateCalculatorData.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_Chart/DataMath.h"

#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"	// for IPacketListManager

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CVertScaleType

CVertScaleType& CVertScaleType::operator=(const CVertScaleType& vertScaleType)
{
	if(this == &vertScaleType)
		return *this;

	m_bIsLog = vertScaleType.IsLog();
	m_bIsInvert = vertScaleType.IsInvert();
	return *this;
}

CVertScaleType::CVertScaleType():
	m_bIsLog(false),
	m_bIsInvert(false)
{
}

CVertScaleType::CVertScaleType(const CVertScaleType& vertScaleType):
	m_bIsLog(vertScaleType.IsLog()),
	m_bIsInvert(vertScaleType.IsInvert())
{
}

CVertScaleType::CVertScaleType(const bool bIsLog, const bool bIsInvert):
	m_bIsLog(bIsLog),
	m_bIsInvert(bIsInvert)
{
}

void CVertScaleType::SetScaleType(const bool bIsLog, const bool bIsInvert)
{
	m_bIsLog = bIsLog;
	m_bIsInvert = bIsInvert;
}

void CVertScaleType::SetScaleLogType(const bool bIsLog)
{
	m_bIsLog = bIsLog;
}

void CVertScaleType::SetScaleInverType(const bool bIsInvert)
{
	m_bIsInvert = bIsInvert;
}

// ----------------------------------------------------------------------------
bool CVertScaleType::IsLog() const
{
	return m_bIsLog;
}

bool CVertScaleType::IsInvert() const
{
	return m_bIsInvert;
}

// ----------------------------------------------------------------------------
double CVertScaleType::ApplyLog(const double& dData) const
{
	if(!m_bIsLog)
		return dData;
	
	return CMath::Log(dData);
}

double CVertScaleType::ApplyExp(const double& dData) const
{
	if(!m_bIsLog)
		return dData;

	return CMath::Exp(dData);
}


///////////////////////////////////////////////////////////////////////////////
// class CDisplayDataVertMinMax

CDisplayDataVertMinMax& CDisplayDataVertMinMax::operator=(const CDisplayDataVertMinMax& data)
{
	if(this == &data)
		return *this;

	m_dMin = data.GetMin();
	m_dMax = data.GetMax();
	m_vertScaleType = data.GetVertScaleType();
	return *this;
}

CDisplayDataVertMinMax::CDisplayDataVertMinMax():
	m_dMin(0.0),
	m_dMax(0.0)
{
}

CDisplayDataVertMinMax::CDisplayDataVertMinMax(const double& dMin, const double& dMax, const CVertScaleType& vertScaleType):
	m_dMin(dMin),
	m_dMax(dMax),
	m_vertScaleType(vertScaleType)
{
}

void CDisplayDataVertMinMax::SetMinMax(const double& dMin, const double& dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;
}

void CDisplayDataVertMinMax::NormalizeMinMax()
{
	if(m_dMax >= m_dMin)
		return;

	double dTemp = m_dMax;
	m_dMax = m_dMin;
	m_dMin = dTemp;
}

// ----------------------------------------------------------------------------
double CDisplayDataVertMinMax::GetMin() const
{
	return m_dMin;
}

double CDisplayDataVertMinMax::GetMax() const
{
	return m_dMax;
}

CVertScaleType CDisplayDataVertMinMax::GetVertScaleType() const
{
	return m_vertScaleType;
}

CVertScaleType& CDisplayDataVertMinMax::GetVertScaleType()
{
	return m_vertScaleType;
}

double CDisplayDataVertMinMax::GetMin_ScaleType() const
{
	double min = m_dMin;
	if(m_vertScaleType.IsInvert())
		min = m_dMax;
	
	return m_vertScaleType.ApplyLog(min);
}

double CDisplayDataVertMinMax::GetMax_ScaleType() const
{
	double max = m_dMax;
	if(m_vertScaleType.IsInvert())
		max = m_dMin;

	return m_vertScaleType.ApplyLog(max);
}


///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculatorData

CCoordinateCalculatorData& CCoordinateCalculatorData::operator=(const CCoordinateCalculatorData& data)
{
	if(this == &data)
		return *this;

	m_region = data.GetRegion();
	m_nDisplayDataStart = data.GetDisplayDataStart();
	m_nDisplayDataEnd = data.GetDisplayDataEnd();
	m_displayDataVertMinMax = data.GetDisplayDataVertMinMax();
	m_dVertDataType = data.GetVertDataType();

	if( m_pPacket_x) m_pPacket_x->Release();
	m_pPacket_x = data.GetPacket_X();
	
	if( m_pPacket_y) m_pPacket_y->Release();
	m_pPacket_y = data.GetPacket_Y();
	
	if(data.m_blockIndex.IsAllValueBiggerThanZero())
	{
		m_blockIndex.SetRow(data.m_blockIndex.GetRow());
		m_blockIndex.SetColumn(data.m_blockIndex.GetColumn());
	}

	// (2009/9/25 - Seung-Won, Bae) Manage AddIn Interfaces.
	if( m_pIChartManager) m_pIChartManager->Release();			// XScale : 관련 Interface로 사용 - vntsorl(20070327)
	m_pIChartManager = data.m_pIChartManager;
	if( m_pIChartManager) m_pIChartManager->AddRef();
	if( m_pIChartOCX) m_pIChartOCX->Release();
	m_pIChartOCX = data.m_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();
	if( m_pIPacketListManager) m_pIPacketListManager->Release();
	m_pIPacketListManager = data.m_pIPacketListManager;
	if( m_pIPacketListManager) m_pIPacketListManager->AddRef();
	if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->Release();
	m_pIDefaultPacketManager = data.m_pIDefaultPacketManager;
	if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->AddRef();

	// (2009/10/15 - Seung-Won, Bae) Support RQ.
	m_strRQ = data.m_strRQ;

	return *this;
}

CCoordinateCalculatorData::CCoordinateCalculatorData( const CCoordinateCalculatorData &data)
{
	m_region = data.GetRegion();
	m_nDisplayDataStart = data.GetDisplayDataStart();
	m_nDisplayDataEnd = data.GetDisplayDataEnd();
	m_displayDataVertMinMax = data.GetDisplayDataVertMinMax();
	m_dVertDataType = data.GetVertDataType();
	
	m_pPacket_x = data.GetPacket_X();
	m_pPacket_y = data.GetPacket_Y();
	m_blockIndex.SetRow(data.m_blockIndex.GetRow());
	m_blockIndex.SetColumn(data.m_blockIndex.GetColumn());

	// (2009/9/25 - Seung-Won, Bae) Manage AddIn Interfaces.
	m_pIChartManager = data.m_pIChartManager;
	if( m_pIChartManager) m_pIChartManager->AddRef();
	m_pIChartOCX = data.m_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();
	m_pIPacketListManager = data.m_pIPacketListManager;
	if( m_pIPacketListManager) m_pIPacketListManager->AddRef();
	m_pIDefaultPacketManager = data.m_pIDefaultPacketManager;
	if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->AddRef();

	// (2009/10/15 - Seung-Won, Bae) Support RQ.
	m_strRQ = data.m_strRQ;
}

CCoordinateCalculatorData::CCoordinateCalculatorData( IChartManager *p_pIChartManager):
	m_region(0, 0, 0, 0),
	m_nDisplayDataStart(0),
	m_nDisplayDataEnd(0),
	m_dVertDataType(1.0),
	m_pPacket_x(NULL),
	m_pPacket_y(NULL)
{
	// (2009/9/25 - Seung-Won, Bae) Manage AddIn Interfaces.
	m_pIChartManager			= p_pIChartManager;
	m_pIChartOCX				= NULL;
	m_pIPacketListManager		= NULL;
	m_pIDefaultPacketManager	= NULL;
	if( m_pIChartManager)
	{
		m_pIChartManager->AddRef();
		m_pIChartOCX				= m_pIChartManager->GetIChartOCX();
	}
	if( m_pIChartOCX)
	{
		m_pIPacketListManager		= m_pIChartOCX->GetIPacketListManager();
		m_pIDefaultPacketManager	= m_pIChartOCX->GetIDefaultPacketManager();
	}

	// (2009/10/15 - Seung-Won, Bae) Support RQ.
	m_strRQ = "DEFAULT";
}

CCoordinateCalculatorData::~CCoordinateCalculatorData()
{
	if( m_pPacket_x) m_pPacket_x->Release();		// 가로 scale <ex:자료일자> packet
	if( m_pPacket_y) m_pPacket_y->Release();		// 세로 scale <ex:종가> packet

	// (2009/9/25 - Seung-Won, Bae) Manage AddIn Interfaces.
	if( m_pIChartManager) m_pIChartManager->AddRef();
	if( m_pIChartOCX) m_pIChartOCX->AddRef();
	if( m_pIPacketListManager) m_pIPacketListManager->AddRef();
	if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->AddRef();
	m_pIChartManager			= NULL;
	m_pIChartOCX				= NULL;
	m_pIPacketListManager		= NULL;
	m_pIDefaultPacketManager	= NULL;
}

void CCoordinateCalculatorData::SetRegion(const CRect& region)
{
	m_region = region;
}

void CCoordinateCalculatorData::SetDisplayDataStartEnd(const int start, const int end)
{
	m_nDisplayDataStart = start;
	m_nDisplayDataEnd = end;
}

void CCoordinateCalculatorData::SetVertDataType(const double& dDataType)
{
	m_dVertDataType = dDataType;
}

void CCoordinateCalculatorData::SetPacket_X(IPacket* pDataPacket)
{
	if( m_pPacket_x) m_pPacket_x->Release();
	m_pPacket_x = pDataPacket;
	if( m_pPacket_x) m_pPacket_x->AddRef();
}

void CCoordinateCalculatorData::SetPacket_Y(IPacket* pClosePacket)
{
	if( m_pPacket_y) m_pPacket_y->Release();
	m_pPacket_y = pClosePacket;
	if( m_pPacket_y) m_pPacket_y->AddRef();
}

// ----------------------------------------------------------------------------
CRect CCoordinateCalculatorData::GetRegion() const
{
	return m_region;
}

int CCoordinateCalculatorData::GetDisplayDataStart() const
{
	return m_nDisplayDataStart;
}

int CCoordinateCalculatorData::GetDisplayDataEnd() const
{
	return m_nDisplayDataEnd;
}

int CCoordinateCalculatorData::GetDisplayDataCount() const
{
	return (m_nDisplayDataEnd - m_nDisplayDataStart +1);
}

double CCoordinateCalculatorData::GetDisplayDataMin_ScaleType() const
{
	return m_displayDataVertMinMax.GetMin_ScaleType();
}

double CCoordinateCalculatorData::GetDisplayDataMax_ScaleType() const
{
	return m_displayDataVertMinMax.GetMax_ScaleType();
}

CDisplayDataVertMinMax CCoordinateCalculatorData::GetDisplayDataVertMinMax() const
{
	return m_displayDataVertMinMax;
}

CDisplayDataVertMinMax& CCoordinateCalculatorData::GetDisplayDataVertMinMax()
{
	return m_displayDataVertMinMax;
}

CVertScaleType CCoordinateCalculatorData::GetVertScaleType() const
{
	return m_displayDataVertMinMax.GetVertScaleType();
}

double CCoordinateCalculatorData::GetVertDataType() const
{
	return m_dVertDataType;
}

IPacket* CCoordinateCalculatorData::GetPacket_X() const
{
	if( m_pPacket_x) m_pPacket_x->AddRef();
	return m_pPacket_x;
}

IPacket* CCoordinateCalculatorData::GetPacket_Y() const
{
	if( m_pPacket_y) m_pPacket_y->AddRef();
	return m_pPacket_y;
}

// ----------------------------------------------------------------------------
bool CCoordinateCalculatorData::DoesCalculator(const CPoint& point) const
{
	if(m_pPacket_x == NULL || GetDisplayDataCount() <= 0)
		return false;
	return (m_region.left <= point.x && m_region.right >= point.x);
}

double CCoordinateCalculatorData::ApplyLog(const double& data) const
{
	return m_displayDataVertMinMax.GetVertScaleType().ApplyLog(data);
}

double CCoordinateCalculatorData::ApplyExp(const double& data) const
{
	return m_displayDataVertMinMax.GetVertScaleType().ApplyExp(data);
}

void CCoordinateCalculatorData::GetCoordinateCalculatorData( const CPoint& point)
{
	// (2009/9/25 - Seung-Won, Bae) Manage AddIn Interfaces.
	if( !IsValidAddInInterface()) return;

	if( m_pIPacketListManager->IsBasePacketString())
	{
		int nRow, nColumn;
		if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn)) return;
		CBlockIndex blockIndex( nRow, nColumn);
		if(!blockIndex.IsAllValueBiggerThanZero()) return;
		
		GetCoordinateCalculatorData( blockIndex);
	}
	else
	{
		// XScale : 관련 Interface로 사용 - vntsorl(20070327)
		int nRow = -1, nColumn = -1;
		if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn)) return;
		if( !CBlockIndex( nRow, nColumn).IsAllValueBiggerThanZero()) return;
		m_blockIndex.SetIndex( nRow, nColumn);
		
		GetCoordinateCalculatorData( m_blockIndex);
	}
}

void CCoordinateCalculatorData::GetCoordinateCalculatorData( const CBlockIndex& bi)
{
	// (2009/9/25 - Seung-Won, Bae) Manage AddIn Interfaces.
	if( !IsValidAddInInterface()) return;

	SetBlockIndex( bi);

	IBlock *pIBlock = m_pIChartManager->GetBlock( bi.GetRow(), bi.GetColumn());
	if( !pIBlock) return;

	// graph drawing region
	SetRegion( pIBlock->GetGraphDrwingRegion());

	// (2009/10/15 - Seung-Won, Bae) Support the coordinate packets of first vertical scale with graph selection and trace close from point.
	BOOL bLog = FALSE, bInvert = FALSE;
	double dMaxY = -DBL_MAX, dMinY = DBL_MAX, dDataType = 1.0;
	IPacket *pIPacketX = NULL;
	IPacket *pIPacketY = NULL;
	ILPCSTR szRQ = pIBlock->GetCoordinateInfos( pIPacketX, pIPacketY, dMinY, dMaxY, dDataType, bLog, bInvert);

	m_strRQ = szRQ;

	SetPacket_X( pIPacketX);
	SetPacket_Y( pIPacketY);

	GetDisplayDataVertMinMax().SetMinMax( dMinY, dMaxY);

	// (2008/2/26 - Seung-Won, Bae) Get invert type from first visible VScale group.
	// 세로 scale log/invert type
	GetDisplayDataVertMinMax().GetVertScaleType().SetScaleType(	( bLog ? true : false), ( bInvert ? true : false));

	// 세로 scale의 data type
	SetVertDataType( dDataType);

	if( pIPacketX) pIPacketX->Release();
	if( pIPacketY) pIPacketY->Release();
	pIBlock->Release();

	int nStartIndex = -1, nEndIndex = -1;
	// 화면의 시작/끝 위치
	if( m_pIPacketListManager->IsBasePacketString())
	{
		if( !m_pIChartManager->GetDataIndexRangeInView( nStartIndex, nEndIndex)) return;
	}
	else
	{
		if( !m_pIChartManager->GetStartEndDrawIdx_Cur( m_strRQ, nStartIndex, nEndIndex)) return;
	}
	SetDisplayDataStartEnd( nStartIndex, nEndIndex);
}

CBlockIndex& CCoordinateCalculatorData::GetBlockIndex()
{
	return m_blockIndex;
}

CBlockIndex CCoordinateCalculatorData::GetBlockIndex() const
{
	return m_blockIndex;
}

bool CCoordinateCalculatorData::SetBlockIndex(const CPoint& point)
{
	// (2009/9/25 - Seung-Won, Bae) Manage AddIn Interfaces.
	if( !IsValidAddInInterface()) return false;

	return ( m_pIChartManager->GetBlockIndex(point, GetBlockIndex().GetRow(), GetBlockIndex().GetColumn()) ? true : false);
}

bool CCoordinateCalculatorData::SetBlockIndex(const CBlockIndex& blockIndex)
{
	if(blockIndex.IsAllValueBiggerThanZero())
	{
		m_blockIndex.SetRow(blockIndex.GetRow());
		m_blockIndex.SetColumn(blockIndex.GetColumn());
		return true;
	}

	return false;
}