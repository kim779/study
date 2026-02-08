// AnalysisData.cpp: implementation of the CAnalysisData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnalysisData.h"

#include "../Include_Addin_133101/I133101/_IPacketListManager.h"		// for IPacketListManager

#include "limits.h"
#include "MessageDefine.h"
#include "CoordinateCalculator.h"
#include "ElementFactory.h"
#include "ElementTreeBuilder.h"

#include "Element.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CAnalysisData::m_strAddInItemName = "ANALYSIS_TOOL";
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CAnalysisData

CAnalysisData::CAnalysisData( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode):
	m_pParent(NULL),
	m_pIChartManager(NULL),
	m_pIDefaultPacketManager(NULL),
	m_eCurrentToolType(CAnalysis::EN_ANALYSIS_NONE),
	m_elementTree( p_hOcxWnd, p_eChartMode)
{
	m_pIChartOCX = NULL;
	GetUndoRedoManager().SetAnalysisData(this);

	// 20081007 JS.Kim	사용자시간대 선택
	m_pIPacketListManager = NULL;
}

CAnalysisData::~CAnalysisData()
{
	if( m_pIChartManager) 
		m_pIChartManager->Release();
	if( m_pIDefaultPacketManager) 
		m_pIDefaultPacketManager->Release();
	if( m_pIChartOCX) 
		m_pIChartOCX->Release();
	// 20081007 JS.Kim	사용자시간대 선택
	if( m_pIPacketListManager) m_pIPacketListManager->Release();
}

// public =====================================================================
void CAnalysisData::SetParent(CWnd* pWnd)
{
	m_pParent = pWnd;
}

void CAnalysisData::SetMainBlock(IChartManager* pMainBlock, IChartOCX *p_pIChartOCX)
{
	if( m_pIChartManager) m_pIChartManager->Release();
	m_pIChartManager = pMainBlock;
	if( m_pIChartManager) m_pIChartManager->AddRef();

	if( m_pIDefaultPacketManager) m_pIDefaultPacketManager->Release();	
	m_pIDefaultPacketManager = p_pIChartOCX->GetIDefaultPacketManager();

	if( m_pIChartOCX) m_pIChartOCX->Release();
	m_pIChartOCX = p_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();

	// 20081007 JS.Kim	사용자시간대 선택
	if( m_pIPacketListManager) m_pIPacketListManager->Release();
	m_pIPacketListManager = m_pIChartOCX->GetIPacketListManager();
}

void CAnalysisData::SetCurrentToolType(const CAnalysis::TOOLTYPE eToolType)
{
	m_eCurrentToolType = eToolType;
}

// ----------------------------------------------------------------------------
CWnd* CAnalysisData::GetParent() const
{
	return m_pParent;
}

IChartManager* CAnalysisData::GetMainBlock() const
{
	if( m_pIChartManager) 
		m_pIChartManager->AddRef();	
	return m_pIChartManager;
}

IPacketManager* CAnalysisData::GetPacketManager() const
{
	if( m_pIDefaultPacketManager) 
		m_pIDefaultPacketManager->AddRef();
	return m_pIDefaultPacketManager;
}

IChartOCX* CAnalysisData::GetChartOCX() const
{
	if( m_pIChartOCX) 
		m_pIChartOCX->AddRef();
	return m_pIChartOCX;
}

CAnalysis::TOOLTYPE CAnalysisData::GetCurrentToolType() const
{
	return m_eCurrentToolType;
}

const CElementTree& CAnalysisData::GetElementTree() const
{
	return m_elementTree;
}

CElementTree& CAnalysisData::GetElementTree()
{
	return m_elementTree;
}

// ----------------------------------------------------------------------------
CString CAnalysisData::GetOCXPath() const
{
	if(m_pIChartOCX == NULL) return "";
	ILPCSTR szHtsPath = m_pIChartOCX->GetHtsPathString();
	return CString( szHtsPath);
}

CString CAnalysisData::GetUserPath() const
{
	if(m_pIChartOCX == NULL) return "";
	ILPCSTR szUserFilePath = m_pIChartOCX->GetUserFilePathString();
	return CString( szUserFilePath);
}

CDisplayAttributes CAnalysisData::GetDisplayAttributes() const
{
	if(m_pIChartManager == NULL)
		return CDisplayAttributes();

	int nDataStartInBlock = 0, nDataEndInBlock = 0;
	// XScale : 영역의 시작/끝 인덱스 구하기 - vntsorl(20070327)
//	m_pIChartManager->GetDataIndexRangeInView( nDataStartInBlock, nDataEndInBlock);
	m_pIChartManager->GetStartEndDrawIdx_Cur("DEFAULT", nDataStartInBlock, nDataEndInBlock);
	return CDisplayAttributes(nDataStartInBlock, nDataEndInBlock);
}

bool CAnalysisData::GetVertScaleType(const CPoint& point, bool& bIsLog, bool& bIsInvert) const
{
	CBlockIndex currentBlockIndex = GetBlockIndexInPoint(point);
	if(!currentBlockIndex.IsAllValueBiggerThanZero())
		return false;

	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn)) return false;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
	if( !pIBlock) return false;

	// (2008/2/26 - Seung-Won, Bae) Check invert type in first visible VScale
	bIsLog = ( pIBlock->IsLogScale() ? true : false);
	bIsInvert = false;
	int nIndex = pIBlock->GetFirstVisibleVScaleIndex();
	if( 0 <= nIndex) bIsInvert = ( pIBlock->IsVScaleInvert( nIndex) ? true : false);
	pIBlock->Release();
	pIBlock = NULL;

	return ( 0 <= nIndex);
}

bool CAnalysisData::GetVertScaleRegion(CRect& rcLeft,CRect& rcRight)
{
	if(m_pIChartManager == NULL)
		return false;

	rcLeft.SetRectEmpty();
	rcRight.SetRectEmpty();
	IBlock *pBlock = m_pIChartManager->GetBlock(0,0);
	pBlock->GetVertScaleRegion(rcLeft,rcRight);	
	pBlock->Release();

	return true;
}

bool CAnalysisData::GetHorzScaleRegion(CRect& rcTop,CRect& rcBottom)
{
	if(m_pIChartManager == NULL)
		return false;

	rcTop.SetRectEmpty();
	rcBottom.SetRectEmpty();
	CRect rcTopTemp;
	CRect rcBottomTemp;	
	int nRowCount = m_pIChartManager->GetRowCount();
	int nColCount = m_pIChartManager->GetColumnCount();
	for(int i=0;i<nRowCount;i++)
	{
		for(int j=0;j<nColCount;j++)
		{
			IBlock *pBlock = m_pIChartManager->GetBlock(i,j);
			pBlock->GetHorzScaleRegion(rcTopTemp,rcBottomTemp);	
			pBlock->Release();

			if(rcTopTemp.IsRectEmpty() == FALSE)
			{
				rcTop.CopyRect(rcTopTemp);
			}

			if(rcBottomTemp.IsRectEmpty() == FALSE)
			{
				rcBottom.CopyRect(rcBottomTemp);
			}
		}
	}	
	
	return true;
}

CCoordinateCalculatorData CAnalysisData::GetCoordinateCalculatorData(const CPoint& point) const
{
	if( m_pIChartManager == NULL) CCoordinateCalculatorData( NULL);

	CCoordinateCalculatorData calData( m_pIChartManager);
	calData.GetCoordinateCalculatorData( point);
	return calData;
}

CBlockIndex CAnalysisData::GetBlockIndexInPoint(const CPoint& point) const
{
	if(m_pIChartManager == NULL)
		return CBlockIndex();

	CBlockIndex bi;
	int nRow, nColumn;
	if( m_pIChartManager->GetBlockIndex(point, nRow, nColumn))
	{ 
		bi.SetRow( nRow);
		bi.SetColumn( nColumn);
	}
	return bi;
}

CBlockIndex CAnalysisData::GetBlockIndexInElement(long lElementAddress) const
{
	const CElement* pSelectedElement = (const CElement*)lElementAddress;
	if(pSelectedElement == NULL)
		return CBlockIndex();

	return m_elementTree.GetBlockIndexInElement(pSelectedElement);
}

bool CAnalysisData::PointInGraphRegion(const CPoint& point) const
{
	if(m_pIChartManager == NULL)
		return false;

	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn)) return false;
	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
	if( !pIBlock) return false;
	CRect graphRegion, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( graphRegion, rctNoFrameAndMargin);
	pIBlock->Release();
	pIBlock = NULL;

	return ( graphRegion.PtInRect(point) ? true : false);
}

// ----------------------------------------------------------------------------
// ocx에 메시지 보내기.
void CAnalysisData::SendMessageToParent(const CCursorShape::CHARTOBJECT cursorShape)
{
	if(m_pParent == NULL)
		return;

	m_pParent->SendMessage(UM_CURSOR_EXCHANGE, cursorShape);
}

// 화면을 다시 그리고 커서를 바꾸기.
void CAnalysisData::InvalidateAndSendMessageToParent(const CCursorShape::CHARTOBJECT cursorShape)
{
	Invalidate();
	SendMessageToParent(cursorShape);
}

// 화면을 다시 그리기
void CAnalysisData::Invalidate()
{
	if(m_pParent == NULL)
		return;

	m_pParent->Invalidate();
}

// element 그리기.
void CAnalysisData::DrawElement(CDC* pDC, CElement* pSelectedElement)
{
	if(m_pIChartManager == NULL)
		return;

	m_elementTree.DrawElement(pDC, pSelectedElement, m_pIChartManager);
}

CElement *CAnalysisData::CopyElement(CElement* pSelectedElement)
{
	if(pSelectedElement == NULL)
		return NULL;

	//>> (2008/12/17 - Seung-Lyong Park) 추세선 역순지우기 및 UndoRedo
	return m_elementTree.CopyElement(pSelectedElement);
	//<< (2008/12/17 - Seung-Lyong Park) 추세선 역순지우기 및 UndoRedo
}

// 20081007 JS.Kim	사용자시간대 선택
IPacket *CAnalysisData::GetDatePacket( const char *p_szRQ) const
{
	if( !m_pIPacketListManager) return NULL;
	return m_pIPacketListManager->GetDatePacket( p_szRQ);
}

// ----------------------------------------------------------------------------
// Undo / Redo
const CUndoRedoManager& CAnalysisData::GetUndoRedoManager() const
{
	return m_UndoRedoManager;
}

CUndoRedoManager& CAnalysisData::GetUndoRedoManager()
{
	return m_UndoRedoManager;
}

BOOL CAnalysisData::MoveElement(CElement* pSelectedElement, CString strElementData_Prev)
{
	if(pSelectedElement == NULL)
		return FALSE;

	// Undo / Redo
	GetUndoRedoManager().RegisterElementToUnRedoManager(URT_MOVE, (long)pSelectedElement, strElementData_Prev);

	return TRUE;
}

// ----------------------------------------------------------------------------
// block 추가시 elementTree을 추가하기.
bool CAnalysisData::InsertElementTree(const CBlockIndex& insertBlockIndex)
{
	if(m_pIChartManager == NULL)
		return false;

	return m_elementTree.InsertElementTree(m_pIChartManager->GetBlockType(), insertBlockIndex);
}

void CAnalysisData::RegisterElementToOrderManager(const long lElementAdress)
{	
	if(lElementAdress == 0)
		return;	
	
	IAddInToolMgr* pAddInToolManager = m_pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER,
															m_strAddInItemName,lElementAdress);
	pAddInToolManager->Release();
}

void CAnalysisData::UnRegisterElementToOrderManager(const long lElementAdress)
{	
	IAddInToolMgr* pAddInToolManager = m_pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER,
															m_strAddInItemName,lElementAdress);
	pAddInToolManager->Release();
}

///////////////////////////////////////////////////////////////////////////////
// class CDrawingRegions

CDrawingRegions& CDrawingRegions::operator=(const CDrawingRegions& drawingRegions)
{
	if(this == &drawingRegions)
		return *this;

	m_blockRegionList = drawingRegions.GetBlockRegionList();
	return *this;
}

CDrawingRegions::CDrawingRegions()
{
}

// public =====================================================================
void CDrawingRegions::Initialize()
{
	m_blockRegionList.RemoveAll();
	m_blockRegionList.SetCurrentNumber(-1);
}

// ----------------------------------------------------------------------------
// 그리기에 필요한 영역들을 setting.
void CDrawingRegions::SetDrawingRegions(IChartManager* pMainBlock, const CPoint& point)
{
	// 영역 범위체크 하지 않고 영역 찾기
	if(!DoseGetDrawingRegionAgain(point))
		return;

	Initialize();
	SetDrawingRegions_Column(pMainBlock, point);
}

void CDrawingRegions::SetDrawingRegions(IChartManager* pMainBlock, const int columnIndex)
{
	Initialize();
	SetDrawingRegions_Column(pMainBlock, columnIndex);
}

// ----------------------------------------------------------------------------
CBlockRegionList CDrawingRegions::GetBlockRegionList() const
{
	return m_blockRegionList;
}

CBlockRegionList& CDrawingRegions::GetBlockRegionList()
{
	return m_blockRegionList;
}

CBlockRegion CDrawingRegions::GetBlockRegion(const int nIndex) const
{
	return m_blockRegionList.GetAt(nIndex);
}

CGraphPartRegion CDrawingRegions::GetGraphPartRegion(const int nIndex) const
{
	return m_blockRegionList.GetAt(nIndex).GetGraphPartRegion();
}

CGraphPartRegion CDrawingRegions::GetCurrentGraphPartRegion() const
{
	return m_blockRegionList.GetCurrentAt().GetGraphPartRegion();
}

int CDrawingRegions::GetRowNumber(const CPoint& point) const
{
	return m_blockRegionList.GetNumber(point);
}

int CDrawingRegions::GetCount() const
{
	return m_blockRegionList.GetCount();
}

CRect CDrawingRegions::GetFullGraphRegion(const int nIndex) const
{
	return GetBlockRegion(nIndex).GetFullGraphRegion();
}

CRect CDrawingRegions::GetFullGraphRegion(const CPoint& point) const
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CRect region = GetBlockRegion(nIndex).GetFullGraphRegion();
		if(region.PtInRect(point))
			return region;
	}
	return CRect(0, 0, 0, 0);
}

CRect CDrawingRegions::GetCurrentFullGraphRegion() const
{
	return GetCurrentGraphPartRegion().GetFull();
}

CRect CDrawingRegions::GetUnmixedGraphRegion(const int nIndex) const
{
	return GetBlockRegion(nIndex).GetUnmixedGraphRegion();
}

CRect CDrawingRegions::GetUnmixedGraphRegion(const CPoint& point) const
{
	for(int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CRect region = GetBlockRegion(nIndex).GetUnmixedGraphRegion();
		if(region.PtInRect(point))
			return region;
	}
	return CRect(0, 0, 0, 0);
}

CRect CDrawingRegions::GetCurrentUnmixedGraphRegion() const
{
	return m_blockRegionList.GetCurrentAt().GetUnmixedGraphRegion();
}

// ----------------------------------------------------------------------------
bool CDrawingRegions::PointInDrawingRegionsInColumn(const CPoint& point) const
{
	CRect region = GetFullGraphRegion(point);
	if(region.IsRectEmpty())
		return false;
	
	return ( region.PtInRect(point) ? true : false);
}

bool CDrawingRegions::PointInCurrentRegion(const CPoint& point) const
{
	return ( GetCurrentFullGraphRegion().PtInRect(point) ? true : false);
}


// private ====================================================================
// 영역을 다시 구할지의 여부
bool CDrawingRegions::DoseGetDrawingRegionAgain(const CPoint& point)
{
	if(GetCount() <= 0)
		return true;

	m_blockRegionList.SetCurrentNumber(point);
	CRect region = m_blockRegionList.GetFullGraphRegionSumInList();
	return (!region.PtInRect(point));
}

// ----------------------------------------------------------------------------
// 그리는 영역 setting. -> point 이용 : 현재 사용되는 row 번호 저장.
void CDrawingRegions::SetDrawingRegions_Column(IChartManager* pMainBlock, const CPoint& point)
{
	if(pMainBlock == NULL)
		return;

	// tool에 관련된 영역이 해당 point의 column 영역에 포함되는지를 보고 
	// column 영역에 포함되지 않을 경우만 찾는다.
	CRect totalGraphRegion = m_blockRegionList.GetFullGraphRegionSumInList();
	if(totalGraphRegion.IsRectEmpty() && totalGraphRegion.PtInRect(point))
		return;

	SetDrawingRegions_Row(pMainBlock, point);
}

void CDrawingRegions::SetDrawingRegions_Row(IChartManager* pMainBlock, const CPoint& point)
{
	if(pMainBlock == NULL)
		return;

	m_blockRegionList.GetBlockRegionList(point, pMainBlock);
	m_blockRegionList.SetCurrentNumber(point);
}

// 그리는 영역 setting. -> column number 이용 : 현재 사용되는 row 없음.
void CDrawingRegions::SetDrawingRegions_Column(IChartManager* pMainBlock, const int columnIndex)
{
	if(columnIndex < 0 || columnIndex >= pMainBlock->GetColumnCount())
		return;

	SetDrawingRegions_Row(pMainBlock, columnIndex);
}

void CDrawingRegions::SetDrawingRegions_Row(IChartManager* pMainBlock, const int columnIndex)
{
	if(pMainBlock == NULL)
		return;

	m_blockRegionList.GetBlockRegionList( columnIndex, pMainBlock);
}

///////////////////////////////////////////////////////////////////////////////
// class CDisplayAttributes

CDisplayAttributes& CDisplayAttributes::operator=(const CDisplayAttributes& displayAttributes)
{
	if(this == &displayAttributes)
		return *this;

	m_nDataStartInBlock = displayAttributes.GetDataStartInBlock();
	m_nDataEndInBlock = displayAttributes.GetDataEndInBlock();

	return *this;
}

CDisplayAttributes::CDisplayAttributes():
	m_nDataStartInBlock(0),
	m_nDataEndInBlock(0)
{
}

CDisplayAttributes::CDisplayAttributes(const int start, const int end):
	m_nDataStartInBlock(start),
	m_nDataEndInBlock(end)
{
}

// public =====================================================================
void CDisplayAttributes::SetDataStartInBlock(const int start)
{
	m_nDataStartInBlock = start;
}

void CDisplayAttributes::SetDataEndInBlock(const int end)
{
	m_nDataEndInBlock = end;
}

// ----------------------------------------------------------------------------
int CDisplayAttributes::GetDataStartInBlock() const
{
	return m_nDataStartInBlock;
}

int CDisplayAttributes::GetDataEndInBlock() const
{
	return m_nDataEndInBlock;
}

int CDisplayAttributes::GetDataCountInBlock() const
{
	return (m_nDataEndInBlock - m_nDataStartInBlock +1);
}

bool CDisplayAttributes::IsAllValueBiggerThanZero() const
{
	return (m_nDataStartInBlock >= 0 && m_nDataEndInBlock >= 0);
}

///////////////////////////////////////////////////////////////////////////////
// class CRegion

CRegion& CRegion::operator=(const CRegion& data)
{
	if(this == &data)
		return *this;

	m_rect = data.GetRect();
	m_displayAttributes = data.GetDisplayAttributes();
	return *this;
}

CRegion::CRegion():
	m_rect(0, 0, 0, 0)
{
}

CRegion::CRegion(const CRegion& region):
	m_rect(region.GetRect()),
	m_displayAttributes(region.GetDisplayAttributes())
{
}

CRegion::CRegion(const CRect& rect, const CDisplayAttributes& displayAttributes):
	m_rect(rect),
	m_displayAttributes(displayAttributes)
{
}

// public =====================================================================
void CRegion::SetRect(const CRect& rect)
{
	m_rect = rect;
}

void CRegion::SetDisplayAttributes(const CDisplayAttributes& displayAttributes)
{
	m_displayAttributes = displayAttributes;
}

CRect CRegion::GetRect() const
{
	return m_rect;
}

CDisplayAttributes CRegion::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

bool CRegion::DoesIndexInDisplayAttributes(const int nIndex) const
{
	return (m_displayAttributes.GetDataStartInBlock() -1 <= nIndex && m_displayAttributes.GetDataEndInBlock() >= nIndex);
}

///////////////////////////////////////////////////////////////////////////////
// class CDisplayData

CDisplayData& CDisplayData::operator=(const CDisplayData& data)
{
	if(this == &data)
		return *this;

	m_eCoordinateType = data.GetCoordinateType();
	m_displayAttributes = data.GetDisplayAttributes();
	m_graphPartRegion = data.GetGraphPartRegion();
	return *this;
}

CDisplayData::CDisplayData():
	m_eCoordinateType(CAnalysis::RELATIVE_POINT)
{
}

CDisplayData::CDisplayData(const CAnalysis::COORDINATETYPE eCoordinateType, 
		const CGraphPartRegion& graphPartRegion, const CDisplayAttributes& displayAttributes):
	m_eCoordinateType(eCoordinateType),
	m_graphPartRegion(graphPartRegion), 
	m_displayAttributes(displayAttributes)
{
}

// public =====================================================================
void CDisplayData::SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType)
{
	m_eCoordinateType = eCoordinateType;
}

void CDisplayData::SetDisplayAttributes(const CDisplayAttributes& displayAttributes)
{
	m_displayAttributes = displayAttributes;
}

void CDisplayData::SetGraphPartRegion(const CGraphPartRegion& graphPartRegion)
{
	m_graphPartRegion = graphPartRegion;
}

bool CDisplayData::IsRelativePointType() const
{
	return (m_eCoordinateType == CAnalysis::RELATIVE_POINT);
}

CAnalysis::COORDINATETYPE CDisplayData::GetCoordinateType() const
{
	return m_eCoordinateType;
}

CDisplayAttributes CDisplayData::GetDisplayAttributes() const
{
	return m_displayAttributes;
}

CGraphPartRegion CDisplayData::GetGraphPartRegion() const
{
	return m_graphPartRegion;
}

CRect CDisplayData::GetDisplayRegion() const
{
	return m_graphPartRegion.GetFull();
}

CRect CDisplayData::GetCoordinateCalRegion() const
{
	return m_graphPartRegion.GetUnmixed();
}

///////////////////////////////////////////////////////////////////////////////
// class CExtractingDataOfModifyType

CExtractingDataOfModifyType& CExtractingDataOfModifyType::operator=(const CExtractingDataOfModifyType& extractingData)
{
	if(this == &extractingData)
		return *this;

	m_nRowSetIndex = extractingData.GetRowSetIndex();
	m_pDrawingRegions = extractingData.GetDrawingRegions();
	m_mousePoint = extractingData.GetMousePoint();
	m_calData = extractingData.GetCoordinateCalculatorData();
	return *this;
}

CExtractingDataOfModifyType::CExtractingDataOfModifyType( IChartManager *p_pIChartManager):
	m_nRowSetIndex(-1),
	m_pDrawingRegions(NULL),
	m_mousePoint(INT_MAX, INT_MAX),
	m_calData( p_pIChartManager)
{
}

CExtractingDataOfModifyType::CExtractingDataOfModifyType(CDrawingRegions* pDrawingRegions, const CPoint& point, IChartManager *p_pIChartManager):
	m_nRowSetIndex(-1),
	m_pDrawingRegions(pDrawingRegions),
	m_mousePoint(point),
	m_calData( p_pIChartManager)
{
}

// public =====================================================================
void CExtractingDataOfModifyType::SetCoordinateCalculatorData(const CCoordinateCalculatorData& calData)
{
	m_calData = calData;
}

void CExtractingDataOfModifyType::SetRowSetIndex(const int nRowSetIndex)
{
	m_nRowSetIndex = nRowSetIndex;
}

int CExtractingDataOfModifyType::GetRowSetIndex() const
{
	return m_nRowSetIndex;
}

// ----------------------------------------------------------------------------
CDrawingRegions* CExtractingDataOfModifyType::GetDrawingRegions() const
{
	return m_pDrawingRegions;
}

CPoint CExtractingDataOfModifyType::GetMousePoint() const
{
	return m_mousePoint;
}

CCoordinateCalculatorData CExtractingDataOfModifyType::GetCoordinateCalculatorData() const
{
	return m_calData;
}

CCoordinateCalculatorData& CExtractingDataOfModifyType::GetCoordinateCalculatorData()
{
	return m_calData;
}

CDisplayAttributes CExtractingDataOfModifyType::GetDisplayAttributes() const
{
	return CDisplayAttributes(m_calData.GetDisplayDataStart(), m_calData.GetDisplayDataEnd());
}

// ----------------------------------------------------------------------------
// 영역들의 갯수
int CExtractingDataOfModifyType::GetDrawingRegionCount() const
{
	if(m_pDrawingRegions == NULL)
		return 0;

	return m_pDrawingRegions->GetCount();
}

// 해당 영역 가져오기
CBlockRegion CExtractingDataOfModifyType::GetBlockRegion(const int nIndex) const
{
	if(m_pDrawingRegions == NULL)
		return CBlockRegion();

	return m_pDrawingRegions->GetBlockRegion(nIndex);
}

CGraphPartRegion CExtractingDataOfModifyType::GetGraphPartRegion(const int nIndex) const
{
	if(m_pDrawingRegions == NULL)
		return CGraphPartRegion();

	return m_pDrawingRegions->GetGraphPartRegion(nIndex);
}

CRect CExtractingDataOfModifyType::GetFullGraphRegion(const int nIndex) const
{
	if(m_pDrawingRegions == NULL)
		return CRect(0, 0, 0, 0);

	return m_pDrawingRegions->GetFullGraphRegion(nIndex);
}

// 현재 mousePoint가 있는 영역 가져오기
CRect CExtractingDataOfModifyType::GetFullGraphRegionInMousePoint() const
{
	if(m_pDrawingRegions == NULL)
		return CRect(0, 0, 0, 0);

	return m_pDrawingRegions->GetCurrentFullGraphRegion();
}

// ----------------------------------------------------------------------------
// element를 찾을지 검사 유무.
bool CExtractingDataOfModifyType::IsFindElement() const
{
	return (m_pDrawingRegions != NULL && m_pDrawingRegions->GetCount() > 0);
}

// mouse point가 element가 있는 region에 속하는지 유무 (== rowSet Region)
bool CExtractingDataOfModifyType::IsPointInRowSetRegion() const
{
	if(!IsFindElement())
		return false;

	return ( m_pDrawingRegions->GetFullGraphRegion(m_nRowSetIndex).PtInRect(m_mousePoint) ? true : false);
}

///////////////////////////////////////////////////////////////////////////////
// class CElementModifyData

CElementModifyData::CElementModifyData():
	m_modifyType(CToolEnumData::MODIFY_NONE),
	m_pIPacket_x(NULL),
	m_pIChartManager(NULL)												// XScale : 관련 Interface로 사용 - vntsorl(20070327)
{
}

CElementModifyData::CElementModifyData(const CElementModifyData& modifyData)
{
	m_modifyType = modifyData.GetModifyType();
	m_pIPacket_x = modifyData.GetPacket_X();
	m_modifyPoint = modifyData.GetPoint();
	m_pIChartManager = modifyData.GetIChartManager();					// XScale : 관련 Interface로 사용 - vntsorl(20070327)
	m_blockIndex.SetRow(modifyData.GetBlockIndex().GetRow());
	m_blockIndex.SetColumn(modifyData.GetBlockIndex().GetColumn());
}

CElementModifyData::~CElementModifyData()
{
	if( m_pIPacket_x) m_pIPacket_x->Release();
	if( m_pIChartManager) m_pIChartManager->Release();					// XScale : 관련 Interface로 사용 - vntsorl(20070327)
}

CElementModifyData& CElementModifyData::operator=(const CElementModifyData& modifyData)
{
	if(this == &modifyData)
		return *this;

	m_modifyType = modifyData.GetModifyType();

	if( m_pIPacket_x) m_pIPacket_x->Release();
	m_pIPacket_x = modifyData.GetPacket_X();

	m_modifyPoint = modifyData.GetPoint();

	if( m_pIChartManager) m_pIChartManager->Release();					// XScale : 관련 Interface로 사용 - vntsorl(20070327)
	m_pIChartManager = modifyData.GetIChartManager();

	m_blockIndex.SetRow(modifyData.GetBlockIndex().GetRow());
	m_blockIndex.SetColumn(modifyData.GetBlockIndex().GetColumn());

	return *this;
}

// public =====================================================================
void CElementModifyData::SetModifyType(const CToolEnumData::ModifyType modifyType)
{
	m_modifyType = modifyType;
}

void CElementModifyData::SetPacket_X(IPacket* pPacket)
{
	if( m_pIPacket_x) m_pIPacket_x->Release();
	m_pIPacket_x = pPacket;
	if( m_pIPacket_x) m_pIPacket_x->AddRef();
}

void CElementModifyData::SetPoint(const CCoordinate& point)
{
	m_modifyPoint = point;
}

// XScale : 관련 Interface로 사용 - vntsorl(20070327)
void CElementModifyData::SetIChartManager( IChartManager* pIChartManager)
{
	if( m_pIChartManager) m_pIChartManager->Release();
	m_pIChartManager = pIChartManager;
	if( m_pIChartManager) m_pIChartManager->AddRef();
}

// ----------------------------------------------------------------------------
CToolEnumData::ModifyType CElementModifyData::GetModifyType() const
{
	return m_modifyType;
}

IPacket* CElementModifyData::GetPacket_X() const
{
	if( m_pIPacket_x) m_pIPacket_x->AddRef();
	return m_pIPacket_x;
}

CCoordinate CElementModifyData::GetPoint() const
{
	return m_modifyPoint;
}

// 변경할 내용의 좌표
CCoordinate CElementModifyData::GetModifyPoint(const CCoordinate& orgPoint) const
{
	switch(m_modifyType)
	{
	case CToolEnumData::MODIFY_CENTER:
		return GetModifyPoint_Center(orgPoint);
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_MIDDLE:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_X:
	case CToolEnumData::MODIFY_Y:
			return m_modifyPoint;
	}

	assert(false);
	return orgPoint;
}

int CElementModifyData::GetPacket_XIndex(double dXData) const
{
	if(m_pIPacket_x == NULL || m_pIPacket_x->GetDataCount() <= 0)
		return -1;

	return m_pIPacket_x->GetIndexFromData(dXData);	
}

double CElementModifyData::GetPacket_XValue(int nXIndex) const
{
	if(m_pIPacket_x == NULL || m_pIPacket_x->GetDataCount() <= 0)
		return -1;

	double dData = 0.0;
	m_pIPacket_x->GetData( nXIndex, dData);	
	return dData;
}
// private ====================================================================
CCoordinate CElementModifyData::GetModifyPoint_Center(const CCoordinate& orgPoint) const
{
//	if(m_pIPacket_x == NULL || m_pIPacket_x->GetDataCount() <= 0)
//		return CCoordinate(orgPoint.GetX() + m_modifyPoint.GetX(), 
//			orgPoint.GetY() + m_modifyPoint.GetY(), 0);
//
//	double dXDate = m_pIPacket_x->GetIndexFromData(orgPoint.GetX());
//	int nIndex(dXDate + m_modifyPoint.GetX());
//	//SetIndex(nIndex); 
//	double dModifyData = 0.0;
//	m_pIPacket_x->GetDataPastandFuture(nIndex,dModifyData);
//	//if(dModifyData == 0)
//		//return orgPoint;
//	
//	return CCoordinate(dModifyData, orgPoint.GetY() + m_modifyPoint.GetY(), nIndex);

	// XScale : 관련 Interface로 사용 - vntsorl(20070410)
	if( !m_pIChartManager)
		return CCoordinate();

	if(m_pIPacket_x == NULL || m_pIPacket_x->GetDataCount() <= 0)
		return CCoordinate(orgPoint.GetX() + m_modifyPoint.GetX(), 
		// 분차트 근접시간 적용 - ojtaso (20080811)
		//orgPoint.GetY() + m_modifyPoint.GetY(), 0);
		// 20081007 JS.Kim	사용자시간대 선택
		//orgPoint.GetY() + m_modifyPoint.GetY(), m_modifyPoint.GetDateType(), 0);
		orgPoint.GetY() + m_modifyPoint.GetY(), m_modifyPoint.GetTimeDiff(), m_modifyPoint.GetDateType(), 0);
	
	int nCenter_x;
	int nOrgXPos = m_pIChartManager->GetXFromDouble(GetBlockIndex().GetRow(), GetBlockIndex().GetColumn(), orgPoint.GetX(), nCenter_x);

	// (2008/10/20 - Seung-Won, Bae) Use Main RQ of current block.
	ILPCSTR szMainRQ = m_pIChartManager->GetMainRQofBlock( GetBlockIndex().GetRow(), GetBlockIndex().GetColumn());

	// (2008/8/12 - Seung-Won, Bae) Do not move to outside as future.
	int nNewXPos = nOrgXPos + m_modifyPoint.GetX();
	double dModifyData = 0.0;
	int nDTI = m_pIChartManager->FindViewDataIndex_Pt( szMainRQ, nNewXPos, GetBlockIndex().GetColumn(), TRUE);
	if( 0 <= nDTI)
	{
		int nBlockColumn;
		dModifyData = m_pIChartManager->GetDoubleFromX(GetBlockIndex().GetRow(), GetBlockIndex().GetColumn(), nNewXPos, &nBlockColumn, nCenter_x);
	}
	// (2008/9/8 - Seung-Won, Bae) Move to bound if moved out.
	else
	{
		nDTI = m_pIChartManager->FindViewDataIndex_Pt( "DEFAULT", nNewXPos, GetBlockIndex().GetColumn());
		nNewXPos = m_pIChartManager->GetXPosition( "DEFAULT", nDTI, GetBlockIndex().GetColumn());
		CElementModifyData *pThis = ( CElementModifyData *)this;	// force to no const.
		pThis->m_modifyPoint.SetX( nNewXPos - nOrgXPos, 0);
	}


	// 2011.02.01 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	// 이동된 날짜시간값(dModifyData)을 현재 선택된 주기의 날짜포맷에 맞게 처리..
	// 이때, 주기별 사용안하는 날짜시간 Data는 유지하도록 처리 필요.
	// 예) 분간 추세선 x값을 일간에서 이동시... (날짜시간 14Byte 포맷 가정)
	// 이동 전 날짜시간 orgPoint.m_dX	: 20101209200000.000 (YYYYMMDDhhmmss 포맷)
	// 이동 후 날짜시간 dModifyData		: 20101206000000.000 (YYYYMMDDhhmmss 에서 hhmmss는 0으로 초기화됨)
	// ==>>
	// dModifyData을 재조정				: 20101206200000.000 (YYYYMMDDhhmmss 에서 hhmmss 부분 다시 살림)
	//

	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)m_pIPacket_x->GetDateUnitType();

	//
	ILPCSTR szPacketType = m_pIPacket_x->GetType();
	CString strType = szPacketType;

	if (strType == _T("YYYYMMDDHHMMSS"))
	{
		CString strOrg = CDataConversion::DoubleToString(orgPoint.GetX());
		CString strMod = CDataConversion::DoubleToString(dModifyData);
		CString strNew = strMod;

		switch(eDateUnit)
		{
		case CScaleBaseData::HORZ_DAILY:
		case CScaleBaseData::HORZ_WEEKLEY:
			// 일간/주간 : 년월일 변경, 시분초 유지
			strNew = strMod.Left(8) + strOrg.Right(6);
			break;
		case CScaleBaseData::HORZ_MONTHLY:		// 월 
			// 월간 : 년월 변경, 일자는 1일로됨, 시분초 유지
			strNew = strMod.Left(8) + strOrg.Right(6);
			break;
		case CScaleBaseData::HORZ_TIME_ONE:
		case CScaleBaseData::HORZ_TIME_TWO:
			// 분간 : 년월일시분 변경, 초는 유지
			strNew = strMod.Left(12) + strOrg.Right(2);
			break;
		case CScaleBaseData::HORZ_TICK:
		case CScaleBaseData::HORZ_TIME_SECOND:
			// 틱초 : 년월일시분초 변경
			strNew = strMod;
			break;
		}
		dModifyData = atof(strNew);
	}
	// 2011.02.01 by SYS <<
	
	// 20081007 JS.Kim	사용자시간대 선택
	return CCoordinate(dModifyData, orgPoint.GetY() + m_modifyPoint.GetY(), m_modifyPoint.GetTimeDiff(), m_modifyPoint.GetDateType()/*m_pIPacket_x->GetType()*/, 0);
}

// XScale : 관련 Interface로 사용 - vntsorl(20070327)
IChartManager* CElementModifyData::GetIChartManager() const
{
	if( m_pIChartManager) m_pIChartManager->AddRef();
	return m_pIChartManager;
}

CBlockIndex& CElementModifyData::GetBlockIndex()
{
	return m_blockIndex;
}

CBlockIndex CElementModifyData::GetBlockIndex() const
{
	return m_blockIndex;
}

bool CElementModifyData::SetBlockIndex(const CPoint& point)
{
	if( m_pIChartManager)
		return ( m_pIChartManager->GetBlockIndex(point, GetBlockIndex().GetRow(), GetBlockIndex().GetColumn()) ? true : false);

	return false;
}

bool CElementModifyData::SetBlockIndex(const CBlockIndex& blockIndex)
{
	if(blockIndex.IsAllValueBiggerThanZero())
	{
		m_blockIndex.SetRow(blockIndex.GetRow());
		m_blockIndex.SetColumn(blockIndex.GetColumn());
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////
// class CSelectedElementDrawingData

CSelectedElementDrawingData& CSelectedElementDrawingData::operator=(const CSelectedElementDrawingData& selectedDrawingData)
{
	if(this == &selectedDrawingData)
		return *this;

	m_eModifyType = selectedDrawingData.GetModifyType();
	m_pDrawingRegions = selectedDrawingData.GetDrawingRegions();
	m_calData = selectedDrawingData.GetCoordinateCalculatorData();
	m_point = selectedDrawingData.GetPoint();
	m_coordinate = selectedDrawingData.GetPoint();
	return *this;
}

CSelectedElementDrawingData::CSelectedElementDrawingData(const CToolEnumData::ModifyType eModifyType, 
		CDrawingRegions* pDrawingRegions, const CCoordinateCalculatorData& calData, const CPoints& points):
	m_eModifyType(eModifyType),
	m_pDrawingRegions(pDrawingRegions),
	m_calData(calData),
	m_point(points.GetDrawingPoint()),
	m_coordinate(points.GetSavingPoint())
{
	
}

// public =====================================================================
CToolEnumData::ModifyType CSelectedElementDrawingData::GetModifyType() const
{
	return m_eModifyType;
}

CDrawingRegions* CSelectedElementDrawingData::GetDrawingRegions()
{
	return m_pDrawingRegions;
}

CDrawingRegions* CSelectedElementDrawingData::GetDrawingRegions() const
{
	return m_pDrawingRegions;
}

const CCoordinateCalculatorData& CSelectedElementDrawingData::GetCoordinateCalculatorData() const
{
	return m_calData;
}

CPoint CSelectedElementDrawingData::GetPoint() const
{
	return m_point;
}

CCoordinate CSelectedElementDrawingData::GetCoordinate() const
{
	return m_coordinate;
}

// ----------------------------------------------------------------------------
// 그릴 좌표
CPoint CSelectedElementDrawingData::GetSelectedDrawPoint(const CAnalysis::COORDINATETYPE& eType, const bool bIsAutoPrice, const CCoordinate& orgPoint) const
{
	if(eType == CAnalysis::RELATIVE_POINT)
		return GetSelectedDrawPoint_Data(bIsAutoPrice, orgPoint);

	return GetSelectedDrawPoint_Pixel(orgPoint);
}

CRect CSelectedElementDrawingData::GetCurrentRegion() const
{
	if(m_pDrawingRegions == NULL)
		return CRect(0, 0, 0, 0);

	return m_pDrawingRegions->GetCurrentFullGraphRegion();
}

CGraphPartRegion CSelectedElementDrawingData::GetCurrentGraphPartRegion() const
{
	if(m_pDrawingRegions == NULL)
		return CGraphPartRegion();

	return m_pDrawingRegions->GetCurrentGraphPartRegion();
}

CRegion CSelectedElementDrawingData::GetRegion() const
{
	return CRegion(GetCurrentRegion(), GetDisplayAttributes());
}

CDisplayAttributes CSelectedElementDrawingData::GetDisplayAttributes() const
{
	return CDisplayAttributes(m_calData.GetDisplayDataStart(), m_calData.GetDisplayDataEnd());
}

CVertScaleType CSelectedElementDrawingData::GetVertScaleType() const
{
	return m_calData.GetVertScaleType();
}

// private ====================================================================
CPoint CSelectedElementDrawingData::GetSelectedDrawPoint_Data(const bool bIsAutoPrice, const CCoordinate& orgPoint) const
{
	if(bIsAutoPrice)
		return GetSelectedDrawPoint_Data_AutoPrice(orgPoint);
	return GetSelectedDrawPoint_Data_NoAutoPrice(orgPoint);
}

CPoint CSelectedElementDrawingData::GetSelectedDrawPoint_Data_AutoPrice(const CCoordinate& orgPoint) const
{
	CCoordinateCalculator& coordinateCalculator = CCoordinateCalculatorFinder::FindCoordinateCalculator(true);
	if(m_eModifyType == CToolEnumData::MODIFY_CENTER)
	{
		CPoint drawingPoint = coordinateCalculator.RelativeToAbsolute(m_calData, orgPoint);
		return coordinateCalculator.AbsoluteToAbsolute(m_calData, CPoint(drawingPoint.x + m_point.x, drawingPoint.y + m_point.y));
	}

	CPoint drawingPoint = coordinateCalculator.AbsoluteToAbsolute(m_calData, m_point);

	switch(m_eModifyType)
	{
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_MIDDLE:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_X:
		return CPoint(m_point.x, drawingPoint.y);
	case CToolEnumData::MODIFY_Y:
		return CPoint(drawingPoint.x, m_point.y);
	}

	assert(false);
	return m_point;
}

CPoint CSelectedElementDrawingData::GetSelectedDrawPoint_Data_NoAutoPrice(const CCoordinate& orgPoint) const
{
	switch(m_eModifyType)
	{
	case CToolEnumData::MODIFY_CENTER:
	{
		CCoordinateCalculator& coordinateCalculator = CCoordinateCalculatorFinder::FindCoordinateCalculator(false);
		CPoint drawingPoint = coordinateCalculator.RelativeToAbsolute(m_calData, orgPoint);
		return CPoint(drawingPoint.x + m_point.x, drawingPoint.y + m_point.y);
	}
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_MIDDLE:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_X:
	case CToolEnumData::MODIFY_Y:
		return m_point;
	}

	assert(false);
	return m_point;
}

CPoint CSelectedElementDrawingData::GetDrawingPoint(const bool bIsAutoPrice, const CCoordinate& orgPoint) const
{
	CCoordinateCalculator& coordinateCalculator = CCoordinateCalculatorFinder::FindCoordinateCalculator(bIsAutoPrice);
	if(bIsAutoPrice)
		return coordinateCalculator.AbsoluteToAbsolute(m_calData, m_point);
	return coordinateCalculator.RelativeToAbsolute(m_calData, orgPoint);
}

CPoint CSelectedElementDrawingData::GetSelectedDrawPoint_Pixel(const CCoordinate& orgPoint) const
{
	CPoint drawingPoint = orgPoint.GetPixel();

	switch(m_eModifyType)
	{
	case CToolEnumData::MODIFY_CENTER:
		return CPoint(drawingPoint.x + m_point.x, drawingPoint.y + m_point.y);
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_MIDDLE:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_X:
		return CPoint(m_point.x, drawingPoint.y);
	case CToolEnumData::MODIFY_Y:
		return CPoint(drawingPoint.x, m_point.y);
	}

	assert(false);
	return m_point;
}

///////////////////////////////////////////////////////////////////////////////
// class CElementPointList

CElementPointList& CElementPointList::operator=(const CElementPointList& elementPointList)
{
	if(this == &elementPointList)
		return *this;

	m_dVertDataType = elementPointList.GetVertDataType();
	for(int nIndex = 0; nIndex < elementPointList.GetCount(); nIndex++)
	{
		m_pointList.AddTail(elementPointList.GetAt(nIndex));
	}

	return *this;
}

CElementPointList::CElementPointList():
	m_dVertDataType(1.0)
{
}


// public =====================================================================
void CElementPointList::AddTail(CCoordinate* coordinate)
{
	m_pointList.AddTail(coordinate);
}

void CElementPointList::RemoveAll()
{
	m_pointList.RemoveAll();
}

// ----------------------------------------------------------------------------
void CElementPointList::SetVertDataType(const double& dVertDataType)
{
	m_dVertDataType = dVertDataType;
}

double CElementPointList::GetVertDataType() const
{
	return m_dVertDataType;
}

int CElementPointList::GetCount() const
{
	return m_pointList.GetCount();
}

CCoordinate* CElementPointList::GetAt(const int nIndex)
{
	return m_pointList.GetAt(nIndex);
}

CCoordinate* CElementPointList::GetAt(const int nIndex) const
{
	return m_pointList.GetAt(nIndex);
}

