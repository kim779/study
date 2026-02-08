// SetAidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgdlg.h"
#include "SetAidDlg.h"
//#include "../Common/MyTool.h"
#include "exGrid.h"
#include "../../h/axisgobj.h"
#include "../../h/axisvar.h"
#include "../gCom/PnAidInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_GRID_USED	11

#define GRID_HEIGHT	256
#define GRID_WIDTH	189


#define DEFAULT_ROWS	1 //(Only Heady)	//48 + 1
#define DEFAULT_COLS	2

#define COLOR_DATA_FG		RGB(0, 0, 0)
#define COLOR_DATA_BG		RGB(250, 250, 250)
#define COLOR_HEAD_FG		RGB(0, 0, 0)
#define COLOR_HEAD_BG		RGB(240, 240, 240)
#define COLOR_GRID_BG		RGB(255, 255, 255)
#define COLOR_GRID_SELECT_BG	RGB(211, 211, 211)
#define COLOR_GRID_LINE		RGB(230, 230, 230)


#define ROW_HEIGHT	28	//23


#define CI_ICON		0
#define CI_TEXT		1

/////////////////////////////////////////////////////////////////////////////
// CSetAidDlg dialog


//CSetAidDlg::CSetAidDlg(class CMyTool* pMyTool, char* pcInfo, CWnd* pParent /*=NULL*/)
CSetAidDlg::CSetAidDlg(char* pcInfo, CWnd* pwndView, CWnd* pParent /*=NULL*/)
	: CDialog(CSetAidDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetAidDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


	m_pwndView	= pwndView;

//	m_pMyTool	= pMyTool;
	m_pApp		= (CAxisGDlgApp*)AfxGetApp();

	m_pUsedGrid	= NULL;


	m_paidConfig = (struct _aidConfig*)pcInfo;
	m_pPnAidInfo = (struct _pnAidInfo*)m_paidConfig->pcBuf;
	m_iPnAidInfoCount = m_paidConfig->iCount;
}


void CSetAidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetAidDlg)
	DDX_Control(pDX, IDC_TREE_INDICATOR, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetAidDlg, CDialog)
	//{{AFX_MSG_MAP(CSetAidDlg)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_INDICATOR, OnItemexpandedTreeIndicator)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_INDICATOR, OnSelchangedTreeIndicator)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL, OnButtonDelAll)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_INDICATOR, OnDblclkTreeIndicator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetAidDlg message handlers

BOOL CSetAidDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitBasic();
	InitTree();
	InitUsedGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetAidDlg::InitTree()
{
struct _item
{
	char	name[32];
	int	kind;
};

struct _item	item[] = {
	{"가격 이평", GK_PMA},	
	{"그물차트", GK_NET}, 
	{"A/D Line",	GK_ADLINE},	
	{"ADX",	GK_ADX},	
	{"CCI",	GK_CCI},	
	{"DMI",	GK_DMI},
	{"Demark", GK_DEMARK},	
	{"MACD", GK_MACD},	
	{"MACD Oscillator", GK_MACDOS}, 
	{"Pivot Line", GK_PIVOT}, 
	{"Price Oscillator", GK_PMAO},	
	{"RSI", GK_RSI},
	{"RSI(가중치)", GK_RSIWEIGHT},	// 20090928 : ADD : RSI 지표 추가
	{"TRIX", GK_TRIX}, 
	{"William's %R", GK_WPR},
	{"Parabolic", GK_PARB},

	{"이격도", GK_GAP},	
	{"모멘텀", GK_MOMENTUM}, 
	{"ABRatio", GK_ABRATIO},
	{"Band Width", GK_BANDWIDTH},
	{"Chaikin's Oscillator", GK_CO}, 
	{"Chaikin's Volatility", GK_CV}, 
	{"Ease of Movement", GK_EMV},	
	{"LFI", GK_LFI},
	{"Net Change Oscillator", GK_NCO},
	{"Price Rate-of-Change", GK_PROC},
	{"Sigma", GK_SIGMA}, 
	{"SONAR", GK_SONA},	
	{"SONA Momentum", GK_SONAMOMENTUM}, 
	{"Slow Stochastics", GK_STOCHS},
	{"Fast Stochastics", GK_STOCHF},

	{"거래량", GK_VOL},	
	{"거래량 이평", GK_VMA},	
	{"회전율", GK_RRATE},
	{"거래대금", GK_AMT}, 
	{"거래대금 이평", GK_AMA},		
	{"ADR", GK_ADR},
	{"Money Flow Index", GK_MFI},	
	{"OBV",	GK_OBV},	
	{"Volume Oscillator", GK_VMAO},	
	{"VR", GK_VR},
	{"Volume Rate-of-Change", GK_VROC},

	{"심리도", GK_PSY},	
	{"신심리도", GK_NPSY},		
	
	{"Bollinger Band", GK_BOLB}, 
	{"Envelope", GK_ENVL},

	{"외국인보유(수량)", GK_FRGNHAVE}, 
	{"외국인보유(비율)", GK_FRGNRATE},
	{"개인순매수(수량)", GK_PSNLPBUY}, 
	{"외국인순매수(수량)", GK_FRGNPBUY}, 
	{"기관순매수(수량)", GK_ORGNPBUY},
	{"개인누적순매수(수량)", GK_PSNLTBUY}, 
	{"외국인누적순매수(수량)", GK_FRGNTBUY}, 
	{"기관누적순매수(수량)", GK_ORGNTBUY},
};

	m_tree.DeleteAllItems();

	struct _item	item1[] = 
	{
		{"가격 이평", GK_PMA},	
		{"그물차트", GK_NET}, 
		{"A/D Line",	GK_ADLINE},	
		{"ADX",	GK_ADX},	
		{"CCI",	GK_CCI},	
		{"DMI",	GK_DMI},
		{"Demark", GK_DEMARK},	
		{"MACD", GK_MACD},	
		{"MACD Oscillator", GK_MACDOS}, 
		{"Pivot Line", GK_PIVOT}, 
		{"Price Oscillator", GK_PMAO},	
		{"RSI", GK_RSI},
		{"RSI(가중치)", GK_RSIWEIGHT},	// 20090928 : ADD : RSI 지표 추가
		{"TRIX", GK_TRIX}, 
		{"William's %R", GK_WPR},
		{"Parabolic", GK_PARB}
	};
	struct _item	item2[] = 
	{
		{"이격도", GK_GAP},	
		{"모멘텀", GK_MOMENTUM}, 
		{"ABRatio", GK_ABRATIO},
		{"Band Width", GK_BANDWIDTH},
		{"Chaikin's Oscillator", GK_CO}, 
		{"Chaikin's Volatility", GK_CV}, 
		{"Ease of Movement", GK_EMV},	
		{"LFI", GK_LFI},
		{"Net Change Oscillator", GK_NCO},
		{"Price Rate-of-Change", GK_PROC},
		{"Sigma", GK_SIGMA}, 
		{"SONAR", GK_SONA},	
		{"SONA Momentum", GK_SONAMOMENTUM}, 
		{"Slow Stochastics", GK_STOCHS},
		{"Fast Stochastics", GK_STOCHF}
	};
	struct _item	item3[] = 
	{
		{"거래량", GK_VOL},	
		{"거래량 이평", GK_VMA},	
		{"회전율", GK_RRATE},
		{"거래대금", GK_AMT}, 
		{"거래대금 이평", GK_AMA},		
		{"ADR", GK_ADR},
		{"Money Flow Index", GK_MFI},	
		{"OBV",	GK_OBV},	
		{"Volume Oscillator", GK_VMAO},	
		{"VR", GK_VR},
		{"Volume Rate-of-Change", GK_VROC}
	};
	struct _item	item4[] = 
	{
		{"심리도", GK_PSY},
		{"신심리도", GK_NPSY}		
	};
	struct _item	item5[] = 
	{
		{"Bollinger Band", GK_BOLB}, 
		{"Envelope", GK_ENVL}
	};

	struct _item item6[] = {
		{"외국인보유(수량)", GK_FRGNHAVE}, 
		{"외국인보유(비율)", GK_FRGNRATE},
		{"개인순매수(수량)", GK_PSNLPBUY}, 
		{"외국인순매수(수량)", GK_FRGNPBUY}, 
		{"기관순매수(수량)", GK_ORGNPBUY},
		{"개인누적순매수(수량)", GK_PSNLTBUY}, 
		{"외국인누적순매수(수량)", GK_FRGNTBUY}, 
		{"기관누적순매수(수량)", GK_ORGNTBUY}
	};

	int ii = 0;
	HTREEITEM htiChild;
	HTREEITEM hti1 = m_tree.InsertItem(_T("추세지표"));
	for (ii = 0; ii < sizeof(item1)/sizeof(_item); ii++)
	{
		htiChild = m_tree.InsertItem(item1[ii].name, hti1);
		m_tree.SetItemData(htiChild, item1[ii].kind);
	}

	HTREEITEM hti2 = m_tree.InsertItem(_T("모멘텀지표"));
	for (ii = 0; ii < sizeof(item2)/sizeof(_item); ii++)
	{
		htiChild = m_tree.InsertItem(item2[ii].name, hti2);
		m_tree.SetItemData(htiChild, item2[ii].kind);
	}

	HTREEITEM hti3 = m_tree.InsertItem(_T("거래량지표"));
	for (ii = 0; ii < sizeof(item3)/sizeof(_item); ii++)
	{
		htiChild = m_tree.InsertItem(item3[ii].name, hti3);
		m_tree.SetItemData(htiChild, item3[ii].kind);
	}

	HTREEITEM hti4 = m_tree.InsertItem(_T("시장강도지표"));
	for (ii = 0; ii < sizeof(item4)/sizeof(_item); ii++)
	{
		htiChild = m_tree.InsertItem(item4[ii].name, hti4);
		m_tree.SetItemData(htiChild, item4[ii].kind);
	}

	HTREEITEM hti5 = m_tree.InsertItem(_T("채널지표"));
	for (ii = 0; ii < sizeof(item5)/sizeof(_item); ii++)
	{
		htiChild = m_tree.InsertItem(item5[ii].name, hti5);
		m_tree.SetItemData(htiChild, item5[ii].kind);
	}

	HTREEITEM hti6 = m_tree.InsertItem(_T("기타지표"));
	for (ii = 0; ii < sizeof(item6)/sizeof(_item); ii++)
	{
		htiChild = m_tree.InsertItem(item6[ii].name, hti6);
		m_tree.SetItemData(htiChild, item6[ii].kind);
	}

	m_tree.Expand(hti1, TVE_EXPAND);
}

void CSetAidDlg::OnItemexpandedTreeIndicator(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (pNMTreeView->action == 2)
		m_tree.SetItemImage(pNMTreeView->itemNew.hItem, 4, 4);
	else if (pNMTreeView->action == 1)
		m_tree.SetItemImage(pNMTreeView->itemNew.hItem, 5, 5);

	
	*pResult = 0;
}

void CSetAidDlg::OnSelchangedTreeIndicator(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CSetAidDlg::OnDestroy() 
{
	if (m_pUsedGrid)
	{
		m_pUsedGrid->DestroyWindow();
		delete m_pUsedGrid;
		m_pUsedGrid = NULL;
	}

	RemoveBasic();

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CSetAidDlg::InitUsedGrid()
{
	CFont* pfont = m_pApp->GetFont(m_pwndView, 9, "굴림", FALSE, 0);
	pfont->GetLogFont(&m_logfont);
	m_logfont.lfHeight = 90;

	pfont = m_pApp->GetFont(m_pwndView, 7, "굴림", FALSE, 0);
	pfont->GetLogFont(&m_logfontS);
	m_logfontS.lfHeight = 70;

	m_pUsedGrid = new CExGrid();
	m_pUsedGrid->Create(CRect(250, 10, 250 + GRID_WIDTH, 10 + GRID_HEIGHT), 
		this, ID_GRID_USED, LOWORD(MAKELONG(GVSC_BOTH, GVSC_NONE)), HIWORD(MAKELONG(GVSC_BOTH, GVSC_NONE)), 
		WS_CHILD | WS_VISIBLE | WS_BORDER);
	m_pUsedGrid->Initial(DEFAULT_ROWS, DEFAULT_COLS, 1, 0);
	m_pUsedGrid->SetGridFocusLine(FALSE, TRUE);

	int	aiColumnWidth[]	={ 30,140,};
	char	aacColumnName[][7]={	"지표",	"이름",	};

	GVITEM	gvitem;
	int	ii;

	// Data SetItem
	for (ii = 0; ii < DEFAULT_ROWS; ii++)
	{
		gvitem.attr	= GVAT_ONLYROW;
		gvitem.state	= 0;
		gvitem.mask	= GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_ATTR;

		gvitem.fgcol	= COLOR_DATA_FG;
		gvitem.bkcol	= COLOR_DATA_BG;
		gvitem.row	= ii;


		CopyMemory(&gvitem.font, &m_logfont, sizeof(LOGFONT));

		for (int jj = 0; jj < DEFAULT_COLS; jj++)
		{
			if (ii == 0)	// Head
			{
				gvitem.format = GVFM_CENTER;
				gvitem.fgcol = COLOR_HEAD_FG;
				gvitem.text = aacColumnName[jj];
				gvitem.bkcol = COLOR_HEAD_BG;
				gvitem.attr = GVAT_3DHEAD|GVAT_ONLYROW;
			}
			else
			{
				gvitem.attr = GVAT_ONLYROW;
				gvitem.format = GVFM_RIGHT;
				gvitem.bkcol = COLOR_DATA_BG;
				gvitem.text = "";
			}

			gvitem.col = jj;
			m_pUsedGrid->SetItem(&gvitem);
			m_pUsedGrid->SetColumnWidth(jj, aiColumnWidth[jj]);
		}
	}


//	// header
//	m_pUsedGrid->SetRowHeight(0, ROW_HEIGHT + 2);
	for (ii = 0; ii < DEFAULT_ROWS; ii++)
	{
		m_pUsedGrid->SetRowHeight(ii, ROW_HEIGHT);
	}

	// 라인색깔
	m_pUsedGrid->SetGridColor(COLOR_GRID_LINE);
	// 라인유무
	m_pUsedGrid->SetGridLine(GVLN_BOTH);
	// 라인스타일
	m_pUsedGrid->SetGridLineType(0);
	// 그리드 배경색
	m_pUsedGrid->SetGridBkColor(COLOR_GRID_BG);
	// 그리드 선택색상
	m_pUsedGrid->SetBKSelColor(COLOR_GRID_SELECT_BG);
	// Column size 변경금지
	m_pUsedGrid->SetRowColResize(FALSE, TRUE);

	m_pUsedGrid->SetTitleTips(TRUE);
	m_pUsedGrid->Adjust();

//	m_pUsedGrid->SetImageList(m_pImgLst);
	m_pUsedGrid->SetRowColResize(FALSE, FALSE);

	
	AddUsedGrid();
}

void CSetAidDlg::OnButtonUp() 
{
	int iSelRow = m_pUsedGrid->GetSelectRow();
	if (iSelRow < 2 || iSelRow >= m_pUsedGrid->GetRowCount())
		return;

	ChangeRow(iSelRow - 1, iSelRow);
	m_pUsedGrid->SetSelectRow(iSelRow - 1);
	
}

void CSetAidDlg::OnButtonDown() 
{
	int iSelRow = m_pUsedGrid->GetSelectRow();
	if (iSelRow < 1 || iSelRow >= m_pUsedGrid->GetRowCount() - 1)
		return;

	ChangeRow(iSelRow, iSelRow + 1);
	m_pUsedGrid->SetSelectRow(iSelRow + 1);
	
}

void CSetAidDlg::OnButtonAdd() 
{
	HTREEITEM hti = m_tree.GetSelectedItem( );
	if (!hti)
		return;

	int iKind = m_tree.GetItemData(hti);

	bool bFind = false;
	for (int ii = 1; ii < m_pUsedGrid->GetRowCount(); ii++)
	{
		if (iKind == m_pUsedGrid->GetItemParam(ii, 1))
		{
			bFind = true;
			break;
		}
	}

	if (bFind)
		return;

	CPnAidInfo* pPAI;
	if (!m_mapPnAidInfo.Lookup(iKind, pPAI))
		return;

	AddNewLine();
	int iRow = GetLastIndex();
	if (iRow < 1)
		return;


	
	AddCell(m_pUsedGrid, iRow, 0, pPAI->m_strText, iKind);
	AddCell(m_pUsedGrid, iRow, 1, pPAI->m_strName, iKind);
	m_pUsedGrid->Refresh();
	
}

void CSetAidDlg::OnButtonDel() 
{
	int iSelRow = m_pUsedGrid->GetSelectRow();
	if (iSelRow < 1 || iSelRow >= m_pUsedGrid->GetRowCount())
		return;

	m_pUsedGrid->DeleteRow(iSelRow);
//	AddNewLine();
	
}

void CSetAidDlg::OnButtonDelAll() 
{
	// TODO: Add your control notification handler code here
	int iCount = m_pUsedGrid->GetRowCount();
	for (int ii = 1; ii < iCount; ii++)
	{
		m_pUsedGrid->DeleteRow(1);
//		AddNewLine();
	}
}

void CSetAidDlg::OnOK() 
{
	m_paidConfig->iCountResult = 0;
	CPnAidInfo* pPnAidInfo;
	for (int ii = 1; ii < m_pUsedGrid->GetRowCount(); ii++)
	{
		int iKind = m_pUsedGrid->GetItemParam(ii, 0);
		if (iKind < 1) continue;

		if (m_mapPnAidInfo.Lookup(iKind, pPnAidInfo))
			m_paidConfig->iCountResult++;
	}

	if (m_paidConfig->iCountResult == 0)
	{
		AfxGetMainWnd()->MessageBox("최소 1개의 지표가 있어야 합니다", COMPANYNAME);
		return;
	}
	m_paidConfig->pcBufResult = (char*) new _pnAidInfo[m_paidConfig->iCountResult];

	for (int ii = 1; ii < m_pUsedGrid->GetRowCount(); ii++)
	{
		int iKind = m_pUsedGrid->GetItemParam(ii, 0);
		if (iKind < 1) continue;

		if (m_mapPnAidInfo.Lookup(iKind, pPnAidInfo))
		{
			pPnAidInfo->GetStruck(m_paidConfig->pcBufResult + SZ_PNAIDINFO * (ii - 1));
		}
	}

	
	CDialog::OnOK();
}

void CSetAidDlg::OnButtonDefault() 
{
	OnButtonDelAll();


	struct _itemAid
	{
		char	acName[32];
		char	acText[16];
		int	iKind;
	};
	struct _itemAid	item[] = {
		{"가격 이평", "가격 이평", GK_PMA},		{"그물차트", "그물 차트", GK_NET},
		{"CCI",	"CCI", GK_CCI},				{"MACD", "MACD", GK_MACD},
		{"Fast Stochastics", "F - Stoc", GK_STOCHF},	{"Slow Stochastics", "S - Stoc", GK_STOCHS},
		{"거래량", "거래량", GK_VOL},			{"OBV",	"OBV", GK_OBV},
		{"심리도", "심리도", GK_PSY},			{"Bollinger Band", "Bollinger", GK_BOLB},
		{"Pivot Line", "Pivot", GK_PIVOT}
	};



	int iCount = sizeof (item) / sizeof(_itemAid);
	for (int ii = 0; ii < iCount; ii++)
	{
		AddNewLine();
		AddCell(m_pUsedGrid, ii + 1, 0, CString(item[ii].acText), item[ii].iKind);
		AddCell(m_pUsedGrid, ii + 1, 1, CString(item[ii].acName), item[ii].iKind);
	}

	m_pUsedGrid->Refresh();
}

void CSetAidDlg::AddUsedGrid()
{
//	for (int ii = 0; ii < DEFAULT_ROWS; ii++)
//	{
//		AddCell(m_pUsedGrid, ii + 1, 0, "", NULL);
//		AddCell(m_pUsedGrid, ii + 1, 1, "", NULL);
//	}

	m_pUsedGrid->DeleteNonFixedRows();
	for (int ii = 0; ii < m_iPnAidInfoCount; ii++)
	{
		AddNewLine();
		AddCell(m_pUsedGrid, ii + 1, 0, CString(m_pPnAidInfo[ii].acText, sizeof(m_pPnAidInfo[ii].acText)), (LPARAM)m_pPnAidInfo[ii].iKind);
		AddCell(m_pUsedGrid, ii + 1, 1, CString(m_pPnAidInfo[ii].acName, sizeof(m_pPnAidInfo[ii].acName)), (LPARAM)m_pPnAidInfo[ii].iKind);
	}
}

void CSetAidDlg::AddCell(CfxGrid *pGrid, int iRow, int iCol, CString strText, LPARAM lParam)
{
	strText.TrimRight();

	// Data SetItem
	GVITEM gvitem;
	gvitem.attr	= GVAT_ONLYROW;
	gvitem.state	= 0;
	gvitem.mask	= GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_BKCOLOR | GVMK_FGCOLOR | GVMK_ATTR | GVMK_PARAM;

	gvitem.fgcol	= COLOR_DATA_FG;
	gvitem.bkcol	= COLOR_DATA_BG;
	gvitem.row	= iRow;
	gvitem.col	= iCol;
	gvitem.format	= GVFM_LEFT;
	gvitem.text	= strText;
	gvitem.param	= lParam;

	if (iRow != 0 && iCol == CI_ICON)
		CopyMemory(&gvitem.font, &m_logfontS, sizeof(LOGFONT));
	else
		CopyMemory(&gvitem.font, &m_logfont, sizeof(LOGFONT));

	switch (iCol)	// mask
	{
	case CI_ICON:
//		gvitem.mask	= GVMK_IMAGE | GVMK_FORMAT | GVMK_BKCOLOR | GVMK_PARAM;
//		gvitem.image	= iImageIndex;
		gvitem.format	= GVFM_MULTI;
		gvitem.bkcol	= COLOR_GRID_SELECT_BG;
		break;
	default:
		break;
	}


	switch (iCol)	// attr
	{
	case CI_ICON:
		break;

	case CI_TEXT:
		break;
	}

	pGrid->SetItem(&gvitem);
}

void CSetAidDlg::InitBasic()
{
	struct _itemAid
	{
		char	acName[32];
		char	acText[16];
		int	iKind;
	};

	struct _itemAid	item[] = {
		{"가격 이평", "가격 이평", GK_PMA},		
		{"그물차트", "그물 차트", GK_NET},
		{"A/D Line", "A/D LINE", GK_ADLINE},		
		{"ADX",	"ADX", GK_ADX},
		{"CCI",	"CCI", GK_CCI},
		{"DMI",	"DMI", GK_DMI},
		{"Demark", "Demark", GK_DEMARK},
		{"MACD", "MACD", GK_MACD},
		{"MACD Oscillator", "M Osc", GK_MACDOS},
		{"Pivot Line", "Pivot", GK_PIVOT},
		{"Price Oscillator", "PMAO", GK_PMAO},	
		{"RSI", "RSI", GK_RSI}, 
		{"RSI(가중치)", "RSI(가중치)", GK_RSI},	// 20090928 : ADD : RSI 지표 추가
		{"TRIX", "TRIX", GK_TRIX},
		{"William's %R", "Wil %R", GK_WPR},
		{"Parabolic", "Parabol", GK_PARB},

		{"이격도", "이격도", GK_GAP},
		{"모멘텀", "모멘텀", GK_MOMENTUM},		
		{"ABRatio", "ABRatio", GK_ABRATIO},
		{"Band Width", "Band Width", GK_BANDWIDTH},	
		{"Chaikin's Oscillator", "CO", GK_CO},
		{"Chaikin's Volatility", "CV", GK_CV},
		{"Ease of Movement", "EMV", GK_EMV},
		{"LFI", "LFI", GK_LFI},
		{"Net Change Oscillator", "NCO", GK_NCO},
		{"Price Rate-of-Change", "PROC", GK_PROC},
		{"Sigma", "Sigma", GK_SIGMA},
		{"SONAR", "SONAR", GK_SONA},
		{"SONA Momentum", "SONAM", GK_SONAMOMENTUM},	
		{"Fast Stochastics", "F - Stoc", GK_STOCHF},
		{"Slow Stochastics", "S - Stoc", GK_STOCHS},
		
		{"거래량", "거래량", GK_VOL},		
		{"거래량 이평", "거래 이평", GK_VMA},
		{"거래대금", "거래대금", GK_AMT},
		{"거래대금 이평", "대금 이평", GK_AMA},
		{"회전율", "회전 율", GK_RRATE},	
		{"ADR", "ADR", GK_ADR},
		{"Money Flow Index", "MFI", GK_MFI},
		{"OBV",	"OBV", GK_OBV},
		{"Volume Oscillator", "VMAO", GK_VMAO},
		{"VR", "VR", GK_VR},
		{"Volume Rate-of-Change", "VROC", GK_VROC},
		
		{"심리도", "심리도", GK_PSY},
		{"신심리도", "신심리도", GK_NPSY},		
		
		{"Bollinger Band", "Bollinger", GK_BOLB},
		{"Envelope", "Envlope", GK_ENVL},

		{"외국인보유(수량)", "외인 보유", GK_FRGNHAVE}, 
		{"외국인보유(비율)", "외인 비율", GK_FRGNRATE},
		{"개인순매수(수량)", "개인 순매수", GK_PSNLPBUY}, 
		{"외국인순매수(수량)", "외인 순매수", GK_FRGNPBUY}, 
		{"기관순매수(수량)", "기관 순매수", GK_ORGNPBUY},
		{"개인누적순매수(수량)", "개인 누적순매수", GK_PSNLTBUY}, 
		{"외국인누적순매수(수량)", "외인 누적순매수", GK_FRGNTBUY}, 
		{"기관누적순매수(수량)", "기관 누적순매수", GK_ORGNTBUY}
	};

	int iCount = sizeof (item) / sizeof(_itemAid);

	for (int ii = 0; ii < iCount; ii++)
	{
		CPnAidInfo* pPAI = new CPnAidInfo;
		pPAI->m_iKind = item[ii].iKind;
		pPAI->m_strText = CString(item[ii].acText);
		pPAI->m_strText.TrimRight();
		pPAI->m_strName = CString(item[ii].acName);
		pPAI->m_strName.TrimRight();


		m_mapPnAidInfo.SetAt(pPAI->m_iKind, pPAI);

	}
}

void CSetAidDlg::RemoveBasic()
{
	int iKey;
	CPnAidInfo* pPnAidInfo;
	for (POSITION pos = m_mapPnAidInfo.GetStartPosition(); pos; )
	{
		m_mapPnAidInfo.GetNextAssoc(pos, iKey, pPnAidInfo);
		delete pPnAidInfo;
	}

	m_mapPnAidInfo.RemoveAll();
}

int CSetAidDlg::GetLastIndex() 
{
	CString strText;
	for (int ii = 1; ii < m_pUsedGrid->GetRowCount(); ii++)
	{
		strText = m_pUsedGrid->GetItemText(ii, 1);
		if (strText.IsEmpty())
			return ii;
	}

	return m_pUsedGrid->GetRowCount() - 1;
}

void CSetAidDlg::AddNewLine()
{
	m_pUsedGrid->InsertRow("");
	m_pUsedGrid->SetRowHeight(m_pUsedGrid->GetRowCount() - 1, ROW_HEIGHT);
}

BOOL CSetAidDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == ID_GRID_USED)
	{
		NM_GVITEM* pnmgvitem = (NM_GVITEM*) lParam;

		switch (pnmgvitem->hdr.code)
		{
		case GVNM_DBLCLICKED:
			OnButtonDel();
			break;
		}
	}
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CSetAidDlg::OnDblclkTreeIndicator(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonAdd();
	
	*pResult = 0;
}

void CSetAidDlg::ChangeRow(int iUpRow, int iDownRow)
{

	GVITEM gvitemUp;
	gvitemUp.mask	= GVMK_FORMAT | GVMK_FONT | GVMK_BKCOLOR | GVMK_FGCOLOR | GVMK_ATTR | GVMK_PARAM;
	gvitemUp.row = iUpRow;
	gvitemUp.col = 0;
	m_pUsedGrid->GetItem(&gvitemUp);
	gvitemUp.text = m_pUsedGrid->GetItemText(iUpRow, 0);
	gvitemUp.mask |= GVMK_TEXT;
	gvitemUp.row = iDownRow;

	GVITEM gvitemDown;
	gvitemDown.mask	= GVMK_FORMAT | GVMK_FONT | GVMK_BKCOLOR | GVMK_FGCOLOR | GVMK_ATTR | GVMK_PARAM;
	gvitemDown.row = iDownRow;
	gvitemDown.col = 0;
	m_pUsedGrid->GetItem(&gvitemDown);
	gvitemDown.text = m_pUsedGrid->GetItemText(iDownRow, 0);
	gvitemDown.mask |= GVMK_TEXT;
	gvitemDown.row = iUpRow;

	m_pUsedGrid->SetItem(&gvitemUp);
	m_pUsedGrid->SetItem(&gvitemDown);

	//gvitemUp.mask	= GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_BKCOLOR | GVMK_FGCOLOR | GVMK_ATTR | GVMK_PARAM;
	gvitemUp.mask	= GVMK_FORMAT | GVMK_FONT | GVMK_BKCOLOR | GVMK_FGCOLOR | GVMK_ATTR | GVMK_PARAM;

	gvitemUp.row = iUpRow;
	gvitemUp.col = 1;
	m_pUsedGrid->GetItem(&gvitemUp);
	gvitemUp.text = m_pUsedGrid->GetItemText(iUpRow, 1);
	gvitemUp.mask |= GVMK_TEXT;
	gvitemUp.row = iDownRow;

	//gvitemDown.mask	= GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_BKCOLOR | GVMK_FGCOLOR | GVMK_ATTR | GVMK_PARAM;
	gvitemDown.mask	= GVMK_FORMAT | GVMK_FONT | GVMK_BKCOLOR | GVMK_FGCOLOR | GVMK_ATTR | GVMK_PARAM;
	gvitemDown.row = iDownRow;
	gvitemDown.col = 1;
	m_pUsedGrid->GetItem(&gvitemDown);
	gvitemDown.text = m_pUsedGrid->GetItemText(iDownRow, 1);
	gvitemDown.mask |= GVMK_TEXT;
	gvitemDown.row = iUpRow;

	m_pUsedGrid->SetItem(&gvitemUp);
	m_pUsedGrid->SetItem(&gvitemDown);
}