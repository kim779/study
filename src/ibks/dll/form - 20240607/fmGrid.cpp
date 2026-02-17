// fmGrid.cpp : implementation file
//

#include "stdafx.h"
#include "fmGrid.h"
#include "axform.h"
#include "grid.h"
#include "cblist.h"
#include "image.h"

#include "../../h/axis.h"
#include "../../h/axiserr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CrgbArr {
public:
	CArray	<COLORREF, COLORREF &>	tRGBs;
	CArray	<COLORREF, COLORREF &>	pRGBs;
	CArray	<int, int &>		itRGBs;
};


/////////////////////////////////////////////////////////////////////////////
// CfmGrid

IMPLEMENT_DYNCREATE(CfmGrid, CfmBase)

CfmGrid::CfmGrid() 
{
	EnableAutomation();
}

CfmGrid::CfmGrid(CAxisForm* axform, struct _formR* formR, int iform, struct _cellR* cellR)
	: CfmBase(axform, formR, iform)
{
	if (m_form->size == NOVALUE)
		m_form->size = 0;

	m_tRGB          = getRGB(cBLACK);
	m_stRow         = 0;
	m_totCols       = 0;
	m_totRows       = 0;
	m_showRows      = 0;
	m_save          = svExchange;
	m_totWidth      = 0;
	m_scrolledWidth = 0;
	m_fixedWidth    = 0;
	m_focus         = false;
	m_writingAll    = false;
	m_slit  = 0;
	m_headN = 0;
	m_inits = _T("");

	m_validRows    = 0;
	m_hscroll      = false;
	m_vscroll      = false;
	m_enableScroll = false;
	m_input        = false;
	m_portfolio    = -1;
	m_pic          = NULL;
	m_epic         = NULL;
	m_checks.RemoveAll();

	if (m_form->attr2 & GO_MERGE)
		m_form->attr2 &= ~GO_MULTI;

	m_list = NULL;
	m_columns.RemoveAll();
	if (cellR == NULL)
		return;

	char*	sP;
	int	offs,  multi = 0;
	int	ii, jj, flash = -1;
	CString	tmps  = _T("");
	Ccolumn* column;
	bool	check = false;
	bool	combo = false;

	m_cellR = cellR;
	if (m_form->attr2 & GO_ROW && m_form->vals[3] != NOVALUE)
	{
		offs = m_form->vals[3];
		m_fRGB = (m_axform->m_valueR[offs]   & userCOLOR) ? m_axform->m_valueR[offs]   : getRGB(m_axform->m_valueR[offs]);
		m_sRGB = (m_axform->m_valueR[offs+1] & userCOLOR) ? m_axform->m_valueR[offs+1] : getRGB(m_axform->m_valueR[offs+1]);
	}
	else
	{
		m_fRGB = getRGB(cBASIC);
		m_sRGB = getRGB(cBASIC);
	}
	if (m_form->isEdit == FI_RREC && !(m_form->attr2 & GO_MULTI))
		m_form->isEdit = FI_RBAR;
	if ((m_form->isEdit == FI_RBAR || m_form->isEdit == FI_RREC) && m_form->onEdit != NOVALUE)
		m_rRGB = (m_form->onEdit & userCOLOR) ? m_form->onEdit : getRGB(m_form->onEdit);
	else
		m_rRGB = getRGB(cDBLUE);

	m_totCols = HIWORD(m_form->vals[1]);
	m_totRows = LOWORD(m_form->vals[1]);
	m_form->type = min(m_form->type, m_totCols);
	if (!(m_form->attr2 & GO_FLEX))
		m_validRows = m_totRows;
	if (m_form->attr2 & GO_VSCR)
		m_vscroll = true;
	if (m_form->attr2 & GO_HSCR && m_totCols != 1)
		m_hscroll = true;
	if (!(m_form->attr2 & GO_XHLINE))
		m_slit  = 1;
	m_index = 0;

	m_form->iok    = EIO_NOP;
	m_form->isHint = IH_NOP;
	for (ii = 0; ii < m_totCols; ii++)
	{
		column = new Ccolumn;
		
		column->m_line   = multi;
		column->m_widthx = m_cellR[ii].width;
		column->m_width  = (float) column->m_widthx;
		if (m_cellR[ii].head != NOVALUE)
		{
			column->m_head = (char *)m_cellR[ii].head;
			if (m_form->attr2 & GO_MERGE)
			{
				tmps = column->m_head;
				for ( ; !tmps.IsEmpty(); )
				{
					offs = tmps.Find('|');
					if (offs != -1)
					{
						column->m_heads.Add(tmps.Left(offs));
						tmps = tmps.Mid(offs+1);
					}
					else
					{
						column->m_heads.Add(tmps);
						tmps.Empty();
					}
				}
			}
		}

		if (m_cellR[ii].name != NOVALUE)
		{
			if (m_portfolio < 0 && m_axform->DoSomething(doPORTFOLIO, this, 0, (LPARAM)(char *)m_cellR[ii].name))
				m_portfolio = ii;

			char*	rts;
			
			rts  = (char *)m_cellR[ii].name;
			offs = strlen(rts);
			if (offs >= offsRTS)
				rts += (offs - offsRTS);
			column->m_rts = atoi(rts);
			if (column->m_rts == 0)
			{
				offs = strlen(rts);
				for (int ii = 0; ii < offs; ii++)
				{
// updateXXX_2022XX
					if (!isdigit(static_cast<unsigned char>(*rts)))
					{
						column->m_rts = -1;
						break;
					}
				}
			}
		}

		column->m_pRGB = (m_cellR[ii].hpRGB & userCOLOR) ? m_cellR[ii].hpRGB : getRGB(m_cellR[ii].hpRGB);
		column->m_tRGB = (m_cellR[ii].htRGB & userCOLOR) ? m_cellR[ii].htRGB : getRGB(m_cellR[ii].htRGB);

		if ((m_form->attr2 & GO_MULTI) && (m_cellR[ii].attr & FA_WRAP))
			multi++;
		if (m_cellR[ii].attr & FA_RIGHT)
			m_cellR[ii].alignC = AL_RIGHT;
		if (m_cellR[ii].isHint != IH_NOP)
		{
			m_form->isHint = IH_STRING;
			m_form->onHint = IH_STRING;
		}
		if (m_cellR[ii].properties & PR_SORT)
			m_cellR[ii].properties &= ~PR_MARK;

		if (!check && (m_cellR[ii].kind == CK_CHECK || m_cellR[ii].hkind == CK_CHECK))
			check = true;
		if (!combo && (m_cellR[ii].kind == CK_COMBO || m_cellR[ii].hkind == CK_COMBO))
			combo = true;

		switch (m_cellR[ii].hkind)
		{
		case CK_COMBO:
			char*	data;
			char*	disp;
			data = &m_axform->m_strR[m_cellR[ii].vals[3]];
			disp = &m_axform->m_strR[m_cellR[ii].vals[4]];
			for (jj = 0; jj < (int)m_cellR[ii].vals[5]; jj++)
			{
				column->m_hdisplay.Add(disp);
				disp += strlen(disp);
				disp++;
				column->m_hdata.Add(data);
				data += strlen(data);
				data++;
			}
			break;

		case CK_BUTTON:
			if (m_cellR[ii].properties & (PR_HIMAGE|PR_HIMAGETEXT))
			{
				if (m_cellR[ii].vals[4] != NOVALUE)
				{
					tmps.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, &m_axform->m_strR[m_cellR[ii].vals[4]]);
					column->m_hpic = new Cimage(m_axform->m_draw, tmps);
					if (m_cellR[ii].properties & PR_HIMAGE)
					{
						column->m_head.Empty();
						column->m_heads.RemoveAll();
					}
				}
				else	m_cellR[ii].properties &= ~(PR_HIMAGE|PR_HIMAGETEXT);
			}
			break;
		}

		switch (m_cellR[ii].kind)
		{
		case CK_CHECK:
			if (m_cellR[ii].vals[0] != NOVALUE)
				column->m_edit = (char *)&m_axform->m_strR[m_cellR[ii].vals[0]];
			break;

		case CK_COMBO:
			char*	data;
			char*	disp;
			data = &m_axform->m_strR[m_cellR[ii].vals[0]];
			disp = &m_axform->m_strR[m_cellR[ii].vals[1]];
			for (jj = 0; jj < (int)m_cellR[ii].keys; jj++)
			{
				column->m_display.Add(disp);
				disp += strlen(disp);
				disp++;
				column->m_data.Add(data);
				data += strlen(data);
				data++;
			}
			break;

		case CK_BUTTON:
			if (m_cellR[ii].vals[0] != NOVALUE)
				column->m_edit = (char *)&m_axform->m_strR[m_cellR[ii].vals[0]];
			if (m_cellR[ii].properties & (PR_IMAGE|PR_IMAGETEXT))
			{
				if (!(m_cellR[ii].properties & PR_FLEXCB) && m_cellR[ii].vals[1] != NOVALUE)
				{
					tmps.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, &m_axform->m_strR[m_cellR[ii].vals[1]]);
					column->m_pic = new Cimage(m_axform->m_draw, tmps);
				//	if (m_cellR[ii].properties & PR_IMAGETEXT)
				//		m_cellR[ii].properties |= PR_TRANSPARENT;
				}
			}
			break;

		case CK_GRAPH:
			break;

		default:
			switch (m_cellR[ii].isEdit)
			{
			case IE_EDIT:
				if (m_cellR[ii].onEdit != NOVALUE)
					column->m_edit = (char *)&m_axform->m_strR[m_cellR[ii].onEdit];
				break;
			case IE_CORGB:
				m_cellR[ii].isEdit = IE_NOP;
				if (m_cellR[ii].attr & FA_CORGB && m_cellR[ii].onEdit != NOVALUE)
					m_cellR[ii].isEdit = IE_CORGB;
				break;
			}
			break;
		}

		switch (m_cellR[ii].iok)
		{
		case EIO_INPUT:
			switch (m_form->iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				m_form->iok = EIO_INOUT;	break;
			default:
				m_form->iok = EIO_INPUT;	break;
			}
			m_input = true;
			break;

		case EIO_INOUT:
			m_form->iok = EIO_INOUT;
			m_input = true;
			break;

		case EIO_OUTPUT:
		default:
			switch (m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				m_form->iok = EIO_INOUT;	break;
			case EIO_OUTPUT:
			default:
				m_form->iok = EIO_OUTPUT;	break;
			}
			break;
		}

		if (m_cellR[ii].attr & FA_FLASH)
			flash = ii;

		if (m_cellR[ii].attr & FA_DRAG)
			m_form->attr |= FA_DRAG;

		sP = (char*)column->m_head.operator LPCTSTR();
		if (!(m_form->attr2 & GO_MERGE))
		{
			for (int jj = 0; jj < column->m_head.GetLength(); jj++,sP++)
			{
				if (*sP == '~')
					column->m_ihead++;
			}
		}
		column->m_ref     = ii;
		column->m_visible = (m_cellR[ii].properties & PR_VISIBLE) ? true : false;

		m_columns.Add(column);
	}

	if (m_form->attr2 & GO_EXPAND)
		m_validRows = m_totRows;

	if (flash != -1)
	{
		m_form->attr   |= FA_FLASH;
		m_form->checked = flash;
	}
	if (check)
		loadBitmap(CK_CHECK);
	if (combo)
		loadBitmap(CK_COMBO);
	if (m_form->attr & FA_ENUM)
		loadBitmap(-1);
	loadBitmap(-2);

	m_enCol = m_totCols - 1;
	m_headN  = 0;
	m_multiN = 0;

	float	tWidth = 0;
	COLORREF rgb = 0;
	for (ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);

		if (m_form->attr2 & GO_MERGE)
			m_headN = max(m_headN, column->m_heads.GetSize());
		else
		{
			m_headN  = max(m_headN, (int)column->m_ihead);
			m_multiN = max(m_multiN, column->m_line);
		}

		if (column->m_visible)
		{
			tWidth += column->m_width;
			if (ii < m_form->type)
				m_fixedWidth += column->m_width;
		}
		if (((m_cellR[ii].attr & FA_WRAP) && (m_form->attr2 & GO_MULTI)) || (ii == m_totCols - 1))
		{
			m_totWidth = max(m_totWidth, tWidth);
			tWidth = 0;
		}

		for (int jj = 0; jj < m_totRows; jj++)
		{
			if (m_form->attr2 & GO_ROW)
			{
				if (jj % 2)
					column->m_pRGBs.Add(m_sRGB);
				else
					column->m_pRGBs.Add(m_fRGB);
			}
			else
			{
				rgb = (m_cellR[ii].pRGB  & userCOLOR) ? m_cellR[ii].pRGB  : getRGB(m_cellR[ii].pRGB);
				column->m_pRGBs.Add(rgb);
			}

			if (m_cellR[ii].tRGB & userCOLOR)
			{
				column->m_tRGBs.Add(m_cellR[ii].tRGB);
				column->m_itRGBs.Add(-1);
			}
			else
			{
				column->m_tRGBs.Add(getRGB(m_cellR[ii].tRGB));
				column->m_itRGBs.Add((short)m_cellR[ii].tRGB);
			}

			if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
				column->m_valids.Add(1);
			else
				column->m_valids.Add(0);
			if (m_form->attr2 & GO_TABLE)
			{
				column->m_attrs.Add(m_cellR[ii].attr);
				column->m_enables.Add(1);
			}

			if (m_cellR[ii].attr & FA_FLOAT)
				column->m_floats.Add(0xff);
		}
	}

	if (m_form->attr2 & GO_EXPAND)
		m_form->attr2 &= ~GO_VALID;

	/*
	if (m_form->attr2 & GO_MULTI)
	{
		m_form->type = 0;
		m_fixedWidth = 0;
	}
	*/
	m_totWidth -= m_fixedWidth;

	if (m_form->attr2 & GO_XHEAD)
		m_headN = 0;

	m_stCol = m_form->type;
	for (ii = m_form->type; ii < m_totCols; ii++)
	{
		if (m_columns.GetAt(ii)->m_visible)
		{
			m_stCol = ii;
			break;
		}
	}

	m_multiN++;
	m_hscr.SetRectEmpty();
	m_vscr.SetRectEmpty();
	m_pscr.SetRectEmpty();
	m_vthumb.SetRectEmpty();
	m_hthumb.SetRectEmpty();
	m_editRc.SetRectEmpty();
	m_click  = ckNone;
	m_result = RC_NOP;

	m_move  = mvNone;
	m_moveCol = -1;
	m_delta = 0;
	m_moveRc.SetRectEmpty();
	m_movepos = Cpos(-1, -1);

	m_data.RemoveAll();
	m_edit.RemoveAll();

	m_state = stNone;
	m_statechange = false;
	m_cell.SetRectEmpty();
	m_cbspread   = false;
	m_cbbtn.SetRectEmpty();
	m_edcaret    = false;
	m_edfocusing = true;
	m_edcaretpos = 0;
	m_edstate    = esNone;
	m_edclick    = CPoint(0, 0);

	for (ii = 0; ii < m_totRows; ii++)
	{
		for (int jj = 0; jj < m_totCols; jj++)
		{
			/*
			if (m_cellR[jj].kind == CK_COMBO)
				tmps = getString(jj, ii, 0, false);
			else
				tmps = CString(' ', m_cellR[jj].size);
			*/
			switch (m_cellR[jj].kind)
			{
			case CK_COMBO:
				tmps = getString(jj, ii, 0, false);
				break;
			default:
				switch (m_cellR[jj].iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					tmps = _T("");
					break;
				default:
					tmps = CString(' ', m_cellR[jj].size);
					break;
				}
			}
			m_data.Add(tmps);
			m_edit.Add(editData(tmps, jj, ii));
		}
	}

	CRect	sRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		sRc.DeflateRect(2, 2);

	m_cellH = LOWORD(m_form->keys);
	if (m_cellH > 0)
		m_fixRows  = false;
	else if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		m_fixRows = false;
		m_cellH = (int)(m_fHeight+4);
	}
	else
	{
		m_fixRows  = true;
		m_showRows = HIWORD(m_form->keys);
		if (m_multiN)
			m_showRows *= m_multiN;
		if (m_form->attr2 & GO_MERGE)
			offs = sRc.Height() - (m_showRows+m_headN*m_multiN)*m_slit;
		else
			offs = sRc.Height() - (m_showRows+1)*m_slit;
		if (m_hscroll)
			offs -= szSCROLL;
		m_showRows += m_headN*m_multiN;
		tWidth  = (float)offs/(float)m_showRows;
		m_cellH = (int)tWidth;
	//	if (tWidth - (float)m_cellH > 0.5)
	//		m_cellH++;
	}

	if (m_showRows <= 0)
	{
		m_showRows = (m_totRows + m_headN) * m_multiN;
		if (m_vscroll)
		{
			float	showRows, tHeight = 0;

			if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
				m_enableScroll = true;

			if (m_hscroll)
				sRc.bottom -= szSCROLL;

			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					sRc.top += (4 * m_multiN);
				else
					sRc.top += m_multiN;
			}

			if ((float)sRc.Height() > ((m_headN * m_multiN) * m_cellH))
			{
				showRows = (float)(m_headN * m_multiN);
				if (m_form->attr2 & GO_MERGE)
					tHeight = (float)sRc.Height() - (m_headN*m_multiN)*(m_cellH+m_slit);
				else
					tHeight = (float)sRc.Height() - (m_headN*m_multiN)*m_cellH;
				showRows += (tHeight / (float)(m_cellH + m_slit));
			}
			else
				showRows = (float)sRc.Height() / (float)m_cellH;

			m_showRows = (int)showRows;
			if ( (showRows - (double)m_showRows) > 0.5)
				m_showRows++;

			sRc.right -= szSCROLL;
		}
	}
	else if (m_vscroll)
		sRc.right -= szSCROLL;

	m_showWidth = (float)sRc.Width();
	m_cy = (float)m_cellH;

	m_useInfo = (m_form->attr2 & GO_HEADER) ? true : false;
	if (m_axform->m_mapH->options & OP_OOP)
		m_useInfo = true;
	m_stat  = 0;
	m_page  = _T("");
	m_dir   =  0;
	m_sortc = -1;

	m_setfocus = false;
	m_changed  = false;
	m_curpos   = Cpos(-1, -1);
	for (ii = 0; ii < m_totCols; ii++)
	{
		if (m_columns.GetAt(ii)->m_visible)
		{
			m_curpos.m_row = m_index+1;
			m_curpos.m_col = ii+1;
			break;
		}
	}
	m_cellpos    = m_curpos;
	m_changedpos = Cpos(-1, -1);

	m_select  = false;
	m_lastpos = Cpos(-1, -1);
	m_bottom_gap = 0;
	m_right_gap  = 0;
}

CfmGrid::~CfmGrid()
{
	for (int ii = 0; ii < m_totCols; ii++)
		delete m_columns.GetAt(ii);
	m_columns.RemoveAll();

	if (m_list)
		delete m_list;
	m_data.RemoveAll();
	m_edit.RemoveAll();

	if (m_pic)
		delete m_pic;
	if (m_epic)
		delete m_epic;
	m_checks.RemoveAll();

	int	key;
	Cimage*	image;
	for (POSITION pos = m_scrolls.GetStartPosition(); pos; )
	{
		m_scrolls.GetNextAssoc(pos, key, (Cimage*&)image);
		delete image;
	}
	m_scrolls.RemoveAll();
}


void CfmGrid::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmGrid, CfmBase)
	//{{AFX_MSG_MAP(CfmGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmGrid, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmGrid)
	DISP_PROPERTY_EX(CfmGrid, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CfmGrid, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CfmGrid, "Cell", _getCell, _setCell, VT_BSTR)
	DISP_PROPERTY_EX(CfmGrid, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CfmGrid, "Index", _getIndex, _setIndex, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Row", _getRow, _setRow, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Column", _getColumn, _setColumn, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Head", _getHead, _setHead, VT_BSTR)
	DISP_PROPERTY_EX(CfmGrid, "Rows", _getRows, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Columns", _getColumns, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Pos", _getPos, _setPos, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Page", _getPage, _setPage, VT_BSTR)
	DISP_PROPERTY_EX(CfmGrid, "Direction", _getDirection, _setDirection, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "SortColumn", _getSortColumn, _setSortColumn, VT_BSTR)
	DISP_PROPERTY_EX(CfmGrid, "Action", _getAction, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Info", _getInfo, _setInfo, VT_BSTR)
	DISP_PROPERTY_EX(CfmGrid, "Realtm", _getRealTm, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "HitPos", _getHitPos, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "Focus", _getFocus, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CfmGrid, "RangeColumn1", _getRangeColumn1, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "RangeColumn2", _getRangeColumn2, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "RangeRow1", _getRangeRow1, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "RangeRow2", _getRangeRow2, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CfmGrid, "HeadCell", _getHeadCell, _setHeadCell, VT_BSTR)
	DISP_FUNCTION(CfmGrid, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmGrid, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmGrid, "GetGridCell", _GetCell, VT_BSTR, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetGridCell", _SetCell, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "GetGridText", _GetText, VT_BSTR, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetPRgb", _GetPRgb, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetPRgb", _SetPRgb, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetTRgb", _GetTRgb, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetTRgb", _SetTRgb, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "IsColumnVisible", _IsColumnVisible, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetColumnVisible", _SetColumnVisible, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "IsCellEnable", _IsCellEnable, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetCellEnable", _SetCellEnable, VT_EMPTY, VTS_I4 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "GetHead", _GetHead, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetHead", _SetHead, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "IsColumnSortable", _IsColumnSortable, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetColumnName", _GetColumnName, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetDataPosition", _SetDataPosition, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CfmGrid, "Sort", _Sort, VT_EMPTY, VTS_I4 VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "InsertRow", _InsertRow, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CfmGrid, "RemoveRow", _RemoveRow, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CfmGrid, "Key", _Key, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CfmGrid, "PushGridCell", _Push, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "PopGridCell", _Pop, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "Print", _Print, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmGrid, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmGrid, "SetColumnItems", _SetItems, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "SetCellFocus", _SetCellFocus, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "Import", _Import, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "SetCellAttr", _SetCellAttr, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "Excel", _Excel, VT_EMPTY, VTS_BOOL VTS_BOOL VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "Export", _Export, VT_EMPTY, VTS_BOOL VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "Edit", _Edit, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CfmGrid, "SetSkip", _SetSkip, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "ExportEx", _ExportEx, VT_EMPTY, VTS_BOOL VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "SetCellItems", _SetCellItems, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "MultiSort", _MultiSort, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "Blink", _Blink, VT_EMPTY, VTS_I4 VTS_I4 VTS_BOOL VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetFormat", _SetFormat, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "Excel2", _Excel2, VT_EMPTY, VTS_BOOL VTS_BOOL VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "GetCellAttr", _GetCellAttr, VT_BOOL, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetRows", _SetRows, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CfmGrid, "ExcelEx", _ExcelEx, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "Update", _Update, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "InsertRows", _InsertRows, VT_EMPTY, VTS_I4 VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "GetCell", _GetCellx, VT_BSTR, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "SetCell", _SetCellx, VT_EMPTY, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "GetColumnWidth", _GetColumnWidth, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetColumnWidth", _SetColumnWidth, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetHeadCell", _GetHeadCell, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetHeadCell", _SetHeadCell, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "GetColumnDataKind", _GetColumnDataKind, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetColumnMode", _GetColumnMode, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetColumnLength", _GetColumnLength, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetColumnFormat", _GetColumnFormat, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CfmGrid, "GetColumn", _GetColumn, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "SetGridText", _SetText, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "SetText", _SetTextx, VT_EMPTY, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CfmGrid, "GetFloatDigit", _GetFloatDigit, VT_I4, VTS_I4)
	DISP_FUNCTION(CfmGrid, "SetBlockMode", _SetBlockMode, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CfmGrid, "SetFloatDigit", _SetFloatDigit, VT_EMPTY, VTS_I4 VTS_I4)

	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmGrid to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {ED8E198C-161B-401A-87D0-2E4627E2EF96}
static const IID IID_IfmGrid =
{ 0xed8e198c, 0x161b, 0x401a, { 0x87, 0xd0, 0x2e, 0x46, 0x27, 0xe2, 0xef, 0x96 } };

BEGIN_INTERFACE_MAP(CfmGrid, CfmBase)
	INTERFACE_PART(CfmGrid, IID_IfmGrid, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmGrid message handlers

void CfmGrid::_SetFocus() 
{
	m_axform->DoSomething(doFOCUS, this);
}

void CfmGrid::_Refresh() 
{
	Refresh();
}

BSTR CfmGrid::_GetCell(long row, long column) 
{
	CString strResult = _T("");

	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
		ReadData(strResult, false, -1, column-1, row-1);
	else
		strResult.Empty();

	return strResult.AllocSysString();
}

void CfmGrid::_SetCell(long row, long column, LPCTSTR cells) 
{
	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
	{
		WriteData(cells, true, column-1, row-1);
		if (m_axform->DoSomething(doIGNORE, this))
			IsChanged();
	}
}

BSTR CfmGrid::_GetText(long row, long column) 
{
	CString strResult = _T("");

	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
		ReadData(strResult, true, -1, column-1, row-1);
	else
		strResult.Empty();

	return strResult.AllocSysString();
}

long CfmGrid::_GetPRgb(long row, long column) 
{
	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
		return GetBkColor(column-1, row-1);
	
	return 0;
}

void CfmGrid::_SetPRgb(long row, long column, long rgb) 
{
	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
		SetBkColor(rgb, column-1, row-1);
}

long CfmGrid::_GetTRgb(long row, long column) 
{
	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
		return GetFgColor(column-1, row-1);
	
	return 0;
}

void CfmGrid::_SetTRgb(long row, long column, long rgb) 
{
	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
		SetFgColor(rgb, column-1, row-1);
}

BOOL CfmGrid::_IsColumnVisible(long column) 
{
	return IsColumnVisible(column);
}

void CfmGrid::_SetColumnVisible(long column, BOOL visible) 
{
	SetColumnVisible(column, visible);
}

BOOL CfmGrid::_IsCellEnable(long row, long column) 
{
	return IsCellEnable(row, column);
}

void CfmGrid::_SetCellEnable(long row, long column, BOOL enable) 
{
	SetCellEnable(row, column, enable);
}

BSTR CfmGrid::_GetHead(long column) 
{
	CString strResult = _T("");

	if (column > 0 && column <= m_totCols)
		ReadData(strResult, false, -1, column-1, -2);
	else
		strResult.Empty();

	return strResult.AllocSysString();
}

void CfmGrid::_SetHead(long column, LPCTSTR head) 
{
	if (column > 0 && column <= m_totCols)
		WriteData(head, true, column-1, -2);
}

BOOL CfmGrid::_IsColumnSortable(long column) 
{
	return IsColumnSortable(column);
}

BSTR CfmGrid::_GetColumnName(long column) 
{
	CString	strResult;
	
	strResult = GetColumnName(column);
	return strResult.AllocSysString();
}

void CfmGrid::_SetDataPosition(long method) 
{
	SetDataPosition(method);
}

void CfmGrid::_Sort(long col, BOOL ascend, BOOL string) 
{
	Sort(col, ascend ? true : false, string ? true : false);
}

void CfmGrid::_MultiSort(LPCTSTR cols, BOOL ascend) 
{
	if (strlen(cols) <= 0)
		return;

	CStringArray	tArray,  rArray;
	CString		tmps,  tmpx;
	CWordArray	wArray;
	WORD		idx,  idx2;
	Ccolumn*	column;
	CrgbArr*	rgbArr;
	CArray	<CrgbArr*, CrgbArr*> RGBs;;
	CDWordArray*	attrArr;
	CArray	<CDWordArray*, CDWordArray*> attrs;
	BYTE		tmpb;
	CByteArray*	byteArr = nullptr;
	CArray	<CByteArray*, CByteArray*> valids;
	CArray	<CByteArray*, CByteArray*> enables;
	CArray	<CByteArray*, CByteArray*> floats;
	COLORREF	tRGB,  pRGB;
	CByteArray	columns;
	int		ii, jj, pos;

	tArray.RemoveAll();
	rArray.RemoveAll();
	wArray.RemoveAll();
	RGBs.RemoveAll();
	attrs.RemoveAll();
	valids.RemoveAll();
	enables.RemoveAll();
	floats.RemoveAll();
	columns.RemoveAll();

	tmps = cols;
	tmps.TrimLeft();
	for ( ; !tmps.IsEmpty(); )
	{
		pos = tmps.Find(',');
		if (pos == -1)
		{
			tmpx = tmps;
			tmps.Empty();
		}
		else
		{
			tmpx = tmps.Left(pos++);
			tmps = tmps.Mid(pos);
		}

		tmpx.TrimRight();
		tmps.TrimLeft();

		pos = findColumn(tmpx);
		if (pos < 0)
			continue;

		columns.Add(pos);
	}

	for (ii = 0; ii < m_totCols; ii++)
	{
		rgbArr = new CrgbArr;
		RGBs.Add(rgbArr);

		byteArr = new CByteArray;
		valids.Add(byteArr);

		byteArr = new CByteArray;
		floats.Add(byteArr);

		if (m_form->attr2 & GO_TABLE)
		{
			attrArr = new CDWordArray;
			attrs.Add(attrArr);

			byteArr = new CByteArray;
			enables.Add(byteArr);
		}
	}

	for (ii = 0; ii < m_validRows; ii++)
	{
		tmps.Empty();
		for (int jj = 0; jj < columns.GetSize(); jj++)
			tmps += m_data.GetAt(ii * m_totCols + columns.GetAt(jj));
		tArray.Add(tmps);
		wArray.Add(ii);
	}

	for (ii = 0; ii < m_validRows; ii++)
	{
		for (jj = ii; jj < m_validRows; jj++)
		{
			tmps = tArray.GetAt(ii);
			idx  = wArray.GetAt(ii);
			tmpx = tArray.GetAt(jj);
			idx2 = wArray.GetAt(jj);

			CString text = tmpx;
			text.TrimLeft();
			if (text.IsEmpty())
				continue;
			
			text = tmps;
			text.TrimLeft();
			if (text.IsEmpty())
			{
				tArray.SetAt(ii, tmpx);
				tArray.SetAt(jj, tmps);
				wArray.SetAt(ii, idx2);
				wArray.SetAt(jj, idx);
				continue;
			}

			if (ascend)
			{
				if (compare(true, tmps, tmpx) > 0)
				{
					tArray.SetAt(ii, tmpx);
					tArray.SetAt(jj, tmps);
					wArray.SetAt(ii, idx2);
					wArray.SetAt(jj, idx);
				}
			}
			else
			{
				if (compare(true, tmps, tmpx) < 0)
				{
					tArray.SetAt(ii, tmpx);
					tArray.SetAt(jj, tmps);
					wArray.SetAt(ii, idx2);
					wArray.SetAt(jj, idx);
				}
			}
		}
	}

	for (ii = 0; ii < m_validRows; ii++)
	{
		idx  = wArray.GetAt(ii);
		for (jj = 0; jj < m_totCols; jj++)
		{
			rgbArr = RGBs.GetAt(jj);
			column = getColumn(jj);
			tmps = m_data.GetAt(idx * m_totCols + jj);
			rArray.Add(tmps);

			if (idx < column->m_pRGBs.GetSize() && idx < column->m_tRGBs.GetSize()
					&& idx < column->m_valids.GetSize())
			{
				pRGB = column->m_pRGBs.GetAt(idx);
				rgbArr->pRGBs.Add(pRGB);
				tRGB = column->m_tRGBs.GetAt(idx);
				rgbArr->tRGBs.Add(tRGB);
				tmpb = column->m_valids.GetAt(idx);
				valids.GetAt(jj)->Add(tmpb);
			}

			if (m_cellR[column->m_ref].attr & FA_FLOAT)
			{
				if (idx < column->m_floats.GetSize())
					floats.GetAt(jj)->Add(column->m_floats.GetAt(idx));
			}

			if (m_form->attr2 & GO_TABLE)
			{
				if (idx < column->m_attrs.GetSize())
					attrs.GetAt(jj)->Add(column->m_attrs.GetAt(idx));
				if (idx < column->m_enables.GetSize())
					enables.GetAt(jj)->Add(column->m_enables.GetAt(idx));
			}
		}
	}

	for (ii = 0; ii < m_totCols; ii++)
	{
		column  = m_columns.GetAt(ii);
		rgbArr  = RGBs.GetAt(ii);
		byteArr = valids.GetAt(ii);
		for (jj = 0; jj < m_validRows; jj++)
		{
			if (jj > rgbArr->pRGBs.GetUpperBound() || jj > rgbArr->tRGBs.GetUpperBound()
				|| jj > column->m_pRGBs.GetUpperBound() || jj > column->m_tRGBs.GetUpperBound()
				|| jj > byteArr->GetUpperBound() || jj > column->m_valids.GetUpperBound())
				break;

			pRGB = rgbArr->pRGBs.GetAt(jj);
			column->m_pRGBs.SetAt(jj, pRGB);
			tRGB = rgbArr->tRGBs.GetAt(jj);
			column->m_tRGBs.SetAt(jj, tRGB);
			column->m_valids.SetAt(jj, byteArr->GetAt(jj));
		}

		if (m_cellR[column->m_ref].attr & FA_FLOAT)
		{
			byteArr = floats.GetAt(ii);
			for (jj = 0; jj < m_validRows; jj++)
			{
				if (jj > byteArr->GetUpperBound() || jj > column->m_floats.GetUpperBound())
					break;

				column->m_floats.SetAt(jj, byteArr->GetAt(jj));
			}
		}

		if (m_form->attr2 & GO_TABLE)
		{
			attrArr = attrs.GetAt(ii);
			byteArr = enables.GetAt(ii);
			for (jj = 0; jj < m_validRows; jj++)
			{
				if (jj > attrArr->GetUpperBound() || jj > column->m_attrs.GetUpperBound()
					|| jj > byteArr->GetUpperBound() || jj > column->m_enables.GetUpperBound())
					break;

				column->m_attrs.SetAt(jj, attrArr->GetAt(jj));
				column->m_enables.SetAt(jj, byteArr->GetAt(jj));
			}
		}
	}

	for (ii = 0; ii < rArray.GetSize(); ii++)
	{
		tmps = rArray.GetAt(ii);
		if (ii > m_data.GetUpperBound())
			break;
		m_data.SetAt(ii, tmps);
		pos = ii % m_totCols;
		makeSignData(tmps, pos);
		tmps = editData(tmps, pos, ii / m_totCols);
		m_edit.SetAt(ii, tmps);
	}

	if (m_form->attr2 & GO_ROW)
	{
		for (ii = 0; ii < m_totCols; ii++)
		{
			column = m_columns.GetAt(ii);
			column->m_pRGBs.RemoveAll();
			for (jj = 0; jj < m_totRows; jj++)
			{
				if (jj % 2)
					column->m_pRGBs.Add(m_sRGB);
				else
					column->m_pRGBs.Add(m_fRGB);
			}
		}
	}

	for (ii = 0; ii < m_totCols; ii++)
	{
		rgbArr = RGBs.GetAt(ii);
		rgbArr->pRGBs.RemoveAll();
		rgbArr->tRGBs.RemoveAll();
		delete rgbArr;

		byteArr = valids.GetAt(ii);
		byteArr->RemoveAll();
		delete byteArr;

		byteArr = floats.GetAt(ii);
		byteArr->RemoveAll();
		delete byteArr;

		if (m_form->attr2 & GO_TABLE)
		{
			attrArr = attrs.GetAt(ii);
			attrArr->RemoveAll();
			delete attrArr;

			byteArr = enables.GetAt(ii);
			byteArr->RemoveAll();
			delete byteArr;
		}
	}
	RGBs.RemoveAll();
	attrs.RemoveAll();
	valids.RemoveAll();
	enables.RemoveAll();
	floats.RemoveAll();

	CRect	tmpRc1 = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		tmpRc1.DeflateRect(2, 2);
	if (m_vscroll)
		tmpRc1.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tmpRc1.right = m_pscr.left;
	if (m_hscroll)
		tmpRc1.bottom = m_hscr.top;
	invalidateRect(&tmpRc1, false);
}

void CfmGrid::_InsertRow(long index) 
{
	InsertRow(index);
}

void CfmGrid::_RemoveRow(long index) 
{
	RemoveRow(index);
}

void CfmGrid::_Key(long key) 
{
	m_axform->DoSomething(doKEY, this, (WPARAM)0, (LPARAM)key);
}

void CfmGrid::_Push(long row, long column, LPCTSTR name) 
{
	m_axform->DoSomething(doPUSHX, this, MAKEWPARAM(row-1, column-1), (LPARAM)name);
}

void CfmGrid::_Pop(long row, long column, LPCTSTR name) 
{
	m_axform->DoSomething(doPOPX, this, MAKEWPARAM(row-1, column-1), (LPARAM)name);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

void CfmGrid::_Print() 
{
	m_axform->DoSomething(doPRINT, this);
}

void CfmGrid::_Clear() 
{
	ClearData();
}

void CfmGrid::_SetItems(long col, LPCTSTR items) 
{
	SetItems(col, items);
}

void CfmGrid::_SetCellItems(long row, long col, LPCTSTR items) 
{
	SetCellItems(row, col, items);
}

void CfmGrid::_SetCellFocus(long row, long col)
{
	SetCellFocus(row, col);
}

void CfmGrid::_Import(LPCTSTR files) 
{
	Import(files);
}

void CfmGrid::_SetCellAttr(long row, long column, long dwAdd, long dwRemove) 
{
	SetCellAttr(row, column, dwAdd, dwRemove);
}

void CfmGrid::_Excel(BOOL run, BOOL head, LPCTSTR path) 
{
	m_axform->DoSomething(doEXCEL, this, MAKEWPARAM(run ? 1 : 0, head ? 1 : 0), (LPARAM)path);
}

void CfmGrid::_Excel2(BOOL run, BOOL head, LPCTSTR path) 
{
	m_axform->DoSomething(doEXCEL2, this, MAKEWPARAM(run ? 1 : 0, head ? 1 : 0), (LPARAM)path);
}

void CfmGrid::_ExcelEx(BOOL head) 
{
	m_axform->DoSomething(doEXCELx, this, MAKEWPARAM(0, head ? 1 : 0));
}

BOOL CfmGrid::_GetCellAttr(long row, long col, long cc) 
{
	if (!(m_form->attr2 & GO_TABLE) || row <= 0 || row > m_totRows || col <= 0 || col > m_totCols)
		return FALSE;

	DWORD	attr, attrx = 0;
	Ccolumn* column = getColumn(col-1);
	attr = column->m_attrs.GetAt(row-1);
	if (cc & CC_PRO)
		attrx |= FA_PROTECT;
	if (cc & CC_MAND)
		attrx |= FA_MAND;
	if (cc & CC_SEND)
		attrx |= FA_SEND;

	return (attr & attrx) ? TRUE : FALSE;
}

void CfmGrid::_Export(BOOL head, LPCTSTR path, BOOL append) 
{
	m_axform->DoSomething(doTEXT, this, MAKEWPARAM(append ? 1 : 0, head ? 1 : 0), (LPARAM)path);
}

void CfmGrid::_ExportEx(BOOL run, LPCTSTR top, LPCTSTR bottom, LPCTSTR path) 
{
	CString	data;

	data.Format("%s%c%s%c%s", path, 0x7f, top, 0x7f, bottom);
	m_axform->DoSomething(doEXPORT, this, MAKEWPARAM(run ? 1 : 0, 0), (LPARAM)data.operator LPCTSTR());
}

void CfmGrid::_Edit() 
{
	m_axform->DoSomething(doMODAL, this);
}

void CfmGrid::_SetSkip(LPCTSTR cname, BOOL skip) 
{
	CString	name = CString(cname);
	for (int ii = 0; ii < m_totCols; ii++)
	{
		if (!name.CompareNoCase((char *)m_cellR[ii].name))
		{
			if (skip)
				m_cellR[ii].attr |= FA_SKIP;
			else
				m_cellR[ii].attr &= ~FA_SKIP;
			break;
		}
	}
}

void CfmGrid::_Blink(long row, long col, BOOL set, long color) 
{
	if (row <= 0 || row > m_totRows || col <= 0 || col > m_totCols)
		return;

	m_axform->DoSomething(doBLINK, this, MAKEWPARAM(row-1, MAKEWORD(col-1, set ? 1 : 0)), color);
}

void CfmGrid::_SetFormat(long row, long col, LPCTSTR formats) 
{
	if (row <= 0 || row > m_totRows || col <= 0 || col > m_totCols)
		return;

	CString	text, data;

	row--; col--;
	data = m_data.GetAt(row * m_totCols + col);
	text = formats;
	text.TrimRight();
	if (text.IsEmpty())
	{
		m_data.SetAt(row * m_totCols + col, _T(""));
		WriteData(data, true, col, row);
	}
	else
	{
		CRect	tRc;
		CString	string;
		int	ii, len, dcnt = 0;
		Ccolumn* column = m_columns.GetAt(col);
		DWORD	attr    = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[column->m_ref].attr;

		data.TrimLeft();
		data.TrimRight();
		len = data.GetLength();
		if (len == 0)
			return;

		if (attr & FA_RIGHT)
		{
			string.Empty();
			for (ii = text.GetLength()-1; ii >= 0; ii--)
			{
				if (text[ii] == '9')
					dcnt++;

				if (dcnt >= len)
					break;
			}
			string = text.Mid(ii);
		}
		else
			string = text;

		dcnt = 0; text.Empty();
		for (ii = 0; ii < string.GetLength(); ii++)
		{
			if (string[ii] == '9')
			{
				if (dcnt >= len)
					break;
				else
				{
					text += data[dcnt];
					dcnt++;
				}
			}
			else
				text += string[ii];
		}

		m_edit.SetAt(row * m_totCols + col, text);
		tRc = m_cell;
		tRc.InflateRect(2, 2);
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::_SetRows(long rows) 
{
	if (m_totRows == rows)
		return;

	int	ii, jj, vrows;
	CString	tmps = _T("");
	Ccolumn* column;

	vrows = m_validRows;
	if (rows == 0)
	{
		for (ii = 0; ii < m_totCols; ii++)
		{
			column = m_columns.GetAt(ii);

			column->m_pRGBs.RemoveAll();
			column->m_tRGBs.RemoveAll();
			column->m_itRGBs.RemoveAll();
			column->m_valids.RemoveAll();
			if (m_form->attr2 & GO_TABLE)
			{
				column->m_attrs.RemoveAll();
				column->m_enables.RemoveAll();
			}
			if (m_cellR[column->m_ref].attr & FA_FLOAT)
				column->m_floats.RemoveAll();
		}
		m_data.RemoveAll();
		m_edit.RemoveAll();
	}
	else if (rows < m_totRows)
	{
		for (ii = rows; ii < m_totRows; ii++)
		{
			for (jj = 0; jj < m_totCols; jj++)
			{
				column = m_columns.GetAt(jj);

				m_data.RemoveAt(rows*m_totCols);
				m_edit.RemoveAt(rows*m_totCols);
				column->m_pRGBs.RemoveAt(rows);
				column->m_tRGBs.RemoveAt(rows);
				column->m_itRGBs.RemoveAt(rows);
				column->m_valids.RemoveAt(rows);
				if (m_form->attr2 & GO_TABLE)
				{
					column->m_attrs.RemoveAt(rows);
					column->m_enables.RemoveAt(rows);
				}
				if (m_cellR[column->m_ref].attr & FA_FLOAT)
					column->m_floats.RemoveAt(rows);
			}
		}

		if (m_validRows > rows)
			m_validRows = rows;
	}
	else
	{
		for (ii = m_totRows; ii < rows; ii++)
		{
			for (jj = 0; jj < m_totCols; jj++)
			{
				tmps = CString(' ', m_cellR[jj].size);
				m_data.Add(tmps);
				m_edit.Add(editData(tmps, jj, ii));

				column = getColumn(jj);
				if (m_form->attr2 & GO_ROW)
				{
					if (ii % 2)
						column->m_pRGBs.Add(m_sRGB);
					else
						column->m_pRGBs.Add(m_fRGB);
				}
				else
					column->m_pRGBs.Add((m_cellR[jj].pRGB  & userCOLOR) ? m_cellR[jj].pRGB  : getRGB(m_cellR[jj].pRGB));

				if (m_cellR[jj].tRGB & userCOLOR)
				{
					column->m_tRGBs.Add(m_cellR[jj].tRGB);
					column->m_itRGBs.Add(-1);
				}
				else
				{
					column->m_tRGBs.Add(getRGB(m_cellR[jj].tRGB));
					column->m_itRGBs.Add((short)m_cellR[jj].tRGB);
				}

				if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
					column->m_valids.Add(1);
				else
					column->m_valids.Add(0);
				if (m_form->attr2 & GO_TABLE)
				{
					column->m_attrs.Add(m_cellR[jj].attr);
					column->m_enables.Add(1);
				}
				if (m_cellR[jj].attr & FA_FLOAT)
					column->m_floats.Add(0xff);
			}
		}
	}

	m_totRows = rows;
	if (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND)
		m_validRows = m_totRows;

	CRect	sRc;
	bool	vscroll, enablescroll;

	vscroll = m_vscroll;
	enablescroll = m_enableScroll;

	if (m_totRows*m_multiN > (m_showRows-m_headN*m_multiN))
	{
		m_vscroll = true;
		if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
			m_enableScroll = true;
		else if (m_validRows*m_multiN > (m_showRows-m_headN*m_multiN))
			m_enableScroll = true;
		else	m_enableScroll = false;
	}
	else
	{
		m_vscroll = false;
		m_enableScroll = false;
	}

	sRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		sRc.DeflateRect(2, 2);
	if (vscroll == m_vscroll && enablescroll == m_enableScroll)
	{
		if (!(m_form->attr2 & GO_X3D))
		{
			if (!(m_form->attr2 & GO_XHEAD))
				sRc.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			sRc.top += m_multiN;			// head extra space
		sRc.top  += (int)((m_cy+m_slit) * m_headN * m_multiN);

		if (m_hscroll)
			sRc.bottom = m_hscr.top;
		if (vrows == m_validRows && !m_vscr.IsRectEmpty())
			sRc.right = m_vscr.left;
		else if (m_enableScroll)
		{
			m_vthumb.SetRectEmpty();
			invalidateRect(&m_vscr, false);
		}
	}
	invalidateRect(sRc, false);
}

void CfmGrid::_Update(long index, LPCTSTR data) 
{
	if (index < 0 || index >= m_validRows || strlen(data) == 0)
		return;

	CRect	tRc;
	Ccolumn* column;
	int	pos, col = -1;
	CString	text, dats = data;

	pos = dats.Find('\n');
	if (pos != -1)
		dats = dats.Left(pos);
	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = getColumn(ii);
		if (col < 0 && column->m_visible)
			col = ii;
		if (m_cellR[ii].iok == EIO_INPUT || m_cellR[ii].iok == EIO_NOP)
			continue;

		pos = dats.Find('\t');
		if (pos != -1)
		{
			text = dats.Left(pos++);
			dats = dats.Mid(pos);
		}
		else
		{
			text = dats;
			dats.Empty();
		}
		WriteData(text, false, ii, index);
	}

	if (col >= 0)
		col = m_stCol;
	if (getRect(index, col, tRc))
	{
		tRc.left  = m_pRc.left;
		tRc.right = m_pRc.right;
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 0);
		if (m_vscroll)
			tRc.right = m_vscr.left;
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::_InsertRows(long index, LPCTSTR data, BOOL skipInput) 
{
	if (m_totRows <= 0 || index >= m_totRows)
		return;

	int	pos, count;
	CString	text, dats = data;

	pos = dats.Find('\t');
	if (dats.GetLength() == 0 || pos == -1)
		return;
	
	count = atoi(dats.Left(pos++));
	dats  = dats.Mid(pos);
	for (int ii = 0; ii < count; ii++)
	{
		pos = dats.Find('\n');
		if (pos != -1)
		{
			text = dats.Left(pos++);
			dats = dats.Mid(pos);
		}
		else
		{
			text = dats;
			dats.Empty();
		}

		if (insertRow(index < 0 ? index : ii, text, false, skipInput) == -1)
			break;
	}

	SetDomino(false);
	if (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND)
		m_validRows = m_totRows;

	bool	scroll  = false;
	bool	bScroll = m_enableScroll;
	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;			// head extra space
	tRc.top += (int)((m_cy+m_slit) * m_multiN * m_headN);
	if (m_vscroll && !m_vscr.IsRectEmpty())
		tRc.right = m_vscr.left;
	if ((m_form->attr2 & GO_PAGE) && !m_pscr.IsRectEmpty())
		tRc.right = m_pscr.left;

	m_enableScroll = false;
	if (m_validRows*m_multiN > m_showRows-m_headN*m_multiN)
	{
		scroll = true;
		m_enableScroll = true;
	}

	DoListBox(false);
	if (m_form->attr2 & GO_AUTO)
	{
		CSize	size;
		Ccolumn* column;
		CDC*	dc = getDC();
		bool	hscroll = m_hscroll;
		int	offs, gap = (int)(szSCROLL*m_hR)+4;

		for (int ii = 0; ii < m_totCols; ii++)
		{
			column = m_columns.GetAt(ii);
			setFont(dc, (m_cellR[column->m_ref].properties & PR_BOLD) ? 1 : 0);
			if ((m_form->attr2 & GO_MERGE) && column->m_heads.GetSize() > 0)
			{
				offs = 0;
				for (int jj = 0; jj < column->m_heads.GetSize(); jj++)
				{
					size = dc->GetOutputTextExtent(column->m_heads.GetAt(jj));
					offs = max(offs, size.cx);
				}
			}
			else
			{
				size = dc->GetOutputTextExtent(column->m_head);
				offs = size.cx;
			}

			for (int jj = 0; jj < m_validRows; jj++)
			{
				size = dc->GetOutputTextExtent(m_edit.GetAt(jj*m_totCols+ii));
				if (m_cellR[column->m_ref].kind == CK_COMBO)
					size.cx += gap;
				offs = max(offs, size.cx);
			}
			column->m_width  = (float)offs + 6;
			column->m_widthx = (WORD)(column->m_width / m_hR);
		}
		releaseDC(dc);

		m_hscroll = isHScrollEnabled();
		if (!m_hscroll)
		{
			m_stCol = m_form->type;
			for (int ii = m_form->type; ii < m_totCols; ii++)
			{
				if (m_columns.GetAt(ii)->m_visible)
				{
					m_stCol = ii;
					break;
				}
			}
			m_scrolledWidth = 0;
		}

		if (m_hscroll != hscroll)
		{
			if (!m_fixRows)
			{
				tRc = m_pRc;
				if (!(m_form->attr2 & GO_X3D))
					tRc.DeflateRect(2, 2);
				m_showRows = (m_totRows + m_headN) * m_multiN;
				if (m_vscroll)
				{
					float	showRows, tHeight = 0;

					if (m_hscroll)
						tRc.bottom -= szSCROLL;
					if (!(m_form->attr2 & GO_XHEAD))
					{
						if (!(m_form->attr2 & GO_X3D))
							tRc.top += (4 * m_multiN);
						else
							tRc.top += m_multiN;
					}

// updateXXX_2022XX
					if ((float)tRc.Height() > ((m_headN * m_multiN) * m_cellH))
					{
						showRows = (float)(m_headN * m_multiN);
						if (m_form->attr2 & GO_MERGE)
							tHeight = (float)tRc.Height() - (m_headN*m_multiN)*(m_cy+m_slit);	// m_cellH -> m_cy
						else
							tHeight = (float)tRc.Height() - (m_headN*m_multiN)*m_cy;		// m_cellH -> m_cy
						showRows += (tHeight / (float)(m_cy + m_slit));					// m_cellH -> m_cy
					}
					else
						showRows = (float)tRc.Height() / m_cy;						// (float)m_cellH -> m_cy;

					m_showRows = (int)showRows;
					if ((showRows - (double)m_showRows) > 0.5)
						m_showRows++;
				}
			}
		}

		tRc = m_pRc;
		m_hthumb.SetRectEmpty();
		m_vthumb.SetRectEmpty();
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 2);
		invalidateRect(&tRc, false);
	}
	else if (scroll)
	{
		m_vthumb.SetRectEmpty();
		invalidateRect(&m_vscr, false);

		if (m_hscroll && !m_hscr.IsRectEmpty())
			tRc.bottom = m_hscr.top;
		invalidateRect(&tRc, false);
	}
	else
	{
		if (scroll != bScroll)
		{
			if (m_hscroll && !m_hscr.IsRectEmpty())
				tRc.bottom = m_hscr.top;

			m_vthumb.SetRectEmpty();
			invalidateRect(&m_vscr, false);
		}
		invalidateRect(&tRc, false);
	}
}

BSTR CfmGrid::_GetCellx(long row, LPCTSTR colN) 
{
	CString strResult;
	int	column = findColumn(colN);

	if (row > 0 && row <= m_totRows && column >= 0 && column < m_totCols)
		ReadData(strResult, false, -1, column, row-1);
	else
		strResult.Empty();

	return strResult.AllocSysString();
}

void CfmGrid::_SetCellx(long row, LPCTSTR colN, LPCTSTR data) 
{
	int	column = findColumn(colN);

	if (row > 0 && row <= m_totRows && column >= 0 && column < m_totCols)
	{
		WriteData(data, true, column, row-1);
		if (m_axform->DoSomething(doIGNORE, this))
			IsChanged();
	}
}

long CfmGrid::_GetColumnWidth(long column) 
{
	if (column > 0 && column <= m_totCols)
	{
		Ccolumn* col = getColumn(column-1);
		if (col)
		{
			float	ratio;
			int	width;

			ratio = (float)m_axform->m_rect.Width() / (float)m_axform->m_mapH->width;
			width = (int)(col->m_widthx / ratio);
			return width;
		}
	}
	return 0;
}

void CfmGrid::_SetColumnWidth(long column, long width) 
{
	if (column > 0 && column <= m_totCols)
	{
		Ccolumn* col= getColumn(column-1);
		if (col)
		{
			float	ratio;

			ratio = (float)m_axform->m_rect.Width() / (float)m_axform->m_mapH->width;
			col->m_widthx = (WORD)(width * ratio);
			col->m_width  = (float)col->m_widthx * m_hR;

			CRect	tRc = m_pRc;
			bool	hscroll = m_hscroll;
			m_hscroll = isHScrollEnabled();
			if (!m_hscroll)
			{
				m_stCol = m_form->type;
				for (int ii = m_form->type; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_visible)
					{
						m_stCol = ii;
						break;
					}
				}
				m_scrolledWidth = 0;
			}

			if (m_hscroll != hscroll)
			{
				if (!m_fixRows)
				{
					CRect	sRc = m_pRc;

					if (!(m_form->attr2 & GO_X3D))
						sRc.DeflateRect(2, 2);
					m_showRows = (m_totRows + m_headN) * m_multiN;
					if (m_vscroll)
					{
						float	showRows, height = 0;

						if (m_hscroll)
							sRc.bottom -= szSCROLL;
						if (!(m_form->attr2 & GO_XHEAD))
						{
							if (!(m_form->attr2 & GO_X3D))
								sRc.top += (4 * m_multiN);
							else
								sRc.top += m_multiN;
						}
// updateXXX_2022XX
						if ((float)sRc.Height() > ((m_headN * m_multiN) * m_cellH))
						{
							showRows = (float)(m_headN * m_multiN);
							if (m_form->attr2 & GO_MERGE)
								height = (float)sRc.Height() - (m_headN*m_multiN)*(m_cy + m_slit);	// m_cellH -> m_cy
							else
								height = (float)sRc.Height() - (m_headN*m_multiN)*m_cy;			// m_cellH -> m_cy
							showRows += (height / (float)(m_cy + m_slit));					// m_cellH -> m_cy
						}
						else
							showRows = (float)sRc.Height() / m_cy;						// m_cellH -> m_cy

						m_showRows = (int)showRows;
						if ((showRows - (double)m_showRows) > 0.5)
							m_showRows++;

						sRc.right -= szSCROLL;
						if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
							m_enableScroll = true;
						else if (m_validRows*m_multiN > (m_showRows-m_headN*m_multiN))
							m_enableScroll = true;
						else	m_enableScroll = false;
					}
				}
			}

			m_hthumb.SetRectEmpty();
			if (!(m_form->attr2 & GO_X3D))
				tRc.DeflateRect(2, 2);
			if (m_vscroll)
				m_vthumb.SetRectEmpty();
			invalidateRect(&tRc, false);
		}
	}
}

BSTR CfmGrid::_GetHeadCell(long col) 
{
	CString strResult;

	if (col > 0 && col <= m_totCols)
	{
		Ccolumn* column = m_columns.GetAt(col-1);
		strResult = column->m_hcell;
	}
	else
		strResult.Empty();

	return strResult.AllocSysString();
}

void CfmGrid::_SetHeadCell(long col, LPCTSTR cells) 
{
	if (col > 0 && col <= m_totCols)
	{
		Ccolumn* column = m_columns.GetAt(col-1);
		if (column->m_hcell.CompareNoCase(cells))
		{
			column->m_hcell = cells;
			if (m_cellR[column->m_ref].hkind == CK_CHECK)
			{
				CRect	tRc, cRc;

				tRc = m_pRc;
				if (!(m_form->attr2 & GO_X3D))
					tRc.DeflateRect(2, 2);
				tRc.bottom = tRc.top + (int)(m_cy * m_multiN * m_headN);
				if (!(m_form->attr2 & GO_XHEAD))
				{
					if (!(m_form->attr2 & GO_X3D))
						tRc.bottom += (4 * m_multiN);		// head extra space
					else
						tRc.bottom += m_multiN;			// head extra space
				}
				getRect(0, col-1, cRc, true);
				tRc.left  = cRc.left-1;
				tRc.right = cRc.right+1;
				invalidateRect(&tRc, false);
			}
		}
	}
}

long CfmGrid::_GetColumnDataKind(long col) 
{
	if (col > 0 && col <= m_totCols)
		return m_cellR[col-1].type;

	return 0;
}

long CfmGrid::_GetColumnMode(long col) 
{
	if (col > 0 && col <= m_totCols)
		return m_cellR[col-1].iok;
	return 0;
}

long CfmGrid::_GetColumnLength(long col) 
{
	if (col > 0 && col <= m_totCols)
		return m_cellR[col-1].size;
	return 0;
}

BSTR CfmGrid::_GetColumnFormat(long col) 
{
	CString strResult;

	if (col > 0 && col <= m_totCols)
	{
		Ccolumn* column = m_columns.GetAt(col-1);
		strResult = column->m_edit;
	}
	else
		strResult.Empty();

	return strResult.AllocSysString();
}

long CfmGrid::_GetColumn(LPCTSTR colN) 
{
	return findColumn(colN);
}

void CfmGrid::_SetText(long row, long col, LPCTSTR text) 
{
	if (row > 0 && row <= m_totRows && col > 0 && col <= m_totCols)
	{
		row--;	col--;
		Ccolumn* column = getColumn(col);
		if (row > column->m_valids.GetUpperBound())
			return;

		column->m_valids.SetAt(row, 1);

		CString	data = text;
		bool	skip = false;

		m_data.SetAt(row*m_totCols+col, data);
		if ((m_cellR[col].properties & PR_SUPRESS) && row > 0)
		{
			if (!data.CompareNoCase(m_data.GetAt((row-1)*m_totCols+col)))
				skip = true;
		}

		if (skip)
			m_edit.SetAt(row*m_totCols+col, _T(""));
		else
			m_edit.SetAt(row*m_totCols+col, data);

		if (m_form->properties & PR_VISIBLE)
		{
			CRect	tRc;

			DoListBox(false);
			if (getRect(row, findColumn(col), tRc))
				invalidateRect(&tRc, false);
		} 
	}
}

void CfmGrid::_SetTextx(long row, LPCTSTR colN, LPCTSTR text) 
{
	int	col = findColumn(colN);

	if (row > 0 && row <= m_totRows && col >= 0 && col < m_totCols)
	{
		row--;
		Ccolumn* column = getColumn(col);
		if (row > column->m_valids.GetUpperBound())
			return;

		column->m_valids.SetAt(row, 1);

		bool	skip = false;
		CString	data = text;

		m_data.SetAt(row*m_totCols+col, data);
		if ((m_cellR[col].properties & PR_SUPRESS) && row > 0)
		{
			if (!data.CompareNoCase(m_data.GetAt((row-1)*m_totCols+col)))
				skip = true;
		}

		if (skip)
			m_edit.SetAt(row*m_totCols+col, _T(""));
		else
			m_edit.SetAt(row*m_totCols+col, data);

		if (m_form->properties & PR_VISIBLE)
		{
			CRect	tRc;

			DoListBox(false);
			if (getRect(row, findColumn(col), tRc))
				invalidateRect(&tRc, false);
		} 
	}
}

long CfmGrid::_GetFloatDigit(long col) 
{
	if (col > 0 && col <= m_totCols)
	{
		col--;
		if (m_cellR[col].attr & FA_FLOAT)
			return (int)m_cellR[col].vals[1];
	}
	return 0;
}

void CfmGrid::_SetFloatDigit(long col, long val) 
{
	if (col > 0 && col <= m_totCols)
	{
	 	col--;
	 	if (m_cellR[col].attr & FA_FLOAT)
		 	m_cellR[col].vals[1] = val;
	}
}

void CfmGrid::_SetBlockMode(BOOL on) 
{
	if (on)
		m_form->attr |= FA_COMMA;	// FA_COMMA : GRID에는 Cell 단위에만 사용. block_mode로 사용
	else
		m_form->attr &= FA_COMMA;

}

BSTR CfmGrid::_getText() 
{
	CString strResult;

	ReadData(strResult, true);
	return strResult.AllocSysString();
}

BSTR CfmGrid::_getData() 
{
	CString strResult;

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmGrid::_setData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

BSTR CfmGrid::_getCell() 
{
	CString strResult;

	ReadData(strResult);
	strResult.TrimRight();
	if (strResult.IsEmpty())
		return NULL;
	return strResult.AllocSysString();
}

void CfmGrid::_setCell(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

BOOL CfmGrid::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CfmGrid::_setVisible(BOOL bNewValue) 
{
	CfmBase::SetVisible(bNewValue ? true : false);
}

long CfmGrid::_getIndex() 
{
	return GetCurSel();
}

void CfmGrid::_setIndex(long nNewValue) 
{
	SetCurSel(nNewValue);
}

long CfmGrid::_getRow() 
{
	return GetCellRow();
}

void CfmGrid::_setRow(long nNewValue) 
{
	SetCellRow(nNewValue);
}

long CfmGrid::_getColumn() 
{
	return GetCellColumn();
}

void CfmGrid::_setColumn(long nNewValue) 
{
	SetCellColumn(nNewValue);
}

BSTR CfmGrid::_getHead() 
{
	CString strResult;

	ReadData(strResult, false, -1, -1, -2);
	return strResult.AllocSysString();
}

void CfmGrid::_setHead(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue, true, -1, -2);
}

long CfmGrid::_getRows() 
{
	return GetValidRows();
}

long CfmGrid::_getColumns() 
{
	return m_totCols;
}

long CfmGrid::_getPos() 
{
	long	value = 0;
	
	if (m_stat & statPREV)
		value += 1;
	if (m_stat & statNEXT)
		value += 2;

	return value;
}

void CfmGrid::_setPos(long nNewValue) 
{
	m_stat = 0;
	switch (nNewValue)
	{
	case 1:
		m_stat |= statPREV;	break;
	case 2:
		m_stat |= statNEXT;	break;
	case 3:
		m_stat |= statPREV;
		m_stat |= statNEXT;	break;
	default:
		break;
	}
}

BSTR CfmGrid::_getPage() 
{
	CString strResult;

	strResult = m_page;
	return strResult.AllocSysString();
}

void CfmGrid::_setPage(LPCTSTR lpszNewValue) 
{
	m_page = lpszNewValue;
}

long CfmGrid::_getDirection() 
{
	return m_dir;
}

void CfmGrid::_setDirection(long nNewValue) 
{
	m_dir = nNewValue;
}

BSTR CfmGrid::_getSortColumn() 
{
	CString strResult;

	strResult = (m_sortc >= 0) ? (char *)m_cellR[m_sortc].name : _T("");
	return strResult.AllocSysString();
}

void CfmGrid::_setSortColumn(LPCTSTR lpszNewValue) 
{
	int	value;

	value = findColumn(lpszNewValue);
	if (value >= 0)
		m_sortc = value;
}

long CfmGrid::_getAction() 
{
	return (m_form->attr2 & GO_TOP) ? 1 : 2;
}

BSTR CfmGrid::_getInfo() 
{
	CString strResult;

	strResult = m_inits;
	return strResult.AllocSysString();
}

void CfmGrid::_setInfo(LPCTSTR lpszNewValue) 
{
	m_inits = lpszNewValue;
}

long CfmGrid::_getRealTm() 
{
	return (m_form->attr2 & GO_TOP) ? 1 : 2;
}

long CfmGrid::_getHitPos() 
{
	return m_axform->DoSomething(doHITPOS, this);
}

BOOL CfmGrid::_getFocus() 
{
	return (m_focus ? TRUE : FALSE);
}

long CfmGrid::_getRangeColumn1() 
{
	if (m_focus && m_form->isEdit == FI_CELL && (m_select || m_move == mvSel))
		return min(m_curpos.m_col, m_lastpos.m_col);

	return 0;
}

long CfmGrid::_getRangeColumn2() 
{
	if (m_focus && m_form->isEdit == FI_CELL && (m_select || m_move == mvSel))
		return max(m_curpos.m_col, m_lastpos.m_col);

	return 0;
}

long CfmGrid::_getRangeRow1() 
{
	if (m_focus && m_form->isEdit == FI_CELL && (m_select || m_move == mvSel))
		return min(m_curpos.m_row, m_lastpos.m_row);

	return 0;
}

long CfmGrid::_getRangeRow2() 
{
	if (m_focus && m_form->isEdit == FI_CELL && (m_select || m_move == mvSel))
		return max(m_curpos.m_row, m_lastpos.m_row);

	return 0;
}

BSTR CfmGrid::_getHeadCell() 
{
	CString strResult;

	if (m_cellpos.m_col < 1 || m_cellpos.m_col > m_totCols)
		strResult.Empty();
	else
		strResult = m_columns.GetAt(m_cellpos.m_col-1)->m_hcell;

	return strResult.AllocSysString();
}

void CfmGrid::_setHeadCell(LPCTSTR lpszNewValue) 
{
	if (m_cellpos.m_col > 0 && m_cellpos.m_col <= m_totCols)
	{
		Ccolumn* column = m_columns.GetAt(m_cellpos.m_col-1);
		if (column->m_hcell.CompareNoCase(lpszNewValue))
		{
			column->m_hcell = lpszNewValue;
			if (m_cellR[column->m_ref].hkind == CK_CHECK)
			{
				CRect	tRc, cRc;

				tRc = m_pRc;
				if (!(m_form->attr2 & GO_X3D))
					tRc.DeflateRect(2, 2);
				tRc.bottom = tRc.top + (int)(m_cy * m_multiN * m_headN);
				if (!(m_form->attr2 & GO_XHEAD))
				{
					if (!(m_form->attr2 & GO_X3D))
						tRc.bottom += (4 * m_multiN);		// head extra space
					else
						tRc.bottom += m_multiN;			// head extra space
				}
				getRect(0, m_cellpos.m_col-1, cRc, true);
				tRc.left  = cRc.left-1;
				tRc.right = cRc.right+1;
				invalidateRect(&tRc, false);
			}
		}
	}
}

int CfmGrid::GetName(int col)
{
	if (col < 0)
		return CfmBase::GetName();

	Ccolumn* column = getColumn(col);
	return column->m_rts;
}

bool CfmGrid::IsValid(int& guide)
{
	if (!(m_form->attr2 & GO_TABLE))
		return true;
	
	Ccolumn* column;
	CString	tmps;
	DWORD	attr;
	int	result;
	int	year, month, day;
	char	mday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = getColumn(ii);
		for (int jj = 0; jj < m_totRows; jj++)
		{
			tmps = m_data.GetAt(jj * m_totCols + ii); tmps.Trim();
			attr = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(jj) : m_cellR[ii].attr;
			if (attr & FA_MAND && tmps.IsEmpty())
			{
				guide = AE_EMUST;
				return false;
			}

			if (attr & FA_DATE)
			{
				guide = AE_EDATE;
				if (tmps.GetLength() == 0)
					return true;

				if (m_cellR[ii].size != tmps.GetLength())
					return false;

				switch (m_cellR[ii].size)
				{
				case szYYYYMMDD:
					year = atoi(tmps.Left(4));
					if (year < 1900)
						return false;
					month = atoi(tmps.Mid(4, 2));
					if (month <= 0 || month > 12)
						return false;
					day = atoi(tmps.Mid(6));
					if (day <= 0 || day > 31)
						return false;

					result = mday[month-1];
					if (month == 2 && (year % 4) == 0)
					{
						if ((year % 100) || !(year % 400))
							result++;
					}
					return (day <= result);

				case szYYMMDD:
					year = atoi(tmps.Left(2));
					if (year < 0)
						return false;
					month = atoi(tmps.Mid(2, 2));
					if (month <= 0 || month > 12)
						return false;
					day = atoi(tmps.Mid(4));
					if (day <= 0 || day > 31)
						return false;

					result = mday[month-1];
					if (month == 2 && result == 28)
						result++;
					return (day <= result);

				default:
					break;
				}

				return false;
			}
		}
	}

	return true;
}

void CfmGrid::Adjust(CPoint pt, float hR, float vR)
{
	float	tmpF;
	float	tWidth = 0;
	Ccolumn* column;
	
	m_move = mvNone;
	m_moveRc.SetRectEmpty();
	m_totWidth = m_fixedWidth = 0;
	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);

		tmpF = column->m_widthx * hR;
		column->m_widthx = (WORD)tmpF;
		if ( (tmpF - (double)column->m_widthx) > 0.5 )
			column->m_widthx++;
		column->m_width = column->m_widthx;

		if (column->m_visible)
		{
			tWidth += column->m_width;
			if (ii < m_form->type)
				m_fixedWidth += column->m_width;
		}
		if (((m_cellR[column->m_ref].attr & FA_WRAP) && m_form->attr2 & GO_MULTI) || ii == (m_totCols - 1))
		{
			m_totWidth = max(m_totWidth, tWidth);
			tWidth = 0;
		}
	}

//	if (m_form->attr2 & GO_MULTI)
//		m_fixedWidth = 0;
	m_totWidth -= m_fixedWidth;

	tmpF    = m_cellH * vR;
	m_cellH = (int)tmpF;
	if ( (tmpF - (double)m_cellH) > 0.5 )
		m_cellH++;
	m_cy = (float)m_cellH;

	CfmBase::Adjust(pt, hR, vR);

	if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		CRect	cRc;

		m_axform->m_view->GetClientRect(&cRc);
	//	m_pRc.right  = cRc.right;
		if (m_bottom_gap <= 0)
		{
			int	height;

			height = m_axform->GetHeight();
			if (height > 0)
				m_bottom_gap = height - m_rect.bottom;
		}

		if (m_right_gap <= 0)
		{
			int	width;

			width = m_axform->GetWidth();
			if (width > 0)
				m_right_gap = width - m_rect.right;
		}

		cRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			cRc.DeflateRect(2, 2);

		m_showRows = (m_totRows + m_headN) * m_multiN;
		if (m_vscroll)
		{
			float	showRows, tHeight = 0;

			if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
				m_enableScroll = true;

			if (m_hscroll)
				cRc.bottom -= szSCROLL;

			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					cRc.top += (4 * m_multiN);
				else
					cRc.top += m_multiN;
			}

			if ((float)cRc.Height() > ((m_headN * m_multiN) * m_cellH))
			{
				showRows = (float)(m_headN * m_multiN);
				if (m_form->attr2 & GO_MERGE)
					tHeight = (float)cRc.Height() - (m_headN*m_multiN)*(m_cellH+m_slit);
				else
					tHeight = (float)cRc.Height() - (m_headN*m_multiN)*m_cellH;
				showRows += tHeight / (float)(m_cellH + m_slit);
			}
			else
				showRows = (float)cRc.Height() / (float)m_cellH;

			m_showRows = (int)showRows;
			if ( (showRows - (double)m_showRows) > 0.5)
				m_showRows++;
			cRc.right -= szSCROLL;
		}
		m_showWidth = (float)cRc.Width();
	}
}

void CfmGrid::Resize(float hR, float vR, bool font, bool redraw)
{
	float	tWidth = 0;
	Ccolumn* column;

	m_move = mvNone;
	m_moveRc.SetRectEmpty();
	m_totWidth = m_fixedWidth = 0;
	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);
		if (!(m_form->attr2 & GO_FIX))
			column->m_width = column->m_widthx * hR;
	/*	else	// on coding (2008_02_21)
		{
			if (m_scrolledWidth <= 0)
				column->m_width = column->m_widthx * hR;
		}*/	
		if (column->m_visible)
		{
			tWidth += column->m_width;
			if (ii < m_form->type)
				m_fixedWidth += column->m_width;
		}
		if (((m_cellR[column->m_ref].attr & FA_WRAP) && m_form->attr2 & GO_MULTI) || ii == m_totCols-1)
		{
			m_totWidth = max(m_totWidth, tWidth);
			tWidth = 0;
		}
	}

	if (m_scrolledWidth > 0)
	{
		m_scrolledWidth = 0;
		m_stCol = max(m_form->type, m_stCol);
		if (m_form->type == m_totCols)
			m_stCol = 0;
		for (int ii = m_form->type; ii < m_stCol; ii++)
		{
			if (m_columns.GetAt(ii)->m_visible)
				m_scrolledWidth += m_columns.GetAt(ii)->m_width;
		}
	}

//	if (m_form->attr2 & GO_MULTI)
//		m_fixedWidth = 0;
	m_totWidth -= m_fixedWidth;

	m_vthumb.SetRectEmpty();
	m_hthumb.SetRectEmpty();
	m_hscr.SetRectEmpty(); m_vscr.SetRectEmpty();
	if (!(m_form->attr2 & GO_FIX) || (m_axform->IsResizable() && (m_form->attr2 & GO_FIX) && vR != 1.0))	// 20080309
	{
		m_cy = m_cellH * vR;
		CfmBase::Resize(hR, vR, font, redraw);
	}
	DoListBox(false);

	if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		CRect	cRc;

		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right - (int)(m_right_gap * hR);
		m_pRc.bottom = cRc.bottom - (int)(m_bottom_gap * vR);

		cRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			cRc.DeflateRect(2, 2);

		m_showRows = (m_totRows + m_headN) * m_multiN;
		if (m_vscroll)
		{
			float	showRows, tHeight = 0;

			if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
				m_enableScroll = true;

			if (m_hscroll)
			{
				cRc.bottom -= szSCROLL;
				CRect sRc = m_pRc;
				if (!(m_form->attr2 & GO_X3D))
					sRc.DeflateRect(2, 2);
				if (m_vscroll)
					sRc.right -= (int)(szSCROLL * m_hR);
				if (sRc.Height() > (int)(szSCROLL * m_vR))
					sRc.top = sRc.bottom - (int)(szSCROLL * m_vR);
				m_hscr = sRc;
			}

			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					cRc.top += (4 * m_multiN);
				else
					cRc.top += m_multiN;
			}

// updateXXX_2022XX
			if ((float)cRc.Height() > ((m_headN * m_multiN) * m_cellH))
			{
				showRows = (float)(m_headN * m_multiN);
				if (m_form->attr2 & GO_MERGE)
					tHeight = (float)cRc.Height() - (m_headN*m_multiN)*(m_cy+m_slit);	// m_cellH -> m_cy
				else
					tHeight = (float)cRc.Height() - (m_headN*m_multiN)*m_cy;		// m_cellH -> m_cy
				showRows += tHeight / (float)(m_cy + m_slit);					// m_cellH -> m_cy
			}
			else
				showRows = (float)cRc.Height() / (float)m_cy;					// m_cellH -> m_cy

			m_showRows = (int)showRows;
			if ( (showRows - (double)m_showRows) > 0.5 )
				m_showRows++;
// updateXXX_2022XX
			int	stline = m_stRow;
			if (stline > m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN)))
				stline = m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN));
			m_stRow = max(0, stline);
//
//???
			cRc.right -= szSCROLL;
			m_hthumb.SetRectEmpty();
			m_vthumb.SetRectEmpty();
		}
		m_showWidth = (float)cRc.Width();
	}
}

void CfmGrid::Reload()
{
	CfmBase::Reload();
	DoListBox(false);

	m_showRows      = 0;
	m_totWidth      = 0;
	m_scrolledWidth = 0;
	m_fixedWidth    = 0;
	m_headN         = 0;
	m_multiN        = 0;

	float	tWidth = 0;
	Ccolumn* column;
	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);

		if (m_form->attr2 & GO_MERGE)
			m_headN = max(m_headN, column->m_heads.GetSize());
		else
		{
			m_headN  = max(m_headN, (int)column->m_ihead);
			m_multiN = max(m_multiN, column->m_line);
		}
		column->m_widthx = m_cellR[column->m_ref].width;
		column->m_width = column->m_widthx;

		if (column->m_visible)
		{
			tWidth += column->m_width;
			if (ii < m_form->type)
				m_fixedWidth += column->m_width;
		}
		if (((m_cellR[column->m_ref].attr & FA_WRAP) && (m_form->attr2 & GO_MULTI))
			|| column->m_ref == m_totCols-1)
		{
			m_totWidth = max(m_totWidth, tWidth);
			tWidth = 0;
		}
	}
	m_multiN++;

	if (m_form->attr2 & GO_XHEAD)
		m_headN = 0;

//	if (m_form->attr2 & GO_MULTI)
//		m_fixedWidth = 0;
	m_totWidth -= m_fixedWidth;

	m_move = mvNone;
	m_moveRc.SetRectEmpty();
	m_hscr.SetRectEmpty();
	m_vscr.SetRectEmpty();
	m_pscr.SetRectEmpty();
	m_vthumb.SetRectEmpty();
	m_hthumb.SetRectEmpty();

	m_cell.SetRectEmpty();
	m_cbbtn.SetRectEmpty();

	if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		CRect	cRc(0,0,0,0);
		m_axform->m_view->GetClientRect(&cRc);
		m_pRc.right  = cRc.right;
		m_pRc.bottom = cRc.bottom;
	}

	CRect	sRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		sRc.DeflateRect(2, 2);

	m_cellH = LOWORD(m_form->keys);
	if (m_cellH > 0)
		m_fixRows  = false;
	else if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
	{
		m_fixRows  = false;
		m_cellH = (int)(m_fHeight+4);
	}
	else
	{
		int	dy = 0;

		m_fixRows  = true;
		m_showRows = HIWORD(m_form->keys);
		if (m_multiN)
			m_showRows *= m_multiN;
		if (m_form->attr2 & GO_MERGE)
			dy = sRc.Height() - (m_showRows+m_headN*m_multiN)*m_slit;
		else
			dy = sRc.Height() - (m_showRows+1)*m_slit;
		if (m_hscroll)
			dy -= szSCROLL;
		m_showRows += m_headN*m_multiN;
		tWidth  = (float)dy/(float)m_showRows;
		m_cellH = (int)tWidth;
		if ( (tWidth - (double)m_cellH) > 0.5 )
			m_cellH++;
	}

	if (m_showRows <= 0)
	{
		m_showRows = (m_totRows + m_headN) * m_multiN;
		if (m_vscroll)
		{
			if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
				m_enableScroll = true;

			float	tHeight = 0, showRows = 0;

			if (m_hscroll)
				sRc.bottom -= szSCROLL;

			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					sRc.top += (4 * m_multiN);
				else
					sRc.top += m_multiN;
			}

			if ((float)sRc.Height() > ((m_headN * m_multiN) * m_cellH))
			{
				showRows = (float)(m_headN * m_multiN);
				if (m_form->attr2 & GO_MERGE)
					tHeight = (float)sRc.Height() - (m_headN*m_multiN)*(m_cellH+m_slit);
				else
					tHeight = (float)sRc.Height() - (m_headN*m_multiN)*m_cellH;
				showRows += (tHeight / (float)(m_cellH + m_slit));
			}
			else
				showRows = (float)sRc.Height() / (float)m_cellH;

			m_showRows = (int)showRows;
			if ( (showRows - (double)m_showRows) > 0.5 )
				m_showRows++;

			sRc.right -= szSCROLL;
		}
	}
	else if (m_vscroll)
		sRc.right -= szSCROLL;

	m_showWidth = (float)sRc.Width();
}

int CfmGrid::IsPointInRect(CPoint pt)
{
	int	ret = CfmBase::IsPointInRect(pt);

	if (ret == P_OUT)
		return ret;

	int	start;
	float	width = 0;
	CRect	tRc  = m_pRc;

	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;			// head extra space

	start = tRc.left;

	Ccolumn* column;
	tRc.right = tRc.left;
	if (m_form->type)
	{
		for (int ii = 0; ii < m_form->type; ii++)
		{
			column = m_columns.GetAt(ii);
			if (!column->m_visible)
				continue;

			width += column->m_width;
			tRc.right = start + (int)width;
			if (tRc.PtInRect(pt))
				return column->m_ref+1;

			tRc.left = tRc.right;
		}
	}

	start += (int)m_fixedWidth;
	int	index, rows;

	index = -1;
	rows = min((m_validRows+m_headN)*m_multiN-1, m_showRows);
	for (int ii = m_headN*m_multiN; ii < rows + 1; ii++)
	{
		tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
		{
			if (!(m_form->attr2 & GO_X3D))
				tRc.top += (4 * m_multiN);	// head extra space
			else
				tRc.top += m_multiN;		// head extra space
		}
		tRc.top   += (int)((m_cy + m_slit) * ii);
		tRc.bottom = tRc.top + (int)m_cy;

		if (tRc.PtInRect(pt))
		{
			index = m_stRow + ii - (m_headN * m_multiN);
			if (index >= (m_totRows * m_multiN))
				index = m_totRows * m_multiN - 1;
			break;
		}
	}

	if (index == -1)
		return ret;

	CRect	sRc;
	int	tCol, colidx;

	tCol = m_form->type ? 0 : m_stCol;
	colidx = index % m_multiN;
	if (m_form->attr2 & GO_MULTI)
	{
		for (int ii = 0; ii < m_totCols; ii++)
		{
			if (m_columns.GetAt(ii)->m_line == colidx)
			{
				tCol = ii;
				break;
			}
		}
	}
	
	width = 0;
	sRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		sRc.DeflateRect(2, 2);
	for (int ii = tCol; ii < m_totCols; ii++)
	{
		if (m_form->type && ii == m_form->type)
			ii = m_stCol;

		column = m_columns.GetAt(ii);
		if (!column->m_visible)
			continue;

		if (column->m_line == colidx)
		{
			tRc.left = sRc.left + (int)width;
			width += column->m_width;
			if ((int)width < sRc.Width())
				tRc.right = sRc.left + (int)width;
			else
				tRc.right = sRc.right;

			if (tRc.PtInRect(pt))
				return column->m_ref+1;
		}
	}

	return ret;
}

void CfmGrid::SetFont(int point, int style, CString fonts, bool resize)
{
	if (m_form->fPoint == m_axform->m_mapH->fPoint)
	{
		bool	change = false;

		if (!fonts.IsEmpty() && m_fonts.CompareNoCase(fonts))
		{
			m_fonts = fonts;
			change  = true;
		}

		if (style >= 0 && m_form->fStyle != style)
		{
			m_form->fStyle = style;
			change = true;
		}

		if (m_fPoint != point)
		{
			m_fPoint = m_cfPoint = point;
			change = true;
		}

		if (change)
		{
			TEXTMETRIC metric;
			CDC*	dc = getDC();
			setFont(dc);
			dc->GetTextMetrics(&metric);
			m_fHeight = (BYTE)abs(metric.tmHeight);
			m_fWidth  = (BYTE)abs(metric.tmAveCharWidth);
			releaseDC(dc);

			if (resize)
			{
				float	tWidth = 0.;
				Ccolumn* column;

				m_totWidth = m_fixedWidth = 0;
				for (int ii = 0; ii < m_totCols; ii++)
				{
					column = m_columns.GetAt(ii);
					column->m_width = column->m_widthx * m_hR;
					if (column->m_visible)
					{
						tWidth += column->m_width;
						if (ii < m_form->type)
							m_fixedWidth += column->m_width;
					}
					if (((m_cellR[column->m_ref].attr & FA_WRAP) && m_form->attr2 & GO_MULTI) || ii == m_totCols-1)
					{
						m_totWidth = max(m_totWidth, tWidth);
						tWidth = 0;
					}
				}
			//	if (m_form->attr2 & GO_MULTI)
			//		m_fixedWidth = 0;
				m_totWidth -= m_fixedWidth;
			}

			if (m_form->properties & PR_TRANSPARENT)
				invalidateRect(&m_pRc, true);
			else
				invalidateRect(&m_pRc, false);
		}
	}
}

void CfmGrid::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE) || m_pRc.Height()==0 || m_pRc.Width()==0)
		return;

	m_drawing = true;

	CString	tmps = _T("");
	Ccolumn* col;
	int	ndc = dc->SaveDC();
	CRect	sRc,  sRc1,  sRc2;
	CRect	tRc,  hRc;
	float	dx, dy;
	int	showRows;
	int	ii,  jj,  kk,  ll, getrow, index, fix;
	int	linecnt, colidx, colcnt, stCol, skipN;
	UINT	format;
//	CSize	tSz;
	bool	noWrite, last, first;
	BYTE	valid;
	CBrush*	brush = (CBrush *) 0;
	COLORREF pRGB, tRGB;
	CMap	< int, int, CRect, CRect& > rects;

	rects.RemoveAll();
	setPen(dc, m_bRGB);
//	setFont(dc);

	sRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		sRc.DeflateRect(2, 2);
	else if (m_form->attr2 & GO_3DHEAD)
	{
		sRc.top++;
		sRc.left++;
	}
	if (m_vscroll)// && !(m_form->attr2 & GO_FIX))
		sRc.right -= (int)(szSCROLL * m_hR);
	if (m_hscroll)
		sRc.bottom -= (int)(szSCROLL * m_vR);
	m_showWidth = (float)sRc.Width();
	getrow = (m_headN == 0) ? 0 : 1;

	if (m_form->attr2 & (GO_MERGE|GO_XHEAD))
		skipN = m_headN*m_multiN;
	else
		skipN = m_multiN;

	noWrite = false;
	if (m_form->attr2 & GO_MERGE)
		dy = (float)(sRc.Height() - m_showRows*m_slit);
	else
		dy = (float)(sRc.Height() - (m_showRows-m_headN*m_multiN)*m_slit);
	if (!(m_form->attr2 & GO_X3D))
		dy -= (4 * m_multiN);
	if (m_fixRows)
		m_cy = dy / m_showRows;
	m_stRow = max(0, m_stRow);
	dx = 0;
//	if (m_totCols != 0 && m_totRows != 0 && m_stCol != -1)
	if (m_totCols != 0 && m_stCol != -1)
	{
		dy = (float)sRc.top;
		linecnt = colidx = stCol = 0;

		m_cell.SetRectEmpty();
		m_enCol = m_totCols-1;
		index = 0;
		if (m_form->attr2 & GO_VALID)
			showRows = min(m_showRows, m_validRows * m_multiN + skipN);
		else
			showRows = m_showRows;

		for (kk = 0; kk < showRows; kk++)
		{
			if (((m_form->attr2 & GO_VALID) && kk + m_stRow - skipN == m_validRows * m_multiN)
				|| (!(m_form->attr2 & GO_VALID) && kk + m_stRow - skipN == m_totRows * m_multiN))
				break;

			dx = 0;
			tRc.top = (int)dy;
			dy += m_cy;

			if (dy > sRc.bottom)
				dy = (float)(sRc.bottom - 1);

			dy += m_slit;
			fix = colcnt = 0;

			if ((m_form->attr2 & GO_MERGE && kk >= m_headN)
				|| (!(m_form->attr2 & GO_MERGE) && kk >= m_multiN) || m_headN == 0)
			{
				linecnt = (m_stRow + kk) % m_multiN;
				for (ll = 0; ll < m_totCols; ll++)
				{
					if (m_columns.GetAt(ll)->m_line == linecnt)
					{
						colidx = ll;
						break;
					}
				}
			}

			for (ll = colidx; ll < m_totCols; ll++)
			{
				if (m_columns.GetAt(ll)->m_line == linecnt)
					colcnt++;
			}

			if (m_form->attr2 & GO_MULTI && linecnt != 0)
			{
				float	width = 0;

				jj = stCol = colidx;
				for (ll = colidx; ll < colidx+colcnt; ll++)
				{
					if (m_columns.GetAt(ll)->m_visible)
						width += m_columns.GetAt(ll)->m_width;
					if (m_form->type && width > m_fixedWidth && !fix)
						fix = ll;
					if (width > m_fixedWidth+m_scrolledWidth)
					{
						stCol = ll;
						break;
					}
				}
			}
			else
			{
				stCol = m_stCol;
				fix = m_form->type;
			}

			jj = max(0, stCol);
			if (m_form->type)
				jj = colidx;		// jj = 0;

			first = true;
			if (m_form->attr2 & GO_MERGE)
				index = m_stRow + (kk-m_headN);
			else
				index = (m_stRow + kk)/m_multiN - getrow;
			index = max(0, index);

			if (((m_form->attr2 & GO_MERGE && kk < m_headN)
				|| (!(m_form->attr2 & GO_MERGE) && kk < m_multiN)) && m_headN != 0)
			{
				if (m_form->attr2 & GO_MERGE)
					tRc.bottom = tRc.top + (int)m_cy;
				else
					tRc.bottom = tRc.top + (int)m_cy * m_headN;

				if (m_form->attr2 & (GO_X3D|GO_MERGE))
				{
					tRc.bottom++;
					dy++;
				}
				else
				{
					tRc.bottom += 4;
					dy         += 4;
				}

				if ( (dy - (double)tRc.bottom) > 0.5)
					tRc.bottom++;
				dy = (float)tRc.bottom;

				setFont(dc);
			}

			for ( ; jj < colidx + colcnt ; jj++)
			{
				if (m_form->type && jj == fix)		// m_form->type)
				{
					if (stCol < colidx + colcnt)	// if (m_stCol < colidx + colcnt)
						jj = stCol;		//	jj = m_stCol;
					else	break;
				}

				if (jj >= m_totCols)
					break;

				col = m_columns.GetAt(jj);
				if (!col->m_visible)
					continue;

				tmps = _T("");
				last = false;
				format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

				tRc.left = sRc.left + (int)dx;
				dx += col->m_width;
			//	stCol = !stCol ? jj : stCol;
				if ((int)dx < sRc.Width())
					tRc.right = sRc.left + (int)dx;
				else
				{
					tRc.right = sRc.right;
					m_enCol = jj;
					last = true;
				}

				// head drawing
				if (((m_form->attr2 & GO_MERGE && kk < m_headN)
					|| (!(m_form->attr2 & GO_MERGE) && kk < m_multiN)) && m_headN != 0)
				{
					char	buf[256];
					char*	ptr;
					int	len;

					brush = setBrush(dc, col->m_pRGB);
					setTextColor(dc, col->m_tRGB);
					setPaintColor(dc, col->m_pRGB);

					tRc.bottom = (int)dy;
					if (last && tRc.Width() <= (int)(col->m_width * 0.7))
						format |= DT_LEFT;
					else
					{
						switch (m_cellR[col->m_ref].alignH)
						{
						case AL_LEFT:
							format |= DT_LEFT;      break;
						case AL_RIGHT:
							format |= DT_RIGHT;     break;
						case AL_CENTER:
						default:
							format |= DT_CENTER;    break;
						}
					}

					CRect	cRc = tRc;
					bool	draw = true;
					if (m_form->attr2 & GO_MERGE)
					{
						int	rc, ref;
						
						ref = kk*m_totCols+jj;
						rc = validCell(kk, jj, ref);
						if (last)
						{
							switch (rc)
							{
							case 3:		// left_top, right_bottom
								break;
							case 1:		// left_top
								rects.SetAt(kk*m_totCols+jj, tRc);
								break;
							default:
							case 2:		// right_bottom
								rects.Lookup(ref, cRc);
								cRc |= tRc;
								if (!(ref % m_totCols) && (m_form->attr2 & GO_X3D))
									cRc.left++;
								break;
							}
							if (kk < col->m_heads.GetSize())
								tmps = col->m_heads.GetAt(kk);
						}
						else
						{
							switch (rc)
							{
							case 2:		// right_bottom
								rects.Lookup(ref, cRc);
								cRc |= tRc;
							//	if (!(ref % m_totCols) && (m_form->attr2 & GO_X3D))
							//		cRc.left++;
							case 3:		// left_top, right_bottom
								break;
							case 1:		// left_top
								rects.SetAt(kk*m_totCols+jj, tRc);
							default:
								draw = false;
								break;
							}

							if (rc > 1)	// draw text & rect
							{
								rects.RemoveKey(ref);
								if (kk < col->m_heads.GetSize())
									tmps = col->m_heads.GetAt(kk);
							}
							else	tmps.Empty();
						}
					}
					else
						tmps = col->m_head;

					if (draw)
					{
						hRc = cRc;
						hRc.bottom = cRc.top;
						if (m_cellR[col->m_ref].hkind == CK_BUTTON
							&& (m_cellR[col->m_ref].properties & (PR_HIMAGE|PR_HIMAGETEXT))
							&& col->m_hpic->GetValid(PI_DEFAULT))
						{	// head : button
							col->m_hpic->DrawPicture(dc, cRc, AL_STRETCH, m_axform->m_oldVers, PI_DEFAULT);
						}
						else
						{
							if (!last)
								cRc.right--;
							dc->FillRect(cRc, brush);
							if (!last)
								cRc.right++;
						}
						dc->SetBkMode(TRANSPARENT);
						if (!tmps.IsEmpty())
						{
							if (m_form->attr2 & GO_MERGE)
							{
								col->m_ihead = 1;
								for (ii = 0; ii < tmps.GetLength(); ii++)
								{
									if (tmps.GetAt(ii) == '~')
										col->m_ihead++;
								}
							}

							strcpy_s(buf, sizeof(buf), tmps.GetString());
							for (ii = 0; ii < col->m_ihead; ii++)
							{
								hRc.left  += 2;
								hRc.right -= 2;
								hRc.top    = hRc.bottom+1;
								hRc.bottom = cRc.top + cRc.Height() * (ii+1) / col->m_ihead;

								ptr = strchr(buf, '~');
								if (ptr != NULL)
									len = ptr - buf;
								else
									len = lstrlen(buf);

								tmps = CString(buf, len);
								if (!ii && (m_cellR[col->m_ref].properties & PR_SORT) && (col->m_ref == m_sortc) && (m_dir > 0))
									drawHead(dc, jj, tmps, hRc, format);
								else
									dc->DrawText(tmps, hRc, format);

								if (ptr != NULL)
									lstrcpy(&buf[0], ++ptr); 
							}
						}

						if ((m_form->attr2 & GO_MERGE && kk == m_headN-1) || (!(m_form->attr2 & GO_MERGE) && kk == m_multiN-1))
							drawHead(dc, jj, cRc, tmps);	// check, combo
						if (!(m_form->attr2 & GO_X3D))
							drawEdge(dc, cRc, EDGE_RAISED, BF_RECT);
						else if (m_form->attr2 & GO_3DHEAD)
							drawHead(dc, cRc);	// head border
						else
						{
							setPen(dc, m_bRGB);
							if (jj != colidx && cRc.left != m_pRc.left)
								cRc.left--;
							if (kk != 0 && cRc.top > m_pRc.top)
								cRc.top--;
							if (m_form->attr2 & GO_LINE)
							{
								dc->SelectStockObject(NULL_BRUSH);
								dc->Rectangle(cRc);
							}
							else
							{
								if (!first)
								{
									dc->MoveTo(cRc.left, cRc.top);
									dc->LineTo(cRc.left, cRc.bottom);
								}
								dc->MoveTo(cRc.left, cRc.top);
								dc->LineTo(cRc.right, cRc.top);
								dc->MoveTo(cRc.left, cRc.bottom-1);
								dc->LineTo(cRc.right, cRc.bottom-1);
							}
						}

						if ((m_cellR[col->m_ref].properties & PR_MARK)
							&& ((m_form->attr2 & GO_MERGE && kk == m_headN-1) || (!(m_form->attr2 & GO_MERGE) && kk == m_multiN-1)))
							drawMark(dc, cRc);
					}
				}		// head drawing
				else
				{		// data drawing
	  				if ((m_edit.GetSize()/m_totCols) > index)
						tmps = m_edit.GetAt(index * m_totCols + col->m_ref);
					else
						tmps = CString(' ', m_cellR[col->m_ref].size);

					tRc.bottom = (int)(dy - m_slit);
					if (kk == m_showRows-1)
					{
						if (m_hscroll)
						{
							if (!m_hscr.IsRectEmpty())
								tRc.bottom = min(tRc.bottom, m_hscr.top - m_slit);
							else
								tRc.bottom = min(tRc.bottom, sRc.bottom - m_slit);
						}
						else
						{
							tRc.bottom = min(tRc.bottom, sRc.bottom - m_slit);
							if (tRc.bottom <sRc.bottom-m_slit && sRc.bottom-m_slit-tRc.bottom < 3)
								tRc.bottom = sRc.bottom - m_slit;
						}
					}
					if (tRc.Height() <= (int)(m_cy / 2))
						noWrite = true;

					dc->MoveTo(tRc.left, tRc.bottom);
					if ((m_focus || m_form->attr2 & GO_ALWAYS)
						&& ((m_form->isEdit == FI_RBAR && m_stRow+kk-skipN == m_index)
						|| (m_form->isEdit == FI_RREC && (m_stRow+kk-skipN)/m_multiN+1 == m_curpos.m_row)))
					{
						pRGB = m_rRGB;
						if (m_form->attr2 & GO_FTEXT)
						{
							if (m_cellR[col->m_ref].attr & FA_CONTRAST)
							{
								int	s, offs;
								s = getSIGN(tmps, offs);
								if ((m_cellR[col->m_ref].attr2 & SA_CONTRAST) == SA_PAINT && (s == ct_uLimit || s == ct_lLimit))
									tRGB = col->m_pRGBs.GetAt(m_index/m_multiN);
								else
									tRGB = col->m_tRGBs.GetAt(m_index/m_multiN);
							}
							else
								tRGB = col->m_tRGBs.GetAt(m_index/m_multiN);
						}
						else
							tRGB = getRGB(cWHITE);
					}
					else
					{
						if (index > col->m_pRGBs.GetUpperBound())
						{
							pRGB = col->m_pRGBs.GetAt(col->m_pRGBs.GetUpperBound());
							tRGB = col->m_tRGBs.GetAt(col->m_tRGBs.GetUpperBound());
						}
						else
						{
							pRGB = col->m_pRGBs.GetAt(index);
							tRGB = col->m_tRGBs.GetAt(index);
						}
					}

					if (isReverseCell(index, jj))		// selected cell
					{
						pRGB = getRGB(cBLACK);
						tRGB = getReverseColor(tRGB);
					}

					brush = setBrush(dc, pRGB);
					setTextColor(dc, tRGB);
					setPaintColor(dc, pRGB);
					dc->FillRect(tRc, brush);

					if (!noWrite)
					{
						tRc.left++;
						tRc.right--;
						hRc = tRc;
						hRc.left  += 3;
						hRc.right -= (int)(m_fWidth/2);

	  					if (col->m_valids.GetSize() > index)
							valid = col->m_valids.GetAt(index);
						else
							valid = col->m_valids.GetAt(col->m_valids.GetUpperBound());

						if (valid)
						{
							switch (m_cellR[col->m_ref].iok)
							{
							case EIO_INPUT:
							case EIO_INOUT:
								switch (m_cellR[col->m_ref].kind)
								{
								case CK_COMBO:
									drawCombo(dc, tRc, col, jj, m_stRow+kk, tmps, pRGB, tRGB);
									break;
								case CK_CHECK:
									drawCheck(dc, tRc, col, jj, m_stRow+kk, tmps, pRGB, tRGB);
									break;
								case CK_GRAPH:
									break;
								default:
									drawEdit(dc, tRc, col, jj, m_stRow+kk, tmps, pRGB, tRGB);
									break;
								}
								break;

							default:
								switch (m_cellR[col->m_ref].kind)
								{
								case CK_BUTTON:
									drawButton(dc, tRc, col, jj, m_stRow+kk);
									break;
								case CK_COMBO:
									tmps.TrimLeft();
									tmps.TrimRight();
								/*	if (!tmps.IsEmpty())
									{
										ll = getIndex(jj, index);
										if (ll < col->m_display.GetSize())
											tmps = col->m_display.GetAt(ll);
									}	*/
								default:
									setFont(dc, (m_cellR[col->m_ref].properties & PR_BOLD) ? 1 : 0);
									setPen(dc, pRGB);
									setBrush(dc, tRGB);
									drawPolygonText(dc, hRc, tmps, m_cellR[col->m_ref].alignC, m_cellR[col->m_ref].type);
									break;
								}
								break;
							}
						}
						else
						{
							tmps = _T("");
							switch (m_cellR[col->m_ref].iok)
							{
							case EIO_INPUT:
							case EIO_INOUT:
								switch (m_cellR[col->m_ref].kind)
								{
								case CK_COMBO:
									drawCombo(dc, tRc, col, jj, m_stRow+kk, tmps, pRGB, tRGB);
									break;
								case CK_CHECK:
									drawCheck(dc, tRc, col, jj, m_stRow+kk, tmps, pRGB, tRGB);
									break;
								case CK_GRAPH:
									break;
								default:	// EDIT
									drawEdit(dc, tRc, col, jj, m_stRow+kk, tmps, pRGB, tRGB);
									break;
								}
								break;

							default:
								if (m_cellR[col->m_ref].kind == CK_BUTTON)
									drawButton(dc, tRc, col, jj, m_stRow+kk);
								break;
							}
						}

						tRc.left--;
						tRc.right++;
						if ((m_stRow+kk-skipN == m_index) && m_focus && jj == m_curpos.m_col-1)
							m_cell = tRc;
					}

					setPen(dc, m_bRGB);
					if ((int)dx <= sRc.Width())
					{
						tRc.right--;
						if (!(m_form->attr2 & GO_XHLINE))
						{
							if (m_cellR[col->m_ref].iok == EIO_OUTPUT && m_cellR[col->m_ref].kind == CK_NORMAL
								&& (int)m_cellR[col->m_ref].vals[2] > 0
								&& (m_stRow+kk-skipN)%(int)m_cellR[col->m_ref].vals[2] != (int)m_cellR[col->m_ref].vals[2]-1)
							{
								setPen(dc, pRGB);
							}
							dc->LineTo(tRc.right, tRc.bottom);
							setPen(dc, m_bRGB);
						}
						if (!(m_form->attr2 & GO_XVLINE) && !first)
						{
						//	dc->MoveTo(tRc.right, tRc.top);
						//	dc->LineTo(tRc.right, tRc.bottom);
							dc->MoveTo(tRc.left-1, tRc.top);
							dc->LineTo(tRc.left-1, tRc.bottom);
						}
						if (m_move == mvItem && m_movepos.m_row == m_stRow+kk-skipN && m_movepos.m_col == jj
							&& m_cellR[col->m_ref].cursor == CS_POP)
						{
							CRect	ppRc = tRc;
							if (ppRc.left == m_pRc.left)
								ppRc.left++;
							if (ppRc.bottom == m_pRc.bottom)
								ppRc.bottom--;
							drawEdge(dc, ppRc, EDGE_RAISED, BF_RECT);
						}
					}
					else
					{
						if (!(m_form->attr2 & GO_XHLINE) && !first)
							dc->LineTo(tRc.right, tRc.bottom);
						if (!(m_form->attr2 & GO_XVLINE) && !first)
						{
							dc->MoveTo(tRc.left-1, tRc.top);
							dc->LineTo(tRc.left-1, tRc.bottom);
						}
						if (m_move == mvItem && m_movepos.m_row == m_stRow+kk-skipN && m_movepos.m_col == jj
							&& m_cellR[col->m_ref].cursor == CS_POP)
						{
							CRect	ppRc = tRc;
							if (ppRc.left == m_pRc.left)
								ppRc.left++;
							if (ppRc.bottom == m_pRc.bottom)
								ppRc.bottom--;
							drawEdge(dc, ppRc, EDGE_RAISED, BF_RECT);
						}
					}
				}
				first = false;
				if ((int)dx >= sRc.Width())
					break;
			}	// jj loop

			if ((int)dx < sRc.Width())
			{
				tRc.left = tRc.right;
				if ((m_form->attr2 & GO_MERGE && kk >= m_headN)
					|| (!(m_form->attr2 & GO_MERGE) && kk >= m_multiN))
					tRc.left++;
				tRc.right = sRc.right;
				tRc.bottom++;
				brush = setBrush(dc);	//, getRGB(cBASIC));
             			dc->FillRect(tRc, brush);
			}

			linecnt++;
			colidx += colcnt;

			if (linecnt == m_multiN)
				linecnt = 0;
			if (colidx == m_totCols)
				colidx = 0;

			tmps = _T("");
			if (dy == sRc.bottom)
			{
				noWrite = false;
				break;
			}
		}	// kk loop

	//	if ((int)dy < sRc.bottom && index >= m_totRows-1)
		if ((int)dy < sRc.bottom && (((m_form->attr2 & GO_VALID) && index >= m_validRows-1) || (!(m_form->attr2 & GO_VALID) && index >= m_totRows-1)))
		{
			tRc = sRc;
			tRc.top = (int)dy;
			brush = setBrush(dc);	//, getRGB(cBASIC));
             		dc->FillRect(tRc, brush);
		}
	}
	rects.RemoveAll();

	sRc = m_pRc;
//	if (!(m_hscroll || m_vscroll))
//		sRc.bottom = min(sRc.bottom, (int)dy+3);

	if (m_focus && m_form->isEdit != FI_CELL && !m_cell.IsRectEmpty() && m_state == stEdit)
	{
		tRc = m_cell;
		tRc.right--;
		setPen(dc, getRGB(cBLACK), 2);
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(m_cell);
	}

	if (!(m_form->attr2 & GO_X3D))
		drawEdge(dc, sRc, EDGE_SUNKEN, BF_RECT); 
	else
	{
		setPen(dc, m_bRGB);
		if (m_form->attr2 & GO_LINE)
		{
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(sRc);
		}
		else
		{
			dc->MoveTo(sRc.left, sRc.top);
			dc->LineTo(sRc.right, sRc.top);
		//	if (m_form->attr2 & GO_BOTTOM)
			{
				dc->MoveTo(sRc.left, sRc.bottom);
				dc->LineTo(sRc.right, sRc.bottom);
			}
		}
	}

	if ((m_focus || m_form->attr2 & GO_ALWAYS) && (m_form->isEdit == FI_CELL || m_form->isEdit == FI_RBOX))
	{
		tRc = m_cell;
		if (m_form->isEdit == FI_RBOX && m_totCols)
		{
			tRc.left  = m_pRc.left;
			if (!(m_form->attr2 & GO_X3D))
				tRc.left += 2;
			if ((int)dx < sRc.Width())
				tRc.right = tRc.left + (int)dx;
			else
			{
				tRc.right = m_pRc.right;
				if (!(m_form->attr2 & GO_X3D))
					tRc.right -= 2;
				if (m_vscroll)
					tRc.right -= (int)(szSCROLL * m_hR);
			}
		}
		else if (m_form->isEdit == FI_CELL && !tRc.IsRectEmpty())
			tRc.right--;
		setPen(dc, getRGB(cBLACK));
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(tRc);
	}

	Cimage*	image;
	if (m_hscroll)
	{
		sRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			sRc.DeflateRect(2, 2);
		if (m_vscroll)
			sRc.right -= (int)(szSCROLL * m_hR);
		if (sRc.Height() > (int)(szSCROLL * m_vR))
			sRc.top = sRc.bottom - (int)(szSCROLL * m_vR);

		sRc1 = sRc2 = sRc;
		if (sRc.Width() < (int)(2 * szSCROLL * m_hR))
		{
       			sRc1.right = sRc.left + sRc.Width()/2;
			sRc2.left  = sRc.left + sRc.Width()/2 + 1;
       		}
       		else
     		{
			sRc1.right = sRc.left + (int)(szSCROLL * m_hR);
			sRc2.left  = sRc.right - (int)(szSCROLL * m_hR);

			if (m_totWidth > 0)
			{
				m_hgap = sRc.Width() - (szSCROLL * m_hR) * 2;
				m_hgap = m_hgap / m_totWidth;
			}
			else
				m_hgap = 0;
             	}
		m_hscr = sRc;
		if (m_vscroll)
			sRc.right += (int)(szSCROLL * m_hR);
		brush = setBrush(dc, getRGB(cLGRAY));
             	dc->FillRect(sRc, brush);
		sRc = m_hscr;

		index = (m_click == ckScrollLeft) ? PI_DOWN : PI_DEFAULT;
		if (m_scrolls.Lookup(isLeft, image) && image->GetValid(index))
			image->DrawPicture(dc, sRc1, AL_STRETCH, m_axform->m_oldVers, index);
		else
		{
			if (m_click == ckScrollLeft)
             			dc->DrawFrameControl(sRc1, DFC_SCROLL, DFCS_SCROLLLEFT|DFCS_PUSHED);
			else
             			dc->DrawFrameControl(sRc1, DFC_SCROLL, DFCS_SCROLLLEFT);
		}

		if (m_hthumb.IsRectEmpty())
		{
			sRc1.left = sRc1.right + (int)(m_scrolledWidth * m_hgap);
			if (m_totWidth == m_showWidth - m_fixedWidth + m_scrolledWidth)
				sRc1.right = sRc2.left;
			else
				sRc1.right = sRc1.left + (int)(m_hgap * (m_showWidth - m_fixedWidth));
			sRc1.right = min(sRc2.left, sRc1.right);
			m_hthumb = sRc1;
		}
		if (m_scrolls.Lookup(isLeft, image))
		{
			setPen(dc, getRGB(cSCRLN));
			dc->FillSolidRect(m_hthumb, getRGB(cSCRCL));
			dc->SelectStockObject(NULL_BRUSH);
			dc->Rectangle(m_hthumb);
		}
		else
			dc->DrawFrameControl(m_hthumb, DFC_BUTTON, DFCS_BUTTONPUSH);

		index = (m_click == ckScrollRight) ? PI_DOWN : PI_DEFAULT;
		if (m_scrolls.Lookup(isRight, image) && image->GetValid(index))
			image->DrawPicture(dc, sRc2, AL_STRETCH, m_axform->m_oldVers, index);
		else
		{
			if (m_click == ckScrollRight)
				dc->DrawFrameControl(sRc2, DFC_SCROLL, DFCS_SCROLLRIGHT | DFCS_PUSHED);
			else
				dc->DrawFrameControl(sRc2, DFC_SCROLL, DFCS_SCROLLRIGHT);
		}
	}	// end of drawing Hscroll 

	sRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		sRc.DeflateRect(2, 2);
	if (m_hscroll)
		sRc.bottom -= (int)(szSCROLL * m_vR);
	if  (m_vscroll)
	{
		float	bH;
		CRect	pRc1, pRc2, pRc3;
		int	bN = (m_form->attr2 & GO_PAGE) ? 5 : 2;

	/*	if ((m_form->attr2 & GO_FIX) && (dx < sRc.right-(int)(szSCROLL * m_hR)))
		{
			sRc.left  = (int)(dx+1);
			sRc.right = sRc.left + (int)(szSCROLL * m_hR);
		}
		else */ if (sRc.Width() > (int)(szSCROLL * m_hR))
			sRc.left  = sRc.right  - (int)(szSCROLL * m_hR);

		if (m_form->attr & FA_ENUM)
		{
			sRc.top += ((int)m_cy + 1);
			if (m_form->attr2 & GO_3DHEAD)
				sRc.top++;
		}
		sRc1 = sRc2 = sRc;
		pRc1 = pRc2 = pRc3 = sRc;
		if (sRc.Height() < (int)(bN * szSCROLL * m_vR))
         	{
			bH = (float)sRc.Height() / (float)bN;
          		sRc1.bottom = sRc.top + (int)bH;
			if (m_form->attr2 & GO_PAGE)
			{
				pRc1.top    = sRc1.bottom;
				pRc1.bottom = pRc1.top + (int)bH;
				pRc2.top    = pRc1.bottom;
				pRc2.bottom = pRc2.top + (int)bH;
				pRc3.top    = pRc2.bottom;
				pRc3.bottom = pRc3.top + (int)bH;
           			sRc2.top    = pRc3.bottom;
			}
			else
				sRc2.top    = sRc1.bottom;
           	}
            	else 
             	{
			bH = szSCROLL * m_vR;
              		sRc1.bottom = sRc.top + (int)bH;
			if (m_form->attr2 & GO_PAGE)
			{
				pRc1.top    = sRc1.bottom;
				pRc1.bottom = pRc1.top + (int)bH;
				pRc2.top    = pRc1.bottom;
				pRc2.bottom = pRc2.top + (int)bH;
				pRc3.top    = pRc2.bottom;
				pRc3.bottom = pRc3.top + (int)bH;
			}
                 	sRc2.top    = sRc.bottom - (int)bH;
             	}

		m_vscr = sRc;
		if (m_form->attr2 & GO_PAGE)
		{
			m_pscr = m_vscr = sRc;
			m_pscr.top = pRc1.top;
			m_pscr.bottom = pRc3.bottom;
		}
		brush = setBrush(dc, getRGB(cLGRAY));
             	dc->FillRect(sRc, brush);

		if (m_validRows > 0 && (sRc.Height() >= (int)(bN * szSCROLL * m_vR)))
		{
			m_vgap = sRc.Height() - (szSCROLL * m_vR) * bN;
			m_vgap = m_vgap / (m_validRows * m_multiN);
		}
		else
			m_vgap = 0;

		if (!m_enableScroll)
		{
			if (m_scrolls.Lookup(isUp, image) && image->GetValid(PI_OFF))
				image->DrawPicture(dc, sRc1, AL_STRETCH, m_axform->m_oldVers, PI_OFF);
			else
				dc->DrawFrameControl(sRc1, DFC_SCROLL, DFCS_SCROLLUP|DFCS_INACTIVE);

			if (m_scrolls.Lookup(isDown, image) && image->GetValid(PI_OFF))
				image->DrawPicture(dc, sRc2, AL_STRETCH, m_axform->m_oldVers, PI_OFF);
			else
				dc->DrawFrameControl(sRc2, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_INACTIVE);

			if (m_form->attr2 & GO_PAGE)
			{
				drawPage(dc, pRc1, isPageUp, true, false);	// page up
				drawPage(dc, pRc2, isHome,   true, false);	// home
				drawPage(dc, pRc3, isPageDn, true, false);	// page down
			}
		}
		else
		{
			index = (m_click == ckScrollUp) ? PI_DOWN : PI_DEFAULT;
			if (m_scrolls.Lookup(isUp, image) && image->GetValid(index))
				image->DrawPicture(dc, sRc1, AL_STRETCH, m_axform->m_oldVers,index);
			else
			{
				if (m_click == ckScrollUp)
             				dc->DrawFrameControl(sRc1, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED);
				else
					dc->DrawFrameControl(sRc1, DFC_SCROLL, DFCS_SCROLLUP);
			}

			if (m_vthumb.IsRectEmpty())
			{
				CRect	thumb;

				thumb = sRc1;
				if (m_form->attr2 & GO_PAGE)
					thumb.top = pRc3.bottom + (int)(m_stRow * m_vgap);
				else
					thumb.top = thumb.bottom + (int)(m_stRow * m_vgap);

				if (m_stRow + (m_showRows - m_headN * m_multiN) == (m_validRows * m_multiN))
					thumb.bottom = sRc2.top;
				else
					thumb.bottom = thumb.top + (int)(m_vgap * (m_showRows - m_headN * m_multiN));
				thumb.bottom = min(thumb.bottom, sRc2.top);

				bH++; bH /= 2;
				if (thumb.Height() < bH)
				{
					m_vgap  = sRc.Height() - (szSCROLL * m_vR) * bN;
					m_vgap -= bH;
					m_vgap /= (m_validRows * m_multiN - (m_showRows - m_headN * m_multiN));

					thumb = sRc1;
					if (m_form->attr2 & GO_PAGE)
						thumb.top = pRc3.bottom + (int)(m_stRow * m_vgap);
					else
						thumb.top = thumb.bottom + (int)(m_stRow * m_vgap);

					if (m_stRow + (m_showRows - m_headN * m_multiN) == (m_validRows * m_multiN))
						thumb.bottom = sRc2.top;
					else
						thumb.bottom = thumb.top + (int)bH;
					thumb.bottom = min(thumb.bottom, sRc2.top);
				}
				m_vthumb = thumb;
			}
			if (m_scrolls.Lookup(isUp, image))
			{
				setPen(dc, getRGB(cSCRLN));
				dc->FillSolidRect(m_vthumb, getRGB(cSCRCL));
				dc->SelectStockObject(NULL_BRUSH);
				dc->Rectangle(m_vthumb);
			}
			else
				dc->DrawFrameControl(m_vthumb, DFC_BUTTON, DFCS_BUTTONPUSH);

			index = (m_click == ckScrollDown) ? PI_DOWN : PI_DEFAULT;
			if (m_scrolls.Lookup(isDown, image) && image->GetValid(index))
				image->DrawPicture(dc, sRc2, AL_STRETCH, m_axform->m_oldVers, index);
			else
			{
				if (m_click == ckScrollDown)
					dc->DrawFrameControl(sRc2, DFC_SCROLL, DFCS_SCROLLDOWN | DFCS_PUSHED);
				else
					dc->DrawFrameControl(sRc2, DFC_SCROLL, DFCS_SCROLLDOWN);
			}

			if (m_form->attr2 & GO_PAGE)
			{
				drawPage(dc, pRc1, isPageUp, (m_click == ckPageUp)   ? false : true);
				drawPage(dc, pRc2, isHome,   (m_click == ckFirst)    ? false : true);
				drawPage(dc, pRc3, isPageDn, (m_click == ckPageDown) ? false : true);
			}
		}
	}
	else if (m_form->attr2 & GO_PAGE)
	{
		float	bH;
		CRect	pRc1, pRc2, pRc3;

		if (sRc.Width() > (int)(szSCROLL * m_hR))
			sRc.left  = sRc.right  - (int)(szSCROLL * m_hR);

		if (m_form->attr & FA_ENUM)
		{
			sRc.top += ((int)m_cy + 1);
			if (m_form->attr2 & GO_3DHEAD)
				sRc.top++;
		}

		pRc1 = pRc2 = pRc3 = sRc;
		bH = (float)sRc.Height() / (float)7.0;
          	pRc1.bottom = sRc.top + (int)(bH * 3);
		pRc2.top    = pRc1.bottom;
		pRc2.bottom = pRc2.top + (int)bH;
		pRc3.top    = pRc2.bottom;

		m_pscr = sRc;
		brush = setBrush(dc, getRGB(cLGRAY));
             	dc->FillRect(sRc, brush);

		drawPage(dc, pRc1, isPageUp, (m_click == ckPageUp)   ? false : true, (m_stat & statPREV) ? true : false);
		drawPage(dc, pRc2, isHome,   (m_click == ckFirst)    ? false : true);
		drawPage(dc, pRc3, isPageDn, (m_click == ckPageDown) ? false : true, (m_stat & statNEXT) ? true : false);
	}
	if (m_form->attr & FA_ENUM)
	{
		int	image = PI_DEFAULT;

		m_editRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			m_editRc.DeflateRect(2, 2);
		if (m_editRc.Width() > (int)(szSCROLL * m_hR))
			m_editRc.left = m_editRc.right  - (int)(szSCROLL * m_hR);
		m_editRc.bottom = m_editRc.top + (int)m_cy + 1;
		if (m_form->attr2 & GO_3DHEAD)
			m_editRc.bottom++;
		if (m_form->attr2 & GO_X3D)
			m_editRc.DeflateRect(0, 1, 1, 0);

		brush = setBrush(dc, getRGB(cHEADB));
             	dc->FillRect(m_editRc, brush);
		if (m_click == ckEdit)
			image = PI_DOWN;
		if (m_epic && m_epic->GetValid(image))
			m_epic->DrawPicture(dc, m_editRc, AL_STRETCH, m_axform->m_oldVers, image);
	}

	dc->RestoreDC(ndc);
	m_drawing = false;
}

void CfmGrid::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	m_mousedown = down;

	if (down)
		m_axform->m_cursor = pt;

	if (m_stCol < 0 || !(m_form->properties & PR_ENABLE) || m_totCols <= 0)
		return;

//	if (m_form->isEdit == FI_NONE)
//		m_state = stNone;
	if (down)
	{
		if (m_state == stEdit)
		{
			m_edstate = esNone;
			m_axform->m_view->HideCaret();
		}
		m_state  = stFocus;
		if (m_select || m_move == mvSel)
		{
			m_select = false;
			m_move   = mvNone;
			invalidateRect(&m_pRc, false);
		}
	}
	else
	{
		if (m_move == mvSel)
		{
			m_select = true;
			m_state  = stFocus;
		}
		else
		{
			m_select = false;
		//	m_move   = mvNone;
		}
		switch (m_click)
		{
		case ckItem:
		case ckCombo:
		case ckOrigin:
		case ckEdit:
			break;
		default:
			m_state = stNone;
			break;
		}
	}

	switch (m_state)
	{
	case stEdit:
		if (!lbuttonEdit(down, pt, result))
		{
			lbutton(down, pt, result);
			m_statechange = true;
		}
		else
			m_statechange = false;
		break;
	case stCombo:
		if (!lbuttonCombo(down, pt, result))
		{
			lbutton(down, pt, result);
			m_statechange = true;
		}
		else
			m_statechange = false;
		break;
	case stCheck:
		if (!lbuttonCheck(down, pt, result))
		{
			lbutton(down, pt, result);
			m_statechange = true;
		}
		else
			m_statechange = false;
		break;
	case stButton:
		if (!lbuttonButton(down, pt, result))
		{
			lbutton(down, pt, result);
			m_statechange = true;
		}
		else
			m_statechange = false;
		break;
	case stFocus:
		lbutton(down, pt, result);
		if (down)
		{
			switch (m_state)
			{
			case stEdit:
				lbuttonEdit(down, pt, result);	break;
			case stCombo:
				lbuttonCombo(down, pt, result);	break;
			case stCheck:
				lbuttonCheck(down, pt, result);	break;
			case stButton:
				lbuttonButton(down, pt, result);break;
			default:
				if (result != RC_ITEM)
					m_state = stNone;
				if (down)
					m_result = result;
				break;
			}
		}
		break;
	case stNone:
	default:
		lbutton(down, pt, result);
		if (down)
			m_result = result;
		if (m_click == ckItem && down)
			m_state = stFocus;
		break;
	}

	if (!down)
		m_click = ckNone;
}

void CfmGrid::OnRButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_stCol < 0 || !(m_form->properties & PR_ENABLE) || m_totCols <= 0 || !down)
		return;

	switch (m_state)
	{
	case stEdit:
	case stCombo:
	case stCheck:
	case stButton:	break;
	case stFocus:
		rbutton(down, pt, result);
		if (result != RC_ITEM)
			m_state = stNone;
		break;
	case stNone:
	default:
		rbutton(down, pt, result);
		break;
	}
}

void CfmGrid::OnDblClick(CPoint pt, int& result)
{
	result = RC_NOP;
	m_select = false;
	m_move   = mvNone;

	DoListBox(false);
	if (m_vscroll && m_enableScroll && m_vscr.PtInRect(pt))
	{
		CRect	tmpRc1, tmpRc2;
		int	stRow = m_stRow;

		tmpRc1 = tmpRc2 = m_vscr;
		if (m_vscr.Height() <= (int)(2 * szSCROLL * m_vR))
		{
			tmpRc1.bottom = tmpRc1.top + (m_vscr.Height()) / 2;
			tmpRc2.top    = tmpRc2.top + (m_vscr.Height()) / 2 + 1;
		}
		else 
		{
			tmpRc1.bottom = tmpRc1.top + (int)(szSCROLL * m_vR);
			tmpRc2.top    = tmpRc2.bottom - (int)(szSCROLL * m_vR);
		}

		if (tmpRc1.PtInRect(pt))
		{
			if (m_stRow <= 0)
			{
				if (!m_writingAll && m_save == svExchange)
				{
					if (!m_useInfo || (m_useInfo && (m_stat & statPREV)))
						result = RC_SCRUP;	// exFirst
					else
						result = RC_UPC;
				}
			}
			else
			{
				result = RC_UPC;
				m_stRow--;
			}
		}
		else if (tmpRc2.PtInRect(pt))
		{
			if (m_stRow + m_showRows - (m_headN * m_multiN) >= (m_validRows * m_multiN))
			{
				if (!m_writingAll && m_save == svExchange)
				{
					if (!m_useInfo || (m_useInfo && (m_stat & statNEXT)))
						result = RC_SCRDN;	// exLast
					else
						result = RC_DNC;
				}
			}
			else
			{
				result = RC_DNC;
				m_stRow++;
			}
		}

		if (stRow != m_stRow)
		{
			tmpRc1 = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				tmpRc1.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					tmpRc1.top += (4 * m_multiN);	// head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				tmpRc1.top += m_multiN;			// head extra space
			tmpRc1.top  += (int)((m_cy+m_slit) * m_headN * m_multiN);
			tmpRc1.right = m_vscr.left;
			if (m_hscroll)
				tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);
			tmpRc2 = m_vscr;
			tmpRc2.top    += (int)(szSCROLL * m_vR);
			tmpRc2.bottom -= (int)(szSCROLL * m_vR);
			m_vthumb.SetRectEmpty();
			invalidateRect(&tmpRc2, false);
		}
	}
	else if (m_pRc.PtInRect(pt))
	{
		CRect	tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 2);
		tRc.bottom = tRc.top + (int)(m_cy * m_headN * m_multiN);
		if (m_vscroll)
			tRc.right = m_vscr.left;
		if (!(m_form->attr2 && GO_XHEAD))
		{
			if (!(m_form->attr2 & GO_X3D))
				tRc.bottom += (4 * m_multiN);
			else
				tRc.bottom += m_multiN;
		}
		if (tRc.PtInRect(pt) && m_headN != 0)
		{
			result = RC_HEAD;
			return;
		}

		tRc.top    = tRc.bottom;
		tRc.bottom = m_hscroll ? m_hscr.top : m_pRc.bottom;
		if (tRc.PtInRect(pt))
			result = RC_ITEM;
	}
}

bool CfmGrid::OnRepeat(CPoint pt, int& result)
{
	CRect	tRc;
	float	tWidth = 0;
	int	bN = 0, ii = 0;

	result = RC_NOP;
	switch (m_click)
	{
	case ckPageUp:
		tRc = m_pscr;
		if (m_form->attr2 & GO_PAGE)
		{
			tWidth = (float)tRc.Height() / (float)7.0;
			tRc.bottom = tRc.top + (int)(tWidth*3);
		}
		else
			tRc.bottom = tRc.top + (tRc.Height())/3;
		
		if (!tRc.PtInRect(pt))
			return false;

		if (m_stRow <= 0 || m_form->attr2 * GO_PAGE)
		{
			if (!m_writingAll && m_save == svExchange)
			{
				if (!m_useInfo || (m_useInfo && (m_stat & statPREV)))
					result = RC_PAGEUP;	// exFirst
				else
					result = RC_UPC;
			}
			m_result = result;
			return false;
		}
		keyPrior(result);
		result = RC_UPC;
		break;

	case ckPageDown:
		tRc = m_pscr;
		if (m_form->attr2 & GO_PAGE)
		{
			tWidth = (float)tRc.Height() / (float)7.0;
			tRc.top   += (int)(tWidth*3);
			tRc.bottom = tRc.top + (int)tWidth;
		}
		else
			tRc.top += (tRc.Height()/3*2);

		if (!tRc.PtInRect(pt))
			return false;

		if ((m_stRow + m_showRows - (m_headN * m_multiN) >= (m_validRows * m_multiN))
			|| m_form->attr2 & GO_PAGE)
		{
			if (!m_writingAll && m_save == svExchange)
			{
				if (!m_useInfo || (m_useInfo && (m_stat & statNEXT)))
					result = RC_PAGEDN;	// exLast
				else
					result = RC_DNC;
			}
			m_result = result;
			return false;
		}
		keyNext(result);
		result = RC_DNC;
		break;

	case ckScrollUp:
		tRc = m_vscr;
		bN  = (m_form->attr2 & GO_PAGE) ? 5 : 2;
		if (tRc.Height() < (int)(bN * szSCROLL * m_vR))
          		tRc.bottom = tRc.top + (tRc.Height()) / bN;
            	else 
              		tRc.bottom = tRc.top + (int)(szSCROLL * m_vR);

		if (!tRc.PtInRect(pt))
			return false;

		if (m_stRow <= 0)
		{
			if (!m_writingAll && m_save == svExchange)
			{
				if (!m_useInfo || (m_useInfo && (m_stat & statPREV)))
					result = RC_SCRUP;	// exFirst
				else
					result = RC_UPC;
			}
			m_result = result;
			return false;
		}

		result = RC_UPC;
		m_stRow--;
		tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
		{
			tRc.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += (4 * m_multiN);		// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;				// head extra space
		tRc.top  += (int)((m_cy+m_slit) * m_headN * m_multiN);
		tRc.right = m_vscr.left;
		if (m_hscroll)
			tRc.bottom = m_hscr.top;
		invalidateRect(&tRc, false);
		tRc = m_vscr;
		tRc.top    += (int)(szSCROLL * m_vR);
		tRc.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tRc, false);
		break;

	case ckScrollDown:
		tRc = m_vscr;
		bN  = (m_form->attr2 & GO_PAGE) ? 5 : 2;
		if (tRc.Height() < (int)(bN * szSCROLL * m_vR))
          		tRc.top = tRc.bottom - (tRc.Height()) / bN;
            	else 
              		tRc.top = tRc.bottom - (int)(szSCROLL * m_vR);

		if (!tRc.PtInRect(pt))
			return false;

		if (m_stRow + m_showRows - (m_headN * m_multiN) >= (m_validRows * m_multiN))
		{
			if (!m_writingAll && m_save == svExchange)
			{
				if (!m_useInfo || (m_useInfo && (m_stat & statNEXT)))
					result = RC_SCRDN;	// exLast
				else
					result = RC_DNC;
			}
			m_result = result;
			return false;
		}

		result = RC_DNC;
		m_stRow++;
		tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
		{
			tRc.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += (4 * m_multiN);		// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;				// head extra space
		tRc.top  += (int)((m_cy+m_slit) * m_headN * m_multiN);
		tRc.right = m_vscr.left;
		if (m_hscroll)
			tRc.bottom = m_hscr.top;
		invalidateRect(&tRc, false);
		tRc = m_vscr;
		tRc.top    += (int)(szSCROLL * m_vR);
		tRc.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tRc, false);
		break;

	case ckScrollLeft:
		tRc = m_hscr;
		if (tRc.Width() < (int)(2 * szSCROLL * m_hR))
          		tRc.right = tRc.left + tRc.Width()/2;
             	else
			tRc.right = tRc.left + (int)(szSCROLL * m_hR);

		if (!tRc.PtInRect(pt))
			return false;

		if (m_scrolledWidth <= 0)
		{
			m_result = result;
			return false;
		}

		while (true)
		{
			m_stCol--;
			if (m_columns.GetAt(m_stCol)->m_visible || m_stCol <= m_form->type)
				break;
		}
		m_stCol = max(m_stCol, m_form->type);
		if (m_form->type == m_totCols)
			m_stCol = 0;
		m_scrolledWidth = 0;
		for (ii = m_form->type; ii < m_stCol; ii++)
		{
			if (m_columns.GetAt(ii)->m_visible)
				m_scrolledWidth += m_columns.GetAt(ii)->m_width;
		}

		tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 2);
		if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
			m_vthumb.SetRectEmpty();
		else
		{
			if (m_vscroll)
				tRc.right = m_vscr.left;
			if (m_form->attr2 & GO_PAGE)
				tRc.right = m_pscr.left;
		}
		tRc.bottom = m_hscr.top;
		invalidateRect(&tRc, false);

		tRc = m_hscr;
		m_hthumb.SetRectEmpty();
		tRc.left  += (int)(szSCROLL * m_hR);
		tRc.right -= (int)(szSCROLL * m_hR);
		invalidateRect(&tRc, false);
		break;

	case ckScrollRight:
		tRc = m_hscr;
		if (tRc.Width() < (int)(2 * szSCROLL * m_hR))
          		tRc.left = tRc.right - tRc.Width()/2;
             	else
			tRc.left = tRc.right - (int)(szSCROLL * m_hR);

		if (!tRc.PtInRect(pt))
			return false;

		for (ii = m_totCols-1; ii >= m_stCol; ii--)
			tWidth += m_columns.GetAt(ii)->m_width;

		if (tWidth <= m_showWidth - m_fixedWidth)
		{
			m_result = result;
			return false;
		}

		while (true)
		{
			if (m_stCol >= (m_totCols - 1))
				break;
			m_stCol++;
			if (m_columns.GetAt(m_stCol)->m_visible)
				break;
		}

		m_stCol = min(m_stCol, m_totCols-1);
		if (m_form->type == m_totCols)
			m_stCol = 0;
		m_scrolledWidth = 0;
		for (ii = m_form->type; ii < m_stCol; ii++)
		{
			if (m_columns.GetAt(ii)->m_visible)
				m_scrolledWidth += m_columns.GetAt(ii)->m_width;
		}

		tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 2);
		if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
			m_vthumb.SetRectEmpty();
		else
		{
			if (m_vscroll)
				tRc.right = m_vscr.left;
			if (m_form->attr2 & GO_PAGE)
				tRc.right = m_pscr.left;
		}
		tRc.bottom = m_hscr.top;
		invalidateRect(&tRc, false);

		tRc = m_hscr;
		m_hthumb.SetRectEmpty();
		tRc.left  += (int)(szSCROLL * m_hR);
		tRc.right -= (int)(szSCROLL * m_hR);
		invalidateRect(&tRc, false);
		break;

	default:
		m_result = result;
		return false;
	}

	m_result = result;
	return true;
}

bool CfmGrid::OnMouseWheel(int delta, int& result)
{
	if (m_writingAll || m_drawing)
		return false;

	int	stline = m_stRow;
	CRect	tRc;

	m_result = result = RC_NOP;
	if (delta == 0 || !m_vscroll || !m_enableScroll)
		return false;

	delta  *= WHEEL_AMOUNT;
	delta  *= -1;
	stline += delta;
	if (delta > 0)
	{
		if (stline > m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN)))
		{
			if (!m_writingAll && m_save == svExchange && (!m_useInfo || (m_useInfo && (m_stat & statNEXT))))
				result = RC_SCRDN;	// exLast
			stline = m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN));
		}
	}
	else
	{
		if (stline < 0)
		{
			if (!m_writingAll && m_save == svExchange && (!m_useInfo || (m_useInfo && (m_stat & statPREV))))
				result = RC_SCRUP;	// exFirst
			stline = 0;
		}
	}

	if (stline == m_stRow)
	{
		m_result = result;
		return false;
	}

// updateXX_20200428, Wheel 처리 변경
	if (stline - m_stRow < WHEEL_AMOUNT)
		result = RC_NOP;

	m_stRow = stline;
	tRc = m_vscr;
	if (m_form->attr2 & GO_PAGE)
		tRc.top = m_pscr.bottom;
	else
		tRc.top += (int)(szSCROLL * m_vR);
	tRc.bottom -= (int)(szSCROLL * m_vR);
	m_vthumb.SetRectEmpty();
	invalidateRect(&tRc, false);

	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;			// head extra space
	tRc.right = m_vscr.left;
	tRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);

	if (m_state == stEdit)
	{
		m_axform->m_view->HideCaret();
		m_edcaret    = true;
		m_edfocusing = false;
	}

	if (m_hscroll)
		tRc.bottom = min(tRc.bottom, m_hscr.top);
	invalidateRect(&tRc, false);

	m_result = result;
	return true;
}

int CfmGrid::OnDrag(CPoint pt, int& result)
{
	int	idx;
	CRect	tRc;

	m_result = result = RC_NOP;
	if (m_click == ckNone)
		return drag(pt, result);

	if (m_form->isEdit == FI_CELL && m_click == ckItem && m_curpos.m_col > 0 && m_curpos.m_row > 0
		&& !(m_cellR[m_columns.GetAt(m_curpos.m_col-1)->m_ref].attr & FA_DRAG))// || m_click == ckHead)
	{
		int	row = 0, col = 0;
		CRect	hRc(0,0,0,0), vRc(0,0,0,0);

		if (findCell(pt, row, col, hRc, vRc))
		{
			if (m_lastpos.m_row != row || m_lastpos.m_col != col)
			{
				m_lastpos.m_row = row;
				m_lastpos.m_col = col;
				if (m_lastpos.m_row != m_curpos.m_row || m_lastpos.m_col != m_curpos.m_col)
					m_move = mvSel;
				if (!hRc.IsRectEmpty())
					invalidateRect(&hRc, false);
				if (!vRc.IsRectEmpty())
					invalidateRect(&vRc, false);
			}
		}
		return -1;
	}

	int	size;
	CRect	iRc1,  iRc2;

	switch (m_click)
	{
	case ckVthumb:
		idx = m_stRow;
		size = m_vthumb.Height();
		iRc1 = m_vthumb;
		iRc2 = m_vthumb;
		tRc  = m_vscr;
		if (m_form->attr2 & GO_PAGE)
			tRc.top = m_pscr.bottom;
		else
			tRc.top += (int)(szSCROLL * m_vR);
		tRc.bottom -= (int)(szSCROLL * m_vR);
		if (!tRc.PtInRect(pt) || m_vthumb.IsRectEmpty())
			return -1;

		m_vthumb.top   += (pt.y - m_point.y);
		m_vthumb.bottom = m_vthumb.top + size;
		if (m_vthumb.top <= tRc.top)
		{
			if (!m_writingAll && m_save == svExchange
				&& (!m_useInfo || (m_useInfo && (m_stat & statPREV))))
				result = RC_SCRUP;	// exFirst
			m_stRow = 0;
			m_vthumb.top    = tRc.top;
			m_vthumb.bottom = m_vthumb.top + size;
		}
		else if (m_vthumb.bottom >= tRc.bottom)
		{
			if (!m_writingAll && m_save == svExchange
				&& (!m_useInfo || (m_useInfo && (m_stat & statNEXT))))
				result = RC_SCRDN;
			m_stRow = m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN));
			m_vthumb.bottom = tRc.bottom;
			m_vthumb.top    = m_vthumb.bottom - size;
		}
		else
		{
			m_stRow = max(0, (int)((m_vthumb.top - tRc.top) / m_vgap));
			if (!m_writingAll && m_save == svExchange)
			{
				if (m_stRow < 0 && (!m_useInfo || (m_useInfo && (m_stat & statPREV))))
					result  = RC_SCRUP;
				else if ((m_stRow > m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN)))
					&& (!m_useInfo || (m_useInfo && (m_stat & statNEXT))))
					result  = RC_SCRDN;
			}
			m_stRow = max(0, min(m_stRow, m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN))));
		}
		m_point = pt;

		iRc1.bottom = m_vthumb.top;
		iRc1.NormalizeRect();
		iRc2.top    = iRc2.bottom;
		iRc2.bottom = m_vthumb.bottom;
		iRc2.NormalizeRect();
		
		if (idx != m_stRow)
		{
			tRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				tRc.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					tRc.top += (4 * m_multiN);	// head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += m_multiN;			// head extra space
			tRc.right = m_vscr.left;
			tRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);
			if (m_hscroll)
				tRc.bottom = min(tRc.bottom, m_hscr.top);

			invalidateRect(&tRc, false);
		}

		if (iRc1.top != iRc1.bottom)
		{
			iRc1.bottom += 2;
			invalidateRect(&iRc1, false);
		}
		if (iRc2.top != iRc2.bottom)
		{
			iRc2.top -= 2;
			invalidateRect(&iRc2, false);
		}
		break;

	case ckHthumb:
		break;
	}
	
	m_result = result;
	return -1;
}

bool CfmGrid::OnDrop(CPoint pt, CString data, CString& key)
{
	int	col = findColumn(key);
	int	row = findRow(pt.y);

	if (col < 0 || row < 0)
		return false;

	WriteData(data, true, col, row);
	return true;
}

int CfmGrid::OnKey(int key, int& result)
{
	if (!(m_form->properties & PR_ENABLE))
		return 0;

	Ccolumn* column;

	if (m_curpos.m_col >= 1 && m_curpos.m_row >= 1)
	{
		column = m_columns.GetAt(m_curpos.m_col-1);
		if (m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT)
		{
			switch (m_cellR[column->m_ref].kind)
			{
			case CK_COMBO:
			//	if (m_cellR[column->m_ref].vals[2] != NOVALUE && key == (int)m_cellR[column->m_ref].vals[2])
				if (key == VK_DOWN || (m_cellR[column->m_ref].vals[2] != NOVALUE && key == (int)m_cellR[column->m_ref].vals[2]))
				{
					result = RC_NOP;
					if (m_cell.IsRectEmpty())
						getRect(m_curpos.m_row-1, m_curpos.m_col-1, m_cell);
					if (makeListBox(m_curpos.m_row-1, m_curpos.m_col-1, m_cell))
					{
						m_cbspread = true;
						result = RC_CBSPREAD;
					}
					return 1;
				}

			default:
				break;
			}
		}
	}

	switch (key)
	{
	case VK_ESCAPE:
	case VK_DELETE:
	case VK_BACK:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_RETURN:
		if (m_curpos.m_col < 1 || m_curpos.m_row < 1)
			break;
		column = m_columns.GetAt(m_curpos.m_col-1);
		if ((m_form->attr2 & GO_TABLE) && !column->m_enables.GetAt(m_curpos.m_row-1))
			break;
	default:
		CfmBase::OnKey(key, result);
		break;
	}

	return 0;
}

void CfmGrid::InsertRow(int index, CString str)
{
	if (m_totRows <= 0 || index >= m_totRows)
		return;

	int	stRow     = m_stRow;
	int	validRows = m_validRows;
	int	row       = insertRow(index, str);
	bool	scroll    = false;
	bool	bScroll   = m_enableScroll;

	if (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND)
		m_validRows = m_totRows;

	if (row >= 0)		// first & N
	{
		if (m_stRow != 0 && row*m_multiN < m_stRow)
		{
			m_stRow++;
			if (m_stRow >= m_validRows*m_multiN - (m_showRows - m_headN*m_multiN))
				m_stRow--;
		}
	}
	else			// last
	{
		if (m_stRow < m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN)))
		{
			if (validRows >= m_validRows)
				m_stRow--;
		}
	}

	m_stRow  = max(0, min(m_stRow, m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN))));
	if (stRow != m_stRow)
		scroll = true;

	if (m_validRows*m_multiN > (m_showRows-m_headN*m_multiN))
		m_enableScroll = true;

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else
	{
		tRc.DeflateRect(1, 1);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;		// head extra space
	}
	tRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = m_pscr.left;
	if (m_hscroll)
		tRc.bottom = m_hscr.top;
	if (!m_writingAll)
		invalidateRect(&tRc, false);

	if (scroll || m_enableScroll)
	{
		tRc = m_vscr;
		if (bScroll && m_enableScroll)
		{
			if (m_form->attr2 & GO_PAGE)
				tRc.top = m_pscr.bottom;
			else
				tRc.top += (int)(szSCROLL * m_vR);
			tRc.bottom -= (int)(szSCROLL * m_vR);
		}
		m_vthumb.SetRectEmpty();
		if (!m_writingAll)
			invalidateRect(&tRc, false);
	}
}

void CfmGrid::InsertRows(int index, CString str)
{
	if (m_totRows <= 0 || index >= m_totRows)
		return;

	int	pos, ii;
	CString	text = _T("");

//	str.TrimRight();
	for (ii = 0; !str.IsEmpty(); ii++)
	{
		if (index < 0)
		{
			pos = str.ReverseFind('\n');
			if (pos != -1)
			{
				text = str.Mid(pos+1);
				str  = str.Left(pos);
			}
			else
			{
				text = str;
				str.Empty();
			}
		}
		else
		{
			pos = str.Find('\n');
			if (pos != -1)
			{
				text = str.Left(pos++);
				str  = str.Mid(pos);
			}
			else
			{
				text = str;
				str.Empty();
			}
		}
//2021/02/17 [KJS] insert 중지 -1 이면..		
		if (insertRow(index < 0 ? index : ii, text, true) == -1)
			break;
	}

	if (m_form->attr2 & GO_ROW)
	{
		Ccolumn* col;

		for (ii = 0; ii < m_totCols; ii++)
		{
			col = getColumn(ii);
			col->m_pRGBs.RemoveAll();
			for (int jj = 0; jj < m_totRows; jj++)
			{
				if (jj % 2)
					col->m_pRGBs.Add(m_sRGB);
				else
					col->m_pRGBs.Add(m_fRGB);

				if ((m_cellR[ii].attr & FA_CONTRAST) && (m_cellR[ii].attr2 & SA_CONTRAST) == SA_PAINT)
				{
					text = m_data.GetAt(jj*m_totCols+ii);
					makeSignData(text, ii, jj);
				}
			}
		}
	}

	int	stRow     = m_stRow;
	bool	scroll    = false;
	bool	bScroll   = m_enableScroll;

	SetDomino(false);
	if (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND)
		m_validRows = m_totRows;

	m_stRow = index*m_multiN;
	m_stRow = max(0, min(m_stRow, m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN))));
	if (stRow != m_stRow)
		scroll = true;

	if (m_validRows*m_multiN > (m_showRows-m_headN*m_multiN))
		m_enableScroll = true;

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else
	{
		tRc.DeflateRect(1, 1);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;		// head extra space
	}
	tRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = m_pscr.left;
	if (m_hscroll)
		tRc.bottom = m_hscr.top;
	if (!m_writingAll)
		invalidateRect(&tRc, false);

	if (scroll || m_enableScroll)
	{
		tRc = m_vscr;
		if (bScroll && m_enableScroll)
		{
			if (m_form->attr2 & GO_PAGE)
				tRc.top = m_pscr.bottom;
			else
				tRc.top += (int)(szSCROLL * m_vR);
			tRc.bottom -= (int)(szSCROLL * m_vR);
		}
		m_vthumb.SetRectEmpty();
		if (!m_writingAll)
			invalidateRect(&tRc, false);
	}
}

void CfmGrid::RemoveRow(int index)
{
	if (index >= m_totRows)
		return;

	Ccolumn*	column;
	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);
		if (index >= 0)		// first & N
		{
			if (m_form->attr2 & GO_TABLE)
			{
				column->m_attrs.RemoveAt(index);
				column->m_enables.RemoveAt(index);
			}
			column->m_valids.RemoveAt(index);
			column->m_itRGBs.RemoveAt(index);
			column->m_tRGBs.RemoveAt(index);
			column->m_pRGBs.RemoveAt(index);
			if (m_cellR[column->m_ref].attr & FA_FLOAT)
				column->m_floats.RemoveAt(index);

			m_data.RemoveAt(index * m_totCols);
			m_edit.RemoveAt(index * m_totCols);
		}
		else			// last
		{
			if (m_validRows > 0)
			{
				if (m_form->attr2 & GO_TABLE)
				{
					column->m_attrs.RemoveAt(m_validRows-1);
					column->m_enables.RemoveAt(m_validRows-1);
				}
				column->m_valids.RemoveAt(m_validRows-1);
				column->m_itRGBs.RemoveAt(m_validRows-1);
				column->m_tRGBs.RemoveAt(m_validRows-1);
				column->m_pRGBs.RemoveAt(m_validRows-1);
				if (m_cellR[column->m_ref].attr & FA_FLOAT)
					column->m_floats.RemoveAt(m_validRows-1);

				m_data.RemoveAt((m_validRows-1)*m_totCols);
				m_edit.RemoveAt((m_validRows-1)*m_totCols);
			}
		}
	}

	if (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND)
		m_validRows = m_totRows;
	else if (m_validRows > 0)
		m_validRows--;

	CString	tmps = _T("");
	int	itRGB{}, offs = m_data.GetSize();
	COLORREF	pRGB;
	COLORREF	tRGB;

	if (offs < m_totRows * m_totCols)
	{
		for (int ii = 0; ii < m_totRows * m_totCols - offs; ii++)
		{
			column = getColumn(ii);
			switch (m_cellR[ii].kind)
			{
			case CK_COMBO:
				tmps = getString(ii, m_totRows-1, 0, false);
				break;
			default:
				switch (m_cellR[ii].iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					tmps = _T("");
					break;
				default:
					tmps = CString(' ', m_cellR[ii].size);
					break;
				}
			}

			m_data.Add(tmps);
			m_edit.Add(editData(tmps, ii, m_totRows-1));

			if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
				column->m_valids.Add(1);
			else
				column->m_valids.Add(0);
			if (m_form->attr2 & GO_TABLE)
			{
				column->m_attrs.Add(m_cellR[ii].attr);
				column->m_enables.Add(1);
			}
// updateXXX_2022XX
			if (m_totRows > 1)
			{
				itRGB = column->m_itRGBs.GetAt(m_totRows - 2);		
				pRGB = column->m_pRGBs.GetAt(m_totRows - 2);
				tRGB = column->m_tRGBs.GetAt(m_totRows - 2);
				
				column->m_itRGBs.Add(itRGB);
				column->m_pRGBs.Add(pRGB);
				column->m_tRGBs.Add(tRGB);
			}
			else
			{
				pRGB = column->m_pRGBs.GetAt(0);
				tRGB = column->m_tRGBs.GetAt(0);

				column->m_itRGBs.Add(itRGB);
				column->m_pRGBs.Add(pRGB);
				column->m_tRGBs.Add(tRGB);
			}
			if (m_cellR[ii].attr & FA_FLOAT)
				column->m_floats.Add(0xff);
		}
	}

	if (m_form->attr2 & GO_ROW)
	{
		for (int ii = 0; ii < m_totCols; ii++)
		{
			column = m_columns.GetAt(ii);
			column->m_pRGBs.RemoveAll();
			for (int jj = 0; jj < m_totRows; jj++)
			{
				if (jj % 2)
					column->m_pRGBs.Add(m_sRGB);
				else
					column->m_pRGBs.Add(m_fRGB);

				if (m_cellR[column->m_ref].attr & FA_CONTRAST && (m_cellR[column->m_ref].attr2 & SA_CONTRAST) == SA_PAINT)
				{
					tmps = m_data.GetAt(jj*m_totCols+column->m_ref);
					makeSignData(tmps, ii, jj);
				}
			}
		}
	}

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;	// head extra space
	tRc.top += (int)((m_cy+m_slit) * m_multiN * m_headN);
	m_vthumb.SetRectEmpty();
	invalidateRect(&tRc, false);
}

void CfmGrid::WriteAll(CString str, bool redraw)
{
	CString	tmps;
	int	row, count, offs;
	int	gap = 0, endrow = m_validRows, stRow, validRows;
	Ccolumn* column;
	CRect	tRc;

	m_writingAll = true;
	if (!m_setfocus)
		m_edstate = esNone;
	m_edcaret    = true;
	m_edfocusing = false;
	m_changed    = false;
	m_click = ckNone;
	if (!(m_stat & statSORT))
	{
		if (m_dir > 0 && m_sortc > 0)
		{
			CRect	cRc(0,0,0,0);

			tRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				tRc.DeflateRect(2, 2);
			tRc.bottom = tRc.top + (int)(m_cy * m_multiN * m_headN);
			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					tRc.bottom += (4 * m_multiN);		// head extra space
				else
					tRc.bottom += m_multiN;			// head extra space
			}
			getRect(0, m_sortc, cRc, true);
			tRc.left  = cRc.left-1;
			tRc.right = cRc.right+1;
			invalidateRect(&tRc, false);
		}
		m_dir = 0;
	}

	row   = m_validRows-1;
	stRow = m_stRow;
	count = offs = 0;
	validRows = m_validRows;
	str.Replace('\0', ' ');
	if (m_index >= 0)
		gap = max(0, m_index - m_stRow);
	if (m_save == svExchange)
	{
		int	index = 0;
		for (int ii = 0; ii < m_totRows; ii++)
		{
			for (int jj = 0; jj < m_totCols; jj++)
			{
				column = getColumn(jj);
				switch (m_cellR[jj].iok)
				{
				case EIO_INPUT:
				case EIO_NOP:
					index++;
					continue;
				default:
					if (m_cellR[jj].attr & FA_SKIP)
					{
						index++;
						continue;
					}
					break;
				}
				tmps = CString(' ', m_cellR[jj].size);
				m_data.SetAt(index, tmps);
				m_edit.SetAt(index++, editData(tmps, jj, ii));
			}
		}

		m_index = 0;
		m_stRow = 0;
	}
	else if (m_save == svAppend)
		endrow = m_validRows*m_multiN-1;

	CString	tmpx;
	int	ii, here;
	for (ii = 0; ; ii++)
	{
		if (str.IsEmpty() || ii >= m_totRows)
			break;

		count++;
		offs = str.Find('\n');
		if (offs != -1)
		{
			tmps = str.Left(offs);
			if (str.GetLength() > offs+1)
				str = str.Mid(offs+1);
			else
				str = _T("");
		}
		else
		{
			tmps = str;
			str  = _T("");
		}

		switch (m_save)
		{
		case svAppend:
			insertRow(-1, _T(""));
			row = m_validRows-1;
			break;
		case svInsert:
			insertRow(ii, _T(""));
			row = ii;
			break;
		case svExchange:
		default:
			row = ii;
			m_validRows = ii+1;
			break;
		}

		for (int jj = 0; jj < m_totCols; jj++)
		{
			column = getColumn(jj);
			if (m_cellR[jj].iok == EIO_INPUT || m_cellR[jj].iok == EIO_NOP)
				continue;

			if (m_cellR[jj].attr & FA_SKIP)
			{
				column->m_valids.SetAt(row, 1);
				continue;
			}

			if ((m_axform->m_mapH->options & OP_OOP) && jj >= m_form->type && !column->m_visible)
				continue;

			here = tmps.Find('\t');
			if (here != -1)
			{
				tmpx = tmps.Left(here);
				if (tmps.GetLength() > here+1)
					tmps = tmps.Mid(here+1);
				else
					tmps = _T("");
			}
			else
			{
				here = tmps.Find('\r');
				if (here != -1)
					tmpx = tmps.Left(here);
				else
					tmpx = tmps;
				tmps = _T("");
			}
			WriteData(tmpx, false, jj, row);
		}
	}

	SetDomino(false);
	if (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND)
		m_validRows = m_totRows;

	bool	scroll  = false;
	bool	bScroll = m_enableScroll;
	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;			// head extra space
	tRc.top += (int)((m_cy+m_slit) * m_multiN * m_headN);
	if (m_vscroll && !m_vscr.IsRectEmpty())
		tRc.right = m_vscr.left;
	if ((m_form->attr2 & GO_PAGE) && !m_pscr.IsRectEmpty())
		tRc.right = m_pscr.left;

	if (ii == 0)
	{
		if (m_save == svExchange && redraw)
		{
			if (m_hscroll && !m_hscr.IsRectEmpty())
				tRc.bottom = m_hscr.top;
			invalidateRect(&tRc, false);

			m_enableScroll = false;
			if (m_validRows*m_multiN > (m_showRows-m_headN*m_multiN))
				m_enableScroll = true;
			if (bScroll != m_enableScroll)
			{
				m_vthumb.SetRectEmpty();
				invalidateRect(&m_vscr, false);
			}
		}
		m_save = svExchange;
		m_writingAll = false;
		m_setfocus   = false;
		return;
	}

	switch (m_save)
	{
	case svExchange:
		m_stRow = 0;
		break;
	case svInsert:
		if (m_form->attr2 & GO_THUMB)
		{
			if (m_validRows*m_multiN >= stRow+(m_showRows-m_headN*m_multiN))
				here = max(0, row*m_multiN - (m_showRows-m_headN*m_multiN));
		}
		else
		{
			here = (row+1)*m_multiN-1;
			here = max(0, min(here, m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN))));
		}
		m_stRow = here;
		row = 0;
		break;
	case svAppend:
		if (m_form->attr2 & GO_THUMB)
		{
			here = min(endrow, endrow+ii*m_multiN - (m_showRows-m_headN*m_multiN));
			here = max(0, min(here, m_validRows*m_multiN - (m_showRows-m_headN*m_multiN)));
		}
		else
		{
			here = stRow;
		//	here = stRow + 1;
		//	if (validRows >= m_validRows)
		//		here -= count;
		}
		m_stRow = here;
		break;
	}

	if (m_save == svAppend && (m_validRows * m_multiN < stRow + (m_showRows-m_headN*m_multiN)))
	{
		tRc.top   += (int)((m_cy + m_slit) * (endrow+1));
		tRc.bottom = tRc.top + (int)((m_validRows * m_multiN - endrow)*m_cy)+4;
		m_stRow = stRow;
	}
	if (m_stRow != stRow)
		scroll = true;
	if (m_index >= 0)
	{
		m_index = m_stRow + gap;
		m_curpos.m_row = (m_index / m_multiN);
		m_curpos.m_row++;
		m_lastpos = m_curpos;
	}
	m_enableScroll = false;
	if (m_validRows*m_multiN > m_showRows-m_headN*m_multiN)
	{
		scroll = true;
		m_enableScroll = true;
	}

	if (redraw)
	{
		DoListBox(false);
		if (m_form->attr2 & GO_AUTO)
		{
			CSize	size;
			CDC*	dc = getDC();
			bool	hscroll = m_hscroll;

			gap = (int)(szSCROLL*m_hR)+4;
			for (ii = 0; ii < m_totCols; ii++)
			{
				column = m_columns.GetAt(ii);
				setFont(dc, (m_cellR[column->m_ref].properties & PR_BOLD) ? 1 : 0);
				if ((m_form->attr2 & GO_MERGE) && column->m_heads.GetSize() > 0)
				{
					offs = 0;
					for (int jj = 0; jj < column->m_heads.GetSize(); jj++)
					{
						size = dc->GetOutputTextExtent(column->m_heads.GetAt(jj));
						offs = max(offs, size.cx);
					}
				}
				else
				{
					size = dc->GetOutputTextExtent(column->m_head);
					offs = size.cx;
				}

				for (int jj = 0; jj < m_validRows; jj++)
				{
					size = dc->GetOutputTextExtent(m_edit.GetAt(jj*m_totCols+ii));
					if (m_cellR[column->m_ref].kind == CK_COMBO)
						size.cx += gap;
					offs = max(offs, size.cx);
				}
				column->m_width  = (float)offs + 6;
				column->m_widthx = (WORD)(column->m_width / m_hR);
			}
			releaseDC(dc);

			m_hscroll = isHScrollEnabled();
			if (!m_hscroll)
			{
				m_stCol = m_form->type;
				for (ii = m_form->type; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_visible)
					{
						m_stCol = ii;
						break;
					}
				}
				m_scrolledWidth = 0;
			}

			if (m_hscroll != hscroll)
			{
				if (!m_fixRows)
				{
					tRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
						tRc.DeflateRect(2, 2);
					m_showRows = (m_totRows + m_headN) * m_multiN;
					if (m_vscroll)
					{
						float	showRows, tHeight = 0;

						if (m_hscroll)
							tRc.bottom -= szSCROLL;
						if (!(m_form->attr2 & GO_XHEAD))
						{
							if (!(m_form->attr2 & GO_X3D))
								tRc.top += (4 * m_multiN);
							else
								tRc.top += m_multiN;
						}

						if ((float)tRc.Height() > ((m_headN * m_multiN) * m_cellH))
						{
							showRows = (float)(m_headN * m_multiN);
							if (m_form->attr2 & GO_MERGE)
								tHeight = (float)tRc.Height() - (m_headN*m_multiN)*(m_cy+m_slit);	// m_cellH -> m_cy
							else
								tHeight = (float)tRc.Height() - (m_headN*m_multiN)*m_cy;		// m_cellH -> m_cy
							showRows += (tHeight / (float)(m_cy + m_slit));					// m_cellH -> m_cy
						}
						else
							showRows = (float)tRc.Height() / m_cy;						// m_cellH -> m_cy

						m_showRows = (int)showRows;
						if ( (showRows - (double)m_showRows) > 0.5)
							m_showRows++;
					}
				}
			}

			tRc = m_pRc;
			m_hthumb.SetRectEmpty();
			m_vthumb.SetRectEmpty();
			if (!(m_form->attr2 & GO_X3D))
				tRc.DeflateRect(2, 2);
			invalidateRect(&tRc, false);
		}
		else if (scroll)
		{
			m_vthumb.SetRectEmpty();
			invalidateRect(&m_vscr, false);

			if (m_hscroll && !m_hscr.IsRectEmpty())
				tRc.bottom = m_hscr.top;
			invalidateRect(&tRc, false);
		}
		else
		{
			if (scroll != bScroll)
			{
				if (m_hscroll && !m_hscr.IsRectEmpty())
					tRc.bottom = m_hscr.top;

				m_vthumb.SetRectEmpty();
				invalidateRect(&m_vscr, false);
			}
			invalidateRect(&tRc, false);
		}
	}
	m_save = svExchange;
	m_writingAll = false;
	m_setfocus   = false;
}

void CfmGrid::WriteData(CString data, bool redraw, int col, int row)
{
	if (col >= m_totCols)
		return;

	if (row == -2)	// head
	{
		if (col == -1)
			col = m_cellpos.m_col-1;
		if (col >= 0)
		{
			Ccolumn* column = getColumn(col);
			column->m_head = data;
			if (m_form->attr2 & GO_MERGE)
			{
				int	offs;

				column->m_heads.RemoveAll();
				for ( ; !data.IsEmpty(); )
				{
					offs = data.Find('|');
					if (offs != -1)
					{
						column->m_heads.Add(data.Left(offs));
						data = data.Mid(offs+1);
					}
					else
					{
						column->m_heads.Add(data);
						data.Empty();
					}
				}
			}

			if (col >= m_form->type && col < m_stCol)
				return;

			CRect	tRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				tRc.DeflateRect(2, 2);
			tRc.bottom = tRc.top + (int)(m_cy * m_multiN * m_headN);
			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					tRc.bottom += (4 * m_multiN);			// head extra space
				else
					tRc.bottom += m_multiN;				// head extra space
			}
			if (redraw && (m_form->properties & PR_VISIBLE) && m_headN != 0)
				invalidateRect(&tRc, false);
		}
		return;
	}

	if (col == -1 && row == -1)
	{
		if (m_cellpos.m_row < 1 || m_cellpos.m_row > m_totRows
			|| m_cellpos.m_col < 1 || m_cellpos.m_col > m_totCols)
			return;

		row = m_cellpos.m_row-1;
		col = m_columns.GetAt(m_cellpos.m_col-1)->m_ref;
	}

	CString	tmps = _T("");
	int	ii,  jj;

	Ccolumn* column = getColumn(col);
	if (m_cellR[col].kind == CK_BUTTON && (m_cellR[col].properties & PR_FLEXCB))
	{
		bool	image = false;

		if (m_cellR[col].properties & PR_IMAGE)
			image = true;
		else if (m_cellR[col].properties & PR_IMAGETEXT)
		{
			tmps = data;
			tmps.MakeLower();
			if (tmps.Find(".bmp") != -1 || tmps.Find(".gif") != -1)
				image = true;
		}

		if (image)
		{
			CRect	tRc;

			DoListBox(false);
			tmps.Format(_T("%s\\%s\\%s"), m_axform->m_root.GetString(), IMAGEDIR, data);
			Cimage* image = new Cimage(m_axform->m_draw, tmps);
			if (image)
			{
				column->m_images.SetAt(row, image);
				if (getRect(row, findColumn(col), tRc))
				{
					tRc.InflateRect(1, 4);
					invalidateRect(&tRc, false);
				}
			}
		}
		return;
	}

	if (m_cellR[col].isEdit == IE_CORGB)
		doColumnEditCORGB(row, col, data);

	DWORD attr = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[col].attr;
	makeData(data, m_cellR[col].size, attr, m_cellR[col].attr2, m_cellR[col].type);

	if (row > column->m_valids.GetUpperBound())
		return;

	column->m_valids.SetAt(row, 1);

	tmps = _T("");
	for (jj = 0; jj < data.GetLength(); jj++)
		tmps += data[jj] ? data[jj] : ' ';

	if (m_cellR[col].attr & FA_FLOAT)
	{
		tmps.TrimLeft();
		tmps.TrimRight();

		CString	str, text;
		int	floatx;
		char	sign   = '\0';

		ii = tmps.FindOneOf("+-");
		if (ii >= 0)
		{
			sign = tmps.GetAt(ii);
			tmps = tmps.Mid(ii+1);
		}

		floatx = tmps.Find('.');
		if (floatx >= 0)
		{
			text = tmps.Left(floatx);
			if (m_cellR[col].vals[1] == m_cellR[col].size)
				tmps = text;
			else
			{
				str  = text.Right(m_cellR[col].size - m_cellR[col].vals[1]);
				text = tmps.Mid(floatx+1);
				text = text.Left(m_cellR[col].vals[1]);
				if (str.IsEmpty())
				{
					str += _T("0");
					floatx++;
				}
				text += CString('0', m_cellR[col].vals[1] - text.GetLength());
				str  += text;
				tmps = str;
			}
		}
		else if (tmps.IsEmpty() && (m_cellR[col].attr & FA_ZEROSUP))
			floatx = 0xff;
		else
		{
			if (m_cellR[col].attr & FA_RIGHT)
				tmps = tmps.Right(m_cellR[col].size);
			else
				tmps = tmps.Left(m_cellR[col].size);

			floatx = tmps.GetLength() - m_cellR[col].vals[1];
			if (floatx <= 0)
			{
				tmps   = CString('0', abs(floatx)+1) + tmps;
				floatx = 1;
			}
		}

		if (sign)
		{
			tmps.Insert(0, sign);
			floatx++;
		}
		column->m_floats.SetAt(row, floatx);
	}
	else
	{
		if (m_cellR[col].attr & FA_RIGHT || m_cellR[col].alignC == AL_RIGHT)
			tmps = tmps.Right(m_cellR[col].size);
		else
			tmps = tmps.Left(m_cellR[col].size);
	}

	bool	skip = false;
	m_data.SetAt(row*m_totCols+col, tmps);
	if ((m_cellR[col].properties & PR_SUPRESS) && row > 0)
	{
		if (!tmps.CompareNoCase(m_data.GetAt((row-1)*m_totCols+col)))
			skip = true;
	}

	if (skip)
		m_edit.SetAt(row*m_totCols+col, _T(""));
	else
	{
		makeSignData(tmps, col, row);
		CString	tmpE = editData(tmps, col, row);
		m_edit.SetAt(row*m_totCols+col, tmpE);
	}

	if (redraw && (m_form->properties & PR_VISIBLE))
	{
		CRect	tRc;

		DoListBox(false);
		if (getRect(row, findColumn(col), tRc))
			invalidateRect(&tRc, false);
	} 
}

void CfmGrid::ReadAll(CString& data, bool edit, bool head)
{
	int	rows;
	Ccolumn* col;
	CString	tmps = _T("");

	data = _T("");
	rows = GetValidRows();
	if (rows == 0)
		return;

	if (head)
	{
		for (int ii = 0; ii < m_totCols; ii++)	// read head
		{
			if (edit)
				col = m_columns.GetAt(ii);
			else
				col = getColumn(ii);

			if (!col->m_visible)
				continue;

			ReadData(tmps, edit, -1, ii, -2);
			data += tmps;
			data += '\t';
		}
		data += '\n';
	}

	for (int ii = 0; ii < rows; ii++)
	{
		tmps = _T("");
		for (int jj = 0; jj < m_totCols; jj++)
		{
			if (edit)
				col = m_columns.GetAt(jj);
			else
				col = getColumn(jj);

			if (!col->m_visible)
				continue;

			if (ii*m_totCols+jj <= m_data.GetUpperBound())
			{
				/*
				if (edit && m_cellR[col->m_ref].kind == CK_COMBO)
				{
					int	combo = getIndex(jj, ii, m_data.GetAt(ii*m_totCols+jj));
					if (combo < col->m_display.GetSize())
						tmps += col->m_display.GetAt(combo);
					else
						tmps += m_data.GetAt(ii*m_totCols+jj);
				}
				else	*/
					tmps += (edit ? m_edit.GetAt(ii*m_totCols+col->m_ref) : m_data.GetAt(ii*m_totCols+jj));
			}
			else
				tmps += CString(' ', m_cellR[col->m_ref].size);
			tmps += '\t';
		}
		data += tmps;
		data += '\n';
	}
}

void CfmGrid::ReadData(CString& data, bool edit, int size, int col, int row)
{
	data = _T("");
	if (row != -3 && col >= m_totCols)
		return;

	if (col == -1 && row == -1)
	{
		if (m_cellpos.m_row < 1 || m_cellpos.m_row > m_totRows
			|| m_cellpos.m_col < 1 || m_cellpos.m_col > m_totCols)
			return;

		row = m_cellpos.m_row -1;
		col = m_columns.GetAt(m_cellpos.m_col-1)->m_ref;
	}

	if (edit || row == -2 || row == -3)
	{
		readData(data, edit, col, row);
		return;
	}

	if (m_stCol < 0 || col < 0 || row < 0)
		return;

	if (row * m_totCols + col > m_data.GetUpperBound())
		return;

	int	len;
	char	filler = ' ';
	char	sign   = ' ';
	Ccolumn* column = getColumn(col);

	CString tmps = m_data.GetAt(row * m_totCols + col); tmps.Trim();

	if (size < 0)
		size = m_cellR[col].size;
	switch (m_cellR[col].kind)
	{
	case CK_COMBO:
		if (tmps.IsEmpty())
			tmps = getString(col, row, 0, false);
		break;
	case CK_CHECK:
		if (tmps.IsEmpty())
			tmps = _T('0');
		break;
	case CK_GRAPH:
	default:
		break;
	}

	switch (m_cellR[col].iok)
	{
	case EIO_INPUT:
	case EIO_INOUT:
		if (m_cellR[col].attr & (FA_COMMA|FA_ZERO))
		{
			filler = '0';
			sign   = '0';
		}

		if (tmps.FindOneOf("+-") == 0 && (m_cellR[col].attr & FA_RIGHT))
		{
			sign = tmps[0];
			tmps = tmps.Mid(1);

			if (m_cellR[col].type == IO_NUM || m_cellR[col].type == IO_DIGIT)
				filler = '0';
		}

		len  = tmps.GetLength();
		if (m_cellR[col].attr & FA_RIGHT)
		{
			if (m_cellR[col].attr & FA_FLOAT)
			{
				BYTE	floatx = column->m_floats.GetAt(row);
				if (floatx == 0xff)
				{
					tmps  = tmps.Right(size - m_cellR[col].vals[1]);
					data  = CString(filler, size - m_cellR[col].vals[1] - tmps.GetLength());
					data += tmps;
					data += CString(filler, m_cellR[col].vals[1]);
				}
				else
				{
					CString	tmpx;
					bool	is_sign = (sign == '+' || sign == '-') ? true : false;

					tmpx  = m_data.GetAt(row * m_totCols + col).Left(floatx);
					tmpx  = is_sign ? tmpx.Mid(1) : tmpx;
					data  = CString(filler, size - m_cellR[col].vals[1] - tmpx.GetLength());
					data += tmpx;

					tmpx  = m_data.GetAt(row * m_totCols + col).Mid(floatx);
					tmpx  = tmpx.Left(m_cellR[col].vals[1]);
					data += tmpx;
					data += CString(filler, m_cellR[col].vals[1] - tmpx.GetLength());
				}
			}
			else
			{
				data  = CString(filler, max(size, len) - len);
				data += tmps.Right(min(size, len));
			}
			if (filler != sign)
				data.SetAt(0, sign);
		}
		else
		{
			data  = tmps.Left(min(size, len));
			data += CString(filler, max(size, len) - len);
		}
		break;

	case EIO_OUTPUT:
	default:
		len = tmps.GetLength();
		if (m_cellR[col].attr & FA_RIGHT)
		{
			data  = CString(' ', max(size, len) - len);
			data += tmps.Right(min(size, len));
		}
		else
		{
			data  = tmps.Left(min(size, len));
			data += CString(' ', max(size, len) - len);
		}
		break;
	}
}

void CfmGrid::ClearData(bool selectOnly)
{
	COLORREF rgb;
	int	ii, stCol;

	DoListBox(false);
	if (m_save == svExchange)
	{
		CString	tmps = _T("");
		int	index = 0;

		for (ii = 0; ii < m_totRows; ii++)
		{
			for (int jj = 0; jj < m_totCols; jj++)
			{
				switch (m_cellR[jj].kind)
				{
				case CK_COMBO:
					tmps = getString(jj, ii, 0, false);
					break;
				default:
					switch (m_cellR[jj].iok)
					{
					case EIO_INPUT:
					case EIO_INOUT:
						tmps = _T("");
						break;
					default:
						tmps = CString(' ', m_cellR[jj].size);
						break;
					}
				}

				m_data.SetAt(index, tmps);
				m_edit.SetAt(index++, editData(tmps, jj, ii));
			}
		}
		if (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND)
			m_validRows = m_totRows;
		else
			m_validRows = 0;

		for (ii = 0; ii < m_totCols; ii++)
		{
			Ccolumn* column = getColumn(ii);
			column->m_pRGB = (m_cellR[ii].hpRGB & userCOLOR) ? m_cellR[ii].hpRGB : getRGB(m_cellR[ii].hpRGB);
			column->m_tRGB = (m_cellR[ii].htRGB & userCOLOR) ? m_cellR[ii].htRGB : getRGB(m_cellR[ii].htRGB);
			column->m_pRGBs.RemoveAll();
			column->m_tRGBs.RemoveAll();
			column->m_itRGBs.RemoveAll();
			column->m_valids.RemoveAll();
			column->m_attrs.RemoveAll();
			column->m_enables.RemoveAll();
			for (int jj = 0; jj < m_totRows; jj++)
			{
				if (m_form->attr2 & GO_ROW)
				{
					if (jj % 2)
						column->m_pRGBs.Add(m_sRGB);
					else
						column->m_pRGBs.Add(m_fRGB);
				}
				else
				{
					rgb = (m_cellR[ii].pRGB  & userCOLOR) ? m_cellR[ii].pRGB : getRGB(m_cellR[ii].pRGB);
					column->m_pRGBs.Add(rgb);
				}
				if (m_cellR[ii].tRGB & userCOLOR)
				{
					column->m_tRGBs.Add(m_cellR[ii].tRGB);
					column->m_itRGBs.Add(-1);
				}
				else
				{
					column->m_tRGBs.Add(getRGB(m_cellR[ii].tRGB));
					column->m_itRGBs.Add((short)m_cellR[ii].tRGB);
				}

				if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
					column->m_valids.Add(1);
				else
					column->m_valids.Add(0);
			
				if (m_form->attr2 & GO_TABLE)
				{
					column->m_attrs.Add(m_cellR[ii].attr);
					column->m_enables.Add(1);
				}

				if (m_cellR[ii].attr & FA_FLOAT)
					column->m_floats.SetAt(jj, 0xff);
			}
		}
	}

	m_curpos = Cpos(-1, -1);
	m_index  = m_stRow = 0;
	for (ii = 0; ii < m_totCols; ii++)
	{
		if (m_columns.GetAt(ii)->m_visible)
		{
			m_curpos.m_row = m_index+1;
			m_curpos.m_col = ii+1;
			break;
		}
	}
	m_cellpos = m_curpos;

	stCol = m_stCol;
	m_stCol = m_form->type;
	for (ii = m_form->type; ii < m_totCols; ii++)
	{
		if (m_columns.GetAt(ii)->m_visible)
		{
			m_stCol = ii;
			break;
		}
	}
	m_scrolledWidth = 0;
	CRect	tRc = m_pRc;
	if (stCol != m_stCol)
	{
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 2);
	}
	else
	{
		if (!(m_form->attr2 & GO_X3D))
		{
			tRc.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;			// head extra space
		tRc.top += (int)((m_cy+m_slit) * m_multiN * m_headN);
	}

	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = m_pscr.left;
	if (m_hscroll)
		tRc.bottom = min(tRc.bottom, m_hscr.top);
	m_enableScroll = false;
	if (m_vscroll && (!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
		m_enableScroll = true;

	m_edcaret      = true;
	m_edfocusing   = false;
	m_state        = stNone;
	m_edstate      = esNone;
	invalidateRect(&tRc, false);

	if (m_vscroll)
	{
		m_vthumb.SetRectEmpty();
		invalidateRect(&m_vscr, false);
	}

	if (m_hscroll)
	{
		tRc = m_hscr;
		tRc.left  += (int)(szSCROLL * m_hR);
		tRc.right -= (int)(szSCROLL * m_hR);
		m_hthumb.SetRectEmpty();
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::UpdateData(int key, bool moving, int& result)
{
	if (m_curpos.m_col < 1)
		return;

	int	col     = m_curpos.m_col-1;
	Ccolumn* column = m_columns.GetAt(col);
	DWORD	attr    = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(m_curpos.m_row-1) : m_cellR[column->m_ref].attr;

	if ((m_cbspread || m_state == stCombo) && m_curpos.m_row >= 1)
	{
		int	index, idx;
		CString	data = m_data.GetAt((m_curpos.m_row-1)*m_totCols+column->m_ref);

		result    = RC_NOP;
		m_changed = false;
		index     = getIndex(col, m_curpos.m_row-1, data, true);

		if (data.GetLength() == m_cellR[column->m_ref].size)
			data = _T("");

		if (key < 0x80)
			data += (TCHAR)key;
		else
		{
			data += (TCHAR)(HIBYTE(LOWORD(key)));
			data += (TCHAR)(LOBYTE(LOWORD(key)));
		}

		idx = getIndex(col, m_curpos.m_row-1, data, true);
		if (idx >= 0 && index != idx)
		{
			m_changed = true;
			result    = RC_CHANGED;
		}

		m_data.SetAt((m_curpos.m_row-1)*m_totCols+column->m_ref, data);
		data = editData(data, column->m_ref, m_curpos.m_row-1, true);
		m_edit.SetAt((m_curpos.m_row-1)*m_totCols+column->m_ref, data);
		if (m_changed)
		{
			if (m_cbspread)
			{
				result = RC_CBCLOSE;
				m_changedpos = m_curpos;
				m_axform->DoComboBox(this);
				DoListBox(false);
			}
			CRect	tRc = m_cell;
			tRc.InflateRect(2, 2);
			invalidateRect(&tRc, false);
		}
		return;
	}

	if (m_state == stCheck && key == ' ')
	{
		CString	data;

		result = RC_NOP;
		data   = m_data.GetAt((m_curpos.m_row-1)*m_totCols+column->m_ref);
		if (!data.Compare("1"))
			data = _T("0");
		else
			data = _T("1");

		m_data.SetAt((m_curpos.m_row-1)*m_totCols+column->m_ref, data);
		m_edit.SetAt((m_curpos.m_row-1)*m_totCols+column->m_ref, data);
		if (m_cellR[column->m_ref].properties & PR_CHECKED)
		{
			for (int ii = 0; ii < m_totRows; ii++)
			{
				if (ii == m_curpos.m_row-1)
					continue;
				m_data.SetAt(ii*m_totCols+column->m_ref, _T("0"));
				m_edit.SetAt(ii*m_totCols+column->m_ref, _T("0"));
			}
		}

		result = RC_CHANGED;
		m_changed    = true;
		m_changedpos = m_curpos;

		CRect	tRc = m_cell;
		if (m_cellR[column->m_ref].properties & PR_CHECKED)
		{
			tRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				tRc.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					tRc.top += (4 * m_multiN);	// head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += m_multiN;			// head extra space
			tRc.top += (int)((m_cy+m_slit) * m_multiN * m_headN);
			if (m_vscroll && !m_vscr.IsRectEmpty())
				tRc.right = m_vscr.left;
			if ((m_form->attr2 & GO_PAGE) && !m_pscr.IsRectEmpty())
				tRc.right = m_pscr.left;
			if (m_hscroll && !m_hscr.IsRectEmpty())
				tRc.bottom = m_hscr.top;
			tRc.left  = m_cell.left;
			tRc.right = m_cell.right;
		}
		tRc.InflateRect(1, 2, 1, 4);
		invalidateRect(&tRc, false);
		return;
	}

	if (m_state != stEdit || m_edstate == esNone)
		return;

	result = RC_NOP;
	if (attr & FA_PROTECT)
		return;

	if (!checkKey(key, m_cellR[column->m_ref].type, attr))
	{
	//	result = RC_OVERFLOW;
		return;
	}

	int	floatx = 0xff;
	int	ii, len, index = (m_curpos.m_row-1)*m_totCols+column->m_ref;
	CRect	tRc;
	CString	tmps = _T("");
	CString	data = m_data.GetAt(index);

	tmps = data;
	tmps.TrimRight();
	if (tmps.IsEmpty() && m_cellR[column->m_ref].alignC == AL_LEFT)
		data.Empty();
	tmps.Empty();
	if (m_cellR[column->m_ref].type == IO_NUM && m_cellR[column->m_ref].attr & FA_RIGHT)
		data.TrimRight();
	if (m_edstate == esReverse)
	{
		data.Empty();
		m_edcaretpos = 0;
		if (m_cellR[column->m_ref].attr & FA_FLOAT)
			column->m_floats.SetAt(m_curpos.m_row-1, 0xff);
		m_edstate = esNormal;
	}

	switch (key)
	{
	case '+':
	case '-':
		if (m_cellR[column->m_ref].type == IO_NUM && data.GetLength() > 0)
			return;
		break;
	case '.':
		if (m_cellR[column->m_ref].attr & FA_XFLOAT)
			return;
		break;
	default:
		break;
	}

	if (m_cellR[column->m_ref].attr & (FA_FLOAT|FA_COMMA))
	{
		if (m_cellR[column->m_ref].attr & FA_FLOAT)
			floatx = column->m_floats.GetAt(m_curpos.m_row-1);
		switch (key)
		{
		case '.':
			if (floatx != 0xff)
				return;

			floatx = data.GetLength();
			if (floatx == 0)
			{
				data = '0';
				floatx++;
				m_edcaretpos = 1;
			}
			if (m_cellR[column->m_ref].attr & FA_FLOAT)
				column->m_floats.SetAt(m_curpos.m_row-1, floatx);
			m_data.SetAt(index, data);
			data = editData(data, column->m_ref, m_curpos.m_row-1, true);
			m_edit.SetAt(index, data);

			m_changed    = true;
			m_edfocusing = false;
			tRc = m_cell;
			tRc.InflateRect(2, 2);
			invalidateRect(&tRc, false);
			return;

		case '0':
			if (floatx != 0xff)
				break;

			for (ii = 0; ii < data.GetLength(); ii++)
			{
				if (data[ii] != '0')
					break;
			}
			if (ii >= data.GetLength())
			{
				data = '0';
				m_edcaretpos = 1;

				m_data.SetAt(index, data);
				data = editData(data, column->m_ref, m_curpos.m_row-1, true);
				m_edit.SetAt(index, data);

				m_changed    = true;
				m_edfocusing = false;
				tRc = m_cell;
				tRc.InflateRect(2, 2);
				invalidateRect(&tRc, false);
				return;
			}
			break;

		case '1': case '2': case '3':
		case '4': case '5': case '6':
		case '7': case '8': case '9':
			if ((m_cellR[column->m_ref].attr & FA_FLOAT) && floatx != 0xff)
				break;

			for (ii = 0; ii < data.GetLength(); ii++)
			{
				if (data[ii] != '0' && data[ii] != ' ')
					break;
			}

			if (ii >= data.GetLength())
			{
				data.Empty();
				m_edcaretpos = 0;
			}
			break;
		}
	}

	if ((m_cellR[column->m_ref].attr & FA_FLOAT)
		&& ((floatx == 0xff && m_edcaretpos >= m_cellR[column->m_ref].size - (int)m_cellR[column->m_ref].vals[1])
		|| (floatx != 0xff && data.GetLength() - floatx >= (int)m_cellR[column->m_ref].vals[1]))
		|| (!(m_cellR[column->m_ref].attr & FA_FLOAT) && m_edcaretpos >= m_cellR[column->m_ref].size))
	{
	//	result = RC_OVERFLOW;
		return;
	}

	m_edstate = esNormal;
	m_cellpos = m_curpos;
	if (m_cellR[column->m_ref].attr & FA_RIGHT)
	{
		tmps = data.Left(m_edcaretpos);
		if (m_cellR[column->m_ref].attr & FA_UPPER)
			tmps += (TCHAR)toupper(key);
		else
			tmps += (TCHAR)key;
		len = (key < 0x80)? 1 : 2;
	}
	else
	{
		tmps = data.Left(m_edcaretpos);
		len = (key < 0x80)? 1 : 2;
		if (len == 1)
		{
			if (m_cellR[column->m_ref].attr & FA_UPPER)
				tmps += (TCHAR)toupper(key);
			else
				tmps += (TCHAR)key;
		}
		else
		{
			tmps += (TCHAR)(HIBYTE(LOWORD(key)));
			tmps += (TCHAR)(LOBYTE(LOWORD(key)));
		}

	//	int	pos = (whichHangulEdit(m_edcaretpos, data) == WC_H1) ? m_edcaretpos + 2 : m_edcaretpos + len;
		int	pos = 0;
		switch (whichHangulEdit(m_edcaretpos, data))
		{
		case WC_H1:
			pos = m_edcaretpos + 2;
			break;
		case WC_AN:
			if (len == 2 && m_cellR[column->m_ref].type == IO_MIXED)
			{
				pos = m_edcaretpos + 1;
				break;
			}
		case WC_H2:
			pos = m_edcaretpos + len;
			break;
		}

		if (pos < data.GetLength())
			tmps += data.Mid(pos);
	}

	if (moving)
		m_edcaretpos += len;

	data = tmps;
	m_data.SetAt(index, data);
	data = editData(data, column->m_ref, m_curpos.m_row-1, true);
	m_edit.SetAt(index, data);

	if ((m_form->attr2 & GO_TABLE) && !(m_cellR[column->m_ref].attr & FA_FLOAT))
	{
		if (m_edcaretpos == m_cellR[column->m_ref].size)
			result = RC_CHANGED;
	}

	m_changed    = true;
	m_edcaret    = true;
	m_edfocusing = false;
	m_changedpos = m_curpos;
	tRc = m_cell;
	tRc.InflateRect(2, 2);
	invalidateRect(&tRc, false);
}

void CfmGrid::InsertData(int key, bool moving, int& result)
{
	UpdateData(key, moving, result);
}

void CfmGrid::GetEnum(CString& text, int& value, int func)
{
	if (func >= 0)
	{
		text.Empty();
		switch (func)
		{
		case 0:		// get grid state
			if (!(m_form->attr & FA_COMMA) && (m_state != stEdit || m_edstate == esNone))
				value = 0;
			else
				value = MAKELONG(m_curpos.m_row, m_columns.GetAt(m_curpos.m_col-1)->m_ref+1);
			break;

		case 1:		// get multiline grid text : for Excel
			text = readAll(value ? true : false);
			break;

		case 98:	// grid information header size
			value = sizeof(_gridHo);
			break;
		case 99:	// total info.
			getInformation(text);	break;
		case 100:	// state, pos
			value = 0;
			if (m_stat & statPREV)
				value += 1;
			if (m_stat & statNEXT)
				value += 2;
			break;
		case 101:	// page info.
			text = m_page;		break;
		case 102:	// save
			text = m_inits;		break;
		case 103:	// sort direction
			value = m_dir;		break;
		case 104:	// sort column
			text = (m_sortc >= 0) ? (char *)m_cellR[m_sortc].name : _T("");
			break;
		case 107:	// sortable
			if (m_cellpos.m_col > 0 && m_cellpos.m_col <= m_totCols)
			{
				Ccolumn* col;
				col = m_columns.GetAt(m_cellpos.m_col-1);
				value = (m_cellR[col->m_ref].properties & PR_SORT) ? 1 : 0;
			}
			break;
		case 105:	// action
		case 108:	// realtime data insert
			value = (m_form->attr2 & GO_TOP) ? 1 : 2;
			break;
		case 109:	// column symbol
			if (m_cellpos.m_col > 0 && m_cellpos.m_col <= m_totCols)
				text = (char *)m_cellR[m_columns.GetAt(m_cellpos.m_col-1)->m_ref].name;
			break;
		case 253:	// find row & is portfolio grid
			if (m_portfolio >= 0)
			{
				POINT	pt;

				::GetCursorPos(&pt);
				m_axform->m_view->ScreenToClient(&pt);
				value = findRow(pt.y);
				if (value < 0)
					text.Empty();
				else
				{
					if ((m_cellR[m_portfolio].attr & FA_REF) && m_cellR[m_portfolio].vals[0] != NOVALUE)
						func = m_cellR[m_portfolio].vals[0];
					else	func = m_portfolio;
					ReadData(text, false, -1, func, value);
				}
			}
			break;
		default:
			break;
		}
		return;
	}

	switch (value)
	{
	case -1:
		text.Empty();
		value = 1;
		switch (m_state)
		{
		case stEdit:
			if (m_edstate != esNone)
				value = 0;
			break;
		case stFocus:
		default:
			break;
		}
		break;

	default:
		value = findColumn(text);
		break;
	}
}

void CfmGrid::SetEnum(CString text, int value, int func)
{
	int	ii, offs;
	CString	tmps;
	Ccolumn* column;
	bool	grid_ini = (func == 200) ? true : false;

	if (!grid_ini && func > 0)	// func valid
	{
		switch (func)
		{
		case 55:	// measure combolist
			if (m_list)
				m_list->MeasureItemX((LPMEASUREITEMSTRUCT)value);
			break;
		case 56:	// draw combolist
			if (m_cbspread && m_list)
				m_list->DrawItemX((LPDRAWITEMSTRUCT)value);
			break;
		case 98:	// set border color
			m_bRGB = value;		break;
		case 99:	// total info.
			setInformation(text);	break;
		case 100:	// state
			m_stat = 0;
			switch (value)
			{
			case 1:
				m_stat |= statPREV;	break;
			case 2:
				m_stat |= statNEXT;	break;
			case 3:
				m_stat |= statPREV;
				m_stat |= statNEXT;	break;
			case 0:
			default:
				break;
			}
			break;
		case 101:	// page info.
			m_page = text;		break;
		case 102:	// save
			m_inits = text;		break;
		case 103:	// sort direction
			m_dir = value;		break;
		case 104:	// sort column
			value = findColumn(text);
			if (value >= 0)
				m_sortc = value;
			break;
		case 106:	// sort direction & column
			setSortInfo(text);	break;
		default:
			break;
		}

		return;
	}

	if (value == -1)
	{
		switch (m_state)
		{
		case stEdit:
			if (m_edstate != esNone)
			{
				m_edstate    = esNormal;
				m_edcaret    = true;
				m_edfocusing = false;
				m_edcaretpos = 0;
				WriteData(_T(""));
			}
			break;
		default:
			invalidateRect(&m_pRc, false);
			break;
		}
		return;
	}

	float	width = 0;
	CArray	<Ccolumn*, Ccolumn *> arCOL;

	arCOL.RemoveAll();
	if (text.IsEmpty())
	{
		for (ii = grid_ini ? 0 : m_form->type; ii < m_totCols; ii++)
		{
			column = getColumn(ii);
			if (column)
			{
				column->m_visible = (m_cellR[ii].properties & PR_VISIBLE) ? true : false;
				arCOL.Add(column);
			}
		}
	}
	else
	{
		CArray	<Ccolumn*, Ccolumn *> arCOLx;
		int	multi = 0;

		arCOLx.RemoveAll();
		for (ii = grid_ini ? 0 : m_form->type; ii < m_totCols; ii++)
			arCOLx.Add(getColumn(ii));

		for ( ; !text.IsEmpty(); )
		{
			offs = text.Find('|');
			if (offs == -1)
			{
				tmps = text;
				text.Empty();
			}
			else
			{
				tmps = text.Left(offs++);
				text = text.Mid(offs);
			}

			tmps.Trim();
			for (ii = 0; ii < arCOLx.GetSize(); ii++)
			{
				if (!tmps.CompareNoCase((char *)m_cellR[arCOLx.GetAt(ii)->m_ref].name))
				{
					column = arCOLx.GetAt(ii);
					column->m_visible = true;
					column->m_line = multi;
					if ((m_form->attr2 & GO_MULTI) && (m_cellR[column->m_ref].attr & FA_WRAP))
						multi++;
					arCOL.Add(column);
					arCOLx.RemoveAt(ii);
					break;
				}
			}
		}

		for (ii = 0; ii < arCOLx.GetSize(); ii++)
		{
			column = arCOLx.GetAt(ii);
			column->m_line = multi;
			if ((m_form->attr2 & GO_MULTI) && (m_cellR[column->m_ref].attr & FA_WRAP))
				multi++;
			column->m_visible = false;
			arCOL.Add(arCOLx.GetAt(ii));
		}
		arCOLx.RemoveAll();
	}

	if (grid_ini)
		m_columns.RemoveAll();
	else
		m_columns.SetSize(m_form->type);

	for (ii = 0; ii < arCOL.GetSize(); ii++)
		m_columns.Add(arCOL.GetAt(ii));

	m_stCol = m_form->type;
	for (ii = m_form->type; ii < m_totCols; ii++)
	{
		if (m_columns.GetAt(ii)->m_visible)
		{
			m_stCol = ii;
			break;
		}
	}

	bool	hscroll = m_hscroll;
	m_hscroll = isHScrollEnabled();
	if (!m_hscroll)
		m_scrolledWidth = 0;
	if (m_hscroll != hscroll)
	{
		if (!m_fixRows)
		{
			CRect	sRc = m_pRc;

			if (!(m_form->attr2 & GO_X3D))
				sRc.DeflateRect(2, 2);
			m_showRows = (m_totRows + m_headN) * m_multiN;
			if (m_vscroll)
			{
				float	showRows, tHeight = 0;

				if (m_hscroll)
					sRc.bottom -= szSCROLL;
				if (!(m_form->attr2 & GO_XHEAD))
				{
					if (!(m_form->attr2 & GO_X3D))
						sRc.top += (4 * m_multiN);
					else
						sRc.top += m_multiN;
				}
// updateXXX_2022XX
				if ((float)sRc.Height() > ((m_headN * m_multiN) * m_cellH))
				{
					showRows = (float)(m_headN * m_multiN);
					if (m_form->attr2 & GO_MERGE)
						tHeight = (float)sRc.Height() - (m_headN*m_multiN)*(m_cy+m_slit);	// m_cellH -> m_cy
					else
						tHeight = (float)sRc.Height() - (m_headN*m_multiN)*m_cy;		// m_cellH -> m_cy
					showRows += (tHeight / (float)(m_cy + m_slit));					// m_cellH -> m_cy
				}
				else
					showRows = (float)sRc.Height() / (float)m_cy;					// m_cellH -> m_cy

				m_showRows = (int)showRows;
				if ( (showRows - (double)m_showRows) > 0.5)
					m_showRows++;

				sRc.right -= szSCROLL;

				if ((!(m_form->attr2 & GO_FLEX) || m_form->attr2 & GO_EXPAND) && m_input)
					m_enableScroll = true;
				else if (m_validRows*m_multiN > (m_showRows-m_headN*m_multiN))
					m_enableScroll = true;
				else	m_enableScroll = false;
			}
		}
	}

	m_hthumb.SetRectEmpty();
	if (value)
	{
		CRect	tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
		{
			tRc.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;			// head extra space
		if (m_vscroll)
			tRc.right = m_vscr.left;
		if (m_form->attr2 & GO_PAGE)
			tRc.right = m_pscr.left;
		if (hscroll != m_hscroll && m_vscroll)
			tRc.right = m_vscr.right;
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::SetFocus(bool focus)
{
	bool	whole = false;

	if (m_focus == focus)
		return;

	if (focus && m_mousedown && m_click == ckHead && m_cellpos.m_col > 0)
	{
		Ccolumn* col = m_columns.GetAt(m_cellpos.m_col-1);
		if (m_cellR[col->m_ref].hkind != CK_NORMAL)
		{
			m_focus = focus;
			return;
		}
	}

	if (focus && m_state == stNone && m_validRows > 0 && (m_form->iok == EIO_INPUT || m_form->iok == EIO_INOUT))
	{
		if (m_curpos.m_col <= 0 || m_curpos.m_row <= 0)
		{
			for (int ii = 0; ii < m_validRows; ii++)
			{
				for (int jj = 0; jj < m_totCols; jj++)
				{
					Ccolumn* column = m_columns.GetAt(jj);
					DWORD	attr = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(ii) : m_cellR[column->m_ref].attr;

					if (!column->m_visible || (attr & FA_PROTECT))
						continue;
					if (m_form->attr2 & GO_TABLE && !column->m_enables.GetAt(ii))
						continue;

					if (m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT)
					{
						m_index = ii*m_multiN;
						m_curpos.m_col = jj+1;
						m_curpos.m_row = ii+1;
						changeIME(ii, jj);
						break;
					}
				}
				if (m_curpos.m_col > 0)
					break;
			}
		}

		if (m_curpos.m_col > 0)
		{
			Ccolumn* column = m_columns.GetAt(m_curpos.m_col-1);
			DWORD	attr = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(m_curpos.m_row-1) : m_cellR[column->m_ref].attr;

			if (attr & FA_PROTECT || (m_form->attr2 & GO_TABLE && !column->m_enables.GetAt(m_curpos.m_row-1)))
				m_state = stFocus;
			else
			{
				switch (m_cellR[column->m_ref].kind)
				{
				case CK_CHECK:
					if (m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT)
						m_state = stCheck;
					break;
				case CK_COMBO:
					if (m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT)
						m_state = stCombo;
					break;
				case CK_BUTTON:
					m_state = stButton;	break;
				case CK_NORMAL:
					if (m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT)
						m_state = stEdit;
					break;
				default:
					break;
				}
			}

			if (m_move == mvDrag)
				m_state = stFocus;
		}
	}

	if (m_state == stEdit)
	{
		if (focus)
		{
			if (m_curpos.m_col > 0 && m_curpos.m_row > 0)
			{
				CString	tmps;

				tmps = m_data.GetAt((m_curpos.m_row-1)*m_totCols + m_columns.GetAt(m_curpos.m_col-1)->m_ref);
			//	tmps.Trim();
				m_edcaretpos = tmps.GetLength();
				if (m_edcaretpos != 0)
					m_edstate = esReverse;
				else
					m_edstate = esNormal;
				m_edfocusing = false;
			}
		}
		else
		{
			m_edstate = esNone;
			m_axform->m_view->HideCaret();
		}
		m_edcaret = true;
	}

	CRect	tRc = m_cell;
	if (!tRc.IsRectEmpty() || getRect(m_curpos.m_row-1, m_curpos.m_col-1, tRc))
	{
		tRc.InflateRect(1, 4);
		invalidateRect(&tRc, false);
	}

	if (!focus)
	{
		if (m_move == mvSel || m_select)
			whole = true;
		m_move   = mvNone;
		m_select = false;
		m_state  = stNone;
		m_click  = ckNone;
		m_mousedown = false;
		DoListBox(false);
	}

	int	stRow = m_stRow;
	if (focus && m_state != stNone && m_state != stFocus && (m_form->iok == EIO_INPUT || m_form->iok == EIO_INOUT))
	{
		if (m_index < m_stRow)
			m_stRow = max(0, m_index);
		else if (m_index >= m_stRow + m_showRows - (m_headN * m_multiN))
			m_stRow = max(0, min(m_index, m_validRows*m_multiN - (m_showRows-(m_headN*m_multiN))));
	}

	if (m_focus != focus)
	{
		tRc = m_pRc;
		m_focus = focus;
		if (!(m_form->attr2 & GO_X3D))
		{
			tRc.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;	// head extra space
		tRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);

		if (whole)
		{
			tRc.InflateRect(0, 2);
			invalidateRect(&tRc, false);
		}
		else
		{
			if (stRow == m_stRow)
			{
				if (m_form->isEdit == FI_RREC)
				{
					tRc.top   += (int)((m_cy+m_slit) * (m_index/m_multiN*m_multiN - m_stRow));
					tRc.bottom = tRc.top + (int)(m_cy+m_slit)*m_multiN + 5;
				}
				else
				{
					tRc.top   += (int)((m_cy+m_slit) * (m_index-m_stRow));
					tRc.bottom = tRc.top + (int)m_cy + 5;
				}
			}
			if (m_vscroll)
				tRc.right = m_vscr.left;
			if (m_form->attr2 & GO_PAGE)
				tRc.right = m_pscr.left;
			if (m_hscroll)
				tRc.bottom = min(tRc.bottom, m_hscr.top);
			tRc.InflateRect(0, 3);
			invalidateRect(&tRc, false);
			if (m_vscroll && (stRow != m_stRow))
			{
				tRc = m_vscr;
				if (m_form->attr2 & GO_PAGE)
					tRc.top = m_pscr.bottom;
				else
					tRc.top += (int)(szSCROLL * m_vR);
				tRc.bottom = m_vscr.bottom - (int)(szSCROLL * m_vR);
				m_vthumb.SetRectEmpty();
				invalidateRect(&tRc, false);
			}
		}

	//	if (m_focus)
			m_axform->DoSomething(doSETFOCUS, this);
	}
	else
	{
		if (m_focus && stRow != m_stRow)
		{
			tRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				tRc.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					tRc.top += (4 * m_multiN);	// head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += m_multiN;			// head extra space
			tRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
			if (whole)
			{
				tRc.InflateRect(0, 2);
				invalidateRect(&tRc, false);
			}
			else
			{
				if (m_vscroll)
					tRc.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tRc.right = m_pscr.left;
				if (m_hscroll)
					tRc.bottom = min(tRc.bottom, m_hscr.top);
				tRc.InflateRect(0, 2);
				invalidateRect(&tRc, false);
				if (m_vscroll && (stRow != m_stRow))
				{
					tRc = m_vscr;
					if (m_form->attr2 & GO_PAGE)
						tRc.top = m_pscr.bottom;
					else
						tRc.top += (int)(szSCROLL * m_vR);
					tRc.bottom = m_vscr.bottom - (int)(szSCROLL * m_vR);
					m_vthumb.SetRectEmpty();
					invalidateRect(&tRc, false);
				}
			}
		}
	}
}

bool CfmGrid::IsChanged(bool reset)
{
	bool	changed = m_changed;
	if (changed)
		m_cellpos = m_changedpos;
	if (reset)
		m_changed = false;

	return changed;
}

void CfmGrid::SetCellRow(int row)
{
	if (row <= m_totRows)
		m_cellpos.m_row = row;
}

void CfmGrid::SetCellColumn(int col)
{
	if (col > 0 && col <= m_totCols)
		m_cellpos.m_col = findColumn(col-1)+1;
}

int CfmGrid::GetCellRow()
{
	return m_cellpos.m_row;
}

int CfmGrid::GetCellColumn()
{
	int	col = 0;
	if (m_cellpos.m_col > 0)
		col = m_columns.GetAt(m_cellpos.m_col-1)->m_ref;
	return col+1;
}

int CfmGrid::GetValidRows()
{
	return m_validRows;
}

void CfmGrid::SetAttributes(DWORD attr, bool set, int col, int row)
{
	if (!(m_form->attr2 & GO_TABLE) || attr == 0)
		return;
	if (col < 0 || col >= m_totCols || row < 0 || row >= m_totRows)
		return;

	if (attr & FA_REF)
	{
		attr &= ~FA_REF;

		m_state = stEdit;
		m_cellpos.m_col = findColumn(col)+1;
		m_cellpos.m_row = row+1;
		m_curpos = m_cellpos;
		m_edcaretpos = m_data.GetAt(row*m_totCols+col).GetLength();
		if (m_edcaretpos != 0)
			m_edstate = esReverse;
		else
			m_edstate = esNormal;

		m_edfocusing = false;
		m_edcaret    = true;
		m_setfocus   = true;
	}

	Ccolumn* column = getColumn(col);
	DWORD	attributes = column->m_attrs.GetAt(row);
	if (set)
		attributes |= attr;
	else
		attributes &= ~attr;

	column->m_attrs.SetAt(row, attributes);
}

void CfmGrid::SetVisible(bool visible, int col)
{
	if (col < 0)
	{
		CfmBase::SetVisible(visible);
		return;
	}

	if (col >= m_totCols)
		return;

	Ccolumn* column = getColumn(col);
	if (column->m_visible == visible)
		return;
	column->m_visible = visible;
	if (!(m_form->attr & FA_ENUM))
	{
		if (visible)
			m_cellR[col].properties |= PR_VISIBLE;
		else
			m_cellR[col].properties &= ~PR_VISIBLE;
	}
	if (m_stCol < 0 || (col >= m_form->type && col < m_stCol))
		m_stCol = col;

	invalidateRect(&m_pRc, false);
}

bool CfmGrid::IsVisible(int col)
{
	if (col < 0)
		return CfmBase::IsVisible(col);

	if (col >= m_totCols)
		return false;

	return getColumn(col)->m_visible;
}

void CfmGrid::SetEnable(bool enable)
{
	if (!(m_form->attr2 & GO_TABLE))
		return;

	for (int ii = 0; ii < m_totCols; ii++)
	{
		Ccolumn* column = getColumn(ii);
		for (int jj = 0; jj < column->m_enables.GetSize(); jj++)
			column->m_enables.SetAt(jj, enable ? 1 : 0);
	}

	if (m_form->properties & PR_VISIBLE)
		invalidateRect(&m_pRc, false);
}

bool CfmGrid::IsEnable()
{
	return true;

	/*
	if (!(m_form->attr2 & GO_TABLE) || m_cellpos.m_col <= 0 || m_cellpos.m_col > m_totCols
			|| m_cellpos.m_row <= 0 || m_cellpos.m_row > m_totRows)
		return true;

	Ccolumn* column = m_columns.GetAt(m_cellpos.m_col-1);
	return (column->m_enables.GetAt(m_cellpos.m_row-1) == 1);
	*/
}

void CfmGrid::SetCurSel(int index, bool redraw)
{
	if (index < 0 || index >= m_validRows)
		return;

	int	idx    = m_index;
	int	nIndex = index * m_multiN;
	m_index = nIndex;
	if (redraw && idx != m_index)
	{
		if (m_index < m_stRow)
			m_stRow = m_index;
		else if (m_index > m_stRow+(m_showRows-m_headN*m_multiN))
			m_stRow = max(0, m_index-(m_showRows-m_headN*m_multiN)+1);

		m_vthumb.SetRectEmpty();
		CRect	tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
		{
			tRc.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;			// head extra space
		invalidateRect(tRc, false);
	}
// updateXXX_2022XX
	CRect rc;
	getRect(index, 1, rc);
	CPoint pt = rc.CenterPoint();
	lbutton(true, pt, idx);
}

int CfmGrid::GetCurSel()
{
	return (m_index / m_multiN);
}

void CfmGrid::SetFgColor(int rgb, int col, int row)
{
	if (col == -1 && row == -1)
	{
		if (m_cellpos.m_row < 1 || m_cellpos.m_col < 1)
			return;

		row = m_cellpos.m_row - 1;
		col = m_columns.GetAt(m_cellpos.m_col-1)->m_ref;
	}

	if (col >= m_totCols)
		return;

	Ccolumn* column = getColumn(col);
	if (row > column->m_tRGBs.GetUpperBound())
		return;

	if (!setFgColor(row, col, rgb))
		return;

	if (m_form->properties & PR_VISIBLE)
	{
		CRect	tRc;
		if (getRect(row, findColumn(col), tRc, (row == -1) ? true : false))
			invalidateRect(&tRc, false);
	}
}

void CfmGrid::SetBkColor(int rgb, int col, int row)
{
	if (col == -1 && row == -1)
	{
		if (m_cellpos.m_row < 1 || m_cellpos.m_col < 1)
			return;

		row = m_cellpos.m_row - 1;
		col = m_columns.GetAt(m_cellpos.m_col-1)->m_ref;
	}

	if (col >= m_totCols)
		return;

	Ccolumn* column = getColumn(col);
	if (row > column->m_pRGBs.GetUpperBound())
		return;

	if (row >= 0 && m_cellR[col].attr & FA_CONTRAST && (m_cellR[col].attr2 & SA_CONTRAST) == SA_PAINT)
		return;

	if (!setBkColor(row, col, rgb))
		return;

	if (m_form->properties & PR_VISIBLE)
	{
		CRect	tRc;

		tRc.SetRectEmpty();
		if (row < 0)
		{
			if (m_headN == 0)
				return;

			tRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				tRc.DeflateRect(2, 2);
			tRc.bottom = tRc.top + (int)(m_cy * m_multiN * m_headN);
			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					tRc.bottom += (4 * m_multiN);		// head extra space
				else
					tRc.bottom += m_multiN;			// head extra space
			}
		}
		else getRect(row, findColumn(col), tRc);

		if (!tRc.IsRectEmpty())
			invalidateRect(&tRc, false);
	}
}

int CfmGrid::GetFgColor(int col, int row)
{
	if (col == -1 && row == -1)
	{
		if (m_cellpos.m_row < 1 || m_cellpos.m_col < 1)
			return -1;

		row = m_cellpos.m_row - 1;
		col = m_columns.GetAt(m_cellpos.m_col-1)->m_ref;
	}

	if (col >= m_totCols)
		return -1;

	COLORREF rgb = 0;
	Ccolumn* column = getColumn(col);
	if (row < column->m_tRGBs.GetSize())
	{
		rgb = column->m_tRGBs.GetAt(row);
		return rgb;//getINDEX(rgb);
	}

	return -1;
}

int CfmGrid::GetBkColor(int col, int row)
{
	if (col == -1 && row == -1)
	{
		if (m_cellpos.m_row < 1 || m_cellpos.m_col < 1)
			return -1;

		row = m_cellpos.m_row - 1;
		col = m_columns.GetAt(m_cellpos.m_col-1)->m_ref;
	}

	if (col >= m_totCols)
		return -1;

	COLORREF rgb = 0;
	Ccolumn* column = getColumn(col);
	if (row < column->m_pRGBs.GetSize())
	{
		rgb = column->m_pRGBs.GetAt(row);
		return rgb;//getINDEX(rgb);
	}

	return -1;
}

void CfmGrid::SetRowColor(int fRgb, int sRgb)
{
	CString	tmps;
	Ccolumn* column;

	if (fRgb != 0xffffffff)
		m_fRGB = fRgb;
	if (sRgb != 0xffffffff)
		m_sRGB = sRgb;

	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);
		column->m_pRGBs.RemoveAll();
		for (int jj = 0; jj < m_totRows; jj++)
		{
			if (jj % 2)
				column->m_pRGBs.Add(m_sRGB);
			else
				column->m_pRGBs.Add(m_fRGB);

			if ((m_cellR[column->m_ref].attr & FA_CONTRAST) && (m_cellR[column->m_ref].attr2 & SA_CONTRAST) == SA_PAINT)
			{
				tmps = m_data.GetAt(jj*m_totCols+ii);
				makeSignData(tmps, ii, jj);
			}
		}
	}

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;	// head extra space
	tRc.top += (int)((m_cy+m_slit) * m_multiN * m_headN);
	invalidateRect(&tRc, false);
}

void CfmGrid::SetDomino(bool redraw)
{
	DWORD	rgb;
	Ccolumn* column;

	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);
		if (!(m_cellR[column->m_ref].attr & FA_DOMINO))
			continue;

		if (m_cellR[column->m_ref].keys == NOVALUE)
			continue;

		if (m_cellR[column->m_ref].size <= 0)
			continue;

		if (m_cellR[column->m_ref].keys < 0 || (int)(m_cellR[column->m_ref].keys) >= m_totCols)
			continue;

		for (int row = 0; row < m_validRows; row++)
		{
			rgb = GetFgColor(m_cellR[column->m_ref].keys, row);
			if (redraw)
				SetFgColor(rgb, column->m_ref, row);
			else
				setFgColor(row, column->m_ref, rgb);
			column->m_itRGBs.SetAt(row, m_columns.GetAt(m_cellR[column->m_ref].keys)->m_itRGBs.GetAt(row));
		}
	}
}

int CfmGrid::GetSize()
{
	return m_totRows;
}

void CfmGrid::Sort(int col, bool ascend, bool string)
{
	if (col <= 0)
		return;

	int	stdCol = col-1;
	CStringArray	tArray,  rArray;
	CString		tmps,  tmpx;
	CWordArray	wArray;
	WORD		idx,  idx2;
	Ccolumn*	column;
	CrgbArr*	rgbArr;
	CArray	<CrgbArr*, CrgbArr*> RGBs;
	CDWordArray*	attrArr;
	CArray	<CDWordArray*, CDWordArray*> attrs;
	BYTE		tmpb;
	CByteArray*	byteArr;
	CArray	<CByteArray*, CByteArray*> valids;
	CArray	<CByteArray*, CByteArray*> enables;
	CArray	<CByteArray*, CByteArray*> floats;
	COLORREF	tRGB,  pRGB;
	int		ii, jj, itRGB;
	
	tArray.RemoveAll();
	rArray.RemoveAll();
	wArray.RemoveAll();
	RGBs.RemoveAll();
	attrs.RemoveAll();
	valids.RemoveAll();
	enables.RemoveAll();
	floats.RemoveAll();

	for (ii = 0; ii < m_totCols; ii++)
	{
		rgbArr = new CrgbArr;
		RGBs.Add(rgbArr);

		byteArr = new CByteArray;
		valids.Add(byteArr);

		byteArr = new CByteArray;
		floats.Add(byteArr);

		if (m_form->attr2 & GO_TABLE)
		{
			attrArr = new CDWordArray;
			attrs.Add(attrArr);

			byteArr = new CByteArray;
			enables.Add(byteArr);
		}
	}

	for (ii = 0; ii < m_validRows; ii++)
	{
		tmps = m_data.GetAt(ii * m_totCols + stdCol);
		tArray.Add(tmps);
		wArray.Add(ii);
	}

	for (ii = 0; ii < m_validRows; ii++)
	{
		for (jj = ii; jj < m_validRows; jj++)
		{
			tmps = tArray.GetAt(ii);
			idx  = wArray.GetAt(ii);
			tmpx = tArray.GetAt(jj);
			idx2  = wArray.GetAt(jj);

			CString text = tmpx;
			text.TrimLeft();
			if (string && text.IsEmpty())
				continue;
			
			text = tmps;
			text.TrimLeft();
			if (string && text.IsEmpty())
			{
				tArray.SetAt(ii, tmpx);
				tArray.SetAt(jj, tmps);
				wArray.SetAt(ii, idx2);
				wArray.SetAt(jj, idx);
				continue;
			}

			if (ascend)
			{
				if (compare(string, tmps, tmpx, stdCol) > 0)
				{
					tArray.SetAt(ii, tmpx);
					tArray.SetAt(jj, tmps);
					wArray.SetAt(ii, idx2);
					wArray.SetAt(jj, idx);
				}
			}
			else
			{
				if (compare(string, tmps, tmpx, stdCol) < 0)
				{
					tArray.SetAt(ii, tmpx);
					tArray.SetAt(jj, tmps);
					wArray.SetAt(ii, idx2);
					wArray.SetAt(jj, idx);
				}
			}
		}
	}

	for (ii = 0; ii < m_validRows; ii++)
	{
		idx  = wArray.GetAt(ii);
		for (jj = 0; jj < m_totCols; jj++)
		{
			rgbArr = RGBs.GetAt(jj);
			column = getColumn(jj);
			tmps = m_data.GetAt(idx * m_totCols + jj);
			rArray.Add(tmps);

			if (idx < column->m_pRGBs.GetSize() && idx < column->m_tRGBs.GetSize()
					&& idx < column->m_itRGBs.GetSize() && idx < column->m_valids.GetSize())
			{
				pRGB = column->m_pRGBs.GetAt(idx);
				rgbArr->pRGBs.Add(pRGB);
				tRGB = column->m_tRGBs.GetAt(idx);
				rgbArr->tRGBs.Add(tRGB);
				itRGB = column->m_itRGBs.GetAt(idx);
				rgbArr->itRGBs.Add(itRGB);
				tmpb = column->m_valids.GetAt(idx);
				valids.GetAt(jj)->Add(tmpb);
			}

			if (m_cellR[jj].attr & FA_FLOAT)
			{
				if (idx < column->m_floats.GetSize())
					floats.GetAt(jj)->Add(column->m_floats.GetAt(idx));
			}

			if (m_form->attr2 & GO_TABLE)
			{
				if (idx < column->m_attrs.GetSize())
					attrs.GetAt(jj)->Add(column->m_attrs.GetAt(idx));
				if (idx < column->m_enables.GetSize())
					enables.GetAt(jj)->Add(column->m_enables.GetAt(idx));
			}
		}
	}

	for (ii = 0; ii < m_totCols; ii++)
	{
		column  = m_columns.GetAt(ii);
		rgbArr  = RGBs.GetAt(column->m_ref);
		byteArr = valids.GetAt(column->m_ref);
		for (jj = 0; jj < m_validRows; jj++)
		{
			if (jj > rgbArr->pRGBs.GetUpperBound() || jj > rgbArr->tRGBs.GetUpperBound() || jj > rgbArr->itRGBs.GetUpperBound()
				|| jj > column->m_pRGBs.GetUpperBound() || jj > column->m_tRGBs.GetUpperBound()
				|| jj > byteArr->GetUpperBound() || jj > column->m_valids.GetUpperBound())
				break;

			pRGB = rgbArr->pRGBs.GetAt(jj);
			column->m_pRGBs.SetAt(jj, pRGB);
			tRGB = rgbArr->tRGBs.GetAt(jj);
			column->m_tRGBs.SetAt(jj, tRGB);
			itRGB = rgbArr->itRGBs.GetAt(jj);
			column->m_itRGBs.SetAt(jj, itRGB);
			column->m_valids.SetAt(jj, byteArr->GetAt(jj));
		}

		if (m_cellR[column->m_ref].attr & FA_FLOAT)
		{
			byteArr = floats.GetAt(ii);
			for (jj = 0; jj < m_validRows; jj++)
			{
				if (jj > byteArr->GetUpperBound() || jj > column->m_floats.GetUpperBound())
					break;

				column->m_floats.SetAt(jj, byteArr->GetAt(jj));
			}
		}

		if (m_form->attr2 & GO_TABLE)
		{
			attrArr = attrs.GetAt(column->m_ref);
			byteArr = enables.GetAt(column->m_ref);
			for (jj = 0; jj < m_validRows; jj++)
			{
				if (jj > attrArr->GetUpperBound() || jj > column->m_attrs.GetUpperBound()
					|| jj > byteArr->GetUpperBound() || jj > column->m_enables.GetUpperBound())
					break;

				column->m_attrs.SetAt(jj, attrArr->GetAt(jj));
				column->m_enables.SetAt(jj, byteArr->GetAt(jj));
			}
		}
	}

	if (m_form->attr2 & GO_ROW)
	{
		for (ii = 0; ii < m_totCols; ii++)
		{
			column = m_columns.GetAt(ii);
			column->m_pRGBs.RemoveAll();
			for (jj = 0; jj < m_totRows; jj++)
			{
				if (jj % 2)
					column->m_pRGBs.Add(m_sRGB);
				else
					column->m_pRGBs.Add(m_fRGB);
			}
		}
	}

	for (ii = 0; ii < rArray.GetSize(); ii++)
	{
		tmps = rArray.GetAt(ii);
		if (ii > m_data.GetUpperBound())
			break;
		m_data.SetAt(ii, tmps);
		col = ii % m_totCols;
		makeSignData(tmps, col, ii / m_totCols);
		tmps = editData(tmps, col, ii / m_totCols);
		m_edit.SetAt(ii, tmps);
	}

	for (ii = 0; ii < m_totCols; ii++)
	{
		rgbArr = RGBs.GetAt(ii);
		rgbArr->pRGBs.RemoveAll();
		rgbArr->tRGBs.RemoveAll();
		rgbArr->itRGBs.RemoveAll();
		delete rgbArr;

		byteArr = valids.GetAt(ii);
		byteArr->RemoveAll();
		delete byteArr;

		byteArr = floats.GetAt(ii);
		byteArr->RemoveAll();
		delete byteArr;

		if (m_form->attr2 & GO_TABLE)
		{
			attrArr = attrs.GetAt(ii);
			attrArr->RemoveAll();
			delete attrArr;

			byteArr = enables.GetAt(ii);
			byteArr->RemoveAll();
			delete byteArr;
		}
	}
	RGBs.RemoveAll();
	attrs.RemoveAll();
	valids.RemoveAll();
	enables.RemoveAll();
	floats.RemoveAll();

	CRect	tmpRc1 = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		tmpRc1.DeflateRect(2, 2);
	if (m_vscroll)
		tmpRc1.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tmpRc1.right = m_pscr.left;
	if (m_hscroll)
		tmpRc1.bottom = m_hscr.top;
	invalidateRect(&tmpRc1, false);
}

BOOL CfmGrid::IsColumnVisible(long column)
{
	if (column > 0 && column <= m_totCols)
	{
		Ccolumn* col= getColumn(column-1);
		if (col)
			return col->m_visible ? TRUE : FALSE;
	}
	return FALSE;
}

void CfmGrid::SetColumnVisible(long column, BOOL visible)
{
	if (column > 0 && column <= m_totCols)
		SetVisible(visible ? true : false, column-1);
}

BOOL CfmGrid::IsCellEnable(long row, long column)
{
	if (row > 0 && row <= m_totRows && column > 0 && column <= m_totCols)
	{
		Ccolumn* col = getColumn(column-1);
		if (row <= col->m_enables.GetSize())
			return col->m_enables.GetAt(row-1) ? TRUE : FALSE;
	}

	return FALSE;
}

void CfmGrid::SetCellEnable(long row, long column, BOOL enable)
{
	row--; column--;
	if (row >= 0 && row < m_totRows && column >= 0 && column < m_totCols)
	{
		Ccolumn* col = getColumn(column);
		if (row < col->m_enables.GetSize())
		{
			col->m_enables.SetAt(row, enable ? 1 : 0);
			if (m_form->properties & PR_VISIBLE)
			{
				CRect	tRc(0,0,0,0);
				if (getRect(row, findColumn(column), tRc))
					invalidateRect(&tRc, false);
			}
		}
	}
}

BOOL CfmGrid::IsColumnSortable(long column)
{
	if (column > 0 && column <= m_totCols)
		return (m_cellR[column-1].properties & PR_SORT) ? TRUE : FALSE;

	return FALSE;
}

CString CfmGrid::GetColumnName(long column)
{
	CString	name;

	name.Empty();
	if (column > 0 && column <= m_totCols)
		name = (char *)m_cellR[column-1].name;

	return name;
}

void CfmGrid::SetDataPosition(long method)
{
	switch (method)
	{
	case 1:
		m_save = svInsert;	break;
	case 2:
		m_save = svAppend;	break;
	case 0:
	default:
		m_save = svExchange;	break;
	}
}

void CfmGrid::SetItems(int col, CString items)
{
	if (col > 0 && col <= m_totCols)
	{
		col--;
		Ccolumn* column = getColumn(col);
		if (m_cellR[col].kind == CK_COMBO && !(m_cellR[col].properties & PR_FLEXCB))
		{
			CString	tmps, indexs;
			int	pos;
			
			pos = items.Find('\t');
			if (items.IsEmpty() || items.GetLength() < m_cellR[col].size || pos == -1)
				return;

			tmps = items.Left(pos);
			if (tmps.GetLength() < m_cellR[col].size)
				indexs = _T("");
			else
				indexs = tmps.Left(m_cellR[col].size);

			column->m_data.RemoveAll();
			column->m_display.RemoveAll();
			m_cellR[col].keys = 0;
			items = (items.GetLength() > pos+1) ? items.Mid(pos+1) : _T("");
			for ( ; !items.IsEmpty(); )
			{
				pos = items.Find('\t');
				if (pos != -1)
				{
					tmps  = items.Left(pos);
					items = (items.GetLength() > pos+1) ? items.Mid(pos+1) : _T("");
				}
				else
				{
					tmps  = items;
					items = _T("");
				}

				column->m_data.Add(tmps.Left(m_cellR[col].size));
				tmps = (tmps.GetLength() > m_cellR[col].size) ? tmps.Mid(m_cellR[col].size) : _T("");
				column->m_display.Add(tmps);
			}

			if (indexs.IsEmpty() && column->m_data.GetSize())
				indexs = column->m_data.GetAt(0);
			m_cellR[column->m_ref].keys = column->m_data.GetSize();
			for (int ii = 0; ii < m_totRows; ii++)
				WriteData(indexs, true, col, ii);
		}
	}
}

void CfmGrid::SetCellItems(long row, long col, CString items)
{
	if (row > 0 && row <= m_validRows && col > 0 && col <= m_totCols)
	{
		CString	str;
		Ccolumn* column;
		
		col--; row--;
		str = items;
		column = getColumn(col);
		if (!str.IsEmpty() && m_cellR[col].kind == CK_COMBO && (m_cellR[col].properties & PR_FLEXCB))
		{
			int	pos;
			CString	tmps, indexs;
		
			pos = str.Find('\t');
			if (str.GetLength() < m_cellR[col].size || pos == -1)
				return;

			tmps = str.Left(pos);
			if (tmps.GetLength() < m_cellR[col].size)
				indexs = _T("");
			else
				indexs = tmps.Left(m_cellR[col].size);

			str = (str.GetLength() > pos+1) ? str.Mid(pos+1) : _T("");
			column->m_items.SetAt(row, str);
			if (indexs.IsEmpty() && !str.IsEmpty())
				indexs = getString(col, row, 0);
			WriteData(indexs, true, col, row);
		}
	}
}

void CfmGrid::SetCellFocus(long row, long col) 
{
	CRect	tRc;
	Ccolumn* column;
	
	col--; row--;
	column = getColumn(col);
	if (row < 0 || row >= m_totRows || col < 0 || col >= m_totCols
			|| m_cellR[col].iok == EIO_NOP || m_cellR[col].iok == EIO_OUTPUT
			|| !((m_form->attr2 & GO_TABLE) && column->m_enables.GetAt(row)))
		return;

	m_index   = row * m_multiN;
	m_cellpos = Cpos(row+1, findColumn(col)+1);
	m_curpos  = m_cellpos;
	changeIME(row, m_curpos.m_col-1);
	switch (m_cellR[col].kind)
	{
	case CK_NORMAL:
		m_state = stEdit;	break;
	case CK_CHECK:
		m_state = stCheck;	break;
	case CK_COMBO:
		m_state = stCombo;	break;
	case CK_BUTTON:
		m_state = stButton;	break;
	default:
		m_state = stFocus;	break;
	}
	tRc.SetRectEmpty();
	if (getRect(row, m_curpos.m_col-1, tRc))
		m_cell = tRc;

	m_axform->DoSomething(doFOCUS, this);
}

void CfmGrid::Import(CString files)
{
	CFile	file;
	DWORD	size;
	char*	wb;

	if (files.IsEmpty())
	{
		CFileDialog fDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*|");
		if (fDlg.DoModal() != IDOK)
			return;
		files = fDlg.GetPathName();
	}

	if (!file.Open(files, CFile::modeRead|CFile::typeBinary))
		return;

	size = (DWORD)file.GetLength();
	wb = new char[size+1];
	if (file.Read(wb, size) != size)
	{
		delete[] wb;
		file.Close();
		return;
	}
	wb[size] = '\0';
	WriteAll(wb);
	delete[] wb;
	file.Close();
}

void CfmGrid::SetCellAttr(long row, long column, long attrAdd, long attrRemove)
{
	if (!(m_form->attr2 & GO_TABLE))
		return;

	struct	_FCC	fcc;

	fcc.fcc = 0x1a;
	fcc.scc = (unsigned char)attrAdd;
	fcc.rcc = (unsigned char)attrRemove;
	m_axform->DoSomething(doATTR, this, MAKEWPARAM(row-1, column-1), (LPARAM)&fcc);
}

void CfmGrid::Refresh()
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	DoListBox(false);
	invalidateRect(&m_pRc, false);
}

void CfmGrid::DoListBox(bool apply, bool focus)
{
	if (m_cbspread && m_list)
	{
		if (!focus)
			m_cbspread = false;
		if (apply)
		{
			int	col, sel = m_list->GetCurSel();

			col = m_columns.GetAt(m_curpos.m_col-1)->m_ref;
			if (sel >= 0 && sel < m_list->GetCount())
				WriteData(getString(col, m_curpos.m_row-1, sel, false), true, col, m_curpos.m_row-1); 
			m_changed    = true;
			m_changedpos = m_curpos;
			m_axform->DoComboBox(this);
		}
		delete m_list;
		m_list  = NULL;
		m_state = stCombo;
		m_axform->DoSomething(doCOMBO, this, 0);
		m_axform->m_view->SetFocus();
		m_axform->DoSomething(doFOCUS, this);
	}
}

bool CfmGrid::IsFocus(HWND hWnd)
{
	if (hWnd == NULL && m_list && m_cbspread)
		return true;
	return false;
}

bool CfmGrid::IsAutoThousand()
{
	Ccolumn* col;
	
	col = m_columns.GetAt(m_curpos.m_col-1);
	switch (m_cellR[col->m_ref].iok)
	{
	case EIO_INPUT:
	case EIO_INOUT:
		if (m_cellR[col->m_ref].kind == CK_NORMAL && (m_cellR[col->m_ref].properties & PR_THOUSAND))
			return true;
	default:
		break;
	}
	
	return false;
}
