// NewsObj.cpp: implementation of the CNewsObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartNewsAddInImp.h"
#include "NewsObj.h"
#include "resource.h"
//#include "CISBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// NewsObj 관련
#define	TYPE_ESM					0
#define	TYPE_IDS1					1000
#define	TYPE_IDS2					1001
#define TYPE_FW1					1200
#define TYPE_FW2					1202
#define TYPE_FW3					1203

// IDS status
#define ENGINE_STOP 0x01
#define ENGINE_RUN  0x02
#define ENGINE_PAUSE 0x03

// FW Status
#define FW_STOP		0x00
#define FW_RUN		0x01
IMPLEMENT_SERIAL(CNewsObj, CObject, 0);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CNewsObj::CNewsObj()
:m_rc(0,0,WIDTH,HEIGHT)
, m_bSelected(FALSE)
{
	m_ptAt.x = m_ptAt.y = 0;
	m_bDrag = FALSE;
	m_pdcMem = NULL;
}

CNewsObj::~CNewsObj()
{
	if(m_pdcMem)
	{
		m_pdcMem->DeleteDC();
		m_dcTransMem.DeleteDC();
		delete m_pdcMem;
		m_pdcMem = NULL;
	}
}

BOOL CNewsObj::Initialize()
{	
	if(m_pdcMem) return FALSE;	// 이미 초기화를 했으면
	m_pdcMem = new CDC;
	m_size.cx = WIDTH;
	m_size.cy = HEIGHT;	
	
	m_bitmap.LoadBitmap(IDB_NEWS_FOLDER);		
	CClientDC dc(NULL);
	m_pdcMem->CreateCompatibleDC(&dc);
	m_dcTransMem.CreateCompatibleDC(&dc);

	m_pOldBmp = m_pdcMem->SelectObject(&m_bitmap);	
	// Select the mask bitmap into the appropriate dc	
	m_dcTransMem.BitBlt(0, 0, WIDTH, HEIGHT, m_pdcMem, 0, 0, SRCCOPY);

	m_bitmap.DeleteObject();
	
	return TRUE;
}

void CNewsObj::Draw(CDC *pDC)
{
	pDC->BitBlt(m_ptAt.x,m_ptAt.y,m_size.cx,m_size.cy,m_pdcMem,0,0,SRCCOPY);
// 	pDC->BitBlt(m_ptAt.x,m_ptAt.y,m_size.cx,m_size.cy,m_pdcMem,0,0,SRCINVERT);
//	pDC->BitBlt(m_ptAt.x,m_ptAt.y,m_size.cx,m_size.cy,&m_dcTransMem,0,0,SRCAND);
//	pDC->BitBlt(m_ptAt.x,m_ptAt.y,m_size.cx,m_size.cy,m_pdcMem,0,0,SRCINVERT);
}


void CNewsObj::Erase(CDC *pDC)
{
}

void CNewsObj::ButtonDownAt(const CPoint &pt)
{
	m_bDrag = TRUE;
	m_ptOffset = (CPoint) (pt - m_ptAt);
}

void CNewsObj::SetPos(const CPoint &ptNew)
{
	if(m_bDrag)
	{
		m_ptAt = (CPoint) (ptNew - m_ptOffset);
	}
	else
		m_ptAt = ptNew;
}

BOOL CNewsObj::IsInRegion(CPoint point)
{


	if( m_ptAt.x < point.x && m_ptAt.x+WIDTH > point.x && m_ptAt.y < point.y && m_ptAt.y+HEIGHT > point.y)
	//if(m_rc.PtInRect(point))
		return TRUE;
	else
		return FALSE;
}

//*************************************************
// CNewsObjList
//
void CNewsObjList::DeleteAll()
{
	CNewsObj* tmpNewsObj;
	POSITION pos = GetHeadPosition();

	while(pos != NULL)
	{
		tmpNewsObj = GetNextNewsObj(pos);
		//delete tmpNewsObj->m_NewsObj.name;
		//delete tmpNewsObj->m_NewsObj.Parentname;
		//delete tmpNewsObj->m_NewsObj.nickname;
		delete tmpNewsObj;
	}
}

int CNewsObjList::GetTotalNewsObj()
{
	return GetCount();
}

int CNewsObjList::GetNewsObjCount(DWORD type)
{
	int cnt=0;
	CNewsObj* pNewsObj;

	POSITION pos = GetHeadPosition();
	while(pos)
	{
		pNewsObj = GetNextNewsObj(pos);
		//if(pNewsObj->m_NewsObj.Type == type)
		//	cnt++;
	}
	return cnt;

}

CNewsObj* CNewsObjList::FindNewsObj(DWORD type)
{
	CNewsObj* pNewsObj;

	POSITION pos = GetHeadPosition();
	while(pos)
	{
		pNewsObj = GetNextNewsObj(pos);
		//if(pNewsObj->m_NewsObj.Type == type)
		//	return pNewsObj;
	}
	return NULL;
}

CNewsObj* CNewsObjList::GetNextServer(POSITION pos)
{
	CNewsObj* pNewsObj;

	while(pos)
	{
		pNewsObj = GetNextNewsObj(pos);
		//if(pNewsObj->m_NewsObj.Type == TYPE_ESM)
		//	return pNewsObj;
	}
	return NULL;
}


void CNewsObj::SetDate(long lDate/*IN*/)
{
	int a, b, c;	// 시간 데이터를 분리
	m_lxData = lDate;

	// 시작일
	a = lDate/10000;
	lDate = lDate%10000;
	b = lDate/100;
	lDate = lDate%100;
	c = lDate;

	m_strDate.Format("%d/%d/%d", a,b,c);
}
