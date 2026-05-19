
#include "stdafx.h"
#include "../ChildView.h"
#include "awAlignBAr.h"
#include "awObjectLoad.h"
#include "../h/mapform.h"
#include "fmTracker.h"
#include "../h/fmObject.h"
#include "awFormCtrl.h"

#define ALIGN_LEFT		0
#define ALIGN_RIGHT		1
#define ALIGN_TOP		2
#define ALIGN_BOTTOM		3
#define ALIGN_HCENTER		4
#define ALIGN_VCENTER		5
#define ALIGN_WHCENTER		6
#define ALIGN_WVCENTER		7
#define QUAL_H			8
#define QUAL_V			9
#define QUAL_HSIZE		10
#define QUAL_VSIZE		11

CFmAlignBar::CFmAlignBar(CChildView* view, class CFmCtrl* FmCtrl)
{
	m_FmCtrl = FmCtrl;
	m_view = view;
	m_stdRc.SetRectEmpty();
	m_iSpace = 0;
}

CFmAlignBar::~CFmAlignBar()
{
}

CPoint CFmAlignBar::GetAlignRect(int iPos, CRect &rc, bool bStd)
{
	if(bStd) m_stdRc.UnionRect(m_stdRc,rc);
	CPoint iOffsetPos;
	switch(iPos)
	{
	case ALIGN_LEFT:
		iOffsetPos.x = rc.left;
		iOffsetPos.y = 0;
		break;
	case ALIGN_RIGHT:
		iOffsetPos.x = rc.right;
		iOffsetPos.y = 0;
		break;
	case ALIGN_TOP:
		iOffsetPos.x = 0;
		iOffsetPos.y = rc.top;
		break;
	case ALIGN_BOTTOM:
		iOffsetPos.x = 0;
		iOffsetPos.y = rc.bottom;
		break;
	case ALIGN_HCENTER:
		if(bStd)
		{
			iOffsetPos.x = rc.left;
			iOffsetPos.y = 0;
		}
		else
		{
			if(rc.EqualRect(m_stdRc))
			{
				iOffsetPos.x = rc.left;
			}
			else
			{
				iOffsetPos.x = rc.left - ((m_stdRc.Width() - rc.Width()) / 2);
			}
			iOffsetPos.y = 0;
		}
		break;
	case ALIGN_VCENTER:
		if(bStd)
		{
			iOffsetPos.x = 0;
			iOffsetPos.y = rc.top;
		}
		else
		{
			iOffsetPos.x = 0;
			if(rc.EqualRect(m_stdRc))
			{
				iOffsetPos.y = rc.top;
			}
			else
			{
				iOffsetPos.y = rc.top - ((m_stdRc.Height() - rc.Height()) / 2);
			}
		}
		break;
	case ALIGN_WHCENTER:
		if(bStd)
		{
			CRect rrc;
			GetClientRect(m_view->m_hWnd,rrc);
			iOffsetPos.x = rrc.Width()/2 - m_stdRc.Width()/2;
			iOffsetPos.y = 0;
		}
		else
		{
			iOffsetPos.x = m_stdRc.left;
			iOffsetPos.y = 0;
		}
		break;
	case ALIGN_WVCENTER:
		if(bStd)
		{
			CRect rrc;
			GetClientRect(m_view->m_hWnd,rrc);
			iOffsetPos.x = 0;
			iOffsetPos.y = rrc.Height()/2 - m_stdRc.Height()/2;
		}
		else
		{
			iOffsetPos.x = 0;
			iOffsetPos.y = m_stdRc.top;
		}
		break;
	case QUAL_H:
		if(bStd)
		{
			iOffsetPos.x = m_stdRc.left;
			iOffsetPos.y = 0;
			m_stdRc.CopyRect(rc);
		}
		else
		{
			if(rc.EqualRect(m_stdRc))
			{
				iOffsetPos.x = rc.left;
			}
			else 
			{
				iOffsetPos.x = m_stdRc.left + (rc.left - m_stdRc.right) - m_iSpace;
			}
			iOffsetPos.y = 0;
			m_stdRc.right = rc.right + (m_stdRc.left - iOffsetPos.x);
		}
		break;
	case QUAL_V:
		if(bStd)
		{
			iOffsetPos.x = 0;
			iOffsetPos.y = m_stdRc.top;
			m_stdRc.CopyRect(rc);
		}
		else
		{
			iOffsetPos.x = 0;
			if(rc.EqualRect(m_stdRc))
			{
				iOffsetPos.y = rc.top;
			}
			else 
			{
				iOffsetPos.y = m_stdRc.top + (rc.top - m_stdRc.bottom) - m_iSpace;
			}
			m_stdRc.bottom = rc.bottom + (m_stdRc.top - iOffsetPos.y);
		}
		break;
	case QUAL_HSIZE:
		iOffsetPos.x = 0;
		iOffsetPos.y = 0;
		rc.right = rc.left + m_stdRc.Width();
		break;
	case QUAL_VSIZE:
		iOffsetPos.x = 0;
		iOffsetPos.y = 0;
		rc.bottom = rc.top + m_stdRc.Height();
		break;
	}

	return iOffsetPos;
}

void CFmAlignBar::AlignObject(int iPos)
{
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_view->m_selF))->getProp();
	CPoint iSelPos = 0;
	CPoint iCurPos = 0;

	if(Prop->group)
	{
		if(iPos == QUAL_HSIZE || iPos == QUAL_VSIZE)
			iSelPos = GetAlignRect(iPos,Prop->rect,true);
		else
			iSelPos = GetAlignRect(iPos,Prop->groupRc,true);
		//iSelPos = GetAlignRect(iPos,Prop->groupRc,true);
	}
	else
	{
		iSelPos = GetAlignRect(iPos,Prop->rect,true);
	}

	CfmTracker* fmTracker = NULL;
	CformProp* curProp = NULL;
	CfmObject* Obj = NULL;
	CWnd* Wnd = NULL;
	for (size_t ii = 0; ii < (size_t)m_FmCtrl->m_fmTkArry.GetCount(); ii++)
	{
		fmTracker = m_FmCtrl->m_fmTkArry.GetAt(ii);
		curProp = fmTracker->m_ObjLoad->getProp();
		Obj = fmTracker->m_ObjLoad->getObject();
		Wnd = fmTracker->m_ObjLoad->getWnd();
		//if(Prop == curProp) continue;

		if(curProp->group)
		{
			if(iPos == QUAL_HSIZE || iPos == QUAL_VSIZE)
			{
				iCurPos = 0;
				curProp->groupRc.SetRectEmpty();
			}
			else
			{
				iCurPos = iSelPos - GetAlignRect(iPos,curProp->groupRc,false);
				curProp->groupRc.OffsetRect(iCurPos);
			}
			CformProp* subProp = NULL;
			CfmObject* subObj = NULL;
			CWnd* subWnd = NULL;
			for (size_t jj = 0; jj < (size_t)m_view->m_mapH->forms.GetCount(); jj++)
			{
				subProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(jj))->getProp();
				if(curProp == subProp->group)
				{
					subObj = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(jj))->getObject();
					subWnd = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(jj))->getWnd();

					if(iPos == QUAL_HSIZE || iPos == QUAL_VSIZE)
						GetAlignRect(iPos,subProp->rect,false);
					else
						subProp->rect.OffsetRect(iCurPos);
					curProp->groupRc.UnionRect(curProp->groupRc,subProp->rect);
					subObj->Reload();
					subWnd->MoveWindow(subProp->rect);
				}
			}
			fmTracker->SetPostion(curProp->groupRc.left, curProp->groupRc.top, curProp->groupRc.Width(), curProp->groupRc.Height());
			fmTracker->MoveTracker();
			fmTracker->Redraw();
		}
		else
		{
			iCurPos = iSelPos - GetAlignRect(iPos,curProp->rect,false);
			curProp->rect.OffsetRect(iCurPos);
			Obj->Reload();
			Wnd->MoveWindow(curProp->rect);
			fmTracker->SetPostion(curProp->rect.left, curProp->rect.top, curProp->rect.Width(), curProp->rect.Height());
			fmTracker->MoveTracker();
			fmTracker->Redraw();
		}
	}
	m_FmCtrl->InvalidateObject();
}
void CFmAlignBar::AlignLeft()
{
	m_stdRc.SetRectEmpty();
	AlignObject(ALIGN_LEFT);
}
void CFmAlignBar::AlignRight()
{
	m_stdRc.SetRectEmpty();
	AlignObject(ALIGN_RIGHT);
}
void CFmAlignBar::AlignTop()
{
	m_stdRc.SetRectEmpty();
	AlignObject(ALIGN_TOP);
}
void CFmAlignBar::AlignBottom()
{
	m_stdRc.SetRectEmpty();
	AlignObject(ALIGN_BOTTOM);
}
void CFmAlignBar::AlignHCenter()
{
	m_stdRc.SetRectEmpty();
	AlignObject(ALIGN_HCENTER);
}
void CFmAlignBar::AlignVCenter()
{
	m_stdRc.SetRectEmpty();
	AlignObject(ALIGN_VCENTER);
}
void CFmAlignBar::AlignWHCenter()
{
	AlignUnionRect();
	AlignObject(ALIGN_WHCENTER);
}
void CFmAlignBar::AlignWVCenter()
{
	AlignUnionRect();
	AlignObject(ALIGN_WVCENTER);
}

void CFmAlignBar::AlignUnionRect()
{
	m_stdRc.SetRectEmpty();
	CfmTracker* fmTracker = NULL;
	CformProp* curProp = NULL;
	CformProp* subProp = NULL;
	for (size_t ii = 0; ii < (size_t)m_FmCtrl->m_fmTkArry.GetCount(); ii++)
	{
		fmTracker = m_FmCtrl->m_fmTkArry.GetAt(ii);
		curProp = fmTracker->m_ObjLoad->getProp();
		if(curProp->group)
		{
			for (size_t jj = 0; jj < (size_t)m_view->m_mapH->forms.GetCount(); jj++)
			{
				subProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(jj))->getProp();
				m_stdRc.UnionRect(m_stdRc,subProp->rect);
			}
		}
		else
		{
			m_stdRc.UnionRect(m_stdRc,curProp->rect);
		}
	}
}

bool CFmAlignBar::CalcSpace(int iQual)
{
	if(m_FmCtrl->m_fmTkArry.GetCount() <= 1)
		return false;
	AlignUnionRect();
	CRect groupRc;
	groupRc.SetRectEmpty();
	if(iQual == QUAL_H) m_iSpace = m_stdRc.Width();
	else if(iQual == QUAL_V) m_iSpace = m_stdRc.Height();
	CfmTracker* fmTracker = NULL;
	CformProp* curProp = NULL;
	CformProp* subProp = NULL;
	for (size_t ii = 0; ii < (size_t)m_FmCtrl->m_fmTkArry.GetCount(); ii++)
	{
		fmTracker = m_FmCtrl->m_fmTkArry.GetAt(ii);
		curProp = fmTracker->m_ObjLoad->getProp();
		if(curProp->group)
		{
			for (size_t jj = 0; jj < (size_t)m_view->m_mapH->forms.GetCount(); jj++)
			{
				subProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(jj))->getProp();
				//if(iQual == QUAL_H) m_iSpace -= subProp->rect.Width();
				//else if(iQual == QUAL_V) m_iSpace -= subProp->rect.Height();
				if(subProp->group == curProp)
					groupRc.UnionRect(groupRc, subProp->rect);
			}
			if(iQual == QUAL_H) m_iSpace -= groupRc.Width();
			else if(iQual == QUAL_V) m_iSpace -= groupRc.Height();
		}
		else
		{
			if(iQual == QUAL_H) m_iSpace -= curProp->rect.Width();
			else if(iQual == QUAL_V) m_iSpace -= curProp->rect.Height();
		}
	}
	m_iSpace /= m_FmCtrl->m_fmTkArry.GetCount() - 1;
	return true;
}

void CFmAlignBar::QualHSpace()
{
	if(CalcSpace(QUAL_H))
		AlignObject(QUAL_H);
}

void CFmAlignBar::QualVSpace()
{
	if(CalcSpace(QUAL_V))
		AlignObject(QUAL_V);
}

void CFmAlignBar::QualHSize()
{
	m_stdRc.SetRectEmpty();
	AlignObject(QUAL_HSIZE);
}

void CFmAlignBar::QualVSize()
{
	m_stdRc.SetRectEmpty();
	AlignObject(QUAL_VSIZE);
}