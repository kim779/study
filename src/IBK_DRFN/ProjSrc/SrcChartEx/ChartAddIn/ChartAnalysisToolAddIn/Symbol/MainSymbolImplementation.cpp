// MainSymbolImplementation.cpp: implementation of the CMainSymbolImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "MainSymbolImplementation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <Math.h>											// for HUGE_VAL

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()		// 20081007 JS.Kim	사용자시간대 선택
#include "../Include_Chart/Conversion.h"					// 20081007 JS.Kim	사용자시간대 선택
#include "../Include_Analysis/CoordinateCalculatorData.h"

#include "SymbolSingleton.h"
#include "SymbolTool.h"
#include "DrawingSymbol.h"
//좌표 계산
#include "CoordinateCalculator.h"
//설정Box 띄우기
#include "Properties_Manager.h"

// Undo / Redo
#include "Order.h"

CMap<int, int, CSymbolDlg*, CSymbolDlg*> CMainSymbolImplementation::g_mapSymbolDlg;
CMap<int, int, CSymbolBaseData::TOOLTYPE,CSymbolBaseData::TOOLTYPE> CMainSymbolImplementation::g_mapSymbolType;

//////////////////////////////////////////////////////////////////////
// Static SymbolDlg
//////////////////////////////////////////////////////////////////////
void CMainSymbolImplementation::InitSymbolDlg()
{
	g_mapSymbolDlg.InitHashTable(1);
	g_mapSymbolType.InitHashTable(1);
}

void CMainSymbolImplementation::DestSymbolDlg()
{
	int nIndex = 0;
	CSymbolDlg* pSymbolDlg = NULL;

	if(g_mapSymbolDlg.GetCount() > 0)
	{
		POSITION pos = g_mapSymbolDlg.GetStartPosition();
		while(pos)
		{
			g_mapSymbolDlg.GetNextAssoc(pos, nIndex, pSymbolDlg);
			
			if(pSymbolDlg != NULL) {
				pSymbolDlg->DestroyWindow();
				delete pSymbolDlg;				
			}
			pSymbolDlg = NULL;		
		 }
		 g_mapSymbolDlg.RemoveAll();
	}

	g_mapSymbolType.RemoveAll();
}

BOOL CMainSymbolImplementation::IsExistBuletDlg()
{
	int nIndex = 0;
	CSymbolDlg* pSymbolDlg = NULL;

	BOOL bExistSymbolDlg = FALSE;
	if(g_mapSymbolDlg.GetCount() > 0)
	{
		POSITION pos = g_mapSymbolDlg.GetStartPosition();
		while(pos)
		{
			g_mapSymbolDlg.GetNextAssoc(pos, nIndex, pSymbolDlg);			
			if(pSymbolDlg != NULL) 
			{
				bExistSymbolDlg = TRUE;
				break;
			}
		}
	}

	return bExistSymbolDlg;
}

void CMainSymbolImplementation::SetSymbolType(const CSymbolBaseData::TOOLTYPE bulettype)
{
	g_mapSymbolType.SetAt(0,bulettype);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMainSymbolImplementation::CMainSymbolImplementation():
	m_pSymbolDlg(NULL),
	m_pSymbolTool(NULL),
	m_pDrawingSymbol(NULL)
{	
	m_pIChartOCX = NULL;
	m_ptOldMouse = CPoint(-1,-1);	

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_ptOldLBtnUp	= CPoint(-1,-1);
	m_eScaleType	= CScaleBaseData::HORZ_DAILY;
	// 2011.02.07 by SYS <<
}

CMainSymbolImplementation::~CMainSymbolImplementation()
{
	//DeleteSymbolDlg();
		DestSymbolDlg();
}

//기호 Dlg 생성.
void CMainSymbolImplementation::CreateSymbolDlg()
{
	m_pSymbolDlg = NULL;
	if(!g_mapSymbolDlg.Lookup(0, m_pSymbolDlg))
	{
		m_pSymbolDlg = new CSymbolDlg(this, &m_symboldata, m_symboldata.GetParent());
		m_pSymbolDlg->Create(IDD_DLG_SYMBOL);
		
		g_mapSymbolDlg.SetAt(0, m_pSymbolDlg);
	}
	
	int nIndex = g_mapSymbolType.GetCount();
	g_mapSymbolType.SetAt(0,CSymbolBaseData::SYMBOL_ARROW_LEFT);

	m_symboldata.SetBuletType(CSymbolBaseData::SYMBOL_ARROW_LEFT);
	m_pSymbolDlg->SetData(&m_symboldata);

	//dlg에 symbol을 설정하고.
	if(m_pDrawingSymbol != NULL) 
	{
		Initial();
		m_symboldata.Invalidate();
	}
	m_pSymbolDlg->ShowWindow(SW_SHOW);
}

//기호 Dlg에서 선택했던 기호Type을 얻는다.
//해당하는 Tool를 얻는다.
bool CMainSymbolImplementation::OnLButtonDown(CDC* pDC, const CPoint &point)
{
	if(m_symboldata.GetSymbolType() == CSymbolBaseData::SYMBOL_BULET && IsExistBuletDlg() == FALSE)
	{
		DestSymbolDlg();
		m_symboldata.SetSymbolType(CSymbolBaseData::SYMBOL_SELECTION);
	}

	GetSymbolTool();
	if(m_pSymbolTool != NULL) 
	{ //마우스 눌려진 위치의 Column, Row.

		// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
		CCoordinateCalculatorData calData = m_pSymbolTool->GetCoordinateCalculatorData( point);
		if( -HUGE_VAL == calData.GetDisplayDataMax_ScaleType() || -HUGE_VAL == calData.GetDisplayDataMin_ScaleType()) return false;

		m_pDrawingSymbol = m_pSymbolTool->OnLButtonDown(pDC, point);
		
		//다른 추세선 선택 해제 		
		if(m_pDrawingSymbol != NULL)
		{
			//20000을 더해주는 이유 - 다른 쪽에서 해제 못하게
			//내가 Check할때는 20000을 뺀다
			int nElementType = m_symboldata.GetSymbolType() + 20000;
			m_pIChartOCX->RunToolCommand(CToolOptionInfo::T_TOOL_SELECTION,nElementType);
		}
		return IsMyMouse();
	}

	return false;
}

// 다시 그린다.
bool CMainSymbolImplementation::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	if(m_pSymbolTool == NULL)
		return false;

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	CCoordinateCalculatorData calData = m_pSymbolTool->GetCoordinateCalculatorData( point);
	if( -HUGE_VAL == calData.GetDisplayDataMax_ScaleType() || -HUGE_VAL == calData.GetDisplayDataMin_ScaleType()) return false;

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	m_ptOldLBtnUp = point;
	// 2011.02.08 by SYS <<
	
	m_pSymbolTool->OnLButtonUp(pDC, point);
	m_pSymbolTool->Initial();
	m_symboldata.Invalidate();
	return IsMyMouse();
}

bool CMainSymbolImplementation::OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point)
{
	m_ptOldMouse = point;
	if(m_pSymbolTool != NULL) 
		return m_pSymbolTool->OnMouseMove(pDC,nFlags, point);

	return false;
}

bool CMainSymbolImplementation::OnLButtonDbClk(CDC* pDC, const CPoint &point)
{
	if(m_pDrawingSymbol == NULL)
		return false;
	
	IChartManager *pIChartManager = m_symboldata.GetMainBlock();
	IPacketManager* pPackManager = m_symboldata.GetPacketManager();
	
	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	CProperties_Manager manager(m_pDrawingSymbol, m_symboldata.GetPropertiesData(), pIChartManager,
// 								m_symboldata.GetUserFilePath(),
// 								pPackManager->GetBaseTimeDataType(),
// 								( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL));

	int nRow, nCol;
	pIChartManager->GetBlockIndex(point, nRow, nCol);
	CBlockIndex blockIndex( nRow, nCol);

	//
	ILPCSTR szHelpMsg = NULL;
	CString strFormatX;	
	pPackManager->GetHelpMessageItem("RDATET", szHelpMsg);	// 보조메시지의 UD 타입을 얻어온다.	
	strFormatX = szHelpMsg;
	
	CProperties_Manager manager(m_pDrawingSymbol, m_symboldata.GetPropertiesData(), pIChartManager,
								m_symboldata.GetUserFilePath(),
								pPackManager->GetBaseTimeDataType(),
								( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL),
								blockIndex, strFormatX);
	// 2011.02.08 by SYS <<

	if( pIChartManager) pIChartManager->Release();
	if( pPackManager) pPackManager->Release();
	
	// Undo / Redo
	CElementSaverBuilder builder;
	CString strSymbolData_Prev = builder.GetSavedSymbolData_ForUndoRedo(m_symboldata.GetElementTree()->GetOrder(), m_pDrawingSymbol);
	if(manager.PopUpPropertiesBox())
	{
		CString strSymbolData_After = builder.GetSavedSymbolData_ForUndoRedo(m_symboldata.GetElementTree()->GetOrder(), m_pDrawingSymbol);
		if(strSymbolData_Prev.Compare(strSymbolData_After))
			m_symboldata.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_MODIFY_ENV, (long)m_pDrawingSymbol, strSymbolData_Prev, strSymbolData_After);

		m_symboldata.Invalidate();
		return true;
	}

	return false;
}

//다시 그린다. OnLButtonDown(즉, DrawStart가 true일때)도 그리게 되면 제대로 그림이 안그려진다.
//그래서, DrawStart(즉, Up했을때..)가 false일때만 그린다.
void CMainSymbolImplementation::OnDraw(CDC *pDC)
{
	//도형을 그릴때 같이 그려지면 그림이 엉망이 되니까..도형을 그리는 동안은 그리면 안된다.
	//그런데 이러게 하면 도형을 그리고 있는 동안은 기호가 그려지지 않는다.
//	if(m_pSymbolTool == NULL || m_pSymbolTool != NULL && m_pSymbolTool->IsDrawStart() == false) {
	m_symboldata.DrawElement(pDC, m_pDrawingSymbol);

	if(m_pSymbolTool != NULL && m_pDrawingSymbol != NULL)
		m_pSymbolTool->OnDraw(pDC);
}

void CMainSymbolImplementation::DrawSymbolTool(CDC *pDC)
{
	if(m_pSymbolTool != NULL)
	{
		//if(m_pDrawingSymbol != NULL)
		//	m_pDrawingSymbol->OnDraw(pDC);
		//else
			m_pSymbolTool->OnDraw(pDC);
	}
}

void CMainSymbolImplementation::SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex)
{
	m_symboldata.GetElementTree()->SetModifiedBlockIndexs(sourceBlockIndex, TargetBlockIndex);
}

void CMainSymbolImplementation::SetMainBlock_Object(IChartManager *pMainBlock,IChartOCX *p_pIChartOCX)
{
	m_pIChartOCX = p_pIChartOCX;
	m_symboldata.SetMainBlock_Object(pMainBlock,p_pIChartOCX);
	m_singleton.SetOcxHwnd( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);
}

bool CMainSymbolImplementation::InsertBlockIndex(const CBlockIndex &insertBlockIndex)
{
	if(!insertBlockIndex.IsAllValueBiggerThanZero())
		return false;

	//블럭영역과 Symbol관리 리스트를 수정한다.
	if(!m_symboldata.GetElementTree()->InsertBlockIndex(insertBlockIndex))
		return false;
	return true;
}

void CMainSymbolImplementation::DeleteAllSymbols()
{
	m_symboldata.GetElementTree()->DeleteAllSymbols();
	m_symboldata.GetUndoRedoManager().DeleteAll();		// Undo / Redo
	Initial();
	m_symboldata.Invalidate();
}

void CMainSymbolImplementation::DeleteAllSymbolsInBlock(const CBlockIndex &blockIndex)
{
	CList<CDrawingSymbol*, CDrawingSymbol*> elementList;
	m_symboldata.GetElementTree()->GetElementListOfBlock(blockIndex, elementList);
	POSITION pos = elementList.GetHeadPosition();
	CDrawingSymbol* pDrawingSymbol = NULL;
	while(pos)
	{
		if(pDrawingSymbol = elementList.GetNext(pos))
			m_symboldata.UnRegisterElementToOrderManager((long)pDrawingSymbol);
	}

	m_symboldata.GetElementTree()->DeleteAllSymbolsInBlock(blockIndex);

	// Undo / Redo
	CList<CUndo_Redo_Info*, CUndo_Redo_Info*> undoredoInfoList;
	m_symboldata.GetUndoRedoManager().GetUndoRedoInfoListOfBlock(blockIndex, undoredoInfoList);
	pos = undoredoInfoList.GetHeadPosition();
	while(pos)
	{
		m_symboldata.GetUndoRedoManager().UnRegisterElementToUnRedoManager((long)undoredoInfoList.GetNext(pos));
	}
	
	m_symboldata.Invalidate();
}

bool CMainSymbolImplementation::DeleteCurrentSymbols()
{
	if(m_pDrawingSymbol == NULL)
		return false;

	m_symboldata.UnRegisterElementToOrderManager((long)m_pDrawingSymbol);

	// Undo / Redo
	m_symboldata.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)m_pDrawingSymbol);

	bool bResult = m_symboldata.GetElementTree()->DeleteCurrentDrawingSymbol(m_pDrawingSymbol);
	Initial();
	m_symboldata.Invalidate();
	return bResult;
}

bool CMainSymbolImplementation::OnRButtonUp(const CPoint &point)
{
	IChartManager *pIChartManager = m_symboldata.GetMainBlock();
	if( pIChartManager)
	{
		pIChartManager->Release();
		//sy 2005.04.06 -> 이미 선택된 심볼이 있을 경우엔 메뉴가 나오게 한다.
		return (m_pSymbolTool != NULL && m_pDrawingSymbol != NULL);
		/*if(m_symboldata.FindSymbol(point) != NULL)
			return true;
		return false;*/
	}
	return false;
}

bool CMainSymbolImplementation::SetColor(const COLORREF &symbolColor)
{
	if(m_pDrawingSymbol == NULL)
		return false;

	m_pDrawingSymbol->SetColor(symbolColor);
	m_symboldata.Invalidate();
	return true;
}

void CMainSymbolImplementation::Initial()
{
	m_pSymbolTool = NULL;
	m_pDrawingSymbol = NULL;
	m_singleton.InitialTool();	
}
//cdc 필요없는데..이제.
void CMainSymbolImplementation::InitializeBlock(CDC *pDC, CWnd *pParentWnd)
{
	m_symboldata.SetParent(pParentWnd);
}

void CMainSymbolImplementation::SetShapeDrawingType(const COLORREF& crLine,const int& nStyle,const int& nWeight,const bool& bFill)
{
	m_symboldata.GetPropertiesData()->SetColor(crLine);
	if(nWeight >= 0)
	{
		m_symboldata.GetPropertiesData()->SetStyle(nStyle);
		m_symboldata.GetPropertiesData()->SetWeight(nWeight);
		m_symboldata.GetPropertiesData()->SetFillState(bFill);
	}
}

void CMainSymbolImplementation::SetCurrentSymbolTool(const CSymbol::SYMBOLTOOL symbolType)
{
	if(IsExistBuletDlg() == TRUE && m_pSymbolDlg != NULL)  
	{
		//버튼 해제 
		if(symbolType == CSymbol::SYMBOL_SELECTION)
			m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_SYMBOL_BULET,FALSE);

		if(!m_pSymbolDlg->IsSymbolDlgClose())
		{	//Dlg가 떠있을때는 ocx에서 받은 Tool을 Set하지 말자.
			if(symbolType == CSymbol::SYMBOL_BULET)
				return;
			//bulet형이 아닌 다른 형이 들어왔을때 적용
			if(m_pSymbolDlg->GetSafeHwnd())
				m_pSymbolDlg->ShowWindow(SW_HIDE);
		}
	}

	//edit가 delete 되지 않은 상황에서 
	//if(m_pDrawingSymbol)
	{
		//CSymbolBaseData::TOOLTYPE eType = m_pDrawingSymbol->GetSymbolType();
		CSymbolBaseData::TOOLTYPE eType = m_symboldata.GetSymbolType();
		if(symbolType == CSymbol::SYMBOL_SELECTION && eType == CSymbolBaseData::SYMBOL_TEXT)
		{
			//DeleteCurrentSymbols();
			CWnd* pWnd = m_symboldata.GetParent()->GetFocus();
			if(pWnd != NULL && pWnd != m_symboldata.GetParent())
				pWnd->SendMessage(WM_KILLFOCUS, 0, 0);
		}
	}

	m_symboldata.SetSymbolType(CSymbolBaseData::TOOLTYPE(symbolType));
	GetSymbolTool();
}

void CMainSymbolImplementation::GetSymbolTool()
{
	bool bInvalidate = false;
	if(m_pSymbolTool != NULL || m_pDrawingSymbol != NULL)
		bInvalidate = true;

	m_pSymbolTool = NULL;
	m_pDrawingSymbol = NULL;
	CSymbolBaseData::TOOLTYPE type = m_symboldata.GetSymbolType();

	//다중 차트에서 Symbol처리를 위해서 ..	
	if(IsExistBuletDlg() == TRUE)
	{
		if(type != CSymbolBaseData::SYMBOL_BULET)
			m_symboldata.SetSymbolType(CSymbolBaseData::SYMBOL_BULET);

		int nIndex = g_mapSymbolType.GetCount();
		CSymbolBaseData::TOOLTYPE typeBullet = CSymbolBaseData::SYMBOL_ARROW_LEFT;
		if(nIndex > 0)
			g_mapSymbolType.Lookup(nIndex-1,typeBullet);
			
		m_pSymbolTool = m_singleton.Find(&m_symboldata,typeBullet);
	}
	else if(type == CSymbolBaseData::SYMBOL_SELECTION)
	{
		m_pSymbolTool = m_singleton.Find(&m_symboldata, CSymbolBaseData::SYMBOL_SELECTION);
	}
	else 
	{
		if(type == CSymbolBaseData::SYMBOL_BULET)
			m_pSymbolTool = m_singleton.Find(&m_symboldata,m_symboldata.GetBuletType());
		else
			m_pSymbolTool = m_singleton.Find(&m_symboldata, type);
	}

	//sy 2005.04.06
	if(bInvalidate)
		m_symboldata.Invalidate();
}

bool CMainSymbolImplementation::OnRButtonMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	if(menuType == CMenuType::MENU_DELETE) // 삭제
		return DeleteCurrentSymbols();
	else if(menuType == CMenuType::MENU_COLOR) // color
		return SetColor((COLORREF)wParam);
	else if(menuType == CMenuType::MENU_SET)
	{// 설정 Dlg
		IChartManager *pIChartManager = m_symboldata.GetMainBlock();
		IPacketManager* pPackManager = m_symboldata.GetPacketManager();


		// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 		CProperties_Manager manager(m_pDrawingSymbol, m_symboldata.GetPropertiesData(), pIChartManager,
// 									m_symboldata.GetUserFilePath(),
// 									pPackManager->GetBaseTimeDataType(),
// 									( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL));

		int nRow, nCol;
		pIChartManager->GetBlockIndex(m_ptOldLBtnUp, nRow, nCol);
		CBlockIndex blockIndex( nRow, nCol);

		//
		ILPCSTR szHelpMsg = NULL;
		CString strFormatX;	
		pPackManager->GetHelpMessageItem("RDATET", szHelpMsg);	// 보조메시지의 UD 타입을 얻어온다.	
		strFormatX = szHelpMsg;

		CProperties_Manager manager(m_pDrawingSymbol, m_symboldata.GetPropertiesData(), pIChartManager,
									m_symboldata.GetUserFilePath(),
									pPackManager->GetBaseTimeDataType(),
									( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL),
									blockIndex, strFormatX);

		// 2011.02.08 by SYS <<

		if( pIChartManager) pIChartManager->Release();
		if( pPackManager) pPackManager->Release();
		if(manager.PopUpPropertiesBox())
		{
			m_symboldata.Invalidate();
			return true;
		}
		return false;
	}
	return false;
}

void CMainSymbolImplementation::OnSetColor(const COLORREF &color)
{
	m_symboldata.GetPropertiesData()->SetColor(color);
}

CString CMainSymbolImplementation::GetSavedSymbolsData()
{
	return m_symboldata.GetSavedSymbolsData();
}

void CMainSymbolImplementation::BuildSymbolsFromString(const CString &Data)
{
 	m_symboldata.BuildSymbolsFromString(Data);
	m_symboldata.Invalidate();
}

void CMainSymbolImplementation::SetFilePath(const CString &path)
{
	m_symboldata.SetFilePath(path);
}

bool CMainSymbolImplementation::IsMyMouse()
{
	if(m_symboldata.GetSymbolType() == CSymbolBaseData::SYMBOL_SELECTION && m_pDrawingSymbol != NULL) 
		return true;
	else if(m_symboldata.GetSymbolType() == CSymbolBaseData::SYMBOL_SELECTION && m_pDrawingSymbol == NULL) 
		return false;
	//선택 툴이 아닌경우는 tool이 NUll이 아니니까..True 
	return true;
}

void CMainSymbolImplementation::DeleteSymbolDlg()
{
	int nIndex = 0;
	CSymbolDlg* pSymbolDlg = NULL;

	if(g_mapSymbolDlg.GetCount() > 0)
	{
		POSITION pos = g_mapSymbolDlg.GetStartPosition();
		while(pos)
		{
			g_mapSymbolDlg.GetNextAssoc(pos, nIndex, pSymbolDlg);
			
			if(pSymbolDlg != NULL) {
				pSymbolDlg->DestroyWindow();
				delete pSymbolDlg;				
			}
			pSymbolDlg = NULL;		
		 }
		 g_mapSymbolDlg.RemoveAll();
	}

	g_mapSymbolType.RemoveAll();
}

void CMainSymbolImplementation::DeleteLastSymbol()
{
	CDrawingSymbol* pDrawingSymbol = m_symboldata.GetElementTree()->GetTail();
	m_symboldata.GetElementTree()->DeleteLastSymbol();

	// Undo / Redo
	m_symboldata.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)pDrawingSymbol);

	Initial();
	m_symboldata.Invalidate();
}

bool CMainSymbolImplementation::DeleteSymbolPos(const long lAddress)
{
	// Undo / Redo
	m_symboldata.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, lAddress);

	bool bDelete = m_symboldata.GetElementTree()->DeleteSymbolPos(lAddress);
	if(bDelete == true)
	{
		Initial();
		m_symboldata.Invalidate();
	} 
	return bDelete;
}

bool CMainSymbolImplementation::UndoElement(const long lAddress)
{
	return (m_symboldata.GetUndoRedoManager().UndoElement(lAddress) == TRUE);
}

bool CMainSymbolImplementation::RedoElement(const long lAddress)
{
	return (m_symboldata.GetUndoRedoManager().RedoElement(lAddress) == TRUE);
}

CSymbol::SYMBOLTOOL CMainSymbolImplementation::GetToolType()
{
	switch(m_symboldata.GetSymbolType())
	{
		case CSymbolBaseData::SYMBOL_BULET:
		case CSymbolBaseData::SYMBOL_ARROW_LEFT:  //왼쪽 화살표
		case CSymbolBaseData::SYMBOL_ARROW_RIGHT: //오른쪽 화살표
		case CSymbolBaseData::SYMBOL_ARROW_UP:  //위쪽 화살표
		case CSymbolBaseData::SYMBOL_ARROW_DOWN:  //아래쪽 화살표
		case CSymbolBaseData::SYMBOL_ARROW_LEFTTOP:  //왼쪽 위 방향 화살표
		case CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM: //왼쪽 아래 방향 화살표
		case CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP:  //오른쪽 위 방향 화살표
		case CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM: //오른쪽 아래 방향 화살표
		case CSymbolBaseData::SYMBOL_NUM_ONE:  //숫자 1
		case CSymbolBaseData::SYMBOL_NUM_TWO:  //2
		case CSymbolBaseData::SYMBOL_NUM_THREE: //3
		case CSymbolBaseData::SYMBOL_NUM_FOUR:  //4
		case CSymbolBaseData::SYMBOL_NUM_FIVE:  //5
		case CSymbolBaseData::SYMBOL_NUM_SIX:  //6
		case CSymbolBaseData::SYMBOL_NUM_SEVEN:  //7
		case CSymbolBaseData::SYMBOL_NUM_EIGHT:  //8
		case CSymbolBaseData::SYMBOL_NUM_NINE:  //9
		case CSymbolBaseData::SYMBOL_NUM_TEN:  //10
		case CSymbolBaseData::SYMBOL_TEXT_A:  //a
		case CSymbolBaseData::SYMBOL_TEXT_B: //b
		case CSymbolBaseData::SYMBOL_TEXT_C: //c
		case CSymbolBaseData::SYMBOL_TEXT_D: //d
		case CSymbolBaseData::SYMBOL_TEXT_E: //e
		case CSymbolBaseData::SYMBOL_TEXT_F: //f
		case CSymbolBaseData::SYMBOL_TEXT_G: //g
		case CSymbolBaseData::SYMBOL_TEXT_H: //h
		case CSymbolBaseData::SYMBOL_CHAR_1:		//☞
		case CSymbolBaseData::SYMBOL_CHAR_2:		//☜
		case CSymbolBaseData::SYMBOL_CHAR_3:		//◆
		case CSymbolBaseData::SYMBOL_CHAR_4:		//★
		case CSymbolBaseData::SYMBOL_CHAR_5:		//●
		case CSymbolBaseData::SYMBOL_CHAR_6:		//■		
			return CSymbol::SYMBOL_BULET;
	}
	return (CSymbol::SYMBOLTOOL)m_symboldata.GetSymbolType();
}

CSymbol::SYMBOLTOOL CMainSymbolImplementation::GetSelectedToolType()
{
	if(m_pDrawingSymbol == NULL)
		return CSymbol::SYMBOL_NONE;

	return (CSymbol::SYMBOLTOOL)m_symboldata.GetSymbolType();
}
 
void CMainSymbolImplementation::SetElementCopy()
{
	if(m_pDrawingSymbol == NULL)	
		return;

//>> (2008/12/16 - Seung-Lyong Park) Symbol 복사
	CDrawingSymbol* pNewDrawingSymbol = NULL;
	pNewDrawingSymbol = m_symboldata.CopySymbol(m_pDrawingSymbol);
	if(pNewDrawingSymbol == NULL)
		return;

	m_pDrawingSymbol = pNewDrawingSymbol;

	IChartManager *pIChartManager = m_symboldata.GetMainBlock();
	IBlock* pBlock = pIChartManager->GetBlock(m_symboldata.GetBlockIndexInSymbol((long)m_pDrawingSymbol).GetRow(), m_symboldata.GetBlockIndexInSymbol((long)m_pDrawingSymbol).GetColumn());
	pIChartManager->Release();
	CRect rcGrpRegion = pBlock->GetGraphDrwingRegion();
	pBlock->Release();

	CPoint ptGap(0,0);
	CSymbolBaseData::TOOLTYPE eType = m_pDrawingSymbol->GetSymbolType();
	CCoordinateCalculator calculator;
	CCoordinateCalculatorData calData = m_pSymbolTool->GetCoordinateCalculatorData( rcGrpRegion.CenterPoint());

	if( eType == CSymbolBaseData::SYMBOL_SQUARE || eType == CSymbolBaseData::SYMBOL_CIRCLE ||
		eType == CSymbolBaseData::SYMBOL_TRIANGLE )
	{
		ptGap.Offset(10,10);
		m_pDrawingSymbol->TranslatePoint(false, rcGrpRegion.CenterPoint(), ptGap, rcGrpRegion, calData);
	}
	else if(eType == CSymbolBaseData::SYMBOL_TEXT)
	{
		CCoordinate coordStart = m_pDrawingSymbol->GetStartPoint();
		CPoint pt1(calculator.RelativeToAbsolute(calData, coordStart));
		ptGap.Offset(-10,-10);
		m_pDrawingSymbol->TranslatePoint(false, pt1, ptGap, rcGrpRegion, calData);
	}
	else
	{
		CCoordinate coordStart = m_pDrawingSymbol->GetStartPoint();
		CPoint pt1(calculator.RelativeToAbsolute(calData, coordStart));
		pt1.Offset(10,10);
		m_pDrawingSymbol->TranslatePoint(false, pt1, ptGap, rcGrpRegion, calData);
	}
	
	m_symboldata.Invalidate();
//<< (2008/12/16 - Seung-Lyong Park) Symbol 복사
}

CRect CMainSymbolImplementation::GetDrawingTotalRegion(CPoint point)
{
	IChartManager *pIChartManager = m_symboldata.GetMainBlock();
	CRect rectRegion = pIChartManager->GetGraphRegionSumInColumn(point);
	pIChartManager->Release();	
	return rectRegion;
}

void CMainSymbolImplementation::ChangeAllSymbolsColor(const COLORREF& crLine,const COLORREF& crText)
{
	m_symboldata.GetElementTree()->ChangeAllSymbolsColor(crLine, crText);
}

// 20081007 JS.Kim	사용자시간대 선택
// Graph의 시간대가 변경되면 Symbol 의 시간도 바꿔준다.
bool CMainSymbolImplementation::RecalcSymbolXPoint(const char* szName, int nTimeDiff)
{
	IChartManager *pIChartManager = m_symboldata.GetMainBlock();
	ILPCSTR szCurrentRQ = pIChartManager->GetCurrentRQ();
	IPacket* pPacket= m_symboldata.GetDatePacket( szCurrentRQ);
	CString strCurrentRQ(szCurrentRQ);

	int	nBaseTimeDiff;
	if( strlen(szName) == 0 )	nBaseTimeDiff = pPacket->GetOrgBaseTimeDifference();		// 원래시간으로 되돌린 경우
	else						nBaseTimeDiff = nTimeDiff;

	int nBlkColCount = pIChartManager->GetColumnCount();
	int nBlkRowCount;
	for( int col = 0; col < nBlkColCount; col++ )
	{
		nBlkRowCount = pIChartManager->GetRowCountInColumn(col);
		for( int row = 0; row < nBlkRowCount; row++ )
		{
			ILPCSTR szMasterRQ = pIChartManager->GetMainRQofBlock(row, col);
			if( strCurrentRQ.Compare(szMasterRQ) )		continue;

			RecalcElementListXPoint(row, col, nBaseTimeDiff, pPacket);
		}
	}	
	
	if( pPacket) pPacket->Release();
	if( pIChartManager) pIChartManager->Release();

	return true;
}

void CMainSymbolImplementation::RecalcElementListXPoint(int row, int col, int nBaseTimeDiff, IPacket* pPacket)
{
	CList<CDrawingSymbol*, CDrawingSymbol*> elementList;

	CBlockIndex blkIdx(row, col);
	m_symboldata.GetElementTree()->GetElementListOfBlock(blkIdx, elementList);

	ILPCSTR szPacketType = pPacket->GetType();

	double dXDate, oldXDate;
	int oldTimeDiff;
	CCoordinate coordinate;
	CDrawingSymbol* pDrawingSymbol = NULL;
	POSITION pos = elementList.GetHeadPosition();
	while(pos)
	{
		if( !(pDrawingSymbol = elementList.GetNext(pos)) )	break;
		for( int i=0; i<2; i++ )
		{
			if( i == 0 )		coordinate = pDrawingSymbol->GetStartPoint();
			else				coordinate = pDrawingSymbol->GetEndPoint();

			oldXDate = coordinate.GetX();
			oldTimeDiff = coordinate.GetTimeDiff();
			if( oldTimeDiff != nBaseTimeDiff )
			{
				dXDate = CDataConversion::CalcTimeDiff(oldXDate, nBaseTimeDiff - oldTimeDiff, pPacket->GetOrgBaseTimeDifference(), CString( szPacketType));
				coordinate.SetX(dXDate);
				coordinate.SetTimeDiff(nBaseTimeDiff);

				if( i == 0 )		pDrawingSymbol->SetStartCoord(coordinate);
				else				pDrawingSymbol->SetEndCoord(coordinate);
			}
		}
	}
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
void CMainSymbolImplementation::SetXScaleDrawType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType)
{
	m_eScaleType = eScaleType;
	m_symboldata.SetXScaleDrawType(eScaleType);
}

CScaleBaseData::HORZSCALEDRAWERTYPE CMainSymbolImplementation::GetXScaleDrawType()
{
	return m_eScaleType;
}
// 2011.02.07 by SYS <<

