// gridColumn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "gridColumn.h"
#include "afxdialogex.h"

#include"../h/mainvar.h"
#include "../awTool/awObjectLoad.h"
#include "../h/mapform.H"


// CGridColumn 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGridColumn, CDialogEx)

CGridColumn::CGridColumn(CWnd* pParent, class mapForm* mapForm, int index)
	: CDialogEx(CGridColumn::IDD, pParent)
{
	m_mapH = mapForm;
	m_index = index;

	m_gridInfo = _T("Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||cellcharType\tMode|3|0-Free;1-Input;2-Output;3-Both;||cellioType\tType|3|0-Normal;1-Check;2-Combo;3-Button;4-Graph;||cellkind\tCheckBoxText|1|||cellstrings\tExclusiveCheck|3|$bool|0x00000004|cellstyles\tComboCount|2|1|1011|entryCombo|Edit Combo Contents and display the number of contents.\tButtonType|3|0-Text;1-Bmp;2-Both;|0x00000008;0x00000010|cellstyles\tButtonText|1|0256||celldisplays\tButtonImage|2||1003|cellimages\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||cellkeyParam\tLength|1|||cellsize\tWidth|1|||cellwidth\tHead|1||1|cellheadText\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignmentHead\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignmentText\tImageAlignment|3|0-Center;5-Stretch;6-Tile;||cellalignmentImage\tHeadBackColor|2||1002|cellheadPaintColor\tHeadForeColor|2||1002|cellheadTextColor\tDataBackColor|2||1002|cellpaintColor\tDataForeColor|2||1002|celltextColor\tHintType|3|$typeHint||celltypeHint\tHintText|1|||cellhintText\tDomino|1|||celldominoName\tReference|1|||cellreferenceName\tVisible|3|$bool|0x00000002|cellstyles\tSort|3|$bool|0x00000020|cellstyles\tShowFormat|3|$bool|0x00000040|cellstyles|Choose format option.\tFloatDigit|1|||celloffsets\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||cellauxAttributes\tIgnoreFloating0|3|$bool|0x00010000|cellattributes\tColumnSuppress|3|$bool|0x00000080|cellstyles\tEdit|4||1007|xxx\tAttributes|4||1004|xxx");
	m_gridExInfo = _T("Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||cellcharType\tMode|3|0-Free;1-Input;2-Output;3-Both;||cellioType\tType|3|0-Normal;1-Check;2-Combo;3-Button;4-Graph;||cellkind\tCheckBoxText|1|||cellstrings\tExclusiveCheck|3|$bool|0x00000004|cellstyles\tComboCount|2|1|1011|entryCombo|Edit Combo Contents and display the number of contents.\tButtonType|3|0-Text;1-Bmp;2-Both;|0x00000008;0x00000010|cellstyles\tButtonText|1|0256||celldisplays\tButtonImage|2||1003|cellimages\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||cellkeyParam\tLength|1|||cellsize\tWidth|1|||cellwidth\tHead|1||1|cellheadText\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignmentHead\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignmentText\tImageAlignment|3|0-Center;5-Stretch;6-Tile;||cellalignmentImage\tHeadBackColor|2||1002|cellheadPaintColor\tHeadForeColor|2||1002|cellheadTextColor\tDataBackColor|2||1002|cellpaintColor\tDataForeColor|2||1002|celltextColor\tHintType|3|$typeHint||celltypeHint\tHintText|1|||cellhintText\tDomino|1|||celldominoName\tReference|1|||cellreferenceName\tVisible|3|$bool|0x00000002|cellstyles\tSort|3|$bool|0x00000020|cellstyles\tShowFormat|3|$bool|0x00000040|cellstyles|Choose format option.\tFloatDigit|1|||celloffsets\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||cellauxAttributes\tIgnoreFloating0|3|$bool|0x00010000|cellattributes\tColumnSuppress|3|$bool|0x00000080|cellstyles\tEdit|4||1007|xxx\tAttributes|4||1004|xxx\tSubTotal|3|$bool|0x00008000|cellstyles\tTotal|3|$bool|0x00800000|cellstyles\tformula|1|||cellExFormula");
	m_tableInfo = _T("Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||cellcharType\tLength|1|||cellsize\tHead|1||1|cellheadText\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignmentHead\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignmentText\tHeadBackColor|2||1002|cellheadPaintColor\tHeadForeColor|2||1002|cellheadTextColor\tDataBackColor|2||1002|cellpaintColor\tDataForeColor|2||1002|celltextColor\tHintType|3|$typeHint||celltypeHint\tHintText|1|||cellhintText\tFloatDigit|1|||celloffsets\tDomino|1|||celldominoName\tReference|1|||cellreferenceName\tVisible|3|$bool|0x00000002|cellstyles\tEdit|4||1007|xxx\tAttributes|4||1004|xxx");
	m_multiLine = false;
	m_kind = fmGRID;
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

	//CFormItem formItem(m_mapH);
	//CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	//CfmObject*	Obj = ObjLoad->getObject();
	CformProp*	Prop = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
	int	nRows, nWidth = 90, nColCnt = 7;
	CRect	rc;

	m_kind = Prop->kind;
	switch (m_kind)
	{
	case fmGRID:
		nRows = m_gridInfo.Replace("\t", "\t");
		rc.SetRect(0, 30, nWidth * nColCnt + 80, nRows * 17 + 48);
		break;
	case fmGRIDEX:
		nRows = m_gridExInfo.Replace("\t", "\t");
		rc.SetRect(0, 30, nWidth * nColCnt + 80, nRows * 17 + 48);
		break;
	case fmTABLE:
		nRows = m_tableInfo.Replace("\t", "\t");
		rc.SetRect(0, 30, nWidth * nColCnt + 80, nRows * 17 + 63);
		break;
	default:
		break;
	}

	m_propGrid.SetCellSize(nWidth, 16);
	if (!m_propGrid.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, rc,  this, 110))
		PostMessage(WM_CLOSE);

	switch (m_kind)
	{
	case fmGRID:
	case fmGRIDEX:
		SetWindowPos(NULL, 0, 0, nWidth * nColCnt + 90, nRows * 17 + 76, SWP_NOZORDER | SWP_NOMOVE);
		break;
	case fmTABLE:
		SetWindowPos(NULL, 0, 0, nWidth * nColCnt + 90, nRows * 17 + 91, SWP_NOZORDER | SWP_NOMOVE);
		break;
	default:
		break;
	}

	m_propGrid.Initialize(m_mapH);
	m_propGrid.SetIndex(m_index);
	m_propGrid.ResetGrid();
	
	switch (m_kind)
	{
	case fmGRID:
		m_propGrid.SetHeadInfo(102, m_gridInfo);	// grid cell
		m_propGrid.SetHeadInfo(102);
		break;
	case fmGRIDEX:
		m_propGrid.SetHeadInfo(102, m_gridExInfo);	// gridEx cell
		m_propGrid.SetHeadInfo(102);
		break;
	case fmTABLE:
		m_propGrid.SetHeadInfo(103, m_tableInfo);	// table row
		m_propGrid.SetHeadInfo(103);
		break;
	default:
		break;
	}
	
	m_propGrid.LoadData(m_index, -1);
	m_propGrid.Invalidate();

	switch (m_kind)
	{
	case fmGRID:
	case fmGRIDEX:
		if (Prop->auxAttributes & gaMERGE)
		{
			Prop->auxAttributes &= ~gaMULTI;
			m_multiLine = false;
			GetDlgItem(IDC_MULTILINE)->EnableWindow(FALSE);
		}
		else
		{
			if (Prop->auxAttributes & gaMULTI)
				m_multiLine = true;
			else
				m_multiLine = false;
		}
		break;
	case fmTABLE:
		GetDlgItem(IDC_MULTILINE)->ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}

	UpdateData(FALSE);
	if (m_kind == fmTABLE)
		SetWindowText(_T("Table Row Edit Dialog"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGridColumn::OnOK()
{
	CformProp*	Prop = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
	m_propGrid.EraseCtrls();
	if ((m_kind == fmGRID || m_kind == fmGRIDEX) && Prop->auxAttributes & gaMULTI)
	{
		if (Prop->cellProperties != NULL)
		{
			int	line, colN;
			line = colN = 0;
			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				colN++;
				((cellOb*)Prop->cellProperties->GetAt(ii))->lineN = (BYTE)line;
				if (((cellOb*)Prop->cellProperties->GetAt(ii))->attributes & faWRAP)
					line++;
			}
		}
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
//	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, 1);
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
	if (m_kind != fmGRID && m_kind != fmGRIDEX)
		return;

	UpdateData();
	if (m_multiLine)
		((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->auxAttributes |= gaMULTI;
	else
		((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->auxAttributes &= ~gaMULTI;
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
}
