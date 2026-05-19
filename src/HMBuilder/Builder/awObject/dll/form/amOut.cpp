// fmOut.cpp : implementation file
//

#include "stdafx.h"
#include "amOut.h"
#include "amform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamOut

IMPLEMENT_DYNCREATE(CamOut, CamBase)

CamOut::CamOut()
{
	EnableAutomation();
}

CamOut::CamOut(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
//	if (m_form->isEdit == IE_EDIT && m_form->onEdit != NOVALUE)
//		m_edit = (char *)&m_axform->m_strR[m_form->onEdit];

	m_edit = CString(m_form->editS);

	m_strR.Empty();
	m_data.Empty();
	m_tRc = m_pRc;

	//if (m_form->attr & FA_RIGHT)
	//	m_form->alignment = AL_RIGHT;
	//m_form->checked = m_form->iok;
	CString sImage = CString(m_form->str2);
	TRIM(sImage);

	CString path = FORMAT("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, sImage);
	if (!sImage.IsEmpty())
		m_form->properties |= (PR_IMAGE|PR_IMAGETEXT) ;
	if (m_form->properties & PR_IMAGE  && !sImage.IsEmpty())	
		m_pic = new Cimage(m_axform->m_pAmDraw, path);
	else 
		m_pic = NULL;

	if (m_form->attr & FA_FLOATx)
	{
		m_form->attr |= FA_FLOAT;
		m_form->offs = 0;
	}

}

CamOut::~CamOut()
{
	SAFE_DELETE(m_pic);
}


void CamOut::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamOut, CamBase)
	//{{AFX_MSG_MAP(CamOut)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamOut, CamBase)
	//{{AFX_DISPATCH_MAP(CamOut)
	/*
	DISP_PROPERTY_EX(CamOut, "Text", _getText, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CamOut, "Data", _getData, _setData, VT_BSTR)
	DISP_PROPERTY_EX(CamOut, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CamOut, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CamOut, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CamOut, "Skip", _getSkip, _setSkip, VT_BOOL)
	DISP_FUNCTION(CamOut, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamOut, "Blink", _Blink, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CamOut, "Push", _Push, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CamOut, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmOut to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CCBF17FF-B2D8-4122-A2D1-80FDD7B5BA8C}
static const IID IID_IfmOut =
{ 0xccbf17ff, 0xb2d8, 0x4122, { 0xa2, 0xd1, 0x80, 0xfd, 0xd7, 0xb5, 0xba, 0x8c } };

BEGIN_INTERFACE_MAP(CamOut, CamBase)
	INTERFACE_PART(CamOut, IID_IfmOut, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamOut message handlers

void CamOut::_Refresh() 
{
	Refresh();
}

void CamOut::_Blink(BOOL set, long color) 
{
	m_axform->DoSomething(doBLINK, this, set ? 1 : 0, (LPARAM)color);
}

void CamOut::_Push(LPCTSTR name) 
{
	m_axform->DoSomething(doPUSH, this, (WPARAM)0, (LPARAM)name);
}

void CamOut::_Clear() 
{
	ClearData();
}

LPCTSTR CamOut::GetText() 
{
	CString strResult;

	ReadData(strResult, true);
	return (LPCTSTR)strResult;
}

void CamOut::SetText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

LPCTSTR CamOut::GetData() 
{
	CString strResult;

	ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CamOut::SetData(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CamOut::GetPRgb() 
{
	return m_pRGB;
}

void CamOut::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CamOut::GetTRgb() 
{
	return m_tRGB;
}

void CamOut::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}

BOOL CamOut::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamOut::SetVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

BOOL CamOut::GetEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CamOut::SetEnable(BOOL bNewValue) 
{
	CamBase::SetEnable(bNewValue ? true : false);
}

BOOL CamOut::GetSkip() 
{
	return (m_form->attr & FA_SKIP) ? TRUE : FALSE;
}

void CamOut::SetSkip(BOOL bNewValue) 
{
	if (bNewValue)
		m_form->attr |= FA_SKIP;
	else
		m_form->attr &= ~FA_SKIP;
}

void CamOut::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());
	CSize	tSz;

#ifdef _GDI
	CFont* oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);
	
	int	ndc = dc->SaveDC();
	int	sMode;	

	setTextColor(dc);
#endif


//	if (!(m_form->properties & PR_TRANSPARENT))
	g.FillRectangle(setgBrush(), RECTG(m_pRc));
	
	m_tRc = m_pRc;
	m_tRc.left   += 3;
	m_tRc.right  -= 3;
//	if ((m_form->checked == PN_BORDER && m_form->borders != BD_NONE) || m_form->checked == PN_LINE)
	if (m_form->borders != BD_NONE)	
	{
		m_tRc.top    += 2;
		m_tRc.bottom -= 2;
	}

	sMode = dc->SetBkMode(TRANSPARENT);
	if (m_form->properties & PR_IMAGE)
	{
		m_pic->DrawPicture(g, m_pRc, m_form->alignImage, PI_DEFAULT);
		setBrush(dc, m_tRGB);
		drawPolygonText(dc, m_tRc, m_data, m_form->alignment, m_form->type);
	}
	else
	{
		setBrush(dc, m_tRGB);
		drawPolygonText(dc, m_tRc, m_data, m_form->alignment, m_form->type);

	//	switch (m_form->checked)
	//	{
	//	case PN_BORDER:
			switch (m_form->borders)
			{
			case BD_RAISED:
				drawEdge(dc, m_pRc, EDGE_RAISED, BF_RECT);	break;
			case BD_SUNKEN:
				drawEdge(dc, m_pRc, EDGE_SUNKEN, BF_RECT);	break;
			case BD_BUMP:
				drawEdge(dc, m_pRc, EDGE_BUMP, BF_RECT);	break;
			case BD_ETCHED:
				drawEdge(dc, m_pRc, EDGE_ETCHED, BF_RECT);	break;
			case BD_LINE:
				g.DrawRectangle(setgPen(GRGB(m_bRGB)), RECTG(m_pRc));	break;
			default:
				break;
			}
	//		break;

	//	case PN_LINE:		
	//#ifdef _last		
	//		g.DrawRectangle(setgPen(m_bRGB), RECTG(m_pRc));
	//#else
	//		g.DrawRectangle(setgPen(GRGB(m_bRGB)), RECTG(m_pRc));
	//#endif
	//	case PN_NONE:
	//		break;
	//	}

	//	dc->RestoreDC(ndc);
	}

#ifdef _GDI
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);
#endif
}

void CamOut::ReadData(CString& data, bool edit, int col, int row)
{
	//CamBase::ReadData(data, edit, col, row);
	int	len,  flen;

	if (edit)
	{
		len = m_data.GetLength();
		flen = m_form->size;
		if (m_form->attr & FA_COMMA)
			flen = flen + (flen - 1)/3;

		flen = max(flen, len);
		/*
		if (m_form->attr & FA_RIGHT)
		{
			data  = CString(' ' , flen - len);
			data += m_data;
		}
		else
		*/
		{
			m_strRTmp  = m_strR;
			m_strRTmp += CString(' ', flen - len);
			data = m_strRTmp;
		}
		return;
	}

	len  = m_strR.GetLength();
	flen = max(m_form->size, len);
	/*
	if (m_form->attr & FA_RIGHT)
	{
		data  = CString(' ', flen - len);
		data += m_strR;
	}
	else
	*/
	{
		m_strRTmp  = m_strR;
		m_strRTmp += CString(' ', flen - len);
		data = m_strRTmp;
	}
}

void CamOut::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_form->attr & FA_CORGB)
		doEditCORGB(data);

	makeData(data, m_form->size, m_form->attr, m_form->attr2, m_form->type);
	if (m_form->attr & FA_FLOAT)
	{
		data.TrimLeft();
		data.TrimRight();

		CString	text, tmps, str;
		int	pos;
		char	sign   = '\0';

		text = data;
		pos  = text.FindOneOf("+-");
		if (pos >= 0)
		{
			sign = text.GetAt(pos);
			text = text.Mid(pos+1);
		}

		pos = text.Find('.');
		if (pos >= 0)
		{
			tmps = text.Left(pos);
			if (m_form->offs == m_form->size)
				text = tmps;
			else
			{
				str  = tmps.Right(m_form->size -m_form->offs);
				tmps = text.Mid(pos+1);
				tmps = tmps.Left(m_form->offs);
				if (str.IsEmpty())
					str += _T("0");
				str += CString('0', m_form->offs - tmps.GetLength());
				str += tmps;
				text = str;
			}
		}
		else if (!(text.IsEmpty() && (m_form->attr & FA_ZEROSUP)))
		{
			/*
			if (m_form->attr & FA_RIGHT)
				text = text.Right(m_form->size);
			else
			*/
				text = text.Left(m_form->size);

			pos = text.GetLength() - m_form->offs;
			if (pos <= 0)
				text = CString('0', abs(pos)+1) + text;
		}
		if (sign)
			text.Insert(0, sign);
		data = text;
	}
	else
	{
		if (/*(m_form->attr & FA_RIGHT) || */m_form->alignment == AL_RIGHT)
			data = data.Right(m_form->size);
		else
			data = data.Left(m_form->size);
	}

	makeSignData(data);
	if (!m_strR.Compare(data))
		return;

	m_strR = data;
	m_strR.TrimRight();
	editData();

	if (redraw && (m_form->properties & PR_VISIBLE))
	{
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
//		else
//			invalidateRect(&m_tRc, false);
	}
}

void CamOut::ClearData(bool selectOnly)
{
	if (m_data.IsEmpty() && m_strR.IsEmpty())
		return;

	m_data.Empty();
	m_strR.Empty();

//	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_tRc, true);
//	else
//		invalidateRect(&m_tRc, false);
}

void CamOut::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
//		else
//			invalidateRect(&m_tRc, false);
	}
}

void CamOut::editData()
{
	m_data.Empty();
	if (m_strR.IsEmpty())
		return;

	int	len,  ii;
	CString	tmpS = _T("");
	m_data = m_strR;

	if (!m_edit.IsEmpty())//&& m_form->isEdit == IE_EDIT)
	{
		int	dcnt = 0;
		m_strR.TrimLeft();
		m_strR.TrimRight();
		len = m_strR.GetLength();
		if (len == 0)
			return;

		CString	format;
		/*
		if (m_form->attr & FA_RIGHT)
		{
			format.Empty();
			for (ii = m_edit.GetLength()-1; ii >= 0; ii--)
			{
				if (m_edit[ii] == '9')
					dcnt++;

				if (dcnt >= len)
					break;
			}
			format = m_edit.Mid(ii);
		}
		else
		*/
			format = m_edit;

		dcnt = 0;
		for (ii = 0; ii < format.GetLength(); ii++)
		{
			if (format[ii] == '9')
			{
				if (dcnt >= len)
					break;
				else
				{
					tmpS += m_strR[dcnt];
					dcnt++;
				}
			}
			else
				tmpS += format[ii];
		}
		m_data = tmpS;
 		return;
	}

	if (m_form->attr & FA_PASSWD)
	{
		CString	tmps = m_strR;
		tmps.TrimRight();
		m_data  = _T("");
		for (ii = 0; ii < tmps.GetLength(); ii++)
			tmpS += '*';
		m_data = tmpS;
		return;
	}

	m_data = m_strR;
	formatData(m_form->attr, m_data, (m_form->offs == m_form->size) ? -1 : (int)m_form->offs);
	
	
	if (m_form->attr & (FA_COMMA|FA_FLOAT))
	{
		bool	zero  = true;
		CString	valS  = _T("");
		CString	tmpS_ = _T("");
		int	index = 0;

		m_strR.TrimLeft();
		m_strR.TrimRight();
		if (m_strR.GetLength() == 0)
			return;

		valS = CString(' ', m_strR.GetLength());
		for (ii = 0; ii < m_strR.GetLength(); ii++)
		{
			switch (m_strR[ii])
			{
			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
			case '.':
				zero = false;
				valS.SetAt(ii, 'V');
				break;

			case '0':
			case ' ':
				if (zero)
				{
					valS.SetAt(ii, 'G');
					m_strR.SetAt(ii, ' ');
				}
				else
					valS.SetAt(ii, 'V');
				break;

			default:
				valS.SetAt(ii, 'S');
				break;
			}
		}

		if ((m_form->attr & FA_FLOAT) && (valS.Find('V')==-1) && (valS.Find('S')==-1))
			return;

		if (zero)
		{
			m_data = '0';
			return;
		}

		tmpS = m_strR;
		valS.MakeReverse();
		tmpS.MakeReverse();

		ii = 0;
		if (m_form->attr & FA_FLOAT)
		{
			for (ii = 0; ii < (int)m_form->offs; ii++)
			{
				if (ii > valS.GetLength()-1)
				{
					tmpS_ += '0';
					continue;
				}

				switch (valS[ii])
				{
				case 'V':
				case 'S':
					tmpS_ += tmpS[ii];
					break;
				case 'G':
					tmpS_ += '0';
				default:
					break;
				}
			}
			tmpS_ += '.';
			if (ii > valS.GetLength()-1)
			{	
				tmpS_ += '0';
				tmpS_.MakeReverse();
				m_data = tmpS_;
				return;
			}
			else
			{
				if (valS[ii] == 'G')
				{
					tmpS_ += '0';
					index++;
					ii++;
				}

				if (!(m_form->attr & FA_COMMA))
				{
					for ( ; ii < tmpS.GetLength(); ii++)
						tmpS_ += tmpS[ii];

					tmpS_.MakeReverse();
					m_data = tmpS_;
					return;
				}
				if (index > 0)
					ii--;
			}
			if (valS.GetLength() > ii)
				valS = valS.Mid(ii);
			else
				valS = _T("");

			if (tmpS.GetLength() > ii)
				tmpS = tmpS.Mid(ii);
			else
				tmpS = _T("");
		}
		else
		{
			ii = tmpS.Find('.');
			if (ii >= 0)
			{
				if (ii < tmpS.GetLength()-1)
				{
					ii++;
					tmpS_ += tmpS.Left(ii);
					if (valS[ii] == 'G')
					{
						tmpS_ += '0';
						index++;
					}
				}
				else if (ii == tmpS.GetLength() -1)
				{
					ii++;
					tmpS_ += tmpS.Left(ii);
					tmpS_ += '0';
					index++;
				}

				if (valS.GetLength() > ii)
					valS = valS.Mid(ii);
				else
					valS = _T("");

				if (tmpS.GetLength() > ii)
					tmpS = tmpS.Mid(ii);
				else
					tmpS = _T("");

				int	signpos = valS.Find('S');
				CString	tmpX  = valS;
				CString tmpX_ = tmpS;
				if (signpos >= 0)
				{
					tmpX  = valS.Left(signpos);
					if (valS.GetLength() > signpos)
						tmpX += valS.Mid(signpos);

					tmpX_  = tmpS.Left(signpos);
					if (tmpS.GetLength() > signpos)
						tmpX_ += tmpS.Mid(signpos);
				}
				else
				{
					tmpX  += ' ';
					tmpX_ += ' ';
				}

				valS = tmpX;
				tmpS = tmpX_;
			}
			else
			{
				if (valS.GetLength() > ii)
					valS = valS.Mid(ii);
				else
					valS = _T("");

				if (tmpS.GetLength() > ii)
					tmpS = tmpS.Mid(ii);
				else
					tmpS = _T("");
			}
		}

		for (ii = 0;  ii < valS.GetLength(); ii++)
		{
			if (ii < index)
				continue;

			if ((ii != 0) && ((ii % 3) == 0))
			{
				switch (valS[ii])
				{
				case 'G':
					tmpS_ += ' ';
					break;
				case 'V':
					tmpS_ += ',';
				default:
					break;
				}
			}
			tmpS_ += tmpS[ii];
		}

		tmpS_.MakeReverse();
		m_data = tmpS_;
	}
	
}
