// BaseToolSingleton.h: interface for the CBaseToolSingleton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASETOOLSINGLETON_H__8BF57596_D64E_4096_9484_FFF849741D7D__INCLUDED_)
#define AFX_BASETOOLSINGLETON_H__8BF57596_D64E_4096_9484_FFF849741D7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockTool.h"
#include "ScaleTool.h"
#include "GraphTool.h"

// 블럭 최대화 추가 - ojtaso (20080519)
interface IPropertyMap;
class CBlockToolSingleton
{
public:
	// 블럭 최대화 추가 - ojtaso (20080519)
	CBlockToolSingleton(const bool bUsingMoveTool, const bool bUsingDeleteTool, const bool bUsingSizeTool);

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	// tool을 사용할지 여부
	void SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing);

	CBlockTool* GetTool(CMainBlock* pMainBlock, const CPoint& point);

	void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

private:
	bool m_bUsingMoveTool;
	bool m_bUsingDeleteTool;
	// 블럭 최대화 추가 - ojtaso (20080519)
	bool m_bUsingSizeTool;
	CBlockMoveTool m_blockMoveTool;
	CBlockDeleteTool m_blockDeleteTool;
	// 블럭 최대화 추가 - ojtaso (20080519)
	CBlockSizeTool m_blockSizeTool;
};

class CScaleToolSingleton
{
public:
	CScaleToolSingleton(const bool bUsingHorzTool, const bool bUsingVertTool);

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	// tool을 사용할지 여부
	void SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing);

	CScaleTool* GetTool(CMainBlock* pMainBlock, const CPoint& point);

private:
	bool m_bUsingHorzTool;
	bool m_bUsingVertTool;
	CHorzScaleTool m_horzScaleTool;
	CVertScaleTool m_vertScaleTool;
};

class CGraphToolSingleton
{
public:
	CGraphToolSingleton(const bool bUsingTitleTool, const bool bUsingSubGraphTool);

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	// tool을 사용할지 여부
	void SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing);

	// (2006/11/1 - Seung-Won, Bae) Support Graph Title First.
	CGraphTool* GetTitleTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useSubGraphTool = true);
	CGraphTool* GetShapeTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useSubGraphTool = true);

private:
	bool m_bUsingTitleTool;
	bool m_bUsingSubGraphTool;
	CSubGraphTitleTool m_subGraphTitleTool;
	CSubGraphTool m_subGraphTool;
};

// ----------------------------------------------------------------------------
class CBaseToolSingleton  
{
public:
	CBaseToolSingleton();

	void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);
	// tool을 사용할지 여부
	void SetUsingTool(const CBlockBaseData::TOOLTYPE eToolType, const bool bIsUsing);

	CBaseTool* GetTool(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point, const bool useScaleTool = true, const bool useSubGraphTool = true);

	void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

private:
	bool m_bUsingComparLineTool;
	CCompartLineTool m_compartLineTool;
	CBlockToolSingleton m_blockTool;
	CScaleToolSingleton m_scaleTool;
	CGraphToolSingleton m_graphTool;
};

#endif // !defined(AFX_BASETOOLSINGLETON_H__8BF57596_D64E_4096_9484_FFF849741D7D__INCLUDED_)
