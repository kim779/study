// fmGridx.cpp: implementation of the CfmGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxmt.h>
#include <math.h>

#include "grid.h"
#include "fmGrid.h"
#include "axForm.h"
#include "cblist.h"
#include "image.h"

#include "../lib/axislib.h"
#include "../../h/axiserr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CfmGrid::insertRow(int index, CString str, bool dataOnly, BOOL skip)
{
	CString	tmps  = _T("");
	CString	tmps_ = _T("");
	int	offs  = 0;
	COLORREF tRGB, pRGB;
	int	itRGB;
	bool	allSpace = false;
	int	row = index;
	Ccolumn* col;

	tmps_ = str;
	tmps_.TrimRight();
	if (tmps_.GetLength() == 0)
		allSpace = true;

	tmps_.Empty();
	offs = str.Find('\n');
	if (offs >= 0)
		str = str.Left(offs);

	for (int ii = 0; ii < m_totCols; ii++)
	{
		col = getColumn(ii);
		if (skip && (m_cellR[ii].iok == EIO_INPUT || m_cellR[ii].iok == EIO_NOP))
			tmps = _T("");	// tmps = CString(' ', m_cellR[ii].size);
		else
		{
			if (allSpace)
			{
			//	tmps = CString(' ', m_cellR[ii].size);
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
			else
			{
				offs = str.Find('\t');
				if (offs >= 0)
				{
					tmps = str.Left(offs);
					offs++;
					if (str.GetLength() > offs)
						str = str.Mid(offs);
					else
						str = _T("");
				}
				else if (ii == m_totCols - 1)
					tmps = str;
			}
		}

		if (m_cellR[ii].kind == CK_COMBO)
		{
			tmps_ = tmps;
			tmps_.Trim();
			if (tmps_.IsEmpty())
				tmps = getString(ii, (index >= 0) ? index : m_validRows-1, 0, false);
		}

		if (index >= 0)		// first & N
		{
//2020/02/17 [KJS] index가 배열크기보다 같거나 크면 insert 중지	 대표로 (m_pRGBs, m_tRGBs, m_itRGBs, m_floats, m_attrs, m_enables, m_data, m_edit)		
			if (!col->m_valids.InsertAt(index, 1))
				return -1;

			pRGB  = col->m_pRGBs.GetAt(index);
			tRGB  = col->m_tRGBs.GetAt(index);
			itRGB = col->m_itRGBs.GetAt(index);
			col->m_pRGBs.InsertAt(index, pRGB);
			col->m_tRGBs.InsertAt(index, tRGB);
			col->m_itRGBs.InsertAt(index, itRGB);
			if (m_cellR[ii].attr & FA_FLOAT)
				col->m_floats.InsertAt(index, 0xff);
			if (m_form->attr2 & GO_TABLE)
			{
				col->m_attrs.InsertAt(index, m_cellR[ii].attr);
				col->m_enables.InsertAt(index, 1);
			}

			m_data.InsertAt(index*m_totCols+ii, tmps);
			m_edit.InsertAt(index*m_totCols+ii, tmps);
			WriteData(tmps, false, ii, index);
		}
		else			// last
		{
			if (m_validRows < m_totRows)
			{
				col->m_valids.SetAt(m_validRows, 1);
				pRGB  = col->m_pRGBs.GetAt(max(0, m_validRows-1));
				tRGB  = col->m_tRGBs.GetAt(max(0, m_validRows-1));
				itRGB = col->m_itRGBs.GetAt(max(0, m_validRows-1));
				col->m_pRGBs.SetAt(m_validRows, pRGB);
				col->m_tRGBs.SetAt(m_validRows, tRGB);
				col->m_itRGBs.SetAt(m_validRows, itRGB);
				if (m_cellR[ii].attr & FA_FLOAT)
					col->m_floats.SetAt(m_validRows, 0xff);
				if (m_form->attr2 & GO_TABLE)
				{
					col->m_attrs.InsertAt(m_validRows, m_cellR[ii].attr);
					col->m_enables.InsertAt(m_validRows, 1);
				}

				m_data.SetAt(m_validRows * m_totCols+ii, tmps);
				m_edit.SetAt(m_validRows * m_totCols+ii, tmps);
			}
			else
			{
				col->m_valids.Add(1);
				pRGB  = col->m_pRGBs.GetAt(col->m_tRGBs.GetUpperBound());
				tRGB  = col->m_tRGBs.GetAt(col->m_tRGBs.GetUpperBound());
				itRGB = col->m_itRGBs.GetAt(col->m_tRGBs.GetUpperBound());
				col->m_pRGBs.Add(pRGB);
				col->m_tRGBs.Add(tRGB);
				col->m_itRGBs.Add(itRGB);
				if (m_cellR[ii].attr & FA_FLOAT)
					col->m_floats.Add(0xff);
				if (m_form->attr2 & GO_TABLE)
				{
					col->m_attrs.Add(m_cellR[ii].attr);
					col->m_enables.Add(1);
				}

				m_data.Add(tmps);
				m_edit.Add(tmps);
			}
			WriteData(tmps, false, ii, m_validRows);
			row = m_validRows;
		}
	}

	if (m_validRows < m_totRows)
		m_validRows++;

	offs = m_data.GetSize();
	if (offs > m_totRows * m_totCols)
	{
		if (index >= 0)
		{
			for (int ii = 0; ii < offs - m_totRows * m_totCols; ii++)
			{
				col = m_columns.GetAt(ii);
				m_data.RemoveAt(m_totRows * m_totCols);
				m_edit.RemoveAt(m_totRows * m_totCols);
				col->m_valids.RemoveAt(m_totRows);
				col->m_pRGBs.RemoveAt(m_totRows);
				col->m_tRGBs.RemoveAt(m_totRows);
				col->m_itRGBs.RemoveAt(m_totRows);
				if (m_cellR[col->m_ref].attr & FA_FLOAT)
					col->m_floats.RemoveAt(m_totRows);
				if (m_form->attr2 & GO_TABLE)
				{
					col->m_attrs.RemoveAt(m_totRows);
					col->m_enables.RemoveAt(m_totRows);
				}
			}
		}
		else
		{
			for (int ii = 0; ii < m_totCols; ii++)
			{
				col = m_columns.GetAt(ii);
				m_data.RemoveAt(0);
				m_edit.RemoveAt(0);
				col->m_valids.RemoveAt(0);
				col->m_pRGBs.RemoveAt(0);
				col->m_tRGBs.RemoveAt(0);
				col->m_itRGBs.RemoveAt(0);
				if (m_cellR[col->m_ref].attr & FA_FLOAT)
					col->m_floats.RemoveAt(0);
				if (m_form->attr2 & GO_TABLE)
				{
					col->m_attrs.RemoveAt(0);
					col->m_enables.RemoveAt(0);
				}
			}
// updateXXX_2022XX ???	
			m_stRow--;
		}
	}

	if (!dataOnly && (m_form->attr2 & GO_ROW))
	{
		for (int ii = 0; ii < m_totCols; ii++)
		{
			col = getColumn(ii);
			col->m_pRGBs.RemoveAll();
			for (int jj = 0; jj < m_totRows; jj++)
			{
				if (jj % 2)
					col->m_pRGBs.Add(m_sRGB);
				else
					col->m_pRGBs.Add(m_fRGB);

				if (m_cellR[ii].attr & FA_CONTRAST && (m_cellR[ii].attr2 & SA_CONTRAST) == SA_PAINT)
				{
					tmps = m_data.GetAt(jj*m_totCols+ii);
					makeSignData(tmps, ii, jj);
				}
			}
		}
	}

	return row;
}

void CfmGrid::readData(CString& data, bool edit, int col, int row)
{
	int	len,  flen;

	if (row == -2)
	{
		if (col == -1)
			col = edit ? m_columns.GetAt(m_cellpos.m_col-1)->m_ref : m_cellpos.m_col-1;
		if (col >= 0)
		{
			Ccolumn* column;
		
			column = edit ? m_columns.GetAt(col) : getColumn(col);
			data   = column->m_head;
		}
		return;
	}

	if (row == -3)
	{
		row  = (short)HIWORD(col);
		col  = (short)LOWORD(col);
		data = _T("");

		switch (m_cellR[col].isHint)
		{
		case IH_STRING:
			data = (char *)m_cellR[col].onHint;
			break;
		case IH_REF:
			row = findRow(row, true);
			if (row >= 0)
			{
				col = findColumn((char *)m_cellR[col].onHint);
				if (col >= 0)
					ReadData(data, true, -1, col, row);
			}
			break;
		}
		return;
	}

	if ((m_form->type != m_totCols && m_stCol < m_form->type) || col < 0 || row < 0)
		return;

	CString	tmps;
	Ccolumn* column = m_columns.GetAt(col);
	col = column->m_ref;

	if (row * m_totCols + col > m_edit.GetUpperBound())
		return;

	switch (m_cellR[col].kind)
	{
	case CK_COMBO:
		tmps = m_data.GetAt(row * m_totCols + col); tmps.Trim();
		if (tmps.IsEmpty())
			data = getString(col, row, 0);
		else
			data = getString(col, row, tmps);
		return;
	case CK_CHECK:
		data = m_data.GetAt(row * m_totCols + col);
		if (data.IsEmpty())
			data = _T('0');
		return;
	case CK_GRAPH:
	default:
		break;
	}

	tmps = m_edit.GetAt(row * m_totCols + col);
	len  = tmps.GetLength();
	flen = m_cellR[col].size;
	if (m_cellR[col].attr & FA_COMMA)
		flen += (flen - 1)/3;
	if (m_cellR[col].attr & FA_FLOAT)
		flen++;

	flen = max(flen, len);
	if (m_cellR[col].attr & FA_RIGHT)
	{
		data  = CString(' ', flen-len);
		data += tmps;
	}
	else
	{
		data  = tmps;
		data += CString(' ', flen-len);
	}
}

CString CfmGrid::readAll(bool head)	// edited string
{
	int	rows;
	Ccolumn* col;
	CString	tmps = _T(""), data;

	data.Empty();
	rows = GetValidRows();
	if (rows == 0)
		return data;

	if (head)
	{
		for (int ii = 0; ii < m_totCols; ii++)	// read head
		{
			col = m_columns.GetAt(ii);
			if (col->m_visible)
			{
				ReadData(tmps, true, -1, col->m_ref, -2);
				data += tmps;
				data += '\t';
			}
			if (m_cellR[col->m_ref].attr & FA_WRAP)
				data += '\n';
		}
		data += '\n';
	}

	for (int ii = 0; ii < rows; ii++)
	{
		tmps.Empty();
		for (int jj = 0; jj < m_totCols; jj++)
		{
			col = m_columns.GetAt(jj);
			if (col->m_visible)
			{
				if (ii*m_totCols+col->m_ref <= m_data.GetUpperBound())
					tmps += m_edit.GetAt(ii*m_totCols+col->m_ref);
				else
					tmps += CString(' ', m_cellR[col->m_ref].size);
				tmps += '\t';
			}
			if (m_cellR[col->m_ref].attr & FA_WRAP)
				tmps += '\n';
		}
		data += tmps;
		data += '\n';
	}
	return data;
}

CString CfmGrid::editData(CString str, int col, int row, bool input)
{
	CString tmps, tmps_;
	int	ii,  len,  dcnt = 0;
	DWORD	attr;
	Ccolumn* column;

	column = getColumn(col);
	row    = max(row, 0);
	attr   = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[col].attr;
	tmps   = str;
	if (attr & FA_RIGHT)
		tmps.TrimRight();

	switch (m_cellR[col].kind)
	{
	case CK_BUTTON:
		return tmps;
	case CK_COMBO:
		return getString(col, row, tmps);
	default:
		break;
	}

	if (m_cellR[col].isEdit == IE_EDIT && !column->m_edit.IsEmpty())
	{
		tmps.Empty();
		tmps_ = str;

		str.TrimLeft();
		str.TrimRight();
		len = str.GetLength();
		if (len == 0 && m_cellR[col].iok == EIO_OUTPUT)
			return tmps;

		CString	format;
		if (attr & FA_RIGHT)
		{
			format.Empty();
			for (ii = column->m_edit.GetLength()-1; ii >= 0; ii--)
			{
				if (column->m_edit[ii] == '9')
					dcnt++;

				if (dcnt >= len)
					break;
			}
			format = column->m_edit.Mid(ii);
		}
		else
			format = column->m_edit;

		dcnt = 0;
		for (ii = 0; ii < format.GetLength(); ii++)
		{
			if (format[ii] == '9')
			{
				if (dcnt >= len)
				{
					if ((m_cellR[col].iok == EIO_INPUT || m_cellR[col].iok == EIO_INOUT)
						&& (m_cellR[col].properties & PR_EDIT))
						tmps += ' ';
					else	break;
				}
				else
				{
					tmps += str[dcnt];
					dcnt++;
				}
			}
			else
				tmps += format[ii];
		}
		return tmps;
	}

	if (attr & FA_PASSWD)
	{
		tmps_ = str;
		tmps_.TrimRight();
		tmps.Empty();
		for (ii = 0; ii < tmps_.GetLength(); ii++)
			tmps += '*';
		return tmps;
	}

	if (!input)
	{
		formatData(attr, tmps, (m_cellR[col].vals[1] == m_cellR[col].size) ? -1 : (int)m_cellR[col].vals[1], m_cellR[col].size);
		return tmps;
	}

	if (attr & FA_COMMA || attr & FA_FLOAT)
	{
		bool	zero = true;
		CString	valS = _T("");
		int	idx = 0;
		int	floatx = 0xff;

		if (attr & FA_FLOAT)
			floatx = (row < column->m_floats.GetSize()) ? column->m_floats.GetAt(row) : 0xff;

		tmps_.Empty();
		tmps.TrimLeft();
		tmps.TrimRight();
		if (tmps.GetLength() == 0)
			return tmps;

		valS = CString(' ', tmps.GetLength());
		for (ii = 0; ii < tmps.GetLength(); ii++)
		{
			switch (tmps[ii])
			{
			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
			case '.':
				zero = false;
				valS.SetAt(ii, 'V');
				break;

			case '0':
				if ((m_cellR[col].attr & FA_FLOAT)
					&& (ii >= floatx-1 || (floatx == 0xff && !ii)))
				{
					zero = false;
					valS.SetAt(ii, 'V');
					break;
				}

			case ' ':
				if (zero)
				{
					valS.SetAt(ii, 'G');
					tmps.SetAt(ii, ' ');
				}
				else
					valS.SetAt(ii, 'V');
				break;

			default:
				valS.SetAt(ii, 'S');
				break;
			}
		}

		if (zero)// && tmps_.IsEmpty())
		{
		//	if (attr & FA_COMMA)
			ii = valS.Find('S');
			if (ii != -1)
				tmps_ += tmps.GetAt(ii);
			else
				tmps_ += '0';
			return tmps_;
		}

		tmps_ = tmps;
		tmps_.MakeReverse();
		valS.MakeReverse();
		tmps.Empty();
		ii = 0;
		if (attr & FA_FLOAT)
		{
			if (m_cellR[col].iok == EIO_INPUT || m_cellR[col].iok == EIO_INOUT)
			{
				if (floatx != 0xff)
				{
					for (ii = 0; ii < tmps_.GetLength() - floatx; ii++)
					{
						if (ii > valS.GetLength()-1)
						{
							tmps += '0';
							continue;
						}

						switch (valS[ii])
						{
						case 'V':
						case 'S':
							tmps += tmps_[ii];
							break;
						case 'G':
							tmps += '0';
						default:
							break;
						}
					}
					tmps += '.';
				}
			}
			else
			{
				for (ii = 0; ii < (int)m_cellR[col].vals[1]; ii++)
				{
					if (ii > valS.GetLength()-1)
					{
						tmps += '0';
						continue;
					}

					switch (valS[ii])
					{
					case 'V':
					case 'S':
						tmps += tmps_[ii];
						break;
					case 'G':
						tmps += '0';
					default:
						break;
					}
				}
				tmps += '.';
			}

			if (ii > valS.GetLength()-1)
			{
				tmps += '0';
				tmps.MakeReverse();
				if (attr & FA_ENUM)
				{
					ii = tmps.Find('.');
					if (ii >= 0)
					{
						tmps_ = tmps.Mid(ii+1);
						if (!atoi(tmps_))
							tmps = tmps.Left(ii);
					}
				}
				return tmps;
			}
			else
			{
				if (valS[ii] == 'G')
				{
					tmps += '0';
					idx++;
					ii++;
				}

				if (!(attr & FA_COMMA))
				{
					for ( ; ii < tmps_.GetLength(); ii++)
						tmps += tmps_[ii];

					tmps.MakeReverse();
					if (attr & FA_ENUM)
					{
						ii = tmps.Find('.');
						if (ii >= 0)
						{
							tmps_ = tmps.Mid(ii+1);
							if (!atoi(tmps_))
								tmps = tmps.Left(ii);
						}
					}
					return tmps;
				}
				if (idx > 0)
					ii--;
			}
			if (valS.GetLength() > ii)
				valS = valS.Mid(ii);
			else
				valS = _T("");

			if (tmps_.GetLength() > ii)
				tmps_ = tmps_.Mid(ii);
			else
				tmps_ = _T("");
		}
		else
		{
			ii = tmps_.Find('.');
			if (ii >= 0)
			{
				if (ii < tmps_.GetLength()-1)
				{
					ii++;
					tmps += tmps_.Left(ii);
					if (valS[ii] == 'G')
					{
						tmps += '0';
						idx++;
					}
				}
				else if (ii == tmps_.GetLength() -1)
				{
					ii++;
					tmps += tmps_.Left(ii);
					tmps += '0';
					idx++;
				}

				if (valS.GetLength() > ii)
					valS = valS.Mid(ii);
				else
					valS = _T("");

				if (tmps_.GetLength() > ii)
					tmps_ = tmps_.Mid(ii);
				else
					tmps_ = _T("");

				int	sign = valS.Find('S');
				CString	tmpX  = valS;
				CString tmpX_ = tmps_;
				if (sign >= 0)
				{
					tmpX  = valS.Left(sign);
				//	tmpX += ' ';
					if (valS.GetLength() > sign)
						tmpX += valS.Mid(sign);

					tmpX_  = tmps_.Left(sign);
				//	tmpX_ += ' ';
					if (tmps_.GetLength() > sign)
						tmpX_ += tmps_.Mid(sign);
				}
				else
				{
					tmpX  += ' ';
					tmpX_ += ' ';
				}
				valS = tmpX;
				tmps_ = tmpX_;
			}
		}

		for (ii = 0;  ii < valS.GetLength(); ii++)
		{
			if (ii < idx)
				continue;

			if ((ii != 0) && ((ii % 3) == 0))
			{
				switch (valS[ii])
				{
				case 'G':
					tmps += ' ';
					break;
				case 'V':
					tmps += ',';
				default:
					break;
				}
			}
			tmps += tmps_[ii];
		}

		tmps.MakeReverse();
	}

	if ((attr & FA_FLOAT) && (attr & FA_ENUM))
	{
		ii = tmps.Find('.');
		if (ii >= 0)
		{
			tmps_ = tmps.Mid(ii+1);
			if (!atoi(tmps_))
				tmps = tmps.Left(ii);
		}
	}

	return tmps;
}

void CfmGrid::makeSignData(CString& data, int col, int row)
{
	Ccolumn* column = getColumn(col);
	if (!(m_cellR[col].attr & FA_CONTRAST) || data.IsEmpty())
		return;

	CString	tmps = _T("");
	int	state = ct_Flat;

	data.TrimLeft();
	if (data.IsEmpty())
		return;

	state = m_axform->GetContrast(data.GetAt(0), tmps);
	tmps += CString(' ', m_cellR[col].size-data.GetLength());
	tmps += data.Mid(1);
	data = tmps;

	int	rgb;
	DWORD	upRGB, dnRGB;
	if (m_cellR[col].onEdit != NOVALUE)
	{
		upRGB = m_axform->m_valueR[m_cellR[col].onEdit];
		dnRGB = m_axform->m_valueR[m_cellR[col].onEdit+1];
	}

	switch (state)
	{
	case ct_uLimit:
	case ct_Rise:
		rgb = (m_cellR[col].onEdit == NOVALUE) ? cRGB_UP : upRGB;
		break;
	case ct_lLimit:
	case ct_Fall:
		rgb = (m_cellR[col].onEdit == NOVALUE) ? cRGB_DN : dnRGB;
		break;
	case ct_Flat:
	default:
		rgb = m_cellR[col].tRGB;	break;
	}
	rgb = (rgb < 0) ? m_cellR[col].tRGB : rgb;

	if ((m_cellR[col].attr2 & SA_CORGB) == SA_COSUP)
		data = data.Mid(2);

	if (row >= 0)
	{
		if ((m_cellR[col].attr2 & SA_CONTRAST) == SA_PAINT)
		{
			if (state == ct_uLimit || state == ct_lLimit)
			{
				setFgColor(row, col, getRGB(cWHITE));
				setBkColor(row, col, rgb);
			}
			else
			{
				setFgColor(row, col, rgb);
				if (m_form->attr2 & GO_ROW)
					column->m_pRGBs.SetAt(row, (row % 2) ? m_sRGB : m_fRGB);
				else
				{
					rgb = (m_cellR[col].pRGB  & userCOLOR) ? m_cellR[col].pRGB : getRGB(m_cellR[col].pRGB);
					column->m_pRGBs.SetAt(row, rgb);
				}

			}
		}
		else	setFgColor(row, col, rgb);
	}
}

void CfmGrid::doColumnEditCORGB(int row, int col, CString& data)
{
	if (m_cellR[col].onEdit == NOVALUE)
		return;

	int rgb = getCORGB(m_cellR[col].onEdit, data, m_cellR[col].size, m_cellR[col].attr2 & SA_CORGB);
	rgb = (rgb < 0) ? m_cellR[col].tRGB : rgb;
	setFgColor(row, col, rgb);
}

void CfmGrid::drawEdit(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB, COLORREF tRGB)
{
	CRect	tRc;
	CSize	size;
	CPoint	point;
	CBrush*	brush;
	bool	focus_this = false;
	int	skipN, index, getrow = (m_headN == 0) ? 0 : 1;

	setFont(dc, (m_cellR[column->m_ref].properties & PR_BOLD) ? 1 : 0);
	if (m_form->attr2 & (GO_MERGE|GO_XHEAD))
		skipN = m_headN*m_multiN;
	else
		skipN = m_multiN;
	if (m_form->attr2 & GO_MERGE)
		index = row - m_headN;
	else
		index = row/m_multiN-getrow;
	
	if ((row-skipN == m_index) && (col == m_curpos.m_col-1))
	{
		focus_this = true;
		m_cell = rc;
	}

	if (m_edcaret && focus_this)
	{
		m_edcaret = false;
		if (m_state == stEdit)
			m_axform->m_view->CreateSolidCaret(2, (int)m_fHeight);
		else
		{
			focus_this = false;
			m_axform->m_view->HideCaret();
		}
	}

	if (m_focus && focus_this && m_state == stEdit)
	{
		brush = setBrush(dc, getRGB(cWHITE));
		if (m_edstate == esReverse)
		{
			setTextColor(dc, getRGB(cWHITE));
			setPaintColor(dc, getRGB(cDBLUE));
		}
		else
		{
			setTextColor(dc, getRGB(cDBLUE));
			setPaintColor(dc, getRGB(cWHITE));
		}
	}
	else
	{
		if (pRGB == 0xffffffff)
		{
			if (isReverseCell(index, col))		// selected cell
				pRGB = getRGB(cBLACK);
			else if (index > column->m_pRGBs.GetUpperBound())
				pRGB = column->m_pRGBs.GetAt(column->m_pRGBs.GetUpperBound());
			else
				pRGB = column->m_pRGBs.GetAt(index);

		}

		if (tRGB == 0xffffffff)
		{
			if (index > column->m_tRGBs.GetUpperBound())
				tRGB = column->m_tRGBs.GetAt(column->m_tRGBs.GetUpperBound());
			else
				tRGB = column->m_tRGBs.GetAt(index);
			if (isReverseCell(index, col))
				tRGB = getReverseColor(tRGB);
		}

		brush = setBrush(dc, pRGB);
		setTextColor(dc, tRGB);
		setPaintColor(dc, pRGB);
	}

	if ((m_form->attr2 & GO_TABLE) && (index <= column->m_enables.GetUpperBound())
			&& !column->m_enables.GetAt(index))	// disable
	{
		brush = setBrush(dc, getRGB(cXBG));
		setTextColor(dc, getRGB(cXFG));
		setPaintColor(dc, getRGB(cXBG));
	}
	dc->FillRect(rc, brush);

	tRc = rc;
	tRc.DeflateRect(2, 2);

	if (m_cellR[column->m_ref].isEdit != IE_EDIT || !(m_cellR[column->m_ref].properties & PR_EDIT))
	{
		data.TrimLeft();
		data.TrimRight();
	}
	CString	tmps = data;
	if (data.GetLength() != 0)
	{
		size = dc->GetOutputTextExtent(data);
		if (size.cx > tRc.Width())
		{
			for (int ii = data.GetLength(); ii >= 0; ii--)
			{
				size = dc->GetOutputTextExtent(data.Left(ii));
				if (size.cx < tRc.Width())
				{
					//	tmps = data.Left(ii);
					//	if (focus_this && (m_state == stEdit))
					//		m_edcaretpos = min(m_edcaretpos, ii);
					break;
				}
			}
		}
	}
	else
	{
		size.cx = 0;
		size.cy = (int)m_fHeight;
	}
	
	if (m_cellR[column->m_ref].attr & FA_RIGHT)
		dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT|DT_NOPREFIX);
	else
	{
		dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);

		if ((m_data.GetSize()/m_totCols) > index)
			data = m_data.GetAt(index*m_totCols+col);
		else
			data = CString(' ', m_cellR[column->m_ref].size);

		if (focus_this)
		{
			data.TrimLeft();
			data.TrimRight();
			switch (m_edstate)
			{
			case esReverse :
				m_edcaretpos = data.GetLength();
			//	size = dc->GetOutputTextExtent(data);
				size.cx++;
				break;
			case esClick :
				for (m_edcaretpos = 0; m_edcaretpos < data.GetLength(); m_edcaretpos++)
				{
					if (whichHangulEdit(m_edcaretpos, data) == WC_H1)
					{
						m_edcaretpos++;
						size = dc->GetOutputTextExtent(tmps, calcCaret(data, column->m_edit));
					}
					else
						size = dc->GetOutputTextExtent(tmps, calcCaret(data, column->m_edit));
				
					if (tRc.left + size.cx >= m_edclick.x)
						break;
				}

				if (whichHangulEdit(m_edcaretpos, data) == WC_H2)
					m_edcaretpos--;
				m_edstate = esNormal;
			case esNormal :
			//	szie = dc->GetOutputTextExtent(tmps, calcCaret(data, column->m_edit));
				break;
			case esNone :
			default :
				break;
			}
		}
	}

	if (focus_this && (m_state == stEdit) && !m_edfocusing)
	{
		if ((m_cellR[column->m_ref].attr & FA_RIGHT) || m_cellR[column->m_ref].alignC == AL_RIGHT)
			point.x = tRc.left + tRc.Width() - 2;
		else
			point.x = tRc.left + size.cx;

		point.y = tRc.top + (tRc.Height() - (int)m_fHeight)/2;
		setCaretPos(point);
		m_axform->m_view->ShowCaret();
		m_edfocusing = true;
	}
}

void CfmGrid::drawCombo(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB, COLORREF tRGB)
{
	CRect	tRc, rBtn, rItem;
	CBrush*	brush = (CBrush *) 0;
	bool	spread, scroll, focus_this;
	int	skipN, index, getrow = (m_headN == 0) ? 0 : 1;

	spread = scroll = focus_this = false;
	if (m_form->attr2 & (GO_MERGE|GO_XHEAD))
		skipN = m_headN*m_multiN;
	else
		skipN = m_multiN;
	if (m_form->attr2 & GO_MERGE)
		index = row - m_headN;
	else
		index = row/m_multiN-getrow;

	if (m_focus && (row-skipN == m_index) && (col == m_curpos.m_col-1))
	{
		focus_this = true;
		m_cell = rc;
		if (m_state == stCombo)
			spread = m_cbspread;
	}

	UINT style = DFCS_SCROLLCOMBOBOX;
//	setPen(dc);
	setFont(dc, (m_cellR[column->m_ref].properties & PR_BOLD) ? 1 : 0);
	tRc = rc;
	tRc.left += 4;

	if (spread || (focus_this && m_state == stCombo))
	{
		brush = setBrush(dc, getRGB(cWHITE));
		setTextColor(dc, getRGB(cDBLUE));
		setPaintColor(dc, getRGB(cWHITE));
	}
	else
	{
		if (pRGB == 0xffffffff)
		{
			if (isReverseCell(index, col))
				pRGB = getRGB(cBLACK);
			else if (index > column->m_pRGBs.GetUpperBound())
				pRGB = column->m_pRGBs.GetAt(column->m_pRGBs.GetUpperBound());
			else
				pRGB = column->m_pRGBs.GetAt(index);
		}

		if (tRGB == 0xffffffff)
		{
			if (index > column->m_tRGBs.GetUpperBound())
				tRGB = column->m_tRGBs.GetAt(column->m_tRGBs.GetUpperBound());
			else
				tRGB = column->m_tRGBs.GetAt(index);
			if (isReverseCell(index, col))
				tRGB = getReverseColor(tRGB);
		}

		brush = setBrush(dc, pRGB);
		setTextColor(dc, tRGB);
		setPaintColor(dc, pRGB);
	}

	if ((m_form->attr2 & GO_TABLE) && (index <= column->m_enables.GetUpperBound())
		&& !column->m_enables.GetAt(index))	// disable
	{
		brush = setBrush(dc, getRGB(cXBG));
		setTextColor(dc, getRGB(cXFG));
		setPaintColor(dc, getRGB(cXBG));
		style |= DFCS_INACTIVE;
	}
	else if (m_click == ckCombo && focus_this)
		style |= DFCS_PUSHED;

	dc->FillRect(rc, brush);

	dc->DrawText(data, tRc, DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
//	drawEdge(dc, rc, EDGE_SUNKEN, BF_RECT);

	rBtn = rc;
	index = (style & DFCS_PUSHED) ? PI_DOWN : PI_DEFAULT;
	if (m_pic && m_pic->GetValid(index))
	{
	//	rBtn.DeflateRect(0, 1, 1, 1);
		rBtn.left = rBtn.right - (int)(szCOMBO*m_hR);
		if (rBtn.left >= rc.left)
			m_pic->DrawPicture(dc, rBtn, AL_STRETCH, m_axform->m_oldVers, index);
		else
			rBtn.left = rc.left;
	}
	else
	{
		rBtn.right  -= 2;
		rBtn.left    = rBtn.right - (int)(szCOMBO*m_hR);
		rBtn.top    += 2;
		rBtn.bottom -= 2;
		if (rBtn.left >= rc.left)
			dc->DrawFrameControl(rBtn, DFC_SCROLL, style);
		else
			rBtn.left = rc.left;
	}
	if (focus_this && m_state == stCombo)
		m_cbbtn = rBtn;
}

void CfmGrid::drawCheck(CDC* dc, CRect rc, Ccolumn* column, int col, int row, CString data, COLORREF pRGB, COLORREF tRGB)
{
	UINT	style = DFCS_BUTTONCHECK;
	CRect	tRc, tmpRc;
	CBrush*	brush = (CBrush *) 0;
	CSize	size;
	CString	tmps;
	CBitmap* bitmap = (CBitmap*) 0;
	int	image = 0, skipN, index, getrow = (m_headN == 0) ? 0 : 1;

	setFont(dc, (m_cellR[column->m_ref].properties & PR_BOLD) ? 1 : 0);
	if (m_form->attr2 & (GO_MERGE|GO_XHEAD))
		skipN = m_headN*m_multiN;
	else
		skipN = m_multiN;
	if (m_form->attr2 & GO_MERGE)
		index = row - m_headN;
	else
		index = row/m_multiN-getrow;

	if (row-skipN == m_index && m_focus && col == m_curpos.m_col-1 && m_state == stCheck)
	{
		brush = setBrush(dc, getRGB(cWHITE));
		setTextColor(dc, getRGB(cBLACK));
		setPaintColor(dc, getRGB(cWHITE));
		m_cell = rc;
		if (m_mousedown && m_click == ckItem)
			style |= DFCS_INACTIVE;
	}
	else
	{
		if (pRGB == 0xffffffff)
		{
			if (isReverseCell(index, col))
				pRGB = getRGB(cBLACK);
			else if (index > column->m_pRGBs.GetUpperBound())
				pRGB = column->m_pRGBs.GetAt(column->m_pRGBs.GetUpperBound());
			else
				pRGB = column->m_pRGBs.GetAt(index);
		}

		if (tRGB == 0xffffffff)
		{
			if (index > column->m_tRGBs.GetUpperBound())
				tRGB = column->m_tRGBs.GetAt(column->m_tRGBs.GetUpperBound());
			else
				tRGB = column->m_tRGBs.GetAt(index);
			if (isReverseCell(index, col))
				tRGB = getReverseColor(tRGB);
		}

		brush = setBrush(dc, pRGB);
		setTextColor(dc, tRGB);
		setPaintColor(dc, pRGB);
	}

	if ((m_form->attr2 & GO_TABLE) && index <= column->m_enables.GetUpperBound() && !column->m_enables.GetAt(index))	// disable
	{
		setTextColor(dc, getRGB(cXFG));
		style |= DFCS_INACTIVE;
	}

	tRc = tmpRc = rc;
	dc->FillRect(rc, brush);

	if (atoi(data) >= 1)
		style |= DFCS_CHECKED;

	tmps = column->m_edit; tmps.Trim();
	if (tmps.IsEmpty())
		tmps = "가나다ABW";
	size = dc->GetOutputTextExtent(tmps);
	tRc.bottom = rc.top  + size.cy;
	if (rc.Height() > tRc.Height())
	{
		int	dx = max(0, (rc.Height() - tRc.Height())/2 - 1);
		tRc.top += dx;
		tRc.bottom = min(rc.bottom, tRc.top+size.cy+1);
	}
	tRc.right  = rc.left + tRc.Height();
	size = dc->GetOutputTextExtent(column->m_edit);
	if (!column->m_edit.IsEmpty())
		size.cx += 3;
	switch (m_cellR[column->m_ref].alignC)
	{
	case AL_CENTER:
		index = rc.Width() - (tRc.Width() + size.cx);
		index /= 2;
		break;
	case AL_RIGHT:
		index = rc.Width() - (tRc.Width() + size.cx + 2);
		break;
	case AL_LEFT:
	default:
		index = 3;
		break;
	}
	tRc.OffsetRect(max(index, 0), 1);
	if (style & DFCS_CHECKED)
		image = (style & DFCS_INACTIVE) ? 3 : 1;
	else if (style & DFCS_INACTIVE)
		image = 2;
	else	image = 0;

	if (m_checks.Lookup(image, bitmap))
	{
		CDC	memDC;
		BITMAP	bm = {};

		memDC.CreateCompatibleDC(dc);
		bitmap->GetBitmap(&bm);
		if (!m_axform->m_oldVers)
		{
			CBitmap* pbmp = (CBitmap *)memDC.SelectObject(bitmap);
			::TransparentBlt(dc->m_hDC, tRc.left, tRc.top, tRc.Width(), tRc.Height(),
						memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		else
			transparentBlt(dc, bitmap, tRc.left, tRc.top, tRc.Width(), tRc.Height());
	}
	else	dc->DrawFrameControl(tRc, DFC_BUTTON, style);

	if (rc.Height() < rc.Width())
	{
		tmpRc.top = tRc.top;
		tmpRc.bottom = tRc.bottom;
		tmpRc.left = tRc.right + 3;
		dc->DrawText(column->m_edit, tmpRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
	}
}

void CfmGrid::drawButton(CDC* dc, CRect rc, Ccolumn* column, int col, int row)
{
	CRect	tRc;
	CBrush*	brush;
	bool	border, focus_this = false;
	int	skipN, index, getrow = (m_headN == 0) ? 0 : 1;
	COLORREF pRGB, tRGB;

	if (m_form->attr2 & (GO_MERGE|GO_XHEAD))
		skipN = m_headN*m_multiN;
	else
		skipN = m_multiN;
	if (m_form->attr2 & GO_MERGE)
		index = row - m_headN;
	else
		index = row/m_multiN-getrow;

	if (m_focus && row-skipN == m_index && col == m_curpos.m_col-1 && m_state == stButton)
	{
		m_cell = rc;
		focus_this = true;
	}

	if (index > column->m_pRGBs.GetUpperBound())
		pRGB = column->m_pRGBs.GetAt(column->m_pRGBs.GetUpperBound());
	else
		pRGB = column->m_pRGBs.GetAt(index);

	if (index > column->m_tRGBs.GetUpperBound())
		tRGB = column->m_tRGBs.GetAt(column->m_tRGBs.GetUpperBound());
	else
		tRGB = column->m_tRGBs.GetAt(index);

	border = false;
	if (m_cellR[column->m_ref].properties & (PR_IMAGE|PR_IMAGETEXT))
	{
		if (m_cellR[column->m_ref].properties & PR_FLEXCB)
		{
			Cimage*	image;
			if (column->m_images.Lookup(index, image) && image->GetValid(PI_DEFAULT))
				image->DrawPicture(dc, rc, m_cellR[column->m_ref].alignC, m_axform->m_oldVers, PI_DEFAULT);
		}
		else if (column->m_pic->GetValid(PI_DEFAULT))
			column->m_pic->DrawPicture(dc, rc, m_cellR[column->m_ref].alignC, m_axform->m_oldVers, PI_DEFAULT);

		if (focus_this && m_mousedown)
			border = true;
	}
	else if (!(m_cellR[column->m_ref].properties & PR_TRANSPARENT))
	{
		border = true;
		brush  = setBrush(dc, pRGB);
		dc->FillRect(rc, brush);
	}

	if (!(m_cellR[column->m_ref].properties & PR_IMAGE) || m_cellR[column->m_ref].properties & PR_IMAGETEXT)
	{
		UINT	format;

		setFont(dc, (m_cellR[column->m_ref].properties & PR_BOLD) ? 1 : 0);
		if (m_cellR[column->m_ref].properties & (PR_IMAGETEXT|PR_TRANSPARENT))
			dc->SetBkMode(TRANSPARENT);
		else
			setPaintColor(dc, pRGB);

		format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		if (m_cellR[column->m_ref].properties & PR_IMAGETEXT)
			format |= DT_CENTER;
		else
		{
			switch (m_cellR[column->m_ref].alignC)
			{
			case AL_RIGHT:
				format |= DT_RIGHT;	break;
			case AL_LEFT:
				format |= DT_LEFT;	break;
			case AL_CENTER:
			default :
				format |= DT_CENTER;	break;
			}
		}

		if ((m_form->attr2 & GO_TABLE) && (index <= column->m_enables.GetUpperBound())
			&& !column->m_enables.GetAt(index))
		{
			setTextColor(dc, getRGB(cWHITE));
			dc->DrawText(column->m_edit, rc, format);
			rc.left -= 1;
			rc.top  -= 1;
			setTextColor(dc, getRGB(cXFG));
			dc->SetBkMode(TRANSPARENT);
			dc->DrawText(column->m_edit, rc, format);
		}
		else
		{
			setTextColor(dc, tRGB);
			dc->DrawText(column->m_edit, rc, format);
		}
	}

	if (border)
	{
		if (focus_this && m_mousedown)
			drawEdge(dc, rc, EDGE_SUNKEN, BF_RECT);
		else
			drawEdge(dc, rc, EDGE_RAISED, BF_RECT);
	}

	rc.DeflateRect(3, 3);
	if (focus_this)
	{
		dc->SelectStockObject(NULL_BRUSH);
		dc->SelectStockObject(BLACK_PEN);
		dc->DrawFocusRect(rc);
	}
}

void CfmGrid::drawHead(CDC* dc, int col, CString data, CRect rc, UINT format)
{
	CRect	sRc;
	CSize	size, ssize;
	CString tmps;
	CPoint	pts[3];

	tmps  = data;
	tmps += sASCEND;

	size  = dc->GetOutputTextExtent(tmps);
	ssize = dc->GetOutputTextExtent(sASCEND);
	ssize.cx = ssize.cx * 3 / 4;
	if (rc.Width() > size.cx)
	{
		int	gap, ww;

		sRc = rc;
		if (format & DT_LEFT)
		{
			sRc.left += size.cx;
			sRc.right = sRc.left + ssize.cx;
		}
		else if (format & DT_RIGHT)
			sRc.left  = rc.right - ssize.cx;
		else
		{
			gap = rc.Width() - size.cx;
			gap /= 2;
			sRc.right -= gap;
			sRc.left = sRc.right - ssize.cx;
		}
		rc.right = sRc.left;

		ww = sRc.Width();
		if (ww >= 4)
			ww = (ww/2*2)-1;
		sRc.right = sRc.left + ww;
		gap = sRc.Width()/2+1;
		sRc.top += (sRc.Height()-gap)/2;
		if (m_dir != 1)	// descending
			sRc.top++;
		sRc.bottom = sRc.top + gap-1;

		setPen(dc, getRGB(cEDGE));
		setBrush(dc, getRGB(cEDGE));
		dc->SetPolyFillMode(ALTERNATE);
		switch (m_dir)
		{
		case 1:		// sASCEND ▲
			ww = sRc.Width()/2;
			pts[0].x = sRc.left+ww;		pts[0].y = sRc.top;
			pts[1].x = sRc.left;		pts[1].y = sRc.bottom;
			pts[2].x = sRc.right-1;		pts[2].y = sRc.bottom;
			dc->Polygon(pts, 3);
			break;

		case 2:		// sDESCEND ▼
			ww = sRc.Width()/2;
			pts[0].x = sRc.right-1;		pts[0].y = sRc.top;
			pts[1].x = sRc.left;		pts[1].y = sRc.top;
			pts[2].x = sRc.left+ww;		pts[2].y = sRc.bottom;
			dc->Polygon(pts, 3);
			break;
		}

		tmps = data;
		tmps += _T(" ");
		dc->DrawText(tmps, rc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT|DT_NOPREFIX);
	}
	else
		dc->DrawText(data, rc, format);
}

void CfmGrid::drawPage(CDC* dc, CRect rc, int type, bool up, bool enable)
{
	CPoint	pts[3];
	CBrush*	br;
	Cimage*	image;
	CRect	rect, tRc;
	int	index, gap = (rc.Width() > rc.Height()) ? rc.Height() : rc.Width();

	if (enable)
		index = up ? PI_DEFAULT : PI_DOWN;
	else	index = PI_OFF;

	if (m_scrolls.Lookup(type, image) && image->GetValid(index))
	{
		image->DrawPicture(dc, rc, AL_STRETCH, m_axform->m_oldVers, index);
		return;
	}

	rect = tRc = rc;
	gap /= 5;
	tRc.left   += ((tRc.Width() - (gap*2))/2);
	tRc.right   = tRc.left + gap*2;
	tRc.top    += ((tRc.Height() - (gap*2))/2);
	tRc.bottom  = tRc.top + gap*2;

	br = setBrush(dc, GetSysColor(COLOR_SCROLLBAR));
	dc->FillRect(&rect, br);
	dc->SetPolyFillMode(ALTERNATE);
	switch (type)
	{
	case isHome:		// home : ⊙
		if (enable)
			dc->SelectStockObject(BLACK_PEN);
		else
			setPen(dc, getRGB(cGRAY));
		dc->SelectObject(GetSysColorBrush(COLOR_SCROLLBAR));
		tRc.top--; tRc.left--;
		dc->Ellipse(&tRc);
		break;

	case isPageUp:		// page up
		if (enable)
		{
			dc->SelectStockObject(BLACK_PEN);
			dc->SelectStockObject(BLACK_BRUSH);
		}
		else
		{
			setPen(dc, getRGB(cGRAY));
			setBrush(dc, getRGB(cGRAY));
		}

		rc  = tRc;
		tRc.bottom = tRc.top + rc.Height()/2-1;
		tRc.right = tRc.left + (tRc.Width()/2)*2-1;
		pts[0].y = tRc.top;
		pts[1].y = tRc.bottom;
		pts[2].y = tRc.bottom;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.left;
		pts[2].x = tRc.right-1;
		dc->Polygon(pts, 3);

		tRc.top = tRc.bottom+1;
		tRc.bottom = tRc.top + rc.Height()/2 - 1;
		pts[0].y = tRc.top;
		pts[1].y = tRc.bottom;
		pts[2].y = tRc.bottom;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.left;
		pts[2].x = tRc.right-1;
		dc->Polygon(pts, 3);
		break;

	case isPageDn:		// page down
		if (enable)
		{
			dc->SelectStockObject(BLACK_PEN);
			dc->SelectStockObject(BLACK_BRUSH);
		}
		else
		{
			setPen(dc, getRGB(cGRAY));
			setBrush(dc, getRGB(cGRAY));
		}

		rc = tRc;
		tRc.bottom = tRc.top + rc.Height()/2-1;
		tRc.right = tRc.left + (tRc.Width()/2)*2-1;
		pts[0].y = tRc.bottom;
		pts[1].y = tRc.top;
		pts[2].y = tRc.top;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.right-1;
		pts[2].x = tRc.left;
		dc->Polygon(pts, 3);

		tRc.top = tRc.bottom+1;
		tRc.bottom = tRc.top + rc.Height()/2-1;
		pts[0].y = tRc.bottom;
		pts[1].y = tRc.top;
		pts[2].y = tRc.top;
		pts[0].x = tRc.left + (tRc.Width()/2);
		pts[1].x = tRc.right-1;
		pts[2].x = tRc.left;
		dc->Polygon(pts, 3);
		break;

	default:
		break;
	}

	dc->DrawEdge(rect, up ? EDGE_RAISED : EDGE_SUNKEN, BF_RECT);
}

void CfmGrid::drawHead(CDC* dc, CRect rect)
{
	CPoint	pts[6];
	int	gap = 1;

	pts[0].x = rect.right-gap;	pts[0].y = rect.top;
	pts[1].x = rect.left;		pts[1].y = rect.top;
	pts[2].x = rect.left;		pts[2].y = rect.bottom-gap;
	pts[3].x = rect.left;		pts[3].y = pts[2].y;
	pts[4].x = pts[3].x;		pts[4].y = rect.top;
	pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
	setPen(dc, getRGB(cWHITE));
	setBrush(dc, getRGB(cWHITE));
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);

	pts[0].x = rect.right-gap;	pts[0].y = rect.top;
	pts[1].x = pts[0].x;		pts[1].y = rect.bottom-gap;
	pts[2].x = rect.left;		pts[2].y = pts[1].y;
	pts[3].x = rect.left;		pts[3].y = rect.bottom-gap;
	pts[4].x = rect.right-gap;	pts[4].y = rect.bottom-gap;
	pts[5].x = rect.right-gap;	pts[5].y = rect.top;
	setPen(dc, getRGB(cHEAD));		// 181, 199, 218
	setBrush(dc, getRGB(cHEAD));
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 6);
}

void CfmGrid::drawHead(CDC* dc, int col, CRect rc, CString data)
{
	Ccolumn* column = m_columns.GetAt(col);

	switch (m_cellR[column->m_ref].hkind)
	{
	case CK_COMBO:
		drawCombo(dc, rc, col);
		break;
	case CK_CHECK:
		drawCheck(dc, rc, col, data);
	default:
		break;
	}
}

void CfmGrid::drawMark(CDC* dc, CRect rc)
{
	CPoint	pts[3];
	int	gap = rc.Height() / 4;

	pts[0].x = rc.right-3;		pts[0].y = rc.bottom-3;
	pts[1].x = pts[0].x-gap+1;	pts[1].y = pts[0].y;
	pts[2].x = pts[0].x;		pts[2].y = pts[0].y-gap+1;
	setPen(dc, getRGB(cEDGE));
	setBrush(dc, getRGB(cEDGE));
	dc->SetPolyFillMode(ALTERNATE);
	dc->Polygon(pts, 3);
}

void CfmGrid::drawCombo(CDC* dc, CRect rc, int col)
{
	CRect	tRc;
	CPoint	pts[3];
	CSize	size;
	int	gap, ww;

	tRc = rc;
	size = dc->GetOutputTextExtent(sASCEND);
	size.cx = size.cx * 3 / 4;
	tRc.left  = rc.right - size.cx;

	ww = tRc.Width();
	if (ww >= 4)
		ww = (ww/2*2)-1;
	tRc.right = tRc.left + ww;

	gap = tRc.Width()/2+1;
	tRc.top += ((tRc.Height()-gap)/2+1);
	tRc.bottom = tRc.top + gap-1;
	tRc.OffsetRect(-2, 2);

	setPen(dc, getRGB(cBLACK));
	setBrush(dc, getRGB(cBLACK));
	dc->SetPolyFillMode(ALTERNATE);

	ww = tRc.Width()/2;
	pts[0].x = tRc.right-1;		pts[0].y = tRc.top;
	pts[1].x = tRc.left;		pts[1].y = tRc.top;
	pts[2].x = tRc.left+ww;		pts[2].y = tRc.bottom;
	dc->Polygon(pts, 3);
}

void CfmGrid::drawCheck(CDC* dc, CRect rc, int col, CString data)
{
	CRect	tRc;
	CSize	size;
	UINT	style;
	bool	empty = false;
	int	offset, image = -1;
	CBitmap* bitmap = (CBitmap*) 0;
	Ccolumn* column = m_columns.GetAt(col);

	style = DFCS_BUTTONCHECK;
	if (m_mousedown && m_click == ckHead && m_cellpos.m_col == col+1)
		style |= DFCS_INACTIVE;
	if (!column->m_hcell.IsEmpty() && column->m_hcell.GetAt(0) == '1')
		style |= DFCS_CHECKED;

	data.TrimLeft(); data.TrimRight();
	if (data.IsEmpty())
	{
		empty = true;
		data = "가나다ABW";
	}
	size = dc->GetOutputTextExtent(data);
	tRc = rc;
	tRc.bottom = rc.top  + size.cy;
	if (rc.Height() > tRc.Height())
	{
		int	dx = max(0, (rc.Height() - tRc.Height())/2);
		tRc.top += dx;
		tRc.bottom = min(rc.bottom, tRc.top+size.cy+1);
	}
	tRc.right  = rc.left + tRc.Height();
	if (empty)
	{
		offset = rc.Width() - tRc.Width();
		offset /= 2;
	}
	else
		offset = 3;
	tRc.OffsetRect(max(offset, 0), 0);

	if (style & DFCS_CHECKED)
		image = (style & DFCS_INACTIVE) ? 3 : 1;
	else if (style & DFCS_INACTIVE)
		image = 2;
	else	image = 0;

	if (m_checks.Lookup(image, bitmap))
	{
		CDC	memDC;
		BITMAP	bm = {};

		memDC.CreateCompatibleDC(dc);
		bitmap->GetBitmap(&bm);
		if (!m_axform->m_oldVers)
		{
			CBitmap* pbmp = (CBitmap *)memDC.SelectObject(bitmap);
			::TransparentBlt(dc->m_hDC, tRc.left, tRc.top, tRc.Width(), tRc.Height(),
						memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
			memDC.SelectObject(pbmp);
		}
		else
			transparentBlt(dc, bitmap, tRc.left, tRc.top, tRc.Width(), tRc.Height());
	}
	else	dc->DrawFrameControl(tRc, DFC_BUTTON, style);

}

void CfmGrid::lbutton(bool down, CPoint pt, int& result)
{
	CRect	sRc, tmpRc1, tmpRc2, tmpRc3, cell;
	int	ii,  idx,  oldSIdx,  oldSCol;
	int	tCol, colidx,  validLine = 0;
	float	tWidth = 0;
	int	skipN, rows = m_showRows;
	Ccolumn* col = (Ccolumn*) 0;
	DWORD	attr;
	Cpos	oldPos;

	result = RC_NOP;
	if (m_form->attr2 & (GO_MERGE|GO_XHEAD))
		skipN = m_headN*m_multiN;
	else
		skipN = m_multiN;

	switch (m_move)
	{
	case mvMove:
		if (down)
		{
			m_move  = mvDrag;
			m_point = pt;
			m_delta = m_moveRc.left - pt.x;
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

			CDC* dc  = getDC();
			dc->SelectStockObject(NULL_PEN);
			dc->SelectStockObject(DKGRAY_BRUSH);
			dc->SetROP2(R2_NOT);
			dc->Rectangle(m_moveRc);
			releaseDC(dc);
		}
		return;

	case mvDrag:
		if (!down)
		{
			m_move = mvNone;
			CDC* dc = getDC();
			dc->SelectStockObject(NULL_PEN);
			dc->SelectStockObject(DKGRAY_BRUSH);
			dc->SetROP2(R2_NOT);
			dc->Rectangle(m_moveRc);
			releaseDC(dc);

			m_delta = pt.x - m_point.x;
			if (m_delta > m_pRc.Width())
				break;

			Ccolumn* column = m_columns.GetAt(m_moveCol);
			if (m_delta >= 0 || (m_delta < 0 && abs(m_delta) < column->m_width))
				column->m_width += m_delta;
			else
				column->m_width = 5;
			column->m_widthx = (WORD)(column->m_width / m_hR);

			CRect	tRc = m_pRc;
			bool	hscroll = m_hscroll;
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
					CRect	sRc = m_pRc;

					if (!(m_form->attr2 & GO_X3D))
						sRc.DeflateRect(2, 2);
					m_showRows = (m_totRows + m_headN) * m_multiN;
					if (m_vscroll)
					{
						float	showRows, tHeight = 0;

						if (m_hscroll)
							sRc.bottom -= (int)(szSCROLL * m_vR);
						if (!(m_form->attr2 & GO_XHEAD))
						{
							if (!(m_form->attr2 & GO_X3D))
								sRc.top += (4 * m_multiN);
							else
								sRc.top += m_multiN;
						}

						if ((float)sRc.Height() > ((m_headN * m_multiN) * m_cy))
						{
							showRows = (float)(m_headN * m_multiN);
							if (m_form->attr2 & GO_MERGE)
								tHeight = (float)sRc.Height() - (m_headN*m_multiN)*(m_cy+m_slit);
							else
								tHeight = (float)sRc.Height() - (m_headN*m_multiN)*m_cy;
							showRows += (tHeight / (float)(m_cy + m_slit));
						}
						else
							showRows = (float)sRc.Height() / (float)m_cy;

						m_showRows = (int)showRows;
						if ( (showRows - (double)m_showRows) > 0.5)
							m_showRows++;

						sRc.right -= (int)(szSCROLL * m_hR);

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
			{
				if (m_hscroll != hscroll && m_enableScroll)
					m_vthumb.SetRectEmpty();
				else
					tRc.right = m_vscr.left;
			}
			invalidateRect(&tRc, false);
		}
		return;

	case mvNone:
	default:
		break;
	}

	idx = 0;
	oldSIdx = m_stRow;
	oldSCol = m_stCol;
	oldPos  = m_curpos;
	if (down)
	{
		m_click = ckNone;
		cell.SetRectEmpty();
		m_cell.SetRectEmpty();
		if ((m_form->attr & FA_ENUM) && m_editRc.PtInRect(pt))
		{
			m_click = ckEdit;
			m_state = stFocus;
		}
		else if (m_vscroll && m_enableScroll && m_vscr.PtInRect(pt))
		{
			if ((m_form->attr2 & GO_PAGE) && !m_pscr.IsRectEmpty() && m_pscr.PtInRect(pt))
			{
				tmpRc1 = tmpRc2 = tmpRc3 = m_pscr;
				tmpRc1.bottom = tmpRc1.top + m_pscr.Height()/3;
				tmpRc2.top = tmpRc1.bottom;
				tmpRc2.bottom = tmpRc2.top + m_pscr.Height()/3;
				tmpRc3.top = tmpRc2.bottom;

				if (tmpRc1.PtInRect(pt))
				{
					if (m_stRow <= 0)
					{
						if (!m_writingAll && m_save == svExchange)
						{
							if (!m_useInfo || (m_useInfo && (m_stat & statPREV)))
								result = RC_PAGEUP;	// exFirstPage
							else
								result = RC_UPC;
						}
					}
					else
					{
						keyPrior(result);
						result = RC_UPC;
					}
					m_click = ckPageUp;
				}
				else if (tmpRc2.PtInRect(pt))
				{
					m_click = ckFirst;
					result = RC_FIRST;
				}
				else if (tmpRc3.PtInRect(pt))
				{
					if (m_stRow + m_showRows - (m_headN * m_multiN) >= (m_validRows * m_multiN))
					{
						if (!m_writingAll && m_save == svExchange)
						{
							if (!m_useInfo || (m_useInfo && (m_stat & statNEXT)))
								result = RC_PAGEDN;	// exLastPage
							else
								result = RC_DNC;
						}
					}
					else
					{
						keyNext(result);
						result = RC_DNC;
					}
					m_click = ckPageDown;
				}
			}
			else
			{
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

					m_click = ckScrollUp;
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

					m_click = ckScrollDown;
				}
				else if (m_vthumb.PtInRect(pt))
				{
					m_click  = ckVthumb;
					result   = RC_THUMB;
					m_point = pt;
				}
				else
				{
					if (m_pscr.IsRectEmpty())
						tmpRc1.top = tmpRc1.bottom;
					else
						tmpRc1.top = m_pscr.bottom;
					tmpRc1.bottom = tmpRc1.top + (int)(m_stRow * m_vgap);
					tmpRc2.top    = tmpRc1.bottom  + m_vthumb.Height();
	 				tmpRc2.bottom = m_vscr.bottom - (int)(szSCROLL * m_vR);

					if (tmpRc1.PtInRect(pt))
						m_click = ckRgnUp;
					else if (tmpRc2.PtInRect(pt))
						m_click = ckRgnDown;
				}
			}
		}
		else if ((m_form->attr2 & GO_PAGE) && !m_pscr.IsRectEmpty() && m_pscr.PtInRect(pt))
		{
			float	bH;
			
			bH = (float)m_pscr.Height() / (float)7.0;

			tmpRc1 = tmpRc2 = tmpRc3 = m_pscr;
			tmpRc1.bottom = tmpRc1.top + (int)(bH*3);
			tmpRc2.top = tmpRc1.bottom;
			tmpRc2.bottom = tmpRc2.top + (int)bH;
			tmpRc3.top = tmpRc2.bottom;

			if (tmpRc1.PtInRect(pt))
			{
				if (!m_writingAll && m_save == svExchange)
				{
					if (!m_useInfo || (m_useInfo && (m_stat & statPREV)))
					{
						result  = RC_PAGEUP;	// exFirstPage
						m_click = ckPageUp;
					}
				}
			}
			else if (tmpRc2.PtInRect(pt))
			{
				m_click = ckFirst;
				result  = RC_FIRST;
			}
			else if (tmpRc3.PtInRect(pt))
			{
				if (!m_writingAll && m_save == svExchange)
				{
					if (!m_useInfo || (m_useInfo && (m_stat & statNEXT)))
					{
						result  = RC_PAGEDN;	// exLastPage
						m_click = ckPageDown;
					}
				}
			}
		}
		else if (m_hscroll && m_hscr.PtInRect(pt))
		{
			tmpRc1 = tmpRc2 = m_hscr;
			if (m_hscr.Width() <= (int)(2 * szSCROLL * m_hR))
         		{
          			tmpRc1.right = tmpRc1.left + (m_hscr.Width() / 2);
                 		tmpRc2.left  = tmpRc2.left + (m_hscr.Width() / 2 + 1);
             		}
             		else
             		{
                 		tmpRc1.right = tmpRc1.left  + (int)(szSCROLL * m_hR);
                 		tmpRc2.left  = tmpRc2.right - (int)(szSCROLL * m_hR);
             		}

			if (tmpRc1.PtInRect(pt))
			{
				m_click = ckScrollLeft;
				result  = RC_LEFT;
			}
			else if (tmpRc2.PtInRect(pt))
			{
				m_click = ckScrollRight;
				result  = RC_RIGHT;
			}
			else if (m_hthumb.PtInRect(pt))
			{
				m_click  = ckHthumb;
				result   = RC_THUMB;
				m_point = pt;
			}
			else
			{
				tmpRc1.left  = tmpRc1.right;
				tmpRc1.right = tmpRc1.left + (int)(m_scrolledWidth * m_hgap);
				if (tmpRc1.PtInRect(pt))
					m_click = ckRgnLeft;
				else
				{
					tmpRc1.left  = tmpRc1.right  + (int)(m_hgap * (m_showWidth - m_fixedWidth));
					tmpRc1.right = m_hscr.right - (int)(szSCROLL * m_hR);
					if (tmpRc1.PtInRect(pt))
						m_click = ckRgnRight;
				}
			}
		}
		else if (m_pRc.PtInRect(pt))
		{
			tmpRc1 = m_pRc;
			if (!m_vscr.IsRectEmpty())
				tmpRc1.right = m_vscr.left;
			tmpRc1.bottom = tmpRc1.top + (int)(m_cy * m_headN * m_multiN);
			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					tmpRc1.bottom += (4 * m_multiN);	// head extra space
				else
					tmpRc1.bottom += m_multiN;		// head extra space
			}
			if (tmpRc1.PtInRect(pt) && m_headN != 0)
			{
				m_click = ckHead;
				result  = RC_HEAD;
			}
			else
			{
				tmpRc1.top = tmpRc1.bottom;
				tmpRc1.bottom = m_pRc.bottom;
				if (!(m_form->attr2 & GO_X3D))
					tmpRc1.bottom -= 2;
				if (m_hscroll && !m_hscr.IsRectEmpty())
					tmpRc1.bottom = m_hscr.top;
				if (tmpRc1.PtInRect(pt))
				{
					m_click = ckItem;
					result  = RC_ITEM;
				}
			}
		}

		idx = 0;
		switch (m_click)
		{
		case ckHead:
			for (ii = 0; ii < skipN; ii++)
			{
				tmpRc1 = m_pRc;
				tmpRc1.top    = m_pRc.top  + (int)(m_cy*m_headN * ii);
				tmpRc1.bottom = tmpRc1.top + (int)(m_cy*m_headN);
				if (!(m_form->attr2 & GO_XHEAD))
				{
					if (!(m_form->attr2 & GO_X3D))
						tmpRc1.bottom += (4 * m_multiN);	// head extra space
					else
						tmpRc1.bottom += m_multiN;		// head extra space
				}
				if (tmpRc1.PtInRect(pt))
				{
					idx = ii;
					break;
				}
			}
			colidx = idx % m_multiN;
			tCol   = m_form->type ? 0 : m_stCol;
			if (m_form->attr2 & GO_MULTI)
			{
				for (ii = 0; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_line == colidx)
					{
						tCol = ii;
						break;
					}
				}
			}
			tWidth = 0;
			sRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				sRc.DeflateRect(2, 2);
			for (ii = tCol; ii < m_totCols; ii++)
			{
				if (m_form->type && ii == m_form->type)
					ii = m_stCol;

				col = m_columns.GetAt(ii);
				if (!col->m_visible)
					continue;

				if (col->m_line == colidx)
				{
					tmpRc1.left = sRc.left + (int)tWidth;
					tWidth += col->m_width;
					if ((int)tWidth < sRc.Width())
						tmpRc1.right = sRc.left + (int)tWidth;
					else
						tmpRc1.right = sRc.right;

					if (tmpRc1.PtInRect(pt))
					{
						m_cellpos.m_col = ii;
						break;
					}
				}
				else
				{
					m_cellpos.m_col = -1;
					break;
				}
			}
			if (ii >= m_totCols)
				m_cellpos.m_col = -1;

			m_cellpos.m_col++;
			if (m_cellpos.m_col > 0)
			{
				col = m_columns.GetAt(m_cellpos.m_col-1);
				if (m_cellR[col->m_ref].properties & PR_SORT)
				{
					if (m_stat & statSORT)
					{
						result = RC_SEND;
						if (m_sortc != col->m_ref)
						{
							m_dir   = 0;
							m_sortc = col->m_ref;
						}
					}
					else
					{
						if (m_sortc == col->m_ref)
							m_dir = (m_dir == 2) ? 1 : 2;
						else
						{
							m_dir   = 2;
							m_sortc = col->m_ref;
						}

						Sort(col->m_ref+1, (m_dir == 1) ? true : false,
							(m_cellR[col->m_ref].type == IO_NUM || m_cellR[col->m_ref].type == IO_DIGIT) ? false : true);
					}
				}

				if (m_cellR[col->m_ref].hcursor == CS_SORTd)
					m_axform->DoSomething(doCURSOR, this, (m_dir == 1) ? CS_SORTa : CS_SORTd);
				else if (m_cellR[col->m_ref].hcursor >= CS_CURSOR)
					m_axform->DoSomething(doCURSOR, this, m_cellR[col->m_ref].hcursor);
			}
			else
				result = RC_OUT;
			break;

		case ckItem:
			idx = m_index;
			rows = min((m_validRows+m_headN)*m_multiN-1, m_showRows);

			m_index = -1;
			for (ii = m_headN*m_multiN; ii < rows + 1; ii++)
			{
				tmpRc1 = m_pRc;
				if (!(m_form->attr2 & GO_XHEAD))
				{
					if (!(m_form->attr2 & GO_X3D))
						tmpRc1.top += (4 * m_multiN);		// head extra space
					else
						tmpRc1.top += m_multiN;			// head extra space
				}
				tmpRc1.top   += (int)((m_cy + m_slit) * ii);
				tmpRc1.bottom = tmpRc1.top + (int)m_cy;

				if (tmpRc1.PtInRect(pt))
				{
					m_index = m_stRow + ii - (m_headN * m_multiN);
					if (m_index >= (m_totRows * m_multiN))
						m_index = m_totRows * m_multiN - 1;
					cell = tmpRc1;
					break;
				}
			}

			if (m_index == -1)
			{
				m_index = idx;
				result  = RC_OUT;
			}
			else if (m_form->attr2 & GO_ALWAYS)
			{
				sRc = m_pRc;
				if (!(m_form->attr2 & GO_X3D))
				{
					sRc.DeflateRect(2, 2);
					if (!(m_form->attr2 & GO_XHEAD))
						sRc.top += (4 * m_multiN);		// head extra space
				}
				else if (!(m_form->attr2 & GO_XHEAD))
					sRc.top += m_multiN;				// head extra space
				sRc.top   += (int)((m_cy+m_slit) * (m_headN * m_multiN));
				if (m_form->isEdit == FI_RREC)
				{
					sRc.top   += (int)((m_cy+m_slit) * (idx/m_multiN*m_multiN - m_stRow));
					sRc.bottom = sRc.top + (int)((m_cy+m_slit)*m_multiN) + 3;
				}
				else
				{
					sRc.top   += (int)((m_cy+m_slit) * (idx - m_stRow));
					sRc.bottom = sRc.top + (int)m_cy + 3;
				}
				if (m_vscroll)
					sRc.right = m_vscr.left;
				sRc.InflateRect(0, 4);
				if (m_hscroll)
					sRc.bottom = min(sRc.bottom, m_hscr.top);
				invalidateRect(&sRc, false);
			}

			m_cellpos.m_row = (m_index / m_multiN);
			m_cellpos.m_row++;
			colidx = m_index % m_multiN;
			tCol = m_form->type ? 0 : m_stCol;
			if (m_form->attr2 & GO_MULTI)
			{
				for (ii = 0; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_line == colidx)
					{
						tCol = ii;
						break;
					}
				}
			}
			tWidth = 0;
			sRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				sRc.DeflateRect(2, 2);
			for (ii = tCol; ii < m_totCols; ii++)
			{
				if (m_form->type && ii == m_form->type)
					ii = m_stCol;

				col = m_columns.GetAt(ii);
				if (!col->m_visible)
					continue;

				if (col->m_line == colidx)
				{
					tmpRc1.left = sRc.left + (int)tWidth;
					tWidth += col->m_width;
					if ((int)tWidth < sRc.Width())
						tmpRc1.right = sRc.left + (int)tWidth;
					else
						tmpRc1.right = sRc.right;

					if (tmpRc1.PtInRect(pt))
					{
						cell.IntersectRect(cell, tmpRc1);
						m_cellpos.m_col = ii;
						break;
					}
				}
				else
				{
					m_cellpos.m_col = -1;
					break;
				}
			}

			if (ii >= m_totCols)
				m_cellpos.m_col = -1;
			
			switch (m_state)
			{
			case stFocus:
			case stEdit:
			case stCombo:
			case stCheck:
			case stButton:
				if (m_cellpos.m_col < 0)
					break;

				col  = m_columns.GetAt(m_cellpos.m_col);
				attr = (m_form->attr2 & GO_TABLE) ? col->m_attrs.GetAt(m_cellpos.m_row-1) : m_cellR[col->m_ref].attr;
				if ((m_form->attr2 & GO_TABLE) && (m_cellpos.m_row-1 <= col->m_enables.GetUpperBound())
					&& !col->m_enables.GetAt(m_cellpos.m_row-1))	// disable
				{
					m_state = stFocus;
					break;
				}

				if (!(attr & FA_PROTECT) && col->m_visible
					&& (m_cellR[col->m_ref].iok == EIO_INPUT || m_cellR[col->m_ref].iok == EIO_INOUT
						|| m_cellR[col->m_ref].kind == CK_BUTTON || m_cellR[col->m_ref].kind == CK_CHECK))
				{
					switch (m_cellR[col->m_ref].kind)
					{
					case CK_COMBO:
						m_state = stCombo;	break;
					case CK_CHECK:
						m_state = stCheck;	break;
					case CK_BUTTON:
						m_state = stButton;	break;
					default:
						m_state   = stEdit;
						m_edstate = esNone;	break;
					}
					m_cell = cell;
				}
				else
					m_state = stFocus;
				break;
			default:
				break;
			}

			m_cellpos.m_col++;
			if (col && m_cellpos.m_col > 0)
			{
				m_curpos = m_lastpos = m_cellpos;
				changeIME(m_curpos.m_row-1, m_curpos.m_col-1);
				if ((m_cellR[col->m_ref].iok == EIO_INPUT || m_cellR[col->m_ref].iok == EIO_INOUT)
						&& m_cellR[col->m_ref].kind == CK_NORMAL)
					m_axform->DoSomething(doCURSOR, this, CS_IBEAM);
				else if (m_cellR[col->m_ref].cursor >= CS_CURSOR)
					m_axform->DoSomething(doCURSOR, this, m_cellR[col->m_ref].cursor);
			}
			else
			{
				m_index = idx;
				result  = RC_OUT;
				m_cellpos.m_row = -1;
			}
 			break;

		case ckRgnUp:
			m_stRow = max(0, m_stRow - (m_showRows-(m_multiN*m_headN)));
			break;

		case ckRgnDown:
			m_stRow += (m_showRows - (m_multiN * m_headN));
			m_stRow  = max(0, min(m_stRow, m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN))));
			break;

		case ckScrollLeft:
			if (m_scrolledWidth > 0 && m_stCol > m_form->type)
			{
				while (true)
				{
					m_stCol--;
					if (m_columns.GetAt(m_stCol)->m_visible || m_stCol <= m_form->type)
						break;
				}
			}
			m_stCol = max(m_stCol, m_form->type);
			break;

		case ckScrollRight:
			tCol = m_totCols;
			if (m_form->attr2 & GO_MULTI)
			{
				for (ii = 0; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_line > 0)
					{
						tCol = ii;
						break;
					}
				}
			}

			tWidth = 0;
			for (ii = tCol-1; ii >= oldSCol; ii--)
			{
				if (m_columns.GetAt(ii)->m_visible)
					tWidth += m_columns.GetAt(ii)->m_width;
			}

			if (tWidth > m_showWidth - m_fixedWidth)
			{
				while (true)
				{
					if (m_stCol >= tCol-1)
						break;
					m_stCol++;
					if (m_columns.GetAt(m_stCol)->m_visible)
						break;
				}
			}
			m_stCol = min(m_stCol, tCol-1);
			break;

		case ckRgnLeft:
			if (m_scrolledWidth > 0)
			{
				while (true)
				{
					m_stCol--;
					if (m_columns.GetAt(m_stCol)->m_visible || m_stCol <= m_form->type)
						break;
				}
				tWidth = 0;
				for (ii = m_form->type; ii < m_stCol; ii++)
				{
					if (m_columns.GetAt(ii)->m_visible)
						tWidth += m_columns.GetAt(ii)->m_width;
				}

				if (tWidth > 0)
				{
					while (true)
					{
						m_stCol--;
						if (m_columns.GetAt(m_stCol)->m_visible || m_stCol <= m_form->type)
							break;
					}
				}
			}
			m_stCol = max(m_stCol, m_form->type);
			break;

		case ckRgnRight:
			tCol = m_totCols;
			if (m_form->attr2 & GO_MULTI)
			{
				for (ii = 0; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_line > 0)
					{
						tCol = ii;
						break;
					}
				}
			}

			tWidth = 0;
			for (ii = tCol-1; ii >= oldSCol; ii--)
			{
				if (m_columns.GetAt(ii)->m_visible)
					tWidth += m_columns.GetAt(ii)->m_width;
			}

			if (tWidth > m_showWidth - m_fixedWidth)
			{
				while (true)
				{
					if (m_stCol >= tCol-1)
						break;
	 				m_stCol++;
					if (m_columns.GetAt(m_stCol)->m_visible)
						break;
				}

				tWidth = 0;
				for (ii = tCol-1; ii >= m_stCol; ii--)
				{
					if (m_columns.GetAt(ii)->m_visible)
						tWidth += m_columns.GetAt(ii)->m_width;
				}

				if (tWidth > m_showWidth - m_fixedWidth)
				{
					while (true)
					{
						if (m_stCol >= tCol-1)
							break;
						m_stCol++;
						if (m_columns.GetAt(m_stCol)->m_visible)
							break;
					}
				}
			}
			m_stCol = min(m_stCol, tCol-1);
			break;

		default:
			break;
		}
	}

	m_scrolledWidth = 0;
	m_stCol = max(m_form->type, m_stCol);
	if (m_form->type == m_totCols)
		m_stCol = 0;
	for (ii = m_form->type; ii < m_stCol; ii++)
	{
		if (m_columns.GetAt(ii)->m_visible)
			m_scrolledWidth += m_columns.GetAt(ii)->m_width;
	}

	switch (m_click)
	{
	case ckHead:
		if (m_cellpos.m_col > 0)
		{
			col = m_columns.GetAt(m_cellpos.m_col-1);
			tmpRc1 = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				tmpRc1.DeflateRect(2, 2);
			tmpRc1.bottom = tmpRc1.top + (int)(m_cy * m_multiN * m_headN);
			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					tmpRc1.bottom += (4 * m_multiN);		// head extra space
				else
					tmpRc1.bottom += m_multiN;			// head extra space
			}
			getRect(0, m_cellpos.m_col-1, tmpRc2, true);
			tmpRc1.left = tmpRc2.left;
			tmpRc1.right = tmpRc2.right;

			switch (m_cellR[col->m_ref].hkind)
			{
			case CK_CHECK:
				if (!down && tmpRc1.PtInRect(pt))
				{
					if (col->m_hcell == "1")
						col->m_hcell = _T("0");
					else	col->m_hcell = _T("1");
				}
				invalidateRect(&tmpRc1, false);
				break;

			case CK_COMBO:
				if (!down && tmpRc1.PtInRect(pt))
				{
					tmpRc1.InflateRect(1, 0);
					makePopup(m_cellpos.m_col-1, CPoint(tmpRc1.right, tmpRc1.bottom));
				}
				break;
			}
		}
		break;

	case ckItem:
		if (!m_focus || !down)	break;

		tmpRc1 = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
		{
			tmpRc1.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tmpRc1.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tmpRc1.top += m_multiN;			// head extra space

		tmpRc1.top += (int)((m_cy+m_slit) * (m_headN * m_multiN));
		if (m_form->isEdit == FI_RREC)
		{
			tmpRc1.top   += (int)((m_cy+m_slit) * (m_index/m_multiN*m_multiN - m_stRow));
			tmpRc1.bottom = tmpRc1.top + (int)((m_cy+m_slit)*m_multiN);
		}
		else
		{
			tmpRc1.top   += (int)((m_cy+m_slit) * (m_index - m_stRow));
			tmpRc1.bottom = tmpRc1.top + (int)m_cy+m_slit;
		}
		if (m_vscroll)
			tmpRc1.right = m_vscr.left;
		if (m_form->attr2 & GO_PAGE)
			tmpRc1.right = m_pscr.left;
		if (m_hscroll)
			tmpRc1.bottom = min(tmpRc1.bottom, m_hscr.top);
		tmpRc1.InflateRect(1, 4);
		invalidateRect(&tmpRc1, false);

		tmpRc2 = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
		{
			tmpRc2.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tmpRc2.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tmpRc2.top += m_multiN;			// head extra space

		tmpRc2.top += (int)((m_cy+m_slit) * (m_headN * m_multiN));
		if (m_form->isEdit == FI_RREC)
		{
			tmpRc2.top   += (int)((m_cy+m_slit) * (idx/m_multiN*m_multiN - m_stRow));
			tmpRc2.bottom = tmpRc2.top + (int)((m_cy+m_slit)*m_multiN) + 5;
		}
		else
		{
			tmpRc2.top   += (int)((m_cy+m_slit) * (idx - m_stRow));
			tmpRc2.bottom = tmpRc2.top + (int)m_cy+m_slit + 3;
		}
		if (m_vscroll)
			tmpRc2.right = m_vscr.left;
		if (m_form->attr2 & GO_PAGE)
			tmpRc2.right = m_pscr.left;
		if (m_hscroll)
			tmpRc2.bottom = min(tmpRc2.bottom, m_hscr.top);
		tmpRc2.InflateRect(1, 4);
		invalidateRect(&tmpRc2, false);
		break;

	case ckScrollUp:
		if (oldSIdx != m_stRow)
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
			tmpRc1.top  += (int)((m_cy + m_slit) * m_headN * m_multiN);
			tmpRc1.right = m_vscr.left;
			if (m_hscroll)
				tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);
			tmpRc2 = m_vscr;
			tmpRc2.bottom -= (int)(szSCROLL * m_vR);
		}
		else
		{
			tmpRc2 = m_vscr;
			tmpRc2.bottom = tmpRc2.top + (int)(szSCROLL * m_vR);
		}
		m_vthumb.SetRectEmpty();
		invalidateRect(&tmpRc2, false);
		break;

	case ckRgnUp:
		m_click = ckNone;
		if (oldSIdx != m_stRow)
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
			tmpRc1.top  += (int)((m_cy + m_slit) * m_headN * m_multiN);
			tmpRc1.right = m_vscr.left;
			if (m_hscroll)
				tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);
		}

		tmpRc2 = m_vscr;
		tmpRc2.top    += (int)(szSCROLL * m_vR);
		tmpRc2.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tmpRc2, false);
		break;

	case ckRgnDown:
		m_click = ckNone;
		if (oldSIdx != m_stRow)
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
			tmpRc1.top  += (int)((m_cy + m_slit) * m_headN * m_multiN);
			tmpRc1.right = m_vscr.left;
			if (m_hscroll)
				tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);
		}

		tmpRc2 = m_vscr;
		tmpRc2.top    += (int)(szSCROLL * m_vR);
		tmpRc2.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tmpRc2, false);
		break;

	case ckScrollDown:
		if (oldSIdx != m_stRow)
		{
			tmpRc1 = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				tmpRc1.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					tmpRc1.top += (4 * m_multiN);	//head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				tmpRc1.top += m_multiN;			//head extra space
			tmpRc1.top  += (int)((m_cy + m_slit) * m_headN * m_multiN);
			tmpRc1.right = m_vscr.left;
			if (m_hscroll)
				tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);
			tmpRc2 = m_vscr;
			tmpRc2.top += (int)(szSCROLL * m_vR);
		}
		else
		{
			tmpRc2 = m_vscr;
			tmpRc2.top = tmpRc2.bottom - (int)(szSCROLL * m_vR);
		}
		m_vthumb.SetRectEmpty();
		invalidateRect(&tmpRc2, false);
		break;

	case ckFirst:
		tmpRc1 = m_pscr;
		if (m_vscroll)
		{
			tmpRc1.top += (m_pscr.Height()/3);
			tmpRc1.bottom = tmpRc1.top + (m_pscr.Height()/3);
		}
		else if (m_form->attr2 & GO_PAGE)
		{
			float	bH = (float)tmpRc1.Height() / (float)7.0;
			tmpRc1.top += (int)(bH*3);
			tmpRc1.bottom = tmpRc1.top + (int)bH;
		}
		invalidateRect(&tmpRc1, false);
		break;

	case ckPageUp:
		tmpRc1 = m_pscr;
		if (m_vscroll)
			tmpRc1.bottom = tmpRc1.top + (m_pscr.Height()/3);
		else if (m_form->attr2 & GO_PAGE)
		{
			float	bH = (float)tmpRc1.Height() / (float)7.0;
			tmpRc1.bottom = tmpRc1.top + (int)(bH*3);
		}
		invalidateRect(&tmpRc1, false);
		break;

	case ckPageDown:
		tmpRc1 = m_pscr;
		if (m_vscroll)
		{
			tmpRc1.top += (m_pscr.Height()/3*2);
			tmpRc1.bottom = tmpRc1.top + (m_pscr.Height()/3);
		}
		else if (m_form->attr2 & GO_PAGE)
		{
			float	bH = (float)tmpRc1.Height() / (float)7.0;
			tmpRc1.top += (int)(bH*4);
		}
		invalidateRect(&tmpRc1, false);
		break;

	case ckScrollLeft:
		if (oldSCol != m_stCol)
		{
			tmpRc1 = m_pRc;
			if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
				m_vthumb.SetRectEmpty();
			else
			{
				if (m_vscroll)
					tmpRc1.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tmpRc1.right = m_pscr.left;
			}
			tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);

			tmpRc2 = m_hscr;
			tmpRc2.right -= (int)(szSCROLL * m_hR);
		}
		else
		{
			tmpRc2 = m_hscr;
			tmpRc2.right = tmpRc2.left + (int)(szSCROLL * m_hR);
		}
		m_hthumb.SetRectEmpty();
		invalidateRect(&tmpRc2, false);
		break;

	case ckRgnLeft:
	case ckRgnRight:
		m_click = ckNone;
		if (oldSCol != m_stCol)
		{
			tmpRc1 = m_pRc;
			if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
				m_vthumb.SetRectEmpty();
			else
			{
				if (m_vscroll)
					tmpRc1.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tmpRc1.right = m_pscr.left;
			}
			tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);
		}

		tmpRc2 = m_hscr;
		tmpRc2.left  += (int)(szSCROLL * m_hR);
		tmpRc2.right -= (int)(szSCROLL * m_hR);
		m_hthumb.SetRectEmpty();
		invalidateRect(&tmpRc2, false);
		break;

	case ckScrollRight:
		if (oldSCol != m_stCol)
		{
			tmpRc1 = m_pRc;
			if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
				m_vthumb.SetRectEmpty();
			else
			{
				if (m_vscroll)
					tmpRc1.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tmpRc1.right = m_pscr.left;
			}
			tmpRc1.bottom = m_hscr.top;
			invalidateRect(&tmpRc1, false);

			tmpRc2 = m_hscr;
			tmpRc2.left += (int)(szSCROLL  * m_hR);
		}
		else
		{
			tmpRc2 = m_hscr;
			tmpRc2.left = tmpRc2.right - (int)(szSCROLL * m_hR);
		}
		m_hthumb.SetRectEmpty();
		invalidateRect(&tmpRc2, false);
		break;

	case ckHthumb:
		if (down)
			break;

		if (pt.x != m_point.x)
		{
// updateXXX_2022XX	
			if (CfmBase::IsPointInRect(pt) == P_OUT)
				break;

			int	size;
			CRect	tRc1,  tRc2,  iRc1,  iRc2;
			idx = m_stCol;

			size = m_hthumb.Width();
			iRc1 = m_hthumb;
			iRc2 = m_hthumb;
			tRc1 = tRc2 = m_hscr;
			tRc1.right = tRc1.left  + (int)(szSCROLL * m_hR);
			tRc2.left  = tRc2.right - (int)(szSCROLL * m_hR);
			if (m_hthumb.left < tRc1.right || m_hthumb.right > tRc2.left)
				break;

			m_hthumb.left += (pt.x - m_point.x);
			m_hthumb.right = m_hthumb.left + size;

			tCol = m_totCols;
			if (m_form->attr2 & GO_MULTI)
			{
				for (ii = 0; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_line > 0)
					{
						tCol = ii;
						break;
					}
				}
			}

			if (m_hthumb.left <= tRc1.right)
			{
				for (ii = m_form->type; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_visible)
					{
						m_stCol = ii;
						break;
					}
				}
				m_hthumb.left  = tRc1.right;
				m_hthumb.right = m_hthumb.left + size;
			}
			else if (m_hthumb.right >= tRc2.left)
			{
				CRect	tRc = m_pRc;
				float	width = 0;
			
				if (!(m_form->attr2 & GO_X3D))
					tRc.DeflateRect(2, 2);
				if (m_vscroll)
					tRc.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tRc.right = m_pscr.left;
				for (ii = tCol-1; ii >= m_form->type; ii--)
				{
					if (!m_columns.GetAt(ii)->m_visible)
						continue;

					width += m_columns.GetAt(ii)->m_width;
					if (width > tRc.Width() - m_fixedWidth)
						break;
					m_stCol = ii;
				}
				m_hthumb.right = tRc2.left;
				m_hthumb.left  = m_hthumb.right - size;
			}
			else
			{
				float	width, dx;

				size  = tRc2.left - tRc1.right;
				dx    = (float)m_hthumb.left - (float)tRc1.right;
				width = (m_totWidth * dx) / size;
				m_scrolledWidth = dx = 0;
				for (ii = m_form->type; ii < tCol; ii++)
				{
					if (!m_columns.GetAt(ii)->m_visible)
						continue;

					dx += m_columns.GetAt(ii)->m_width;
					m_stCol = ii;
					if (dx > width)
						break;
					m_scrolledWidth = dx;
					if (dx == width)
						break;
				}
				m_hthumb.left = tRc1.right + (int)(m_scrolledWidth * m_hgap);
				if (m_totWidth == m_showWidth - m_fixedWidth + m_scrolledWidth)
					m_hthumb.right = tRc2.left;
				else
					m_hthumb.right = m_hthumb.left + (int)(m_hgap * (m_showWidth - m_fixedWidth));
				m_hthumb.right = min(tRc2.left, m_hthumb.right);
			}
			m_stCol = min(tCol-1, max(m_stCol, m_form->type));
			if (m_form->type == m_totCols)
				m_stCol = 0;
			m_point = pt;
			m_scrolledWidth = 0;
			for (ii = m_form->type; ii < m_stCol; ii++)
			{
				if (m_columns.GetAt(ii)->m_visible)
					m_scrolledWidth += m_columns.GetAt(ii)->m_width;
			}

			iRc1.right = m_hthumb.left;
			iRc1.NormalizeRect();
			iRc2.left  = iRc2.right;
			iRc2.right = m_hthumb.right;
			iRc2.NormalizeRect();
			
			if (idx != m_stCol)
			{
				tRc2 = m_pRc;
				if (m_axform->IsResizable() && (m_form->attr2 & GO_FIX))
					m_vthumb.SetRectEmpty();
				else
				{
					if (m_vscroll)
						tRc2.right = m_vscr.left;
					if (m_form->attr2 & GO_PAGE)
						tRc2.right = m_pscr.left;
				}
				tRc2.bottom = min(tRc2.bottom, m_hscr.top);
				invalidateRect(&tRc2, false);
			}
			if (iRc1.left != iRc1.right)
			{
				iRc1.right += 2;
				invalidateRect(&iRc1, false);
			}
			if (iRc2.left != iRc2.right)
			{
				iRc2.left -= 2;
				invalidateRect(&iRc2, false);
			}
		}
		break;

	case ckEdit:
		if (!down)
			m_axform->DoSomething(doMODAL, this);
		invalidateRect(&m_editRc, false);
		break;

	case ckNone:
	default :
		break;
	}

	if (!down)
	{
		m_point = CPoint(0, 0);
		m_click  = ckNone;
	}
}

bool CfmGrid::lbuttonEdit(bool down, CPoint pt, int& result)
{
	if (!m_cell.PtInRect(pt))
	{
		m_state   = stFocus;
		m_edstate = esNone;
		m_axform->m_view->HideCaret();
		return false;
	}
	if (m_validRows <= 0)
		return false;

	CRect	tRc = m_cell;
	tRc.InflateRect(2, 4);
	if (down || (!down && m_statechange))
	{
		CString	tmps;

		if (down)
			result = RC_ITEM;

		switch (m_edstate)
		{
		case esNone:
			if (m_curpos.m_col < 1 || m_curpos.m_row < 1)
				break;

			tmps = m_data.GetAt((m_curpos.m_row-1)*m_totCols + m_columns.GetAt(m_curpos.m_col-1)->m_ref);
			tmps.Trim();
			m_edstate    = tmps.IsEmpty() ? esClick : esReverse;
			m_edfocusing = false;
			m_edcaret    = true;
			m_edcaretpos = tmps.GetLength();
			invalidateRect(&tRc, false);
			break;

		case esReverse:
			if (m_cellR[m_columns.GetAt(m_curpos.m_col-1)->m_ref].attr & FA_COMMA)
			{
				m_edstate = esNormal;
				return true;
			}
		case esNormal:
			if (down)
			{
				m_edclick = pt;
				m_edstate = esClick;
			}
			else
			{
				m_edfocusing = false;
				invalidateRect(&tRc, false);
			}
			break;

		default:
			break;
		}
	}
	else
	{
		switch (m_edstate)
		{
		case esReverse:
			if (m_cellR[m_columns.GetAt(m_curpos.m_col-1)->m_ref].attr & FA_COMMA)
				break;
			invalidateRect(&tRc, false);
			break;
		case esNormal:
		case esClick:
			m_edfocusing = false;
			invalidateRect(&tRc, false);
			break;
		default:
			break;
		}
	}

	return true;
}

bool CfmGrid::lbuttonCombo(bool down, CPoint pt, int& result)
{
	CRect	tmpRect;
//	CRect	tmpRc1,  tmpRc2;
	Ccolumn* column = m_columns.GetAt(m_curpos.m_col-1);

	if (!down && !m_focus)
	{
		m_click = ckNone;
		DoListBox(false);
		m_cbspread = false;
		invalidateRect(&m_cell, false);
		return false;
	}

	if (m_validRows <= 0)
		return false;

	switch (m_click)
	{
	case ckCombo:
	case ckOrigin:
		if (m_cell.IsRectEmpty())
			getRect(m_curpos.m_row-1, m_curpos.m_col-1, m_cell);
		if (makeListBox(m_curpos.m_row-1, m_curpos.m_col-1, m_cell))
		{
			m_cbspread = true;
			result = RC_CBSPREAD;
		}
	default:
		break;
	}

	if (down || (!down && m_statechange))
	{
		if (down)
			result = RC_ITEM;

		if (m_cbbtn.IsRectEmpty())
		{
			m_cbbtn = m_cell;
			m_cbbtn.DeflateRect(2, 2);
			m_cbbtn.left = m_cbbtn.right - (int)(szSCROLL*m_hR);
		}

		if (m_cbbtn.PtInRect(pt))
			m_click = ckCombo;
		else if (m_cell.PtInRect(pt))
			m_click = ckOrigin;
		else
		{
			m_click = ckNone;
			DoListBox(false);
			m_cbspread = false;
			m_cbbtn.SetRectEmpty();
		}
	}

	switch (m_click)
	{
	case ckOrigin:
		if (down && m_cbspread)
		{
			DoListBox(false);
			m_cbspread = false;
			m_click = ckItem;	// ckNone;
			result  = RC_CBCLOSE;
		}
		if (!down && !m_statechange)
			break;

		tmpRect = m_cell;
		tmpRect.DeflateRect(1, 1);
		invalidateRect(&tmpRect, false);
		break;

	case ckCombo:
		if (!(down || (!down && m_statechange)))
		{
			if (!m_cbspread)
				result = RC_CBCLOSE;
		}
		else if (down && m_cbspread)
		{
			DoListBox(false);
			m_cbspread = false;
			m_click = ckItem;	// ckNone;
			result  = RC_CBCLOSE;
		}
		tmpRect = m_cell;
		tmpRect.DeflateRect(1, 1);
		invalidateRect(&tmpRect, false);
		break;

	case ckNone:
	default:
		m_click = ckNone;
		if (!down && !m_statechange)
			break;
		if (m_cbspread)
		{
			DoListBox(false);
			invalidateRect(&m_cell, false);
		}
		result = RC_NOP;
		return false;
	}

	if (!down && !m_statechange)
	{
		m_point = CPoint(0, 0);
		m_click = ckNone;
	}

	if (result != RC_CHANGED && result != RC_ITEM)
		result = RC_NOP;
	return true;
}

bool CfmGrid::lbuttonCheck(bool down, CPoint pt, int& result)
{
	CRect	tRc, cell(0,0,0,0);// = m_cell;
	int	col = m_columns.GetAt(m_curpos.m_col-1)->m_ref;

	result = RC_NOP;
	if (m_validRows <= 0)
		return false;
	if (!getRect(m_curpos.m_row-1, m_curpos.m_col-1, cell))
		return false;
	if (down)
	{
		if (cell.PtInRect(pt))
			result = RC_ITEM;
		else
			return false;
	}
	else if (!down)// || (!down && m_statechange))
	{
		if (!cell.PtInRect(pt))
			return false;

		CString	data = m_data.GetAt((m_curpos.m_row-1)*m_totCols +col);
		if (!data.Compare("1"))
			data = _T("0");
		else
			data = _T("1");

		m_data.SetAt((m_curpos.m_row-1)*m_totCols+col, data);
		m_edit.SetAt((m_curpos.m_row-1)*m_totCols+col, data);
		if (m_cellR[col].properties & PR_CHECKED)
		{
			for (int ii = 0; ii < m_totRows; ii++)
			{
				if (ii == m_curpos.m_row-1)
					continue;
				m_data.SetAt(ii*m_totCols+col, _T("0"));
				m_edit.SetAt(ii*m_totCols+col, _T("0"));
			}
		}

	//	result = RC_CHANGED;
	//	m_changed    = true;
	//	m_changedpos = m_curpos;
	}
	tRc = cell;

	if (m_cellR[col].properties & PR_CHECKED)
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
		tRc.left  = cell.left;
		tRc.right = cell.right;
	}
	tRc.InflateRect(0, 0, 0, 1);
	invalidateRect(&tRc, false);
	return true;
}

bool CfmGrid::lbuttonButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_validRows <= 0)
		return false;

	if (down && !m_cell.PtInRect(pt))
		return false;

	result = down ? RC_ITEM : RC_NOP;
	if (m_cellR[m_columns.GetAt(m_curpos.m_col-1)->m_ref].properties & PR_IMAGETEXT)
		invalidateRect(&m_cell, true);
	else
		invalidateRect(&m_cell, false);
	return true;
}

void CfmGrid::rbutton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_totCols <= 0)
		return;

	CRect	sRc, tmpRc1;
	int	ii,  idx;
	int	tCol,  colidx;
	float	tWidth;
	Ccolumn* col;

	if (m_pRc.PtInRect(pt))
	{
		tmpRc1 = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			tmpRc1.DeflateRect(2, 2);
		tmpRc1.bottom = tmpRc1.top + (int)(m_cy * m_headN * m_multiN);
		if (!(m_form->attr2 && GO_XHEAD))
		{
			if (!(m_form->attr2 & GO_X3D))
				tmpRc1.bottom += (4 * m_multiN);	// head extra space
			else
				tmpRc1.bottom += m_multiN;		// head extra space
		}
		if (tmpRc1.PtInRect(pt) && m_headN != 0)
			result = RC_HEAD;
		else
		{
			tmpRc1.top = tmpRc1.bottom;
			tmpRc1.bottom = m_pRc.bottom;
			if (!(m_form->attr2 & GO_X3D))
				tmpRc1.bottom -= 2;
			if (m_hscroll && !m_hscr.IsRectEmpty())
				tmpRc1.bottom = m_hscr.top;
			if (tmpRc1.PtInRect(pt))
				result = RC_ITEM;
		}
	}

	idx = 0;
	switch (result)
	{
	case RC_HEAD:
		for (ii = 0; ii < m_multiN; ii++)
		{
			tmpRc1 = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				tmpRc1.DeflateRect(2, 2);
			tmpRc1.top    = m_pRc.top  + (int)(m_cy*m_headN * ii);
			tmpRc1.bottom = tmpRc1.top + (int)(m_cy*m_headN);
			if (!(m_form->attr2 & GO_XHEAD))
			{
				if (!(m_form->attr2 & GO_X3D))
					tmpRc1.bottom += (4 * m_multiN);	// head extra space
				else
					tmpRc1.bottom += m_multiN;		// head extra space
			}
			if (tmpRc1.PtInRect(pt))
			{
				idx = ii;
				break;
			}
		}
		colidx = idx % m_multiN;
		tCol   = m_form->type ? 0 : m_stCol;
		if (m_form->attr2 & GO_MULTI)
		{
			for (ii = 0; ii < m_totCols; ii++)
			{
				if (m_columns.GetAt(ii)->m_line == colidx)
				{
					tCol = ii;
					break;
				}
			}
		}

		tWidth = 0;
		sRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			sRc.DeflateRect(2, 2);
		for (ii = tCol; ii < m_totCols; ii++)
		{
			if (m_form->type && ii == m_form->type)
				ii = m_stCol;

			col = m_columns.GetAt(ii);
			if (!col->m_visible)
				continue;
			if (col->m_line == colidx)
			{
				tmpRc1.left = sRc.left + (int)tWidth;
				tWidth += col->m_width;
				if ((int)tWidth < sRc.Width())
					tmpRc1.right = sRc.left + (int)tWidth;
				else
					tmpRc1.right = sRc.right;

				if (tmpRc1.PtInRect(pt))
				{
					m_cellpos.m_col = ii;
					break;
				}
			}
			else
			{
				m_cellpos.m_col = -1;
				break;
			}
		}
		if (ii >= m_totCols)
		{
			result = RC_OUT;
			m_cellpos.m_col = -1;
		}
		else
			m_cellpos.m_col++;
		break;

	case RC_ITEM:
		for (ii = m_headN*m_multiN; ii < m_showRows + 1; ii++)
		{
			tmpRc1 = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				tmpRc1.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					tmpRc1.top += (4 * m_multiN);	// head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				tmpRc1.top += m_multiN;	// head extra space
			tmpRc1.top   += (int)((m_cy + m_slit) * ii);
			tmpRc1.bottom = tmpRc1.top + (int)m_cy;

			if (tmpRc1.PtInRect(pt))
			{
				idx = m_stRow + ii - (m_headN * m_multiN);
				if (idx >= (m_totRows * m_multiN))
					idx = m_totRows * m_multiN - 1;
				break;
			}
		}
		m_cellpos.m_row = (idx / m_multiN);
		m_cellpos.m_row++;
		colidx = idx % m_multiN;
		tCol   = m_form->type ? 0 : m_stCol;
		if (m_form->attr2 & GO_MULTI)
		{
			for (ii = 0; ii < m_totCols; ii++)
			{
				if (m_columns.GetAt(ii)->m_line == colidx)
				{
					tCol = ii;
					break;
				}
			}
		}

		tWidth = 0;
		sRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			sRc.DeflateRect(2, 2);
		for (ii = tCol; ii < m_totCols; ii++)
		{
			if (m_form->type && ii == m_form->type)
				ii = m_stCol;

			col = m_columns.GetAt(ii);
			if (!col->m_visible)
				continue;

			if (col->m_line == colidx)
			{
				tmpRc1.left = sRc.left + (int)tWidth;
				tWidth += col->m_width;
				if ((int)tWidth < sRc.Width())
					tmpRc1.right = sRc.left + (int)tWidth;
				else
					tmpRc1.right = sRc.right;

				if (tmpRc1.PtInRect(pt))
				{
					m_cellpos.m_col = ii;
					break;
				}
				tmpRc1.left = tmpRc1.right;
			}
			else
			{
				m_cellpos.m_col = -1;
				break;
			}
		}
		if (ii >= m_totCols)
		{
			result = RC_OUT;
			m_cellpos.m_col = -1;
			m_cellpos.m_row = -1;
		}
		else
			m_cellpos.m_col++;
		break;

	default :
		break;
	}
}

int CfmGrid::drag(CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_move == mvDrag)
	{
		CDC*	dc;
		int	gap;

		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		dc = getDC();
		dc->SelectStockObject(NULL_PEN);
		dc->SelectStockObject(DKGRAY_BRUSH);
		dc->SetROP2(R2_NOT);
		dc->Rectangle(m_moveRc);

		gap = m_moveRc.Width();
		m_moveRc.left  = pt.x + m_delta;
		m_moveRc.right = m_moveRc.left + gap;
		dc->Rectangle(m_moveRc);
		releaseDC(dc);
		return -1;
	}

	Cpos	pos;
	Ccolumn* col = (Ccolumn*) 0;
	int	move, rc = CfmBase::OnDrag(pt, result);

	move   = m_move;
	m_move = mvNone;
	pos = m_movepos;
	m_movepos = Cpos(-1, -1);
	if (m_move != mvDrag && !(m_form->attr2 & GO_XSIZE) && getMovableColumn(pt))
	{
		m_move = mvMove;
		if (move == mvItem)
		{
			CRect	tRc;

			if (pos.m_col >= 0 && pos.m_col < m_totCols)
			{
				col = m_columns.GetAt(pos.m_col);
				if (m_cellR[col->m_ref].cursor == CS_POP && getRect(pos.m_row, pos.m_col, tRc))
				{
					tRc.InflateRect(1, 1);
					invalidateRect(&tRc, false);
				}
			}
		}
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		return rc;
	}

	if (result == RC_IN)
	{
		if ((!m_vscr.IsRectEmpty() && m_vscr.PtInRect(pt)) || (!m_hscr.IsRectEmpty() && m_hscr.PtInRect(pt))
			|| (!m_pscr.IsRectEmpty() && m_pscr.PtInRect(pt)))
		{
			if (!pos.IsEmpty() && move == mvItem)
			{
				CRect	tRc;

				col = m_columns.GetAt(pos.m_col);
				if (m_cellR[col->m_ref].cursor == CS_POP && getRect(pos.m_row, pos.m_col, tRc))
				{
					tRc.InflateRect(0, 1);
					invalidateRect(&tRc, false);
				}
			}
			m_axform->DoSomething(doCURSOR, this, CS_NONE);
			return rc;
		}

		CRect	pRc, tRc, sRc;
		int	skip{}, idx{};
		int	colidx{}, scol{};
		float	width{};

		if (m_form->attr2 & (GO_MERGE|GO_XHEAD))
			skip = m_headN*m_multiN;
		else
			skip = m_multiN;

		pRc = m_pRc;
		if (!m_vscr.IsRectEmpty())
			pRc.right = m_vscr.left;
		pRc.bottom = pRc.top + (int)(m_cy * m_headN * m_multiN);
		if (!(m_form->attr2 & GO_XHEAD))
		{
			if (!(m_form->attr2 & GO_X3D))
				pRc.bottom += (4 * m_multiN);	// head extra space
			else
				pRc.bottom += m_multiN;		// head extra space
		}

		if (pRc.PtInRect(pt) && m_headN != 0)		// in head
		{
			for (int ii = 0; ii < skip; ii++)
			{
				tRc = pRc;
				tRc.top    = pRc.top + (int)(m_cy*m_headN * ii);
				tRc.bottom = tRc.top + (int)(m_cy*m_headN);
				if (!(m_form->attr2 & GO_XHEAD))
				{
					if (!(m_form->attr2 & GO_X3D))
						tRc.bottom += (4 * m_multiN);	// head extra space
					else
						tRc.bottom += m_multiN;		// head extra space
				}
				if (tRc.PtInRect(pt))
				{
					idx = ii;
					break;
				}
			}

			colidx = idx % m_multiN;
			scol   = m_form->type ? 0 : m_stCol;
			if (m_form->attr2 & GO_MULTI)
			{
				for (int ii = 0; ii < m_totCols; ii++)
				{
					if (m_columns.GetAt(ii)->m_line == colidx)
					{
						scol = ii;
						break;
					}
				}
			}
			width = 0;
			sRc = tRc;
			for (int ii = scol; ii < m_totCols; ii++)
			{
				if (m_form->type && ii == m_form->type)
					ii = m_stCol;

				col = m_columns.GetAt(ii);
				if (!col->m_visible)
					continue;

				if (col->m_line == colidx)
				{
					sRc.left = tRc.left + (int)width;
					width += col->m_width;
					if ((int)width < tRc.Width())
						sRc.right = tRc.left + (int)width;
					else
						sRc.right = tRc.right;

					if (sRc.PtInRect(pt))
					{
						m_movepos.m_col = ii;
						break;
					}
				}
				else
					break;
			}

			if (col && m_movepos.m_col != -1)
			{
				m_move = mvHead;
				if (m_cellR[col->m_ref].hcursor == CS_SORTd)
				{
					if (m_sortc == col->m_ref)
						m_axform->DoSomething(doCURSOR, this, (m_dir == 2) ? CS_SORTa : CS_SORTd);
					else
						m_axform->DoSomething(doCURSOR, this, CS_SORTd);
				}
				else if (m_cellR[col->m_ref].hcursor >= CS_CURSOR)
					m_axform->DoSomething(doCURSOR, this, m_cellR[col->m_ref].hcursor);
			}
		}
		else
		{
			pRc.top = pRc.bottom;
			pRc.bottom = m_pRc.bottom;
			if (!(m_form->attr2 & GO_X3D))
				pRc.bottom -= 2;
			if (m_hscroll && !m_hscr.IsRectEmpty())
				pRc.bottom = m_hscr.top;
			if (pRc.PtInRect(pt))			// in item
			{
				int	ii, rows;

				idx  = -1;
				rows = min((m_validRows+m_headN)*m_multiN-1, m_showRows);
				tRc = pRc; tRc.bottom = tRc.top;
				for (ii = m_headN*m_multiN; ii < rows+1; ii++)
				{
					tRc.top    = tRc.bottom + m_slit;
					tRc.bottom = tRc.top + (int)m_cy;
					if (tRc.PtInRect(pt))
					{
						idx = m_stRow + ii - (m_headN * m_multiN);
						if (idx >= (m_totRows * m_multiN))
							idx = m_totRows * m_multiN - 1;
						sRc = tRc;
						break;
					}
				}

				if (idx != -1)
				{
					m_movepos.m_row = idx;
					colidx = idx % m_multiN;
					scol = m_form->type ? 0 : m_stCol;
					if (m_form->attr2 & GO_MULTI)
					{
						for (ii = 0; ii < m_totCols; ii++)
						{
							if (m_columns.GetAt(ii)->m_line == colidx)
							{
								scol = ii;
								break;
							}
						}
					}
					width = 0;
					tRc = pRc;
					for (ii = scol; ii < m_totCols; ii++)
					{
						if (m_form->type && ii == m_form->type)
							ii = m_stCol;

						col = m_columns.GetAt(ii);
						if (!col->m_visible)
							continue;

						if (col->m_line == colidx)
						{
							tRc.left = pRc.left + (int)width;
							width += col->m_width;
							if ((int)width < pRc.Width())
								tRc.right = pRc.left + (int)width;
							else
								tRc.right = pRc.right;

							if (tRc.PtInRect(pt))
							{
								sRc.IntersectRect(sRc, tRc);
								m_movepos.m_col = ii;
								break;
							}
						}
						else
						{
							m_movepos.m_col = -1;
							break;
						}
					}

					if (ii >= m_totCols)
						m_movepos.m_col = -1;
					
					if (m_movepos.m_col != -1)
					{
						m_move = mvItem;
						if (m_cellR[col->m_ref].cursor == CS_POP && !pos.Equal(m_movepos))
						{
							if (sRc.left == m_pRc.left)
								sRc.left++;
							invalidateRect(&sRc, false);
							if ((m_cellR[col->m_ref].iok == EIO_INPUT || m_cellR[col->m_ref].iok == EIO_INOUT)
									&& m_cellR[col->m_ref].kind == CK_NORMAL)
								m_axform->DoSomething(doCURSOR, this, CS_IBEAM);
						}
						else if ((m_cellR[col->m_ref].iok == EIO_INPUT || m_cellR[col->m_ref].iok == EIO_INOUT)
								&& m_cellR[col->m_ref].kind == CK_NORMAL)
							m_axform->DoSomething(doCURSOR, this, CS_IBEAM);
						else if (m_cellR[col->m_ref].cursor >= CS_CURSOR)
							m_axform->DoSomething(doCURSOR, this, m_cellR[col->m_ref].cursor);
					}
				}
			}
		}

		if (!pos.IsEmpty() && (move == mvItem && !pos.Equal(m_movepos)))
		{
			col = m_columns.GetAt(pos.m_col);
			if (m_cellR[col->m_ref].cursor == CS_POP && getRect(pos.m_row, pos.m_col, tRc))
			{
				tRc.InflateRect(0, 1);
				invalidateRect(&tRc, false);
			}
		}
	}
	else
	{
		if (!pos.IsEmpty() && move == mvItem)
		{
			CRect	tRc;

			col = m_columns.GetAt(pos.m_col);
			if (m_cellR[col->m_ref].cursor == CS_POP && getRect(pos.m_row, pos.m_col, tRc))
			{
				tRc.InflateRect(0, 1);
				invalidateRect(&tRc, false);
			}
		}
		m_axform->DoSomething(doCURSOR, this, CS_NONE);
	}

	return rc;
}

void CfmGrid::keyDelete(int& result)
{
	result = RC_NOP;
	if (m_state != stEdit)
		return;

	Ccolumn* column = m_columns.GetAt(m_curpos.m_col-1);
	int	index = (m_curpos.m_row-1)*m_totCols+column->m_ref;
	CString	tmps  = _T("");
	CString	data  = m_data.GetAt(index);
// updateXXX_2022XX	
	if (m_edstate == esReverse || (m_cellR[column->m_ref].attr & FA_FLOAT)
		|| (m_cellR[column->m_ref].isEdit == IE_EDIT && !column->m_edit.IsEmpty()))
	{
		data.Empty();
		m_edcaretpos = 0;
		m_edstate    = esNormal;
		if (m_cellR[column->m_ref].attr & FA_FLOAT)
			column->m_floats.SetAt(m_curpos.m_row-1, 0xff);
	}
	else
	{
		if (m_edcaretpos == data.GetLength())
		{
		//	result = RC_NODELETE;
			return;
		}
		else
		{
			tmps += data.Left(m_edcaretpos);
			if (whichHangulEdit(m_edcaretpos, data) == WC_H1)
			{
				if (data.GetLength() > m_edcaretpos+2)
					tmps += data.Mid(m_edcaretpos+2);
			}
			else
			{
				if (data.GetLength() > m_edcaretpos+1)
					tmps += data.Mid(m_edcaretpos+1);
			}
			data = tmps;
		}
	}

	m_data.SetAt(index, data);
	data = editData(data, column->m_ref, m_curpos.m_row-1, true);
	m_edit.SetAt(index, data);

	m_changed    = true;
	m_edfocusing = false;
	m_changedpos = m_curpos;
	CRect	tRc  = m_cell;
	tRc.InflateRect(2, 2);
	invalidateRect(&tRc, false);
}

void CfmGrid::keyBackspace(int& result)
{
	result = RC_NOP;
	if (m_state != stEdit)
		return;

	int	pos;
	Ccolumn* column = m_columns.GetAt(m_curpos.m_col-1);
	int	index = (m_curpos.m_row-1)*m_totCols+column->m_ref;
	CString	tmps  = _T("");
	CString	data  = m_data.GetAt(index);

// updateXXX_2022XX
	if (m_edstate == esReverse || (m_cellR[column->m_ref].attr & FA_FLOAT)
		|| (m_cellR[column->m_ref].isEdit == IE_EDIT && !column->m_edit.IsEmpty()))
	{
		data.Empty();
		m_edcaretpos = 0;
		m_edstate    = esNormal;
		if (m_cellR[column->m_ref].attr & FA_FLOAT)
			column->m_floats.SetAt(m_curpos.m_row-1, 0xff);
	}
	else
	{
		if (m_edcaretpos == 0)
		{
		//	result = RC_NODELETE;
			return;
		}
		else
		{
			if (m_cellR[column->m_ref].attr & FA_RIGHT)
			{
				m_edcaretpos--;
				tmps += data.Left(m_edcaretpos);
			}
			else
			{
				pos = m_edcaretpos;
				m_edcaretpos--;
				if (whichHangulEdit(m_edcaretpos, data) == WC_H2)
					m_edcaretpos--;
				
				tmps += data.Left(m_edcaretpos);
				if (data.GetLength() > pos)
					tmps += data.Mid(pos);
			}
			data = tmps;
		}
	}

	m_data.SetAt(index, data);
	data = editData(data, column->m_ref, m_curpos.m_row-1, true);
	m_edit.SetAt(index, data);

	m_changed    = true;
	m_edfocusing = false;
	m_changedpos = m_curpos;
	CRect	tRc  = m_cell;
	tRc.InflateRect(2, 2);
	invalidateRect(&tRc, false);
}

int CfmGrid::keyArrowLeft(int& result)
{
	bool	update;
	CString	data;
	CRect	tRc;
	Ccolumn* column = m_columns.GetAt(m_curpos.m_col-1);

	switch (m_state)
	{
	case stEdit:
		update = false;
		data   = m_data.GetAt((m_curpos.m_row-1)*m_totCols+column->m_ref);
		tRc    = m_cell;
		tRc.InflateRect(2, 2);
		if (m_edstate == esReverse)
		{
			m_edcaretpos = data.GetLength();
			m_edstate = esNormal;
			update    = true;
		}

		if (!(m_cellR[column->m_ref].attr & FA_RIGHT))
		{
			m_edstate = esNormal;
			if (m_edcaretpos > 0)
			{
				m_edcaretpos--;
				if (whichHangulEdit(m_edcaretpos, data) == WC_H2)
					m_edcaretpos--;
			
				m_edfocusing = false;
				update       = true;
			}
			else
				return (-1);
		}
		if (update)
			invalidateRect(&tRc, false);
		break;

	case stCombo:
	case stCheck:
	case stButton:	break;
	default:
		if (m_form->isEdit == FI_CELL)
		{
			int	pos, stCol;
			CRect	pRc;
			Cpos	lastpos = m_lastpos;

			if (result == RC_SHIFT)
				pos = m_lastpos.m_col;
			else
			{
				m_select = false;
				m_move   = mvNone;
				pos = m_curpos.m_col;
			}

			while (pos-1 > 0)
			{
				pos--;
				if (m_columns.GetAt(pos-1)->m_visible)
					break;
			}

			if (result == RC_SHIFT)
			{
				if (m_lastpos.m_col == pos)
					break;
				m_select = true;
				m_lastpos.m_col = pos;
	
				if (pos-1 >= m_stCol)
				{
					refreshSel(lastpos, VK_LEFT);
					return 0;
				}
			}
			else
			{
				if (m_curpos.m_col == pos)
					break;
				m_curpos.m_col  = pos;
				m_lastpos = m_curpos;
				changeIME(m_curpos.m_row-1, m_curpos.m_col-1);
			}

			pRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				pRc.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					pRc.top += (4 * m_multiN);	// head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				pRc.top += m_multiN;			// head extra space
			if (m_vscroll)
				pRc.right = m_vscr.left;
			if (m_form->attr2 & GO_PAGE)
				pRc.right = m_pscr.left;
			pRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);

			stCol = m_stCol;
			if (pos-1 < m_stCol)
			{
				m_stCol = max(pos-1, m_form->type);
				if (stCol != m_stCol)
				{
					if (m_form->type == m_totCols)
						m_stCol = 0;
					m_scrolledWidth = 0;
					for (int ii = m_form->type; ii < m_stCol; ii++)
					{
						if (m_columns.GetAt(ii)->m_visible)
							m_scrolledWidth += m_columns.GetAt(ii)->m_width;
					}

					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);	// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;	// head extra space
					if (m_vscroll)
						pRc.right = m_vscr.left;
					if (m_form->attr2 & GO_PAGE)
						pRc.right = m_pscr.left;
					pRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);
				}
			}

			invalidateRect(&pRc, false);
		}
		else
		{
			if (!m_hscroll || m_stCol < 0)
				return 0;

			if (m_scrolledWidth > 0)
			{
				CRect	tmpRc;
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
				for (int ii = m_form->type; ii < m_stCol; ii++)
				{
					if (m_columns.GetAt(ii)->m_visible)
						m_scrolledWidth += m_columns.GetAt(ii)->m_width;
				}

				tmpRc = m_pRc;
				if (m_vscroll)
					tmpRc.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tmpRc.right = m_pscr.left;
				tmpRc.bottom = m_hscr.top;
				invalidateRect(&tmpRc, false);

				tmpRc = m_hscr;
				tmpRc.left  += (int)(szSCROLL * m_hR);
				tmpRc.right -= (int)(szSCROLL * m_hR);
				invalidateRect(&tmpRc, false);
			}
		}
		break;
	}

	return 0;
}

void CfmGrid::keyArrowRight(int& result)
{
	bool	update;
	CString	data;
	CRect	tRc;
	int	ii, stCol;
	float	tWidth = 0;
	Ccolumn* column = m_columns.GetAt(m_curpos.m_col-1);

	switch (m_state)
	{
	case stEdit:
		update = false;
		data   = m_data.GetAt((m_curpos.m_row-1)*m_totCols+column->m_ref);
		tRc    = m_cell;
		tRc.InflateRect(2, 2);
		if (m_edstate == esReverse)
		{
			m_edcaretpos = data.GetLength();
			m_edstate = esNormal;
			update    = true;
		}

		if (!(m_cellR[column->m_ref].attr & FA_RIGHT))
		{
			m_edstate = esNormal;
			if (m_edcaretpos < data.GetLength())
			{
				m_edcaretpos++;
				if (whichHangulEdit(m_edcaretpos, data) == WC_H2)
					m_edcaretpos++;

				m_edfocusing = false;
				update       = true;
			}
		}

		if (update)
			invalidateRect(&tRc, false);
		break;

	case stCombo:
	case stCheck:
	case stButton:	break;
	default:
		if (m_form->isEdit == FI_CELL)
		{
			int	pos;
			CRect	pRc;
			Cpos	lastpos = m_lastpos;

			if (result == RC_SHIFT)
				pos = m_lastpos.m_col;
			else
			{
				m_select = false;
				m_move   = mvNone;
				pos = m_curpos.m_col;
			}

			while (pos < m_columns.GetSize())
			{
				pos++;
				if (m_columns.GetAt(pos-1)->m_visible)
					break;
			}

			stCol = m_stCol;
			tWidth = 0;
			for (ii = stCol; ii < pos; ii++)
			{
				if (m_columns.GetAt(ii)->m_visible)
					tWidth += m_columns.GetAt(ii)->m_width;
			}

			if (tWidth > m_showWidth - m_fixedWidth)
			{
				while (true)
				{
					if (m_stCol > m_totCols)
						break;
					m_stCol++;
					if (m_columns.GetAt(m_stCol)->m_visible)
						break;
				}
			}

			m_stCol = min(m_stCol, m_totCols-1);
			if (m_form->type == m_totCols)
				m_stCol = 0;

			if (result == RC_SHIFT)
			{
				if (m_lastpos.m_col == pos)
					break;
				m_select = true;
				m_lastpos.m_col = pos;
				if (stCol == m_stCol)
				{
					refreshSel(lastpos, VK_RIGHT);
					return;
				}
			}
			else
			{
				if (m_curpos.m_col == pos)
					break;
				m_curpos.m_col  = pos;
				m_lastpos = m_curpos;
				changeIME(m_curpos.m_row-1, m_curpos.m_col-1);
			}

			pRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
			{
				pRc.DeflateRect(2, 2);
				if (!(m_form->attr2 & GO_XHEAD))
					pRc.top += (4 * m_multiN);	// head extra space
			}
			else if (!(m_form->attr2 & GO_XHEAD))
				pRc.top += m_multiN;			// head extra space
			if (m_vscroll)
				pRc.right = m_vscr.left;
			if (m_form->attr2 & GO_PAGE)
				pRc.right = m_pscr.left;
			pRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);

			if (stCol != m_stCol)
			{
				m_scrolledWidth = 0;
				for (int ii = m_form->type; ii < m_stCol; ii++)
				{
					if (m_columns.GetAt(ii)->m_visible)
						m_scrolledWidth += m_columns.GetAt(ii)->m_width;
				}

				pRc = m_pRc;
				if (!(m_form->attr2 & GO_X3D))
				{
					pRc.DeflateRect(2, 2);
					if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += (4 * m_multiN);	// head extra space
				}
				else if (!(m_form->attr2 & GO_XHEAD))
					pRc.top += m_multiN;			// head extra space
				if (m_vscroll)
					pRc.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					pRc.right = m_pscr.left;
				pRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);
			}

			invalidateRect(&pRc, false);
		}
		else
		{
			if (!m_hscroll || (m_stCol < 0))
				return;

			stCol  = m_stCol;
			tWidth = 0;
			for (ii = m_totCols-1; ii >= stCol; ii--)
			{
				if (m_columns.GetAt(ii)->m_visible)
					tWidth += m_columns.GetAt(ii)->m_width;
			}

			if (tWidth > m_showWidth - m_fixedWidth)
			{
				while (true)
				{
					if (m_stCol >= (m_totCols - 1))
						break;
					m_stCol++;
					if (m_columns.GetAt(m_stCol)->m_visible)
						break;
				}
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

			if (stCol != m_stCol)
			{
				tRc = m_pRc;
				if (m_vscroll)
					tRc.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tRc.right = m_pscr.left;
				tRc.bottom = m_hscr.top;
				invalidateRect(&tRc, false);

				tRc = m_hscr;
				tRc.left  += (int)(szSCROLL * m_hR);
				tRc.right -= (int)(szSCROLL * m_hR);
				invalidateRect(&tRc, false);
			}
		}
		break;
	}
}

void CfmGrid::keyArrowUp(int& result)
{
	CRect	pRc;
	int	stRow, state;

	state  = result;
	result = RC_NOP;
	switch (m_state)
	{
	case stEdit:
		Ccolumn* column;
		column = m_columns.GetAt(m_curpos.m_col-1);
		if ((m_form->attr2 & GO_TABLE) && !column->m_enables.GetAt(m_curpos.m_row-1))
		{
			m_state = stFocus;
			m_edfocusing = true;
			m_edcaret    = false;
		}
		else
		{
			m_edfocusing = false;
			m_edcaret    = true;
			m_edcaretpos = 0;
		}
		[[fallthrough]];
	default:
		if (m_form->isEdit == FI_CELL && !(m_form->attr2 & GO_MULTI) && state == RC_SHIFT)
		{
			if (m_lastpos.m_row > 1)
			{
				Cpos	lastpos = m_lastpos;

				m_select = true;
				stRow = m_stRow;
				m_lastpos.m_row--;
				if (m_vscroll && (m_lastpos.m_row-1)*m_multiN < m_stRow)
					m_stRow = (m_lastpos.m_row-1)*m_multiN;

				if (stRow != m_stRow)
				{
					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);	// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;	// head extra space
					if (m_vscroll)
						pRc.right = m_vscr.left;
					if (m_form->attr2 & GO_PAGE)
						pRc.right = m_pscr.left;
					pRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);
					invalidateRect(&pRc, false);
				}
				else
					refreshSel(lastpos, VK_UP);
			}
		}
		else
		{
			if (state != RC_SHIFT && (m_index <= 0
				|| (m_form->isEdit == FI_RREC && m_curpos.m_row <= 1 && m_index <= 0)))
			{
				if ((m_vscroll || m_form->attr2 & GO_PAGE) && !m_writingAll && m_save == svExchange
					&& (!m_useInfo || (m_useInfo && (m_stat & statPREV))))
				{
					result = RC_SCRUP;	// exFirst
				}
			}
			else
			{
				int	idx;

				idx   = m_index;
				stRow = m_stRow;
				if (m_form->isEdit == FI_RREC)
					m_index = max(0, m_index-m_multiN);
				else
					m_index--;
				if (m_changed)
				{
					result = RC_CHANGED;
					m_changedpos = m_curpos;
				}
				else if (m_form->attr2 & GO_KEY)
					result = RC_ITEM;

				m_curpos.m_row = (m_index / m_multiN);
				m_curpos.m_row++;
				m_lastpos = m_curpos;
				changeIME(m_curpos.m_row-1, m_curpos.m_col-1);

				if (m_vscroll && m_index < m_stRow)
				{
					if (m_form->isEdit == FI_RREC)
						m_stRow = max(0, m_stRow-m_multiN);
					else
						m_stRow--;
				}

				if (stRow != m_stRow || m_select || m_move == mvSel)
				{
					m_select = false;
					m_move   = mvNone;

					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);	// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;			// head extra space
					if (m_vscroll)
						pRc.right = m_vscr.left;
					pRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
					if (m_hscroll)
						pRc.bottom = m_hscr.top;
					invalidateRect(&pRc, false);

					if (m_vscroll)
					{
						int	height = m_vthumb.Height();
						CRect	pRc2   = m_vthumb;
						pRc = m_vthumb;
						CRect	sRc, thumb;

						sRc = thumb = m_vscr;
						if (m_form->attr2 & GO_PAGE)
              						sRc.top = m_pscr.bottom;
						else
							sRc.top += (int)(szSCROLL * m_vR);
                 				sRc.bottom = m_vscr.bottom - (int)(szSCROLL * m_vR);

						thumb.top = sRc.top + (int)(m_stRow * m_vgap);
						if (m_stRow + (m_showRows - m_headN * m_multiN) == (m_validRows * m_multiN))
							thumb.bottom = sRc.bottom;
						else
							thumb.bottom = thumb.top + m_vthumb.Height();
						thumb.bottom = min(thumb.bottom, sRc.bottom);
						m_vthumb = thumb;

						pRc.bottom = m_vthumb.top;
						pRc.NormalizeRect();
						pRc2.top    = pRc2.bottom;
						pRc2.bottom = m_vthumb.bottom;
						pRc2.NormalizeRect();

						if (pRc.top != pRc.bottom)
						{
							pRc.bottom += 2;
							invalidateRect(&pRc, false);
						}
						if (pRc2.top != pRc2.bottom)
						{
							pRc2.top -= 2;
							invalidateRect(&pRc2, false);
						}
					}
				}
				else
				{
					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);		// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;				// head extra space
					pRc.top   += (int)((m_cy+m_slit) * (m_headN * m_multiN));
					if (m_form->isEdit == FI_RREC)
					{
						pRc.top   += (int)((m_cy+m_slit) * (m_index/m_multiN*m_multiN - m_stRow));
						pRc.bottom = pRc.top + (int)((m_cy+m_slit)*m_multiN) + 3;
					}
					else
					{
						pRc.top   += (int)((m_cy+m_slit) * (m_index - m_stRow));
						pRc.bottom = pRc.top + (int)m_cy + 3;
					}
					if (m_vscroll)
						pRc.right = m_vscr.left;
					pRc.InflateRect(0, 4);
					if (m_hscroll)
						pRc.bottom = min(pRc.bottom, m_hscr.top);
					invalidateRect(&pRc, false);

					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);		// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;				// head extra space
					pRc.top   += (int)((m_cy+m_slit) * (m_headN * m_multiN));
					if (m_form->isEdit == FI_RREC)
					{
						pRc.top   += (int)((m_cy+m_slit) * (idx/m_multiN*m_multiN - m_stRow));
						pRc.bottom = pRc.top + (int)((m_cy+m_slit)*m_multiN) + 3;
					}
					else
					{
						pRc.top   += (int)((m_cy+m_slit) * (idx - m_stRow));
						pRc.bottom = pRc.top + (int)m_cy + 3;
					}
					if (m_vscroll)
						pRc.right = m_vscr.left;
					pRc.InflateRect(0, 4);
					if (m_hscroll)
						pRc.bottom = min(pRc.bottom, m_hscr.top);
					invalidateRect(&pRc, false);
				}
			}
		}
		break;
	}

	m_result = result;
}

void CfmGrid::keyArrowDown(int& result)
{
	CRect	pRc;
	int	stRow, state;

	state  = result;
	result = RC_NOP;
	switch (m_state)
	{
	case stEdit:
		Ccolumn* column;
		column = m_columns.GetAt(m_curpos.m_col-1);
		if ((m_form->attr2 & GO_TABLE) && !column->m_enables.GetAt(m_curpos.m_row-1))
		{
			m_state = stFocus;
			m_edfocusing = true;
			m_edcaret    = false;
		}
		else
		{
			m_edfocusing = false;
			m_edcaret    = true;
			m_edcaretpos = 0;
		}

	default:
		if (m_form->isEdit == FI_CELL && !(m_form->attr2 & GO_MULTI) && state == RC_SHIFT)
		{
			if (m_lastpos.m_row < m_validRows)
			{
				Cpos	lastpos = m_lastpos;

				m_select = true;
				stRow = m_stRow;
				m_lastpos.m_row++;
				if (m_vscroll && m_stRow+m_showRows-(m_headN*m_multiN) <= (m_lastpos.m_row-1)*m_multiN)
					m_stRow++;

				if (stRow != m_stRow)
				{
					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);	// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;			// head extra space
					if (m_vscroll)
						pRc.right = m_vscr.left;
					if (m_form->attr2 & GO_PAGE)
						pRc.right = m_pscr.left;
					pRc.top  += (int)((m_cy+m_slit)*m_headN*m_multiN);
					invalidateRect(&pRc, false);
				}
				else
					refreshSel(lastpos, VK_DOWN);
			}
		}
		else
		{
			if ((m_index >= m_validRows*m_multiN-1
				|| (m_form->isEdit == FI_RREC && m_curpos.m_row == m_validRows && m_index == m_validRows*m_multiN-1))
				&& state != RC_SHIFT)
			{
				if ((m_vscroll || m_form->attr2 & GO_PAGE) && !m_writingAll && m_save == svExchange
					&& (!m_useInfo || (m_useInfo && (m_stat & statNEXT))))
				{
					result = RC_SCRDN;
				}
			}
			else
			{
				int	idx;

				idx   = m_index;
				stRow = m_stRow;
				if (m_form->isEdit == FI_RREC)
					m_index = min(m_validRows*m_multiN-1, m_index+m_multiN);
				else
					m_index++;
				if (m_changed)
				{
					result = RC_CHANGED;
					m_changedpos = m_curpos;
				}
				else if (m_form->attr2 & GO_KEY)
					result = RC_ITEM;

				m_curpos.m_row = (m_index / m_multiN);
				m_curpos.m_row++;
				m_lastpos = m_curpos;
				changeIME(m_curpos.m_row-1, m_curpos.m_col-1);

				if (m_vscroll && stRow+m_showRows-(m_headN*m_multiN) <= m_index)
				{
					if (m_form->isEdit == FI_RREC)
						m_stRow = min(m_stRow+m_multiN, m_validRows*m_multiN - (m_showRows-(m_multiN*m_headN)));
					else
						m_stRow++;
				}

				if (stRow != m_stRow || m_select || m_move == mvSel)
				{
					m_select = false;
					m_move   = mvNone;

					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);	// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;	// head extra space
					if (m_vscroll)
						pRc.right = m_vscr.left;
					pRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
					if (m_hscroll)
						pRc.bottom = m_hscr.top;
					invalidateRect(&pRc, false);

					if (m_vscroll)
					{
						int	height = m_vthumb.Height();
						CRect	pRc2   = m_vthumb;
						pRc = m_vthumb;

						CRect	sRc, thumb;
						sRc = thumb = m_vscr;
						if (m_form->attr2 & GO_PAGE)
              						sRc.top = m_pscr.bottom;
						else
							sRc.top += (int)(szSCROLL * m_vR);
                 				sRc.bottom = m_vscr.bottom - (int)(szSCROLL * m_vR);

						thumb.top = sRc.top + (int)(m_stRow * m_vgap);
						if (m_stRow + (m_showRows - m_headN * m_multiN) == m_validRows * m_multiN)
							thumb.bottom = sRc.bottom;
						else
							thumb.bottom = thumb.top + m_vthumb.Height();
						thumb.bottom = min(thumb.bottom, sRc.bottom);
						m_vthumb = thumb;

						pRc.bottom = m_vthumb.top;
						pRc.NormalizeRect();
						pRc2.top    = pRc2.bottom;
						pRc2.bottom = m_vthumb.bottom;
						pRc2.NormalizeRect();

						if (pRc.top != pRc.bottom)
						{
							pRc.bottom += 2;
							invalidateRect(&pRc, false);
						}
						if (pRc2.top != pRc2.bottom)
						{
							pRc2.top -= 2;
							invalidateRect(&pRc2, false);
						}
					}
				}
				else
				{
					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);		// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;		// head extra space
					pRc.top   += (int)((m_cy+m_slit) * (m_headN * m_multiN));
					if (m_form->isEdit == FI_RREC)
					{
						pRc.top   += (int)((m_cy+m_slit) * (m_index/m_multiN*m_multiN - m_stRow));
						pRc.bottom = pRc.top + (int)((m_cy+m_slit)*m_multiN) + 3;
					}
					else
					{
						pRc.top   += (int)((m_cy+m_slit) * (m_index - m_stRow));
						pRc.bottom = pRc.top + (int)m_cy + 3;
					}
					if (m_vscroll)
						pRc.right = m_vscr.left;
					pRc.InflateRect(0, 4);
					if (m_hscroll)
						pRc.bottom = min(pRc.bottom, m_hscr.top);
					invalidateRect(&pRc, false);

					pRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
					{
						pRc.DeflateRect(2, 2);
						if (!(m_form->attr2 & GO_XHEAD))
							pRc.top += (4 * m_multiN);		// head extra space
					}
					else if (!(m_form->attr2 & GO_XHEAD))
						pRc.top += m_multiN;		// head extra space
					pRc.top   += (int)((m_cy+m_slit) * (m_headN * m_multiN));
					if (m_form->isEdit == FI_RREC)
					{
						pRc.top   += (int)((m_cy+m_slit) * (idx/m_multiN*m_multiN - m_stRow));
						pRc.bottom = pRc.top + (int)((m_cy+m_slit)*m_multiN) + 3;
					}
					else
					{
						pRc.top   += (int)((m_cy+m_slit) * (idx - m_stRow));
						pRc.bottom = pRc.top + (int)m_cy + 3;
					}
					if (m_vscroll)
						pRc.right = m_vscr.left;
					pRc.InflateRect(0, 4);
					if (m_hscroll)
						pRc.bottom = min(pRc.bottom, m_hscr.top);
					invalidateRect(&pRc, false);
				}
			}
		}
		break;
	}

	m_result = result;
}

void CfmGrid::keyReturn(int& result)
{
	if (m_axform->m_mapH->options & OP_CR2TB)
	{
		keyTab(result);
		return;
	}

	CRect	tRc;

	result = RC_NOP;
	switch (m_state)
	{
	case stCombo:
		DoListBox(false);
		result = RC_CBCLOSE;	// RC_OK ??? manie
		invalidateRect(&m_cell, false);
		break;

	case stEdit:
	case stCheck:
		tRc = m_cell;
		tRc.InflateRect(2, 2);
		m_state   = stFocus;
		m_edstate = esNone;
		m_axform->m_view->HideCaret();
		invalidateRect(&tRc, false);
		break;
	case stNone:
	default:
		result = RC_NOP;
		break;
	}
}

void CfmGrid::keyEscape(int& result)
{
	m_select = false;
	m_move   = mvNone;
	keyReturn(result);
}

void CfmGrid::keyPrior(int& result)
{
	result = RC_NOP;
	if (!(m_state == stNone || m_state == stFocus))
		return;

	if (!(m_form->attr2 & GO_PAGE) && (!m_vscroll || m_validRows <= 0))
		return;

	if (m_stRow <= 0)
	{
		if (!m_writingAll && m_save == svExchange
			&& (!m_useInfo || (m_useInfo && (m_stat & statPREV))))
			result = RC_SCRUP;
		m_result = result;
		return;
	}	

	int	stline = m_stRow;
	m_stRow  = max(m_stRow-(m_showRows-(m_multiN*m_headN)), 0);
	m_index -= (stline - m_stRow);

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;			// head extra space
	tRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
	if (m_hscroll)
		tRc.bottom = m_hscr.top;
	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = m_pscr.left;
	invalidateRect(&tRc, false);

	if (m_vscroll)
	{
		tRc = m_vscr;
		if (m_form->attr2 & GO_PAGE)
			tRc.top = m_pscr.bottom;
		else
			tRc.top += (int)(szSCROLL * m_vR);
		tRc.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::keyNext(int& result)
{
	result = RC_NOP;
	if (!(m_state == stNone || m_state == stFocus))
		return;

	if (!(m_form->attr2 & GO_PAGE) && (!m_vscroll || m_validRows <= 0))
		return;

	if (m_stRow >= m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN)))
	{
		if (!m_writingAll && m_save == svExchange
			&& (!m_useInfo || (m_useInfo && (m_stat & statNEXT))))
			result = RC_SCRDN;	// exLast
		m_result = result;
		return;
	}

	int	stline = m_stRow;
	m_stRow += (int)(m_showRows - (m_multiN * m_headN));
	m_stRow  = max(0, min(m_stRow, m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN))));
	m_index += (m_stRow - stline);

	CRect	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;			// head extra space
	tRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
	if (m_hscroll)
		tRc.bottom = m_hscr.top;
	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = m_pscr.left;
	invalidateRect(&tRc, false);

	if (m_vscroll)
	{
		tRc = m_vscr;
		if (m_form->attr2 & GO_PAGE)
			tRc.top = m_pscr.bottom;
		else
			tRc.top += (int)(szSCROLL * m_vR);
		tRc.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::keyHome(int& result)
{
	result = RC_NOP;
	if (!(m_state == stNone || m_state == stFocus))
		return;

	if ((m_stRow == 0 && m_index == 0) || m_validRows <= 0)
		return;

	int	stline = m_stRow;
	int	index  = m_index;
	CRect	tRc    = m_pRc;

	m_stRow = m_index = 0;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;	// head extra space
	tRc.top  += (int)((m_cy+m_slit) * m_headN * m_multiN);
	if (m_hscroll)
		tRc.bottom = m_hscr.top;
	if (stline == m_stRow)
	{
		if (m_form->isEdit == FI_RREC)
			tRc.bottom = min(tRc.bottom, (int)(tRc.top+(m_cy+m_slit)*(index/m_multiN*m_multiN - m_index/m_multiN*m_multiN + 1)));
		else
			tRc.bottom = min(tRc.bottom, (int)(tRc.top+(m_cy+m_slit)*(index-m_index+1)));
	}
	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = m_pscr.left;
	invalidateRect(&tRc, false);

	if (m_vscroll)
	{
		tRc = m_vscr;
		if (m_form->attr2 & GO_PAGE)
			tRc.top = m_pscr.bottom;
		else
			tRc.top += (int)(szSCROLL * m_vR);
		tRc.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::keyEnd(int& result)
{
	result = RC_NOP;
	if (!(m_state == stNone || m_state == stFocus))
		return;

	if (m_validRows <= 0)
		return;

	int	stline;
	
	stline = m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN));
	if (m_stRow == stline && m_index == m_validRows*m_multiN-1)
		return;

	CRect	tRc   = m_pRc;
	int	index = m_index;

	stline  = m_stRow;
	m_stRow = m_validRows*m_multiN - (m_showRows - (m_multiN * m_headN));
	m_index = m_validRows*m_multiN - 1;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;	// head extra space
	tRc.top  += (int)((m_cy+m_slit) * m_headN * m_multiN);
	if (m_hscroll)
		tRc.bottom = m_hscr.top;
	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = m_pscr.left;
	if (stline == m_stRow)
	{
		if (m_form->isEdit == FI_RREC)
			tRc.top = min(tRc.top, (int)(tRc.top+(m_cy+m_slit)*(index/m_multiN*m_multiN)));
		else
			tRc.top = min(tRc.top, (int)(tRc.top+(m_cy+m_slit)*index));
	}
	invalidateRect(&tRc, false);
	if (m_vscroll)
	{
		tRc = m_vscr;
		if (m_form->attr2 & GO_PAGE)
			tRc.top = m_pscr.bottom;
		else
			tRc.top += (int)(szSCROLL * m_vR);
		tRc.bottom -= (int)(szSCROLL * m_vR);
		m_vthumb.SetRectEmpty();
		invalidateRect(&tRc, false);
	}
}

void CfmGrid::keyTab(int& result)
{
	if (!m_input || m_state == stNone || m_state == stFocus)
	{
		result  = RC_NOP;
		m_state = stNone;
		return;
	}

	bool	hscroll;
	int	state  = result;
	Cpos	prePos = m_curpos;
	int	stCol  = m_stCol;

	result = RC_NOP;
	if (m_changed)
	{
		result = RC_CHANGED;
		m_changedpos = m_curpos;
	}

	if (findCell((state == RC_SHIFT) ? true : false, m_curpos.m_row-1, m_curpos.m_col))
	{
		result  = (result == RC_NOP) ? RC_OK : result;
		m_stCol = findStartColumn(m_curpos.m_col-1);
		hscroll = (stCol == m_stCol) ? false : true;
		m_index = (m_curpos.m_row-1) * m_multiN + m_columns.GetAt(m_curpos.m_col-1)->m_line;

		switch (m_state)
		{
		case stCombo:
			m_cbspread = false;
			DoListBox(false);
			break;

		case stEdit:
			m_axform->m_view->HideCaret();
			break;
		}

		switch (m_cellR[m_columns.GetAt(m_curpos.m_col-1)->m_ref].kind)
		{
		case CK_COMBO:
			m_state = stCombo;	break;
		case CK_CHECK:
			m_state = stCheck;	break;
		case CK_BUTTON:
			m_state = stButton;	break;
		default:
			m_state = stEdit;	break;
		}

		CRect	tRc;
		if (prePos.m_row == m_curpos.m_row
			&& m_columns.GetAt(prePos.m_col-1)->m_line == m_columns.GetAt(m_curpos.m_col-1)->m_line)
		{
			if (hscroll)
			{
				tRc = m_pRc;
				if (!(m_form->attr2 & GO_X3D))
					tRc.DeflateRect(2, 2);
			}
			else
			{
				tRc = m_cell;
				if (m_form->isEdit == FI_RREC)
				{
					if (m_columns.GetAt(m_curpos.m_col-1)->m_line > 0)
						tRc.top -= (int)((m_cy+m_slit)*m_columns.GetAt(m_curpos.m_col-1)->m_line);
					tRc.bottom = tRc.top + (int)((m_cy+m_slit)*m_multiN);
				}
				tRc.InflateRect(2, 4);
				tRc.left  = m_pRc.left;
				tRc.right = m_pRc.right;
				if (m_vscroll)
					tRc.right = m_vscr.left;
				if (m_form->attr2 & GO_PAGE)
					tRc.right = m_pscr.left;
			}
			if (m_hscroll)
			{
				if (hscroll)
					m_hthumb.SetRectEmpty();
				else
					tRc.bottom = min(tRc.bottom, m_hscr.top);
			}
			if (m_state == stEdit)
			{
				CString	tmps;

				tmps = m_data.GetAt((m_curpos.m_row-1)*m_totCols + m_columns.GetAt(m_curpos.m_col-1)->m_ref);
				tmps.Trim();
				m_edstate    = tmps.IsEmpty() ? esNormal : esReverse;
				m_edfocusing = false;
				m_edcaret    = true;
				m_edcaretpos = tmps.GetLength();
			}
			invalidateRect(&tRc, false);
		}
		else
		{
			if (m_index < m_stRow)
				m_stRow = m_index;
			else if (m_index >= m_stRow+m_showRows-m_headN*m_multiN)
				m_stRow = max(0, m_index-(m_showRows-m_headN*m_multiN)+1);
			else
			{
				if (hscroll)
				{
					tRc = m_pRc;
					if (!(m_form->attr2 & GO_X3D))
						tRc.DeflateRect(2, 2);
				}
				else
				{
					tRc = m_cell;
					if (m_form->isEdit == FI_RREC)
					{
						if (m_columns.GetAt(prePos.m_col-1)->m_line > 0)
							tRc.top -= (int)((m_cy+m_slit)*m_columns.GetAt(prePos.m_col-1)->m_line);
						tRc.bottom = tRc.top + (int)((m_cy+m_slit)*m_multiN);
					}
					tRc.InflateRect(2, 4);
					tRc.left  = m_pRc.left;
					tRc.right = m_pRc.right;
					if (m_vscroll)
						tRc.right = m_vscr.left;
					if (m_form->attr2 & GO_PAGE)
						tRc.right = m_pscr.left;
				}
				if (m_hscroll)
				{
					if (hscroll)
						m_hthumb.SetRectEmpty();
					else
						tRc.bottom = m_hscr.top;
				}
				invalidateRect(&tRc, false);

				if (m_state == stEdit)
				{
					CString	tmps;

					tmps = m_data.GetAt((m_curpos.m_row-1)*m_totCols + m_columns.GetAt(m_curpos.m_col-1)->m_ref);
					tmps.TrimLeft(); tmps.TrimRight();
					m_edstate    = tmps.IsEmpty() ? esNormal : esReverse;
					m_edfocusing = false;
					m_edcaret    = true;
					m_edcaretpos = tmps.GetLength() ;
				}

				if (getRect(m_curpos.m_row-1, m_curpos.m_col-1, tRc))
				{
					if (m_form->isEdit == FI_RREC)
					{
						if (m_columns.GetAt(m_curpos.m_col-1)->m_line > 0)
							tRc.top -= (int)((m_cy+m_slit)*m_columns.GetAt(m_curpos.m_col-1)->m_line);
						tRc.bottom = tRc.top + (int)((m_cy+m_slit)*m_multiN);
					}
					tRc.InflateRect(2, 4);
					tRc.left  = m_pRc.left;
					tRc.right = m_pRc.right;
					invalidateRect(&tRc, false);
				}
				return;
			}

			if (m_state == stEdit)
			{
				m_edstate    = esReverse;
				m_edfocusing = false;
				m_edcaret    = true;
				m_edcaretpos = m_data.GetAt((m_curpos.m_row-1)*m_totCols + m_columns.GetAt(m_curpos.m_col-1)->m_ref).GetLength();
			}

			tRc = m_pRc;
			if (!(m_form->attr2 & GO_X3D))
				tRc.DeflateRect(2, 2);

			if (m_vscroll)
				tRc.right = m_vscr.left;
			if (m_form->attr2 & GO_PAGE)
				tRc.right = m_pscr.left;
			if (!hscroll)
				tRc.top += (int)(m_cy * m_headN * m_multiN);
			if (m_hscroll)
				tRc.bottom = m_hscr.top;
			invalidateRect(&tRc, false);

			if (m_vscroll)
			{
				int	height = m_vthumb.Height();
				CRect	tRc2 = m_vthumb;
				tRc = m_vthumb;
				CRect	sRc, thumb;

				sRc = thumb = m_vscr;
				if (m_form->attr2 & GO_PAGE)
              				sRc.top = m_pscr.bottom;
				else
					sRc.top += (int)(szSCROLL * m_vR);
                 		sRc.bottom = m_vscr.bottom - (int)(szSCROLL * m_vR);

				thumb.top = sRc.top + (int)(m_stRow * m_vgap);
				if (m_stRow + (m_showRows - m_headN * m_multiN) == (m_validRows * m_multiN))
					thumb.bottom = sRc.bottom;
				else
					thumb.bottom = thumb.top + m_vthumb.Height();
				thumb.bottom = min(thumb.bottom, sRc.bottom);
				m_vthumb = thumb;

				tRc.bottom = m_vthumb.top;
				tRc.NormalizeRect();
				tRc2.top    = tRc2.bottom;
				tRc2.bottom = m_vthumb.bottom;
				tRc2.NormalizeRect();

				if (tRc.top != tRc.bottom)
				{
					tRc.bottom += 2;
					invalidateRect(&tRc, false);
				}
				if (tRc2.top != tRc2.bottom)
				{
					tRc2.top -= 2;
					invalidateRect(&tRc2, false);
				}
			}

			if (hscroll)
			{
				int	height = m_hthumb.Width();
				CRect	tRc2 = m_hthumb;
				CRect	sRc1, sRc2;

				tRc = m_hthumb;
				sRc1 = sRc2 = m_hscr;
              			sRc1.right = m_hscr.left  + (int)(szSCROLL * m_hR);
                 		sRc2.left  = m_hscr.right - (int)(szSCROLL * m_hR);

				sRc1.left = sRc1.right + (int)(m_scrolledWidth * m_hgap);
				if (m_totWidth == m_showWidth - m_fixedWidth + m_scrolledWidth)
					sRc1.right = sRc2.left;
				else
					sRc1.right = sRc1.left + (int)(m_hgap * (m_showWidth - m_fixedWidth));
				sRc1.right = min(sRc1.right, sRc2.left);
				m_vthumb = sRc1;

				tRc.right = m_vthumb.left;
				tRc.NormalizeRect();
				tRc2.left  = tRc2.right;
				tRc2.right = m_hthumb.right;
				tRc2.NormalizeRect();

				if (tRc.left != tRc.right)
				{
					tRc.right += 2;
					invalidateRect(&tRc, false);
				}
				if (tRc2.left != tRc2.right)
				{
					tRc2.left -= 2;
					invalidateRect(&tRc2, false);
				}
			}
		}
	}
	else
	{
		result  = (result == RC_NOP)? RC_NOP : result;
		if (m_state == stEdit)
			m_axform->m_view->HideCaret();
		m_state = stNone;
	}
}

int CfmGrid::findColumn(CString name, bool origin)
{
	name.Trim();
	if (origin)
	{
		for (int ii = 0; ii < m_totCols; ii++)
		{
			if (!name.CompareNoCase((char *)m_cellR[ii].name))
				return ii;
		}
	}
	else
	{
		for (int ii = 0; ii < m_totCols; ii++)
		{
			if (!name.CompareNoCase((char *)m_cellR[m_columns.GetAt(ii)->m_ref].name))
				return ii;
		}
	}
	return -1;
}

int CfmGrid::findColumn(int ref)
{
	for (int ii = 0; ii < m_totCols; ii++)
	{
		if (m_columns.GetAt(ii)->m_ref == ref)
			return ii;
	}

	return -1;
}

Ccolumn* CfmGrid::getColumn(int ref)
{
	for (int ii = 0; ii < m_totCols; ii++)
	{
		if (m_columns.GetAt(ii)->m_ref == ref)
			return m_columns.GetAt(ii);
	}

	return NULL;
}

int CfmGrid::findRow(int py, bool isrow)
{
	CRect	tRc;
	int	row = -1, rows = min((m_validRows+m_headN)*m_multiN-1, m_showRows);
	

	for (int ii = m_headN*m_multiN; ii < rows + 1; ii++)
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
		tRc.top   += (int)((m_cy+m_slit) * ii);
		tRc.bottom = tRc.top + (int)m_cy;
		if (m_hscroll)
			tRc.bottom = min(tRc.bottom, m_hscr.top);
		if (tRc.top <= py && py <= tRc.bottom)
		{
			row = m_stRow + ii - (m_headN * m_multiN);
			if (row >= (m_totRows * m_multiN))
				row = m_totRows * m_multiN - 1;
			if (isrow)
				row /= m_multiN;
			break;
		}
	}

	return row;
}

int CfmGrid::findRow(int py, CRect& rc, CRect& rcx)
{
	CRect	tRc;
	int	row = -1, rows = min((m_validRows+m_headN)*m_multiN-1, m_showRows);
	
	for (int ii = m_headN*m_multiN; ii < rows + 1; ii++)
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
		tRc.top   += (int)((m_cy+m_slit) * ii);
		tRc.bottom = tRc.top + (int)m_cy;
		if (m_lastpos.m_row-1 == m_stRow + ii - (m_headN * m_multiN))
		{
			rcx = tRc;
			if (!rc.IsRectEmpty())
				break;
		}
		if (rc.IsRectEmpty() && tRc.top <= py && py <= tRc.bottom)
		{
			rc = tRc;
			row = m_stRow + ii - (m_headN * m_multiN);
			if (row >= (m_totRows * m_multiN))
				row = m_totRows * m_multiN - 1;
			if (!rcx.IsRectEmpty())
				break;
		}
	}

	return row;
}

int CfmGrid::findStartColumn(int col)
{
//	if (!m_hscroll || (m_form->attr2 & GO_MULTI))
//		return m_stCol;

	if (!m_hscroll)
		return m_stCol;

	CRect	tRc;
	float	width;
	bool	change;
	int	start, stCol;

	tRc = m_pRc;
	change = false;
	start = stCol = m_stCol;

	if (start >= col)
		start = m_form->type;
	if (!(m_form->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);
	if (m_vscroll && !m_vscr.IsRectEmpty())
		tRc.right = m_vscr.left;

	if (m_columns.GetAt(start)->m_line != m_columns.GetAt(col)->m_line)
	{
		change = true;
		for (int ii = 0; ii < m_totCols; ii++)
		{
			if (m_columns.GetAt(ii)->m_line == m_columns.GetAt(col)->m_line)
			{
				start = stCol = ii;
				break;
			}
		}
		
		if (m_form->type > 0)
		{
			width = 0;
			for (int ii = stCol; ii < m_totCols; ii++)
			{
				if (m_columns.GetAt(ii)->m_line != m_columns.GetAt(col)->m_line)
				{
					start = stCol;
					break;
				}

				if (m_columns.GetAt(ii)->m_visible)
					width += m_columns.GetAt(ii)->m_width;
				if (width > m_fixedWidth)
				{
					start = ii;
					break;
				}
			}
		}
	}

	for ( ; true; )
	{
		width = 0;
		for (int ii = start; ii <= col; ii++)
		{
			if (m_columns.GetAt(ii)->m_visible)
				width += m_columns.GetAt(ii)->m_width;
		}

		if (width < tRc.Width() - m_fixedWidth)
			break;

		start++;
		if (start >= m_totCols)
			start = m_form->type;
		if (m_form->type == m_totCols)
			start = 0;
	}

	if (change && start != m_stCol)
	{
		if (start == stCol)
			start = m_form->type;
		else
		{
			int 	ii;
			float	ww;

			width = ww = 0;
			for (ii = stCol; ii < start; ii++)
				width += m_columns.GetAt(ii)->m_width;
			for (ii = m_stCol; ii < m_totCols; ii++)
			{
				if (m_columns.GetAt(ii)->m_line > 0)
				{
					start = m_form->type;
					break;
				}
				ww += m_columns.GetAt(ii)->m_width;
				if (ww > width)
				{
					start = ii;
					break;
				}
			}
		}
	}

	m_scrolledWidth = 0;
	for (int ii = m_form->type; ii < start; ii++)
	{
		if (m_columns.GetAt(ii)->m_visible)
			m_scrolledWidth += m_columns.GetAt(ii)->m_width;
	}

	return start;
}

bool CfmGrid::findCell(bool back, int row, int col)
{
	bool	quit = false;
	DWORD	attr;
	Ccolumn* column;
	int	ii;

	for (ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);
		if ((m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT) && column->m_visible)
			break;
	}
	if (ii >= m_totCols || row >= m_validRows)
		return false;

	if (back)
	{
		col -= 2;
		for (; row >= 0; row--)
		{
			for (; col >= 0; col--)
			{
				column = m_columns.GetAt(col);
				attr = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[column->m_ref].attr;
				if (!column->m_visible || (attr & FA_PROTECT))
					continue;
				if (m_form->attr2 & GO_TABLE && !column->m_enables.GetAt(row))
					continue;

				if (m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT
					|| m_cellR[column->m_ref].kind == CK_BUTTON)
				{
					m_index = row * m_multiN;
					m_curpos.m_row = row+1;
					m_curpos.m_col = col+1;
					m_cellpos = m_curpos;
					changeIME(m_curpos.m_row-1, m_curpos.m_col-1);
					return true;
				}
			}
			col = m_totCols-1;
		}
	}
	else
	{
		for (; row < m_validRows; row++)
		{
			for (; col < m_totCols; col++)
			{
				column = m_columns.GetAt(col);
				attr = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[column->m_ref].attr;
				if (!column->m_visible || attr & FA_PROTECT)
					continue;
				if (m_form->attr2 & GO_TABLE && !column->m_enables.GetAt(row))
					continue;

				if (m_cellR[column->m_ref].iok == EIO_INPUT || m_cellR[column->m_ref].iok == EIO_INOUT
					|| m_cellR[column->m_ref].kind == CK_BUTTON)
				{
					m_index = row * m_multiN;
					m_curpos.m_row = row+1;
					m_curpos.m_col = col+1;
					m_cellpos = m_curpos;
					changeIME(m_curpos.m_row-1, m_curpos.m_col-1);
					return true;
				}
			}
			col = 0;
		}
	}

	if (!(m_form->attr & FA_WRAP))
	{
		if (back)
			return findCell(back, m_validRows-1, m_totCols-1);

		return findCell(back, 0, 0);
	}

	return false;
}

bool CfmGrid::findCell(CPoint pt, int& row, int& col, CRect& hRc, CRect& vRc)
{
	if (m_form->attr2 & GO_MULTI)
		return false;

	CRect	tRc, cRc, hRcx, vRcx;
	float	width;
	Ccolumn* column;
	bool	find = false;

	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);
	if (!(m_form->attr2 & GO_XHEAD))
	{
		tRc.top += (int)((m_cy+m_slit) * m_headN * m_multiN);
		if (!(m_form->attr2 & GO_X3D))
			tRc.top += (4 * m_multiN);		// head extra space
		else
			tRc.top += m_multiN;			// head extra space
	}
	if (m_vscroll)
		tRc.right = m_vscr.left;
	if (m_form->attr2 & GO_PAGE)
		tRc.right = min(tRc.right, m_pscr.left);

	hRc.SetRectEmpty();
	vRc.SetRectEmpty();
	hRcx.SetRectEmpty();
	vRcx.SetRectEmpty();
	width = (float) tRc.left;
	col = m_form->type ? 0 : m_stCol;
	for (int ii = col; ii < m_totCols; ii++)
	{
		cRc = tRc;
		if (m_form->type && ii == m_form->type)
			ii = m_stCol;

		column = m_columns.GetAt(ii);
		if (!column->m_visible)
			continue;

		cRc.left  = (int)width;
		width    += column->m_width;
		if (width > tRc.right)
			break;

		cRc.right = (int)width;
		if (ii == m_lastpos.m_col-1)
		{
			vRcx = cRc;
			if (find)
				break;
		}

		if (!find && cRc.PtInRect(pt))
		{
			vRc = cRc;
			row = findRow(pt.y, hRc, hRcx);
			if (row >= 0)
			{
				row++; col = ii+1;
				find = true;
				if (!vRcx.IsRectEmpty())
					break;
			}
		}

		if ((m_form->attr2 & GO_MULTI) && (m_cellR[column->m_ref].attr & FA_WRAP))
			width = (float)tRc.left;
	}

	if (find)
	{
		if (m_lastpos.m_col == col)
		{
			if (m_lastpos.m_row != row)
				hRc |= hRcx;
		}
		else
		{
			if (m_lastpos.m_row == row)
			{
				vRc |= vRcx;
				hRc.SetRectEmpty();
			}
			else
			{
				hRc |= hRcx;
				vRc |= vRcx;
			}
		}
		hRc.InflateRect(1, 2);
		vRc.InflateRect(1, 2);
		return true;
	}

	return false;
}

bool CfmGrid::setFgColor(int row, int col, int rgb)
{
	Ccolumn* column = getColumn(col);
	if (rgb < 0)
	{
		if ((int)column->m_itRGBs.GetAt(row) < 0)
			return false;
		rgb = (int)column->m_itRGBs.GetAt(row);
	}

	DWORD color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (row == -1)
	{
		if (color == column->m_tRGB)
			return false;

		column->m_tRGB = color;
		return true;
	}
	if (color == column->m_tRGBs.GetAt(row) && ((rgb & userCOLOR) || (!(rgb & userCOLOR) && rgb == (int)column->m_itRGBs.GetAt(row))))
		return false;

	column->m_tRGBs.SetAt(row, color);
	column->m_itRGBs.SetAt(row, (rgb & userCOLOR) ? -1 : rgb);
	return true;
}

bool CfmGrid::setBkColor(int row, int col, int rgb)
{
	Ccolumn* column = getColumn(col);
	DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	switch (row)
	{
	case -1:
		if (color == column->m_pRGB)
			return false;

		column->m_pRGB = color;
		return true;

	case -2:
		if (color == m_rRGB)
			return false;

		m_rRGB = color;
		return true;
	}

	if (color == column->m_pRGBs.GetAt(row))
		return false;

	column->m_pRGBs.SetAt(row, color);
	return true;
}

void CfmGrid::setInformation(CString info)
{
	CString	item;
	int	offs   = 0;
	int	size[] = { 1, 1, 16, 1, 4, 80 };

	for (int ii = 0; ii < sizeof(size)/sizeof(int); ii++)
	{
		item = info.Left(size[ii]);
		info = info.Mid(size[ii]);

		switch (ii)
		{
		case 0:		// mode
			_SetDataPosition(atoi(item));	break;
		case 1:		// sort
			m_dir = atoi(item);		break;
		case 2:		// symbol
			offs = findColumn(item);
			if (offs >= 0)
				m_sortc = offs;
			break;
		case 3:		// stat
			m_stat = item.GetAt(0);		break;
		case 4:		// page
			m_page = item;			break;
		case 5:		// save
			m_inits = item;			break;
		default:
			break;
		}
	}

	if (!(m_form->attr2 & GO_XHEAD))
	{
		CRect	tRc = m_pRc;
		if (!(m_form->attr2 & GO_X3D))
			tRc.DeflateRect(2, 2);
		tRc.bottom  = tRc.top + (int)((m_cy+m_slit) * m_multiN * m_headN);
		if (!(m_form->attr2 & GO_X3D))
			tRc.bottom += (4 * m_multiN);		// head extra space
		else
			tRc.bottom += m_multiN;			// head extra space
		if ((m_form->properties & PR_VISIBLE) && m_headN != 0)
			invalidateRect(&tRc, false);
	}
	if ((m_form->attr2 & GO_PAGE) && !m_pscr.IsRectEmpty())
		invalidateRect(&m_pscr, false);
}

void CfmGrid::getInformation(CString& info)
{
	int	key = 0;

	switch (m_result)
	{
	case RC_PAGEUP:
		key = 1;	break;
	case RC_PAGEDN:
		key = 2;	break;
	case RC_SCRUP:
		key = 3;	break;
	case RC_SCRDN:
		key = 4;	break;
	case RC_FIRST:
		key = 5;	break;
	case RC_SEND:
		key = 6;	break;
	default:
		key = 0;	break;
	}

	m_result = RC_NOP;
	info.Format(_T("%02d%04d%d%d%d%-16s%d%04d%-80s"),
						getVisibleRows(),			// visible Rows
						m_totRows,				// Rows
						(m_form->attr2 & GO_FLEX) ? 1 : 0,	// flexible row ?
						(m_form->attr2 & GO_TOP)  ? 1 : 2,	// RTM insert pos
						m_dir,					// sort direrction
						(m_sortc < 0) ?
						"" : (char *)m_cellR[m_sortc].name,
											// sort column
						key,					// key
						atoi(m_page),				// page
						m_inits.GetString());				// save

	if ((m_axform->m_mapH->options & (OP_OOP|OP_OOP2)) == OP_OOP2)
		info += "00";								// # of columns
}

void CfmGrid::setSortInfo(CString info)
{
	int	sortc;
	CRect	tRc;

	m_dir = atoi(info.Left(1));
	sortc = findColumn(info.Mid(1));
	if (sortc >= 0)
		m_sortc = sortc;

	if (m_form->attr2 & GO_XHEAD)
		return;

	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);
	tRc.bottom = tRc.top + (int)(m_cy * m_multiN * m_headN);
	if (!(m_form->attr2 & GO_X3D))
		tRc.bottom += (4 * m_multiN);	// head extra space
	if ((m_form->properties & PR_VISIBLE) && m_headN != 0)
		invalidateRect(&tRc, false);
}

int CfmGrid::validCell(int index, int col, int& ref)
{
	int	href, vref, stCol, rc = 0;
	CString	text, string;
	bool	left, right, top, bottom;
	Ccolumn *colx, *column = m_columns.GetAt(col);

	stCol = m_form->type ? 0 : m_stCol;
	left = right = top = bottom = false;
	if (index >= column->m_heads.GetSize())
		text = column->m_head;
	else
		text = column->m_heads.GetAt(index);

	href = ref % m_totCols;
	vref = ref / m_totCols;
	if (!col || (m_hscroll && col == stCol))
		left = true;
	else
	{
		colx = m_columns.GetAt(col-1);
		if (index >= colx->m_heads.GetSize())
			string = colx->m_head;
		else
			string = colx->m_heads.GetAt(index);

		if (text.Compare(string))
			left = true;
		else
		{
			href = col-1;
			for ( ; href > stCol; href--)
			{
				colx = m_columns.GetAt(href-1);
				if (index >= colx->m_heads.GetSize())
					string = colx->m_head;
				else
					string = colx->m_heads.GetAt(index);
				if (text.Compare(string))
					break;
			}
			href = max(0, href);
		}
	}

	if (col >= m_columns.GetUpperBound())
		right = true;
	else
	{
		colx = m_columns.GetAt(col+1);
		if (index >= colx->m_heads.GetSize())
			string = colx->m_head;
		else
			string = colx->m_heads.GetAt(index);
		if (text.Compare(string))
			right = true;
	}

	if (index <= 0 || !column->m_heads.GetUpperBound())
		top = true;
	else
	{
		string = column->m_heads.GetAt(index-1);
		if (text.Compare(string))
			top = true;
		else
		{
			vref = index-1;
			for ( ; vref > 0; vref--)
			{
				string = column->m_heads.GetAt(vref-1);
				if (text.Compare(string))
					break;
			}
			vref = max(0, vref);
		}
	}

	if (index >= column->m_heads.GetUpperBound() || !column->m_heads.GetUpperBound())
		bottom = true;
	else
	{
		string = column->m_heads.GetAt(index+1);
		if (text.Compare(string))
			bottom = true;
	}

	ref = vref * m_totCols + href;
	if (left && top)
		rc += 1;
	if (right && bottom)
		rc += 2;
	return rc;
}

bool CfmGrid::getRect(int row, int col, CRect& rect, bool head)
{
	if (row < 0 || col < 0 || m_pRc.Height() ==0 || m_pRc.Width() == 0)
		return false;

	if (!head && ((row * m_multiN < m_stRow) || (row * m_multiN > m_stRow + (m_showRows-m_headN*m_multiN))))
		return false;

	if (!(m_form->attr2 & GO_MULTI) && (col >= m_form->type && (col < m_stCol || col > m_enCol)))
		return false;

	int	ii, linecnt  = m_stRow % m_multiN;
	int	skipline = 0;
	int	colidx   = m_form->type ? 0 : m_stCol;
	Ccolumn* column  = m_columns.GetAt(col);

	if (m_form->attr2 & GO_MULTI)
	{
		for (ii = 0; ii < m_totCols; ii++)
		{
			if (m_columns.GetAt(ii)->m_line == column->m_line)
			{
				colidx = ii;
				break;
			}
		}
	}

	linecnt -= column->m_line;
	skipline = (row * m_multiN) - m_stRow;
	if (linecnt != 0)
		skipline += column->m_line;

	CRect	tRc = m_pRc;
	if (!head)
	{
		if (!(m_form->attr2 & GO_X3D))
		{
			tRc.DeflateRect(2, 2);
			if (!(m_form->attr2 & GO_XHEAD))
				tRc.top += (4 * m_multiN);	// head extra space
		}
		else if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += m_multiN;			// head extra space
		tRc.top   += (int)((m_cy+m_slit) * m_headN * m_multiN);
		tRc.top   += (int)((m_cy+m_slit) * skipline);
		tRc.bottom = tRc.top + (int)m_cy + 2;
	}

	for (ii = colidx; ii < col; ii++)
	{
		if (m_form->type && ii == m_form->type)
		{
			if (m_stCol < col)
				ii = m_stCol;
			else
				break;
		}
		if (m_columns.GetAt(ii)->m_visible)
			tRc.left += (int)m_columns.GetAt(ii)->m_width;
	}
	tRc.right = tRc.left + (int)column->m_width;
	if (ii == m_stCol)
		tRc.left++;
	if (m_vscroll)
		tRc.right = min(tRc.right, m_vscr.left);
	else if (m_form->attr2 & GO_PAGE)
		tRc.right = min(tRc.right, m_pscr.left);
	else
		tRc.right = min(tRc.right, m_pRc.right);
	if (tRc.left > tRc.right)
		return false;
	if (m_hscroll && !m_hscr.IsRectEmpty())
		tRc.bottom = min(tRc.bottom, m_hscr.top);
	tRc.bottom = min(tRc.bottom, m_pRc.bottom-2);
	if (tRc.top > tRc.bottom)
		return false;
	rect = tRc;
	return true;
}

bool CfmGrid::makeListBox(int row, int col, CRect rItem, bool head)
{
	if (m_list)
		return false;

	CRect	cRc;
	CStringArray	arr;
	CDC*	dc      = getDC();
	CFont*	font    = setFont(dc);
	Ccolumn* column = m_columns.GetAt(col);
	int	height = (int)m_fHeight + 4;	// 4 = edge(2) * 2

	arr.RemoveAll();
	if (m_cellR[column->m_ref].properties & PR_FLEXCB)
	{
		int	pos;
		CString	str, text;

		if (column->m_items.Lookup(row, str))
		{
			for ( ; !str.IsEmpty(); )
			{
				pos = str.Find('\t');
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

				if (text.GetLength() > m_cellR[column->m_ref].size)
					arr.Add(text.Mid(m_cellR[column->m_ref].size));
				else
					arr.Add(_T(""));
			}
		}
	}
	else
	{
		arr.Copy(head ? column->m_hdisplay : column->m_display);
	}

	height = (++height)/2*2;
	cRc = setRect(dc, col, rItem, arr, height);
	releaseDC(dc);

	m_list = new CcbList(this, arr, font, height, rItem);
	m_axform->DoSomething(doCOMBO, this, 1);
//	if (!m_list->Create(WS_CHILD|WS_VISIBLE, cRc, AfxGetMainWnd(), 0))
	if (!m_list->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, cRc, m_axform->m_view, 226))
	{
		m_axform->DoSomething(doCOMBO, this, 0);
		delete m_list;
		m_list = NULL;
		return false;
	}
	return true;
}

bool CfmGrid::makePopup(int col, CPoint pt)
{
	Ccolumn* column = m_columns.GetAt(col);

	if (column->m_hdisplay.GetSize() > 0)
	{
		int	base = 100, index = 0;
		CMenu *popM = new CMenu();
		if (!popM->CreatePopupMenu())
		{
			delete popM;
			return false;
		}

		for (index = 0; index < column->m_hdisplay.GetSize(); index++)
			popM->AppendMenu(MF_STRING|MF_ENABLED, base+index, column->m_hdisplay.GetAt(index));
		m_axform->m_view->ClientToScreen(&pt);
		index = popM->TrackPopupMenu(TPM_RIGHTALIGN|TPM_RETURNCMD, pt.x, pt.y, m_axform->m_view);
		delete popM;
		if (index >= base)
		{
			index -= base;
			if (index >= 0 && index < column->m_hdata.GetSize())
				column->m_hcell = column->m_hdata.GetAt(index);
			return true;
		}
	}

	return false;
}

CRect CfmGrid::setRect(CDC* dc, int col, CRect rItem, CStringArray& arr, int height)
{
	int	value, count;
	CSize	size(0,0);
	CRect	rect, tRc; // , wRc;

	value = 0;
	count = (arr.GetSize() > MAX_COMBO_LINE) ? MAX_COMBO_LINE : arr.GetSize();

	tRc = rItem;
	tRc.OffsetRect(m_size);
	tRc.top = tRc.bottom+1;
	tRc.right--;
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		size  = dc->GetOutputTextExtent(arr.GetAt(ii));
		value = max(value, size.cx);
	}
	tRc.right  = max(tRc.left+value+GetSystemMetrics(SM_CXVSCROLL)+5, tRc.right);
	tRc.bottom = (int)(tRc.top+(height+1)*count);
//	tRc.bottom = (int)(tRc.top+(m_fHeight+1)*count);

	rect = tRc;
	m_axform->m_view->GetClientRect(&tRc);
	if (rect.right > tRc.right)
		rect.OffsetRect(tRc.right-rect.right, 0);
	if (rect.bottom > tRc.bottom)
	{
		int	aboveH = min(rect.Height(), m_pRc.top - 1);
		int	belowH = tRc.bottom - (m_pRc.bottom+1);

		if (aboveH > belowH)
		{
			rect.bottom = rItem.top+m_size.cy;
			rect.top    = rect.bottom - aboveH;
		}
		else
			rect.bottom = tRc.bottom;
	}
	return rect;

	/*
	rect = tRc;
	m_axform->m_view->ClientToScreen(&tRc);
	AfxGetMainWnd()->GetClientRect(&wRc);
	AfxGetMainWnd()->ClientToScreen(&wRc);
	if (tRc.right > wRc.right)
		tRc.OffsetRect(wRc.right-tRc.right, 0);
	tRc.top    = max(tRc.top, 0);
	tRc.left   = max(tRc.left, 0);
	tRc.bottom = min(tRc.bottom, wRc.bottom);
	AfxGetMainWnd()->ScreenToClient(&tRc);
	return tRc;
	*/
}

int CfmGrid::getStart(int index, int show, int count)
{
	int	start = index;
	start = max(0, min(start, count-show+1));
	return start;
}

int CfmGrid::getIndex(int col, int row, CString data, bool force)
{
	if (col < 0)
		col = max(0, m_curpos.m_col-1);
	if (row < 0)
		row = max(0, m_curpos.m_row-1);

	int	index;
	CString	text;
	Ccolumn* column = m_columns.GetAt(col);
	DWORD	attr    = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[column->m_ref].attr;

	if (data.IsEmpty())
	{
		if ((m_data.GetSize()/m_totCols) > row)
			data = m_data.GetAt(row * m_totCols + column->m_ref);
		else
			data = CString(' ', m_cellR[column->m_ref].size);
	}

	if (m_cellR[column->m_ref].properties & PR_FLEXCB)
	{
		if (column->m_items.Lookup(row, text))
		{
			int	pos;
			for (index = 0; !text.IsEmpty(); index++)
			{
				if (!data.CompareNoCase(text.Left(m_cellR[column->m_ref].size)))
					return index;

				pos  = text.Find('\t');
				if (pos != -1)
					text = text.Mid(pos+1);
				else
					text.Empty();
			}
		}

		return (force ? -1 : 0);
	}

	for (index = 0; index < (int)m_cellR[column->m_ref].keys; index++)
	{
		text = column->m_data.GetAt(index);
		if (data.GetLength() < m_cellR[column->m_ref].size && (attr & FA_RIGHT))
		{
			text.TrimLeft();
			text.TrimRight();
		}
		if (!data.CompareNoCase(text))
			break;
	}

	if (force)
		return ((index < (int)m_cellR[column->m_ref].keys) ? index : -1);

	return ((index < (int)m_cellR[column->m_ref].keys) ? index : 0);
}

CString CfmGrid::getString(int col, int row, CString data)
{
	if (col < 0)
	{
		col = max(0, m_curpos.m_col-1);
		col = m_columns.GetAt(col)->m_ref;
	}
	if (row < 0)
		row = max(0, m_curpos.m_row-1);

	int	index;
	CString	text;
	Ccolumn* column = getColumn(col);
	DWORD	attr    = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[col].attr;

	if (data.IsEmpty())
	{
		if ((m_data.GetSize()/m_totCols) > row)
			data = m_data.GetAt(row * m_totCols + col);
		else
			data = CString(' ', m_cellR[col].size);
	}

	if (m_cellR[col].properties & PR_FLEXCB)
	{
		if (column->m_items.Lookup(row, text))
		{
			int	pos;
			CString	str;

			for (index = 0; !text.IsEmpty(); index++)
			{
				pos = text.Find('\t');
				if (pos == -1)
				{
					str = text;
					text.Empty();
				}
				else
				{
					str  = text.Left(pos++);
					text = text.Mid(pos);
				}

				if (!data.CompareNoCase(str.Left(m_cellR[col].size)))
					return str.Mid(m_cellR[col].size);
			}
		}
	}
	else
	{
		for (index = 0; index < (int)m_cellR[col].keys; index++)
		{
			text = column->m_data.GetAt(index);
			if (data.GetLength() < m_cellR[col].size && (attr & FA_RIGHT))
			{
				text.TrimLeft();
				text.TrimRight();
			}
			if (!data.CompareNoCase(text))
				return column->m_display.GetAt(index);
		}
	}

	return _T("");
}

CString	CfmGrid::getString(int col, int row, int sel, bool display)
{
	CString	data = _T("");
	Ccolumn* column = getColumn(col);
	DWORD	attr    = (m_form->attr2 & GO_TABLE) ? column->m_attrs.GetAt(row) : m_cellR[col].attr;

	if (m_cellR[col].properties & PR_FLEXCB)
	{
		if (column->m_items.Lookup(row, data))
		{
			int	pos;
			CString	text;

			for (int ii = 0; ii <= sel && !data.IsEmpty(); ii++)
			{
				pos = data.Find('\t');
				if (pos == -1)
				{
					text = data;
					data.Empty();
				}
				else
				{
					text = data.Left(pos++);
					data = data.Mid(pos);
				}
			}
			
			if (!text.IsEmpty())
				return (display ? text.Mid(m_cellR[col].size) : text.Left(m_cellR[col].size));
		}
	}
	else if (sel < (int)m_cellR[col].keys)
		return (display ? column->m_display.GetAt(sel) : column->m_data.GetAt(sel));

	return _T("");
}

int CfmGrid::calcCaret(CString data, CString edit)
{
	int pos = data.GetLength();

	if (data.IsEmpty())
		return pos;

	if (!edit.IsEmpty())
	{
		int	len,  dcnt = 0;
		
		len = pos;
		for (int ii = 0; ii < edit.GetLength(); ii++)
		{
			if (edit[ii] == '9')
			{
				pos = ii;
				if (dcnt == m_edcaretpos)
					break;
				else
				{
					if (m_edcaretpos == len)
						pos = edit.GetLength();
					dcnt++;
				}
			}
		}
	}
	else
		pos = m_edcaretpos;

	return pos;
}

int CfmGrid::getRowIndex(CPoint pt)
{
	CRect	tRc;
	int	idx, rows;

	if (m_vscr.PtInRect(pt) || (m_hscroll && m_hscr.PtInRect(pt)))
		return (-1);
	else if (m_pRc.PtInRect(pt))
	{
		tRc = m_pRc;
		tRc.bottom = tRc.top + (int)(m_cy * m_headN * m_multiN);
		if (tRc.PtInRect(pt) && m_headN != 0)
			return (-1);
	}
	else
		return (-1);

	idx  = -1;
	rows = min((m_validRows+m_headN)*m_multiN-1, m_showRows);

	for (int ii = m_headN*m_multiN; ii < rows + 1; ii++)
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
		tRc.top   += (int)((m_cy+m_slit) * ii);
		tRc.bottom = tRc.top + (int)m_cy;

		if (tRc.PtInRect(pt))
		{
			idx = m_stRow + ii - (m_headN * m_multiN);
			if (idx >= (m_totRows * m_multiN))
				idx = m_totRows * m_multiN - 1;
			break;
		}
	}
	if (idx == -1)
		return (-1);

	return (idx / m_multiN);
}

int CfmGrid::getVisibleRows()
{
	return (m_showRows - m_headN*m_multiN)/m_multiN;
}

bool CfmGrid::getMovableColumn(CPoint pt)
{
	if (m_form->attr2 & GO_MULTI)
		return false;

	CRect	tRc;
	float	width;
	Ccolumn* column;

	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);
	tRc.bottom = tRc.top + (int)(m_cy * m_headN * m_multiN);
	if (!(m_form->attr2 && GO_XHEAD))
	{
		if (!(m_form->attr2 & GO_X3D))
			tRc.bottom += (4 * m_multiN);		// head extra space
		else
			tRc.bottom += m_multiN;			// head extra space
	}
	if (!tRc.PtInRect(pt) || m_headN == 0)
		return false;

	width = (float) tRc.left;
	int	ii = m_form->type ? 0 : m_stCol;
	for ( ; ii < m_totCols; ii++)
	{
		if (m_form->type && ii == m_form->type)
			ii = m_stCol;

		column = m_columns.GetAt(ii);
		if (!column->m_visible)
			continue;

		width += column->m_width;
		if (width >= tRc.right)
			break;
		if ((float)pt.x >= width-2 && (float)pt.x <= width+2)
		{
			m_moveCol = ii;
			m_moveRc.SetRect((int)width-1, tRc.top, (int)width+1, m_pRc.bottom);
			m_moveRc.OffsetRect(m_size);
			if (!(m_form->attr2 & GO_X3D))
				m_moveRc.bottom -= 2;
			return true;
		}
		if ((m_form->attr2 & GO_MULTI) && (m_cellR[column->m_ref].attr & FA_WRAP))
			width = (float)tRc.left;
	}

	return false;
}

bool CfmGrid::isHScrollEnabled()
{
	Ccolumn* column;
	int	offs;
	float	width;

	m_totWidth   = width = 0;
	m_fixedWidth = 0;
	for (int ii = 0; ii < m_totCols; ii++)
	{
		column = m_columns.GetAt(ii);
		offs = (int)column->m_width;
		if ( (column->m_width - (double)offs) > 0.5)
			column->m_width = (float)offs + 1;
		else
			column->m_width = (float)offs;

		if (column->m_visible)
		{
			width += column->m_width;
			if (ii < m_form->type)
				m_fixedWidth += column->m_width;
		}
		if (((m_cellR[column->m_ref].attr & FA_WRAP) && m_form->attr2 & GO_MULTI) || ii == (m_totCols - 1))
		{
			m_totWidth = max(m_totWidth, width);
			width = 0;
		}
	}

//	if (m_form->attr2 & GO_MULTI)
//		m_fixedWidth = 0;
	m_totWidth -= m_fixedWidth;

	if (m_totWidth == 0)
		return false;

	return (((float)m_pRc.Width() >= (m_totWidth + m_fixedWidth)) ? false : true);
}

bool CfmGrid::isReverseCell(int row, int col)
{
	if (m_form->isEdit != FI_CELL && row == m_curpos.m_row-1 && col == m_curpos.m_col-1)
		return false;

	if (m_focus && m_form->isEdit == FI_CELL && (m_select || m_move == mvSel))
	{
		CRect	rect;

		rect.left   = max(m_curpos.m_col,  0);
		rect.top    = max(m_curpos.m_row,  0);
		rect.right  = max(m_lastpos.m_col, 0);
		rect.bottom = max(m_lastpos.m_row, 0);
		rect.NormalizeRect();
		rect.OffsetRect(-1, -1);

		if (col >= rect.left && col <= rect.right && row >= rect.top && row <= rect.bottom)
			return true;
	}

	return false;
}

void CfmGrid::refreshSel(Cpos pos, int key)
{
	if (m_lastpos.m_row <= 0 || m_lastpos.m_col <= 0)
		return;

	switch (key)
	{
	case VK_LEFT:
		if (pos.m_col <= m_curpos.m_col)
			pos.m_col = m_lastpos.m_col;
		break;
	case VK_RIGHT:
		if (pos.m_col >= m_curpos.m_col)
			pos.m_col = m_lastpos.m_col;
		break;
	case VK_UP:
		if (pos.m_row <= m_curpos.m_row)
			pos.m_row = m_lastpos.m_row;
		break;
	case VK_DOWN:
		if (pos.m_row >= m_curpos.m_row)
			pos.m_row = m_lastpos.m_row;
		break;
	}

	CRect	tRc, pRc;
	int	linecnt  = m_stRow % m_multiN;
	int	skipline = 0;
	int	colidx   = m_form->type ? 0 : m_stCol;
	Ccolumn* column  = m_columns.GetAt(pos.m_col-1);

 	if (m_form->attr2 & GO_MULTI)
	{
		for (int ii = 0; ii < m_totCols; ii++)
		{
			if (m_columns.GetAt(ii)->m_line == column->m_line)
			{
				colidx = ii;
				break;
			}
		}
	}
	linecnt -= column->m_line;
	skipline = (pos.m_row-1)*m_multiN - m_stRow;
	if (linecnt != 0)
		skipline += column->m_line;

	tRc = m_pRc;
	if (!(m_form->attr2 & GO_X3D))
	{
		tRc.DeflateRect(2, 2);
		if (!(m_form->attr2 & GO_XHEAD))
			tRc.top += (4 * m_multiN);	// head extra space
	}
	else if (!(m_form->attr2 & GO_XHEAD))
		tRc.top += m_multiN;		// head extra space
	tRc.top   += (int)((m_cy+m_slit) * m_headN * m_multiN);
	tRc.top   += (int)((m_cy+m_slit) * skipline);
	tRc.bottom = tRc.top + (int)m_cy + 3;

	for (int ii = colidx; ii < pos.m_col-1; ii++)
	{
		if (m_form->type && ii == m_form->type)
		{
			if (m_stCol < pos.m_col-1)
				ii = m_stCol;
			else
				break;
		}
		if (m_columns.GetAt(ii)->m_visible)
			tRc.left += (int)m_columns.GetAt(ii)->m_width;
	}
	tRc.right = tRc.left + (int)column->m_width;
	if (m_vscroll)
		tRc.right = min(tRc.right, m_vscr.left);
	else if (m_form->attr2 & GO_PAGE)
		tRc.right = min(tRc.right, m_pscr.left);
	else
		tRc.right = min(tRc.right, m_pRc.right);

	pRc.left = min(m_cell.left, tRc.left);
	pRc.top  = min(m_cell.top, tRc.top);
	pRc.right  = max(m_cell.right, tRc.right);
	pRc.bottom = max(m_cell.bottom, tRc.bottom);
	pRc.NormalizeRect();

	pRc.left = max(pRc.left, m_pRc.left+(int)m_fixedWidth);
	pRc.top  = max(pRc.top, m_pRc.top+(int)(m_cy * m_headN * m_multiN));
	if (m_vscroll)
		pRc.right = min(pRc.right, m_vscr.left);
	else if (m_form->attr2 & GO_PAGE)
		pRc.right = min(pRc.right, m_pscr.left);
	else
		pRc.right = min(pRc.right, m_pRc.right);
	if (m_hscroll)
		pRc.bottom = min(pRc.bottom, m_hscr.top);
	else
		pRc.bottom = min(pRc.bottom, m_pRc.bottom);
	pRc.NormalizeRect();
	invalidateRect(&pRc, false);
}

void CfmGrid::changeIME(int row, int col)
{
	DWORD	attr;
	Ccolumn* column;
	bool	han = false;

	if (row < 0 || col < 0)
		return;

	column = m_columns.GetAt(col);
	if (m_cellR[column->m_ref].iok == EIO_OUTPUT || m_cellR[column->m_ref].iok == EIO_NOP)
		return;

	if (m_form->attr2 & GO_TABLE && row >= 0 && row < column->m_attrs.GetSize())
		attr = column->m_attrs.GetAt(row);
	else
		attr = m_cellR[column->m_ref].attr;

	switch (m_cellR[column->m_ref].kind)
	{
	case CK_NORMAL:
	case CK_COMBO:
		switch (m_cellR[column->m_ref].type)
		{
		case IO_HAN:
			han = true;
			break;
		case IO_MIXED:
		case IO_ANY:
			if (m_cellR[column->m_ref].attr & FA_HAN)
			{
				han = true;
				break;
			}
		default:
			break;
		}
		break;
	default:
		return;
	}

	m_axform->DoSomething(doIME, this, han ? 1 : 0);
}

double CfmGrid::compare(bool string, CString arg1, CString arg2, int col)
{
	if (string)
	{
		if (m_cellR[col].vals[3] != NOVALUE && m_cellR[col].vals[3] > 0)
		{
			arg1 = arg1.Mid(m_cellR[col].vals[3]);
			arg2 = arg2.Mid(m_cellR[col].vals[3]);
		}
		return arg1.Compare(arg2);
	}

	CString	tmps = _T("");

	arg1.Trim(); arg2.Trim();

	char	sign;
	int	ii = 0;

	if (!arg1.IsEmpty())
	{
		ii = 0;
		sign = ' ';
		if (arg1[ii] == '-' || arg1[ii] == '+')
		{
			sign = arg1[ii];
			ii++;
		}
		else if (col >= 0 && m_cellR[col].attr & FA_CONTRAST)
		{
			switch (arg1[ii])
			{
			case '1':	// 상한
			case '+':	// 상승
			case '2':
				sign = '+'; ii++;
				break;
			case '4':	// 하한
			case '-':	// 하락
			case '5':
				sign = '-';
			default:
				ii++; break;
			}
		}

		if (arg1.GetLength() > ii)
			arg1 = arg1.Mid(ii);
		else
			arg1 = _T("");
		arg1.TrimLeft();
		tmps = _T("");
		for (ii = 0; ii < arg1.GetLength(); ii++)
		{
			if (arg1.GetAt(ii) >= '0' && arg1.GetAt(ii) <= '9')
				tmps += arg1.GetAt(ii);
		}
		if (sign != ' ')
			tmps.Insert(0, sign);
		arg1 = tmps;
	}

	if (!arg2.IsEmpty())
	{
		ii = 0;
		sign = ' ';
		if (arg2[ii] == '-' || arg2[ii] == '+')
		{
			sign = arg2[ii];
			ii++;
		}
		else if (col >= 0 && m_cellR[col].attr & FA_CONTRAST)
		{
			switch (arg2[ii])
			{
			case '1':	// 상한
			case '+':	// 상승
			case '2':
				sign = '+'; ii++;
				break;
			case '4':	// 하한
			case '-':	// 하락
			case '5':
				sign = '-';
			default:
				ii++; break;
			}
		}

		if (arg2.GetLength() > ii)
			arg2 = arg2.Mid(ii);
		else
			arg2 = _T("");
		arg2.TrimLeft();
		tmps = _T("");
		for (ii = 0; ii < arg2.GetLength(); ii++)
		{
			if (arg2.GetAt(ii) >= '0' && arg2.GetAt(ii) <= '9')
				tmps += arg2.GetAt(ii);
		}
		if (sign != ' ')
			tmps.Insert(0, sign);
		arg2 = tmps;
	}

	double	value1 = atof(arg1);
	double	value2 = atof(arg2);

	return (value1 - value2);
}

void CfmGrid::loadBitmap(int type)
{
	CString	path;

	switch (type)
	{
	case CK_CHECK:
		for (int ii = 0; ii < 4; ii++)
		{
			switch (ii)
			{
			case 0:	path.Format(_T("%s/%s/axcheck.bmp"),	LPCTSTR(m_axform->m_root), IMAGEDIR);	break;	// normal
			case 1:	path.Format(_T("%s/%s/axcheck_c.bmp"),	LPCTSTR(m_axform->m_root), IMAGEDIR);	break;	// checked
			case 2:	path.Format(_T("%s/%s/axcheck_d.bmp"),	LPCTSTR(m_axform->m_root), IMAGEDIR);	break;	// normal_disable
			case 3:	path.Format(_T("%s/%s/axcheck_cd.bmp"), LPCTSTR(m_axform->m_root), IMAGEDIR);	break;	// checked_disable
			}
			CBitmap* pic = m_axform->m_draw->SetBitmap((char *)path.operator LPCTSTR());
			if (pic)
				m_checks.SetAt(ii, pic);
		}
		break;

	case CK_COMBO:
		path.Format(_T("%s\\%s\\axcombo.bmp"), m_axform->m_root.GetString(), IMAGEDIR);
		m_pic = new Cimage(m_axform->m_draw, path);
		break;

	case -1:	// edit-columns bitmap
		path.Format(_T("%s\\%s\\axgrid_ed.bmp"), m_axform->m_root.GetString(), IMAGEDIR);
		m_epic = new Cimage(m_axform->m_draw, path);
		break;

	case -2:	// scroll images
		Cimage*	image;
		for (int ii = 0; ii < 7; ii++)
		{
			switch (ii)
			{
			case isUp:	path.Format("%s\\%s\\axScrUp.bmp", m_axform->m_root, IMAGEDIR);		break;	// Scroll Up
			case isDown:	path.Format("%s\\%s\\axScrDown.bmp", m_axform->m_root, IMAGEDIR);	break;	// Scroll Down
			case isHome:	path.Format("%s\\%s\\axScrHome.bmp", m_axform->m_root, IMAGEDIR);	break;	// Scroll Home
			case isPageUp:	path.Format("%s\\%s\\axScrPageUp.bmp", m_axform->m_root, IMAGEDIR);	break;	// Scroll Page Up
			case isPageDn:	path.Format("%s\\%s\\axScrPageDn.bmp", m_axform->m_root, IMAGEDIR);	break;	// Scroll Page Down
			case isLeft:	path.Format("%s\\%s\\axScrLeft.bmp", m_axform->m_root, IMAGEDIR);	break;	// Scroll Left
			case isRight:	path.Format("%s\\%s\\axScrRight.bmp", m_axform->m_root, IMAGEDIR);	break;	// Scroll Right
			}
			image = new Cimage(m_axform->m_draw, path);
			if (image)
				m_scrolls.SetAt(ii, image);
		}
		break;
	}
}


//-------------------------------------------------------------------
//
//	class Ccolumn
//
Ccolumn::Ccolumn()
{
	m_rts   = -1;
	m_ihead = 1;

	m_head.Empty();
	m_hcell.Empty();
	m_heads.RemoveAll();

	m_pRGBs.RemoveAll();
	m_tRGBs.RemoveAll();
	m_itRGBs.RemoveAll();

	m_edit.Empty();
	m_display.RemoveAll();
	m_data.RemoveAll();
	m_items.RemoveAll();

	m_valids.RemoveAll();
	m_attrs.RemoveAll();
	m_enables.RemoveAll();
	m_floats.RemoveAll();

	m_hdisplay.RemoveAll();
	m_display.RemoveAll();

	m_hpic = NULL;
	m_pic  = NULL;
	m_images.RemoveAll();
}

Ccolumn::~Ccolumn()
{
	m_pRGBs.RemoveAll();
	m_tRGBs.RemoveAll();
	m_itRGBs.RemoveAll();
	m_valids.RemoveAll();

	m_display.RemoveAll();
	m_data.RemoveAll();

	m_attrs.RemoveAll();
	m_enables.RemoveAll();
	m_floats.RemoveAll();

	m_hdisplay.RemoveAll();
	m_hdata.RemoveAll();

	m_items.RemoveAll();
	m_heads.RemoveAll();
	if (m_hpic)
		delete m_hpic;
	if (m_pic)
		delete m_pic;
	if (m_images.GetCount() > 0)
	{
		int	key;
		Cimage*	image;
		for (POSITION pos = m_images.GetStartPosition(); pos; )
		{
			m_images.GetNextAssoc(pos, key, (Cimage*&)image);
			delete image;
		}
		m_images.RemoveAll();
	}
}