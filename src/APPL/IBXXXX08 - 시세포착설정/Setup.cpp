#include "stdafx.h"
#include "IBXXXX08.h"
#include "Setup.h"
#include "libDefine.h"
//#include "../../h/axisfire.h"
#include "../../h/fxCommonx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { ITCOL_CODE = 0, ITCOL_NAME };
// sise
struct _grInfo grInfoSetup[] = {
	{ "현재가",	70,	"2023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "전일대비",	70,	"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,	"",	0},
	{ "매도호가",	70,	"2025",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "매수호가",	70,	"2026",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "등락률",	60,	"2033",	GVFM_RIGHT,	GVAT_CONDITION,			"",	0},
	{ "거래량",	78,	"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
};
enum { IUCOL_CURR = 0, IUCOL_DIFF, IUCOL_MDGA, IUCOL_MSGA, IUCOL_RATE, IUCOL_GVOL };

CSetup::CSetup(CWnd* pParent, CWnd* pView)
	: CDialog(CSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetup)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pView = pView;
	
	initConstruct();
}

void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetup)
	DDX_Control(pDX, IDC_INFO, m_info);
	//}}AFX_DATA_MAP
}

void CSetup::initConstruct()
{
	m_pSetupSise = nullptr;
	m_titleS = "시세포착설정";
	m_bPopup = false;
	memset(&m_codeinfo, 0x00, sz_codeinfo);
}

void CSetup::destroy()
{
	if (m_pSetupSise)
		LIB_DELETE(m_pSetupSise);
	m_bPopup = false;
}

BEGIN_MESSAGE_MAP(CSetup, CDialog)
	//{{AFX_MSG_MAP(CSetup)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CSetup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(m_titleS);
	initControl();
	m_bPopup = true;
	return 0;
}

void CSetup::OnDestroy() 
{
	destroy();
	CDialog::OnDestroy();
}

void CSetup::initControl()
{
	// 코드정보(code, name)
	/*
	m_info.SetWindowText("종목코드\t");
	m_info.setHeadColor(GetColorA(m_pView, 74), GetColorA(m_pView, 98));
	m_info.setColor(GetColorA(m_pView, 68), GetColorA(m_pView, 69), GetColorA(m_pView, 65));
	m_info.m_nHeadWidth = 45;
	m_info.SetAttr(0, DA_BOLD | DA_CENTER);


	// 시세grid
	CRect rc;
	GetDlgItem(IDC_POSSETUPSISE)->GetWindowRect(rc);
	ScreenToClient(rc);
	m_pSetupSise = new CExGrid();
	m_pSetupSise->Create(rc, this, IDC_GRSETUPSISE, GVSC_NONE, nullptr, WS_CHILD|WS_VISIBLE);
	initGrid(m_pSetupSise, IDC_GRSETUPSISE);

	updateSise();
	*/
}

void CSetup::SetCodeInfo(_codeinfo *codeinfo)
{
	memcpy(&m_codeinfo, codeinfo, sz_codeinfo);
}

void CSetup::GetCodeInfo(_codeinfo *codeinfo)
{
	memcpy(codeinfo, &m_codeinfo, sz_codeinfo);
}

void CSetup::Alert(CMapStringToString &rtmStore)
{
}

void CSetup::SetSise(CString dataS)
{
	m_siseS = dataS;
}

void CSetup::SetTitle(CString titleS)
{
	m_titleS = titleS;
}

void CSetup::SetFont(LOGFONT lf, CFont *pFont)
{
	m_lf = lf;
	m_pFont = pFont;
}

void CSetup::initGrid(CExGrid* pGrid, UINT idc)
{
	_GVITEM gvitem;
	COLORREF gridHead;
	CFont* pFont = nullptr;
	int ii = 0;
	int colCnt = 0;
	
	gridHead = GetColorA(m_pView, CLRGRHB);

	gvitem.row = 0;
	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR|GVMK_MISC|GVMK_PARAM;
	
	gvitem.font = m_lf;
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = GetColorA(m_pView, CLRGRHF);
	gvitem.bkcol = gridHead;

	switch (idc)
	{
	case IDC_GRSETUPSISE:
		pGrid->SetGridFocusLine(FALSE, FALSE);
		colCnt = sizeof(grInfoSetup)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.symbol = grInfoSetup[ii].symbols;
			gvitem.text = grInfoSetup[ii].heads;
			gvitem.attr = 0;
			gvitem.col = ii;
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoSetup[ii].width);
		}
		insertGrid(pGrid, idc, SISEROW);
		break;
	}
	
	pGrid->SetRowHeight(0, GRIDROW_HEIGHT);
	pGrid->SetRowColResize(FALSE, FALSE);
	pGrid->SetBKSelColor(GetColorA(m_pView, CLRGRSEL));
	pGrid->SetGridColor(GetColorA(m_pView, CLRGRLINE));
	pGrid->SetBkColor(GetColorA(m_pView, CLRGRROWB1));
	pGrid->Adjust();
}

int CSetup::insertGrid(CExGrid* pGrid, UINT idc, int row)
{
	_GVITEM gvitem;
	CString	 tmpS, statS;
	double val = 0;

	gvitem.font   = m_lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol = GetColorA(m_pView, CLRGRROWB1);
	gvitem.fgcol  = GetColorA(m_pView, CLRGRROWF);

	row = pGrid->InsertRow(" ", row);
	pGrid->SetRowHeight(row, GRIDROW_HEIGHT);
	gvitem.row    = row;

	int col = 0;
	switch (idc)
	{
	case IDC_GRSETUPSISE:
		for (col = 0; col < sizeof(grInfoSetup)/sizeof(_grInfo); col++)
		{
			gvitem.text = "";
			gvitem.col = col;
			gvitem.format = grInfoSetup[col].format;
			gvitem.attr = grInfoSetup[col].attr;
			pGrid->SetItem(&gvitem);
		}
		break;
	}
	return row;
}

void CSetup::updateSise()
{
	if (!m_pSetupSise)	return;

	CString dataS = m_siseS;
	CString tmpS;
	int pos = 0;
	while (!dataS.IsEmpty())
	{
		tmpS = parseX(dataS, "\t");
		switch (pos)
		{
		case 0:	// 종목코드
			//m_info.SetData(ITCOL_CODE, tmpS);		
			break;
		case 1:	// 종목명
			//m_info.SetData(ITCOL_NAME, tmpS);		
			break;
		case 2:	// 현재가
			m_pSetupSise->SetItemText(SISEROW, IUCOL_CURR, tmpS);		break;
		case 3:	// 대비
			m_pSetupSise->SetItemText(SISEROW, IUCOL_DIFF, tmpS);		break;
		case 4:	// 매도호가
			m_pSetupSise->SetItemText(SISEROW, IUCOL_MDGA, tmpS);		break;
		case 5:	// 매수호가
			m_pSetupSise->SetItemText(SISEROW, IUCOL_MSGA, tmpS);		break;
		case 6:	// 등락률
			m_pSetupSise->SetItemText(SISEROW, IUCOL_RATE, tmpS);		break;
		case 7:	// 거래량
			m_pSetupSise->SetItemText(SISEROW, IUCOL_GVOL, tmpS);		break;
		
		}

		pos++;
	}
}

int CSetup::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}

void CSetup::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
