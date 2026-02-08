// MultiFODialog.cpp : implementation file
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "MultiFODialog.h"

#define DEFAULT_JQTY ("1")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHK_MARK _T("∨")
#define DEL_COLUMN	"|"
#define DEL_ROW		"#"

#define STANDARDNUM 11
/////////////////////////////////////////////////////////////////////////////
// CMultiFODialog dialog


CMultiFODialog::CMultiFODialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiFODialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiFODialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMultiFODialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiFODialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiFODialog, CDialog)
	//{{AFX_MSG_MAP(CMultiFODialog)
	ON_BN_CLICKED(IDC_RADIO_MON1, OnRadioMon1)
	ON_BN_CLICKED(IDC_RADIO_MON2, OnRadioMon2)
	ON_BN_CLICKED(IDC_RADIO_MON3, OnRadioMon3)
	ON_BN_CLICKED(IDC_RADIO_MON4, OnRadioMon4)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVNM_LMOUSEUP,	IDC_FGRID, OnFGridLeftUp)
	ON_NOTIFY(GVNM_ENDEDIT,		IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_SELCHANGED,	IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_ENDRETURN,	IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_ENDTABKEY,	IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_LMOUSEUP,	IDC_OGRID, OnOGridLeftUp)
	ON_NOTIFY(GVNM_ENDEDIT,		IDC_OGRID, OnOGridCellChanged)
	ON_NOTIFY(GVNM_SELCHANGED,	IDC_OGRID, OnOGridCellChanged)
	ON_NOTIFY(GVNM_ENDRETURN,	IDC_OGRID, OnOGridCellChanged)
	ON_NOTIFY(GVNM_ENDTABKEY,	IDC_OGRID, OnOGridCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiFODialog message handlers

BOOL CMultiFODialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	/* Load Data */
	loadFJCode();
	loadOJCode();

	/* Radio-button setting */
	GetDlgItem(IDC_RADIO_MON1)->SetWindowText((LPCSTR)(m_month[3].Mid(2,2)+"월물"));
	GetDlgItem(IDC_RADIO_MON2)->SetWindowText((LPCSTR)(m_month[2].Mid(2,2)+"월물"));
	GetDlgItem(IDC_RADIO_MON3)->SetWindowText((LPCSTR)(m_month[1].Mid(2,2)+"월물"));
	GetDlgItem(IDC_RADIO_MON4)->SetWindowText((LPCSTR)(m_month[0].Mid(2,2)+"월물"));

	/* Future grid setting */
	InitFGrid();

	/* Option grid setting */
	InitOGrid(0);

	((CButton*)GetDlgItem(IDC_RADIO_MON1))->SetCheck(TRUE);
	return TRUE;
}

void CMultiFODialog::InitFGrid()
{
	CRect stRect, gridRect;

	GetDlgItem(IDC_STATIC_F)->GetWindowRect(stRect);
	ScreenToClient(stRect);

	//MapDialogRect(&stRect);
	
	gridRect.top	= stRect.bottom;
	gridRect.bottom	= stRect.top + 120;
	gridRect.left	= stRect.left;
	gridRect.right	= stRect.right;
	m_fgrid.Create(gridRect, this, IDC_FGRID, GVSC_NONE, GVDD_NONE);
	
	GVITEM item;
	LOGFONT lf;
	int col, row, iColumns = 6;
	COLORREF defBgCol = RGB(0xE3,0xE3,0xE3);
	COLORREF defFgCol = RGB(0x00,0x00,0x00);
	COLORREF colWhite = RGB(0xFF,0xFF,0xFF);
	COLORREF defBlue  = RGB(0x02,0x01,0xFE);
	COLORREF defRed   = RGB(0xFF,0x00,0x00);
	
	m_fgrid.Initial(5, iColumns, 1, 0);
	
	GetFont()->GetLogFont(&lf);
	lf.lfHeight = 90;
	int cWidth[] = {30, 57, 120, 40, 40, 60};
	int cAttr[] = { GVAT_CHECK, GVAT_TEXT, GVAT_TEXT, GVAT_TEXT, GVAT_TEXT, GVAT_EDIT|GVAT_COMMA};
	int cFormat[] = { GVFM_CENTER, GVFM_LEFT, GVFM_LEFT, GVFM_CENTER, GVFM_CENTER, GVFM_RIGHT};
	char *cCaption[] = { "선택", "코드", "종목명", "매도", "매수", "수량"};
	COLORREF cBkColors[] = { defBgCol, defBgCol, defBgCol, defBlue, defRed, defBgCol };
	COLORREF cFgColors[] = { defFgCol, defFgCol, defFgCol, colWhite, colWhite, defFgCol };
	
	// Header Setting
	for(col=0; col<iColumns; ++col)
	{
		item.state = 0;
		item.row = 0;
		item.col = col;
		item.state = 0;
		item.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		item.format = GVFM_HEADER;
		item.attr = 0;
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		item.fgcol = cFgColors[col];
		item.bkcol = cBkColors[col];
		item.text = cCaption[col];	
		m_fgrid.SetColumnWidth(col, cWidth[col]);
		m_fgrid.SetItem(&item);
	}
	
	// Column Setting
	cFgColors[3] = defBlue; cFgColors[4] = defRed;
	for(col=0; col<iColumns; ++col)
	{
		item.state = 0;
		item.mask = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = cFormat[col];
		item.attr = cAttr[col];
		item.fgcol = cFgColors[col];
		item.bkcol = colWhite;
		m_fgrid.SetBKSelColor(colWhite);
		m_fgrid.SetFGSelColor(defFgCol);
		m_fgrid.SetColumnItems(col, &item);
	}
	
	// Data Setting
	for(row=0; row<m_fgrid.GetRowCount(); ++row)
	{
		m_fgrid.SetRowHeight(row, 20);
		if (row>0)
		{
			m_fgrid.SetItemText(row, 1, m_fjcodes[row-1].code);
			m_fgrid.SetItemText(row, 2, m_fjcodes[row-1].name);
			m_fgrid.SetItemText(row, 3, CHK_MARK);
		}
	}
	
	m_fgrid.SetRowColResize(FALSE, FALSE);
	m_fgrid.SetGridFocusLine(FALSE, FALSE);
	m_fgrid.Adjust();
}

int CMultiFODialog::GetOptionCount(int midx)
{
	int cnt = 0;
	for(size_t i=0; i<m_ojcodes.size(); ++i)
	{
		cnt += (m_ojcodes[i].put[midx].yorn=='1') ? 1 : 0;
	}
	return cnt;
}

void CMultiFODialog::InitOGrid(int midx)
{
	if (!::IsWindow(m_ogrid.GetSafeHwnd()))
	{
		CRect stRect, stRect2, dlgRect, gridRect;
		GetClientRect(dlgRect);
		GetDlgItem(IDC_STATIC_O)->GetWindowRect(stRect);
		ScreenToClient(stRect);
		GetDlgItem(IDC_STATIC_MON)->GetWindowRect(stRect2);
		ScreenToClient(stRect2);
		
		gridRect.top	= stRect2.bottom + 28;
		gridRect.bottom	= dlgRect.bottom - 40;
		gridRect.left	= stRect.left;
		gridRect.right	= stRect.right;
		m_ogrid.Create(gridRect, this, IDC_OGRID, GVSC_VERT, GVDD_NONE);
	}
	else
	{
		m_ogrid.Clear();
	}
	
	GVITEM item;
	LOGFONT lf;
	int ii, col, row, gidx, iColumns = 11;
	COLORREF defBgCol = RGB(0xE3,0xE3,0xE3);
	COLORREF defFgCol = RGB(0x00,0x00,0x00);
	COLORREF colWhite = RGB(0xFF,0xFF,0xFF);
	COLORREF defBlue  = RGB(0x02,0x01,0xFE);
	COLORREF defRed   = RGB(0xFF,0x00,0x00);
		
	m_ogrid.Initial(GetOptionCount(midx)+1, iColumns, 1, 0);
	
	GetFont()->GetLogFont(&lf);
	lf.lfHeight = 90;

	int cWidth[] = {30, 55, 30, 30, 0, 45, 0, 30, 30, 55, 30};
	int cAttr[] = {	GVAT_CHECK, GVAT_EDIT|GVAT_COMMA, GVAT_TEXT, GVAT_TEXT,		// left
					GVAT_HIDDEN|GVAT_TEXT, GVAT_TEXT, GVAT_HIDDEN|GVAT_TEXT,	// code, amt, code
					GVAT_TEXT, GVAT_TEXT, GVAT_EDIT|GVAT_COMMA, GVAT_CHECK};	// right
	int cFormat[] = {	GVFM_CENTER, GVFM_RIGHT, GVFM_CENTER, GVFM_CENTER,
						GVFM_CENTER, GVFM_CENTER, GVFM_CENTER,
						GVFM_CENTER, GVFM_CENTER, GVFM_RIGHT, GVFM_CENTER };
	char *cCaption[] = {"선택", "수량", "매도", "매수", "종목코드", "행사가", "종목코드", "매수", "매도", "수량", "선택"};
	COLORREF cBkColors[] = { defBgCol, defBgCol, defBlue, defRed, defBgCol, defBgCol, defBgCol, defRed, defBlue, defBgCol, defBgCol };
	COLORREF cFgColors[] = { defFgCol, defFgCol, colWhite, colWhite, defFgCol, defFgCol, defFgCol, colWhite, colWhite, defFgCol, defFgCol };
	COLORREF cAtmColor = RGB(0xB4,0xFF,0xFF);

	// Header Setting
	for(col=0; col<iColumns; ++col)
	{
		item.state = 0;
		item.row = 0;
		item.col = col;
		item.state = 0;
		item.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		item.format = GVFM_HEADER;
		item.attr = 0;
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		item.fgcol = cFgColors[col];
		item.bkcol = cBkColors[col];
		item.text = cCaption[col];	
		m_ogrid.SetColumnWidth(col, cWidth[col]);
		m_ogrid.SetItem(&item);
	}
	
	// Column Setting
	cFgColors[2] = cFgColors[8] = defBlue;
	cFgColors[3] = cFgColors[7] = defRed;
	cFgColors[4] = cFgColors[6] = colWhite;
	cFgColors[5] = defFgCol;
	for(col=0; col<iColumns; ++col)
	{
		item.state = 0;
		item.mask = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = cFormat[col];
		item.attr = cAttr[col];
		item.fgcol = cFgColors[col];
		if (col!=5)
			item.bkcol = colWhite;
		else
			item.bkcol = cAtmColor;
		m_ogrid.SetBKSelColor(colWhite);
		m_ogrid.SetFGSelColor(defFgCol);
		m_ogrid.SetColumnItems(col, &item);
	}
	
	// Data Setting
	for(row=0; row<m_ogrid.GetRowCount(); ++row)
		m_ogrid.SetRowHeight(row, 20);

	for(ii=0, gidx=1; ii<m_ojcodes.size(); ++ii)
	{
		if (m_ojcodes[ii].put[midx].yorn=='1')
		{
			CString code;
			CString price = CString(m_ojcodes[ii].price, OPriceLen);
			price.Insert(3,".");

			code = CString(m_ojcodes[ii].call[STANDARDNUM - midx -1].cod2, OCodeLen -1); code.TrimRight();
			m_ogrid.SetItemText(gidx, 4, code);
			m_ogrid.SetItemText(gidx, 5, (LPCTSTR)price);
			code = CString(m_ojcodes[ii].put[midx].cod2, OCodeLen -1); code.TrimRight();
			m_ogrid.SetItemText(gidx, 6, code);
			m_ogrid.SetItemText(gidx, 2, CHK_MARK);
			m_ogrid.SetItemText(gidx, 8, CHK_MARK);

			if (m_ojcodes[ii].atmg==1)
			{
				m_ogrid.SetItemBkColor(gidx, 5, RGB(0xFF,0xFF,0xB4), true);
				m_ogrid.SetSelectRow(gidx+4);
			}
			else
				m_ogrid.SetItemBkColor(gidx, 5, defBgCol, true);
			++gidx;
		}
	}
	
	m_ogrid.SetRowColResize(FALSE, FALSE);
	m_ogrid.SetGridFocusLine(FALSE, FALSE);
	m_ogrid.Adjust();
}

void CMultiFODialog::loadOJCode()
{
	m_month.clear();

	int	codeN, len;
	CFile	file;
	CString path;
	struct ojcodh OJCodh;
	struct ojcode OJCode;
	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\opcode.dat";
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}
	len = file.Read(&OJCodh, sizeof(struct ojcodh));
	
	for (int jj = 0; jj < STANDARDNUM; jj++)
	{
		m_month.push_back( CString(OJCodh.cjym[jj], 6).Right(4) );
	}

	codeN = (file.GetLength() - len) / sizeof(struct ojcode);	
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));
		m_ojcodes.push_back(OJCode);
	}
	std::reverse(m_ojcodes.begin(), m_ojcodes.end());
	file.Close();
}

void CMultiFODialog::loadFJCode()
{
	m_fjcodes.clear();

	int	codeN;
	CFile	file;
	CString path;
	struct  fjcode  FJCode;
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\fjcode.dat";
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}

	codeN = file.GetLength() / sizeof(struct fjcode);
	for (int ii = 0; ii < codeN; ii++)
	{
		FCODE fcode;
		file.Read(&FJCode, sizeof(struct fjcode));
		fcode.code = CString(FJCode.cod2, FCodeLen);
		fcode.name = CString(FJCode.hnam, FNameLen);
		fcode.mchk = FJCode.mchk;
		m_fjcodes.push_back(fcode);
	}

	file.Close();
}

void CMultiFODialog::OnFGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;

	int row;
	if (item->row==0)	// Header Click
	{
		switch(item->col)
		{
		case 3:
			for(row=1; row<m_fgrid.GetRowCount(); ++row)
			{
				m_fgrid.SetItemText(row, 3, CHK_MARK);
				m_fgrid.SetItemText(row, 4, _T(""));
			}
			break;
		case 4:
			for(row=1; row<m_fgrid.GetRowCount(); ++row)
			{
				m_fgrid.SetItemText(row, 3, _T(""));
				m_fgrid.SetItemText(row, 4, CHK_MARK);
			}
			break;
		}
	}
	else if (item->row>0)
	{
		switch(item->col)
		{
		case 3:
			m_fgrid.SetItemText(item->row, 3, CHK_MARK);
			m_fgrid.SetItemText(item->row, 4, _T(""));
			break;
		case 4:
			m_fgrid.SetItemText(item->row, 3, _T(""));
			m_fgrid.SetItemText(item->row, 4, CHK_MARK);
			break;
		case 5:	// 수량 선택
			m_fgrid.SetFocusCellEdit(item->row, item->col, true);
			break;
		}
	}
	//*pResult = 0;
}

void CMultiFODialog::OnFGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;
	
	TRACE("CellChanged : col[%d] row[%d]\n", item->col, item->row);

	if (item->row>0 && item->col==5)
	{
		CString val = m_fgrid.GetItemText(item->row, item->col);
		int ival = atoi((LPCSTR)val);
		if (ival>0)
			m_fgrid.SetItemParam(item->row, 0, 1);
		else
			m_fgrid.SetItemText(item->row, item->col, "");
	}
	//*pResult = 0;
}

void CMultiFODialog::OnOGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;

	int row;
	if (item->row==0)	// Header Click
	{
		switch(item->col)
		{
		case 2:
			for(row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 2, CHK_MARK);
				m_ogrid.SetItemText(row, 3, _T(""));
			}
			break;
		case 3:
			for(row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 2, _T(""));
				m_ogrid.SetItemText(row, 3, CHK_MARK);
			}
			break;
		case 7:
			for(row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 7, CHK_MARK);
				m_ogrid.SetItemText(row, 8, _T(""));
			}
			break;
		case 8:
			for(row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 7, _T(""));
				m_ogrid.SetItemText(row, 8, CHK_MARK);
			}
			break;
		}
	}
	else if (item->row>0)
	{
		switch(item->col)
		{
		case 2:
			m_ogrid.SetItemText(item->row, 2, CHK_MARK);
			m_ogrid.SetItemText(item->row, 3, _T(""));
			break;
		case 3:
			m_ogrid.SetItemText(item->row, 2, _T(""));
			m_ogrid.SetItemText(item->row, 3, CHK_MARK);
			break;
		case 7:
			m_ogrid.SetItemText(item->row, 7, CHK_MARK);
			m_ogrid.SetItemText(item->row, 8, _T(""));
			break;
		case 8:
			m_ogrid.SetItemText(item->row, 7, _T(""));
			m_ogrid.SetItemText(item->row, 8, CHK_MARK);
			break;
		case 1:	// 수량 선택
		case 9:
			m_ogrid.SetFocusCellEdit(item->row, item->col, true);
			break;
		}
	}
	*pResult = 0;
}

void CMultiFODialog::OnOGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;
	
	TRACE("CellChanged : col[%d] row[%d]\n", item->col, item->row);

	if (item->row>0 && (item->col==1 || item->col==9))
	{
		CString val = m_ogrid.GetItemText(item->row, item->col);
		int ival = atoi((LPCSTR)val);
		if (ival>0)
		{
			switch (item->col)
			{
			case 1: m_ogrid.SetItemParam(item->row, 0, 1); break;
			case 9: m_ogrid.SetItemParam(item->row, 10, 1); break;
			}
		}
		else
			m_ogrid.SetItemText(item->row, item->col, "");
	}
	*pResult = 0;
}
void CMultiFODialog::OnRadioMon1() 
{
	InitOGrid(0);
}

void CMultiFODialog::OnRadioMon2() 
{
	InitOGrid(1);
}

void CMultiFODialog::OnRadioMon3() 
{
	InitOGrid(2);
}

void CMultiFODialog::OnRadioMon4() 
{
	InitOGrid(3);
}

void CMultiFODialog::OnOK()
{
	CDialog::OnOK();
}

void CMultiFODialog::OnCancel()
{
	CDialog::OnCancel();
}

CString CMultiFODialog::GetData()
{
	int row;
	CString data = "";

	for(row=1; row<m_fgrid.GetRowCount(); ++row)
	{
		if (m_fgrid.GetItemParam(row, 0)==1)	// check box
		{
			if (m_fgrid.IsNumeric(m_fgrid.GetItemText(row, 5)))	// jqty edit
			{
				data += m_fgrid.GetItemText(row, 1) + DEL_COLUMN;
				data += (m_fgrid.GetItemText(row, 4)==CHK_MARK) ? "2"DEL_COLUMN : "1"DEL_COLUMN;
				data += DEL_COLUMN;
				data += m_fgrid.GetItemText(row, 5) + DEL_ROW;
			}
		}
	}
	for(row=1; row<m_ogrid.GetRowCount(); ++row)
	{
		// call
		if (m_ogrid.GetItemParam(row, 0)==1)	// check box
		{
			if (m_ogrid.IsNumeric(m_ogrid.GetItemText(row, 1)))	// jqty edit
			{
				data += m_ogrid.GetItemText(row, 4) + DEL_COLUMN;
				data += (m_ogrid.GetItemText(row, 2)==CHK_MARK) ? "1"DEL_COLUMN : "2"DEL_COLUMN;
				data += m_ogrid.GetItemText(row, 1) + DEL_ROW;
			}
		}
		// put
		if (m_ogrid.GetItemParam(row, 10)==1)	// check box
		{
			if (m_ogrid.IsNumeric(m_ogrid.GetItemText(row, 9)))	// jqty edit
			{
				data += m_ogrid.GetItemText(row, 6) + DEL_COLUMN;
				data += (m_ogrid.GetItemText(row, 7)==CHK_MARK) ? "2"DEL_COLUMN : "1"DEL_COLUMN;
				data += m_ogrid.GetItemText(row, 9) + DEL_ROW;
			}
		}
	}
	return data;
}

int CMultiFODialog::GetSelectCount()
{
	int row, cnt=0;
	for(row=1; row<m_fgrid.GetRowCount(); ++row)
		if (m_fgrid.GetItemParam(row, 0)==1)	// check box
			++cnt;

	for(row=1; row<m_ogrid.GetRowCount(); ++row)
	{
		// call
		if (m_ogrid.GetItemParam(row, 0)==1)	// check box
			++cnt;
		// put
		if (m_ogrid.GetItemParam(row, 10)==1)	// check box
			++cnt;
	}
	return cnt;
}