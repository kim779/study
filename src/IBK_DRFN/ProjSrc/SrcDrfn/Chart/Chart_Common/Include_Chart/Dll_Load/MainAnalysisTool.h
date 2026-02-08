// MainAnalysisTool.h: interface for the CMainAnalysisTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINANALYSISTOOL_H__3FDFEE5F_7570_4D60_9191_CFCFFE6115D7__INCLUDED_)
#define AFX_MAINANALYSISTOOL_H__3FDFEE5F_7570_4D60_9191_CFCFFE6115D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseData.h"
#include "ToolBaseData.h"

class CMainBlock;
class CMainToolImplementation;

class AFX_EXT_CLASS CMainAnalysisTool  
{
public:
	CMainAnalysisTool();
	CMainAnalysisTool(CWnd* pParent);
	virtual ~CMainAnalysisTool();

	int GetElementCount() const;
	CString GetSavedElementsData() const;
	CAnalysis::TOOLTYPE GetCurrentToolType() const;
	bool GetData_Line(CList<CPoint, CPoint&>& elementData, double& dVertDataType) const;

	void Set(CWnd* pParent, CMainBlock* pMainBlock, const COLORREF& color, 
		const bool bIsAutoPrice = false, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	void SetParent(CWnd* pParent);
	void SetMainBlock(CMainBlock* pMainBlock);
	void SetAutoPriceTool(const bool bIsAutoPrice);
	void SetAutoPriceElement(const bool bIsAutoPrice);
	void SetBaseColor(const COLORREF& penColor, const COLORREF& textColor);
	void SetBasePenColor(const COLORREF& color);
	void SetBasePenThickness(const int nThickness);
	void SetTextPosition(const CAnalysis::TOOLTYPE eToolType, const bool bLeft, const bool bRight, const bool bTop, const bool bBottom);
	void SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType);
	void SetCurrentAnalysisTool(const CAnalysis::TOOLTYPE eToolTYPE);
	
	// mouse event
	bool OnLButtonDown(CDC* pDC, const CPoint& point);
	bool OnLButtonUp(CDC* pDC, const CPoint& point);
	bool OnMouseMove(CDC* pDC, const CPoint& point);
	CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, const CPoint& point);
	bool OnMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	// 그리기
	void OnDraw(CDC* pDC);
	

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
	void DeleteAllElementInBlock(const CBlockIndex& blockIndex);

	// element 색 변경
	void ChangeAllElementsColor(const COLORREF& penColor, const COLORREF& textColor);

private:
	CMainToolImplementation* m_pMainToolImpl;
};

#endif // !defined(AFX_MAINANALYSISTOOL_H__3FDFEE5F_7570_4D60_9191_CFCFFE6115D7__INCLUDED_)
