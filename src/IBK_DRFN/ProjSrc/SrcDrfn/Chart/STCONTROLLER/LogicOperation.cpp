// LogicOperation.cpp: implementation of the CLogicOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicOperation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicOperation::CLogicOperation()
{
	m_nOperation = -1;
	m_pLogicItemLeft = NULL;
	m_pLogicItemRight = NULL;
}

CLogicOperation::~CLogicOperation()
{
	if(m_pLogicItemLeft)
	{
		delete m_pLogicItemLeft;
		m_pLogicItemLeft = NULL;
	}
	if(m_pLogicItemRight)
	{
		delete m_pLogicItemRight;
		m_pLogicItemRight = NULL;
	}		
}

void CLogicOperation::SetOperation(int nOperation)
{
	m_nOperation = nOperation;
}

void CLogicOperation::SetItemLeft(CLogicItem* pLogicItemLeft, BOOL bNoError /*= FALSE*/)
{
	if(m_pLogicItemLeft&&!bNoError)
	{
		::AfxMessageBox("SetItemLeft Error!");
	}
	pLogicItemLeft->SetParent(this);
	m_pLogicItemLeft = pLogicItemLeft;
}

void CLogicOperation::SetItemRight(CLogicItem* pLogicItemRight, BOOL bNoError /*= FALSE*/)
{
	if(m_pLogicItemRight&&!bNoError)
	{
		::AfxMessageBox("SetItemRight Error!");
	}
	pLogicItemRight->SetParent(this);
	m_pLogicItemRight = pLogicItemRight;
}

void CLogicOperation::SetIndex(long lIndex)
{
	m_pLogicItemLeft->SetIndex(lIndex);
	m_pLogicItemRight->SetIndex(lIndex);
}

double CLogicOperation::GetValue()
{
	switch(m_nOperation)
	{
	case DISPIID_ADD:// + 
		{
			return (m_pLogicItemLeft->GetValue() + m_pLogicItemRight->GetValue());
		}
		break;
	case DISPIID_SUB:// -
		{
			return (m_pLogicItemLeft->GetValue() - m_pLogicItemRight->GetValue());
		}
		break;
	case DISPIID_MUL:// *
		{
			return (m_pLogicItemLeft->GetValue() * m_pLogicItemRight->GetValue());
		}
		break;
	case DISPIID_DIV:// ³ª´©±â
		{
			return (m_pLogicItemLeft->GetValue() / m_pLogicItemRight->GetValue());
		}
		break;
	}
	return 0;
}