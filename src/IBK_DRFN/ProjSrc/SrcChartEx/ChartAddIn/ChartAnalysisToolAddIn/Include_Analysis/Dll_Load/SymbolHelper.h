// SymbolHelper.h: interface for the CSymbolHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLHELPER_H__D2022450_936F_4315_899B_E517896270BA__INCLUDED_)
#define AFX_SYMBOLHELPER_H__D2022450_936F_4315_899B_E517896270BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include/LoadDll.h"
#include "../Include_Analysis/BlockBaseEnum2.h"				// for CSymbol

#include "BlockBaseData.h"

class CMainSymbol;

//mainsymbol 생성
typedef CMainSymbol* (WINAPI *fpMakeMainSymbol)();
typedef void (WINAPI *fpDeleteMainSymbol)(CMainSymbol*);
//기호 dll 초기화
typedef void (WINAPI *fpSetMainBlock)(IChartManager*, CMainSymbol*,IChartOCX *);
typedef void (WINAPI *fpInitialize)(CDC* , CWnd* , CMainSymbol*);
typedef void (WINAPI *fpSetDeskFilePath)(const CString& , CMainSymbol*);
typedef void (WINAPI *fpSetCurrentSymbolType)(const CSymbol::SYMBOLTOOL, CMainSymbol*);
typedef void (WINAPI *fpSetSymbolCopy)(CMainSymbol*);
typedef void (WINAPI *fpSetShapeDrawingType)(const COLORREF&,const int&,const int&,const bool&,CMainSymbol*);
typedef void (WINAPI *fpLoadSymbolDlg)(CMainSymbol*);
typedef BOOL (WINAPI *fpIsExistSymbolDlg)(CMainSymbol*);
//화면저장
typedef CString (WINAPI *fpGetSavedElementsData)(CMainSymbol*);
typedef void (WINAPI *fpBuildElementsFromString)(const CString& , CMainSymbol*);
//Mouse 이벤트
typedef bool (WINAPI *fpOnLButtonDown)(CDC* , const CPoint&, CMainSymbol*);
typedef bool (WINAPI *fpOnLButtonUp)(CDC* , const CPoint&, CMainSymbol*);
typedef bool (WINAPI *fpOnLButtonDbClk)(CDC* , const CPoint&, CMainSymbol*);
typedef bool (WINAPI *fpOnRButtonUp)(const CPoint&, CMainSymbol*);
typedef bool (WINAPI *fpOnMouseMove)(CDC* ,const UINT &nFlags, const CPoint&, CMainSymbol*);
//그리기
typedef void (WINAPI *fpOnDraw)(CDC*, CMainSymbol*);
//블럭 추가, 블럭이동
typedef void (WINAPI *fpInsertElementTree)(const CBlockIndex&, CMainSymbol*);
typedef void (WINAPI *fpMoveElementTree)(const CBlockIndex&, const CBlockIndex&, CMainSymbol*);
//기호 지우기
typedef void (WINAPI *fpDeleteCurrentSymbol)(CMainSymbol*);
typedef void (WINAPI *fpDeleteTail)(CMainSymbol*);
typedef bool (WINAPI *fpDeleteElementPos)(CMainSymbol*,const long&);
typedef void (WINAPI *fpDeleteAllSymbol)(CMainSymbol*);
typedef void (WINAPI *fpDeleteAllElementInBlock)(const CBlockIndex&, CMainSymbol*);
//기호 색깔, 삭제 설정.
typedef bool (WINAPI *fpOnMenu)(const CMenuType::ELEMENTMENU, const WPARAM, CMainSymbol*);
typedef void (WINAPI *fpSetColor)(const COLORREF&, CMainSymbol*);
//기호 Dlg 파괴하기
typedef void (WINAPI *fpDestroySymbolDlg)(CMainSymbol*);
//Undo Redo 함수
typedef bool (WINAPI *fpUndoElementPos)(CMainSymbol*,const long&);
typedef bool (WINAPI *fpRedoElementPos)(CMainSymbol*,const long&);
//tooltype 얻기
typedef CSymbol::SYMBOLTOOL (WINAPI *fpGetCurrentToolType)(CMainSymbol*);
typedef CSymbol::SYMBOLTOOL (WINAPI *fpGetSelectToolType)(CMainSymbol*);
//DrawCursor관련 함수
typedef CRect (WINAPI *fpGetGraphTotalRegion)(const CPoint&,CMainSymbol*);
typedef void (WINAPI *fpDrawTool)(CDC*,CMainSymbol*);
//전체 색상 변경
typedef void (WINAPI *fpChangeAllElementsColor)(const COLORREF&,const COLORREF&,CMainSymbol*);
// 20081007 JS.Kim	사용자시간대 선택
typedef bool (WINAPI *fpRecalcElementXPoint)(const char*, int, CMainSymbol*);

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
typedef void (WINAPI *fpSetXScaleType)(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, CMainSymbol*);
// 2011.02.07 by SYS <<


class CSymbolHelper : public CLoadDllLib  
{
public:
	fpSetMainBlock m_pfpSetMainBlock;
	fpInitialize m_pfpInitialize;
	fpSetDeskFilePath m_pfpSetDeskFilePath;
	fpSetCurrentSymbolType m_pfpSetCurrentSymbolType;
	fpSetSymbolCopy m_pfpSetSymbolCopy;
	fpSetShapeDrawingType m_pfpSetShapeDrawingType;	
	fpLoadSymbolDlg m_pfpLoadSymbolDlg;
	fpIsExistSymbolDlg m_pfpIsExistSymbolDlg;
	fpGetSavedElementsData m_pfpGetSavedElementsData;
	fpBuildElementsFromString m_pfpBuildElementsFromString;
	fpOnLButtonDown m_pfpOnLButtonDown;
	fpOnLButtonUp m_pfpOnLButtonUp;
	fpOnLButtonDbClk m_pfpOnLButtonDbClk;
	fpOnRButtonUp m_pfpOnRButtonUp;
	fpOnMouseMove m_pfpOnMouseMove;
	fpOnDraw m_pfpOnDraw;
	fpInsertElementTree m_pfpInsertElementTree;
	fpMoveElementTree m_pfpMoveElementTree;
	fpDeleteCurrentSymbol m_pfpDeleteCurrentSymbol;
	fpDeleteTail m_pfpDeleteTail;
	fpDeleteElementPos m_pfpDeleteElementPos;
	fpDeleteAllSymbol m_pfpDeleteAllSymbol;
	fpDeleteAllElementInBlock m_pfpDeleteAllElementInBlock;
	fpOnMenu m_pfpOnMenu;
	fpSetColor m_pfpSetColor;
	fpDestroySymbolDlg m_pfpDestroySymbolDlg;
	fpGetCurrentToolType m_pfpGetCurrentToolType;
	fpGetSelectToolType m_pfpGetSelectToolType;
	fpMakeMainSymbol m_pfpMakeMainSymbol;
	fpDeleteMainSymbol m_pfpDeleteMainSymbol;
	fpUndoElementPos m_pfpUndoElementPos;
	fpRedoElementPos m_pfpRedoElementPos;
	fpGetGraphTotalRegion m_pfpGetGraphTotalRegion;
	fpDrawTool m_pfpDrawTool;
	fpChangeAllElementsColor m_pfpChangeAllElementsColor;
	fpRecalcElementXPoint m_pfpRecalcElementXPoint;// 20081007 JS.Kim	사용자시간대 선택

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	fpSetXScaleType m_pfpSetXScaleType;
	// 2011.02.07 by SYS <<


	CMainSymbol* m_mainsymbol;	

	CSymbolHelper(LPCSTR _szDll)
	{
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"Symbol.dll");
		else
			m_szDllName = _T("Symbol.dll");
		m_hLib = NULL;
		MakeNull(m_pfpSetMainBlock);
		MakeNull(m_pfpInitialize);
		MakeNull(m_pfpSetDeskFilePath);
		MakeNull(m_pfpSetCurrentSymbolType);
		MakeNull(m_pfpSetShapeDrawingType);
		MakeNull(m_pfpLoadSymbolDlg);
		MakeNull(m_pfpIsExistSymbolDlg);
		MakeNull(m_pfpGetSavedElementsData);
		MakeNull(m_pfpBuildElementsFromString);
		MakeNull(m_pfpOnLButtonDown);
		MakeNull(m_pfpOnLButtonUp);
		MakeNull(m_pfpOnLButtonDbClk);
		MakeNull(m_pfpOnRButtonUp);
		MakeNull(m_pfpOnMouseMove);
		MakeNull(m_pfpOnDraw);	
		MakeNull(m_pfpInsertElementTree);		
		MakeNull(m_pfpMoveElementTree);	
		MakeNull(m_pfpDeleteCurrentSymbol);	
		MakeNull(m_pfpDeleteTail);	
		MakeNull(m_pfpDeleteElementPos);	
		MakeNull(m_pfpDeleteAllSymbol);		
		MakeNull(m_pfpDeleteAllElementInBlock);	
		MakeNull(m_pfpOnMenu);	
		MakeNull(m_pfpSetColor);		
		MakeNull(m_pfpDestroySymbolDlg);	
		MakeNull(m_pfpGetCurrentToolType);
		MakeNull(m_pfpGetSelectToolType);
		MakeNull(m_pfpMakeMainSymbol);	
		MakeNull(m_pfpDeleteMainSymbol);	
		MakeNull(m_pfpSetSymbolCopy);
		MakeNull(m_pfpUndoElementPos);
		MakeNull(m_pfpRedoElementPos);
		MakeNull(m_pfpGetGraphTotalRegion);
		MakeNull(m_pfpDrawTool);
		MakeNull(m_pfpChangeAllElementsColor);
		MakeNull(m_pfpRecalcElementXPoint);// 20081007 JS.Kim	사용자시간대 선택

		// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		MakeNull(m_pfpSetXScaleType);	
		// 2011.02.07 by SYS <<

		LoadLib();

		m_mainsymbol = MakeMainSymbol();
	}

	CSymbolHelper()
	{
		m_szDllName = _T("Symbol.dll");
		m_hLib = NULL;
		MakeNull(m_pfpSetMainBlock);
		MakeNull(m_pfpInitialize);
		MakeNull(m_pfpSetDeskFilePath);
		MakeNull(m_pfpSetCurrentSymbolType);
		MakeNull(m_pfpSetShapeDrawingType);
		MakeNull(m_pfpLoadSymbolDlg);
		MakeNull(m_pfpIsExistSymbolDlg);
		MakeNull(m_pfpGetSavedElementsData);
		MakeNull(m_pfpBuildElementsFromString);
		MakeNull(m_pfpOnLButtonDown);
		MakeNull(m_pfpOnLButtonUp);
		MakeNull(m_pfpOnLButtonDbClk);
		MakeNull(m_pfpOnRButtonUp);
		MakeNull(m_pfpOnMouseMove);
		MakeNull(m_pfpOnDraw);	
		MakeNull(m_pfpInsertElementTree);		
		MakeNull(m_pfpMoveElementTree);
		MakeNull(m_pfpDeleteCurrentSymbol);	
		MakeNull(m_pfpDeleteTail);		
		MakeNull(m_pfpDeleteElementPos);		
		MakeNull(m_pfpDeleteAllSymbol);		
		MakeNull(m_pfpDeleteAllElementInBlock);	
		MakeNull(m_pfpOnMenu);	
		MakeNull(m_pfpSetColor);
		MakeNull(m_pfpDestroySymbolDlg);				
		MakeNull(m_pfpGetCurrentToolType);
		MakeNull(m_pfpGetSelectToolType);
		MakeNull(m_pfpMakeMainSymbol);	
		MakeNull(m_pfpDeleteMainSymbol);	
		MakeNull(m_pfpSetSymbolCopy);
		MakeNull(m_pfpUndoElementPos);
		MakeNull(m_pfpRedoElementPos);
		MakeNull(m_pfpGetGraphTotalRegion);
		MakeNull(m_pfpDrawTool);
		MakeNull(m_pfpChangeAllElementsColor);
		MakeNull(m_pfpRecalcElementXPoint);		//// 20081007 JS.Kim	사용자시간대 선택

		// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		MakeNull(m_pfpSetXScaleType);
		// 2011.02.07 by SYS <<

		LoadLib();
		m_mainsymbol = MakeMainSymbol();
	}

	~CSymbolHelper()
	{
		DeleteMainSymbol(m_mainsymbol);
	}
		
	//MainBlock의 주소를 기호Dll에 Setting한다.
	//LoadSymbolDlg함수를 호출하기 전에 Setting되어야 한다.
	void SetMainBlock(IChartManager* pMainBlock,IChartOCX *p_pIChartOCX)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpSetMainBlock	= (fpSetMainBlock)::GetProcAddress(m_hLib, "SetMainBlock");
		if(m_pfpSetMainBlock == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpSetMainBlock(pMainBlock, m_mainsymbol,p_pIChartOCX);
		return; 
	}

	//Block정보 Settting
	void Initialize(CDC* pDC, CWnd* pParentWnd)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpInitialize	= (fpInitialize)::GetProcAddress(m_hLib, "Initialize_Block");
		if(m_pfpInitialize == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpInitialize(pDC, pParentWnd, m_mainsymbol);
		return; 
	}

	//Block정보 Settting
	void SetDeskFilePath(const CString& path)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpSetDeskFilePath	= (fpSetDeskFilePath)::GetProcAddress(m_hLib, "SetDeskFilePath");
		if(m_pfpSetDeskFilePath == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpSetDeskFilePath(path, m_mainsymbol);
		return; 
	}

	//현재 어떤 Symbol이 선택되었는지 알려준다.
	void SetCurrentSymbolType(const CSymbol::SYMBOLTOOL symbolType)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpSetCurrentSymbolType	= (fpSetCurrentSymbolType)::GetProcAddress(m_hLib, "SetCurrentSymbolType");
		if(m_pfpSetCurrentSymbolType == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpSetCurrentSymbolType(symbolType, m_mainsymbol);
		return; 
	}
	
	//Selected Symbol Copy
	void SetSymbolCopy()
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpSetSymbolCopy	= (fpSetSymbolCopy)::GetProcAddress(m_hLib, "SetSymbolCopy");
		if(m_pfpSetSymbolCopy == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpSetSymbolCopy(m_mainsymbol);
		return; 
	}

	//현재 선택된 도형의 속성 설정
	void SetShapeDrawingToolType(const COLORREF& crLine,const int& nStyle,const int& nWeight,const bool& bFill)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpSetShapeDrawingType = (fpSetShapeDrawingType)::GetProcAddress(m_hLib, "SetShapeDrawingToolType");
		if(m_pfpSetShapeDrawingType == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpSetShapeDrawingType(crLine,nStyle,nWeight,bFill,m_mainsymbol);
		return; 
	}

	//기호 Dlg를 띄운다.
	void LoadSymbolDlg()
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpLoadSymbolDlg	= (fpLoadSymbolDlg)::GetProcAddress(m_hLib, "LoadSymbolDlg");
		if(m_pfpLoadSymbolDlg == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpLoadSymbolDlg(m_mainsymbol);
		return; 
	}
	
	BOOL IsExistSymbolDlg()
	{
		if(!m_hLib) 
			return FALSE;//CS_LOADERROR;

		m_pfpIsExistSymbolDlg	= (fpIsExistSymbolDlg)::GetProcAddress(m_hLib, "IsExistSymbolDlg");
		if(m_pfpIsExistSymbolDlg == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return FALSE;//CS_LOADERROR;
		}
		
		return m_pfpIsExistSymbolDlg(m_mainsymbol);
	}

	//기호 Dlg를 띄운다.
	void DestroySymbolDlg()
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpDestroySymbolDlg	= (fpDestroySymbolDlg)::GetProcAddress(m_hLib, "DestroySymbolDlg");
		if(m_pfpDestroySymbolDlg == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpDestroySymbolDlg(m_mainsymbol);
		return; 
	}

	//화면저장을 위해서 String을 얻는다.
	CString GetSavedElementsData()
	{
		if(!m_hLib) 
			return "";//CS_LOADERROR;

		m_pfpGetSavedElementsData	= (fpGetSavedElementsData)::GetProcAddress(m_hLib, "GetSavedElementsData");
		if(m_pfpGetSavedElementsData == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return "";//CS_LOADERROR;
		}
		return m_pfpGetSavedElementsData(m_mainsymbol);
	}

	//저장 되어 있던 String으로 기호를 그린다.
	void BuildElementsFromString(const CString& Data)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpBuildElementsFromString	= (fpBuildElementsFromString)::GetProcAddress(m_hLib, "BuildElementsFromString");
		if(m_pfpBuildElementsFromString == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpBuildElementsFromString(Data, m_mainsymbol);
		return; 
	}

	//LButton이 눌렸을때.
	bool OnLButtonDown(CDC* pDC, const CPoint& point)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpOnLButtonDown	= (fpOnLButtonDown)::GetProcAddress(m_hLib, "LButtonDown");
		if(m_pfpOnLButtonDown == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpOnLButtonDown(pDC, point, m_mainsymbol);
	}

	//LButtonUp 했을때
	bool OnLButtonUp(CDC* pDC, const CPoint& point)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpOnLButtonUp	= (fpOnLButtonUp)::GetProcAddress(m_hLib, "LButtonUp");
		if(m_pfpOnLButtonUp == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpOnLButtonUp(pDC, point, m_mainsymbol); 
	}

	//LButtonDbClk 했을때
	bool OnLButtonDbClk(CDC* pDC, const CPoint& point)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpOnLButtonDbClk	= (fpOnLButtonDbClk)::GetProcAddress(m_hLib, "LButtonDbClk");
		if(m_pfpOnLButtonDbClk == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpOnLButtonDbClk(pDC, point, m_mainsymbol); 
	}

	//RButtonUp 했을때..
	bool OnRButtonUp(const CPoint& point)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpOnRButtonUp	= (fpOnRButtonUp)::GetProcAddress(m_hLib, "RButtonUp");
		if(m_pfpOnRButtonUp == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpOnRButtonUp(point, m_mainsymbol);  
	}

	//MouseMove했을때.
	bool OnMouseMove(CDC* pDC,const UINT &nFlags, const CPoint& point)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpOnMouseMove	= (fpOnMouseMove)::GetProcAddress(m_hLib, "MouseMove");
		if(m_pfpOnMouseMove == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpOnMouseMove(pDC,nFlags, point, m_mainsymbol);
	}

	//ocx OnDraw시 호출한다.
	void OnDraw(CDC* pDC)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpOnDraw	= (fpOnDraw)::GetProcAddress(m_hLib, "ReDraw");
		if(m_pfpOnDraw == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpOnDraw(pDC, m_mainsymbol);
		return; 
	}

	//블럭이 Insert 되었을때 호출한다.
	void InsertElementTree(const CBlockIndex &insertBlockIndex)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpInsertElementTree = (fpInsertElementTree)::GetProcAddress(m_hLib, "InsertElementTree");
		if(m_pfpInsertElementTree == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpInsertElementTree(insertBlockIndex, m_mainsymbol);
		return; 
	}

	//이 함수는 Object에서 블럭이 이동되었을때 호출한다.
	void MoveElementTree(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpMoveElementTree = (fpMoveElementTree)::GetProcAddress(m_hLib, "MoveElementTree");
		if(m_pfpMoveElementTree == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpMoveElementTree(sourceBlockIndex, TargetBlockIndex, m_mainsymbol);
		return; 
	}

	//현재 선택한 기호만을 삭제한다.
	void DeleteCurrentSymbol()
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpDeleteCurrentSymbol = (fpDeleteCurrentSymbol)::GetProcAddress(m_hLib, "DeleteCurrentSymbol");
		if(m_pfpDeleteCurrentSymbol == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpDeleteCurrentSymbol(m_mainsymbol);
		return; 
	}

	//순서대로 지우기. 맨마지막에 생성된 객체를 지운다.
	void DeleteTail()
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpDeleteTail = (fpDeleteTail)::GetProcAddress(m_hLib, "DeleteTail");
		if(m_pfpDeleteTail == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpDeleteTail(m_mainsymbol);
		return; 
	}

	//순서대로 지우기. 맨마지막에 생성된 객체를 지운다.
	bool DeleteElementPos(const long& lAddress)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpDeleteElementPos = (fpDeleteElementPos)::GetProcAddress(m_hLib, "DeleteElementPos");
		if(m_pfpDeleteElementPos == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpDeleteElementPos(m_mainsymbol,lAddress);
	}
	
	//ocx 포함되어 있는 모든 기호를 삭제한다.
	void DeleteAllSymbol()
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpDeleteAllSymbol = (fpDeleteAllSymbol)::GetProcAddress(m_hLib, "DeleteAllSymbol");
		if(m_pfpDeleteAllSymbol == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpDeleteAllSymbol(m_mainsymbol);
		return; 
	}

	//선택된 블럭안의 모든 기호를 지운다.
	void DeleteAllElementInBlock(const CBlockIndex& blockIndex)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpDeleteAllElementInBlock = (fpDeleteAllElementInBlock)::GetProcAddress(m_hLib, "DeleteAllElementInBlock");
		if(m_pfpDeleteAllElementInBlock == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpDeleteAllElementInBlock(blockIndex, m_mainsymbol);
		return; 
	}

	//기호 색깔을 Setting 한다.
	bool OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpOnMenu = (fpOnMenu)::GetProcAddress(m_hLib, "RButtonMenu");
		if(m_pfpOnMenu == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpOnMenu(menuType, wParam, m_mainsymbol); 
	}

	//기호 색깔 초기값을 Setting 한다.
	void SetColor(const COLORREF& color)
	{
		if(!m_hLib) 
			return;//CS_LOADERROR;

		m_pfpSetColor = (fpSetColor)::GetProcAddress(m_hLib, "SetToolColor");
		if(m_pfpSetColor == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return;//CS_LOADERROR;
		}
		m_pfpSetColor(color, m_mainsymbol); 
	}

	//기호 색깔 초기값을 Setting 한다.
	CSymbol::SYMBOLTOOL GetCurrentToolType()
	{
		if(!m_hLib) 
			return CSymbol::SYMBOLTOOL(0);//CS_LOADERROR;

		m_pfpGetCurrentToolType = (fpGetCurrentToolType)::GetProcAddress(m_hLib, "GetCurrentSymbolToolType");
		if(m_pfpGetCurrentToolType == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CSymbol::SYMBOLTOOL(0);//CS_LOADERROR;
		}
		return m_pfpGetCurrentToolType(m_mainsymbol); 
	}

	CSymbol::SYMBOLTOOL GetSelectToolType()
	{
		if(!m_hLib) 
			return CSymbol::SYMBOLTOOL(0);//CS_LOADERROR;

		m_pfpGetSelectToolType = (fpGetSelectToolType)::GetProcAddress(m_hLib, "GetSelectToolType");
		if(m_pfpGetSelectToolType == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CSymbol::SYMBOLTOOL(0);//CS_LOADERROR;
		}
		return m_pfpGetSelectToolType(m_mainsymbol); 
	}

	CMainSymbol* MakeMainSymbol()
	{
		if(!m_hLib) 
			return NULL;//CS_LOADERROR;

		m_pfpMakeMainSymbol = (fpMakeMainSymbol)::GetProcAddress(m_hLib, "MakeMainSymbol");
		if(m_pfpMakeMainSymbol == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;//CS_LOADERROR;
		}
		return m_pfpMakeMainSymbol(); 
	}

	void DeleteMainSymbol(CMainSymbol* pMainSymbol)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpDeleteMainSymbol = (fpDeleteMainSymbol)::GetProcAddress(m_hLib, "DeleteMainSymbol");
		if(m_pfpDeleteMainSymbol == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpDeleteMainSymbol(pMainSymbol); 
	}

	//UndoElement
	bool UndoElementPos(const long& lAddress)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpUndoElementPos = (fpUndoElementPos)::GetProcAddress(m_hLib, "UndoElementPos");
		if(m_pfpUndoElementPos == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpUndoElementPos(m_mainsymbol,lAddress);
	}

	//RedoElement
	bool RedoElementPos(const long& lAddress)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpRedoElementPos = (fpRedoElementPos)::GetProcAddress(m_hLib, "RedoElementPos");
		if(m_pfpRedoElementPos == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpRedoElementPos(m_mainsymbol,lAddress);
	}

	//RedoElement
	CRect GetGraphTotalRegion(const CPoint& point)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpGetGraphTotalRegion = (fpGetGraphTotalRegion)::GetProcAddress(m_hLib, "GetGraphTotalRegion");
		if(m_pfpGetGraphTotalRegion == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpGetGraphTotalRegion(point,m_mainsymbol);
	}

	//RedoElement
	void DrawTool(CDC* pDC)
	{
		if(!m_hLib) 
			return;//CS_LOADERROR;

		m_pfpDrawTool = (fpDrawTool)::GetProcAddress(m_hLib, "DrawTool");
		if(m_pfpDrawTool == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return;//CS_LOADERROR;
		}
		m_pfpDrawTool(pDC,m_mainsymbol);
	}

	void ChangeAllElementsColor(const COLORREF& crLine,const COLORREF& crText)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;

		m_pfpChangeAllElementsColor = (fpChangeAllElementsColor)::GetProcAddress(m_hLib, "ChangeAllElementsColor");
		if(m_pfpChangeAllElementsColor == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpChangeAllElementsColor(crLine,crText,m_mainsymbol);
		return; 
	}

	// 20081007 JS.Kim	사용자시간대 선택
	bool RecalcElementXPoint(const char* szName, int nTimeDiff)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpRecalcElementXPoint = (fpRecalcElementXPoint)::GetProcAddress(m_hLib, "RecalcElementXPoint");
		if(m_pfpRecalcElementXPoint == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpRecalcElementXPoint(szName, nTimeDiff, m_mainsymbol); 
	}

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	void SetXScaleType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType)
	{
		if(!m_hLib) 
			return ;//CS_LOADERROR;
		
		m_pfpSetXScaleType	= (fpSetXScaleType)::GetProcAddress(m_hLib, "SetXScaleType");
		if(m_pfpSetXScaleType == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return ;//CS_LOADERROR;
		}
		m_pfpSetXScaleType(eScaleType, m_mainsymbol);
		return; 
	}
	// 2011.02.07 by SYS <<

};

#endif // !defined(AFX_SYMBOLHELPER_H__D2022450_936F_4315_899B_E517896270BA__INCLUDED_)
