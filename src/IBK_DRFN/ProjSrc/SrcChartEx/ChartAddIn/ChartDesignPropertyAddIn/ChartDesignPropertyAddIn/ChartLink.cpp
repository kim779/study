// ChartLink.cpp : implementation file
//	Effect : ChartLink를 생성, 삭제, 추가, 삽입하는데 관련하는 함수
//	Owner : Myoungsun Kim

#include "stdafx.h"
#include "ChartLink.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "MyTreeCtrl.h"									// for CMyTreeCtrl

/////////////////////////////////////////////////////////////////////////////
// CChartLink

//	Chart Create 
CChartLink::CChartLink(CChartLink *Parent, int Type, LPSTR Name, HWND p_hOcxWnd)
{
	m_iType = Type ;			//	Chart Node Type
	m_strName = Name ;			//	Chart Type Name (선 차트, ....)
	m_hItem = (HTREEITEM)0 ;	// Handle of an item in a tree-view control

	m_Parent = Parent ;
	m_Child = NULL ;
	m_Prev = NULL ;
	m_Next = NULL ;

	 m_bScaleJoin = FALSE;
	 m_nScalePosition = 2;
	 //sy 2006.04.17.
	 m_bSignalUse = false;
	 m_bSignalShow = false;
	 //sy end
	 m_bLowHighUpDown = false;
	 m_bLowNowUpDown = false;
	 m_bHighNowUpDown = false;
	 m_bPreNowContrast = false;
	 m_nCurData = 0;

//Block
	m_strDateScaleCompart = ".";
	m_strTimeScaleCompart = ":";
	m_nHorizonpos = 3;
	m_nHorizonStyle = 0;
	m_nColumnIndex = 0;
    m_nLog = 0;
	m_nScaleCompartType = 1;

	m_nScaleExpressionType = 0;

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	m_bMinMaxLock	= FALSE;
	m_eMinMaxType	= CScaleBaseData::VERT_MINMAX_OF_VIEW;
	m_dMax			= 0;
	m_dMin			= 0;

	// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
	m_nUpSpace		= 0;
	m_nDownSpace	= 0;

	//2007.01.22 Scale사용자 설정,호가단위별
	m_dHogaNumber = 0.0;
	m_dUserNumber = 0.0;
	m_nUserHogaType = 0;

	// 지표 뒤집기 - ojtaso (20071023)
	m_bIsInvert = FALSE;

	// (2008/1/15 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;

	//Block, Item
	//sy 2004.1.28. -> x/y축 scale의 기본값을 넣는다.
	AutoScaleData((CString)Name);
}

CChartLink::~CChartLink()
{
	if(m_Child != NULL)
		m_Child->DeleteAll();
	if(m_Next != NULL)
		m_Next->m_Prev = m_Prev;
	if(m_Prev != NULL)
		m_Prev->m_Next = m_Next;
}

/////////////////////////////////////////////////////////////////////////////
// CChartLink message handlers

//	Delete Tree
BOOL CChartLink::DeleteAll()
{
	if(this == NULL) return FALSE;
	if(m_Next != NULL)
		m_Next->DeleteAll();

	m_Next = NULL;
	if(m_Prev != NULL)
		m_Prev->m_Next = NULL;
	delete this;

	return TRUE;

}

//	Add Chart
BOOL CChartLink::Add(CChartLink * pItem)
{
	if(m_Next == NULL) {
		m_Next = pItem ;
		pItem->m_Prev = this ;
		pItem->m_Parent = m_Parent ;
	}
	else
		m_Next->Add(pItem) ;
	return TRUE ;
}

BOOL CChartLink::AddChild(CChartLink* pItem)
{
	if(m_Child == NULL) {
		m_Child = pItem ;
		pItem->m_Parent = this ;
	}
	else
		m_Child->Add(pItem) ;
	return TRUE ;
}

BOOL CChartLink::Insert(CChartLink * pItem)
{
	if(m_Prev == NULL)
	{
		m_Prev = pItem ;
		pItem->m_Next = this ;
		pItem->m_Parent = m_Parent ;
		if(m_Parent != NULL && m_Parent->m_Child == this)
			m_Parent->m_Child = pItem ;
	}
	else
		m_Prev->Insert(pItem) ;
	return TRUE ;
}

BOOL CChartLink::InsertChild(CChartLink * pItem)
{
	if(m_Child == NULL)
	{
		m_Child = pItem ;
		pItem->m_Parent = this ;
	}
	else
		m_Child->Insert(pItem) ;
	return TRUE ;
}

CChartLink* CChartLink::Find(HTREEITEM hItem)
{
	if(m_hItem == hItem) return this ;
	if(m_Child != NULL)
	{
		CChartLink* pLink = m_Child->Find(hItem) ;
		if(pLink != NULL)
			return pLink ;
	}
	if(m_Next != NULL)
		return m_Next->Find(hItem) ;
	return NULL ;
}

void CChartLink::Cut()
{
	if(m_Parent != NULL && m_Parent->m_Child == this)
		m_Parent->m_Child = m_Next;

	if(m_Prev != NULL)
		m_Prev->m_Next = m_Next;

	if(m_Next != NULL)
		m_Next->m_Prev = m_Prev;

	m_Next = NULL;
	m_Prev = NULL;
	m_Parent = NULL;
}

void CChartLink::Delete()
{
	if(m_Parent != NULL && m_Parent->m_Child == this)
		m_Parent->m_Child = m_Next;

	if(m_Prev != NULL)
		m_Prev->m_Next = m_Next;

	if(m_Next != NULL)
		m_Next->m_Prev = m_Prev;

	m_Next = NULL;
	m_Prev = NULL;
	m_Parent = NULL;
	if(m_Child)	{
		m_Child->DeleteAll() ;
		m_Child = NULL ;
	}
	delete this ;
}

void CChartLink::InsertAfterTarget(CChartLink *pItem)
{
	if(m_Next == NULL) {
		m_Next = pItem;
		pItem->m_Prev = this;
		pItem->m_Parent = m_Parent;
	}
	else {
		this->m_Next->m_Prev = pItem;
		pItem->m_Prev = this;
		pItem->m_Next = this->m_Next;
		this->m_Next = pItem;
		pItem->m_Parent = m_Parent;
	}
}

//sy 2004.1.28. -> x/y축 scale의 기본값을 넣는다.
// "Block(i,j)" -> x축 scale
// "Vertical Scale" -> y축 scale
void CChartLink::AutoScaleData(const CString& strName)
{
	if(strName.Find("Block") != -1)
		m_strScaleStyle = _LTEXT0( C0_DATE_TIME);
	else if(strName.Find("Vertical Scale") != -1)
		m_strScaleStyle = _LTEXT6( C6_INDICATOR_DATA);
	else
		m_strScaleStyle = "";
}
//sy end

void CChartLink::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	AutoScaleData( m_strName);
}
