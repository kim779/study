// MainAnalysisTool.h: interface for the CMainAnalysisTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINANALYSISTOOL_H__3FDFEE5F_7570_4D60_9191_CFCFFE6115D7__INCLUDED_)
#define AFX_MAINANALYSISTOOL_H__3FDFEE5F_7570_4D60_9191_CFCFFE6115D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"			// for Ixxxxxx.h
#include "../Include_Analysis/ToolBaseData.h"
#include "../Include_Analysis/BlockBaseEnum2.h"			// for CAnalysis
#include "BlockBaseData.h"

class CMainToolImplementation;

#ifndef AFX_EXT_CLASS_ANALYSISTOOL
	#ifdef _ANALYSISTOOL_EXPORT								// in MainAnalysisTool
		#define AFX_EXT_CLASS_ANALYSISTOOL	AFX_CLASS_EXPORT
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_ANALYSISTOOL	AFX_CLASS_IMPORT
	#endif
#endif

class AFX_EXT_CLASS_ANALYSISTOOL CMainAnalysisTool  
{
public:
//	CMainAnalysisTool();
	CMainAnalysisTool( CWnd* pParent, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	virtual ~CMainAnalysisTool();

	int GetElementCount() const;
	CString GetSavedElementsData() const;
	CAnalysis::TOOLTYPE GetCurrentToolType() const;
	CToolEnumData::ElementType GetSelectedToolType() const;
	bool GetData_Line(CList<CPoint, CPoint&>& elementData, double& dVertDataType) const;

	bool OnToolCopy();		//추세선 복사

	void SetParent(CWnd* pParent);
	void SetMainBlock( IChartManager* pMainBlock, IChartOCX *p_pIChartOCX);
	void SetAutoPriceTool(const bool bIsAutoPrice);
	void SetAutoPriceElement(const bool bIsAutoPrice);
	void SetBaseColor(const COLORREF& penColor, const COLORREF& textColor);
	void SetBasePenColor(const COLORREF& color);
	void SetBasePenThickness(const int nThickness);
	void SetLineDrawingInfo(const CAnalysis::TOOLTYPE eToolType, const COLORREF crLine, const int nStyle, const int nWeight);
	void SetTextPosition(const CAnalysis::TOOLTYPE eToolType, const bool bLeft, const bool bRight, const bool bTop, const bool bBottom, const bool bRightRate=true);
	void SetLineExtEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nLeftExt,const int& nRightExt);
	void SetEquiDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nEquiDiv);
	void SetPriceRangeDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nPriceRangeDiv);	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	void SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetCurrentAnalysisTool(const CAnalysis::TOOLTYPE eToolTYPE);
	void SetFiboCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient);//// 2008.08.20 김진순 피보나치 확장	
	void SetBalanceCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient); // 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
	
	// mouse event
	bool OnLButtonDown(CDC* pDC, const CPoint& point);
	bool OnLButtonDbClk(CDC* pDC, const CPoint& point);
	bool OnLButtonUp(CDC* pDC, const CPoint& point);
	bool OnMouseMove(CDC* pDC, const CPoint& point,int nFlags,const HBITMAP p_hbmpMainOSB);
	CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, const CPoint& point);
	bool OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	CRect GetGraphTotalRegion(CPoint point);		
	BOOL  GetCurrentPosData(CPoint point,CString &strTextRight,CString &strTextBottom);

	// 그리기
	void OnDraw(CDC* pDC,BOOL bDrawAfter=FALSE);
	void DrawTool(CDC* pDC);
	
	// element 만들기.
	void BuildElementsFromString(const CString& strElementsData);
	// block 추가시 elementTree을 추가하기.
	bool InsertElementTree(const CBlockIndex& insertBlockIndex);
	// block 이동
	bool MoveElementTree(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);

	// element 삭제
	void DeleteAllElement();
	bool DeleteElement(const CToolEnumData::ElementType eElementTYPE);
	bool DeleteCurrentElement();
	void DeleteTail();
	BOOL DeleteElementPos(const long lAddress);
	void DeleteAllElementInBlock(const CBlockIndex& blockIndex);

	// element 색 변경
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);
	
	// UnDo/Redo 
	BOOL UndoElement(long lUndoID);
	BOOL RedoElement(long lRedoID);

	BOOL DoesAction() const;

private:
	CMainToolImplementation* m_pMainToolImpl;
protected:
	void	Set( CWnd* pParent, IChartManager* pMainBlock, IChartOCX *p_pIChartOCX, const COLORREF& color, 
				const bool bIsAutoPrice = false, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

// 20081007 JS.Kim	사용자시간대 선택
public:
	bool	RecalcElementXPoint( const char* szName, int nTimeDiff);
};

#endif // !defined(AFX_MAINANALYSISTOOL_H__3FDFEE5F_7570_4D60_9191_CFCFFE6115D7__INCLUDED_)
