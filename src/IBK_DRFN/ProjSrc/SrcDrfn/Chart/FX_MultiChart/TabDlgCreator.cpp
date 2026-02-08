// TabDlgCreator.cpp: implementation of the CTabDlgCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabDlgCreator.h"
#include "./define/ChartMsgDef.h"
#include "TabIndices.h"
#include "TabCodeSearch.h"
#include "TabSystemTradingEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define CREATETAB(_pTab,_class) {\
		_pTab = new _class;\
		if(_pTab) {\
		_pTab->m_pParentDlg = m_pParentDlg;\
		_pTab->m_pMultiChartWnd = m_pMultiChartWnd;\
		_pTab->m_pChartMng = m_pChartMng;\
		_pTab->Create(_class::IDD, m_pTabCtrl);} }


CTabDlgCreator::CTabDlgCreator()
{
	m_pParentDlg = NULL;
	m_pTabCtrl = NULL;
	m_pMultiChartWnd = NULL;
	m_pChartMng = NULL;
}

CTabDlgCreator::~CTabDlgCreator()
{

}

void CTabDlgCreator::Init(CWnd* pMultiChartWnd, CLeftBarDlg *pParentDlg,CTabCtrl *pTabCtrl,CWnd *pChartMng)
{
	m_pMultiChartWnd = pMultiChartWnd;
	m_pParentDlg = pParentDlg;
	m_pTabCtrl = pTabCtrl;
	m_pChartMng = pChartMng;
}

int CTabDlgCreator::GetCntOfTabDlg()
{	
	return STOCK_TABTOTALCNT;
}


void CTabDlgCreator::GetCaptionOfTabDlg(UINT nIndex, char *pszCaption)
{
	switch(nIndex)
	{
	case TABNO_ASSISTINDEX:
		strcpy(pszCaption,TAB_ASSISTINDEX);
		break;
	case TABNO_CODESEARCH:
		strcpy(pszCaption,TAB_CODESEARCH);
		break;
	case TABNO_SYSTEMTRADING:
		strcpy(pszCaption,TAB_SYSTEMTRADING);
		break;	
	}
}

CTabDlg* CTabDlgCreator::CreateTabDlg(UINT nIndex)
{
	CTabDlg* _pTab = NULL;

	switch(nIndex)
	{
	case TABNO_ASSISTINDEX:		CREATETAB (_pTab,CTabIndices)				break;
	case TABNO_CODESEARCH:		CREATETAB (_pTab,CTabCodeSearch)			break;
	case TABNO_SYSTEMTRADING:	CREATETAB (_pTab,CTabSystemTradingEx)		break;
	}
	return _pTab;
}
