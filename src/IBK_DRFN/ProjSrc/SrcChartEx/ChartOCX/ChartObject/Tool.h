// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOL_H__8271865E_145F_401B_9A3F_DD98FCC88A86__INCLUDED_)
#define AFX_TOOL_H__8271865E_145F_401B_9A3F_DD98FCC88A86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseData.h"

interface IChartCtrl;
class CMainBlock;

class CBaseToolEnumData
{
public:
	enum CURSORTYPE
	{
		CTNONE = 0,		// cursor 변경 없음
		SPLITE_ROW,		// block 가로위치 변경 cursor
		SPLITE_COLUMN,	// block 세로위치 변경 cursor
		SPLITE_ALL,		// block의 모든 위치 변경 cursor
		SELECT,			// 객체 선택 cursor
		MOVE,			// 선택된 객체 이동
		NODROP,			// 선택된 객체 이동 안됨
		REMOVE,			// Delete
		INSERT,			// Insert Graph 분리/삽입 - ojtaso (20070731)
	};
};


// 기본 base Tool
class CBaseTool
{
public:
	CBaseTool();

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	IChartCtrl* GetIChartCtrl() const;
	CWnd* GetParent() const;
	HWND GetParent_Handle();

	// Tool의 작업 완료여부 (true = 작업완료)
	bool DoesProcessEnded() const;

	// Tool의 모든 내용을 초기화
	virtual void InitializeAllData();
	// 자신의 ToolType을 가져오기
	virtual CBlockBaseData::TOOLTYPE GetToolType() const = 0;

	// mouse
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);

	virtual bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	// 지표 뒤집기 - ojtaso (20071023)
	virtual bool GetMenuCheck(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

protected:
	void ChangeCursor(const CBaseToolEnumData::CURSORTYPE& cursorType);
	void SetProcessEnded(const bool isProcessEnded);

	void Invalidate(const bool bErase = true);
	void InvalidateRect(const CRect& region, const bool bErase = true);

	int GetMargin() const;

protected:
	IChartCtrl* m_pIChartCtrl; // 부모(OCX) Interface
	bool m_bProcessEnded;	// Tool의 작업이 종료 여부. (true = 종료)
};

#endif // !defined(AFX_TOOL_H__8271865E_145F_401B_9A3F_DD98FCC88A86__INCLUDED_)
