// TabDlgCreator.cpp: implementation of the CTabDlgCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabDlgCreator.h"
//#include "TabChart.h"
#include "./define/ChartMsgDef.h"
//#include "TabIndices.h"

//#include "TabCodeSearch.h"
//#include "TabMyFavorite.h"
//#include "TabOrder.h"
#include "StdDialog.h"
#include "TabStockDlg.h"
//#include "TabFutOptDlg.h"
#include "TabUpJongDlg.h"
//#include "TabForeignDlg.h"
#include "TabCodeSearchFO.h"
#include "TabELWDlg.h"
#include "TabForeignDlg.h"

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
		_pTab->m_pCompareChartWnd = m_pCompareChartWnd;\
		_pTab->m_pChartMng = m_pChartMng;\
		_pTab->Create(_class::IDD, m_pTabCtrl);} }


CTabDlgCreator::CTabDlgCreator()
{
	m_pParentDlg		= NULL;
	m_pTabCtrl			= NULL;
	m_pCompareChartWnd	= NULL;
	m_pChartMng			= NULL;
	m_pDBMgr			= NULL;
}

CTabDlgCreator::~CTabDlgCreator()
{

}

void CTabDlgCreator::Init(CWnd* pCompareChartWnd, CLeftBarDlg *pParentDlg,CTabCtrl *pTabCtrl,CWnd *pChartMng, void *pDBMgr)
{
	m_pCompareChartWnd = pCompareChartWnd;
	m_pParentDlg = pParentDlg;
	m_pTabCtrl = pTabCtrl;
	m_pChartMng = pChartMng;
	m_pDBMgr	= pDBMgr;
}

int CTabDlgCreator::GetCntOfTabDlg()
{	
	return COMPARECHART_TABTOTALCNT;
}


void CTabDlgCreator::GetCaptionOfTabDlg(UINT nIndex, char *pszCaption)
{
	if(((CStdDialog*)m_pCompareChartWnd)->m_nScreenType == CStdDialog::Screen_ELWChart)
	{
		strcpy(pszCaption, TAB_ELW);
		return;
	}

	if(((CStdDialog*)m_pCompareChartWnd)->m_nScreenType == CStdDialog::Screen_ForeignChart)
	{
		switch(nIndex)
		{
//	20130515 이상신 : 해외지수 삭제.(issue No - 0006748)
/*		case TABNO_STOCK:
			strcpy(pszCaption,"해외");
			break;
		case TABNO_UPJONG:
			strcpy(pszCaption,TAB_UPJONG);
			break;
*/
		case TABNO_STOCK:
			strcpy(pszCaption,TAB_UPJONG);
			break;
		}
		return;
	}
	switch(nIndex)
	{
//		case TABNO_FAVORITE:
//			strcpy(pszCaption,TAB_FAVORITE);
//			break;
		case TABNO_STOCK:
			strcpy(pszCaption,TAB_STOCK);
			break;
		case TABNO_UPJONG:
			strcpy(pszCaption,TAB_UPJONG);
			break;
		case TABNO_FUTOPT:
			strcpy(pszCaption,TAB_FUTOPT);
			break;
	//	case TABNO_FOREIGN:
	//		strcpy(pszCaption,TAB_FOREIGN);
			break;
	}
}

CTabDlg* CTabDlgCreator::CreateTabDlg(UINT nIndex)
{
	CTabDlg* _pTab = NULL;
	if(((CStdDialog*)m_pCompareChartWnd)->m_nScreenType == CStdDialog::Screen_ELWChart)
	{
		CREATETAB(_pTab,CTabELWDlg)
		return _pTab;
	}

	if(((CStdDialog*)m_pCompareChartWnd)->m_nScreenType == CStdDialog::Screen_ForeignChart)
	{
		switch(nIndex)
		{
//	20130515 이상신 : 해외지수 삭제.(issue No - 0006748)
/*		case TABNO_STOCK	:	
			CREATETAB(_pTab,CTabForeignDlg)		break;
		case TABNO_UPJONG	:	
			CREATETAB(_pTab,CTabUpJongDlg)		break;
*/
		case TABNO_STOCK	:	
			CREATETAB(_pTab,CTabUpJongDlg)		break;
		}
		return _pTab;
	}

	switch(nIndex)
	{
//		case TABNO_FAVORITE	:	
//			CREATETAB(_pTab,CTabFavoriteGrp)	break;
		case TABNO_STOCK	:	
			CREATETAB(_pTab,CTabStockDlg)		break;
		case TABNO_UPJONG	:	
			CREATETAB(_pTab,CTabUpJongDlg)		break;
		case TABNO_FUTOPT	:	
			//CREATETAB(_pTab,CTabFutOptDlg)		break;
			CREATETAB(_pTab,CTabCodeSearchFO)		break;
	//	case TABNO_FOREIGN:	
	//		CREATETAB(_pTab,CTabForeignDlg)		break;	
	}

	return _pTab;
}
