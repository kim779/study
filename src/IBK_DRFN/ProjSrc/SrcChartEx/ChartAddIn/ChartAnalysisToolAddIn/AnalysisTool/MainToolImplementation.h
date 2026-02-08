// MainToolImplementation.h: interface for the CMainToolImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINTOOLIMPLEMENTATION_H__5FD50F5D_9B3C_40E5_B232_F68DD123275C__INCLUDED_)
#define AFX_MAINTOOLIMPLEMENTATION_H__5FD50F5D_9B3C_40E5_B232_F68DD123275C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "LabelWnd.h"
#include "AnalysisData.h"
#include "ToolSingleton.h"
#include "../Include/ICanCallYou.h"

class CTool;
class CElement;

class CMainToolImplementation  
{
public:
	class CCanCallYou : public ICanCallYou
	{
	public:
		STDMETHOD(DoCallYou)(int nCallType, LONG dwData);
	} m_xCanCallYou;
	friend class CCanCallYou;

public:
	CMainToolImplementation( CWnd* pParent, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

	int GetElementCount() const;
	CString GetSavedElementsData() const;
	CAnalysis::TOOLTYPE GetCurrentToolType() const;
	CToolEnumData::ElementType GetSelectedToolType() const;
	bool GetData_Line(CList<CPoint, CPoint&>& elementData, double& dVertDataType) const;

	bool OnToolCopy();		//추세선 복사

	void SetParent(CWnd* pParent);
	void SetMainBlock( IChartManager* pMainBlock, IChartOCX *p_pIChartOCX);
	void SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetAutoPriceTool(const bool bIsAutoPrice);
	void SetAutoPriceElement(const bool bIsAutoPrice);
	void SetBaseColor(const COLORREF& penColor, const COLORREF& textColor);
	void SetBasePenColor(const COLORREF& color);
	void SetBasePenThickness(const int nThickness);
	void SetLineDrawingInfo(const CAnalysis::TOOLTYPE eToolType, const COLORREF crLine, const int nStyle, const int nWeight);
	void SetLineExtEnvironment(const CAnalysis::TOOLTYPE eToolType, const int& nLeftExt, const int& nRightExt);
	void SetTextPosition(const CAnalysis::TOOLTYPE eToolType, const bool bLeft, const bool bRight, const bool bTop, const bool bBottom, const bool bRightRate=true);
	void SetEquiDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nEquiDiv);
	void SetPriceRangeDivEnvironment(const CAnalysis::TOOLTYPE eToolType,const int& nPriceRangeDiv);	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	void SetCurrentAnalysisTool(const CAnalysis::TOOLTYPE eToolTYPE,BOOL bRedraw = TRUE);
	void SetFiboCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strFiboCoefficient);//// 2008.08.20 김진순 피보나치 확장	
	void SetBalanceCoefficient(const CAnalysis::TOOLTYPE eToolType,const CString& strBalanceCoefficient); // 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청

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
	void OnDraw(CDC* pDC);
	void DrawTool(CDC *pDC);
	
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

	//Undo Redo 
	BOOL UndoElement(long lUndoID);
	BOOL RedoElement(long lRedoID);
private:
	IChartOCX	*m_pIChartOCX;
	CPoint m_ptOldLBtnUp;
	CPoint m_ptOldMouse;
	CTool* m_pTool;	
	CAnalysisData m_analysisData;
	CElement* m_pSelectedElement;
	CToolSingleton m_toolSingleton;		
	
	void Initialize();
	// 현재 Tool이 select tool 인지의 여부. (true = selectTool)
	bool DoesCurrentSelectTool() const;
	// 현재 tool이 다중 선택 tool 인지 여부.
	bool DoesCurrentMultiTool() const;

	bool GetData(const CElementPointList& elementPointList, CList<CPoint, CPoint&>& elementData, double& dVertDataType) const;
	CMenuType::MENUTYPE GetRButtonMenuType(const CPoint& point);

	void SetElementDelete();

public:
	// tool 을 실행할지의 여부. (true = 실행)
	bool DoesAction() const;

// 20081007 JS.Kim	사용자시간대 선택
public:
	bool	RecalcElementXPoint(const char* szName, int nTimeDiff);
	void	RecalcElementListXPoint(int row, int col, int nBaseTimeDiff, IPacket* pPacket);
};

#endif // !defined(AFX_MAINTOOLIMPLEMENTATION_H__5FD50F5D_9B3C_40E5_B232_F68DD123275C__INCLUDED_)
