// MemberImgList.cpp: implementation of the CMemberImgList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExButton.h"
#include "MemberImgList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberImgList::CMemberImgList(LPVOID lpControl) : CMember(lpControl)
{
	m_pButton = (CButton_Friendly*)lpControl;
	m_pImageList = NULL;
	m_nNormal    = -1;
	m_nDown		 = -1;
	m_nDisable	 = -1;
	m_nOver		 = -1;
	m_bButtonDown= FALSE;
	m_bCheck	 = FALSE;
	m_bLbuttonDown= FALSE;
	m_lpszText = NULL;
}

CMemberImgList::~CMemberImgList()
{
	if(m_lpszText)
	{
		::free(m_lpszText);
		m_lpszText = NULL;
	}
}

void CMemberImgList::SetMemeberImgList(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText, COLORREF clrText, COLORREF clrTextDis)
{
	if(m_lpszText) {
		::free(m_lpszText);
		m_lpszText = NULL;
	}

	if(lpszText)
		m_lpszText = ::strdup(lpszText);

	m_pImageList = pImageList;

	m_nNormal = nNormal;
	m_nDown = nDown;
	m_nDisable = nDisable;
	m_nOver = nOver;

	m_clrText = clrText;
	m_clrTextDis = clrTextDis;
}

void CMemberImgList::SetText(LPCTSTR lpszText)
{
	if(m_lpszText)
		::free(m_lpszText);

	m_lpszText = ::strdup(lpszText);
}

LPCTSTR CMemberImgList::GetText()
{
	if(m_lpszText)
		return m_lpszText;

	return _T("");
}
