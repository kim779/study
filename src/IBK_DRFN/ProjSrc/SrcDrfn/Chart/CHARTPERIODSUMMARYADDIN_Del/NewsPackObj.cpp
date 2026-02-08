// NewsPackObj.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ChartPeriodSummaryAddInImp.h"
#include "resource.h"
#include "NewsPackObj.h"

// CNewsPackList
void CNewsPackList::DeleteAll()
{
	CNewsPackObj* tmpNewsPack;
	POSITION pos = GetHeadPosition();

	while(pos != NULL)
	{
		tmpNewsPack = GetNextNewsPack(pos);
		delete tmpNewsPack;
	}
}

// CNewsPackObj

CNewsPackObj::CNewsPackObj()
{
	m_ptStart = 0;
	m_ptEnd = 0;
	m_size = 0;
	m_bSelected = FALSE;
}

CNewsPackObj::~CNewsPackObj()
{
}


// CNewsPackObj 멤버 함수

void CNewsPackObj::Initilize(void)
{
	CClientDC dc(NULL);
	m_dcMem.CreateCompatibleDC(&dc);
	m_bitmap.CreateCompatibleBitmap(&dc,26,26);
	m_bitmap.LoadBitmap(IDB_NEWS_FOLDER);
	m_pOldBmp = m_dcMem.SelectObject(&m_bitmap);
//	m_dcTransMem.CreateCompatibleDC(&dc);
}

BOOL CNewsPackObj::IsInRegion(CPoint pt)
{
	if((m_ptStart.x - pt.x)/(m_ptStart.y - pt.y) == (pt.x - m_ptEnd.x) /(pt.y - m_ptEnd.y) 
	&& (pt.x >= m_rc.left && pt.x <= m_rc.right && pt.y >= m_rc.top && pt.y <= m_rc.bottom ) )
	{
		m_CurClickedPosition = M_POINT;
		return TRUE;
	}
	return FALSE;
}

void CNewsPackObj::Draw(CDC* pDC)
{
	pDC->BitBlt(m_rc.left, m_rc.top, RC_WIDTH, RC_HEIGHT, &m_dcMem, 0, 0,SRCINVERT);
	pDC->BitBlt(m_rc.left, m_rc.top, RC_WIDTH, RC_HEIGHT, &m_dcTransMem, 0, 0,SRCCOPY);
	pDC->BitBlt(m_rc.left, m_rc.top, RC_WIDTH, RC_HEIGHT, &m_dcMem, 0, 0,SRCINVERT);
}


void CNewsPackObj::SetPos(CPoint pt, int type)
{
	switch(type)
	{
	case S_POINT:
		m_ptStart = pt;
		break;
	case E_POINT:
		m_ptEnd = pt;
		break;
	case M_POINT:		
		m_ptStart += pt - m_ptAt;
		m_ptEnd += pt - m_ptAt;
		m_ptAt = pt;
		break;
	}
}

CPoint CNewsPackObj::GetPos(int type)
{
	switch(type)
	{
	case S_POINT:
		return m_ptStart;
		break;
	case E_POINT:
		return m_ptEnd;
		break;
	default:
		return m_ptAt;
	}
}

void CNewsPackObj::ButtonDownAt(const CPoint &pt)
{	
	switch(m_CurClickedPosition)
	{
	case S_POINT:
		//m_ptStart = pt;
		break;
	case E_POINT:
		//m_ptEnd = pt;
		break;
	case M_POINT:
		m_ptAt = pt;
		m_ptStart += pt - m_ptAt;
		m_ptEnd += pt - m_ptAt;
		break;
	default:
		m_ptStart = pt;
	}
}

void CNewsPackObj::ButtonUpAt(const CPoint &pt)
{	
	switch(m_CurClickedPosition)
	{
	case S_POINT:
		m_ptStart = pt;
		break;
	case E_POINT:
		m_ptEnd = pt;
		break;
	case M_POINT:
		m_ptStart += pt - m_ptAt;
		m_ptEnd += pt - m_ptAt;
		m_ptAt = pt;
		break;
	default:
		m_ptEnd = pt;
	}
}


void CNewsPackObj::Erase(CDC* pDC)
{
	pDC->BitBlt(m_rc.left,m_rc.top,m_rc.Width(),m_rc.Height(),&m_dcMem,0,0,SRCINVERT);
//	TRACE2("Erase : %d, %d",m_rc.left,m_oldrc.left);

}
