// SymbolData.cpp: implementation of the CSymbolData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "SymbolData.h"

#include "../Include_Addin_133101/I133101/_IPacketListManager.h"		// for IPacketListManager

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CSymbolData::m_strAddInItemName = "ANALYSIS_TOOL";
//CString CSymbolData::m_strAddInItemName = "SYMBOL_TOOL";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSymbolData::CSymbolData():
	m_pParent(NULL),
	m_pMainBlock(NULL),
	m_pIChartOCX(NULL),
	m_pIDefaultPacketManager(NULL),
	m_strFilePath(""),
	m_TYPEBulet(CSymbolBaseData::SYMBOL_ARROW_LEFT)
{
	GetUndoRedoManager().SetSymbolData(this);	// Undo / Redo

	// 20081007 JS.Kim	사용자시간대 선택
	m_pIPacketListManager = NULL;

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_eScaleType	= CScaleBaseData::HORZ_DAILY;
	// 2011.02.07 by SYS <<
}

CSymbolData::~CSymbolData()
{
	if( m_pIChartOCX) 
		m_pIChartOCX->Release();
	if( m_pMainBlock) 
		m_pMainBlock->Release();
	if( m_pIDefaultPacketManager) 
		m_pIDefaultPacketManager->Release();
	// 20081007 JS.Kim	사용자시간대 선택
	if( m_pIPacketListManager) m_pIPacketListManager->Release();
}

void CSymbolData::SetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

CWnd* CSymbolData::GetParent() const
{
	return m_pParent;
}

void CSymbolData::SetFilePath(const CString &filepath)
{
	m_strFilePath = filepath;
}

CString CSymbolData::GetFilePath() const
{
	return m_strFilePath;
}

void CSymbolData::SetUserFilePath(const CString &filepath)
{
	m_strUserFilePath = filepath;
}

CString CSymbolData::GetUserFilePath() const
{
	return m_strUserFilePath;
}

CPropertiesData_text* CSymbolData::GetPropertiesData()
{
	return &m_Properties;
}

void CSymbolData::SetMainBlock_Object(IChartManager *pMainBlock,IChartOCX *p_pIChartOCX)
{
	if( m_pIChartOCX) m_pIChartOCX->Release();
	m_pIChartOCX = p_pIChartOCX;
	if(m_pIChartOCX) p_pIChartOCX->AddRef();

	if( m_pMainBlock) m_pMainBlock->Release();
	m_pMainBlock = pMainBlock;
	if(m_pMainBlock) m_pMainBlock->AddRef();

	if( m_pIDefaultPacketManager) 
		m_pIDefaultPacketManager->Release();	
	m_pIDefaultPacketManager = p_pIChartOCX->GetIDefaultPacketManager();
	
	ILPCSTR szUserFilePath = p_pIChartOCX->GetUserFilePathString();
	m_strUserFilePath = szUserFilePath;

	// 20081007 JS.Kim	사용자시간대 선택
	if( m_pIPacketListManager) m_pIPacketListManager->Release();
	m_pIPacketListManager = m_pIChartOCX->GetIPacketListManager();
}

IChartManager* CSymbolData::GetMainBlock() const
{
	if( m_pMainBlock) m_pMainBlock->AddRef();
	return m_pMainBlock;
}

IPacketManager* CSymbolData::GetPacketManager() const
{
	if( m_pIDefaultPacketManager) 
		m_pIDefaultPacketManager->AddRef();
	return m_pIDefaultPacketManager;
}

IChartOCX* CSymbolData::GetChartOCX() const
{
	if( m_pIChartOCX) 
		m_pIChartOCX->AddRef();
	return m_pIChartOCX;
}

void CSymbolData::SetSymbolType(const CSymbolBaseData::TOOLTYPE symboltype)
{
	m_TYPESymbol = symboltype;
}

void CSymbolData::SetBuletType(const CSymbolBaseData::TOOLTYPE bulettype)
{
	//사각, 원, text가 아닐때..
	//기호 Dlg에는 이런 도형이 없으니까..설정하면 안되지.
	//dlg가 띄워진 상태에서도 이제는 선택이 가능하다.
	if(bulettype != CSymbolBaseData::SYMBOL_SQUARE && bulettype != CSymbolBaseData::SYMBOL_CIRCLE && bulettype != CSymbolBaseData::SYMBOL_TRIANGLE && bulettype != CSymbolBaseData::SYMBOL_TEXT)
		m_TYPEBulet = bulettype;
}

CSymbolBaseData::TOOLTYPE CSymbolData::GetSymbolType() const
{
	return m_TYPESymbol;
}

CSymbolBaseData::TOOLTYPE CSymbolData::GetBuletType() const
{
	return m_TYPEBulet;
}

void CSymbolData::DrawElement(CDC *pDC, CDrawingSymbol *pSelected)
{
	m_elementTree.DrawElement(pDC, m_pMainBlock, pSelected);
}

CElementTree* CSymbolData::GetElementTree()
{
	return &m_elementTree;
}

CDrawingSymbol* CSymbolData::FindSymbol(const CPoint &point)
{
	return m_elementTree.FindSymbol(point, m_pMainBlock);
}

CString CSymbolData::GetSavedSymbolsData()
{
	return m_elementTree.GetSavedSymbolsData();
}

void CSymbolData::BuildSymbolsFromString(const CString &Data)
{
	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//m_elementTree.BuildSymbolsFromString(Data);
	m_elementTree.BuildSymbolsFromString(m_eScaleType, Data);
	// 2011.02.07 by SYS <<
}

void CSymbolData::Invalidate()
{
	if(m_pParent == NULL)
		return;

	m_pParent->Invalidate();
}

void CSymbolData::RegisterElementToOrderManager(long lElementAdress)
{
	IAddInToolMgr* pAddInToolManager = m_pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER,
															m_strAddInItemName,lElementAdress);
	pAddInToolManager->Release();
}

void CSymbolData::UnRegisterElementToOrderManager(long lElementAdress)
{
	IAddInToolMgr* pAddInToolManager = m_pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER,
															m_strAddInItemName,lElementAdress);
	pAddInToolManager->Release();
}

//>> (2008/12/16 - Seung-Lyong Park) Symbol 복사
CDrawingSymbol* CSymbolData::CopySymbol(CDrawingSymbol *pDrawingSymbol)
{
	if(pDrawingSymbol == NULL)
		return NULL;

	CDrawingSymbol* pNewDrawingSymbol = (CDrawingSymbol*)m_elementTree.CopyElement(pDrawingSymbol);
	RegisterElementToOrderManager((long)pNewDrawingSymbol);

	// Undo / Redo
	GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)pNewDrawingSymbol);

	return pNewDrawingSymbol;
}
//<< (2008/12/16 - Seung-Lyong Park) Symbol 복사

// ----------------------------------------------------------------------------
// Undo / Redo
const CUndoRedoManager& CSymbolData::GetUndoRedoManager() const
{
	return m_UndoRedoManager;
}

CUndoRedoManager& CSymbolData::GetUndoRedoManager()
{
	return m_UndoRedoManager;
}

BOOL CSymbolData::MoveSymbol(CDrawingSymbol* pSelectedSymbol, CString strSymbolData_Prev)
{
	if(pSelectedSymbol == NULL)
		return FALSE;

	// Undo / Redo
	GetUndoRedoManager().RegisterElementToUnRedoManager(URT_MOVE, (long)pSelectedSymbol, strSymbolData_Prev);

	return TRUE;
}

CBlockIndex CSymbolData::GetBlockIndexInSymbol(long lSymbolAddress) const
{
	const CDrawingSymbol* pSelectedSymbol = (const CDrawingSymbol*)lSymbolAddress;
	if(pSelectedSymbol == NULL)
		return CBlockIndex();

	return m_elementTree.GetBlockIndexInSymbol(pSelectedSymbol);
}

COrder& CSymbolData::GetOrder()
{
	return m_elementTree.GetOrder();
}

// 20081007 JS.Kim	사용자시간대 선택
IPacket *CSymbolData::GetDatePacket( const char *p_szRQ) const
{
	if( !m_pIPacketListManager) return NULL;
	return m_pIPacketListManager->GetDatePacket( p_szRQ);
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
void CSymbolData::SetXScaleDrawType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType)
{
	m_eScaleType = eScaleType;
}

CScaleBaseData::HORZSCALEDRAWERTYPE CSymbolData::GetXScaleDrawType()
{
	return m_eScaleType;
}
// 2011.02.07 by SYS <<

