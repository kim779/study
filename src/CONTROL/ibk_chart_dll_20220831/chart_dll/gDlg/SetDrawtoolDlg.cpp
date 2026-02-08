// SetDrawtoolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgdlg.h"
#include "SetDrawtoolDlg.h"
//#include "../Common/MyTool.h"
#include "exGrid.h"
#include "../../h/axisgobj.h"
#include "../gCom/PnToolInfo.h"
//#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_GRID_BASE	10
#define ID_GRID_USED	11

#define GRID_HEIGHT	256
#define GRID_WIDTH	189

#define DEFAULT_ROWS_BASE	(20 + 1)	//(m_iBaseToolCount + 1)
#define DEFAULT_ROWS_USED	1
#define DEFAULT_COLS		2

#define COLOR_DATA_FG		RGB(0, 0, 0)
#define COLOR_DATA_BG		RGB(250, 250, 250)
#define COLOR_HEAD_FG		RGB(0, 0, 0)
#define COLOR_HEAD_BG		RGB(240, 240, 240)
#define COLOR_GRID_BG		RGB(255, 255, 255)
#define COLOR_GRID_SELECT_BG	RGB(211, 211, 211)
#define COLOR_GRID_LINE		RGB(230, 230, 230)


#define ROW_HEIGHT	23


#define CI_ICON		0
#define CI_TEXT		1


/////////////////////////////////////////////////////////////////////////////
// CSetDrawtoolDlg dialog


//CSetDrawtoolDlg::CSetDrawtoolDlg(class CMyTool* pMyTool, char* pcInfo, CWnd* pParent /*=NULL*/)
CSetDrawtoolDlg::CSetDrawtoolDlg(char* pcInfo, CWnd* pwndView, CWnd* pParent /*=NULL*/)
	: CDialog(CSetDrawtoolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDrawtoolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

//	m_pMyTool	= pMyTool;
	m_pwndView	= pwndView;
	m_pApp		= (CAxisGDlgApp*)AfxGetApp();

	m_ptoolConfig = (struct _toolConfig*)pcInfo;
	m_pImgLst = m_ptoolConfig->pImgLst;
	m_pPnToolInfo = (struct _pnToolInfo*)m_ptoolConfig->pcBuf;
	m_iPnToolInfoCount = m_ptoolConfig->iCount;

	m_pBaseGrid = NULL;
	m_pUsedGrid = NULL;

	m_iBaseToolCount	= 0;
}

void CSetDrawtoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDrawtoolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDrawtoolDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDrawtoolDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ALL, OnButtonAddAll)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL, OnButtonDelAll)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDrawtoolDlg message handlers

BOOL CSetDrawtoolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	InitBaseGrid();
	InitUsedGrid();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetDrawtoolDlg::OnDestroy() 
{
	if (m_pBaseGrid)
	{
		m_pBaseGrid->DestroyWindow();
		delete m_pBaseGrid;
		m_pBaseGrid = NULL;
	}

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

void CSetDrawtoolDlg::RemoveBasic()
{
	int iKey;
	CPnToolInfo* pPnToolInfo;
	for (POSITION pos = m_mapPnToolInfo.GetStartPosition(); pos; )
	{
		m_mapPnToolInfo.GetNextAssoc(pos, iKey, pPnToolInfo);
		delete pPnToolInfo;
	}

	m_mapPnToolInfo.RemoveAll();
}


void CSetDrawtoolDlg::InitUsedGrid()
{
	CFont* pfont = m_pApp->GetFont(m_pwndView, 9, "굴림", FALSE, 0);
	pfont->GetLogFont(&m_logfont);
	m_logfont.lfHeight = 90;

	m_pUsedGrid = new CExGrid();
	m_pUsedGrid->Create(CRect(250, 10, 250 + GRID_WIDTH, 10 + GRID_HEIGHT), 
		this, ID_GRID_USED, LOWORD(MAKELONG(GVSC_BOTH, GVSC_NONE)), HIWORD(MAKELONG(GVSC_BOTH, GVSC_NONE)), 
		WS_CHILD | WS_VISIBLE | WS_BORDER);

	m_pUsedGrid->Initial(DEFAULT_ROWS_USED, DEFAULT_COLS, 1, 0);
	m_pUsedGrid->SetGridFocusLine(FALSE, TRUE);

	int	aiColumnWidth[]	= {21, 149, };
	char	aacColumnName[][7] ={"툴","이름", };

	GVITEM	gvitem;
	int	ii;

	// Data SetItem
	for (ii = 0; ii < DEFAULT_ROWS_USED; ii++)
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

	for (ii = 0; ii < DEFAULT_ROWS_USED; ii++)
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

	m_pUsedGrid->SetImageList(m_pImgLst);
	m_pUsedGrid->SetRowColResize(FALSE, FALSE);

	AddUsedGrid();
}

void CSetDrawtoolDlg::AddUsedGrid()
{
	m_pUsedGrid->DeleteNonFixedRows();

	for (int ii = 0; ii < m_iPnToolInfoCount; ii++)
	{
		if (m_pPnToolInfo[ii].iIndex >= 0 && m_pPnToolInfo[ii].iKind >= toolMOUSE && m_pPnToolInfo[ii].iKind <= toolPENCIL)
		{
			AddNewLine();
		}
	}

	for (int ii = 0; ii < m_iPnToolInfoCount; ii++)
	{
		if (m_pPnToolInfo[ii].iIndex >= 0 && m_pPnToolInfo[ii].iKind >= toolMOUSE && m_pPnToolInfo[ii].iKind <= toolPENCIL)
		{
			AddCell(m_pUsedGrid, m_pPnToolInfo[ii].iIndex, 0, "", (LPARAM)m_pPnToolInfo[ii].iKind, m_pPnToolInfo[ii].iKind * 3);
			CPnToolInfo* pPTI;
			if (!m_mapPnToolInfo.Lookup(m_pPnToolInfo[ii].iKind, pPTI))
				continue;

			//AddCell(m_pUsedGrid, m_pPnToolInfo[ii].iIndex, 1, CString(m_pPnToolInfo[ii].acText, sizeof(m_pPnToolInfo[ii].acText)), (LPARAM)&m_pPnToolInfo[ii].iKind);
			AddCell(m_pUsedGrid, m_pPnToolInfo[ii].iIndex, 1, pPTI->m_strText, (LPARAM)&m_pPnToolInfo[ii].iKind);
		}
	}
}

void CSetDrawtoolDlg::InitBaseGrid()
{

	int aiToolKind[] = {	toolMOUSE, toolHISTORY, toolDELETE, toolINPUT, toolWIDTH,
				toolPALETTE, toolTEND, toolTENDEX, toolHLINE, toolARC,
				toolUARROW, toolGIGAN, toolDAEDEUNG, toolFIBOLINE, toolGANNFAN,
				toolTRILINE, toolAndrewPf, toolPatternUP, toolPatternDN, toolPENCIL};


	CString	astrToolText[] = {	"마우스", "하나씩지우기", "전체지우기", "문자입력", "도구굵기설정",
					"도구색상설정", "추세선", "연장추세선", "가로선", "원",
					"위화살표", "기간 및 등락표시", "대등일표시", "Fibonacci TimeZone", "Gann Fan(상승)",
					"Trizone Line", "Andrews Pichfork", "상승패턴분석", "하락패턴분석", "연필"};

	m_iBaseToolCount = sizeof(aiToolKind) / sizeof(int);

	for (int ii = 0; ii < m_iBaseToolCount; ii++)
	{
		CPnToolInfo* pPTI = new CPnToolInfo;
		pPTI->m_iKind = aiToolKind[ii];
		pPTI->m_strText = CString(astrToolText[ii]);
		pPTI->m_strText.TrimRight();
		pPTI->m_iIndex = ii + 1;

		m_mapPnToolInfo.SetAt(pPTI->m_iKind, pPTI);
	}

	CFont* pfont = m_pApp->GetFont(m_pwndView, 9, "굴림", FALSE, 0);
	pfont->GetLogFont(&m_logfont);
	m_logfont.lfHeight = 90;

	m_pBaseGrid = new CExGrid();
	m_pBaseGrid->Create(CRect(10, 10, 10 + GRID_WIDTH, 10 + GRID_HEIGHT), 
		this, ID_GRID_BASE, LOWORD(MAKELONG(GVSC_BOTH, GVSC_NONE)), HIWORD(MAKELONG(GVSC_BOTH, GVSC_NONE)), 
		WS_CHILD | WS_VISIBLE | WS_BORDER);

	m_pBaseGrid->Initial(DEFAULT_ROWS_BASE, DEFAULT_COLS, 1, 0);
	m_pBaseGrid->SetGridFocusLine(FALSE, TRUE);

	int	aiColumnWidth[]	= {21, 149,};
	char	aacColumnName[][7] = {"툴","이름",};

	GVITEM	gvitem;

	// Data SetItem
	for (int ii = 0; ii < DEFAULT_ROWS_BASE; ii++)
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
			m_pBaseGrid->SetItem(&gvitem);
			m_pBaseGrid->SetColumnWidth(jj, aiColumnWidth[jj]);
		}
	}

	for (int ii = 0; ii < DEFAULT_ROWS_BASE; ii++)
	{
		m_pBaseGrid->SetRowHeight(ii, ROW_HEIGHT);
	}

	// 라인색깔
	m_pBaseGrid->SetGridColor(COLOR_GRID_LINE);
	// 라인유무
	m_pBaseGrid->SetGridLine(GVLN_BOTH);
	// 라인스타일
	m_pBaseGrid->SetGridLineType(0);
	// 그리드 배경색
	m_pBaseGrid->SetGridBkColor(COLOR_GRID_BG);
	// 그리드 선택색상
	m_pBaseGrid->SetBKSelColor(COLOR_GRID_SELECT_BG);
	// Column size 변경금지
	m_pBaseGrid->SetRowColResize(FALSE, TRUE);

	m_pBaseGrid->SetTitleTips(TRUE);
	m_pBaseGrid->Adjust();

	m_pBaseGrid->SetImageList(m_pImgLst);
	m_pBaseGrid->SetRowColResize(FALSE, FALSE);

	for (int ii = 0; ii < m_iBaseToolCount; ii++)
	{
		AddCell(m_pBaseGrid, ii + 1, 0, "", aiToolKind[ii], aiToolKind[ii] * 3);
		AddCell(m_pBaseGrid, ii + 1, 1, astrToolText[ii], NULL);
	}
}

void CSetDrawtoolDlg::AddCell(CfxGrid* pGrid, int iRow, int iCol, CString strText, LPARAM lParam, int iImageIndex)
{
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

	CopyMemory(&gvitem.font, &m_logfont, sizeof(LOGFONT));

	switch (iCol)	// mask
	{
	case CI_ICON:
		gvitem.mask	= GVMK_IMAGE | GVMK_FORMAT | GVMK_BKCOLOR | GVMK_PARAM;
		gvitem.image	= iImageIndex;
		gvitem.format	= GVFM_CENTER;
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

void CSetDrawtoolDlg::OnButtonUp() 
{
	int iSelRow = m_pUsedGrid->GetSelectRow();
	if (iSelRow < 3 || iSelRow >= m_pUsedGrid->GetRowCount())
		return;

	int iKind = m_pUsedGrid->GetItemParam(iSelRow, 0);
	if (iKind == toolMOUSE)
		return;

	ChangeRow(iSelRow - 1, iSelRow);
	m_pUsedGrid->SetSelectRow(iSelRow - 1);
}

void CSetDrawtoolDlg::OnButtonDown() 
{
	int iSelRow = m_pUsedGrid->GetSelectRow();
	if (iSelRow < 2 || iSelRow >= m_pUsedGrid->GetRowCount() - 1)
		return;

	int iKind = m_pUsedGrid->GetItemParam(iSelRow, 0);
	if (iKind == toolMOUSE)
		return;

	ChangeRow(iSelRow, iSelRow + 1);
	m_pUsedGrid->SetSelectRow(iSelRow + 1);
}

void CSetDrawtoolDlg::OnButtonAdd() 
{
	int iSelBaseRow = m_pBaseGrid->GetSelectRow();
	if (iSelBaseRow < 1 || iSelBaseRow >= m_pBaseGrid->GetRowCount())
		return;

	int iSelKind = m_pBaseGrid->GetItemParam(iSelBaseRow, 0);
	if (iSelKind == toolMOUSE)
		return;

	if (IsExist(iSelKind))
		return;

//	struct _pnToolInfo* pPTI = FindPnToolInfoByKind(iSelKind);
//	if (!pPTI || pPTI->iIndex > 0)	// 이미 있으면
//		return;

	

//	IndexUpDown(true, iLastRow);
//	pPTI->iIndex = iLastRow;
//	ChangePopIndex(pPTI->iKind, pPTI->iIndex);

	AddNewLine();
	int iLastRow = GetLastIndex();

	CPnToolInfo* pPTI;
	if (!m_mapPnToolInfo.Lookup(iSelKind, pPTI))
		return;

	AddCell(m_pUsedGrid, iLastRow, 0, "", pPTI->m_iKind, pPTI->m_iKind * 3);
	AddCell(m_pUsedGrid, iLastRow, 1, pPTI->m_strText, pPTI->m_iKind);
	m_pUsedGrid->Refresh();
}

void CSetDrawtoolDlg::OnButtonDel() 
{
	int iSelRow = m_pUsedGrid->GetSelectRow();
	if (iSelRow < 1 || iSelRow >= m_pUsedGrid->GetRowCount())
		return;

	int iKind = m_pUsedGrid->GetItemParam(iSelRow, 0);
	if (iKind <= toolMOUSE || iKind > toolPENCIL)
		return;


//	IndexUpDown(false, pPTI->iIndex);

//	int iEnd = m_pUsedGrid->GetRowCount() - 1;
	m_pUsedGrid->DeleteRow(iSelRow);
//	AddNewLine();
//	pPTI->iIndex = -1;
//	ChangePopIndex(pPTI->iKind , -1);
}

void CSetDrawtoolDlg::OnButtonAddAll() 
{
	for (int ii = 1; ii < m_pBaseGrid->GetRowCount(); ii++)
	{
		int iSelKind = m_pBaseGrid->GetItemParam(ii, 0);
		if (iSelKind == toolMOUSE)
			continue;

		if (IsExist(iSelKind))
			continue;
//		struct _pnToolInfo* pPTI = FindPnToolInfoByKind(iSelKind);
//		if (!pPTI || pPTI->iIndex > 0)	// 이미 있으면
//			continue;

		AddNewLine();
		int iLastRow = GetLastIndex();

//		IndexUpDown(true, iLastRow);
//		pPTI->iIndex = iLastRow;
//		ChangePopIndex(pPTI->iKind, pPTI->iIndex);

		CPnToolInfo* pPTI;
		if (!m_mapPnToolInfo.Lookup(iSelKind, pPTI))
			return;

		AddCell(m_pUsedGrid, iLastRow, 0, "", (LPARAM)pPTI->m_iKind, pPTI->m_iKind * 3);
		AddCell(m_pUsedGrid, iLastRow, 1, pPTI->m_strText, (LPARAM)pPTI->m_iKind);
	}

	m_pUsedGrid->Refresh();
}

void CSetDrawtoolDlg::OnButtonDelAll() 
{
	while (m_pUsedGrid->GetRowCount() > 2)
	{
		m_pUsedGrid->DeleteRow(2);
	}
/*
	for (ii = 0; ii < m_iPnToolInfoCount; ii++)
	{
		if (m_pPnToolInfo[ii].iKind == toolMOUSE)
			continue;
		
		m_pPnToolInfo[ii].iIndex = -1;
		ChangePopIndex(m_pPnToolInfo[ii].iKind , -1);
	}
*/
}
/*
_pnToolInfo* CSetDrawtoolDlg::FindPnToolInfoByIndex(int iIndex)
{	

	for (int ii = 0; ii < m_iPnToolInfoCount; ii++)
	{
		if (m_pPnToolInfo[ii].iIndex == iIndex)
			return &m_pPnToolInfo[ii];
	}

	return NULL;
}

_pnToolInfo* CSetDrawtoolDlg::FindPnToolInfoByKind(int iKind)
{	

	for (int ii = 0; ii < m_iPnToolInfoCount; ii++)
	{
		if (m_pPnToolInfo[ii].iKind == iKind)
			return &m_pPnToolInfo[ii];
	}

	return NULL;
}
*/
/*
void CSetDrawtoolDlg::IndexUpDown(bool bUp, int iIndex)
{

	if (bUp)
	{
		for (int ii = 0; ii < m_iPnToolInfoCount; ii++)
		{
			if (m_pPnToolInfo[ii].iIndex >= iIndex)
				m_pPnToolInfo[ii].iIndex++;
		}
	}
	else	// down
	{

		for (int ii = 0; ii < m_iPnToolInfoCount; ii++)
		{
			if (m_pPnToolInfo[ii].iIndex > iIndex)
				m_pPnToolInfo[ii].iIndex--;
		}
	}
}
*/
void CSetDrawtoolDlg::AddNewLine()
{
	m_pUsedGrid->InsertRow("");
	m_pUsedGrid->SetRowHeight(m_pUsedGrid->GetRowCount() - 1, ROW_HEIGHT);
	m_pUsedGrid->Adjust();
}

void CSetDrawtoolDlg::OnOK() 
{
	int iCount = m_pUsedGrid->GetRowCount() - 1;
	if (iCount < 1)
		return;

	for (int ii = 1; ii < m_pUsedGrid->GetRowCount(); ii++)
	{
		int iKind = m_pUsedGrid->GetItemParam(ii, 0);

		switch (iKind)
		{
		case toolHLINE:
		case toolARC:
		case toolUARROW:
		case toolGIGAN:
		case toolDAEDEUNG:
		case toolFIBOLINE:
		case toolGANNFAN:
		case toolTRILINE:
			iCount++;
			break;
		}
	}

	m_ptoolConfig->iCountResult = iCount;
	struct _pnToolInfo* pPTIBuf = new struct _pnToolInfo[iCount];
	ZeroMemory(pPTIBuf, SZ_PNTOOLINFO * iCount);
	m_ptoolConfig->pcBufResult = (char*)pPTIBuf;
	//struct _PnToolInfo* pPTIBuf = (struct _PnToolInfo*)m_ptoolConfig->pcBufResult;

	int iIndex = 1;
	int iPos = 0;

	// TODO: Add extra validation here
	//for (ii = 0; ii < iCount; ii++)
	for (int ii = 0; ii < m_pUsedGrid->GetRowCount(); ii++)
	{
		int iKind = m_pUsedGrid->GetItemParam(ii + 1, 0);
		CPnToolInfo* pPTI;
		if (!m_mapPnToolInfo.Lookup(iKind, pPTI))
			continue;
		
		pPTI->GetStruck((char*)&pPTIBuf[iPos]);
		pPTIBuf[iPos].iKind = iKind;
		pPTIBuf[iPos].iIndex = iIndex;

		pPTIBuf[iPos].rc.top = 1 + (iIndex - 1) * 26;
		pPTIBuf[iPos].rc.bottom = 24 + (iIndex - 1) * 26;
		pPTIBuf[iPos].rc.left = 10;
		pPTIBuf[iPos].rc.right = 32;
		iPos++;

		int iPopKind = NULL;
		CString strText;
		switch (pPTI->m_iKind)
		{
		case toolHLINE:
			iPopKind = popLINE;
//			strText = "";
			break;
		case toolARC:
			iPopKind = popFIGURE;
//			strText = "";
			break;
		case toolUARROW:
			iPopKind = popARROW;
//			strText = "";
			break;
		case toolGIGAN:
			iPopKind = popGIGAN;
//			strText = "";
			break;
		case toolDAEDEUNG:
			iPopKind = popDAEDEUNG;
//			strText = "";
			break;
		case toolFIBOLINE:
			iPopKind = popFIBO;
//			strText = "";
			break;
		case toolGANNFAN:
			iPopKind = popGANN;
//			strText = "";
			break;
		case toolTRILINE:
			iPopKind = popETC;
//			strText = "";
			break;
		}

		if (iPopKind)
		{
			pPTIBuf[iPos].iKind = iPopKind;
			pPTIBuf[iPos].iIndex = iIndex;
			pPTIBuf[iPos].iBtnState = NULL;
			ZeroMemory(pPTIBuf[iPos].acText, sizeof(pPTIBuf[iPos].acText));

			pPTIBuf[iPos].rc.top = 1 + (iIndex - 1) * 26;
			pPTIBuf[iPos].rc.bottom = 24 + (iIndex - 1) * 26;
			pPTIBuf[iPos].rc.left = 2;
			pPTIBuf[iPos].rc.right = 32;
			iPos++;
		}

		iIndex++;
	}

	CDialog::OnOK();
}

int CSetDrawtoolDlg::GetLastIndex() 
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
/*
bool CSetDrawtoolDlg::ChangePopIndex(int iKind, int iIndex)
{
	int iPopKind = NULL;
	switch (iKind)
	{
	case toolHLINE:
		iPopKind = popLINE;
		break;
	case toolARC:
		iPopKind = popFIGURE;
		break;
	case toolUARROW:
		iPopKind = popARROW;
		break;
	case toolGIGAN:
		iPopKind = popGIGAN;
		break;
	case toolDAEDEUNG:
		iPopKind = popDAEDEUNG;
		break;
	case toolFIBOLINE:
		iPopKind = popFIBO;
		break;
	case toolGANNFAN:
		iPopKind = popGANN;
		break;
	case toolTRILINE:
		iPopKind = popETC;
		break;
	}

	if (!iPopKind)
		return false;

	for (int ii = 0; ii < m_iPnToolInfoCount; ii++)
	{
		if (m_pPnToolInfo[ii].iKind == iPopKind)
		{
			m_pPnToolInfo[ii].iIndex = iIndex;
			return true;
		}
	}

	return false;

}
*/

void CSetDrawtoolDlg::ChangeRow(int iUpRow, int iDownRow)
{

	GVITEM gvitemUp;
	gvitemUp.mask	= GVMK_IMAGE | GVMK_FORMAT | GVMK_BKCOLOR | GVMK_PARAM;
	gvitemUp.row = iUpRow;
	gvitemUp.col = 0;
	m_pUsedGrid->GetItem(&gvitemUp);
	gvitemUp.row = iDownRow;
//	struct _pnToolInfo* pPTIUp  = (struct _pnToolInfo*)gvitemUp.param;


	GVITEM gvitemDown;
	gvitemDown.mask	= GVMK_IMAGE | GVMK_FORMAT | GVMK_BKCOLOR | GVMK_PARAM;
	gvitemDown.row = iDownRow;
	gvitemDown.col = 0;
	m_pUsedGrid->GetItem(&gvitemDown);
	gvitemDown.row = iUpRow;
//	struct _pnToolInfo* pPTIDown  = (struct _pnToolInfo*)gvitemDown.param;

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

//	int iBakup = pPTIUp->iIndex;
//	pPTIUp->iIndex = pPTIDown->iIndex;
//	pPTIDown->iIndex = iBakup;
		
//	ChangePopIndex(pPTIUp->iKind, pPTIUp->iIndex);
//	ChangePopIndex(pPTIDown->iKind, pPTIDown->iIndex);
}


void CSetDrawtoolDlg::OnButtonDefault() 
{
	// TODO: Add your control notification handler code here
	OnButtonDelAll();
	OnButtonAddAll();
}

BOOL CSetDrawtoolDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == ID_GRID_BASE)
	{
		NM_GVITEM* pnmgvitem = (NM_GVITEM*) lParam;

		switch (pnmgvitem->hdr.code)
		{
		case GVNM_DBLCLICKED:
			OnButtonAdd();
			break;
		}
	}
	else 	if (wParam == ID_GRID_USED)
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

bool CSetDrawtoolDlg::IsExist(int iKind)
{

	for (int ii = 1; ii < m_pUsedGrid->GetRowCount(); ii++)
	{
		if (iKind == m_pUsedGrid->GetItemParam(ii, 0))
			return true;
	}

	return false;
}