// ScaleTool.h: interface for the CScaleTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALETOOL_H__70F0FC81_C6FF_454D_A2A2_9A3F5F914081__INCLUDED_)
#define AFX_SCALETOOL_H__70F0FC81_C6FF_454D_A2A2_9A3F5F914081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"

class CMainBlock;
class CBlockImp;
class CHorizontalScalePtrDataList;
class CVerticalScalePtrDataList;

// scale에 관한 Tool
class CScaleTool : public CBaseTool
{
public:
	CScaleTool();

	// Tool의 모든 내용을 초기화
	virtual void InitializeAllData();

	// mouse
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	// 해당 block 번호 찾는다.
	bool Find(CMainBlock* pMainBlock, const CPoint& point);

protected:
	virtual CMenuType::MENUTYPE OnRButtonUp() = 0;
	virtual bool OnMenu_ScaleSet(CMainBlock* pMainBlock, const int nMarginCount) = 0;
	virtual bool FindScale(CBlockImp* pBlock, const CPoint& point) = 0;

	CBlockImp* GetBlock() const;

private:
	CBlockImp* m_pBlock;

	bool OnMenu(CMainBlock* pMainBlock, const CMenuType::ELEMENTMENU menuType, const int nMenuData);
	bool OnMenu_HorzScalePosition(CMainBlock* pMainBlock, const CScaleBaseData::HORZSCALEPOSITION horzPosition);
};


// 가로 scale에 관한 Tool
class CHorzScaleTool : public CScaleTool
{
public:
	CHorzScaleTool();

	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);

protected:
	virtual CMenuType::MENUTYPE OnRButtonUp();
	virtual bool OnMenu_ScaleSet(CMainBlock* pMainBlock, const int nMarginCount);
	virtual bool FindScale(CBlockImp* pBlock, const CPoint& point);

private:
	void GetHorzScalePtrDataList(CMainBlock* pMainBlock, const int nMarginCount, CHorizontalScalePtrDataList& horzScalePtrDataList);
};


// 세로 scale에 관한 Tool
class CVertScaleTool : public CScaleTool
{
public:
	CVertScaleTool();

	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDblClk(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);

protected:
	virtual CMenuType::MENUTYPE OnRButtonUp( CDC* /*pDC*/, CMainBlock* pMainBlock, const CPoint& point);
	virtual CMenuType::MENUTYPE OnRButtonUp();
	virtual bool OnMenu_ScaleSet(CMainBlock* pMainBlock, const int nMarginCount);
	virtual bool FindScale(CBlockImp* pBlock, const CPoint& point);

private:
	void GetVertScalePtrDataList(CMainBlock* pMainBlock, CVerticalScalePtrDataList& vertScalePtrDataList);
	
	void SendMessage_VertScaleSetting();

// (2007/7/13 - Seung-Won, Bae) Reset Selected VScale with Double Click or Popup Menu.
protected:
	CPoint	m_ptSelectedPoint;
protected:
	bool	ShowVertScaleSettingDlg( CMainBlock* pMainBlock, const int p_nSelectedIndex = -1);
};

#endif // !defined(AFX_SCALETOOL_H__70F0FC81_C6FF_454D_A2A2_9A3F5F914081__INCLUDED_)
