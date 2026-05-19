// gridColumn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "gridColumn.h"
#include "afxdialogex.h"

#include"../h/mainvar.h"
#include "../amCom/formItem.h"
#include "../amCom/cellItem.h"

#define ROWSIZE		20
// CGridColumn 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGridColumn, CDialogEx)

CGridColumn::CGridColumn(CWnd* pParent, struct _mapH* mapH, int index)
	: CDialogEx(CGridColumn::IDD, pParent)
{
	m_mapH = mapH;
	m_index = index;

	m_gridInfo  = _T("Name|1|||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tLength|1|||cellsize\tWidth|1|||cellwidth\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2|0|1002|cellhpRGB\tHeadForeColor|2|0|1002|cellhtRGB\tDataBackColor|2|0|1002|cellpRGB\tDataForeColor|2|0|1002|celltRGB\tVisible|3|$bool|0x00000002|cellproperties\tSort|3|$bool|0x00000040|cellproperties\tFloatDigit|1|||cellvals\tHeadImage|2||1003|cellstr4\tRts Symbol|1|||cellrts|rts symbol.\tEdit Format|1||celleditS|celleditS\tDomino Name|1||cellauxs|cellauxs\tText Resize|3|$bool|0x00000800|cellproperties|Text resize in rect\tDefine Margin|3|$bool|0x00000008|cellproperties|Define special margin\tTop Margin|1||1|cellmargin0\tLeft Margin|1||1|cellmargin1\tBottom Margin|1||1|cellmargin2\tRight Margin|1||1|cellmargin3\tAttributes|4||1004|xxx");
	m_gridexInfo  = _T("Name|1|||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tMode|3|0-None;1-Input;3-Output;2-Both;||celliok\tType|3|0-Normal;1-Check;2-Combo;3-Button;4-Custom;9-H Candle Chart;10-Candle Chart;||cellkind\tCheckBoxText|1|||cellstr2\tComboCount|2|1|1011|cellkeys|\tInputStyle|3|0-Text;1-Image;2-Both;|0x0010;0x0020|cellproperties\tInputImage|2||1003|cellstr3\tButtonText|1|0256||celltxt\tCustomName|1|0256||celldat\tCustomProperties|1|0256||cellstr\tChart limit|3|0-High/Low price;1-High/Low limit price;||chartLimit\tLength|1|||cellsize\tFont|2|1|1001|cellfname|Choose Font Name.\tFontSize|1|||cellfsize|Set Font Size.\tFontStyle|3|$fstyle||cellfstyle|Choose Font Style.\tWidth|1|||cellwidth\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2|0|1002|cellhpRGB\tHeadForeColor|2|0|1002|cellhtRGB\tDataBackColor|2|0|1002|cellpRGB\tDataForeColor|2|0|1002|celltRGB\tHeadHeight|1|||cellheadH|Head height in Multiline Grid.\tCellHeight|1|||cellheight|Cell height in Multiline Grid.\tVisible|3|$bool|0x00000002|cellproperties\tSort|3|$bool|0x00000040|cellproperties\tFloatDigit|1|||cellvals\tHeadImage|2||1003|cellstr4\tRts Symbol|1|||cellrts|rts symbol.\tEdit Format|1||celleditS|celleditS\tDomino Name|1||cellauxs|cellauxs\tText Resize|3|$bool|0x00000800|cellproperties|Text resize in rect\tMerge Column|1||cellmerge|cellmerge\tDrag Row|3|$bool|0x00000400|cellproperties|Drag Row\tDefine Margin|3|$bool|0x00000008|cellproperties|Define special margin\tTop Margin|1||1|cellmargin0\tLeft Margin|1||1|cellmargin1\tBottom Margin|1||1|cellmargin2\tRight Margin|1||1|cellmargin3\tAttributes|4||1004|xxx");
	m_tableInfo = _T("Name|1|||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tLength|1|||cellsize\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2|0|1002|cellhpRGB\tHeadForeColor|2|0|1002|cellhtRGB\tDataBackColor|2|0|1002|cellpRGB\tDataForeColor|2|0|1002|celltRGB\tFloatDigit|1|||cellvals\tVisible|3|$bool|0x0002|cellproperties\tRts Symbol|1|||cellrts|rts symbol.\tEdit Format|1||celleditS|celleditS\tDomino Name|1||cellauxs|cellauxs\tText Resize|3|$bool|0x00000800|cellproperties|Text resize in rect\tAttributes|4||1004|xxx");
	m_multiLine = false;
	m_isGrid = true;
}

CGridColumn::~CGridColumn()
{
}

void CGridColumn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_MULTILINE, m_multiLine);
	DDX_Control(pDX, IDOK, m_okBN);
}


BEGIN_MESSAGE_MAP(CGridColumn, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ADDBTN, &CGridColumn::OnAdd)
	ON_BN_CLICKED(IDC_DELBTN, &CGridColumn::OnDelete)
	ON_BN_CLICKED(IDC_INSERTBTN, &CGridColumn::OnInsert)
	ON_BN_CLICKED(IDC_MULTILINE, &CGridColumn::OnMulti)
END_MESSAGE_MAP()


// CGridColumn 메시지 처리기입니다.


BOOL CGridColumn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFormItem formItem(m_mapH);
	int	nRows, nWidth = 90, nColCnt = 7;
	CRect	rc;

	m_isGrid = (formItem.GetForm(m_index)->kind == FM_GRID) ? true : false;
	m_isGridex = (formItem.GetForm(m_index)->kind == FM_GRIDEX) ? true : false;

	nRows = (m_isGrid? m_gridInfo : m_tableInfo).Replace("\t", "\t");
	if (m_isGridex)
		nRows = m_gridexInfo.Replace("\t", "\t");
		
//	if (m_isGrid || m_isGridex)
	if (m_isGridex)
		rc.SetRect(0, 30, nWidth * nColCnt + 80, nRows * 17 + 48);
	else
		rc.SetRect(0, 30, nWidth * nColCnt + 80, nRows * 17 + 63);

	m_propGrid.SetCellSize(nWidth, 16);
	if (!m_propGrid.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, rc,  this, 110))
		PostMessage(WM_CLOSE);

//	if (m_isGrid || m_isGridex)
	if (m_isGridex)
		SetWindowPos(NULL, 0, 0, nWidth * nColCnt + 90, nRows * 17 + 76, SWP_NOZORDER|SWP_NOMOVE);
	else
		SetWindowPos(NULL, 0, 0, nWidth * nColCnt + 90, nRows * 17 + 91, SWP_NOZORDER|SWP_NOMOVE);

	m_propGrid.Initialize(m_mapH);
	m_propGrid.SetIndex(m_index);
	m_propGrid.ResetGrid();
	
	if (m_isGrid)
	{
		m_propGrid.SetHeadInfo(102, m_gridInfo);	// grid cell
		m_propGrid.SetHeadInfo(102);
	}
	else if (m_isGridex)
	{
		m_propGrid.SetHeadInfo(105, m_gridexInfo);	// gridEX cell
		m_propGrid.SetHeadInfo(105);
	}
	else
	{
		m_propGrid.SetHeadInfo(103, m_tableInfo);	// table row
		m_propGrid.SetHeadInfo(103);
	}
	
	m_propGrid.LoadData(m_index, -1);
	m_propGrid.Invalidate();

	if (m_isGrid || m_isGridex)
	{
		if (formItem.GetForm(m_index)->attr2 & GO_MERGE)
		{
			formItem.GetForm(m_index)->attr2 &= ~GO_MULTI;
			m_multiLine = false;
			GetDlgItem(IDC_MULTILINE)->EnableWindow(FALSE);
		}
		else
		{
			if (formItem.GetForm(m_index)->attr2 & GO_MULTI)
				m_multiLine = true;
			else
				m_multiLine = false;
		}
	}
	else
		GetDlgItem(IDC_MULTILINE)->ShowWindow(SW_HIDE);

	if (m_isGridex)
		SetWindowText(_T("GridEx Column Properties"));

	UpdateData(FALSE);
	if (!m_isGrid && !m_isGridex)
		SetWindowText(_T("Table Row Properties"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGridColumn::OnOK()
{
	CFormItem formItem(m_mapH);

	m_propGrid.EraseCtrls();
	if (m_isGrid && formItem.GetForm(m_index)->attr2 & GO_MULTI)
	{
		struct _repR* repR = (struct _repR *)formItem.GetForm(m_index)->auxR;
		CCellItem *cellItem = new CCellItem(repR);

		if (repR)
		{
			int	line, colN;
			line = colN = 0;
			for (int ii = 0; ii < repR->cols; ii ++)
			{
				colN++;
				cellItem->GetCell(ii)->lineN = (BYTE)line;
				if (cellItem->GetCell(ii)->attr & FA_WRAP)
					line++;
			}
		}
		delete cellItem;
	}
//	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);	// prevent for PropertyGridButtonProperty
	CDialogEx::OnOK();
}


BOOL CGridColumn::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_TAB || pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGridColumn::OnClose()
{
	//AfxGetMainWnd()->PostMessageA(WM_USER, ID_USR_REFRESHCTRL, 1);
	CDialogEx::OnClose();
}


void CGridColumn::OnSize(UINT nType, int cx, int cy)
{
	if (IsWindow(m_propGrid.m_hWnd))
		m_propGrid.SetWindowPos(NULL, 0, 0, cx - 3, cy - 30, SWP_NOZORDER|SWP_NOMOVE);

	if (IsWindow(m_okBN.m_hWnd))
	{
		m_okBN.SetWindowPos(NULL, cx - 80, 5, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_DRAWFRAME);
		m_okBN.Invalidate();
	}
	
	CDialogEx::OnSize(nType, cx, cy);
}


void CGridColumn::OnAdd()
{
	m_propGrid.InsertCol(-1);
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
}


void CGridColumn::OnDelete()
{
	m_propGrid.DeleteCol(-1);
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
}


void CGridColumn::OnInsert()
{
	m_propGrid.InsertCol(-2);
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);	
}


void CGridColumn::OnMulti()
{
	if (!m_isGrid)
		return;

	CFormItem formItem(m_mapH);

	UpdateData();
	if (m_multiLine)
		formItem.GetForm(m_index)->attr2 |= GO_MULTI;
	else
		formItem.GetForm(m_index)->attr2 &= ~GO_MULTI;
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
}
