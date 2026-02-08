// ToolMgr.cpp: implementation of the CToolMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolMgr.h"

#include "UserString.h"
#include "Tendency.h"
#include "TendencyEx.h"
#include "HLine.h"
#include "VLine.h"
#include "AdCross.h"
#include "Figure.h"
#include "ArrowTool.h"
#include "Daedeung.h"
#include "Gigan.h"
#include "FiboArc.h"
#include "FiboFan.h"
#include "FiboFan2.h"
#include "FiboLine.h"
#include "FiboRet.h"
#include "GannLine.h"
#include "GannFan.h"
#include "QuardLine.h"
#include "AccelArc.h"
#include "AccelFan.h"
#include "AndrewPF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolMgr::CToolMgr()	{}
CToolMgr::~CToolMgr()	{}

CToolBase* CToolMgr::CreateTool(CWnd* pwndView, CWnd* pwndParent, CRect rectTool, CRect rectDraw, COLORREF rgbBG, CIndcBase* pIndcBase, int iToolKind)
{
	CToolBase* pToolBase = (CToolBase *) 0;

	switch (static_cast<enumTK>(iToolKind))
	{
	case enumTK::tkUSRINPUT:
		pToolBase = new CUserString(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkTENDENCY:
		pToolBase = new CTendency(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkTENDENCYEX:
		pToolBase = new CTendencyEx(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkHLINE:
		pToolBase = new CHLine(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkVLINE:
		pToolBase = new CVLine(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkCROSS:
		pToolBase = new CAdCross(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkRECT:
	case enumTK::tkTRI:
	case enumTK::tkARC:
		pToolBase = new CFigure(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkUARROW:
	case enumTK::tkDARROW:
	case enumTK::tkLARROW:
	case enumTK::tkRARROW:
		pToolBase = new CArrowTool(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkDAEDEUNG:
		pToolBase = new CDaedeung(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkGIGAN:
	case enumTK::tkGIGAN2:
		pToolBase = new CGigan(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkFIBOLINE:
		pToolBase = new CFiboLine(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkFIBOFAN:
		pToolBase = new CFiboFan(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkFIBOFAN2:
		pToolBase = new CFiboFan2(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkFIBOARC:
		pToolBase = new CFiboArc(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkFIBORET:
		pToolBase = new CFiboRet(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkGANNLINE:
		pToolBase = new CGannLine(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkGANNFAN:
		pToolBase = new CGannFan(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkRGANNFAN:
		pToolBase = new CGannFan(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkTRILINE:
	case enumTK::tkTRILINELEVEL:
	case enumTK::tkQUARDLINE:
		pToolBase = new CQuardLine(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkACCELARC:
		pToolBase = new CAccelArc(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkACCELFAN:
		pToolBase = new CAccelFan(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	case enumTK::tkANDREWPICHFORK:
		pToolBase = new CAndrewPF(pwndView, pwndParent, rectTool, rectDraw, rgbBG, pIndcBase, iToolKind);
		break;
	default:
		return (CToolBase *) 0;
	}

	return pToolBase;
}

CToolBase* CToolMgr::CopyTool(CToolBase* pToolBase)
{
	switch (static_cast<enumTK>(pToolBase->GetToolName()))
	{
	case enumTK::tkTENDENCY:
		{
			CTendency *pCopyToolBase = new CTendency((CTendency *)pToolBase);
			return pCopyToolBase;
		}
	case enumTK::tkTENDENCYEX:
		{
			CTendencyEx *pCopyToolBase = new CTendencyEx((CTendencyEx *)pToolBase);
			return pCopyToolBase;
		}
	}

	return NULL;
}
