// ChartCtrl.cpp: implementation of the CChartCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartLanguageTable.h"
#include "ChartCtrl.h"

#include "CodePage.h"					// for CCodePage
#include "MetaTable.h"					// for CMetaTable

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartCtrl::CChartCtrl( HWND p_hOcxWnd, IMetaTable::E_LANGUAGEID p_eLanguageID)
{
	m_hOcxWnd				= p_hOcxWnd;
	m_eLanguageID			= p_eLanguageID;
	m_pLocalCodePage		= NULL;
	m_bDesignMode			= FALSE;
}

CChartCtrl::~CChartCtrl()
{
	if( m_pLocalCodePage)
	{
		delete m_pLocalCodePage;
		m_pLocalCodePage = NULL;
	}
}

void CChartCtrl::CreateLocalLanguageTable( const char *p_szLocalLanguageTable, BOOL p_bDesignMode)
{
	if( m_pLocalCodePage)
	{
		delete m_pLocalCodePage;
		m_pLocalCodePage = NULL;
	}
	m_bDesignMode = p_bDesignMode;
	m_pLocalCodePage = new CCodePage( -1);

	if( !p_szLocalLanguageTable) return;
	if( !*p_szLocalLanguageTable) return;

	size_t nTableLen = strlen( p_szLocalLanguageTable);
	char *szLocalLanguageTable = m_strLocalLanguageTable.GetBufferSetLength( nTableLen);
	strcpy( szLocalLanguageTable, p_szLocalLanguageTable);
	m_pLocalCodePage->LoadMetaTable( szLocalLanguageTable);
}

const char *CChartCtrl::GetLocalLanguageTable(	void)
{
	m_strLocalLanguageTableRetrieveBuffer.Empty();
	if( m_pLocalCodePage) m_pLocalCodePage->GetMetaTableString( m_strLocalLanguageTableRetrieveBuffer);
	return m_strLocalLanguageTableRetrieveBuffer;
}

const char *CChartCtrl::GetLanguageText( IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szMetaText)
{
	const char *szLanguageText = NULL;
	if( IMetaTable::PACKET_NAME == p_eCodePage || IMetaTable::GRAPH_TITLE == p_eCodePage)
		if( m_pLocalCodePage) szLanguageText = m_pLocalCodePage->GetLanguageText( m_eLanguageID, p_szMetaText);
	
	if( szLanguageText) return szLanguageText;

	CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
	
	if( !pCodePage) return p_szMetaText;

	return pCodePage->GetLanguageText( m_eLanguageID, p_szMetaText);
}

// *. Convert LText to MText
//		Caution! p_szLanguageText can be MText already.
//					or p_szLanguageText can be created already.
const char *CChartCtrl::CreateLocalMetaText( const char *p_szLanguageText)
{
	if( !m_pLocalCodePage) return p_szLanguageText;

	const char *szMetaText = m_pLocalCodePage->GetMetaText( m_eLanguageID, p_szLanguageText);
	if( szMetaText) return szMetaText;
	if( !m_bDesignMode) return p_szLanguageText;

	return m_pLocalCodePage->CreateMetaText( m_eLanguageID, p_szLanguageText);
}

// *. Convert LText to MText
//		Caution! p_szMetaText can be created already.
BOOL CChartCtrl::SetLocalMetaText( const char *p_szMetaText, const char *p_szLanguageText)
{
	if( !m_pLocalCodePage || !m_bDesignMode) return FALSE;
	return m_pLocalCodePage->SetLocalMetaText( p_szMetaText, m_eLanguageID, p_szLanguageText);
}

// *. Convert LText to MText
//		Caution! p_szLanguageText can be MText already.
const char *CChartCtrl::GetMetaText( IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szLanguageText)
{

	const char *szMetaText = NULL;
	if( IMetaTable::PACKET_NAME == p_eCodePage || IMetaTable::GRAPH_TITLE == p_eCodePage)
		if( m_pLocalCodePage) szMetaText = m_pLocalCodePage->GetMetaText( m_eLanguageID, p_szLanguageText);
	
	if( szMetaText) return szMetaText;

	CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
	if( !pCodePage)
	{
		return p_szLanguageText;
	}
	return pCodePage->GetMetaText( m_eLanguageID, p_szLanguageText);
}

// *. Convert LText to MText
//		Caution! p_szOtherMOrLText can be MText already.
const char *CChartCtrl::GetMetaTextForScript( IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szOtherMOrLText)
{
	const char *szMetaText = NULL;
	if( IMetaTable::PACKET_NAME == p_eCodePage || IMetaTable::GRAPH_TITLE == p_eCodePage)
		if( m_pLocalCodePage) szMetaText = m_pLocalCodePage->GetMetaTextForScript( m_eLanguageID, p_szOtherMOrLText);
	
	if( !szMetaText)
	{
		CCodePage *pCodePage = g_metaTable.GetCodePage( p_eCodePage);
		if( pCodePage) szMetaText = pCodePage->GetMetaTextForScript( m_eLanguageID, p_szOtherMOrLText);
	}

	return szMetaText;
}