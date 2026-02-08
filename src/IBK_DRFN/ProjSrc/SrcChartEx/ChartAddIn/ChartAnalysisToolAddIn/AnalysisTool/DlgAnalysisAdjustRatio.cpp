// DlgAnalysisAdjustRatio.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAnalysisAdjustRatio.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int G_ADJUST_RATIO_LIST_COUNT = 11;
static	char *G_ADJUST_RATIO_LIST_NAME_K[G_ADJUST_RATIO_LIST_COUNT] = {"가격1","가격2","가격3","조정 백분율",
																	 "33%","38.2%","40%","50%","60%","61.8%","66%"};
static	char *G_ADJUST_RATIO_LIST_NAME_E[G_ADJUST_RATIO_LIST_COUNT] = {"Price1","Price2","Price3","Modified Percentage",
																	 "33%","38.2%","40%","50%","60%","61.8%","66%"};
/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysisAdjustRatio dialog


CDlgAnalysisAdjustRatio::CDlgAnalysisAdjustRatio(HWND hOcxWnd /*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalysisAdjustRatio::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAnalysisAdjustRatio)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	SetOcxHwnd(hOcxWnd);
	m_dValue1 = 0.0;
	m_dValue2 = 0.0;
	m_dValue3 = 0.0;
}


void CDlgAnalysisAdjustRatio::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnalysisAdjustRatio)
	DDX_Control(pDX, IDC_LIST_ADJUST_RATIO, m_ctrlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnalysisAdjustRatio, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysisAdjustRatio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysisAdjustRatio message handlers

BOOL CDlgAnalysisAdjustRatio::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i = 0;
	LV_COLUMN lvc;
	CRect rect;
	m_ctrlList.GetClientRect( &rect );

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvc.fmt = LVCFMT_LEFT;

	for( i=0;i<2;i++)
	{
		lvc.pszText = "Column";
		lvc.cx = rect.Width()/2;
		lvc.iSubItem = 0;
		m_ctrlList.InsertColumn(i, &lvc);
	}

	m_ctrlList.SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);	

	double dAdjustRatioLists[8] ={0.333333,0.382,0.4,0.5,0.6,0.618,0.666666};
	double dValue = m_dValue2 - (m_dValue2 - m_dValue1);
	char chValue[30];
	for(i=0;i<G_ADJUST_RATIO_LIST_COUNT;i++)
	{
		if(i==0)
			sprintf(chValue,"%.2lf",m_dValue1);
		else if(i==1)
			sprintf(chValue,"%.2lf",m_dValue2);
		else if(i==2)
			sprintf(chValue,"%.2lf",m_dValue3);
		else if(i==3)
		{
			if(m_dValue2-m_dValue1 != 0.0)
				sprintf(chValue,"%.2lf",(m_dValue2-m_dValue3)/(m_dValue2-m_dValue1)*100.0);
			else
				strcpy(chValue,"0.0");
		}
		else
		{
			sprintf(chValue,"%.2lf",dValue*dAdjustRatioLists[i-4]);			
		}
		SetListItem(i,chValue);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAnalysisAdjustRatio::SetListItem(int nPos,char *chData)
{
	LV_ITEM	item;
	item.iItem = nPos;
	item.mask = LVIF_TEXT; 
	item.iSubItem = 0;

	IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd);
	switch(eLanguageID)
	{
	case IMetaTable::KOREAN:
		{
			item.pszText = G_ADJUST_RATIO_LIST_NAME_K[nPos];
			m_ctrlList.InsertItem(&item);
			m_ctrlList.SetItemText(nPos,0,G_ADJUST_RATIO_LIST_NAME_K[nPos]);	
		}
		break;
	case IMetaTable::ENGLISH:
		{
			item.pszText = G_ADJUST_RATIO_LIST_NAME_E[nPos];
			m_ctrlList.InsertItem(&item);
			m_ctrlList.SetItemText(nPos,0,G_ADJUST_RATIO_LIST_NAME_E[nPos]);
		}
		break;
	default:
		ASSERT(FALSE);
	};

	m_ctrlList.SetItemText(nPos,1,chData);	
}

void CDlgAnalysisAdjustRatio::SetValue(int nIndex,double dValue)
{
	if(nIndex == 0)
	{
		m_dValue1 = dValue;
	}
	else if(nIndex == 1)
	{
		m_dValue2 = dValue;
	}
	else if(nIndex == 2)
	{
		m_dValue3 = dValue;
	}
}