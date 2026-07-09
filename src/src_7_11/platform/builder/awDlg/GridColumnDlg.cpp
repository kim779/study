// GridColumnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GridColumnDlg.h"
#include "../mainvar.h"
#include "../awCommon/FormItem.h"
#include "../awCommon/CellItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridColumnDlg dialog


CGridColumnDlg::CGridColumnDlg(CWnd* pParent /*=NULL*/, struct _mapH* mapH, int index)
	: CDialog(CGridColumnDlg::IDD, pParent)
{
	m_mapH = mapH;
	m_index = index;

	m_sCellGrid  = "Name|1|||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tMode|3|0-None;1-Input;3-Output;2-Both;||celliok\tType|3|0-Normal;1-Check;2-Combo;3-Button;9-Graph;||cellkind\tCheckBoxText|1|||cellstr2\tExclusiveCheck|3|$bool|0x0004|cellproperties\tComboCount|2|1|1011|cellkeys|\tButtonType|3|0-Text;1-Bmp;2-Both;|0x0010;0x0080|cellproperties\tButtonText|1|0256||celltxt\tButtonImage|2||1003|cellstr3\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||cellvals2\tLength|1|||cellsize\tWidth|1|||cellwidth\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tImageAlignment|3|0-Center;5-Tile;6-Stretch;||cellalignI\tHeadBackColor|2|0|1002|cellhpRGB\tHeadForeColor|2|0|1002|cellhtRGB\tDataBackColor|2|0|1002|cellpRGB\tDataForeColor|2|0|1002|celltRGB\tBoldColumn|3|$bool|0x0200|cellproperties\tHintType|3|$hint||cellisHint\tHintText|1|||cellonHint\tDomino|1|||cellauxs\tReference|1|||cellrefs\tVisible|3|$bool|0x0002|cellproperties\tSort|3|$bool|0x0020|cellproperties\tShowFormat|3|$bool|0x0100|cellproperties\tFloatDigit|1|||cellvals\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||cellattr2\tIgnoreFloating0|3|$bool|0x00400000|cellattr\tColumnSuppress|3|$bool|0x0040|cellproperties\tEdit|4||1007|xxx\tAttributes|4||1004|xxx";
	m_sCellTable = "Name|1|||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tLength|1|||cellsize\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2|0|1002|cellhpRGB\tHeadForeColor|2|0|1002|cellhtRGB\tDataBackColor|2|0|1002|cellpRGB\tDataForeColor|2|0|1002|celltRGB\tHintType|3|$hint||cellisHint\tHintText|1|||cellonHint\tFloatDigit|1|||cellvals\tDomino|1|||cellauxs\tReference|1|||cellrefs\tVisible|3|$bool|0x0002|cellproperties\tEdit|4||1007|xxx\tAttributes|4||1004|xxx";
	//{{AFX_DATA_INIT(CGridColumnDlg)
	m_bMultiLine = FALSE;
	//}}AFX_DATA_INIT
}


void CGridColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridColumnDlg)
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Check(pDX, IDC_MULTILINE, m_bMultiLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridColumnDlg, CDialog)
	//{{AFX_MSG_MAP(CGridColumnDlg)
	ON_BN_CLICKED(IDC_ADDBTN, OnAddbtn)
	ON_BN_CLICKED(IDC_DELBTN, OnDelbtn)
	ON_BN_CLICKED(IDC_MULTILINE, OnMultiline)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_INSERTBTN, OnInsertbtn)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridColumnDlg message handlers

BOOL CGridColumnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CFormItem FormItem(m_mapH);
	m_bGrid = FormItem.getFormPoint(m_index)->kind == FM_TABLE?false:true;
	int nRows = (m_bGrid?m_sCellGrid:m_sCellTable).Replace("\t", "\t"),
		nWidth = 90, nColCnt = 7;
	m_listGrid.SetCellSize(nWidth, 16);
	if (!m_listGrid.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 
		(m_bGrid?CRect(0, 30, nWidth * nColCnt + 80, nRows * 17 + 48/* 609*/):CRect(0, 30, nWidth * nColCnt + 80, nRows * 17 + 63/*352*/)), this, 110))
		PostMessage(WM_CLOSE);

	if (!m_bGrid)
		SetWindowPos(NULL, 0, 0, nWidth * nColCnt + 90, nRows * 17 + 91/*380*/, SWP_NOZORDER|SWP_NOMOVE);
	else
		SetWindowPos(NULL, 0, 0, nWidth * nColCnt + 90, nRows * 17 + 76/*637*/, SWP_NOZORDER|SWP_NOMOVE);

	m_listGrid.Initialize(m_mapH);
	m_listGrid.SetMulti(true);
	m_listGrid.setIndex(m_index);
	m_listGrid.ResetGrid();
	
	if (m_bGrid)
	{
		m_listGrid.setHeadInfo(102/*grid Cell*/, m_sCellGrid);
		m_listGrid.setHeadInfo(102/*grid Cell*/);
	}
	else
	{
		m_listGrid.setHeadInfo(103/*Table Row*/, m_sCellTable);
		m_listGrid.setHeadInfo(103/*Table Row*/);
	}
	
	m_listGrid.LoadData(m_index, -1);
	m_listGrid.Invalidate();

	if (m_bGrid)
	{
		if (FormItem.getFormPoint(m_index)->attr2 & GO_MERGE)
		{
			FormItem.getFormPoint(m_index)->attr2 &= ~GO_MULTI;
			m_bMultiLine = false;
			GetDlgItem(IDC_MULTILINE)->EnableWindow(FALSE);
		}
		else
		{
			if (FormItem.getFormPoint(m_index)->attr2 & GO_MULTI)
				m_bMultiLine = true;
			else
				m_bMultiLine = false;
		}
	}
	else
		GetDlgItem(IDC_MULTILINE)->ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	if (!m_bGrid)
		SetWindowText("Table Row Edit Dialog");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CGridColumnDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CGridColumnDlg::OnAddbtn() 
{
	m_listGrid.insertCol(-1);
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
}

void CGridColumnDlg::OnDelbtn() 
{
	m_listGrid.deleteCol(-1);
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
}

void CGridColumnDlg::OnMultiline() 
{
	if (!m_bGrid)
		return;
	CFormItem FormItem(m_mapH);
	UpdateData();

	if (m_bMultiLine)
		FormItem.getFormPoint(m_index)->attr2 |= GO_MULTI;
	else
		FormItem.getFormPoint(m_index)->attr2 &= ~GO_MULTI;
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
}

void CGridColumnDlg::OnOK() 
{
	m_listGrid.EraseCtrls();
	CFormItem FormItem(m_mapH);

	if (m_bGrid && FormItem.getFormPoint(m_index)->attr2 & GO_MULTI)
	{
		struct _repR *repR   = (struct _repR *)FormItem.getFormPoint(m_index)->auxR;
		CCellItem *cellItem = new CCellItem(repR);
		if (repR)
		{
			int	line, colN;
			line = colN = 0;
			for (int ii = 0; ii < repR->cols; ii ++)
			{
				colN++;
				cellItem->getCellPoint(ii)->lineN = (BYTE)line;
				if (cellItem->getCellPoint(ii)->attr & FA_WRAP)
					line++;
			}
		}
		delete cellItem;
	}
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
	CDialog::OnOK();
}

void CGridColumnDlg::OnClose() 
{
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, 1);
	CDialog::OnClose();
}

void CGridColumnDlg::OnInsertbtn() 
{
	m_listGrid.insertCol(-2);
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);	
}

void CGridColumnDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (IsWindow(m_listGrid.m_hWnd))
		m_listGrid.SetWindowPos(NULL, 0, 0, cx - 3, cy - 30, SWP_NOZORDER|SWP_NOMOVE);

	if (IsWindow(m_okBtn.m_hWnd))
	{
		m_okBtn.SetWindowPos(NULL, cx - 80, 5, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_DRAWFRAME);
		m_okBtn.Invalidate();
	}
	CDialog::OnSize(nType, cx, cy);
}
