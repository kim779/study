// Symbol.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Symbol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BlockBaseData.h"
#include "MainSymbol.h"
#include "MainSymbolImplementation.h"

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSymbolApp

BEGIN_MESSAGE_MAP(CSymbolApp, CWinApp)
	//{{AFX_MSG_MAP(CSymbolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolApp construction

CSymbolApp::CSymbolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSymbolApp object

CSymbolApp theApp;

// ChartObject.dll 주소
void WINAPI SetMainBlock(IChartManager* pMainBlock, CMainSymbol* pMainSymbol,IChartOCX *p_pIChartOCX)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->SetMainBlock_Object(pMainBlock,p_pIChartOCX);
}

void WINAPI Initialize_Block(CDC* pDC, CWnd* pOCX, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	// (2006/2/15 - Seung-Won, Bae) Manage OCX CWnd with Symbol DLL's CWndMap.
	HWND hOCX = pOCX->GetSafeHwnd();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd *pParentWnd = CWnd::FromHandle( hOCX);

	pMainSymbol->InitializeBlock(pDC, pParentWnd);
}

void WINAPI LoadSymbolDlg(CMainSymbol* pMainSymbol)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(pMainSymbol == NULL)
		return;

	pMainSymbol->CreateSymbolDlg();
}

BOOL WINAPI IsExistSymbolDlg(CMainSymbol* pMainSymbol)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(pMainSymbol == NULL)
		return FALSE;

	return pMainSymbol->IsExistBuletDlg();
}

void WINAPI DestroySymbolDlg(CMainSymbol* pMainSymbol)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->DeleteSymbolDlg();
}

void WINAPI SetCurrentSymbolType(const CSymbol::SYMBOLTOOL symbolType, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->SetCurrentSymbolTool(symbolType);
}

void WINAPI SetSymbolCopy(CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->SetElementCopy();
}

void WINAPI SetShapeDrawingToolType(const COLORREF& crLine,const int& nStyle,const int& nWeight,const bool& bFill, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->SetShapeDrawingType(crLine,nStyle,nWeight,bFill);
}

bool WINAPI LButtonDown(CDC* pDC, const CPoint &point, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->OnLButtonDown(pDC, point);
}

bool WINAPI LButtonDbClk(CDC* pDC, const CPoint &point, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->OnLButtonDbClk(pDC, point);
}

bool WINAPI LButtonUp(CDC* pDC, const CPoint &point, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->OnLButtonUp(pDC, point);
}

bool WINAPI MouseMove(CDC* pDC,const UINT &nFlags, const CPoint &point, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->OnMouseMove(pDC,nFlags, point);
}

bool WINAPI RButtonUp(const CPoint &point, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->OnRButtonUp(point);
}

void WINAPI ReDraw(CDC* pDC, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->OnDraw(pDC);
}

// block 추가시 elementSet을 추가하기.
bool WINAPI InsertElementTree(const CBlockIndex& insertBlockIndex, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->InsertBlockIndex(insertBlockIndex);	
}

void WINAPI MoveElementTree(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->SetModifiedBlockIndexs(sourceBlockIndex, TargetBlockIndex);
}

// element 삭제
void WINAPI DeleteCurrentSymbol(CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->DeleteCurrentSymbols(); 
}

// element 삭제, 맨마지막에 생성된 객체를 삭제
void WINAPI DeleteTail(CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->DeleteLastSymbol(); 
}

// element 삭제, 맨마지막에 생성된 객체를 삭제
bool WINAPI DeleteElementPos(CMainSymbol* pMainSymbol,const long& lAddress)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->DeleteSymbolPos(lAddress); 
}

void WINAPI DeleteAllSymbol(CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->DeleteAllSymbols(); 
}

void WINAPI DeleteAllElementInBlock(const CBlockIndex& blockIndex, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->DeleteAllSymbolsInBlock(blockIndex); 
}

bool WINAPI RButtonMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->OnRButtonMenu(menuType, wParam); 
}

void WINAPI SetToolColor(const COLORREF& color, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->OnSetColor(color); 
}

CString WINAPI GetSavedElementsData(CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return "";

	return pMainSymbol->GetSavedSymbolsData(); 
}

void WINAPI BuildElementsFromString(const CString& Data, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->BuildSymbolsFromString(Data); 
}

void WINAPI SetDeskFilePath(const CString& path, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->SetFilePath(path); 
}

CSymbol::SYMBOLTOOL WINAPI GetCurrentSymbolToolType(CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return (CSymbol::SYMBOLTOOL)0;

	return pMainSymbol->GetToolType(); 
}

CSymbol::SYMBOLTOOL WINAPI GetSelectToolType(CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return (CSymbol::SYMBOLTOOL)0;

	return pMainSymbol->GetSelectedToolType(); 
}

CMainSymbol* WINAPI MakeMainSymbol()
{
	return new CMainSymbol();
}

void WINAPI DeleteMainSymbol(CMainSymbol* pMainSymbol)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(pMainSymbol == NULL)
		return;

	delete pMainSymbol;
}

// element Undo
bool WINAPI UndoElementPos(CMainSymbol* pMainSymbol,const long& lAddress)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->UndoElement(lAddress); 
}

bool WINAPI RedoElementPos(CMainSymbol* pMainSymbol,const long& lAddress)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->RedoElement(lAddress); 
}

CRect WINAPI GetGraphTotalRegion(const CPoint& point,CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return false;

	return pMainSymbol->GetDrawingTotalRegion(point); 
}

void WINAPI DrawTool(CDC *pDC,CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->DrawSymbolTool(pDC); 
}

void WINAPI ChangeAllElementsColor(const COLORREF& crLine,const COLORREF& crText,CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->ChangeAllSymbolsColor(crLine,crText); 
}

// 20081007 JS.Kim	사용자시간대 선택
void WINAPI RecalcElementXPoint(const char* szName, int nTimeDiff, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;

	pMainSymbol->RecalcSymbolXPoint(szName,nTimeDiff); 
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
void WINAPI SetXScaleType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, CMainSymbol* pMainSymbol)
{
	if(pMainSymbol == NULL)
		return;
	
	pMainSymbol->SetXScaleDrawType(eScaleType);
}
// 2011.02.07 by SYS <<

