// PnAidInfo.cpp: implementation of the CPnAidInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PnAidInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPnAidInfo::CPnAidInfo()
{
	m_iKind	= -1;
}	

CPnAidInfo::~CPnAidInfo()
{

}


void CPnAidInfo::GetStruck(char* pcBuffer)
{
	struct _pnAidInfo* ppai = (struct _pnAidInfo*)pcBuffer;
	ZeroMemory(ppai, sizeof(struct _pnAidInfo));

	ppai->iKind	= m_iKind;
	ppai->rc	= m_rect;
	CopyMemory(ppai->acText, m_strText.operator LPCTSTR(), min(sizeof(ppai->acText), m_strText.GetLength()));
	CopyMemory(ppai->acName, m_strName.operator LPCTSTR(), min(sizeof(ppai->acName), m_strName.GetLength()));
}

void CPnAidInfo::SetStruck(char* pcBuffer)
{
	struct _pnAidInfo* ppai = (struct _pnAidInfo*)pcBuffer;

	m_iKind		= ppai->iKind;
	m_rect		= ppai->rc;
	m_strText	= CString(ppai->acText);
	m_strName	= CString(ppai->acName);
}

void CPnAidInfo::operator =( const CPnAidInfo& src)
{
	this->m_iKind		= src.m_iKind;
	this->m_rect		= src.m_rect;
	this->m_strText		= src.m_strText;
	this->m_strName		= src.m_strName;

}