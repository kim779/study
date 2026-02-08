// PnToolInfo.cpp: implementation of the CPnToolInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PnToolInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPnToolInfo::CPnToolInfo()
{
	m_iBtnState	= 0;
	m_iKind		= 0;
	m_iIndex	= -1;
	m_rect.SetRectEmpty();
	m_strText.Empty();
}

CPnToolInfo::~CPnToolInfo()
{

}

void CPnToolInfo::GetStruck(char* pcBuffer)
{
	struct _pnToolInfo* ppti = (struct _pnToolInfo*)pcBuffer;
	ZeroMemory(ppti, sizeof(struct _pnToolInfo));

	ppti->iBtnState	= m_iBtnState;
	ppti->iKind	= m_iKind;
	ppti->rc	= m_rect;	// ?
	ppti->iIndex	= m_iIndex;
	CopyMemory(ppti->acText, m_strText, min(sizeof(ppti->acText), m_strText.GetLength()));
}

void CPnToolInfo::SetStruck(char* pcBuffer)
{
	struct _pnToolInfo* ppti = (struct _pnToolInfo*)pcBuffer;

	m_iBtnState	= ppti->iBtnState;
	m_iKind		= ppti->iKind;
	m_iIndex	= ppti->iIndex;
	m_rect		= ppti->rc;	// ?
	m_strText	= CString(ppti->acText);
}

void CPnToolInfo::operator =( const CPnToolInfo& src)
{
	this->m_iBtnState	= src.m_iBtnState;
	this->m_iIndex		= src.m_iIndex;
	this->m_iKind		= src.m_iKind;
	this->m_rect		= src.m_rect;
	this->m_strText		= src.m_strText;

}