// ObjEvent.cpp: implementation of the CObjEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axisGMain.h"
#include "ObjEvent.h"
#include "ObjMgr.h"
#include "DataFormat.h"
#include "GrpGrid.h"
#include "CrossLine.h"
#include "DaedeungDlg.h"
#include "UserInputdlg.h"
#include "ToolTip.h"

#include "../gTool/ToolMgr.h"
#include "../gIndc/GraphMgr.h"
#include "../../h/menudef.h"

#include "PnChart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	rgnGAP	2
#define	markGAP	9
#define	splitMINIUM	2

const int modifyMENUID= 4100;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjEvent::CObjEvent(CWnd *pWnd, class CObjMgr *pObjMgr)
{
	m_pParent = pWnd;
	m_pObjMgr = pObjMgr;
	m_pToolTip = NULL;

	m_iLeftGap = 10;
	m_iRightGap = 10;
	m_nIimerID = 0;
	m_pTipIndc = NULL;

	m_bSelctEtc = false;
	m_bOrderMode = false;

	_envInfo *envinfo = m_pObjMgr->GetEnvInfo();
	if (envinfo->wEnvOption & EO_MAP)
	{
		m_bMapChart = true;
		m_bShowTip = true;
	}
	else
	{
		m_bMapChart = false;
		if (envinfo->display.dwDspOption & DO_NOVIEWTIP)
			m_bShowTip = false;
		else
			m_bShowTip = true;
	}

	m_iBMCnt = 0;
	ZeroMemory(m_acBookMark, sizeof(m_acBookMark));

	Init();
}

CObjEvent::~CObjEvent()
{
	if (m_pToolTip)
	{
		delete m_pToolTip;
		m_pToolTip = NULL;
	}
}

void CObjEvent::Init()
{
	m_iToolSelectIndex = -1;
	m_iTkChoiced = (int)enumTK::tkNONE;
	m_dwObjStatus = 0;
	m_bMouseCaptureTCreate = FALSE;
	m_bGrpSelect = false;
	m_pindcSelect = NULL;
	m_dwObjOpr = 0x00;
	// 2006.11.20 lsh (start)
	m_strClickDateTime = _T("");
	m_strClickPrice = _T("");
	// 2006.11.20 lsh (end)
	
	m_pointStartSize = CPoint(-1, -1);
	m_pointEndSize = CPoint(-1, -1);
	m_iDragRgnKey = -1;
	m_rectDrag.SetRectEmpty();

	RemoveDisplayStack();
	RemoveSplitMap();
	RemoveMarkMap();

	m_bMouseRelease = false;
	m_iCaptureSelectTool = 0;

	RemoveToolQue();
	if (!m_pToolTip)
	{
		m_pToolTip = new CToolTip(m_pObjMgr->m_pwndView, this);
		m_pToolTip->Create();
	}

	ViewGrid(false);
}

void CObjEvent::LButtonDownEvent(CPoint point)
{
	if (m_pObjMgr->IsOrderMode())
		return;

	_envInfo* penvinfo = m_pObjMgr->GetEnvInfo();
	if ((penvinfo->display.dwDspOption & DO_LBTNDNSHOW) && (penvinfo->display.dwDspOption & DO_VIEWGRID))
		ViewGrid(false);
	else
		m_pObjMgr->HideCrossLine();

	if (m_dwObjStatus & GOS_autoScroll)
		return;

	if (!(m_dwObjStatus & GOS_toolState))
	{
		CRect rectSplit;
		int iRKey = IsSplitPos(point, rectSplit);
		if (!rectSplit.IsRectEmpty())
		{
			m_dwObjStatus &= ~GOM_overSplit;
			m_dwObjStatus |= GOS_dragSplit;
			CRect clipRC = GetSplitMoveRect(iRKey);
			SetStateCursor(&clipRC);
			m_rectSplitCur.SetRect(m_pObjMgr->m_rectObj.left, point.y, m_pObjMgr->m_rectObj.right, point.y + 3);
			m_rectSplitPre.SetRectEmpty();
			DrawSplit();

			return;
		}
	}

	CPoint pointAdjust;
	CToolBase* pToolBase = NULL;
	CIndcBase* pIndcBase = SearchPtInGraph(point);
	enumPointWhat pointOWNER = static_cast<enumPointWhat>(SearchPtInRegion(point));

	int iRgnKey;
	bool bLabel;
	switch (pointOWNER)
	{
	case enumPointWhat::pointGRAPH:
		if (m_iToolSelectIndex >= 0 && m_arToolQue.GetSize() > 0)
		{
			pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			pToolBase->HideHolder();
		}

		if (!SearchPtInTool(point))
		{
			m_dwObjStatus &= ~GOS_toolState;
			m_iToolSelectIndex = -1;
		}
		if (!pIndcBase)
			return;

		switch (m_dwObjStatus & GOS_toolState)
		{
		case GOS_toolCreate:
			if (m_pindcSelect)
			{
				m_pindcSelect->ResetSelRgn();
				m_pObjMgr->ReDrawGraphObject(true);
				m_pindcSelect = NULL;
				m_bGrpSelect = false;
			}
			if (m_iTkChoiced != static_cast<int>(enumTK::tkUSRINPUT))
			{
				pToolBase = AddToolQue(m_iTkChoiced, pIndcBase);
				if (pToolBase == (CToolBase *) 0)
					return;
				if (pToolBase->m_eToolKind == enumTK::tkCROSS)
					GetCrossValue(pToolBase, point);

				pToolBase->CreateToolLBDN(point, m_bMouseCaptureTCreate);
			}

			m_dwObjStatus |= GOS_toolCreate;
			m_pointDown = point;
			break;
		case GOS_toolChange:
			if (m_pindcSelect)
			{
				m_pindcSelect->ResetSelRgn();
				m_pObjMgr->ReDrawGraphObject(true);
				m_pindcSelect = NULL;
				m_bGrpSelect = false;
			}

			if (m_iToolSelectIndex < 0)
				break;
			if (m_arToolQue.GetSize() > 0)
				pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			if (!pToolBase)
				return;
			if (!pToolBase->IsVisibleHolder())
			{
				pToolBase->DrawHolder(true);
//				m_pObjMgr->ReDrawGraphObject(true);
			}
			
			pToolBase->ChangeToolLBDN(point, m_bMouseCaptureTCreate);

			if (pToolBase->GetToolName() == static_cast<int>(enumTK::tkUSRINPUT))
			{
				m_dwObjStatus &= ~GOS_toolChange;
				return;
			}
			break;
		case GOS_toolMove:
			if (m_pindcSelect)
			{
				m_pindcSelect->ResetSelRgn();
				m_pObjMgr->ReDrawGraphObject(true);
				m_pindcSelect = NULL;
				m_bGrpSelect = false;
			}

			if (m_iToolSelectIndex < 0)
				break;
			if (m_arToolQue.GetSize() > 0)
				pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			if (pToolBase)
			{
				if (pToolBase->m_eToolKind == enumTK::tkCROSS)
					GetCrossValue(pToolBase, point);
				pToolBase->MoveToolLBDN(point, m_bMouseCaptureTCreate);
			}
			break;
		default:
			ToolHideHolder();
			pIndcBase = IsPtOverLine(point, true, bLabel);
			if (pIndcBase)
			{
				if (m_pindcSelect != pIndcBase)
				{
					if (m_pindcSelect)
						m_pindcSelect->ResetSelRgn();

					if (m_iToolSelectIndex >= 0 && m_iToolSelectIndex < m_arToolQue.GetSize())
					{
						pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
						pToolBase->HideHolder();
						m_iToolSelectIndex = -1;
					}

					m_pindcSelect = pIndcBase;
					m_pObjMgr->ReDrawGraphObject(true);
				}
				m_bGrpSelect = true;
				break;
			}
			else if (m_pindcSelect)
			{
				m_pindcSelect->ResetSelRgn();
				m_pObjMgr->ReDrawGraphObject(true);
				m_pindcSelect = NULL;
				m_bGrpSelect = false;

				return;
			}

			switch (m_pObjMgr->GetMainKind())
			{
			case GK_THREE:
			case GK_PNF:
			case GK_CLOCK:
			case GK_PV:
			case GK_KAGI:
			case GK_RENKO:
			case GK_EQV:
			case GK_CDV:
				break;
			default:
				if ((penvinfo->display.dwDspOption & DO_LBTNDNSHOW) && (penvinfo->display.dwDspOption & DO_VIEWGRID))
				{
					ViewGrid(true, point);
					m_dwObjStatus |= GOS_msLbtnDN;
				}
				else
					MouseResizeArea(point);

				return;
			}
			break;
		}
		break;
	case enumPointWhat::pointTICK:
		if (m_iToolSelectIndex >= 0 && m_arToolQue.GetSize() > 0)
		{
			pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			pToolBase->HideHolder();
			m_iToolSelectIndex = -1;
		}

		if (m_pindcSelect)
		{
			m_pindcSelect->ResetSelRgn();
			m_pObjMgr->ReDrawGraphObject(true);
			m_pindcSelect = NULL;
			m_bGrpSelect = false;
		}

		m_iToolSelectIndex = -1;

		RemoveAndrewPichfork();

		if (m_dwObjStatus & GOS_toolState)
			break;

		switch (m_pObjMgr->GetMainKind())
		{
		case GK_THREE:
		case GK_PNF:
		case GK_CLOCK:
		case GK_PV:
		case GK_KAGI:
		case GK_RENKO:
			return;
		}

		iRgnKey = IsMarkPos(point);
		if (iRgnKey)
		{
			RemoveMarkRegion(iRgnKey);
			return;
		}

		m_iDragRgnKey = SearchPointInDragRegion(point, m_rectDrag);
		if (m_iDragRgnKey == -1)
			break;

		{
			m_dwObjStatus |= GOS_dragRegion;
			CRect	rc = m_pObjMgr->m_rectObj;
			SetStateCursor(&rc);
			m_pObjMgr->ReDrawGraphObject(true);
		}
		break;
	default:
		if (m_iToolSelectIndex >= 0 && m_arToolQue.GetSize() > 0)
		{
			pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			pToolBase->HideHolder();
			m_iToolSelectIndex = -1;
		}

		RemoveAndrewPichfork();

		m_pObjMgr->m_pwndPnChart->ClientToScreen(&point);
		m_iCaptureSelectTool = m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_CHECK_RECT, 0), (long)&point);
		if (m_iCaptureSelectTool != toolNO)
		{
			m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_RELEASE, 0), 0);
			m_bMouseRelease = true;
		}
		else
		{
			if (ResetStateCursor())
			{
				m_dwObjStatus &= ~GOS_toolState;	// 2008.10.06
				m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_RELEASE, 0), 0);
			}
			return;
		}
		break;
	}

	m_dwObjStatus |= GOS_msLbtnDN;
}

void CObjEvent::LButtonUpEvent(CPoint point)
{
	_envInfo* penvinfo = m_pObjMgr->GetEnvInfo();
	if ((penvinfo->display.dwDspOption & DO_LBTNDNSHOW) && (penvinfo->display.dwDspOption & DO_VIEWGRID))
		ViewGrid(false);

	if (m_dwObjStatus & GOS_autoScroll)
		return;

	// 2006.11.20 lsh (start)
	CIndcBase* pindcMain	= m_pObjMgr->GetMainGraph();
	//m_strClickDateTime	= pindcMain->GetDisplayPosHeader(point, false);
	m_strClickDateTime	= pindcMain->GetXposValue(point); 

	if (m_pObjMgr->IsOrderMode())
	{
		m_strClickPrice = "";
		 pindcMain->GetOrderPoint(point, m_strClickPrice);
		m_strClickPrice.Replace(",", "");
		m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_MAP_CLICK_EVENT, 0), (long)&point);
		return;
	}
	// 2006.11.20 lsh (end)
	
	if (m_bMouseRelease)
	{
		m_iTkChoiced = 0;
		SetStateCursor();
		m_dwObjStatus &= ~(GOS_toolCreate | GOS_msLbtnDN);
		m_bMouseRelease = false;

		if (m_iCaptureSelectTool != toolNO)
		{
			switch (m_iCaptureSelectTool)
			{
			case toolINPUT:
			case toolTEND:
			case toolHLINE:	
			case toolVLINE:
			case toolCROSS:
			case toolARC:	
			case toolTRI:
			case toolRECT:
			case toolUARROW:
			case toolGIGAN:
			case toolDAEDEUNG:
			case toolFIBOLINE:
			case toolGANNFAN:
			case toolTRILINE:
			case toolDARROW:
			case toolLARROW:
			case toolRARROW:
			case toolFIBOFAN:
			case toolFIBOFAN2:
			case toolFIBOARC:
			case toolFIBORET:	
			case toolRGANNFAN:
			case toolGANNLINE:
			case toolQUARDLINE:	
			case toolACCELFAN:
			case toolACCELARC:
			case toolGIGAN2:
			case toolAndrewPf:
			case toolPENCIL:
				break;
			default:
				return;
			}
			m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_GET_SELECT_TOOL, 0), m_iCaptureSelectTool);
			m_iCaptureSelectTool = toolNO;
		}

		return;
	}

	if ((m_dwObjStatus & GOS_msLbtnDN) && (m_dwObjStatus & GOS_dragRegion))
	{

		m_dwObjStatus &= ~(GOS_msLbtnDN | GOS_dragRegion);
		m_dwObjOpr &= ~GOM_overRegion;

		SetStateCursor();

		int iRgnKey = SearchPointInDragRegion(point, m_rectDrag, true);
		if (m_iDragRgnKey != iRgnKey)
			SwapRegion(m_iDragRgnKey, iRgnKey);
		m_iDragRgnKey = -1;
		m_pObjMgr->ReDrawGraphObject(true);

		return;
	}

	if (m_dwObjStatus & GOS_dragSplit)
	{
		m_dwObjStatus &= ~GOS_dragSplit;
		SetStateCursor();

		m_rectSplitPre = m_rectSplitCur;
		m_rectSplitCur.SetRectEmpty();
		DrawSplit();
		ReAssignRegion(point);
		m_dwObjStatus &= ~GOS_msLbtnDN;

		return;
	}

	if (m_pointStartSize.x >= 0)
	{
		MouseResizeArea(point, true);
		m_pointStartSize = CPoint(-1, -1);
		m_pObjMgr->ReDrawGraphObject(true);
		m_dwObjStatus &= ~GOS_msLbtnDN;
		
		return;
	}

	if (m_bGrpSelect)
	{
		m_bGrpSelect = false;
		m_dwObjStatus &= ~(GOS_msLbtnDN | GOS_dragObject);
		SetStateCursor();

		if (m_pObjMgr->m_rectGrp.PtInRect(point))
			MoveGraph(point);
		else
		{
			CRect rect(m_pObjMgr->m_rectGrp.left, m_pObjMgr->m_rectGrp.bottom, m_pObjMgr->m_rectGrp.right, m_pObjMgr->m_rectObj.bottom);
			if (rect.PtInRect(point))
				MoveGraphToNewRegion(point);
		}			

		bool bLabel;
		CIndcBase* pindcOverLine = IsPtOverLine(point, false, bLabel);
		if (pindcOverLine)
		{
			m_dwObjOpr |= GOM_overObject;
			SetOverCursor();
		}
		return;
	}

	enumPointWhat pointOWNER = SearchPtInRegion(point);
	CToolBase* pToolBase = (CToolBase *)0;

	int idx;
	switch (pointOWNER)
	{
	case enumPointWhat::pointGRAPH:
		switch (m_dwObjStatus & GOS_toolState)
		{
		case GOS_toolCreate:
			if (m_iTkChoiced == static_cast<int>(enumTK::tkUSRINPUT))
			{
				CIndcBase* pIndcBase = SearchPtInGraph(m_pointDown);

				m_iTkChoiced = 0;
				SetStateCursor();

				pToolBase = AddToolQue(static_cast<int>(enumTK::tkUSRINPUT), pIndcBase);
				if (pToolBase == (CToolBase *) 0)
				{
					m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_RELEASE, 0), 0);
					m_dwObjStatus &= ~(GOS_toolCreate | GOS_msLbtnDN);
					m_pObjMgr->ReDrawGraphObject(true);
					return;
				}
				pToolBase->CreateToolLBUP(m_pointDown, m_bMouseCaptureTCreate);
				m_iToolSelectIndex = m_arToolQue.GetUpperBound();

				m_iTkChoiced = (int)enumTK::tkUSRINPUT;
				SetStateCursor();
				m_pObjMgr->ReDrawGraphObject(true);
				break;
			}	

			idx = m_arToolQue.GetUpperBound();
			if (idx < 0)
			{
				m_pObjMgr->ReDrawGraphObject(true);
				break;
			}
			pToolBase = m_arToolQue.GetAt(idx);
			pToolBase->CreateToolLBUP(point, m_bMouseCaptureTCreate);
			m_iToolSelectIndex = m_arToolQue.GetUpperBound();

			m_pObjMgr->ReDrawGraphObject(true);
			break;
		case GOS_toolChange:
			if (m_iToolSelectIndex < 0)
			{
				m_pObjMgr->ReDrawGraphObject(true);
				break;
			}
			pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			pToolBase->ChangeToolLBUP(point, m_bMouseCaptureTCreate);

			m_dwObjStatus &= ~GOS_toolChange;
			m_pObjMgr->ReDrawGraphObject(true);
			break;
		case GOS_toolMove:
			if (m_iToolSelectIndex < 0)
				break;
			if (m_arToolQue.GetSize() > 0)
				pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			if (pToolBase)
				pToolBase->MoveToolLBUP(point, m_bMouseCaptureTCreate);

			m_dwObjStatus &= ~GOS_toolMove;
			m_pObjMgr->ReDrawGraphObject(true);
			break;
		default:
			m_pObjMgr->ReDrawGraphObject(true);
			break;
		}
		break;
	case enumPointWhat::pointTICK:
		m_pObjMgr->ReDrawGraphObject(true);
		break;		
	case enumPointWhat::pointNONE:
		break;
	}

	m_dwObjStatus &= ~GOS_msLbtnDN;
}

void CObjEvent::MouseMoveEvent(CPoint point, int iValid)
{
	bool bLabel;
	CIndcBase* pIndcBase = IsPtOverLine(point, false, bLabel);

	if (bLabel)
		m_pTipIndc = NULL;
	else
		m_pTipIndc = pIndcBase;

	CString	str = GetTipStr(point);
	if (m_strPosData != str && !(m_dwObjStatus & GOS_toolState))
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_strPosData.Format(_T("%s"), str.GetString());
		
		m_pParent->KillTimer(TIMER_PID);
		m_pParent->SetTimer(TIMER_PID, TIMER_PGAP, NULL);
	}

	MouseMove(false, point, true);
	if (m_pObjMgr->m_arGraphQue.GetSize() <= 0)
		return;

	if (m_dwObjStatus & GOS_autoScroll)
		return;
	if (m_iTkChoiced == (int)enumTK::tkUSRINPUT && (m_dwObjStatus & GOS_toolCreate))
		return;
	if ((m_dwObjStatus & GOS_msLbtnDN) && (m_dwObjStatus & (GOS_dragRegion|GOS_dragObject)))	
		return;

	if (m_pointStartSize.x >= 0)
	{
		MouseResizeArea(point);
		return;
	}

	if (m_dwObjStatus & GOS_dragSplit)
	{
		m_rectSplitPre = m_rectSplitCur;
		m_rectSplitCur.SetRect(m_pObjMgr->m_rectObj.left, point.y, m_pObjMgr->m_rectObj.right, point.y+3);
		DrawSplit();
		return;
	}

	if (m_bGrpSelect)	// move graph 
	{
		if (m_dwObjStatus & GOS_msLbtnDN)
		{
			CRect	rc = m_pObjMgr->m_rectObj;
			m_dwObjStatus |= GOS_dragObject;
			SetStateCursor(&rc);
		}
		return;
	}

	_envInfo* penvinfo = m_pObjMgr->GetEnvInfo();
	if (!(m_dwObjStatus & GOS_msLbtnDN))
	{
		if (!(m_dwObjStatus & GOS_toolState))
		{
			if (!iValid)
				point = CPoint(-1,-1);

			CRect rc;
			IsSplitPos(point, rc);
			if (!rc.IsRectEmpty())
			{
				if (m_dwObjOpr & GOM_overSplit)
				{
					if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
						ViewGrid(true, point);
					return;
				}

				m_dwObjOpr &= ~GOM_overState;
				m_dwObjOpr |= GOM_overSplit;
				SetOverCursor();
				if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
					ViewGrid(true, point);
				return;
			}
			else if (m_dwObjOpr & GOM_overSplit)
			{
				m_dwObjOpr &= ~GOM_overSplit;
				SetOverCursor();
			}
			
			if (pIndcBase)
			{
				m_pTipIndc = pIndcBase;
				if (m_dwObjOpr & GOM_overObject)
				{
					if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
						ViewGrid(true, point);
					return;
				}

				m_dwObjOpr &= ~GOM_overState;
				m_dwObjOpr |= GOM_overObject;
				SetOverCursor();

				if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
					ViewGrid(true, point);

				return;
			}
			else if (m_dwObjOpr & GOM_overObject)
			{
				m_dwObjOpr &= ~GOM_overObject;
				SetOverCursor();
			}

			int	tState = IsPtOverTool(point);
			if (tState == GOM_overToolB)
			{
				if (m_dwObjOpr & GOM_overToolB)
				{
					if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
						ViewGrid(true, point);
					return;
				}

				m_dwObjOpr &= ~GOM_overState;
				m_dwObjOpr |= GOM_overToolB;
				SetOverCursor();
				if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
					ViewGrid(true, point);
				return;
			}
			else if (tState == GOM_overToolH)
			{
				if (m_dwObjOpr & GOM_overToolH)
				{
					if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
						ViewGrid(true, point);
					return;
				}

				m_dwObjOpr &= ~GOM_overState;
				m_dwObjOpr |= GOM_overToolH;
				SetOverCursor();
				if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
					ViewGrid(true, point);
				return;			
			}
			else if (m_dwObjOpr & (GOM_overToolB | GOM_overToolH))
			{
				m_dwObjOpr &= ~(GOM_overToolB |  GOM_overToolH);
				SetOverCursor();
			}

			int rgn = IsMarkPos(point);
			if (rgn)
			{
				if (m_dwObjOpr & GOM_overDelete)
				{
					if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
						ViewGrid(true, point);
					return;
				}

				m_dwObjOpr |= GOM_overDelete;
				SetOverCursor();

				if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
					ViewGrid(true, point);
				return;
			}
			else if (m_dwObjOpr & GOM_overDelete)
			{
				m_dwObjOpr &= ~GOM_overDelete;
				SetOverCursor();
			}
			
			if (!(penvinfo->display.dwDspOption & DO_LBTNDNSHOW))
				ViewGrid(true, point);
			return;
		}

		return;
	}
	else
	{
		if (penvinfo->display.dwDspOption & DO_LBTNDNSHOW)
			ViewGrid(true, point);
	}

	enumPointWhat pointOWNER = SearchPtInRegion(point);
	CToolBase* pToolBase = (CToolBase *)0;
	int	idx;
	switch (pointOWNER)
	{
	case enumPointWhat::pointGRAPH:
		switch (m_dwObjStatus & GOS_toolState)
		{
		case GOS_toolCreate:
			idx = m_arToolQue.GetUpperBound();
			if (idx < 0)
				break;
			pToolBase = m_arToolQue.GetAt(idx);
			if (pToolBase == (CToolBase *) 0)
				break;

			if (pToolBase->m_eToolKind == enumTK::tkCROSS)
				GetCrossValue(pToolBase, point);

			pToolBase->CreateToolLBMV(point, m_bMouseCaptureTCreate);
			break;
		case GOS_toolChange:
			if (m_iToolSelectIndex < 0)
				break;

			pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			pToolBase->ChangeToolLBMV(point, m_bMouseCaptureTCreate);
			break;
		case GOS_toolMove:
			if (m_iToolSelectIndex < 0)
				break;

			if (m_arToolQue.GetSize() > 0)
				pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			if (pToolBase)
			{
				if (pToolBase->m_eToolKind == enumTK::tkCROSS)
					GetCrossValue(pToolBase, point);
				pToolBase->MoveToolLBMV(point, m_bMouseCaptureTCreate);
			}
			break;
		}
		break;
	case enumPointWhat::pointTICK:
		break;
	default:
		break;
	}
}

void CObjEvent::RButtonDownEvent(CPoint pt)
{
	ViewGrid(false);

	if (m_dwObjStatus & GOS_autoScroll)
		return;

	enumPointWhat pointOWNER = SearchPtInRegion(pt);
	switch (pointOWNER)
	{
	case enumPointWhat::pointGRAPH:
		if (ResetStateCursor())
		{
			m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_RELEASE, 0), 0);
			return;
		}
		[[fallthrough]];
	default:
		if (ResetStateCursor())
		{
			m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_RELEASE, 0), 0);
			return;
		}
	}

	m_dwObjStatus |= GOS_msRbtnDN;
}

void CObjEvent::RButtonUpEvent(CPoint point)
{
	if (m_dwObjStatus & GOS_autoScroll)
	{
		m_dwObjStatus &= ~GOS_msRbtnDN;
		return;
	}

	if (m_dwObjStatus & GOS_toolCreate)
	{
		m_pParent->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_RELEASE, 0), 0);

		m_dwObjStatus &= ~(GOS_msRbtnDN | GOS_toolCreate);
		m_iTkChoiced = 0;
		SetStateCursor();
		RemoveAndrewPichfork();

		return;
	}

	ViewGrid(false);

	int iMenuID;
	int iAKind;
	CToolBase* ptoolSelect;
	CToolBase* ptoolCopy;
	CIndcBase* pindcPoint;
	enumPointWhat epwOwner = SearchPtInRegion(point);
	switch (epwOwner)
	{
	case enumPointWhat::pointGRAPH:
		pindcPoint = SearchPtInGraph(point);

		if (!pindcPoint)
		{
			m_dwObjStatus &= ~GOS_msRbtnDN;
			return;
		}

		if (m_iToolSelectIndex >= 0 && m_arToolQue.GetSize() > 0 
			&& (ptoolSelect = m_arToolQue.GetAt(m_iToolSelectIndex)) != NULL
			&& ptoolSelect->IsVisibleHolder())
		{
			if (ptoolSelect->IsVisibleHolder() && ptoolSelect->GetToolName() != (int)enumTK::tkHORIZON)
			{
				iMenuID = DisplayModifyToolMenu();
				switch (iMenuID)
				{
				case 0:	m_dwObjStatus &= ~GOS_msRbtnDN;
					return;
				case modifyMENUID:		// 복사
					ptoolCopy = CToolMgr::CopyTool(ptoolSelect);
					m_arToolQue.Add(ptoolCopy);
					ptoolCopy->ChangeMemberValue();
					ptoolSelect->HideHolder();
					ptoolCopy->DrawHolder(true);
					m_iToolSelectIndex = m_arToolQue.GetUpperBound();
					m_pObjMgr->ReDrawGraphObject(true);
					break;
				case modifyMENUID + 1:		// 삭제
					delete ptoolSelect;
					ptoolSelect = NULL;

					m_arToolQue.RemoveAt(m_iToolSelectIndex);
					m_iToolSelectIndex = -1;
					m_pObjMgr->ReDrawGraphObject(true);
					break;
				case modifyMENUID + 2:		// 전체 삭제
					RemoveToolQue();
					m_iToolSelectIndex = -1;
					m_pObjMgr->ReDrawGraphObject(true);
					break;
				}
			}
			break;
		}

		// 2006.10.13 schbang
		if (!m_bMapChart)
			if (ProcessSmallMenu(point))
				return;

		// 2006.05.17 schbang : 팝업메뉴 떠있을시 툴팁 제거
		m_pParent->KillTimer(TIMER_PID);
		m_pToolTip->ShowWindow(SW_HIDE);
		
		iMenuID = DisplayChartMenu(iAKind, point);
		ProcessChartMenu(iMenuID, iAKind);
		break;
	// 2006.10.26 schbang
	case enumPointWhat::pointTICK:
		pindcPoint = SearchPtInTick(point);
		m_pObjMgr->YAxisConfig(pindcPoint);
		break;
	default:
		break;
	}

	m_dwObjStatus &= ~GOS_msRbtnDN;
}

void CObjEvent::DoubleClickEvent(CPoint point)
{
	CToolBase* pToolBase = NULL;
	CIndcBase* pIndcBase;
	CPoint pointAdjust;

//	int iMainKind = m_pObjMgr->GetMainKind();
	enumPointWhat epwOwner = SearchPtInRegion(point);
	switch (epwOwner)
	{
	case enumPointWhat::pointGRAPH:
		if (m_dwObjStatus & GOS_autoScroll)
			return;
		if (m_dwObjStatus & GOS_toolCreate)
			return;

		if (!SearchPtInTool(point))
		{
			m_dwObjStatus &= ~GOS_toolState;
		}
		else
		{
			m_dwObjStatus &= ~GOS_toolState;
			pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			if (!pToolBase)
				return;

			m_pObjMgr->ToolConfig(pToolBase);
			return;
		}

		pIndcBase = SearchPtInGraph(point);
		if (pIndcBase)
		{
			if (!m_pindcSelect)
				return;

			m_pObjMgr->ChartConfig(m_pindcSelect);
			return;
		}
		break;
	case enumPointWhat::pointTICK:
		pIndcBase = SearchPtInTick(point);
		m_pObjMgr->YAxisConfig(pIndcBase);
		break;
	}
}

void CObjEvent::MouseWheelEvent(bool bLeft)
{
//	if (!m_pindcSelect)
//		return;

	m_pObjMgr->m_pwndPnChart->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, bLeft ? ctrlWheelLeft : ctrlWheelRight), NULL);
}

void CObjEvent::KeyboardEvent(int key)
{
	if (key != VK_DELETE)
		return;

	if (m_pindcSelect)
	{
		switch (m_pObjMgr->GetMainKind())
		{
		case GK_EQV:
		case GK_CDV:
		case GK_PV:
		case GK_CLOCK:
		case GK_THREE:
		case GK_PNF:
		case GK_SWING:
		case GK_KAGI:
		case GK_RENKO:
			return;
		default:
			switch (m_pindcSelect->GetGraphKind())
			{
			case GK_JPN:
			case GK_BAR:
			case GK_LIN:
			case GK_POLE:	
			case GK_AVOL:
			case GK_BALANCE:
				return;
			default:
				break;
			}
			m_pObjMgr->DeleteGraph(m_pindcSelect);
			return;
		}
	}

	CToolBase	*pToolBase;
	if (m_iToolSelectIndex >= 0 && m_iToolSelectIndex < m_arToolQue.GetSize()
		&& (pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex)) != NULL
		&& pToolBase->IsVisibleHolder())
	{
		delete pToolBase;
		pToolBase = NULL;

		m_arToolQue.RemoveAt(m_iToolSelectIndex);
		m_iToolSelectIndex = -1;
		m_pObjMgr->ReDrawGraphObject(true);
	}
}

void CObjEvent::FocusEvent(bool bfocus)
{
	if (!bfocus)
	{
		ViewGrid(false);

		if (m_pindcSelect)
		{
			m_pindcSelect->ResetSelRgn();
		}
		m_pindcSelect = NULL;
		m_bGrpSelect = false;

		if (m_iToolSelectIndex >= 0 && m_iToolSelectIndex < m_arToolQue.GetSize())
		{
			CToolBase *pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			pToolBase->HideHolder();
			m_iToolSelectIndex = -1;
		}
		m_pObjMgr->ReDrawGraphObject(true);
	}
}

bool CObjEvent::ToolSelect(int select)
{
	if (select == toolVIEW)
	{
//		m_pParent->SendMessage(GEM_VIEW,
//			MAKEWPARAM(GEV_INPUT, GVI_ToolView), 0);
		return true;
	}
	
	if (m_pObjMgr->GetMainGraph() == (CIndcBase *) 0)
		return false;

	if (select != toolAndrewPf)
		RemoveAndrewPichfork();

	switch (select)
	{
	case toolMOUSE:
		m_iToolSelectIndex = -1;
		return true;
	case toolSETUP:
		break;
	case toolNUM:
		break;
	case toolDELETE:
		{
			RemoveToolQue();
			m_iToolSelectIndex = -1;
			m_pObjMgr->ReDrawGraphObject(true);
			return true;
		}
		break;
	case toolHISTORY:
		{
			RemoveLastToolQue();
			m_pObjMgr->ReDrawGraphObject(true);
			return true;
		}
		break;
	default:
		if (m_iToolSelectIndex >= 0 && m_iToolSelectIndex < m_arToolQue.GetSize())
		{
			CToolBase *pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			pToolBase->HideHolder();
			m_pObjMgr->ReDrawGraphObject(true);
			m_iToolSelectIndex = -1;
		}

		break;
	}	

	m_iTkChoiced = (int)enumTK::tkNONE;
	switch (select)
	{
	case toolTEND:
		m_iTkChoiced = (int)enumTK::tkTENDENCY;
		break;
	case toolTENDEX:
		m_iTkChoiced = (int)enumTK::tkTENDENCYEX;
		break;
	case toolINPUT:
		m_iTkChoiced = (int)enumTK::tkUSRINPUT;
		break;
	case toolFIBOLINE:
		m_iTkChoiced = (int)enumTK::tkFIBOLINE;
		break;
	case toolFIBOFAN:
		m_iTkChoiced = (int)enumTK::tkFIBOFAN;
		break;
	case toolFIBOFAN2:
		m_iTkChoiced = (int)enumTK::tkFIBOFAN2;
		break;
	case toolFIBOARC:
		m_iTkChoiced = (int)enumTK::tkFIBOARC;
		break;
	case toolFIBORET:
		m_iTkChoiced = (int)enumTK::tkFIBORET;
		break;
	case toolGANNLINE:
		m_iTkChoiced = (int)enumTK::tkGANNLINE;
		break;
	case toolGANNFAN:
		m_iTkChoiced = (int)enumTK::tkGANNFAN;
		break;
	case toolRGANNFAN:
		m_iTkChoiced = (int)enumTK::tkRGANNFAN;
		break;
	case toolHLINE:
		m_iTkChoiced = (int)enumTK::tkHLINE;
		break;
	case toolVLINE:
		m_iTkChoiced = (int)enumTK::tkVLINE;
		break;
	case toolACCELFAN:
		m_iTkChoiced = (int)enumTK::tkACCELFAN;
		break;
	case toolACCELARC:
		m_iTkChoiced = (int)enumTK::tkACCELARC;
		break;
	case toolCROSS:
		m_iTkChoiced = (int)enumTK::tkCROSS;
		break;
	case toolRECT:
		m_iTkChoiced = (int)enumTK::tkRECT;
		break;
	case toolTRI:
		m_iTkChoiced = (int)enumTK::tkTRI;
		break;
	case toolARC:
		m_iTkChoiced = (int)enumTK::tkARC;
		break;
	case toolUARROW:
		m_iTkChoiced = (int)enumTK::tkUARROW;
		break;
	case toolDARROW:
		m_iTkChoiced = (int)enumTK::tkDARROW;
		break;
	case toolLARROW:
		m_iTkChoiced = (int)enumTK::tkLARROW;
		break;
	case toolRARROW:
		m_iTkChoiced = (int)enumTK::tkRARROW;
		break;
	case toolGIGAN:
		m_iTkChoiced = (int)enumTK::tkGIGAN;
		break;
	case toolGIGAN2:
		m_iTkChoiced = (int)enumTK::tkGIGAN2;
		break;
	case toolQUARDLINE:
		m_iTkChoiced = (int)enumTK::tkQUARDLINE;
		break;
	case toolTRILINE:
		m_iTkChoiced = (int)enumTK::tkTRILINE;
		break;
	case toolTRILINELEVEL:
		m_iTkChoiced = (int)enumTK::tkTRILINELEVEL;
		break;
	case toolDAEDEUNG:
		m_iTkChoiced = (int)enumTK::tkDAEDEUNG;
		break;
	case toolDAEDEUNGCFG:
		{
			CString	lgap, rgap;
			lgap.Format("%d", m_iLeftGap);
			rgap.Format("%d", m_iRightGap);
			CDaedeungDlg	dlg(lgap, rgap);
			if (dlg.DoModal() == IDOK)
			{
				m_iLeftGap = atoi(dlg.GetLeftGap());
				m_iRightGap = atoi(dlg.GetRightGap());
				return TRUE;
			}
			return FALSE;
		}
		break;
	case toolAndrewPf:
		m_iTkChoiced = (int)enumTK::tkANDREWPICHFORK;
		break;
	case toolPENCIL:
		m_iTkChoiced = (int)enumTK::tkPENCIL;
		break;
	default:
		m_dwObjStatus &= ~GOS_toolState;
		break;
	}

	m_dwObjStatus |= GOS_toolCreate;
	SetStateCursor();

	return true;
}

void CObjEvent::ToolBackGround(int rgb)
{
	for (int ii = m_arToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		CToolBase	*pToolBase = m_arToolQue.GetAt(ii);
		pToolBase->ChangeBackGround(rgb);
	}

	if (m_pObjMgr)
		m_pObjMgr->ReDrawGraphObject(true);
}

void CObjEvent::ToolColor(int rgb)
{
	struct _envInfo	*envinfo = m_pObjMgr->GetEnvInfo();
	//envinfo->display.toolinfo.lcolor = rgb;
	envinfo->display.toolinfo.rgbLine = rgb;
/*	// 신규 생성에 적용
	for (int ii = m_arToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		CToolBase	*pToolBase = m_arToolQue.GetAt(ii);
		//pToolBase->SetLineStyle(&envinfo->display.toolinfo, true, false, false);
		pToolBase->SetToolColor(rgb);
	}

	if (m_pObjMgr)
		m_pObjMgr->ReDrawGraphObject(true);
*/
}

void CObjEvent::ToolWidth(BYTE btWidth)
{
	struct _envInfo	*envinfo = m_pObjMgr->GetEnvInfo();
	//envinfo->display.toolinfo.lwidth = btWidth;
	envinfo->display.toolinfo.btLineWidth = btWidth;
/*	// 신규 생성에 적용
	for (int ii = m_arToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		CToolBase	*pToolBase = m_arToolQue.GetAt(ii);
		//pToolBase->SetLineStyle(&envinfo->display.toolinfo, false, false, true);
		pToolBase->SetToolWidth(btWidth);
	}

	if (m_pObjMgr)
		m_pObjMgr->ReDrawGraphObject(true);
*/
}

void CObjEvent::ViewGrid(bool bShow, CPoint point)
{
	bool bIsGridMode = m_pObjMgr->IsGridMode();
	if (!bIsGridMode)
	{
		if (point.x == -1)
			m_pObjMgr->HideCrossLine();
		else if (m_pObjMgr->m_rectGrp.PtInRect(point))
			SetDisplayPosData(point, true);
		else
			m_pObjMgr->HideCrossLine();
		
		return;
	}

	//if (!bShow)
	if (!bShow || m_dwObjStatus & GOS_toolState)
	{
		//if (m_pObjMgr->m_pGrpGrid && m_pObjMgr->m_pGrpGrid->IsWindowVisible())
		if (m_pObjMgr->m_pGrpGrid && m_pObjMgr->m_pGrpGrid->m_hWnd && m_pObjMgr->m_pGrpGrid->IsWindowVisible())
			m_pObjMgr->m_pGrpGrid->ShowWindow(SW_HIDE);

		m_pObjMgr->HideCrossLine();
		return;
	}

	if (bShow)
		SelectTable(point);

	m_pObjMgr->CreateGrid(true);
	if (!m_pObjMgr->m_rectGrp.PtInRect(point))
	{
		if (m_pObjMgr->m_pGrpGrid->IsWindowVisible())
		{
			m_pObjMgr->m_pGrpGrid->ShowWindow(SW_HIDE);
		}
		m_pObjMgr->HideCrossLine();

		return;
	}

	SetDisplayPosData(point);
}

void CObjEvent::SelectTable(CPoint pt)
{
	if (!m_pObjMgr->GetTableMode())
		return;

	int mkind = m_pObjMgr->GetMainKind();
	switch (mkind)
	{
	case GK_PV:
	case GK_CLOCK:
	case GK_THREE:	
	case GK_PNF:
	case GK_SWING:
	case GK_KAGI:
	case GK_RENKO:
		break;
	default:
		{
			CIndcBase *pMainIndc = m_pObjMgr->GetMainGraph();
			if (!pMainIndc)
				break;

			int dataPos = pMainIndc->GetDisplayPosIndex(pt);
			dataPos--;
			m_pObjMgr->m_pwndPnChart->SendMessage(GEV_TABLE, tbSelect, dataPos);
		}
		break;
	}
}

CString CObjEvent::GetTipStr(CPoint pt)
{
	CString	strTip = _T("");

	if (!m_bShowTip)
		return strTip;

	CString str1, str2;
	if (m_bMapChart)	// if control
	{
		int iRgnKey;
		int iSelRgn = 0;
		struct	_xRegion* pxRegion;
		for (POSITION pos = m_pObjMgr->m_mapRegion.GetStartPosition(); pos; )
		{
			m_pObjMgr->m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
			if (pxRegion->xRect.PtInRect(pt))
			{
				iSelRgn = iRgnKey;
				break;
			}
		}
		if (iSelRgn == 0)
			return strTip;

		CIndcBase *pMainIndc = m_pObjMgr->GetMainGraph();
		str1 = pMainIndc->GetDisplayPosHeader(pt, false);
		str2 = _T("");
		for (int ii =0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
		{
			CIndcBase *pIndcBase = m_pObjMgr->m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphRegionID() == iSelRgn)
			{
				CString	str3 = pIndcBase->GetDisplayPosData(pt, false);
				if (str2.IsEmpty())
					str2 = str3;
				else
					str2 += _T("\t") + str3;
			}
		}

		if (str2.IsEmpty())
			return strTip;

		strTip = str1 + str2;
	}
	else	// 종합차트
	{
		if (!m_pTipIndc)
			return strTip;

		CIndcBase *pMainIndc = m_pObjMgr->GetMainGraph();

		str1 = pMainIndc->GetDisplayPosHeader(pt, false);
		str2 = m_pTipIndc->GetDisplayPosData(pt, false);
		if (str2.IsEmpty())
			return strTip;

		strTip = str1 + str2;
	}

	CString strExt = (LPCTSTR)m_pParent->SendMessage(WM_USER + 999, (WPARAM)_T("EXTM_TOOLTIP"), (LPARAM)(LPCTSTR)str1);
	if (!strExt.IsEmpty())
	{
		strTip += "\t" + strExt;
	}
	
	return strTip;
}

CIndcBase* CObjEvent::SearchPtInGraph(CPoint point)
{
	bool bDrawTick = (m_pObjMgr->GetEnvInfo()->display.dwDspOption & (DO_DISPRT | DO_DISPLT))? true:false;

	CIndcBase* pIndcBase;
	if (bDrawTick)
	{
		for (int ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
		{
			pIndcBase = m_pObjMgr->m_arGraphQue.GetAt(ii);
			if (pIndcBase->IsPointInGrpRect(point) && pIndcBase->IsTickDraw(GO_TKLEFT | GO_TKRIGHT))
			{
				return pIndcBase;
			}
		}
	}
	else
	{
		for (int ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
		{
			pIndcBase = m_pObjMgr->m_arGraphQue.GetAt(ii);
			if (pIndcBase->IsPointInGrpRect(point))
			{
				return pIndcBase;
			}
		}
	}

	return (CIndcBase *) 0;
}

int CObjEvent::SearchPointInDragRegion(CPoint pt, CRect &DragRC, bool bMouseUP)
{
	if (!m_pObjMgr->m_rectObj.PtInRect(pt))	return -1;
	if (!bMouseUP && m_pObjMgr->m_rectGrp.PtInRect(pt))	return -1;

	switch (m_pObjMgr->GetMainKind())
	{
	case GK_THREE:
	case GK_PNF:
	case GK_SWING:
	case GK_CLOCK:
	case GK_PV:
	case GK_RENKO:
	case GK_KAGI:
		return -1;
	}

	CRect	rc;
	CIndcBase* pIndcBase;
	for (int ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
	{
		pIndcBase = (CIndcBase *) m_pObjMgr->m_arGraphQue.GetAt(ii);
		rc = pIndcBase->GetOrgDrawRect();
		if (rc.top <= pt.y && rc.bottom >= pt.y)
		{
			DragRC = rc;
			return pIndcBase->GetGraphRegionID();
		}
	}

	return -1;
}

enumPointWhat CObjEvent::SearchPtInRegion(CPoint point)
{
	if (m_bMouseCaptureTCreate)
		return enumPointWhat::pointGRAPH;

	if (m_pObjMgr->m_rectGrp.PtInRect(point))
	 	return enumPointWhat::pointGRAPH;
	else if (m_pObjMgr->m_rectObj.PtInRect(point))
		return enumPointWhat::pointTICK;

	return enumPointWhat::pointNONE;
}

bool CObjEvent::SearchPtInTool(CPoint pt)
{
	if (m_dwObjStatus & GOS_toolCreate)
		return true;

	int	size = m_arToolQue.GetSize();
	if (size < 1)
		return false;

	CToolBase* pToolBase;
	if (m_iToolSelectIndex >= 0 && m_iToolSelectIndex < size)
	{
		pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
		if (pToolBase->PtInHolderRect(pt))	// on holder
		{
			m_dwObjStatus |= GOS_toolChange;
			return true;
		}
		if (pToolBase->PtInToolRegion(pt))	// on line
		{
			m_dwObjStatus |= GOS_toolMove;
			return true;
		}
	}

	for (int ii = 0; ii < size; ii++)
	{
		pToolBase = m_arToolQue.GetAt(ii);
		if (pToolBase->PtInHolderRect(pt))	// on holder
		{
			m_iToolSelectIndex = ii;
			m_dwObjStatus |= GOS_toolChange;
			return true;
		}
		if (pToolBase->PtInToolRegion(pt))	// on line
		{
			m_iToolSelectIndex = ii;
			m_dwObjStatus |= GOS_toolMove;
			return true;
		}
	}

	return false;
}

CIndcBase* CObjEvent::IsPtOverLine(CPoint pt, bool bSet, bool &bLabel)
{
	int iCount = m_pObjMgr->m_arGraphQue.GetSize();
	CIndcBase *pIndcBase;
	for (int ii = 0; ii < iCount; ii++)
	{
		pIndcBase = (CIndcBase*) m_pObjMgr->m_arGraphQue.GetAt(ii);
		if (pIndcBase->IsPointInGrpRgn(pt, bLabel, bSet))
		{
			if (bSet)
				m_bGrpSelect = true;
			return pIndcBase;
		}
	}

	return (CIndcBase *)0;
}

int CObjEvent::IsPtOverTool(CPoint pt)
{
	int	size = m_arToolQue.GetSize();
	if (size < 1)
		return 0;

	CToolBase* pToolBase;
	if (m_iToolSelectIndex >= 0 && m_iToolSelectIndex < size)
	{
		pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
		if (pToolBase->PtInHolderRect(pt))	// on holder
			return GOM_overToolH;

		if (pToolBase->PtInToolRegion(pt))	// on line
			return GOM_overToolB;
	}

	for (int ii = 0; ii < size; ii++)
	{
		pToolBase = m_arToolQue.GetAt(ii);
		if (pToolBase->PtInHolderRect(pt))	// on holder
			return GOM_overToolH;

		if (pToolBase->PtInToolRegion(pt))	// on line
			return GOM_overToolB;
	}

	return 0;
}

class CToolBase* CObjEvent::AddToolQue(int toolKind, class CIndcBase *pIndcBase)
{
	CToolBase* pToolBase = (CToolBase *) 0;

	switch (static_cast<enumTK>(toolKind))
	{
	case enumTK::tkUSRINPUT:
		{
			CString	InputStr;

			CUserInputDlg	dlg(InputStr);
			if (dlg.DoModal() == IDCANCEL)
			{
				return (CToolBase *) 0;
			}
			InputStr = dlg.GetDisplayString();

			pToolBase = CToolMgr::CreateTool(m_pObjMgr->m_pwndView, m_pParent, m_pObjMgr->m_rectGrp, pIndcBase->GetOrgDrawRect(), 
				m_pObjMgr->GetEnvInfo()->display.rgbChartBG, pIndcBase, toolKind);
			pToolBase->SetDisplayString(InputStr);
			//pToolBase->SetLineStyle(&m_pObjMgr->GetEnvInfo()->display.toolinfo);
			pToolBase->SetToolInfo(&m_pObjMgr->GetEnvInfo()->display.toolinfo);
		}
		break;
	case enumTK::tkANDREWPICHFORK:
		{
			int idx;
			int pointcount = 0;
			
			idx = m_arToolQue.GetUpperBound();

			if (idx < 0)
			{	
				pToolBase = CToolMgr::CreateTool(m_pObjMgr->m_pwndView, m_pParent, m_pObjMgr->m_rectGrp, pIndcBase->GetOrgDrawRect(), 
					m_pObjMgr->GetEnvInfo()->display.rgbChartBG, pIndcBase, toolKind);
				pToolBase->SetToolInfo(&m_pObjMgr->GetEnvInfo()->display.toolinfo);
			}
			else
			{
				pToolBase = m_arToolQue.GetAt(idx);
				pointcount = pToolBase->m_PointCount;

				if (pToolBase->m_PointCount != 0 && pToolBase->m_eToolKind == enumTK::tkANDREWPICHFORK)
					return pToolBase;
				else
				{
					pToolBase = CToolMgr::CreateTool(m_pObjMgr->m_pwndView, m_pParent, m_pObjMgr->m_rectGrp, pIndcBase->GetOrgDrawRect(), 
						m_pObjMgr->GetEnvInfo()->display.rgbChartBG, pIndcBase, toolKind);
					pToolBase->SetToolInfo(&m_pObjMgr->GetEnvInfo()->display.toolinfo);
				}
			}
		}
		break;
	default:
		pToolBase = CToolMgr::CreateTool(m_pObjMgr->m_pwndView, m_pParent, m_pObjMgr->m_rectGrp, pIndcBase->GetOrgDrawRect(), 
			m_pObjMgr->GetEnvInfo()->display.rgbChartBG, pIndcBase, toolKind);
		{
			int	nToolKind = pToolBase->GetToolName();
			if ((int)enumTK::tkRECT <= nToolKind && nToolKind <= (int)enumTK::tkARC)
				pToolBase->SetToolInfo(&m_pObjMgr->GetEnvInfo()->display.toolinfo, true);
			else
				pToolBase->SetToolInfo(&m_pObjMgr->GetEnvInfo()->display.toolinfo, false);
		}

		m_bSelctEtc = true;
		break;		
	}

	if (pToolBase != (CToolBase *) 0)
		m_arToolQue.Add(pToolBase);

	if (toolKind == (int)enumTK::tkDAEDEUNG)
		pToolBase->InitValue(m_iLeftGap, m_iRightGap);

	return pToolBase;
}

void CObjEvent::RemoveToolQue()
{
	CToolBase* pToolBase = (CToolBase *)NULL;
	for (int ii = m_arToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		pToolBase = m_arToolQue.GetAt(ii);
		delete pToolBase;
		pToolBase = NULL;
	}

	m_arToolQue.RemoveAll();
}

void CObjEvent::RemoveLastToolQue()
{
	int idx = m_arToolQue.GetUpperBound();
	if (idx < 0)
		return;
	CToolBase* pToolBase = m_arToolQue.GetAt(idx);
	if (pToolBase == NULL)
		return;

	delete pToolBase;
	pToolBase = NULL;
	m_arToolQue.RemoveAt(idx);
	
	m_iToolSelectIndex = m_arToolQue.GetUpperBound();
}

// mouse state cursor
bool CObjEvent::SetStateCursor(CRect *pRect)
{
	HCURSOR hCursor = NULL;
	if (m_dwObjStatus & GOS_dragState)
	{
		switch (m_dwObjStatus & GOS_dragState)
		{
		case GOS_dragObject:
			hCursor = AfxGetApp()->LoadCursor(IDC_DRAGLINE);
			break;
		case GOS_dragSplit:
			hCursor = AfxGetApp()->LoadCursor(IDC_SPLIT);
			break;
		case GOS_dragRegion:
			hCursor = AfxGetApp()->LoadCursor(IDC_DRAGRGN);
			break;
		}

		if (pRect && !pRect->IsRectEmpty())	
		{
			m_pParent->ClientToScreen(pRect);
			::ClipCursor(pRect);
		}

		::SetCapture(m_pParent->m_hWnd);

		if (hCursor != NULL)
			::SetCursor(hCursor);

		return true;
	}

	switch (static_cast<enumTK>(m_iTkChoiced))
	{
	case enumTK::tkTENDENCY:
		hCursor = AfxGetApp()->LoadCursor(IDC_TENDLINE);
		break;
	case enumTK::tkTENDENCYEX:
		hCursor = AfxGetApp()->LoadCursor(IDC_TENDLINEEX);
		break;
	case enumTK::tkUSRINPUT:
		hCursor = AfxGetApp()->LoadCursor(IDC_TEXT);
		break;
	case enumTK::tkFIBOLINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBOLINE);
		break;
	case enumTK::tkFIBOFAN:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBOFAN);
		break;
	case enumTK::tkFIBOFAN2:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBOFAN2);
		break;
	case enumTK::tkFIBOARC:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBOARC);
		break;
	case enumTK::tkFIBORET:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBORET);
		break;
	case enumTK::tkGANNLINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_GANNLINE);
		break;
	case enumTK::tkGANNFAN:
		hCursor = AfxGetApp()->LoadCursor(IDC_GANNFAN);
		break;
	case enumTK::tkHLINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_HORIZON);
		break;
	case enumTK::tkVLINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_VLINE);
		break;
	case enumTK::tkRGANNFAN:
		hCursor = AfxGetApp()->LoadCursor(IDC_RGANNFAN);
		break;
	case enumTK::tkACCELFAN:
		hCursor = AfxGetApp()->LoadCursor(IDC_ACCELFAN);
		break;
	case enumTK::tkACCELARC:
		hCursor = AfxGetApp()->LoadCursor(IDC_ACCELARC);
		break;
	case enumTK::tkCROSS:
		hCursor = AfxGetApp()->LoadCursor(IDC_CROSSLINE);
		break;
	case enumTK::tkRECT:
		hCursor = AfxGetApp()->LoadCursor(IDC_RECT);
		break;
	case enumTK::tkTRI:
		hCursor = AfxGetApp()->LoadCursor(IDC_TRIANGLE);
		break;
	case enumTK::tkARC:
		hCursor = AfxGetApp()->LoadCursor(IDC_ARC);
		break;
	case enumTK::tkUARROW:
		hCursor = AfxGetApp()->LoadCursor(IDC_ARROWU);
		break;
	case enumTK::tkDARROW:
		hCursor = AfxGetApp()->LoadCursor(IDC_ARROWD);
		break;
	case enumTK::tkLARROW:
		hCursor = AfxGetApp()->LoadCursor(IDC_ARROWL);
		break;
	case enumTK::tkRARROW:
		hCursor = AfxGetApp()->LoadCursor(IDC_ARROWR);
		break;
	case enumTK::tkHORIZON:
		hCursor = AfxGetApp()->LoadCursor(IDC_HORIZON);
		break;
	case enumTK::tkGIGAN:
	case enumTK::tkGIGAN2:
		hCursor = AfxGetApp()->LoadCursor(IDC_GIGAN);
		break;
	case enumTK::tkQUARDLINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_QUARDLINE);
		break;
	case enumTK::tkTRILINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_TRILINE);
		break;
	case enumTK::tkTRILINELEVEL:
		hCursor = AfxGetApp()->LoadCursor(IDC_TRILEVEL);
		break;
	case enumTK::tkDAEDEUNG:
	case enumTK::tkDAEDEUNGCFG:
		hCursor = AfxGetApp()->LoadCursor(IDC_DAEDEUNG);
		break;
	case enumTK::tkANDREWPICHFORK:
		hCursor = AfxGetApp()->LoadCursor(IDC_ANDREWPF);
		break;
	case enumTK::tkPENCIL:
		hCursor = AfxGetApp()->LoadCursor(IDC_PENCIL);
		break;
	}

	if (m_dwObjStatus & GOS_capture)
	{
		::ClipCursor(NULL);
		::ReleaseCapture();
		m_dwObjStatus &= ~GOS_capture;
		if (hCursor == NULL)
		{
			hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			::SetCursor(hCursor);
			return true;
		}
	}

	if (hCursor != NULL)
	{
		::SetCapture(m_pParent->m_hWnd);
		m_dwObjStatus |= GOS_capture;
		::SetCursor(hCursor);
		return true;
	}

	::ClipCursor(NULL);
	::ReleaseCapture();
	m_dwObjStatus &= ~GOS_capture;
	hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	::SetCursor(hCursor);

	return true;
}

bool CObjEvent::SetOverCursor()
{
	HCURSOR	hCursor = NULL;

	::ClipCursor(NULL);
	::ReleaseCapture();
	m_dwObjStatus &= ~GOS_capture;

	switch (m_dwObjOpr & GOM_overState)
	{
	case GOM_overSplit:
		hCursor = AfxGetApp()->LoadCursor(IDC_SPLIT);
		break;
	case GOM_overToolH:
		hCursor = AfxGetApp()->LoadCursor(IDC_ROTATE);
		break;
	case GOM_overToolB:
	case GOM_overRegion:
	case GOM_overDelete:
	case GOM_overObject:
		hCursor = AfxGetApp()->LoadCursor(IDC_HANDX);
		break;
	}

	if (hCursor != NULL)
	{
		::SetCapture(m_pParent->m_hWnd);
		::SetCursor(hCursor);
		return true;
	}

	hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	::SetCursor(hCursor);

	return true;
}

bool CObjEvent::ResetStateCursor()
{
	if (m_iTkChoiced != (int)enumTK::tkNONE && (m_dwObjStatus & GOS_toolCreate))
	{
		if (m_dwObjStatus & GOS_capture)
		{
			::ClipCursor(NULL);
			::ReleaseCapture();
			m_dwObjStatus &= ~GOS_capture;
		}

		int idx = m_arToolQue.GetUpperBound();
		if (idx >= 0)
		{
			CToolBase* pToolBase = m_arToolQue.GetAt(idx);
			pToolBase->HideHolder();
		}

		m_iTkChoiced = (int)enumTK::tkNONE;
		return true;
	}

	return false;
}

void CObjEvent::GetCrossValue(CToolBase* pToolBase, CPoint pt)
{
	CString tmpS = _T("");
	CIndcBase* pIndcBase = (CIndcBase *) NULL;
	bool bFind = false;
	for (int ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
	{
		pIndcBase = (CIndcBase *) m_pObjMgr->m_arGraphQue.GetAt(ii);
		if (pIndcBase->IsPointInGrpRect(pt))
		{
			bFind = true;
			break;
		}
	}
	if (!bFind || !pIndcBase || !pToolBase)
		return;

	m_pObjMgr->GetCrossValue(pt, pIndcBase, false);
}

CPoint CObjEvent::SetDisplayPosData(CPoint point, bool bOnlyCross)
{
	if (m_pObjMgr->m_pCrossLine == NULL)
		return point;

	CIndcBase* pindcMain = m_pObjMgr->GetMainGraph();
	if (!pindcMain)
		return point;

	CString	sPosVal = "";
	CPoint pointAdjust(-1,-1);
	if (bOnlyCross)
	{
		if (m_pObjMgr->IsOrderMode())
		{
			if (!pindcMain)		return point;
			if (!pindcMain->IsPointInGrpRect(point))
				return point;

			pointAdjust = pindcMain->GetOrderPoint(point, sPosVal);
			if (pointAdjust.y > 0)
				pointAdjust.x = point.x;

			if (pointAdjust.x == -1)
				pointAdjust = point;

			if (!sPosVal.IsEmpty())
			{
				m_pObjMgr->ShowCrossLine(pointAdjust, sPosVal);
				m_pObjMgr->m_pwndPnChart->KillTimer(TIMER_GRID);
				m_pObjMgr->m_pwndPnChart->SetTimer(TIMER_GRID, 250, NULL);
			}

			return pointAdjust;
		}
		else if (m_pObjMgr->IsJTraceMode())
		{
			pointAdjust = pindcMain->GetAdjustPoint(point, sPosVal);
			if (pointAdjust.y > 0)
				pointAdjust.x = point.x;

			if (pointAdjust.x == -1)
				pointAdjust = point;
		}
		else
		{
			pointAdjust = point;
		}

		m_pObjMgr->ShowCrossLine(pointAdjust, sPosVal);
		m_pObjMgr->m_pwndPnChart->KillTimer(TIMER_GRID);
		m_pObjMgr->m_pwndPnChart->SetTimer(TIMER_GRID, 250, NULL);

		return pointAdjust;
	}

	CString strDisp;
	CString strUnit = pindcMain->GetDisplayPosHeader(point) + pindcMain->GetDisplayPosData(point);
	if (!strUnit.IsEmpty())
	{
		strDisp += strUnit;
		strDisp += "\t";
	}
	else
	{
		if (m_pObjMgr->GetMainKind() != GK_BALANCE)
		{
			m_pObjMgr->m_pwndPnChart->KillTimer(TIMER_GRID);
			if (!m_pObjMgr->m_rectGrp.IsRectEmpty())
			{
				m_pObjMgr->m_pGrpGrid->ShowData(_T(""), &point);
				m_pObjMgr->ShowCrossLine(point, sPosVal);
				m_pObjMgr->m_pwndPnChart->KillTimer(TIMER_GRID);
				m_pObjMgr->m_pwndPnChart->SetTimer(TIMER_GRID, 250, NULL);
			}

			return point;
		}
	}

	CString	strPosValue;
	if (m_pObjMgr->IsJTraceMode())
	{
		for (int ii = 0; ii <= m_pObjMgr->m_arGraphQue.GetUpperBound(); ii++)
		{
			CIndcBase* pindcOne = (CIndcBase *) m_pObjMgr->m_arGraphQue.GetAt(ii);
			if (pindcOne == pindcMain)
			{
				pointAdjust = pindcOne->GetAdjustPoint(point, sPosVal);
				if (pointAdjust.y > 0)
					pointAdjust.x = point.x;
			}
			else
			{
				strUnit = pindcOne->GetDisplayPosData(point);
				if (!strUnit.IsEmpty())
				{
					strDisp += strUnit;
					strDisp += "\t";
				}
			}

		}

		if (pointAdjust.x == -1)
			pointAdjust = point;
	}
	else
	{
		for (int ii = 0; ii <= m_pObjMgr->m_arGraphQue.GetUpperBound(); ii++)
		{
			CIndcBase* pindcOne = (CIndcBase *) m_pObjMgr->m_arGraphQue.GetAt(ii);
			if (pindcOne == pindcMain)
			{
				strPosValue = pindcOne->GetYposValue(point, true);
			}
			else
			{
				strUnit = pindcOne->GetDisplayPosData(point);
				if (!strUnit.IsEmpty())
				{
					strDisp += strUnit;
					strDisp += "\t";
				}
			}
		}
		pointAdjust = point;
	}

	m_pObjMgr->m_pwndPnChart->KillTimer(TIMER_GRID);
	if (!m_pObjMgr->m_rectGrp.IsRectEmpty())
	{
		CString	sData = strDisp + strPosValue;
		m_pObjMgr->m_pGrpGrid->ShowData(sData, &pointAdjust);
		m_pObjMgr->ShowCrossLine(pointAdjust, sPosVal);
		m_pObjMgr->m_pwndPnChart->KillTimer(TIMER_GRID);
		m_pObjMgr->m_pwndPnChart->SetTimer(TIMER_GRID, 250, NULL);
	}

	return pointAdjust;
}

void CObjEvent::SaveCurrentPos(int start, int end)
{
	if (start < 0)
	{
		CIndcBase *pIndcBase = m_pObjMgr->GetMainGraph();
		start = pIndcBase->GetDisplayPos();
		end  = pIndcBase->GetDisplayEnd();
	}
	PushDisplayStack(start, end);
}

bool CObjEvent::PopDisplayStack(int &start, int &end)
{
	if (m_arDspQue.GetSize() <= 0)
		return false;
	struct _xDisplay *display = m_arDspQue.GetAt(m_arDspQue.GetUpperBound());
	if (!display)
		return false;

	start = display->start;
	end = display->end;
	delete display;
	display = NULL;
	m_arDspQue.RemoveAt(m_arDspQue.GetUpperBound());

	return true;
}

void CObjEvent::PushDisplayStack(int start, int end)
{
	struct _xDisplay *display = (struct _xDisplay *)new char [sizeof(struct _xDisplay)];

	CIndcBase *pIndcBase = m_pObjMgr->GetMainGraph();
	if (pIndcBase->GetTotalDataCount() <= end)
		end = pIndcBase->GetTotalDataCount() - 1;

	if (start < 0)
		start = 0;

	display->start = start;
	display->end = end;

	int ret = m_arDspQue.Add(display);
}

void CObjEvent::RemoveDisplayStack()
{
	struct _xDisplay* xDisplay = NULL;
	for (int ii = m_arDspQue.GetUpperBound(); ii >= 0; ii--)
	{
		xDisplay = m_arDspQue.GetAt(ii);
		delete xDisplay;
		xDisplay = NULL;
	}
	
	m_arDspQue.RemoveAll();
}

void CObjEvent::MouseResizeArea(CPoint point, bool bEnd)
{
	if (point.x < 0)
		point.x = 0;

	point.x = GetPosition(-1, point);
	if (m_pointStartSize.x < 0) // first LBUTTONDOWN
	{
		::SetCapture(m_pParent->m_hWnd);
		m_pointStartSize = m_pointEndSize = point;
		DrawMouseArea(point, false);
		return;
	}

	if (!bEnd)
	{
		DrawMouseArea(point, true);	// delete pre area, draw current area
		return;
	}

	DrawMouseArea(point, false);
	::ReleaseCapture();

	int iStart = GetPosition(m_pointStartSize.x);
	int iEnd = GetPosition(m_pointEndSize.x);
	if (iStart > iEnd)
	{
		iStart = iEnd + iStart;
		iEnd = iStart - iEnd;
		iStart = iStart - iEnd;
	}

	if (iEnd - iStart + 1 < 10)
	{
		if (!PopDisplayStack(iStart, iEnd))
			return;
		else
			m_pObjMgr->SetDisplayGraph(iStart, iEnd);
	}
	else
	{
		SaveCurrentPos();
		m_pObjMgr->SetDisplayGraph(iStart, iEnd);
	}

	m_pObjMgr->m_pDataFormat->SaveDay(m_pObjMgr->GetDisplayDay());
	m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispScaleChart), m_pObjMgr->GetDisplayDay());
}

void CObjEvent::DrawMouseArea(CPoint point, bool bMid, CDC* pDC)
{
	CDC* pDC2;
	if (!pDC)
		pDC2 = m_pParent->GetDC();
	else
		pDC2 = pDC;

	CPen	*ppenBG = m_pObjMgr->m_pGMainApp->GetPen(m_pObjMgr->m_pwndView, PS_SOLID, 1, m_pObjMgr->GetEnvInfo()->display.rgbChartBG);
	CBrush	*pbrushGray = m_pObjMgr->m_pGMainApp->GetBrush(m_pObjMgr->m_pwndView, RGB(20, 20, 20));
	CPen	*sPen = pDC2->SelectObject(ppenBG);
	CBrush	*sBrush = pDC2->SelectObject(pbrushGray);
	int		sRop = pDC2->SetROP2(R2_XORPEN);
	int		sMode = pDC2->SetBkMode(TRANSPARENT);


	CRect rect = m_pObjMgr->m_rectGrp;
	rect.left = m_pointStartSize.x;
	rect.right = m_pointEndSize.x;
	rect.NormalizeRect();
	rect.IntersectRect(&rect, &m_pObjMgr->m_rectGrp);
	CRect rect2;
	if (bMid)
	{
		pDC2->MoveTo(m_pointEndSize.x, m_pObjMgr->m_rectGrp.top);
		pDC2->LineTo(m_pointEndSize.x, m_pObjMgr->m_rectGrp.bottom);

		m_pointEndSize.x = point.x;
		rect2 = m_pObjMgr->m_rectGrp;
		rect2.left = m_pointStartSize.x;
		rect2.right = m_pointEndSize.x;
		rect2.NormalizeRect();
		rect2.IntersectRect(&rect2, &m_pObjMgr->m_rectGrp);
	}
	else
	{
		rect2.SetRectEmpty();
	}

	pDC2->MoveTo(m_pointEndSize.x, m_pObjMgr->m_rectGrp.top);
	pDC2->LineTo(m_pointEndSize.x, m_pObjMgr->m_rectGrp.bottom);

	CSize sz = CSize(rect.Width(), rect.Height());
	CSize sz2 = CSize(rect2.Width(), rect2.Height());
	pDC2->DrawDragRect(&rect, sz, &rect2, sz2, pbrushGray, pbrushGray);

	pDC2->SelectObject(sPen);
	pDC2->SelectObject(sBrush);
	pDC2->SetBkMode(sMode);
	pDC2->SetROP2(sRop);

	if (!pDC)
		m_pParent->ReleaseDC(pDC2);
}

int CObjEvent::GetPosition(int iPos, CPoint point, bool bBalance)
{
	CIndcBase *pindcMain = m_pObjMgr->GetMainGraph();
	CRect	rectDraw  = pindcMain->GetDrawRect();

	int iDispDay  = pindcMain->GetDisplayDay();
	int iDispPos = pindcMain->GetDisplayPos();
	int iTotalDay = pindcMain->GetTotalDataCount();
	double dSizeOneDay = double(rectDraw.Width()) / double(iDispDay);

	int iIndex;
	if (point.x >= 0)
	{
		if (point.x < rectDraw.left)
		{
			point.x = rectDraw.left + int(dSizeOneDay/2.0);
		}
		else if (point.x > rectDraw.right)
		{
			point.x = rectDraw.right - int(dSizeOneDay/2.0);
		}
		else
		{
			iIndex = static_cast<int>( (static_cast<double>(point.x) - static_cast<double>(rectDraw.left)) / dSizeOneDay );
			point.x = rectDraw.left + int ( static_cast<double>(iIndex) * dSizeOneDay + dSizeOneDay/2.0 );
		}

		return point.x;
	}

	iIndex = static_cast<int>( (static_cast<double>(iPos) - static_cast<double>(rectDraw.left)) / dSizeOneDay );
	if (iDispPos + iIndex >= iTotalDay)
		iIndex = iTotalDay - iDispPos - 1;

	return iDispPos + iIndex;
}

void CObjEvent::DrawSelectArea(CDC* pDC)
{
	if (m_iDragRgnKey < 0)
		return;

	int sMode = pDC->SetBkMode(TRANSPARENT);
	int	sROP = pDC->SetROP2(R2_XORPEN);
	CPen	*cPen = m_pObjMgr->m_pGMainApp->GetPen(m_pObjMgr->m_pwndView, PS_SOLID, 2, _L_WHITE);
	CPen	*sPen = pDC->SelectObject(cPen);

	CRect	drawAREA = m_rectDrag;
	pDC->MoveTo(drawAREA.left, drawAREA.top);
	pDC->LineTo(drawAREA.right, drawAREA.top);
	pDC->LineTo(drawAREA.right, drawAREA.bottom);
	pDC->LineTo(drawAREA.left, drawAREA.bottom);
	pDC->LineTo(drawAREA.left, drawAREA.top);

	pDC->SelectObject(sPen);
	pDC->SetBkMode(sMode);
	pDC->SetROP2(sROP);
}

void CObjEvent::ToolHideHolder()
{
	CToolBase	*pToolBase;
	for (int ii = m_arToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		if (ii == m_iToolSelectIndex)
			continue;

		pToolBase = m_arToolQue.GetAt(ii);
		pToolBase->HideHolder();
	}
}

// 2006.10.16 schbang 수정
void CObjEvent::MoveGraph(CPoint point)
{
	switch (m_pindcSelect->GetGraphKind())
	{
	case GK_JPN:
	case GK_LIN:
	case GK_BAR:
	case GK_POLE:
	case GK_AVOL:
	case GK_BALANCE:
		return;
	default:
		break;
	}

	CIndcBase* pindcPoint = SearchPtInGraph(point);
	int iMovRgn = pindcPoint->GetGraphRegionID();
	int iDelRgn = m_pindcSelect->GetGraphRegionID();
	if (iMovRgn == iDelRgn)
		return;

	switch (m_pObjMgr->m_pDataFormat->GetMainGraph())
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:	
	case GK_BALANCE:
	case GK_AVOL:
		break;
	case GK_EQV:
	case GK_CDV:
	case GK_THREE:	
	case GK_PNF:
	case GK_SWING:
	case GK_CLOCK:
	case GK_PV:
	case GK_KAGI:
	case GK_RENKO:
		return;
	}

// 추가 시작
	CIndcBase* pindcSelect = m_pindcSelect;	// m_pindcSelect가 널로 변하므로 따로 변수를 둠
	if (!m_pObjMgr->YShowConfigByMove(pindcSelect, iMovRgn))
		return;
// 추가 끝

	int iRgnCnt;
	int iGrpCnt;
	char* pcGrpInfo = m_pObjMgr->m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);

	int iPreTopH = 0;
	int iPreTopR = 0;
	int iRgnKey;
	struct	_xRegion* pxRegion = nullptr;
	for (POSITION pos = m_pObjMgr->m_mapRegion.GetStartPosition(); pos; )
	{
		m_pObjMgr->m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
		if (pxRegion->pRect.top == 0)
		{
			iPreTopH = pxRegion->xRect.top;
			break;
		}
	}

	// 옮겨지는 그래프의 영역에 그래프가 하나만 있을경우 영역을 삭제
	if (m_pObjMgr->GetCountGraphinRgn(iDelRgn) <= 1)
		m_pObjMgr->RemoveGraphRegion(iDelRgn);
	
	// 삭제후 다시 추가
	int ii = 0;
	pindcSelect->SetGraphRegionID(iMovRgn);
	CIndcBase* pindcTemp = NULL;
	for (ii = m_pObjMgr->m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pindcTemp = m_pObjMgr->m_arGraphQue.GetAt(ii);
		if (pindcSelect == pindcTemp)
		{
			m_pObjMgr->m_arGraphQue.RemoveAt(ii);
			break;
		}
	}
	m_pObjMgr->m_arGraphQue.Add(pindcSelect);

	

	// 새로운 영역
	int iCount = m_pObjMgr->m_mapRegion.GetCount();
	UINT* apuiSortKey = new UINT[iCount]{ 0, };
	UINT* apuiSortTop = new UINT[iCount]{ 0, };

	// Key 별로 영역의 top 위치를 array에....  sort를 위해
	ii = 0;
	for (POSITION pos = m_pObjMgr->m_mapRegion.GetStartPosition(); pos; )
	{
		m_pObjMgr->m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
		apuiSortKey[ii] = iRgnKey;
		apuiSortTop[ii] = pxRegion->xRect.top;
		ii++;
	}

	// 버블 소트
	for (ii = 0; ii < iCount; ii++)
	{
		for (int jj = ii+1; jj < iCount; jj++)
		{
			if (apuiSortTop[ii] > apuiSortTop[jj])
			{
				apuiSortTop[ii] += apuiSortTop[jj];
				apuiSortKey[ii] += apuiSortKey[jj];

				apuiSortTop[jj] = apuiSortTop[ii] - apuiSortTop[jj];
				apuiSortKey[jj] = apuiSortKey[ii] - apuiSortKey[jj];

				apuiSortTop[ii] = apuiSortTop[ii] - apuiSortTop[jj];
				apuiSortKey[ii] = apuiSortKey[ii] - apuiSortKey[jj];
			}
		}
	}

	double dMRatio, dARatio;
	switch (iCount)
	{
	case 1:
		dMRatio = 1.0;
		dARatio = 0.1;
		break; 
	case 2:
		dMRatio = 0.7;
		dARatio = 0.3;
		break;
	case 3:
		dMRatio = 0.6;
		dARatio = 0.2;
		break;
	case 4:
		dMRatio = 0.46;	
		dARatio = 0.18;
		break;
	case 5:
		dMRatio = 0.40;
		dARatio = 0.15;
		break;
	case 6:
		dMRatio = 0.30;
		dARatio = 0.14;
		break;
	case 7:
		dMRatio = 0.28;
		dARatio = 0.12;
		break;
	default:  
		dMRatio = 0.28;
		dARatio = 0.72 / ( static_cast<double>(iCount) - 1 );
		break;
	}

	int iAddHeight;
	struct _region* pRegion = nullptr;
	CIndcBase* pindcMain = m_pObjMgr->GetMainGraph();
	UINT uiMainRgn = pindcMain->GetGraphRegionID();
	double dRatio;
	for (ii = 0; ii < iCount; ii++)
	{
		if (apuiSortKey[ii])
		{
// updateXXX_202204
			if (!m_pObjMgr->m_mapRegion.Lookup(apuiSortKey[ii], pxRegion))
				continue;
		}

		if (pxRegion == nullptr)
			continue;

		if (apuiSortKey[ii] == uiMainRgn)
			dRatio = dMRatio;
		else
			dRatio = dARatio;

		iAddHeight = int(m_pObjMgr->m_rectGrp.Height() * dRatio);
		pxRegion->xRect.top = iPreTopH;
		pxRegion->xRect.bottom = pxRegion->xRect.top + iAddHeight;
		iPreTopH = pxRegion->xRect.bottom;

		pxRegion->pRect.top = iPreTopR;
		if (ii == iCount - 1)	// 마지막거
		{
			pxRegion->xRect.bottom = m_pObjMgr->m_rectGrp.bottom;
			pxRegion->pRect.bottom = 10000;
		}
		else
		{
			pxRegion->pRect.bottom = pxRegion->pRect.top + int(dRatio * 10000);
		}
		iPreTopR = pxRegion->pRect.bottom;

		// 저장되어질 영역정보에 update
		for (int jj = 0; jj < iRgnCnt; jj++)
		{
			pRegion = (struct _region*)&pcGrpInfo[SZ_REGION * jj];
			if (apuiSortKey[ii] == pRegion->btRKey)
			{
				pRegion->wRTop = WORD(pxRegion->pRect.top);
				pRegion->wRBottom = WORD(pxRegion->pRect.bottom);
				break;
			}
		}
	}

	// 새정보 저장
	char* pcNewGrpInfo = new char[SZ_REGION * iCount + SZ_GRAPH * iGrpCnt];
	int iNRCnt = 0;
	for (ii = 0; ii < iRgnCnt; ii++)
	{
		struct _region *pRegion = (struct _region *)&pcGrpInfo[SZ_REGION * ii];
		if (m_pObjMgr->m_mapRegion.Lookup(pRegion->btRKey, pxRegion))
		{
			CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt], &pcGrpInfo[SZ_REGION * ii], SZ_REGION);
			iNRCnt++;
		}
	}

	int iIndex = 0;
	for (ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
	{
		CIndcBase* pindcTemp = m_pObjMgr->m_arGraphQue.GetAt(ii);
		//if (pindcTemp->IsUnion())
		if (!pindcTemp->IsUnion() && (pindcTemp->GetGraphKind() == GK_AVOL || pindcTemp->GetGraphKind() == GK_BALANCE))
			continue;

		CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt + SZ_GRAPH * iIndex++], pindcTemp->GetGrpInfo(), SZ_GRAPH);
	}

	m_pObjMgr->m_pDataFormat->SetFgraph(pcNewGrpInfo, iNRCnt, iGrpCnt);
	delete[] pcNewGrpInfo;

	// 소트정보 지우기
	if (apuiSortKey)
		delete[] apuiSortKey;
	if (apuiSortTop)
		delete[] apuiSortTop;

	m_pObjMgr->ReviseTick();
	m_pObjMgr->ResizeGraphObject();
	m_pObjMgr->ReDrawGraphObject(true);
}

void CObjEvent::MoveGraphToNewRegion(CPoint point)
{
	int iDelRgn = m_pindcSelect->GetGraphRegionID();
	UINT uiGrpKey = m_pindcSelect->GetGraphID();

	switch (m_pindcSelect->GetGraphKind())
	{
	case GK_JPN:
	case GK_LIN:
	case GK_BAR:
	case GK_POLE:
	case GK_AVOL:
	case GK_BALANCE:
		return;
	default:
		break;
	}

	switch (m_pObjMgr->m_pDataFormat->GetMainGraph())
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:	
	case GK_BALANCE:
	case GK_AVOL:
		break;
	case GK_EQV:
	case GK_CDV:
	case GK_THREE:
	case GK_PNF:
	case GK_SWING:
	case GK_CLOCK:
	case GK_PV:
	case GK_KAGI:
	case GK_RENKO:
		return;
	}

	bool bDelete = false;
	if (m_pObjMgr->GetCountGraphinRgn(iDelRgn) <= 1) 
	{
		bDelete = true;
		m_pObjMgr->RemoveGraphRegion(iDelRgn);
	}

	double	ratio, Mratio, Aratio;
	int	preTopH = 0;
	int	preTopR = 0;
	int	count, rcnt, gcnt;

	char *pGrpInfo = m_pObjMgr->m_pDataFormat->GetGrpInfo(rcnt, gcnt);
	count = m_pObjMgr->m_mapRegion.GetCount();
	UINT	*sortKEY = new UINT[count];
	UINT	*sortTOP = new UINT[count];

	int	ii = 0;
	int	rgnKey;
	struct	_xRegion* xRegion = nullptr;
	for (POSITION pos = m_pObjMgr->m_mapRegion.GetStartPosition(); pos; )
	{
		m_pObjMgr->m_mapRegion.GetNextAssoc(pos, rgnKey, xRegion);
		sortKEY[ii] = rgnKey;
		sortTOP[ii] = xRegion->xRect.top;
		ii++;
	}

	for (ii = 0; ii < count; ii++)
	{
		for (int jj = ii+1; jj < count; jj++)
		{
			if (sortTOP[ii] > sortTOP[jj])
			{
				sortTOP[ii] += sortTOP[jj];
				sortKEY[ii] += sortKEY[jj];

				sortTOP[jj] = sortTOP[ii] - sortTOP[jj];
				sortKEY[jj] = sortKEY[ii] - sortKEY[jj];

				sortTOP[ii] = sortTOP[ii] - sortTOP[jj];
				sortKEY[ii] = sortKEY[ii] - sortKEY[jj];
			}
		}
	}

	CIndcBase *pMainGrp = m_pObjMgr->GetMainGraph();
	UINT	MainRgn = pMainGrp->GetGraphRegionID();
	UINT	aRegionKey = m_pObjMgr->GetUnusedRegionKey();

	switch (count)
	{
	case 1:
		Mratio = 0.7;
		Aratio = 0.3;
		break;
	case 2: 
		Mratio = 0.6;
		Aratio = 0.2;
		break;
	case 3:
		Mratio = 0.46;
		Aratio = 0.18;
		break;
	case 4:
		Mratio = 0.40;
		Aratio = 0.15;
		break;
	case 5: 
		Mratio = 0.30;
		Aratio = 0.14;
		break;
	case 6:
		Mratio = 0.28;
		Aratio = 0.12;
		break;
	default:  
		Mratio = 0.28;	
		Aratio = 0.72 / double(count);
		break;
	}

	int	addHeight;
	for (ii = 0; ii < count; ii++)
	{
		m_pObjMgr->m_mapRegion.Lookup(sortKEY[ii], xRegion);
		if (sortKEY[ii] == MainRgn)	ratio = Mratio;
		else	ratio = Aratio;

		addHeight = int(m_pObjMgr->m_rectGrp.Height() * ratio);
		xRegion->xRect.top = preTopH;
		xRegion->xRect.bottom = xRegion->xRect.top + addHeight;
		preTopH = xRegion->xRect.bottom;

		xRegion->pRect.top = preTopR;
		xRegion->pRect.bottom = xRegion->pRect.top + int(ratio*10000);
		preTopR = xRegion->pRect.bottom;

		for (int jj = 0; jj < rcnt; jj++)
		{
			struct _region *pRegion = (struct _region*)&pGrpInfo[SZ_REGION*jj];
			if (sortKEY[ii] == pRegion->btRKey)
			{
				pRegion->wRTop = WORD(xRegion->pRect.top);
				pRegion->wRBottom = WORD(xRegion->pRect.bottom);
			}
		}
	}

	struct	_xRegion* appRegion;
	appRegion = (struct _xRegion *)new char [sizeof (struct _xRegion)];

	if (xRegion != nullptr)
		CopyMemory(appRegion, xRegion, sizeof (struct _xRegion));
	appRegion->xRect.top = preTopH;
	appRegion->xRect.bottom = m_pObjMgr->m_rectGrp.bottom;
	appRegion->pRect.top = preTopR;
	appRegion->pRect.bottom = 10000;

	m_pObjMgr->m_mapRegion.SetAt(aRegionKey, appRegion);

	char	*pNewGrpInfo;
	if (bDelete)
		pNewGrpInfo = new char[SZ_REGION * (rcnt) + SZ_GRAPH * (gcnt)];
	else
		pNewGrpInfo = new char[SZ_REGION * (rcnt+1) + SZ_GRAPH * (gcnt)];

	struct	_region	region;
	region.btRKey = aRegionKey;
	region.wRLeft = WORD(appRegion->pRect.left);
	region.wRTop = WORD(appRegion->pRect.top);
	region.wRRight = WORD(appRegion->pRect.right);
	region.wRBottom = WORD(appRegion->pRect.bottom);
	
	m_pindcSelect->SetGraphRegionID(aRegionKey);

	int	newRCnt = 0;
	if (bDelete)
	{
		for (ii = 0; ii < rcnt; ii++)
		{
			struct _region *tregion = (_region *)&pGrpInfo[SZ_REGION * ii];
			if (tregion->btRKey != iDelRgn)
			{
				CopyMemory(&pNewGrpInfo[newRCnt*SZ_REGION], tregion, SZ_REGION);
				newRCnt++;
			}
		}
	}
	else
	{
		newRCnt = rcnt;
		CopyMemory(pNewGrpInfo, pGrpInfo, SZ_REGION * rcnt);

		for (ii = 0; ii < rcnt; ii++)
		{
			struct _region *tregion = (_region *)&pGrpInfo[SZ_REGION * ii];
		}
	}
	
	CopyMemory(&pNewGrpInfo[SZ_REGION * newRCnt], &region, SZ_REGION);
	CopyMemory(&pNewGrpInfo[SZ_REGION * (newRCnt+1)], &pGrpInfo[SZ_REGION * rcnt], SZ_GRAPH * gcnt);

	_graph	*graph = (_graph *)m_pindcSelect->GetGrpInfo();
	for (ii = 0; ii < gcnt; ii++)
	{
		_graph	*tgraph = (_graph *)&pNewGrpInfo[SZ_REGION * (newRCnt+1) + SZ_GRAPH*ii];
		if (graph->wKey == tgraph->wKey)
		{
			tgraph->btRgnKey = aRegionKey;
			break;
		}
	}

	m_pObjMgr->m_pDataFormat->SetFgraph(pNewGrpInfo, newRCnt+1, gcnt);

	delete[] pNewGrpInfo;

	if (sortKEY) delete[] sortKEY;
	if (sortTOP) delete[] sortTOP;

	m_pObjMgr->ReviseTick();
	m_pObjMgr->ResizeGraphObject();
	m_pObjMgr->ReDrawGraphObject(true);
}

void CObjEvent::SwapRegion(int RgnKey1, int RgnKey2)
{
	struct _xRegion	xRegion, *xRegion1, *xRegion2;
	if (!m_pObjMgr->m_mapRegion.Lookup(RgnKey1, xRegion1))
		return;
	if (!m_pObjMgr->m_mapRegion.Lookup(RgnKey2, xRegion2))
		return;

	int	rcnt, gcnt;
	char *pGrpInfo = m_pObjMgr->m_pDataFormat->GetGrpInfo(rcnt, gcnt);
	for (int ii = 0; ii < rcnt; ii++)
	{
		struct _region *pRegion = (struct _region *)&pGrpInfo[SZ_REGION*ii];
		if (pRegion->btRKey == UINT(RgnKey1))
		{
			pRegion->wRLeft = UINT(xRegion2->pRect.left);
			pRegion->wRTop = UINT(xRegion2->pRect.top);
			pRegion->wRRight = UINT(xRegion2->pRect.right);
			pRegion->wRBottom = UINT(xRegion2->pRect.bottom);
		}
		if (pRegion->btRKey == UINT(RgnKey2))
		{
			pRegion->wRLeft = UINT(xRegion1->pRect.left);
			pRegion->wRTop = UINT(xRegion1->pRect.top);
			pRegion->wRRight = UINT(xRegion1->pRect.right);
			pRegion->wRBottom = UINT(xRegion1->pRect.bottom);
		}
	}
	
	CopyMemory(&xRegion, xRegion2, sizeof(_xRegion));
	xRegion2->pRect = xRegion1->pRect;
	xRegion2->xRect = xRegion1->xRect;
	xRegion1->pRect = xRegion.pRect;
	xRegion1->xRect = xRegion.xRect;

	m_pObjMgr->ResizeGraphObject();
	m_pObjMgr->ReDrawGraphObject(true);
}

void CObjEvent::AddSplitMap()
{
	RemoveSplitMap();

	struct _xRegion* xRegion;
	int iRgnKey = 0;
	struct _sRegion	*pRect;
	for (POSITION pos = m_pObjMgr->m_mapRegion.GetStartPosition(); pos; )
	{
		m_pObjMgr->m_mapRegion.GetNextAssoc(pos, iRgnKey, xRegion);
		if (!xRegion)
			continue;
		if (xRegion->xRect.bottom+rgnGAP > m_pObjMgr->m_rectGrp.bottom)
			continue;
		pRect = new _sRegion;
		pRect->left = m_pObjMgr->m_rectObj.left;//xRegion->xRect.left;
		pRect->top = xRegion->xRect.bottom-rgnGAP;
		pRect->right = m_pObjMgr->m_rectObj.right;//xRegion->xRect.right;
		pRect->bottom = xRegion->xRect.bottom+rgnGAP;

		m_mapSplit.SetAt(iRgnKey, pRect);
	}
}

void CObjEvent::RemoveSplitMap()
{
	_sRegion* pRect;
	int iRgnKey;
	for (POSITION pos = m_mapSplit.GetStartPosition(); pos; )
	{
		m_mapSplit.GetNextAssoc(pos, iRgnKey, pRect);
		if (!pRect)
			continue;
		LIB_DELETE(pRect);
	}
	
	m_mapSplit.RemoveAll();
}

int CObjEvent::IsSplitPos(CPoint pt, CRect& rRC)
{
	rRC.SetRectEmpty();
	
	_sRegion* pRect;
	int	rKey = 0;
	for (POSITION pos = m_mapSplit.GetStartPosition(); pos; )
	{
		m_mapSplit.GetNextAssoc(pos, rKey, pRect);
		if (!pRect)
			continue;

		if (pRect->top <= pt.y && pRect->bottom >= pt.y
			&& pRect->left <= pt.x && pRect->right >= pt.x)
		{
			rRC.SetRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
			return rKey;
		}
	}

	return 0;
}

CRect CObjEvent::GetSplitMoveRect(int rKey)
{
	CRect rc;
	rc.SetRectEmpty();

	CRect regionRC;
	m_pObjMgr->GetGrpRegion(rKey, regionRC);
	if (regionRC.IsRectEmpty())
		return rc;

	rc = m_pObjMgr->m_rectObj;
	rc.top = regionRC.top;

	int bottomKey = 0;
	m_iSplitTopID = rKey;
	if ((rc.bottom = m_pObjMgr->GetRegionAdjustBottom(regionRC.bottom, bottomKey)) < 0)
		rc.bottom = m_pObjMgr->m_rectGrp.bottom;
	m_iSplitBtmID = bottomKey;
	return rc;
}

void CObjEvent::DrawSplitLine(CDC *pDC)
{
	_sRegion* pRect;
	int rgnKey;

	CPen	*cPen = m_pObjMgr->m_pGMainApp->GetPen(m_pObjMgr->m_pwndView, PS_SOLID, 1, RGB(128,128,128));
	CPen	*sPen = pDC->SelectObject(cPen);
	for (POSITION pos = m_mapSplit.GetStartPosition(); pos; )
	{
		m_mapSplit.GetNextAssoc(pos, rgnKey, pRect);
		if (!pRect)
			continue;

		pDC->MoveTo(pRect->left, pRect->top + rgnGAP);
		pDC->LineTo(pRect->right, pRect->bottom - rgnGAP);
	}
	pDC->SelectObject(sPen);
}

void CObjEvent::DrawSplit()
{
	if (m_rectSplitCur.IsRectEmpty() && m_rectSplitPre.IsRectEmpty())
		return;

	CClientDC clientDC(m_pParent);
	CDC* pDC = &clientDC;
	if (!m_rectSplitPre.IsRectEmpty())
		pDC->DrawFocusRect(m_rectSplitPre);
	if (!m_rectSplitCur.IsRectEmpty())
		pDC->DrawFocusRect(m_rectSplitCur);
}

void CObjEvent::ReAssignRegion(CPoint pt)
{
	int newY = pt.y;

	struct _xRegion* pUpReg;
	struct _xRegion* pDnReg;

	if (!m_pObjMgr->m_mapRegion.Lookup(m_iSplitTopID, pUpReg))
		return;
	if (!m_pObjMgr->m_mapRegion.Lookup(m_iSplitBtmID, pDnReg))
		return;
	if (newY == pUpReg->xRect.bottom)
		return;

	CRect	xRect;
	_sRegion	pRect;	
	xRect = pUpReg->xRect;
	xRect.bottom = pDnReg->xRect.bottom;
	pRect = pUpReg->pRect;
	pRect.bottom = pDnReg->pRect.bottom;

	if (newY < xRect.top + rgnGAP*2)
		newY += rgnGAP*2;
	if (newY > xRect.bottom - rgnGAP*2)
		newY -= rgnGAP*2;

	int minHeight = xRect.Height();
	int grpHeight = 11;
	if (minHeight < grpHeight)	grpHeight = minHeight;

	if (xRect.bottom - newY < grpHeight)
		newY = 	xRect.bottom - grpHeight;
	else if (newY - xRect.top < grpHeight)
		newY = 	xRect.top + grpHeight;

	pUpReg->xRect.bottom = newY;
	pDnReg->xRect.top = newY;

	m_pObjMgr->SaveRegion();
	m_pObjMgr->ResizeGraphObject();
	m_pObjMgr->ReDrawGraphObject(true);
}

void CObjEvent::AddMarkMap()
{
	switch (m_pObjMgr->GetMainKind())
	{
	case GK_CDV:
	case GK_EQV:
	case GK_PNF:
	case GK_THREE:
	case GK_SWING:
	case GK_PV:
	case GK_CLOCK:
	case GK_KAGI:
	case GK_RENKO:
		return;
	default:
		break;
	}

	RemoveMarkMap();

	struct _envInfo	*envinfo = m_pObjMgr->GetEnvInfo();
	bool	bRight = (envinfo->display.dwDspOption & DO_DISPRT)? true:false;
	bool	bLeft = (envinfo->display.dwDspOption & DO_DISPLT)? true:false;

	if (!bLeft && !bRight)
		return;

	struct _xRegion* xRegion;
	int	rgnKey = 0;
	struct _mRegion	*pMark;
	int	mainRgn = m_pObjMgr->GetMainGraph()->GetGraphRegionID();
	for (POSITION pos = m_pObjMgr->m_mapRegion.GetStartPosition(); pos; )
	{
		m_pObjMgr->m_mapRegion.GetNextAssoc(pos, rgnKey, xRegion);
		if (!xRegion)
			continue;
		if (mainRgn == rgnKey)
			continue;

		if (abs(xRegion->xRect.bottom - xRegion->xRect.top) <= markGAP)
			continue;

		pMark = new _mRegion;

		pMark->rgnKey = rgnKey;
		pMark->org.left = m_pObjMgr->m_rectObj.left;
		pMark->org.top = xRegion->xRect.top;
		pMark->org.right = m_pObjMgr->m_rectObj.right;
		pMark->org.bottom = xRegion->xRect.bottom;

		if (bLeft)
		{
			pMark->left.left = pMark->org.left + 2;
			pMark->left.top = xRegion->xRect.top + 2;
			pMark->left.right = pMark->left.left + markGAP;
			pMark->left.bottom = pMark->left.top + markGAP;
		}
		else
		{
			pMark->left.left = 0;
			pMark->left.top = 0;
			pMark->left.right = 0;
			pMark->left.bottom = 0;
		}

		if (bRight)
		{
			pMark->right.right = pMark->org.right - 2;
			pMark->right.top = xRegion->xRect.top + 2;
			pMark->right.left = pMark->right.right - markGAP;
			pMark->right.bottom = pMark->right.top + markGAP;
		}
		else
		{
			pMark->right.left = 0;
			pMark->right.top = 0;
			pMark->right.right = 0;
			pMark->right.bottom = 0;
		}

		m_mapMark.SetAt(rgnKey, pMark);
	}
}

void CObjEvent::RemoveMarkMap()
{
	_mRegion* pMark;
	int	rgnKey;
	for (POSITION pos = m_mapMark.GetStartPosition(); pos; )
	{
		m_mapMark.GetNextAssoc(pos, rgnKey, pMark);
		if (!pMark)	continue;
		LIB_DELETE(pMark);
	}
	
	m_mapMark.RemoveAll();
}

int CObjEvent::IsMarkPos(CPoint point)
{
	_mRegion* pMark;
	int iRKey = 0;
	for (POSITION pos = m_mapMark.GetStartPosition(); pos; )
	{
		m_mapMark.GetNextAssoc(pos, iRKey, pMark);
		if (!pMark)
			continue;

		if (pMark->org.top <= point.y && pMark->org.bottom >= point.y
			&& pMark->org.left <= point.x && pMark->org.right >= point.x)
		{
			CRect rectLeft(pMark->left.left, pMark->left.top, pMark->left.right, pMark->left.bottom);
			CRect rectRight(pMark->right.left, pMark->right.top, pMark->right.right, pMark->right.bottom);
			if (rectLeft.PtInRect(point) || rectRight.PtInRect(point))
				return iRKey;
		}			
	}

	return 0;
}

void CObjEvent::DrawDeleteMark(CDC *pDC)
{
	_mRegion* pMark;
	int	rgnKey;

	CRect	rc;
	CPen	*cPen = m_pObjMgr->m_pGMainApp->GetPen(m_pObjMgr->m_pwndView, PS_SOLID, 1, RGB(0,0,0));
	CPen	*fPen = m_pObjMgr->m_pGMainApp->GetPen(m_pObjMgr->m_pwndView, PS_SOLID, 1, RGB(128, 128, 128));
	CPen	*sPen = pDC->SelectObject(cPen);
	
	CBrush	*cBrush = m_pObjMgr->m_pGMainApp->GetBrush(m_pObjMgr->m_pwndView, RGB(211, 211, 211));
	CBrush	*sBrush = pDC->SelectObject(cBrush);

	CPoint	polygon[5];
	for (POSITION pos = m_mapMark.GetStartPosition(); pos; )
	{
		m_mapMark.GetNextAssoc(pos, rgnKey, pMark);
		if (!pMark)	continue;

		rc.SetRect(pMark->left.left, pMark->left.top, pMark->left.right, pMark->left.bottom);
		if (!rc.IsRectEmpty())
		{
			polygon[0] = CPoint(pMark->left.left, pMark->left.top);
			polygon[1] = CPoint(pMark->left.left, pMark->left.bottom);
			polygon[2] = CPoint(pMark->left.right, pMark->left.bottom);
			polygon[3] = CPoint(pMark->left.right, pMark->left.top);
			polygon[4] = CPoint(pMark->left.left, pMark->left.top);
			pDC->SelectObject(fPen);
			pDC->Polygon(polygon, 5);

			pDC->SelectObject(cPen);
			pDC->MoveTo(pMark->left.left+1, pMark->left.top+1);
			pDC->LineTo(pMark->left.right, pMark->left.bottom);
			pDC->MoveTo(pMark->left.right-1, pMark->left.top+1);
			pDC->LineTo(pMark->left.left, pMark->left.bottom);

		}
		
		rc.SetRect(pMark->right.left, pMark->right.top, pMark->right.right, pMark->right.bottom);
		if (!rc.IsRectEmpty())
		{			
			polygon[0] = CPoint(pMark->right.left, pMark->right.top);
			polygon[1] = CPoint(pMark->right.left, pMark->right.bottom);
			polygon[2] = CPoint(pMark->right.right, pMark->right.bottom);
			polygon[3] = CPoint(pMark->right.right, pMark->right.top);
			polygon[4] = CPoint(pMark->right.left, pMark->right.top);
			pDC->SelectObject(fPen);
			pDC->Polygon(polygon, 5);

			pDC->SelectObject(cPen);
			pDC->MoveTo(pMark->right.left+1, pMark->right.top+1);
			pDC->LineTo(pMark->right.right, pMark->right.bottom);
			pDC->MoveTo(pMark->right.right-1, pMark->right.top+1);
			pDC->LineTo(pMark->right.left, pMark->right.bottom);
		}
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CObjEvent::RemoveMarkRegion(int iDelKey)
{
	double dRatio, dMRatio, dARatio;
	int iPreTopH = 0;
	int iPreTopR = 0;
	int iCount, iRgnCnt, iGrpCnt;
	UINT* puiSortKey = NULL;
	UINT* puiSortTop = NULL;
	char *pcGrpInfo = m_pObjMgr->m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);

	m_pObjMgr->RemoveGraphRegion(iDelKey);
	iCount = m_pObjMgr->m_mapRegion.GetCount();
	puiSortKey = new UINT[iCount];
	puiSortTop = new UINT[iCount];

	int ii = 0, jj = 0, iRgnKey = 0;
	struct _xRegion* pxRegion = nullptr;
	for (POSITION pos = m_pObjMgr->m_mapRegion.GetStartPosition(); pos; )
	{
		m_pObjMgr->m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
		puiSortKey[ii] = iRgnKey;
		puiSortTop[ii] = pxRegion->xRect.top;
		ii++;
	}

	for (ii = 0; ii < iCount; ii++)
	{
		for (int jj = ii+1; jj < iCount; jj++)
		{
			if (puiSortTop[ii] > puiSortTop[jj])
			{
				puiSortTop[ii] += puiSortTop[jj];
				puiSortKey[ii] += puiSortKey[jj];

				puiSortTop[jj] = puiSortTop[ii] - puiSortTop[jj];
				puiSortKey[jj] = puiSortKey[ii] - puiSortKey[jj];

				puiSortTop[ii] = puiSortTop[ii] - puiSortTop[jj];
				puiSortKey[ii] = puiSortKey[ii] - puiSortKey[jj];
			}
		}
	}

	switch (iCount)
	{
	case 1:	dMRatio = 1.0;	dARatio = 0.1;	break; 
	case 2: dMRatio = 0.7;	dARatio = 0.3;	break;
	case 3: dMRatio = 0.6;	dARatio = 0.2;	break;
	case 4: dMRatio = 0.46;	dARatio = 0.18;	break;
	case 5: dMRatio = 0.40;	dARatio = 0.15;	break;
	case 6: dMRatio = 0.30;	dARatio = 0.14;	break;
	case 7: dMRatio = 0.28;	dARatio = 0.12;	break;
	default:  
		dMRatio = 0.28;	
		dARatio = 0.72 / ( static_cast<double>(iCount) - 1 );
		break;
	}

	int iAddHeight;
	struct _region* pRegion;
	CIndcBase* pMainGrp = m_pObjMgr->GetMainGraph();
	UINT uiMainRgn = pMainGrp->GetGraphRegionID();
	for (ii = 0; ii < iCount; ii++)
	{
		m_pObjMgr->m_mapRegion.Lookup(puiSortKey[ii], pxRegion);
		if (puiSortKey[ii] == uiMainRgn)
			dRatio = dMRatio;
		else
			dRatio = dARatio;

		iAddHeight = int(m_pObjMgr->m_rectGrp.Height() * dRatio);
		pxRegion->xRect.top = iPreTopH;
		pxRegion->xRect.bottom = pxRegion->xRect.top + iAddHeight;
		iPreTopH = pxRegion->xRect.bottom;

		pxRegion->pRect.top = iPreTopR;
		if (ii == iCount - 1)
		{
			pxRegion->xRect.bottom = m_pObjMgr->m_rectGrp.bottom;
			pxRegion->pRect.bottom = 10000;
		}
		else
		{
			pxRegion->pRect.bottom = pxRegion->pRect.top + int(dRatio*10000);
		}
		iPreTopR = pxRegion->pRect.bottom;

		for (int jj = 0; jj < iRgnCnt; jj++)
		{
			pRegion = (struct _region*)&pcGrpInfo[SZ_REGION*jj];
			if (puiSortKey[ii] == pRegion->btRKey)
			{
				pRegion->wRTop = WORD(pxRegion->pRect.top);
				pRegion->wRBottom = WORD(pxRegion->pRect.bottom);
				break;
			}
		}
	}

	char* pcNewGrpInfo = new char[SZ_REGION * iCount + SZ_GRAPH * iGrpCnt];
	int iNRCnt = 0;
	for (ii = 0; ii < iRgnCnt; ii++)
	{
		struct _region* pRegion = (struct _region *)&pcGrpInfo[SZ_REGION * ii];
		if (m_pObjMgr->m_mapRegion.Lookup(pRegion->btRKey, pxRegion))
		{
			CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt], &pcGrpInfo[SZ_REGION * ii], SZ_REGION);
			iNRCnt++;
		}
	}

	int iTGCnt = 0;
	CIndcBase* pIndcBase;
	int* piGrpIdx = new int[m_pObjMgr->m_arGraphQue.GetSize()];
	int* piGrpKind = new int[m_pObjMgr->m_arGraphQue.GetSize()];
	for (ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
	{
		pIndcBase = m_pObjMgr->m_arGraphQue.GetAt(ii);
		if (pIndcBase->GetGraphRegionID() == iDelKey)
		{
			if (m_arToolQue.GetSize() > 0)
			{
				int iTDCnt = 0;
				int *piToolIdx = new int[m_arToolQue.GetSize()];
				for (int jj = 0; jj < m_arToolQue.GetSize(); jj++)
				{
					CToolBase *pToolBase = m_arToolQue.GetAt(jj);
					if (((CIndcBase *)pToolBase->GetGrpBase()) == pIndcBase)
					{
						delete pToolBase;
						pToolBase = NULL;
						piToolIdx[iTDCnt++] = jj;
					}
				}
				for (jj = iTDCnt - 1; jj >= 0; jj--)
					m_arToolQue.RemoveAt(piToolIdx[jj]);

				delete[] piToolIdx;
			}

			piGrpIdx[iTGCnt++] = ii;
			piGrpKind[iTGCnt-1] = pIndcBase->GetGraphKind();
			delete pIndcBase;
			pIndcBase = NULL;
		}
	}
	for (ii = iTGCnt - 1; ii >= 0; ii--)
		m_pObjMgr->m_arGraphQue.RemoveAt(piGrpIdx[ii]);
	for (ii = iTGCnt - 1; ii >= 0; ii--)
	{
		bool bExist = false;
		for (int jj = 0; jj < m_pObjMgr->m_arGraphQue.GetSize(); jj++)
		{
			pIndcBase = m_pObjMgr->m_arGraphQue.GetAt(jj);
			if (pIndcBase->GetGraphKind() == piGrpKind[ii])
			{
				bExist = true;
				break;
			}
		}
		if (!bExist)
			m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, aidDelChart), piGrpKind[ii]);
	}

	delete[] piGrpIdx;
	delete[] piGrpKind;

	int iNGCnt = 0;
	for (ii = 0; ii < iGrpCnt; ii++)
	{
		struct _graph	*graph = (struct _graph *)&pcGrpInfo[SZ_REGION * iRgnCnt + SZ_GRAPH * ii];
		if (graph->btRgnKey == iDelKey)
			continue;

		CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt + SZ_GRAPH * iNGCnt], graph, SZ_GRAPH);
		iNGCnt++;
	}

	m_pObjMgr->m_pDataFormat->SetFgraph(pcNewGrpInfo, iNRCnt, iNGCnt);
	delete[] pcNewGrpInfo;

	if (puiSortKey)
		delete[] puiSortKey;
	if (puiSortTop)
		delete[] puiSortTop;

	m_pObjMgr->ReviseTick();
	m_pObjMgr->ResizeGraphObject();
	m_pObjMgr->ReDrawGraphObject(true);

	if (m_pObjMgr->GetTableMode())
	{
		m_pObjMgr->GetTableData();
		m_pParent->SendMessage(GEV_TABLE, tbItem, long(m_pObjMgr->m_strData.operator LPCTSTR()));
	}
}

int CObjEvent::DisplayModifyToolMenu()
{
	struct _menu {
		UINT	uFlag;
		char	item[32];
	} menuString [] = 
	{ 
		{MFT_STRING,		"도구복사"},
		{MFT_STRING,		"도구삭제"},
		{MFT_STRING,		"전체삭제"},
	};

	CPoint pointCur;
	GetCursorPos(&pointCur);

	CToolBase* pToolBase;
	HMENU hMenu = CreatePopupMenu();
	for (int ii = 0; ii < sizeof(menuString) / sizeof(menuString[0]); ii++)
	{
		if (ii == 0)
		{
			pToolBase = m_arToolQue.GetAt(m_iToolSelectIndex);
			if (pToolBase->GetToolName() != (int) enumTK::tkTENDENCY
				&& pToolBase->GetToolName() != (int) enumTK::tkTENDENCYEX)
				continue;
		}

		AppendMenu(hMenu, menuString[ii].uFlag, modifyMENUID + ii, menuString[ii].item);
	}

	return TrackPopupMenu(hMenu, TPM_RIGHTBUTTON|TPM_RETURNCMD, pointCur.x, pointCur.y, 0, m_pParent->m_hWnd, NULL);
}

int CObjEvent::DisplayChartMenu(int& aKind, CPoint pt)
{
	CPoint	curPoint;
	GetCursorPos(&curPoint);

	CMenu baseMENU;
	baseMENU.CreatePopupMenu();
	AppendChartMenu(&baseMENU);

	m_pParent->SendMessage(WM_USER + 999, (WPARAM)_T("EXTM_POPUPMENU"), (LPARAM)&baseMENU);

	int	menuID = baseMENU.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD,
		curPoint.x, curPoint.y, m_pParent, NULL);

	if (menuID <= 0)
	{
		aKind = 0;
		return menuID;
	}
	else if (menuID >= MENU_CHART_AID)
		aKind = g_aMenuAid[menuID - MENU_CHART_AID].nKind;
	else if (menuID >= MENU_CHART_TYPE)
		aKind = g_aMenuType[menuID - MENU_CHART_TYPE].nKind;

	/** remarking by macho@2006.12.12 <begin>
	if (menuID == 3242)
		aKind = GK_ORGNAMT;//	aKind++;
	<end> remarking by macho@2006.12.12 **/

	return menuID;
}

void CObjEvent::AppendChartMenu(CMenu *baseMENU, bool bInner)
{
	bool	bExcelChart = true;
	bool	bSingleChart = false;
	bool	bExtraChart = false;
	bool	bNoTrace = false;	
	int	mkind = m_pObjMgr->GetMainKind();
	switch (mkind)
	{
	case GK_EQV:
	case GK_CDV:
		bSingleChart = true;
		bExtraChart = true;
		break;
	case GK_CLOCK:
	case GK_PV:	
		bExcelChart = false;
		bSingleChart = true;
		bNoTrace = true;
		break;	
	case GK_THREE:
	case GK_PNF:
	case GK_SWING:
	case GK_KAGI:
	case GK_RENKO:
		bExcelChart = false;
		bSingleChart = true;
		bExtraChart = true;
		bNoTrace = true;
		break;
	}

	m_menuType.DestroyMenu();
	m_menuKind.DestroyMenu();
	for (int ii = 0; ii < 8; ii++)
		m_amenuAid[ii].DestroyMenu();

	int menuID = 0;
	int baseID = MENU_CHART_BASE;
	UINT	uFlag = 0;
	for (int ii = 0; ii < sizeof(g_aMenuBase) / sizeof(g_aMenuBase[0]); ii++)
	{
		baseID = MENU_CHART_BASE + ii;
		uFlag = g_aMenuBase[ii].uFlag;
		switch (baseID)
		{
		case MENU_CHART_BASE:
		case MENU_CHART_BASE+1:
		case MENU_CHART_BASE+4:
		case MENU_CHART_BASE+5:
			if (!m_bMapChart)
				continue;
			break;
		case MENU_CHART_BASE+2:
		case MENU_CHART_BASE+3:
			continue;
		case MENU_CHART_BASE+6:	// 차트유형
			if (!bInner)
				continue;

			{
				int insertID;
				m_menuType.CreatePopupMenu();
				for (int jj = 0; jj < sizeof(g_aMenuType) / sizeof(g_aMenuType[0]); jj++)
				{
					insertID = MENU_CHART_TYPE + jj;
					UINT	uFlag = g_aMenuType[jj].uFlag;

					if (mkind == g_aMenuType[jj].nKind)
						uFlag |= MF_CHECKED;

					m_menuType.AppendMenu(uFlag, insertID, g_aMenuType[jj].item);
				}

				baseMENU->InsertMenu(baseID, MF_BYPOSITION|MF_POPUP,
					UINT(m_menuType.GetSafeHmenu()), "차트유형");
			}
			continue;
		case MENU_CHART_BASE+7:	// 차트추가
			if (!bInner)
				continue;
			if (bSingleChart)
				continue;

			{
				int	insertID;
				int	mCnt = sizeof(g_aMenuAid) / sizeof(g_aMenuAid[0]);
				m_menuKind.CreatePopupMenu();

				int	jj = 0;
				if (!m_bMapChart)
				{
					m_amenuAid[jj].CreatePopupMenu();
					for (int kk = 0; kk < m_iBMCnt; kk++)
					{
						for (int ll = 0; ll < mCnt; ll++)
						{
							if (m_acBookMark[kk] == g_aMenuAid[ll].nKind)
							{
								insertID = MENU_CHART_AID + ll;
								m_amenuAid[jj].AppendMenu(g_aMenuAid[ll].uFlag, insertID, g_aMenuAid[ll].item);
								break;
							}
						}
					}
					m_menuKind.InsertMenu(jj, MF_BYPOSITION|MF_POPUP, UINT(m_amenuAid[jj].GetSafeHmenu()), g_aMenuKind[jj].item);
				}
				jj++;

				mCnt = sizeof(g_aMenuKind) / sizeof(g_aMenuKind[0]);
				int	sIndex = 0;
				for (; jj < mCnt; jj++)
				{
					m_amenuAid[jj].CreatePopupMenu();
					for (int kk = 0; kk < g_aMenuKind[jj].nKind; kk++, sIndex++)
					{
						insertID = MENU_CHART_AID + sIndex;
						m_amenuAid[jj].AppendMenu(g_aMenuAid[sIndex].uFlag, insertID, g_aMenuAid[sIndex].item);
					}					
					m_menuKind.InsertMenu(jj, MF_BYPOSITION|MF_POPUP, UINT(m_amenuAid[jj].GetSafeHmenu()), g_aMenuKind[jj].item);
				}

				baseMENU->InsertMenu(baseID, MF_BYPOSITION|MF_POPUP, 
					UINT(m_menuKind.GetSafeHmenu()), "지표추가");
			}
			continue;
		case MENU_CHART_BASE+8:	// 수치조회
			if (!bInner)
				continue;
			if (m_bMapChart)
				continue;

			if ((bSingleChart && !bExtraChart))	
				uFlag |= MF_GRAYED;
			if (m_pObjMgr->IsGridMode())
				uFlag |= MF_CHECKED;
			break;
		case MENU_CHART_BASE+9:	// 자료조회
//			if (bSingleChart && !bExtraChart)	continue;
			if (!bInner)
				continue;
			if (m_bMapChart)
				continue;
			if (m_pObjMgr->GetTableMode())
				uFlag |= MF_CHECKED;
			break;
		case MENU_CHART_BASE+10:	// Log모드
			if (!bInner)
				continue;
			if (bNoTrace)
				continue;
			if (m_pObjMgr->IsLogMode())
				uFlag |= MF_CHECKED;
			break;
		case MENU_CHART_BASE+11:	// 대칭모드
			if (!bInner)
				continue;
			if (bNoTrace)
				continue;
			if (m_pObjMgr->IsReverseMode())
				uFlag |= MF_CHECKED;
			break;
		case MENU_CHART_BASE+12:	// 초기화
			if (!m_bMapChart)
				continue;
			break;
		case MENU_CHART_BASE+13:	// 1st seperator
			if (!bInner)
				continue;
			if (bNoTrace && bSingleChart && !bExcelChart)
				continue;
			break;


		case MENU_CHART_BASE+14:	// 화면설정
			continue;
			if (bSingleChart)
				continue;
			break;
		case MENU_CHART_BASE+15:	// 차트설정
			if (!bInner)
				continue;

			if (!m_pindcSelect)
			{
				uFlag |= MF_GRAYED;
				break;
			}
			break;
		case MENU_CHART_BASE+16:	// 차트삭제
			if (!bInner)
				continue;
			if (bSingleChart)
				continue;

			switch (mkind)
			{
			case GK_JPN:
			case GK_LIN:
			case GK_BAR:
			case GK_POLE:
			case GK_AVOL:
			case GK_BALANCE:
				break;
			default:
				continue;
			}

			if (!m_pindcSelect)
			{
				uFlag |= MF_GRAYED;
				break;
			}

			switch (m_pindcSelect->GetGraphKind())
			{
			case GK_JPN:
			case GK_LIN:
			case GK_BAR:
			case GK_POLE:
			case GK_AVOL:
			case GK_BALANCE:
				uFlag |= MF_GRAYED;
				break;
			}
			break;
		case MENU_CHART_BASE+17:	// 환경설정
			if (!bInner)
				continue;
			break;
		//** macho modify
		//** 
		case MENU_CHART_BASE + 18:  // Separator
			if (!bInner)
				continue;
			if (bInner || m_bMapChart) continue;

			continue;
		case MENU_CHART_BASE + 19:  // 시세 보이기
			if (bInner || m_bMapChart) continue;

			if (IsPanelShow(showSise))
				uFlag |= MF_CHECKED;
			break;

		case MENU_CHART_BASE + 20:  // 도구바 보이기 숨기기
			if (bInner || m_bMapChart) continue;

			if (IsPanelShow(showTool))
				uFlag |= MF_CHECKED;
			break;
		case MENU_CHART_BASE + 21:  // 지표바 보이기 숨기기
			if (bInner || m_bMapChart) continue;

			if (IsPanelShow(showAid))
				uFlag |= MF_CHECKED;
			break;
		case MENU_CHART_BASE + 22:  // 도구바 설정
			if (!bInner)
				continue;
			if (bInner || m_bMapChart) continue;

			break;
		case MENU_CHART_BASE + 23:  // 지표바 설정
			if (!bInner)
				continue;
			if (bInner || m_bMapChart) continue;

			break;
		
		case MENU_CHART_BASE+24:	// 3st seperator
			if (!bInner)
				continue;
			if (m_bMapChart)
				continue;
			break;
		case MENU_CHART_BASE+25:	// Excel
			if (!bInner)
				continue;
			if (!bExcelChart || m_bMapChart)
				continue;
			break;
		case MENU_CHART_BASE+26:	// 이미지 저장하기
			if (!bInner)
				continue;
			if (m_bMapChart)
				continue;
			break;
		case MENU_CHART_BASE+27:	// 인쇄하기
			if (!bInner)
				continue;
			if (m_bMapChart)
				continue;
			break;

		case MENU_CHART_BASE+28:	// seperator
//			if (bSingleChart && !bExtraChart)	continue;
			if (!bInner)
				continue;
			if (m_bMapChart && !bInner)
				continue;
			break;

		case MENU_CHART_BASE+29:	// 십자선숨기기
			if (!bInner)
				continue;
//			if (bNoTrace && !bExtraChart)	continue;
			if (!m_pObjMgr->IsCrossMode())
			{
				uFlag |= MF_CHECKED;
			}
			break;
		case MENU_CHART_BASE+30:	// 십자선 종가따라가기
			if (!bInner)
				continue;
			if (bNoTrace)
				continue;
			if (m_pObjMgr->IsJTraceMode())
			{
				uFlag |= MF_CHECKED;
			}
			break;

// 2006.10.26 schbang
/*
		case MENU_CHART_BASE+25:
			if (mkind != GK_PNF)
				continue;
			if (!m_pindcSelect)
				uFlag |= MF_GRAYED;
			break;
		case MENU_CHART_BASE+26:
			if (mkind != GK_KAGI)
				continue;
			if (!m_pindcSelect)
				uFlag |= MF_GRAYED;
			break;
		case MENU_CHART_BASE+27:
			if (mkind != GK_RENKO)
				continue;
			if (!m_pindcSelect)
				uFlag |= MF_GRAYED;
			break;
*/
		default:
			break;
		}
		baseMENU->AppendMenu(uFlag, baseID, g_aMenuBase[ii].item);
	}
}

void CObjEvent::ProcessChartMenu(int menuID, int aKind)
{
	switch (menuID)
	{
	case 0:					// 선택 에러
		break;
	case MENU_CHART_BASE:		// 추세선
		ToolSelect(toolTEND);
		break;
	case MENU_CHART_BASE+1:	// 십자선
		ToolSelect(toolCROSS);
		break;
	case MENU_CHART_BASE+2:	// 수평선
	case MENU_CHART_BASE+3:	// 수직선
		break;
	case MENU_CHART_BASE+4:	// 전체지우기
		ToolSelect(toolDELETE);
		break;
	case MENU_CHART_BASE+8:	// 수치조회
		m_pObjMgr->SetGridMode();
		break;
	case MENU_CHART_BASE+9:	// 자료조회
		m_pObjMgr->SetTableMode(!m_pObjMgr->GetTableMode());	
		break;
	case MENU_CHART_BASE+10:	// 로그모드
		m_pObjMgr->SetLogMode();
		break;
	case MENU_CHART_BASE+11:	// 대칭모드
		m_pObjMgr->SetReverseMode();
		break;
	case MENU_CHART_BASE+12:	// 초기화
		m_pObjMgr->InitGraph();
		break;
	case MENU_CHART_BASE+14:			// 화면설정
		m_pObjMgr->MapConfig();
		break;
	case MENU_CHART_BASE+15:			// 차트설정
		m_pObjMgr->ChartConfig(m_pindcSelect);
		break;
	case MENU_CHART_BASE+16:			// 챠트 삭제
		m_pObjMgr->DeleteGraph(m_pindcSelect);
		break;
	case MENU_CHART_BASE+17:			// 환경설정
		m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgEnvchart), 0);
		break;
	case MENU_CHART_BASE + 19: // 시세 보이기
		m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, showSise), !IsPanelShow(showSise));
		break;
	case MENU_CHART_BASE + 20: // 도구바 보이기/숨기기
		m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, showTool), !IsPanelShow(showTool));
		break;
	case MENU_CHART_BASE + 21: // 지표바 보이기/숨기기
		m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, showAid), !IsPanelShow(showAid));
		break;
	case MENU_CHART_BASE + 22: // 도구바 설정
		m_pObjMgr->m_pwndPnChart->SendMessage(GEV_TOOL, MAKEWPARAM(TOOL_CONFIG, NULL), NULL);
		break;
	case MENU_CHART_BASE + 23: // 지표바 설정
		m_pObjMgr->m_pwndPnChart->SendMessage(GEV_AID, MAKEWPARAM(AID_CONFIG, NULL), NULL);
		break;
	case MENU_CHART_BASE + 25:	// Excel
		m_pObjMgr->Excel();
		break;
	case MENU_CHART_BASE + 26:	// 이미지로 저장
		m_pObjMgr->SaveImage();
		break;
	case MENU_CHART_BASE + 27:	// 인쇄
		m_pObjMgr->Print();
		break;
	case MENU_CHART_BASE + 29:			// 십자선
		m_pObjMgr->SetCrossMode();
		break;
	case MENU_CHART_BASE + 30:			// 십자선 종가따라가기
		m_pObjMgr->SetJTraceMode();
		break;

/*
	case MENU_CHART_BASE+25:			// PNF
	case MENU_CHART_BASE+26:			// Kagi
	case MENU_CHART_BASE+27:			// Renko
		m_pObjMgr->SetUserDefined(m_pindcSelect);
		break;
*/
	default:
		break;
	}

	if (menuID >= MENU_CHART_AID) 
	{
		m_pObjMgr->ChangeMenuAKind(aKind);
	}
	else if (menuID >= MENU_CHART_TYPE) 
	{
		m_pObjMgr->ChangeMenuMKind(aKind);
	}
}

void CObjEvent::SetBookMark(char *ulist)
{
	m_iBMCnt = 0;
	CopyMemory(m_acBookMark, ulist, sizeof(m_acBookMark));

	for (int ii = 0; ii < MAX_UGRP; ii++, m_iBMCnt++)
	{
		if (m_acBookMark[ii] <= 0)
			break;
	}
}

void CObjEvent::TipMouseMove(CPoint point)
{
	m_pParent->ScreenToClient(&point);

	MouseMove(true, point);
}

void CObjEvent::MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly)
{
	if (bCrossOnly)
	{
		if (m_nIimerID > 0)
		{
			m_pParent->KillTimer(m_nIimerID);
			m_nIimerID = 0;
		}

		if (!m_strPosData.IsEmpty())
		{
			m_nIimerID = TIMER_AID;
			m_pParent->SetTimer(m_nIimerID, TIMER_AGAP, NULL);
		}

		return;
	}

	if (m_strPosData.IsEmpty())
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	if (!bIsTipEvent)
	{
		if (m_pToolTip->IsWindowVisible())
			return;
	}

	CPoint	pt = point;
	m_pParent->ClientToScreen(&pt);
	pt.x += 10;
	pt.y += 10;

	CRect	rect;
	CSize	size;

	rect = m_pToolTip->SetData(m_strPosData);

	size.cx = rect.Width();
	size.cy = rect.Height();

	int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	if ((nWidth - pt.x) < rect.Width())
		pt.x -= rect.Width() + 20;

	rect.left = pt.x;
	rect.right = pt.x + size.cx;
	rect.top = pt.y;
	rect.bottom = rect.top + size.cy;

	m_pToolTip->WindowMove(rect.left, rect.top, rect.Width(), rect.Height());

	if (m_pToolTip->IsWindowVisible())
		m_pToolTip->Invalidate(FALSE);
	else
		m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);

	if (m_nIimerID > 0)
		m_pParent->KillTimer(m_nIimerID);

	m_nIimerID = TIMER_AID;
	m_pParent->SetTimer(m_nIimerID, TIMER_AGAP, NULL);
}

void CObjEvent::SetViewTooltip()
{
	m_bShowTip = !m_bShowTip;
}

void CObjEvent::SetViewTooltip(bool bSet)
{
	m_bShowTip = bSet;
}

void CObjEvent::DrawTopLine(CDC *pDC)
{
	CPen	*cPen = m_pObjMgr->m_pGMainApp->GetPen(m_pObjMgr->m_pwndView, PS_SOLID, 1, RGB(128,128,128));
	CPen	*sPen = pDC->SelectObject(cPen);

	CRect rectC;
	m_pParent->GetClientRect(rectC);
	pDC->MoveTo(rectC.left, rectC.top);
	pDC->LineTo(rectC.right, rectC.top);

	pDC->SelectObject(sPen);
}

CIndcBase* CObjEvent::SearchPtInTick(CPoint point)
{
	CIndcBase* pIndcBase = NULL;
	for (int ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
	{
		pIndcBase = m_pObjMgr->m_arGraphQue.GetAt(ii);
		if (pIndcBase->IsPointInTickRect(point))
		{
			break;
		}
	}

	return pIndcBase;
}

void CObjEvent::RemoveAndrewPichfork()
{
	CToolBase *pToolBase;

	int idx;
	
	idx = m_arToolQue.GetUpperBound();
	
	if (idx < 0)
		return;
	//for (int ii = 0; ii < m_arToolQue.GetSize(); ii++)
	//{
		//pToolBase = m_arToolQue.GetAt(ii);
		pToolBase = m_arToolQue.GetAt(idx);

		if (pToolBase->m_eToolKind == enumTK::tkANDREWPICHFORK && pToolBase->m_PointCount != 0)
		{
			pToolBase->m_PointCount = 0;
			RemoveLastToolQue();
			m_pObjMgr->ReDrawGraphObject(true);
			//break;
		}
	//}
}

// 2006.10.13 schbang
bool CObjEvent::ProcessSmallMenu(CPoint point)
{
	bool bLabel;
	if (!m_pindcSelect || m_pindcSelect != IsPtOverLine(point, true, bLabel))
		return false;

#define ID_SMALL_DELETE		1
#define ID_SMALL_PROPERTY	2
#define ID_SMALL_YSHOW		3
#define ID_SMALL_YCONFIG	4
	

	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_SMALL_PROPERTY, "속성");
	menu.AppendMenu(MF_STRING, ID_SMALL_DELETE, "삭제");
	menu.AppendMenu(MF_STRING, ID_SMALL_YSHOW, "Y축 눈금자 설정");

	// 2006.10.26 schbang
	switch (m_pindcSelect->GetGraphKind())
	{
	case GK_PNF:
		menu.AppendMenu(MF_STRING, ID_SMALL_YCONFIG, "한칸의값 입력");
		break;		
	case GK_KAGI:
		menu.AppendMenu(MF_STRING, ID_SMALL_YCONFIG, "최소반전폭 입력");
		break;
	case GK_RENKO:
		menu.AppendMenu(MF_STRING, ID_SMALL_YCONFIG, "최소변화폭 입력");
		break;
	}


	CPoint pointScr = point;
	m_pParent->ClientToScreen(&pointScr);
	int iMenuID = menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, pointScr.x, pointScr.y, m_pParent, NULL);

	switch (iMenuID)
	{
	case ID_SMALL_DELETE:
		KeyboardEvent(VK_DELETE);
		break;

	case ID_SMALL_PROPERTY:
		m_pObjMgr->ChartConfig(m_pindcSelect);
		break;

	case ID_SMALL_YSHOW:
		return YShowGraph(point);
		break;
	case ID_SMALL_YCONFIG:
		m_pObjMgr->SetUserDefined(m_pindcSelect);
		break;
	}

	return true;
}

// 2006.10.13 schbang 추가
bool CObjEvent::YShowGraph(CPoint point)
{
	bool bLabel;
	if (m_pindcSelect != IsPtOverLine(point, true, bLabel))
		return false;

	CIndcBase* pindcSelect = m_pindcSelect;
	if (!m_pObjMgr->YShowConfigByMenu(pindcSelect))
		return true;

	// 새정보 저장
	int iRgnCnt;
	int iGrpCnt;
	char* pcGrpInfo = m_pObjMgr->m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);
	char* pcNewGrpInfo = new char[SZ_REGION * iRgnCnt + SZ_GRAPH * iGrpCnt];
	int iNRCnt = 0;
	struct	_xRegion* pxRegion;
	for (int ii = 0; ii < iRgnCnt; ii++)
	{
		struct _region *pRegion = (struct _region *)&pcGrpInfo[SZ_REGION * ii];
		if (m_pObjMgr->m_mapRegion.Lookup(pRegion->btRKey, pxRegion))
		{
			CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt], &pcGrpInfo[SZ_REGION * ii], SZ_REGION);
			iNRCnt++;
		}
	}

	int iIndex = 0;
	for (int ii = 0; ii < m_pObjMgr->m_arGraphQue.GetSize(); ii++)
	{
		CIndcBase* pindcTemp = m_pObjMgr->m_arGraphQue.GetAt(ii);
		//if (pindcTemp->IsUnion())
		if (!pindcTemp->IsUnion() && (pindcTemp->GetGraphKind() == GK_AVOL || pindcTemp->GetGraphKind() == GK_BALANCE))
			continue;

		CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt + SZ_GRAPH * iIndex++], pindcTemp->GetGrpInfo(), SZ_GRAPH);
	}

	m_pObjMgr->m_pDataFormat->SetFgraph(pcNewGrpInfo, iNRCnt, iGrpCnt);
	delete[] pcNewGrpInfo;

	m_pObjMgr->ReviseTick();
	m_pObjMgr->ResizeGraphObject();
	m_pObjMgr->ReDrawGraphObject(true);
	return true;
}

BOOL CObjEvent::IsPanelShow(UINT pnKind)
{
	return (BOOL)m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, pnKind), 9999);
}
