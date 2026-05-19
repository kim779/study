// fmUserTab.cpp : implementation file
//

#include "stdafx.h"
#include "amUserTab.h"
#include "amform.h"
#include "../amLib/AmDraw.h"
#include "amObject.h"

//#include "../lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	hSCR		18
#define	wSCR		8
#define	nIMG		6
#define	SCR_BMP		"exarrx.bmp"

/////////////////////////////////////////////////////////////////////////////
// CamUserTab

IMPLEMENT_DYNCREATE(CamUserTab, CamBase)

CamUserTab::CamUserTab()
{
	EnableAutomation();
}

CamUserTab::CamUserTab(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	m_pic = NULL;
	m_hasfocus = false;
	m_count  = LOWORD(m_form->keys);
	m_fpRGB = m_ftRGB = 0;
	
	if (m_form->properties & PR_SORT)//PR_FIXED)
	{
		m_vcount = m_form->size;
		m_last   = m_vcount;
	}
	else 
	{		
		m_vcount = m_count;	//HIWORD(m_form->keys);
		m_last   = m_count;
	}

	if (m_vcount == 0)
		m_vcount = 1;


	m_index = m_form->checked;
	m_start  = 0;

	CString sImage;
	sImage = CString(m_form->str2);

	if (!sImage.IsEmpty())
	{
		m_form->type =  TAB_IMAGE;
	}
	
	m_scroll = false;
	m_click  = ckNone;
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();

	if (m_count <= 0)
		return;

	char*	map	= m_form->str;		//&m_axform->m_strR[m_form->vals[0]];
	char*	caption = m_form->dat;		//= &m_axform->m_strR[m_form->vals[1]];
	
	vector<string> vMAP;
	vector<string> vCAPTION;

	Split(vMAP, string(map), ',');
	Split(vCAPTION, string(caption), ',');

	for each(string s in vCAPTION)
		m_caption.Add(CSTRING(s));


	int nCount = 0;
	for each(string s in vMAP)
	{
		vector<string> vOBJECT;
		Split(vOBJECT,  s, ':');			
		
		if (s[0] == '1')
			nCount++;

		if (vOBJECT.size() == 2)
		{
			m_map.Add(CSTRING(s));
		}
		else if (vOBJECT.size() == 3)
		{
			CString s = FORMAT("%s:%s", vOBJECT[0].data(), vOBJECT[1].data());
			m_map.Add(s);
			m_object.Add(CSTRING(vOBJECT[2]));
		}
			
		vOBJECT.clear();
	}
	m_count = min(m_count, min((int)m_map.GetSize(), (int)m_caption.GetSize()));

	if (m_form->spRGB != 0 && m_form->stRGB != 0)
	{
		m_fpRGB = m_form->spRGB;	//m_axform->m_valueR[m_form->vals[3]];
		m_ftRGB = m_form->stRGB;	//m_axform->m_valueR[m_form->vals[3]+1];
	}
	else
	{
		m_fpRGB = (m_form->type == TAB_NONE) ? cDBLUE : m_form->pRGB;
		m_ftRGB = (m_form->type == TAB_NONE) ? cWHITE : m_form->tRGB;
	}
	
	m_fpRGB = (m_fpRGB & userCOLOR) ? m_fpRGB : getRGB(m_fpRGB);
	m_ftRGB = (m_ftRGB & userCOLOR) ? m_ftRGB : getRGB(m_ftRGB);

	m_type = ET_ITEM;
	m_namex = _T("");
	if (m_form->properties & PR_CHECKED)
		m_hasfocus = true;
	
	/*
	if (m_form->attr & FA_ENUM)
	{
		m_namex = m_name;
		m_path.Format("%s\\%s\\%s", m_axform->m_root, TABDIR, m_namex);
		loadTab();
		m_type = ET_TAB;
	}
	*/

	m_bForm = false;
	if (m_form->checked >= m_count)
		m_bForm = true;

	m_vertical = false;
	if (m_form->alignment == AL_LEFT || m_form->alignment == AL_RIGHT)
		m_vertical = true;

	if (m_axform->m_drawOnly && m_form->type == TAB_IMAGE)
		m_form->type = TAB_BORDER;
	
	m_images.RemoveAll();
	SetEnum(_T(""), 0, 200);
	loadSBitmap();

	int	width;
	CDC*	dc = getDC();
	
	CFont*	font = setFont(dc);
	dc->SelectObject(font);
	

	if (m_form->properties & PR_SORT)
	{
		if (m_vcount < nCount)
			m_scroll = true;
		else 
			m_scroll = false;
	}
	else 
		m_scroll = isScrolled(dc, m_last, width);

	getStartIndex(m_index);
	releaseDC(dc);
}

CamUserTab::~CamUserTab()
{
	m_map.RemoveAll();
	m_caption.RemoveAll();
	m_images.RemoveAll();
//	m_simages.DeleteImageList();
	SAFE_DELETE(m_pic);
}


void CamUserTab::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamUserTab, CamBase)
	//{{AFX_MSG_MAP(CamUserTab)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamUserTab, CamBase)
	//{{AFX_DISPATCH_MAP(CamUserTab)
	/*DISP_PROPERTY_EX(CamUserTab, "ID", _getID, _setID, VT_I4)
	DISP_PROPERTY_EX(CamUserTab, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CamUserTab, "Index", _getIndex, _setIndex, VT_I4)
	DISP_PROPERTY_EX(CamUserTab, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CamUserTab, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_FUNCTION(CamUserTab, "Reload", _Reload, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamUserTab, "GetText", _GetText, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CamUserTab, "SetText", _SetText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CamUserTab, "IsTabVisible", _IsTabVisible, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CamUserTab, "SetTabVisible", _SetTabVisible, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CamUserTab, "Change", _Change, VT_EMPTY, VTS_I4 VTS_BOOL VTS_BSTR)*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmUserTab to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {15AA3B05-68BC-4B05-9662-26A8E1EEB1C8}
static const IID IID_IfmUserTab =
{ 0x15aa3b05, 0x68bc, 0x4b05, { 0x96, 0x62, 0x26, 0xa8, 0xe1, 0xee, 0xb1, 0xc8 } };

BEGIN_INTERFACE_MAP(CamUserTab, CamBase)
	INTERFACE_PART(CamUserTab, IID_IfmUserTab, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamUserTab message handlers

void CamUserTab::_Reload() 
{
	SetEnum(_T(""), -1);
}

LPCTSTR CamUserTab::_GetText(long ID) 
{
	CString strResult;
	int	index;

	index = getIndexByID(ID);
	if (index >= 0 && index < m_caption.GetSize())
		strResult = m_caption.GetAt(index);
	else
		strResult = _T("");

	return (LPCTSTR)strResult;
}

void CamUserTab::_SetText(long ID, LPCTSTR text) 
{
	int	index = getIndexByID(ID);

	if (index >=0 && index < m_caption.GetSize())
	{
		m_caption.SetAt(index, text);
		invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
	}
}

bool CamUserTab::_IsTabVisible(long ID) 
{
	return IsTabVisible(ID);
}

void CamUserTab::_SetTabVisible(long ID, bool visible) 
{
	SetTabVisible(ID, visible);
}

LPCTSTR CamUserTab::_GetOrderInfo(bool isInitial)
{
	if (isInitial)
		return _T("Not Supported");

	return _T("Not Supported");
}

//local data = "1:0:FORM2,1:1:FORM3,1:2:FORM0,1:3:FORM1,\tname1,name2,name3,name4"
void CamUserTab::_SetOrderInfo(LPCTSTR infoString)
{
	char*	chAll	= (LPSTR)infoString;
		
	vector<string> vALL;
	vector<string> vMAP;
	vector<string> vCAPTION;

	Split(vALL, string(chAll), '\t');

	Split(vMAP, vALL[0], ',');
	Split(vCAPTION, vALL[1], ',');

	int ii = 0;
	for each(string s in vCAPTION)
		m_caption.SetAt(ii++, CSTRING(s));

	ii = 0;
	int nCount = 0;
	for each(string s in vMAP)
	{
		vector<string> vOBJECT;
		Split(vOBJECT,  s, ':');			
		
		if (s[0] == '1')
			nCount++;

		if (vOBJECT.size() == 2)
		{
			m_map.SetAt(ii, CSTRING(s));
		}
		else if (vOBJECT.size() == 3)
		{
			CString s = FORMAT("%s:%s", vOBJECT[0].data(), vOBJECT[1].data());
			m_map.SetAt(ii, s);
			m_object.SetAt(ii, CSTRING(vOBJECT[2]));
		}			
		vOBJECT.clear();
		ii++;
	}
	m_count = min(m_count, min((int)m_map.GetSize(), (int)m_caption.GetSize()));
	invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
}


void CamUserTab::_Change(long ID, bool visible, LPCTSTR text) 
{
	CString	str;
	int	index = getIndexByID(ID);

	if (index >= 0 && index < m_caption.GetSize())
	{
		str = m_map.GetAt(index);
		str.SetAt(0, visible ? '1' : '0');
		m_map.SetAt(index, str);
		m_caption.SetAt(index, text);
		invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
	}
}

long CamUserTab::GetID() 
{
	return GetCurSel();
}

long CamUserTab::GetHitpos()
{
	return 0;
}

void CamUserTab::SetID(long nNewValue) 
{
	SetCurSel(nNewValue);
}

LPCTSTR CamUserTab::GetText() 
{
	CString strResult;

	if (m_index >= 0 && m_index < m_map.GetSize() && m_index < m_caption.GetSize())
		strResult = m_caption.GetAt(m_index);
	else
		strResult = _T("");

	return (LPCTSTR)strResult;
}

void CamUserTab::SetText(LPCTSTR lpszNewValue) 
{	
	char*	caption = (LPSTR)lpszNewValue;		//= &m_axform->m_strR[m_form->vals[1]];
	
	vector<string> vCAPTION;

	Split(vCAPTION, string(caption), ',');

	int ii = 0;
	for each(string s in vCAPTION)
		m_caption.SetAt(ii++, CSTRING(s));

	invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);

	//if (m_index >= 0 && m_index < m_map.GetSize() && m_index < m_caption.GetSize())
	//	m_caption.SetAt(m_index, lpszNewValue);
}

LPCTSTR CamUserTab::GetData() 
{
	CString strResult;
	
	ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CamUserTab::SetData(LPCTSTR lpszNewValue) 
{
	char*	map	= (LPSTR)lpszNewValue;		//&m_axform->m_strR[m_form->vals[0]];	
	vector<string> vMAP;

	Split(vMAP, string(map), ',');		
	int nCount = 0;
	int ii = 0;
	for each(string s in vMAP)
	{
		vector<string> vOBJECT;
		Split(vOBJECT,  s, ':');			
		
		if (s[0] == '1')
			nCount++;

		if (vOBJECT.size() == 2)
		{
			m_map.SetAt(ii, CSTRING(s));
		}
		else if (vOBJECT.size() == 3)
		{
			CString s = FORMAT("%s:%s", vOBJECT[0].data(), vOBJECT[1].data());
			m_map.SetAt(ii, s);
			m_object.SetAt(ii, CSTRING(vOBJECT[2]));
		}
		
		ii++;
		vOBJECT.clear();
	}
	m_count = min(m_count, min((int)m_map.GetSize(), (int)m_caption.GetSize()));
	
	//WriteData(lpszNewValue);
	if (m_axform->DoSomething(doIGNORE, this))
		IsChanged();
}

long CamUserTab::GetIndex() 
{
	return GetCurSel();
}

void CamUserTab::SetIndex(long nNewValue) 
{
	SetCurSel(nNewValue);
}

long CamUserTab::GetPRgb() 
{
	return m_pRGB;
}

void CamUserTab::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CamUserTab::GetTRgb() 
{
	return m_tRGB;
}

void CamUserTab::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}

bool CamUserTab::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? true : false;
}

void CamUserTab::SetVisible(bool bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

bool CamUserTab::GetEnable() 
{
	return (m_form->properties & PR_ENABLE) ? true : false;
}

void CamUserTab::SetEnable(bool bNewValue) 
{
	if (bNewValue)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;
}

void CamUserTab::Resize(float hR, float vR, bool font, bool redraw)
{
	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();

	CamBase::Resize(hR, vR, font, redraw);
	
	int	last, width;
	CDC*	dc = getDC();
	CFont*	oldfont;
	CFont*	cfont = setFont(dc);
	oldfont = dc->SelectObject(cfont);
	m_scroll = isScrolled(dc, last, width, 0);
	releaseDC(dc);
}

void CamUserTab::Reload()
{
	CamBase::Reload();

	m_scr.SetRectEmpty();
	m_cell.SetRectEmpty();
}

int CamUserTab::IsPointInRect(CPoint pt)
{
	if (!m_axform->IsFormVisible(this))
		return P_OUT;

	CRect	tRc = m_pRc;

	if ((m_form->properties & PR_VISIBLE) && (m_form->properties & PR_ENABLE) && tRc.PtInRect(pt))
		return P_IN;

	return P_OUT;
}

void CamUserTab::SetFont(int point, int style, CString fonts, bool resize)
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
			CFont*	oldfont;
			CFont*	font = setFont(dc);
			oldfont = dc->SelectObject(font);
			dc->GetTextMetrics(&metric);
			m_fHeight = (BYTE)abs(metric.tmHeight);
			m_fWidth  = (BYTE)abs(metric.tmAveCharWidth);
			releaseDC(dc);

			CRect	tRc = m_pRc;
			tRc.InflateRect(2, 2);
			invalidateRect(tRc, (m_form->type == TAB_IMAGE) ? false : true);
		}
	}
}

void CamUserTab::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE) || m_count <= 0)
		return;
	Graphics	g(dc->GetSafeHdc());
//	int width;

	SolidBrush  *pclrFont,  *pclrBg;
	StringFormat sf;
	UINT	style;
	CRect	tRc, tRc1, pRc, rc;
	CString	tmpS, tmpS_, cells;
	int	tabcnt, dx, cx = 2;
	CPoint	pts[12];
	Point	pt[12];
	CBrush*	brush;
	bool	focus_this, scroll;
	int	ii, jj, ndc = dc->SaveDC();

	int	sMode;
	CFont*	oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);

	tabcnt = 0;
	for (ii = 0; ii < m_count; ii++)
	{
		tmpS = m_map.GetAt(ii);
		if (tmpS[0] == '1')
			tabcnt++;
	}
	if (tabcnt == 0)
		return;

	dx = m_pRc.Width() / tabcnt;
	if (dx < (cx+1)*2)
		return;

	setTextColor(dc);
	setPaintColor(dc);

	pRc = m_pRc;
	scroll = isScrolled(dc, m_last, dx, m_start);
	if (m_vertical)
	{
		if (m_scroll)
			pRc.bottom -= (pRc.Width()/2);
		else
			pRc.bottom = pRc.top + dx;
		dx = pRc.top;
	}
	else
	{
		/*
		if (m_scroll)
		{
			if (m_form->type == TAB_IMAGE)
				pRc.right -= wSCR * 2;
			else
				pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
		}
		else
		*/
		pRc.right = pRc.left + dx;
		dx = pRc.left;
	}

	switch (m_form->type)
	{
	case TAB_IMAGE:
		drawTab(dc, CRect(pRc.left, pRc.top, m_pRc.right, pRc.bottom), scroll);
		if (m_scroll)
		{
			/*
			tRc = pRc;
			tRc.bottom--;
			tRc.left  = pRc.right;
			tRc.right = m_pRc.right;
			drawScroll(dc, tRc);
			*/
	
			CRect dRc = m_pRc;
			dRc.top = (long)(dRc.bottom - (m_pRc.Height() * 2.0 / 3.0)); 
			dRc.left = dRc.right - (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
			drawScroll(dc, dRc);
		}
		return;
	case TAB_BORDER:
	case TAB_OBLIQUE:
		dx++;
	default:
		break;
	}

	m_cell.SetRectEmpty();

	for (ii = m_start; ii < m_last; ii++)
	{
		tmpS = m_map.GetAt(ii);
		if (tmpS[0] != '1')
			continue;

		tmpS = m_caption.GetAt(ii);
		tRc  = pRc;

		if (m_vertical)
			tRc.top = dx;
		else
			tRc.left = dx;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		font = setFont(dc, focus_this ? 1 : -1);
		dc->SelectObject(font);		
		
		if (!scroll || ii < m_last-1)
		{
			/// 탭 사이즈....
			//dx += getSize(g, ii);
			dx += getSize(dc, ii);
			

			if (m_vertical)
			{
				if (m_pRc.right < dx)
					tRc.bottom = m_pRc.bottom;
				else
					tRc.bottom = dx;
			}
			else
			{
				if (m_pRc.right < dx)
					tRc.right = m_pRc.right;
				else
					tRc.right = dx;
			}
		}
		else if ( ii == m_last - 1)
		{
			//dx += getSize(g, ii);
			dx += getSize(dc, ii);
			if (m_vertical)
			{
				if (m_pRc.right < dx)
					tRc.bottom = m_pRc.bottom;
				else
					tRc.bottom = dx;
			}
			else
			{
				if (m_pRc.right < dx)
					tRc.right = m_pRc.right;
				else
					tRc.right = dx;
			}
		}
		
		if (!focus_this)
		{
			switch (m_form->type)
			{
			case TAB_BORDER:
				jj = 2;	break;
			case TAB_OBLIQUE:
				jj = 3;	break;
			default:
				jj = 0;	break;
			}

			switch (m_form->alignment)
			{
			case AL_LEFT:
				tRc.left += jj;			break;
			case AL_RIGHT:
				tRc.right -= jj;		break;
			case AL_BOTTOM:
				tRc.bottom -= jj;		break;
			case AL_TOP:
			default:
				tRc.top += jj;			break;
			}
		}

		switch (m_form->type)
		{
		case TAB_NONE:
			if (!scroll || ii < m_last-1)
				tRc.right--;
			tRc.bottom--;
			drawNoneTab(dc, tRc, tRc1, cx, focus_this, style, sf);
		//	pclrBg = setgBrush();
			pclrFont = setgFBrush();
			break;

		case TAB_OBLIQUE:
			tRc1 = tRc;
			tRc.bottom--;
			dc->SetPolyFillMode(ALTERNATE);
		/*
			pts[0].x = tRc.left;	pts[0].y = tRc.bottom;
			pts[1].x = tRc.left;	pts[1].y = tRc.top+3;
			pts[2].x = tRc.left+3;	pts[2].y = tRc.top;
			pts[3].x = tRc.right-4;	pts[3].y = tRc.top;
			pts[4].x = tRc.right-1;	pts[4].y = tRc.top+3;
			pts[5].x = tRc.right-1;	pts[5].y = tRc.bottom;
			pts[6].x = pts[5].x-1;	pts[6].y = tRc.bottom;
			pts[7].x = pts[6].x;	pts[7].y = tRc.top+3;
			pts[8].x = pts[3].x;	pts[8].y = pts[3].y+1;
			pts[9].x = pts[2].x;	pts[9].y = pts[8].y;
			pts[10].x = pts[1].x+1;	pts[10].y = pts[1].y;
			pts[11].x = pts[0].x+1;	pts[11].y = pts[0].y;
		*/

			pt[0].X = tRc.left;		pt[0].Y = tRc.bottom;
			pt[1].X = tRc.left;		pt[1].Y = tRc.top+3;
			pt[2].X = tRc.left+3;	pt[2].Y = tRc.top;
			pt[3].X = tRc.right-4;	pt[3].Y = tRc.top;
			pt[4].X = tRc.right-1;	pt[4].Y = tRc.top+3;
			pt[5].X = tRc.right-1;	pt[5].Y = tRc.bottom;
			pt[6].X = pt[5].X-1;	pt[6].Y = tRc.bottom;
			pt[7].X = pt[6].X;		pt[7].Y = tRc.top+3;
			pt[8].X = pt[3].X;		pt[8].Y = pt[3].Y+1;
			pt[9].X = pt[2].X;		pt[9].Y = pt[8].Y;
			pt[10].X = pt[1].X+1;	pt[10].Y = pt[1].Y;
			pt[11].X = pt[0].X+1;	pt[11].Y = pt[0].Y;

			//setPen(dc, getRGB(cBLACK));
			//setBrush(dc, getRGB(cBLACK));
			//dc->Polygon(pts, 12);

			g.DrawPolygon(setgPen(clBLACK), pt, 12);
			g.FillPolygon(setgBrush(clBLACK), pt, 12, FillModeAlternate);


			if (scroll && ii == m_last-1)
			{
				sf.SetAlignment(StringAlignmentNear);
				style |= DT_LEFT;
				tmpS = _T(" ") + tmpS;
			}
			else
			{
				sf.SetAlignment(StringAlignmentCenter);
				style |= DT_CENTER;
			}

			if (!focus_this)
			{
				pt[0].X++;
				pt[1].X++;
				pt[2].Y++;
				pt[3].Y++;
				pt[4].X = pt[8].X;	pt[4].Y = pt[8].Y+1;
				pt[5].X = pt[9].X;	pt[5].Y = pt[9].Y+1;
				pt[6].X = pt[10].X+1;	pt[6].Y = pt[10].Y;
				pt[7].X = pt[11].X+1;	pt[7].Y = pt[11].Y;
				//setPen(dc, getRGB(cYWHITE));
				//setBrush(dc, getRGB(cYWHITE));
				//dc->Polygon(pts, 8);
				g.DrawPolygon(setgPen(clYWHITE), pt, 8);
				g.FillPolygon(setgBrush(clYWHITE), pt, 8, FillModeAlternate);


				pt[0] = pt[7];
				pt[1] = pt[6];
				pt[2] = pt[5];
				pt[3] = pt[4];
				pt[4].X = tRc.right-2;	pt[4].Y = tRc.top+3;
				pt[5].X = pt[4].X;		pt[5].Y = tRc.bottom;
				//setPen(dc, m_pRGB);
				//setBrush(dc, m_pRGB);
				//dc->Polygon(pts, 6);
				g.DrawPolygon(setgPen(GRGB(m_pRGB)), pt, 6);
				g.FillPolygon(setgBrush(GRGB(m_pRGB)), pt, 6, FillModeAlternate);

				pt[0].X = pt[3].X+1;	pt[0].Y = tRc.top+2;
				pt[1].X = pt[0].X+1;	pt[1].Y = pt[0].Y+1;
			//	setPen(dc, getRGB(cEDGE));
			//	setBrush(dc, getRGB(cEDGE));
			//	dc->Polygon(pts, 2);
				g.DrawPolygon(setgPen(clEDGE), pt, 2);
				g.FillPolygon(setgBrush(clEDGE), pt, 2, FillModeAlternate);

				pt[0].X = pt[1].X;	pt[0].Y++;
				pt[1].X = pt[0].X;	pt[1].Y = tRc.bottom;
				//dc->Polygon(pts, 2);
				g.DrawPolygon(setgPen(clEDGE), pt, 2);
				g.FillPolygon(setgBrush(clEDGE), pt, 2, FillModeAlternate);

				pt[0].X = tRc.left;		pt[0].Y = tRc.bottom-1;
				pt[1].X = tRc.right-1;	pt[1].Y = tRc.bottom-1;
				pt[2].X = tRc.right-1;	pt[2].Y = tRc.bottom;
				pt[3].X = tRc.left;		pt[3].Y = tRc.bottom;
				//setPen(dc, getRGB(cYWHITE));
				//setBrush(dc, getRGB(cYWHITE));
				//dc->Polygon(pts, 4);
				g.DrawPolygon(setgPen(clYWHITE), pt, 4);
				g.FillPolygon(setgBrush(clYWHITE), pt, 4, FillModeAlternate);

				pt[2].Y--;
				pt[3].Y--;
				//setPen(dc, getRGB(cBOTTOM));
				//setBrush(dc, getRGB(cBOTTOM));
				//dc->Polygon(pts, 4);
				g.DrawPolygon(setgPen(clBOTTOM), pt, 4);
				g.FillPolygon(setgBrush(clBOTTOM), pt, 4, FillModeAlternate);

				setTextColor(dc, m_tRGB);
				setPaintColor(dc, m_pRGB);
				
				pclrFont = setgBrush(GRGB(m_tRGB));
				pclrBg   = setgBrush(GRGB(m_pRGB));
				
				tRc1.DeflateRect(2, 3, 2, 2);
				break;
			}

			pt[0].X++;
			pt[1].X++;				pt[1].Y++;
			pt[2].X += 1;			pt[2].Y++;
			pt[3].X--;				pt[3].Y++;
			pt[4].X--;				pt[4].Y++;
			pt[5].X = pt[4].X;		pt[5].Y = pt[4].Y+1;
			pt[6].X = pt[3].X;		pt[6].Y = pt[3].Y+1;
			pt[7].X = pt[2].X;		pt[7].Y = pt[2].Y+1;
			pt[8].X = pt[1].X+1;	pt[8].Y = pt[1].Y;
			pt[9].X = pt[8].X;		pt[9].Y = pt[0].Y;
			//setPen(dc, getRGB(cYWHITE));
			//setBrush(dc, getRGB(cYWHITE));
			//dc->Polygon(pts, 10);
			g.DrawPolygon(setgPen(clYWHITE), pt, 10);
			g.FillPolygon(setgBrush(clYWHITE), pt, 10, FillModeAlternate);

			pt[0] = pt[9];
			pt[1] = pt[8];
			pt[2] = pt[7];
			pt[3].X = pt[6].X;		pt[3].Y = pt[6].Y;
			pt[4].X = pt[3].X+2;	pt[4].Y = pt[3].Y+2;
			pt[5].X = tRc.right-3;	pt[5].Y = pt[0].Y;
			//setPen(dc, m_fpRGB);
			//setBrush(dc, m_fpRGB);
			//dc->Polygon(pts, 6);
			g.DrawPolygon(setgPen(GRGB(m_fpRGB)), pt, 6);
			g.FillPolygon(setgBrush(GRGB(m_fpRGB)), pt, 6, FillModeAlternate);

			pt[0].X--;				pt[0].Y = pt[1].Y-1;
			pt[1].X = tRc.left+3;	pt[1].Y = pt[2].Y-1;
			//dc->Polygon(pts, 2);
			g.DrawPolygon(setgPen(GRGB(m_fpRGB)), pt, 2);
			g.FillPolygon(setgBrush(GRGB(m_fpRGB)), pt, 2, FillModeAlternate);

			pt[0].X = pt[3].X+1;	pt[0].Y = pt[1].Y;
			pt[1].X = pt[0].X+1;	pt[1].Y = pt[0].Y+1;
//			dc->Polygon(pts, 2);
			g.DrawPolygon(setgPen(GRGB(m_fpRGB)), pt, 2);
			g.FillPolygon(setgBrush(GRGB(m_fpRGB)), pt, 2, FillModeAlternate);


			pt[0].X = pt[1].X+1;	pt[0].Y = pt[1].Y+1;
			pt[1].X = pt[0].X;		pt[1].Y = tRc.bottom;
			//setPen(dc, getRGB(cEDGE));
			//setBrush(dc, getRGB(cEDGE));
//			dc->Polygon(pts, 2);
			g.DrawPolygon(setgPen(clEDGE), pt, 2);
			g.FillPolygon(setgBrush(clEDGE), pt, 2, FillModeAlternate);

			pt[0].X = tRc.left;		pt[0].Y = tRc.bottom;
			pt[1].X = pt[0].X+1;	pt[1].Y = pt[0].Y;
	//		setPen(dc, getRGB(cYWHITE));
	//		setBrush(dc, getRGB(cYWHITE));
	//		dc->Polygon(pts, 2);
			g.DrawPolygon(setgPen(clYWHITE), pt, 2);
			g.FillPolygon(setgBrush(clYWHITE), pt, 2, FillModeAlternate);

			pt[0].X = tRc.right-2;
			pt[1].X = pt[0].X+2;
			//dc->Polygon(pts, 2);
			g.DrawPolygon(setgPen(clYWHITE), pt, 2);
			g.FillPolygon(setgBrush(clYWHITE), pt, 2, FillModeAlternate);

			pt[0].X++;				pt[0].Y--;
			pt[1].X = tRc.right;	pt[1].Y = pt[0].Y+1;
			//setPen(dc, getRGB(cBOTTOM));
			//setBrush(dc, getRGB(cBOTTOM));
			//dc->Polygon(pts, 2);
			g.DrawPolygon(setgPen(clBOTTOM), pt, 2);
			g.FillPolygon(setgBrush(clBOTTOM), pt, 2, FillModeAlternate);

			setTextColor(dc, m_ftRGB);
			//setPaintColor(dc, m_fpRGB);
			pclrFont = setgBrush(GRGB(m_ftRGB));
			pclrBg   = setgBrush(GRGB(m_fpRGB));
			
			tRc1.DeflateRect(2, 5, 1, 0);
			break;

		case TAB_RECT:
			sf.SetLineAlignment(StringAlignmentCenter);
			sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
			sf.SetHotkeyPrefix(HotkeyPrefixNone);
			sf.SetAlignment(StringAlignmentNear);
			style = DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX;
	
			setPen(dc, focus_this ? m_fpRGB : m_pRGB);
			brush = setBrush(dc, focus_this ? m_fpRGB : m_pRGB);
			dc->SetPolyFillMode(ALTERNATE);

			pt[0].X = tRc.left;			pt[0].Y = tRc.bottom;
			pt[1].X = tRc.left;			pt[1].Y = tRc.top;
			
			if (m_scroll && ii == m_last-1 && scroll)
			{
				pt[2].X = tRc.right;		pt[2].Y = tRc.top;
				pt[3].X = tRc.right;		pt[3].Y = tRc.bottom;
				//dc->Polygon(pts, 4);
				g.DrawPolygon(setgPen(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pt, 4);
				g.FillPolygon(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pt, 4, FillModeAlternate);
			}
			else
			{
				pt[2].X = tRc.right-(tRc.Height()/2);	pt[2].Y = tRc.top;
				pt[3].X = tRc.right;					pt[3].Y = tRc.top + (tRc.Height()/2);
				pt[4].X = tRc.right;					pt[4].Y = tRc.bottom;
				g.DrawPolygon(setgPen(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pt, 5);
				g.FillPolygon(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pt, 5, FillModeAlternate);

				//dc->Polygon(pts, 5);
			}

			setPen(dc, getRGB(cBLACK));
			brush = setBrush(dc, getRGB(cBLACK));
			dc->SetPolyFillMode(ALTERNATE);
			pt[0].X = tRc.left;			pt[0].Y = tRc.bottom;
			pt[1].X = tRc.left;			pt[1].Y = tRc.top;
			if (m_scroll && ii == m_last-1 && scroll)
			{
				// 이부분 비교...
				pt[2].X = tRc.right;		pt[2].Y = tRc.top;
				pt[3] = pt[1];
				//dc->Polygon(pts, 4);
				g.DrawPolygon(setgPen(clBLACK), pt, 4);
				g.FillPolygon(setgBrush(clBLACK), pt, 4, FillModeAlternate);
			}
			else
			{
				pt[2].X = tRc.right-(tRc.Height()/2);	pt[2].Y = tRc.top;
				pt[3].X = tRc.right;					pt[3].Y = tRc.top + (tRc.Height()/2);
				pt[4] = pt[2];
				pt[5] = pt[1];
				//dc->Polygon(pts, 6);
				g.DrawPolygon(setgPen(clBLACK), pt, 6);
				g.FillPolygon(setgBrush(clBLACK), pt, 6, FillModeAlternate);
			}

			setPen(dc, getRGB(cYWHITE));
			brush = setBrush(dc, getRGB(cYWHITE));
			dc->SetPolyFillMode(ALTERNATE);
			pt[0].X++;		pt[0].Y--;
			pt[1].X++;		pt[1].Y++;
			pt[2].X--;		pt[2].Y++;
			if (m_scroll && ii == m_last-1 && scroll)
			{
				pt[3] = pt[1];
				//dc->Polygon(pts, 4);
				g.DrawPolygon(setgPen(clYWHITE), pt, 4);
				g.FillPolygon(setgBrush(clYWHITE), pt, 4, FillModeAlternate);
			}
			else
			{
				pt[3].X--;		pt[3].Y++;
				pt[4] = pt[2];
				pt[5] = pt[1];
				//dc->Polygon(pts, 6);
				g.DrawPolygon(setgPen(clYWHITE), pt, 6);
				g.FillPolygon(setgBrush(clYWHITE), pt, 6, FillModeAlternate);
			}

			setPen(dc, getRGB(cBLACK));
			brush = setBrush(dc, getRGB(cBLACK));
			dc->SetPolyFillMode(ALTERNATE);
			if (focus_this)
			{
				pt[0].X = tRc.right;	pt[0].Y = tRc.bottom;
				pt[1].X = tRc.right;	pt[1].Y = tRc.top + (tRc.Height()/2);
				//dc->Polygon(pts, 2);
				g.DrawPolygon(setgPen(clBLACK), pt, 2);
				g.FillPolygon(setgBrush(clBLACK), pt, 2, FillModeAlternate);
			}
			else
			{
				pt[0].X = tRc.left;		pt[0].Y = tRc.bottom;
				pt[1].X = tRc.right;	pt[1].Y = tRc.bottom;
				if (m_scroll && ii == m_last-1 && scroll)
				{	
					//dc->Polygon(pts, 2);
					g.DrawPolygon(setgPen(clBLACK), pt, 2);
					g.FillPolygon(setgBrush(clBLACK), pt, 2, FillModeAlternate);
				}
				else
				{
					pt[2].X = tRc.right;	pt[2].Y = tRc.top + (tRc.Height()/2);
					pt[3] = pt[1];
					pt[4] = pt[0];
					//dc->Polygon(pts, 5);
					g.DrawPolygon(setgPen(clBLACK), pt, 5);
					g.FillPolygon(setgBrush(clBLACK), pt, 5, FillModeAlternate);
				}
			}

			setPen(dc, getRGB(cEDGE));
			brush = setBrush(dc, getRGB(cEDGE));
			dc->SetPolyFillMode(ALTERNATE);
			if (focus_this)
			{
				pt[0].X--;
				pt[1].X--;		pt[1].Y++;
				//dc->Polygon(pts, 2);
				g.DrawPolygon(setgPen(clEDGE), pt, 2);
				g.FillPolygon(setgBrush(clEDGE), pt, 2, FillModeAlternate);
			}
			else
			{
				pt[0].X++;		pt[0].Y--;
				pt[1].X--;		pt[1].Y--;
				if (m_scroll && ii == m_last-1 && scroll)
				{
					//dc->Polygon(pts, 2);
					g.DrawPolygon(setgPen(clEDGE), pt, 2);
					g.FillPolygon(setgBrush(clEDGE), pt, 2, FillModeAlternate);
				}
				else
				{
					pt[2].X--;		pt[2].Y++;
					pt[3] = pt[1];
					pt[4] = pt[0];
					//dc->Polygon(pts, 5);
					g.DrawPolygon(setgPen(clEDGE), pt, 5);
					g.FillPolygon(setgBrush(clEDGE), pt, 5, FillModeAlternate);
				}
			}
			tRc1 = tRc;
			tRc1.top+=2;
			tmpS = _T(" ") + tmpS;
			setTextColor(dc, focus_this ? m_ftRGB : m_tRGB);
			setPaintColor(dc, focus_this ? m_fpRGB : m_pRGB);

			pclrFont = setgBrush(focus_this ? GRGB(m_ftRGB) : GRGB(m_tRGB));
			pclrBg   = setgBrush(focus_this ? GRGB(m_ftRGB) : GRGB(m_tRGB));
			break;

		case TAB_BORDER:
		default:
			switch (m_form->alignment)
			{
			case AL_LEFT:
				sf.SetLineAlignment(StringAlignmentNear);
				sf.SetHotkeyPrefix(HotkeyPrefixNone);
				sf.SetAlignment(StringAlignmentCenter);

				style = DT_CENTER|DT_TOP|DT_NOPREFIX;

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(0, 1);
				else
					tRc1.DeflateRect(0, 2);
				tRc1.right = tRc1.left+1;
				//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));

				tRc1.OffsetRect(1, 0);
				//dc->FillSolidRect(tRc1, getRGB(cYWHITE));		// left				
				g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));

				tRc1 = tRc;
				if (focus_this)
					tRc1.left++;
				else
					tRc1.left += 2;
				tRc1.bottom = tRc1.top+1;
				//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));
				tRc1.OffsetRect(0, 1);
				//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));

				if (focus_this)
				{
					tRc1.left = tRc1.right-1;
					tRc1.OffsetRect(0, -1);
//					dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));

				}
				else
				{
					tRc1.left--;
					tRc1.right = tRc1.left+1;
					//dc->FillSolidRect(tRc1, getRGB(cGRAY));
					g.FillRectangle(setgBrush(clGRAY), RECTGA(tRc1));

				}							// top

				if (!(scroll && ii == m_last-1))
				{
					tRc1 = tRc;
					if (focus_this)
						tRc1.left++;
					else
						tRc1.left += 2;
					tRc1.top = tRc1.bottom-1;
					//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBLACK) : getRGB(cBOTTOM));
					g.FillRectangle(setgBrush(focus_this ? clBLACK : clBOTTOM), RECTGA(tRc1));
	
					if (focus_this)
					{
						tRc1.left = tRc1.right-1;
						//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
						g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
					}
					else
					{
						tRc1.OffsetRect(-1, -1);
						tRc1.right = tRc1.left+1;
						//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
						g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
					}
				}							// bottom

				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.left = tRc1.right - 2;
					//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
					tRc1.right--;
					//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
				}							// right

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(2, 2, 0, 1);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				//dc->FillSolidRect(tRc1, focus_this ? m_fpRGB : m_pRGB); // text rect
				g.FillRectangle(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), RECTGA(tRc1));
		
				break;

			case AL_RIGHT:
				sf.SetLineAlignment(StringAlignmentNear);
				sf.SetHotkeyPrefix(HotkeyPrefixNone);
				sf.SetAlignment(StringAlignmentCenter);
				style = DT_CENTER|DT_TOP|DT_NOPREFIX;

				tRc1 = tRc;				
				if (focus_this)
					tRc1.right--;
				else
					tRc1.right -= 2;
				tRc1.bottom = tRc1.top+1;
				//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));
				tRc1.OffsetRect(0, 1);
				//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));

				if (focus_this)
				{
					tRc1.OffsetRect(0, -1);
					tRc1.right = tRc1.left+1;
					//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
				}
				else
				{
					tRc1.left = tRc1.right;
					tRc1.right++;
					//dc->FillSolidRect(tRc1, getRGB(cGRAY));
					g.FillRectangle(setgBrush(clGRAY), RECTGA(tRc1));
				}							// top

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(0, 1);
				else
					tRc1.DeflateRect(0, 2);
				tRc1.left = tRc1.right-1;
				//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));
				tRc1.OffsetRect(1, 0);
				//dc->FillSolidRect(tRc1, getRGB(cYWHITE));		// right
				g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));

				if (!(scroll && ii == m_last-1))
				{
					tRc1 = tRc;
					if (focus_this)
						tRc1.right--;
					else
						tRc1.right -= 2;
					tRc1.top = tRc1.bottom-1;
					//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBLACK) : getRGB(cBOTTOM));
					g.FillRectangle(setgBrush(focus_this ? clBLACK : clBOTTOM), RECTGA(tRc1));
					if (focus_this)
					{
						tRc1.right = tRc1.left+1;
						//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
						g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
					}
					else
					{
						tRc1.OffsetRect(0, -1);
						tRc1.left = tRc1.right;
						tRc1.right++;
						//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
						g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
					}
				}							// bottom

				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.right = tRc1.left + 2;
					//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
					tRc1.right--;
					//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
				}							// left

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(0, 2, 1, 1);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				//dc->FillSolidRect(tRc1, focus_this ? m_fpRGB : m_pRGB); // text rect
				g.FillRectangle(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), RECTGA(tRc1));
				break;

			case AL_BOTTOM:
				sf.SetLineAlignment(StringAlignmentCenter);
				sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
				sf.SetHotkeyPrefix(HotkeyPrefixNone);
				//sf.SetAlignment(StringAlignmentNear);
				
				style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

				tRc1 = tRc;
				if (focus_this)
					tRc1.bottom--;
				else
					tRc1.bottom -= 2;
				tRc1.right = tRc1.left+2;
				//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
				tRc1.right--;
				//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));	// left
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));

				if (scroll && ii == m_last-1)
				{
					sf.SetAlignment(StringAlignmentNear);
					style |= DT_LEFT;
					tmpS = _T(" ") + tmpS;
				}
				else
				{	
					sf.SetAlignment(StringAlignmentCenter);
					style |= DT_CENTER;
					tRc1 = tRc;
					if (focus_this)
						tRc1.bottom--;
					else
						tRc1.bottom -= 2;
					tRc1.left = tRc1.right-1;
					//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBLACK) : getRGB(cBOTTOM));
					g.FillRectangle(setgBrush(focus_this ? clBLACK : clBOTTOM), RECTGA(tRc1));

					if (focus_this)
					{
						tRc1.bottom = tRc1.top+1;
						//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
						g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
					}
					else
					{
						tRc1.right  = tRc1.left;
						tRc1.left--;
						tRc1.top    = tRc1.bottom;
						tRc1.bottom = tRc1.top+1;
						//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
						g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
					}
				}							// right

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(1, 0);
				else
					tRc1.DeflateRect(2, 0);
				tRc1.top = tRc1.bottom-1;
				//dc->FillSolidRect(tRc1, focus_this ? getRGB(cBOTTOM) : getRGB(cGRAY));
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));
				tRc1.bottom = tRc1.top;
				tRc1.top--;
				//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
				g.FillRectangle(setgBrush(cYWHITE), RECTGA(tRc1));
				if (!focus_this)
				{
					tRc1.left--;
					tRc1.right = tRc1.left+1;
					//dc->FillSolidRect(tRc1, getRGB(cGRAY));
					g.FillRectangle(setgBrush(clGRAY), RECTGA(tRc1));
				}							// bottom

				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.bottom = tRc1.top+1;
					//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
					g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
			
					tRc1.top = tRc1.bottom;
					tRc1.bottom = tRc1.top+1;
					//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
					g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
			
				}							// top

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(2, 0, 1, 1);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				//dc->FillSolidRect(tRc1, focus_this ? m_fpRGB : m_pRGB); // text rect
				g.FillRectangle(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), RECTGA(tRc1));
				break;

			case AL_TOP:
			default:	
				sf.SetLineAlignment(StringAlignmentCenter);
				sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
				sf.SetHotkeyPrefix(HotkeyPrefixNone);
				
				style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

				tRc1 = tRc;
				if (focus_this)
					tRc1.top++;
				else
					tRc1.top += 2;
				tRc1.right = tRc1.left+2;
			
				g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
				tRc1.right = tRc1.left+1;
				tRc1.bottom--;
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));

				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(1, 0);
				else
					tRc1.DeflateRect(2, 0);
				
				tRc1.bottom = tRc1.top+1;
				g.FillRectangle(setgBrush(focus_this ? clBOTTOM : clGRAY), RECTGA(tRc1));
	
				tRc1.top = tRc1.bottom;
				tRc1.bottom++;
				g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
			
				if (!focus_this)
				{
					tRc1.left  = tRc.left+1;
					tRc1.right = tRc1.left+1;
					g.FillRectangle(setgBrush(clGRAY), RECTGA(tRc1));
				}							// top

				
			//	if (scroll && ii == m_last-1)
			//	{
			//		sf.SetAlignment(StringAlignmentNear);
			//		style |= DT_LEFT;
			//		tmpS = _T(" ") + tmpS;
			//	}
			//	else
				{
					sf.SetAlignment(StringAlignmentCenter);
					style |= DT_CENTER;
					tRc1 = tRc;
					if (focus_this)
						tRc1.top++;
					else
						tRc1.top += 2;
					tRc1.left = tRc1.right-1;
					g.FillRectangle(setgBrush(focus_this ? clBLACK : clBOTTOM), RECTGA(tRc1));
	
					if (focus_this)
					{
						tRc1.top = tRc1.bottom-1;
						g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
					}
					else
					{
						tRc1.right  = tRc1.left;
						tRc1.left--;
						tRc1.bottom = tRc1.top;
						tRc1.top--;
						g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
					}
				}							// right

				
				if (!focus_this)
				{
					tRc1 = tRc;
					tRc1.top = tRc1.bottom-2;
					g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));
					tRc1.bottom = tRc1.top+1;
					g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));
				}							// bottom

				
				tRc1 = tRc;
				if (focus_this)
					tRc1.DeflateRect(2, 2, 1, 0);
				else
					tRc1.DeflateRect(2, 2, 2, 2);
				g.FillRectangle(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), RECTGA(tRc1));				
				break;
				
			}
			setTextColor(dc, focus_this ? m_ftRGB : m_tRGB);
		//	setPaintColor(dc, focus_this ? m_fpRGB : m_pRGB);

		//	focus_this = true;
			pclrFont = setgBrush(focus_this ? GRGB(m_ftRGB) : GRGB(m_tRGB));
			pclrBg   = setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB));
			break;
		}
				
		sMode = dc->SetBkMode(TRANSPARENT);
		if (m_vertical)
			drawVText(dc, tmpS, tRc1, style, sf);
		else
		{
			tRc1.DeflateRect(2, 0);
			dc->DrawText(tmpS, tRc1, style);
			//g.DrawString(tmpS.AllocSysString(), -1, setgFont(focus_this ? 1 : 0), RECTF(tRc1), &sf, pclrFont);
		}
	}

	if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
	{
		switch (m_form->alignment)
		{
		case AL_LEFT:
			tRc1 = m_pRc;
			tRc1.left   = tRc1.right - 1;
			tRc1.bottom = tRc1.top + 1;
		//	dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			g.FillRectangle(setgBrush(clYWHITE), RECTG(tRc1));
			tRc1.OffsetRect(-1, 0);
//			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	// top margin
			g.FillRectangle(setgBrush(clBOTTOM), RECTG(tRc1));
			tRc1.top    = tRc.bottom;
			tRc1.bottom = m_pRc.bottom;
			//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			g.FillRectangle(setgBrush(clBOTTOM), RECTG(tRc1));
			tRc1.OffsetRect(1, 0);
			//dc->FillSolidRect(tRc1, getRGB(cYWHITE));	// bottom margin
			g.FillRectangle(setgBrush(clYWHITE), RECTG(tRc1));
			break;

		case AL_RIGHT:
			tRc1 = m_pRc;
			tRc1.right  = tRc1.left + 1;
			tRc1.bottom = tRc1.top + 1;
			//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			g.FillRectangle(setgBrush(clYWHITE), RECTG(tRc1));
			tRc1.OffsetRect(1, 0);
			//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	// top margin
			g.FillRectangle(setgBrush(clBOTTOM), RECTG(tRc1));
			tRc1.top    = tRc.bottom;
			tRc1.bottom = m_pRc.bottom;
			//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			g.FillRectangle(setgBrush(clBOTTOM), RECTG(tRc1));
			tRc1.OffsetRect(-1, 0);
			//dc->FillSolidRect(tRc1, getRGB(cYWHITE));	// bottom margin
			g.FillRectangle(setgBrush(clYWHITE), RECTG(tRc1));	
			break;

		case AL_BOTTOM:
			tRc1 = m_pRc;
			tRc1.bottom = tRc1.top + 1;
			tRc1.right  = tRc1.left + 1;
	//		dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));	
			tRc1.OffsetRect(0, 1);
	//		dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	 // left margin
			g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));	
			
			tRc1.left  = tRc.right;
			tRc1.right = m_pRc.right;
			//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			g.FillRectangle(setgBrush(clBOTTOM), RECTGA(tRc1));		
			tRc1.OffsetRect(0, -1);
		//	dc->FillSolidRect(tRc1, getRGB(cYWHITE));	// right margin
			g.FillRectangle(setgBrush(clYWHITE), RECTGA(tRc1));	

			break;

		case AL_TOP:
		default:
			tRc1 = m_pRc;
			tRc1.top   = tRc1.bottom - 1;
			tRc1.right = tRc1.left + 1;
			//dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			g.FillRectangle(setgBrush(clYWHITE), RECTG(tRc1));	
			tRc1.OffsetRect(0, -1);
			//dc->FillSolidRect(tRc1, getRGB(cBOTTOM));	// left margin
			g.FillRectangle(setgBrush(clBOTTOM), RECTG(tRc1));	

			tRc1.left  = tRc.right;
			tRc1.right = m_pRc.right;
//			dc->FillSolidRect(tRc1, getRGB(cBOTTOM));
			g.FillRectangle(setgBrush(clBOTTOM), RECTG(tRc1));	

			tRc1.OffsetRect(0, 1);
	//		dc->FillSolidRect(tRc1, getRGB(cYWHITE));
			g.FillRectangle(setgBrush(clYWHITE), RECTG(tRc1));	

			break;
		}
	}

	/*
	if (m_scroll)
	{
		tRc = pRc;
		tRc.bottom--;
		tRc.left  = pRc.right;
		tRc.right = m_pRc.right;
		drawScroll(dc, tRc);
	}
	*/

	if (m_scroll)
	{
		CRect dRc = m_pRc;
		dRc.top = dRc.bottom - (long)(m_pRc.Height() * 2.0 / 3.0); 
		dRc.left = dRc.right - (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
		drawScroll(dc, dRc);
	}
	//pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
		
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);
}

void CamUserTab::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (m_count == 0)
		return;

	CRect	tRc,  pRc = m_pRc;
	int	gap;
	bool	focus_this;
	
	if (down)
	{
		CString	tmpS = _T("");
		int	dx   = 0;

		m_axform->m_cursor = pt;
		m_bForm = false;	// true
		m_click = ckNone;
		if (m_scr.PtInRect(pt))
		{
			CRect	tRc1;
			gap = m_scr.Width() / 2;

			tRc = tRc1 = m_scr;
			tRc.right  = tRc.left + gap+1;
			tRc1.left  = tRc.right;
			tRc1.right = tRc1.left + gap;
			if (tRc.PtInRect(pt))
			{
				m_click = ckLeft;
				result  = RC_LEFT;
				scroll();
			}
			else if (tRc1.PtInRect(pt))
			{
				m_click = ckRight;
				result  = RC_RIGHT;
				scroll(false);
			}

			if (m_click != ckNone)
			{
				m_bForm = false;
				tRc = m_pRc;
				tRc.InflateRect(2, 2);
				invalidateRect(tRc, (m_form->type == TAB_IMAGE) ? false : true);
			}
		}
		else
		{
			for (int ii = 0; ii < m_count; ii++)
			{
				tmpS = m_map.GetAt(ii);
				if (tmpS[0] == '1')
					dx++;
			}
			if (dx == 0)
				return;

			CRgn	rgn;
			CPoint	pts[5];

			if (m_vertical)
			{
				if (m_scroll)
					pRc.bottom -= (pRc.Width()/2);
				else
					pRc.bottom = pRc.top + dx;
				dx = pRc.top;
			}
			else
			{
				if (m_scroll)
				{
					if (m_form->type == TAB_IMAGE)
						pRc.right -= wSCR * 2;
					else
						pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
				}
				else
					pRc.right = pRc.left + dx;

				dx = pRc.left;
			}
			if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
				dx++;

			for (int ii = m_start; ii < m_last; ii++)
			{
				tmpS = m_map.GetAt(ii);
				if (tmpS[0] != '1')
					continue;

				tmpS = m_caption.GetAt(ii);
				tRc  = pRc;

				if (m_vertical)
					tRc.top = dx;
				else
					tRc.left = dx;

				focus_this = false;
				if (ii == m_index && m_hasfocus && !m_bForm)
					focus_this = true;

				if (!m_scroll || ii < m_last-1 || (m_scroll && m_last == getLastVisibleItem()+1))
				{
					CDC*	dc = getDC();
					Graphics g(dc->GetSafeHdc());
					
					CFont*	font = setFont(dc);
					dc->SelectObject(font);

					//dx += getSize(g, ii);
					dx += getSize(dc, ii);
					if (m_vertical)
						tRc.bottom = dx;
					else
						tRc.right = dx;
					releaseDC(dc);
				}

				switch (m_form->type)
				{
				case TAB_BORDER:
					if (!focus_this)
						break;

					switch (m_form->alignment)
					{
					case AL_LEFT:
						tRc.left += 2;			break;
					case AL_RIGHT:
						tRc.right -= 2;			break;
					case AL_BOTTOM:
						tRc.bottom -= 2;		break;
					case AL_TOP:
					default:
						tRc.top += 2;			break;
					}
					break;

				case TAB_OBLIQUE:
					if (!focus_this)
						break;

					switch (m_form->alignment)
					{
					case AL_LEFT:
						tRc.left += 3;			break;
					case AL_RIGHT:
						tRc.right -= 3;			break;
					case AL_BOTTOM:
						tRc.bottom -= 3;		break;
					case AL_TOP:
					default:
						tRc.top += 3;			break;
					}
					break;

				case TAB_RECT:
					pts[0].x = tRc.left;		pts[0].y = tRc.top;
					pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
					if (ii == getLastVisibleItem())
					{
						pts[2].x = tRc.right + (tRc.Height()/2);
						pts[2].y = tRc.bottom;
						pts[3].x = pts[2].x - tRc.Height();
						pts[3].y = tRc.top;
						rgn.CreatePolygonRgn(pts, 4, ALTERNATE);
					}
					else
					{
						pts[2].x = tRc.right;
						pts[2].y = tRc.bottom;
						pts[3].x = tRc.right;
						pts[3].y = tRc.top + (tRc.Height()/2);
						pts[4].x = tRc.right - (tRc.Height()/2);
						pts[4].y = tRc.top;
						rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
					}
					break;
				}

				if ((m_form->type == TAB_RECT && rgn.PtInRegion(pt)) || (m_form->type != TAB_RECT && tRc.PtInRect(pt)))
				{
					m_click = ckTab;
					result  = ii;
					m_bForm = false;
					if (result != m_index || m_bForm)
					{
						m_index = result;
						tRc = m_pRc;
						tRc.InflateRect(2, 2);
						invalidateRect(tRc, (m_form->type == TAB_IMAGE) ? false : true);
						m_axform->m_view->UpdateWindow();
					}

					if (m_form->type == TAB_RECT)
						rgn.DeleteObject();
					break;
				}
				if (m_form->type == TAB_RECT)
					rgn.DeleteObject();
			}

			CString sObj = (m_form->auxS);
			TRIM(sObj);

			if (!sObj.IsEmpty())
			{
				if (m_object.GetCount() > m_index)
					SelectCustomForm(sObj, m_object.GetAt(m_index));
				else
					SelectCustomForm(sObj, _T(""));
			}
		}
		return;
	}

	switch (m_click)
	{
	case ckLeft:
		m_click = ckNone;
		gap = m_scr.Width() / 2;
		tRc = m_scr;
		tRc.right = tRc.left + gap+1;
		invalidateRect(tRc, false);
		break;

	case ckRight:
		m_click = ckNone;
		gap = m_scr.Width() / 2;
		tRc = m_scr;
		tRc.left += (gap+1);
		tRc.right = tRc.left + gap;
		invalidateRect(tRc, false);
		break;

	default:
		m_click = ckNone;
		break;
	}
}

int CamUserTab::OnDrag(CPoint pt, int& result)
{
	int	ii, dx = 0;
	bool	focus_this;
	CRgn	rgn;
	CRect	tRc,  pRc  = m_pRc;
	CPoint	pts[5];

	for (ii = 0; ii < m_count; ii++)
	{
		if (m_map.GetAt(ii).GetAt(0) == '1')
			dx++;
	}
	if (dx == 0 || (m_scroll && m_scr.PtInRect(pt)))
		return -1;

	if (m_vertical)
	{
		if (m_scroll)
			pRc.bottom -= (pRc.Width()/2);
		else
			pRc.bottom = pRc.top + dx;
		dx = pRc.top;
	}
	else
	{
		if (m_scroll)
		{
			if (m_form->type == TAB_IMAGE)
				pRc.right -= wSCR * 2;
			else
				pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
		}
		else
			pRc.right = pRc.left + dx;
		dx = pRc.left;
	}
	if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
		dx++;

	for (ii = m_start; ii < m_last; ii++)
	{
		if (m_map.GetAt(ii).GetAt(0)  != '1')
			continue;

		tRc  = pRc;

		if (m_vertical)
			tRc.top = dx;
		else
			tRc.left = dx;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		if (!m_scroll || ii < m_last-1 || (m_scroll && m_last == getLastVisibleItem()+1))
		{
			CDC*	dc = getDC();
			Graphics g(dc->GetSafeHdc());
			
			CFont*	font = setFont(dc, focus_this ? 1 : -1);
			dc->SelectObject(font);			

			//dx += getSize(g, ii);
			dx += getSize(dc, ii);
			if (m_vertical)
				tRc.bottom = dx;
			else
				tRc.right = dx;
			releaseDC(dc);
		}

		switch (m_form->type)
		{
		case TAB_BORDER:
			if (!focus_this)
				break;

			switch (m_form->alignment)
			{
			case AL_LEFT:
				tRc.left += 2;			break;
			case AL_RIGHT:
				tRc.right -= 2;			break;
			case AL_BOTTOM:
				tRc.bottom -= 2;		break;
			case AL_TOP:
			default:
				tRc.top += 2;			break;
			}
			break;

		case TAB_OBLIQUE:
			if (!focus_this)
				break;

			switch (m_form->alignment)
			{
			case AL_LEFT:
				tRc.left += 3;			break;
			case AL_RIGHT:
				tRc.right -= 3;			break;
			case AL_BOTTOM:
				tRc.bottom -= 3;		break;
			case AL_TOP:
			default:
				tRc.top += 3;			break;
			}
			break;

		case TAB_RECT:
			pts[0].x = tRc.left;		pts[0].y = tRc.top;
			pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
			pts[2].x = tRc.right;		pts[2].y = tRc.bottom;
			if (ii == getLastVisibleItem())
			{
				pts[3].x = tRc.right - tRc.Height();
				pts[3].y = tRc.top;
				rgn.CreatePolygonRgn(pts, 4, ALTERNATE);
			}
			else
			{
				pts[3].x = tRc.right;
				pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right - (tRc.Height()/2);
				pts[4].y = tRc.top;
				rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
			}
			break;
		}
				
		if ((m_form->type == TAB_RECT && rgn.PtInRegion(pt)) || (m_form->type != TAB_RECT && tRc.PtInRect(pt)))
		{
			if (m_form->type == TAB_RECT)
				rgn.DeleteObject();
			return ii;
		}
		if (m_form->type == TAB_RECT)
			rgn.DeleteObject();
	}

	return -1;
}

bool CamUserTab::OnDrop(CPoint pt, CString Data, CString& key)
{
	int	ii, dx = 0;
	bool	focus_this;
	CRgn	rgn;
	CRect	tRc,  pRc = m_pRc;
	CPoint	pts[5];
	
	key = _T("");
	for (ii = 0; ii < m_count; ii++)
	{
		if (m_map.GetAt(ii).GetAt(0) == '1')
			dx++;
	}
	if (dx == 0 || (m_scroll && m_scr.PtInRect(pt)))
		return false;

	if (m_vertical)
	{
		if (m_scroll)
			pRc.bottom -= (pRc.Width()/2);
		else
			pRc.bottom = pRc.top + dx;
		dx = pRc.top;
	}
	else
	{
		if (m_scroll)
		{
			if (m_form->type == TAB_IMAGE)
				pRc.right -= wSCR * 2;
			else
				pRc.right -= (int)(float)(m_pRc.Height() * 2.0 / 3.0) * 2;
		}
		else
			pRc.right = pRc.left + dx;
		dx = pRc.left;
	}
	if (m_form->type == TAB_BORDER || m_form->type == TAB_OBLIQUE)
		dx++;

	for (ii = m_start; ii < m_last; ii++)
	{
		tRc  = pRc;

		if (m_vertical)
			tRc.top = dx;
		else
			tRc.left = dx;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		if (!m_scroll || ii < m_last-1
			|| (m_scroll && m_last == getLastVisibleItem()+1))
		{
			CDC*	dc = getDC();
			Graphics g(dc->GetSafeHdc());			
			
			CFont*	font = setFont(dc, focus_this ? 1 : -1);
			dc->SelectObject(font);

			//dx += getSize(g, ii);
			dx += getSize(dc, ii);
			if (m_vertical)
				tRc.bottom = dx;
			else
				tRc.right = dx;
			releaseDC(dc);
		}

		switch (m_form->type)
		{
		case TAB_BORDER:
			if (!focus_this)
				break;

			switch (m_form->alignment)
			{
			case AL_LEFT:
				tRc.left += 2;			break;
			case AL_RIGHT:
				tRc.right -= 2;			break;
			case AL_BOTTOM:
				tRc.bottom -= 2;		break;
			case AL_TOP:
			default:
				tRc.top += 2;			break;
			}
			break;

		case TAB_OBLIQUE:
			if (!focus_this)
				break;

			switch (m_form->alignment)
			{
			case AL_LEFT:
				tRc.left += 3;			break;
			case AL_RIGHT:
				tRc.right -= 3;			break;
			case AL_BOTTOM:
				tRc.bottom -= 3;		break;
			case AL_TOP:
			default:
				tRc.top += 3;			break;
			}
			break;

		case TAB_RECT:
			pts[0].x = tRc.left;		pts[0].y = tRc.top;
			pts[1].x = tRc.left;		pts[1].y = tRc.bottom;
			pts[2].x = tRc.right;		pts[2].y = tRc.bottom;
			if (ii == getLastVisibleItem())
			{
				pts[3].x = tRc.right - tRc.Height();
				pts[3].y = tRc.top;
				rgn.CreatePolygonRgn(pts, 4, ALTERNATE);
			}
			else
			{
				pts[3].x = tRc.right;
				pts[3].y = tRc.top + (tRc.Height()/2);
				pts[4].x = tRc.right - (tRc.Height()/2);
				pts[4].y = tRc.top;
				rgn.CreatePolygonRgn(pts, 5, ALTERNATE);
			}
			break;
		}

		if ((m_form->type == TAB_RECT && rgn.PtInRegion(pt)) || (m_form->type != TAB_RECT && tRc.PtInRect(pt)))
		{
			key = m_map.GetAt(ii);
			if (key.GetLength() > 2)
				key = key.Mid(2);
			else
				key = _T("");
			
			if (m_form->type == TAB_RECT)
				rgn.DeleteObject();
			
			return true;
		}
		if (m_form->type == TAB_RECT)
			rgn.DeleteObject();
	}

	return false;
}

void CamUserTab::SetBkColor(int rgb, int col, int row)
{
	if (col >= 0)		// selected pRGB
	{
		DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
		if (m_fpRGB == color)
			return;

		m_fpRGB = color;
		invalidateRect(&m_pRc, false);
	}
	else
		CamBase::SetBkColor(rgb);
}

void CamUserTab::ReadAll(CString& data, bool edit, bool head)
{
	data.Format("%d", m_index);
	for (int ii = 0; ii < m_count; ii++)
	{
		data += '|';
		data += m_map.GetAt(ii);
		data += m_caption.GetAt(ii);
	}
}

void CamUserTab::ReadData(CString& data, bool edit, int col, int row)
{
	if (m_index < 0 || m_index > m_map.GetUpperBound())
		return;

	data = m_map.GetAt(m_index);
	if (data.GetLength() > 2)
		data = data.Mid(2);
	else
		data = _T("");
}

void CamUserTab::WriteAll(CString str, bool redraw)
{
//	if (!(m_form->attr & FA_DRAG))
//		return;

	int	offs;
	offs = str.Find('|');
	if (offs == -1)
		return;

	CString	tmps;
	m_index = atoi(str.Left(offs));
	str = str.Mid(offs+1);

	m_caption.RemoveAll();
	m_map.RemoveAll();
	for ( ; !str.IsEmpty(); )
	{
		offs = str.Find('|');
		if (offs != -1)
		{
			tmps = str.Left(offs);
			str  = str.Mid(offs+1);
		}
		else
		{
			tmps = str;
			str = _T("");
		}

		if (tmps.GetLength() < L_MAPN)
			continue;

		m_map.Add(tmps.Left(L_MAPN));
		m_caption.Add(tmps.Mid(L_MAPN));
	}
	m_count = min((int)m_map.GetSize(), (int)m_caption.GetSize());

	if (redraw)
		invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
}

void CamUserTab::WriteData(CString data, bool redraw, int key, int visible)
{
	CString	tmps;
	int	index;

	for (int ii = 0; ii < m_count; ii++)
	{
		tmps = m_map.GetAt(ii);
		if (tmps.GetLength() > 2)
			tmps = tmps.Mid(2);
		else
			tmps = _T("");

		index = ii;
		if (key == atoi(tmps))
			break;
	}

	if (index >= m_count)
		return;

	tmps = m_map.GetAt(index);
	tmps.SetAt(0, visible ? '1' : '0');
	m_map.SetAt(index, tmps);
	
	if (data.GetLength() > 0)
	{
		tmps = m_caption.GetAt(index);
		if (tmps.Compare(data))
			m_caption.SetAt(index, data);
	}

	if (redraw)
		invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
}

void CamUserTab::SetCurSel(int index, bool redraw)
{
	if (index == -1)
	{
		m_bForm = true;
		if (redraw)
			invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
		return;
	}

	m_bForm = false;
	CString	tmpS;
	CString sObj = (m_form->auxS);
	TRIM(sObj);
	int	idx = m_index;
	for (int ii = 0; ii < m_count; ii++)
	{
		tmpS = m_map.GetAt(ii);
		if (tmpS.GetLength() > 2)
			tmpS = tmpS.Mid(2);
		else
			tmpS = _T("");

		if (index == atoi(tmpS))
		{
			m_index = ii;			
			SelectCustomForm(sObj, m_object.GetAt(m_index));
			break;
		}
	}

	m_start = getStartIndex(m_index);
	if (redraw && m_index != idx)
		invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
}

int CamUserTab::GetCurSel()
{
	if (m_index < 0 || m_index > m_map.GetUpperBound()
		|| m_index > m_caption.GetUpperBound())
		return 0;

	CString	tmpS = m_map.GetAt(m_index);
	if (tmpS.GetLength() > 2)
		tmpS = tmpS.Mid(2);
	else
		tmpS = _T("");

	return	atoi(tmpS);
}

int CamUserTab::GetSize()
{
	return m_count;
}

void CamUserTab::GetEnum(CString& text, int& value, int func)
{
	if (func == 101)
	{
		int	index;

		index = getIndexByID(value);
		if (index >= 0 && index < m_caption.GetSize())
			text = m_caption.GetAt(index);
		else
			text = _T("");
		return;
	}

	value = m_type;
	switch (m_type)
	{
	case ET_TAB:
	case ET_USER:
		text = m_namex;		break;
	case ET_ITEM:
	default:
		ReadData(text);		break;
	}
}

void CamUserTab::SetEnum(CString text, int value, int func)
{
	switch (func)
	{
	case 101:
		value = getIndexByID(value);
		if (value >=0 && value < m_caption.GetSize())
			m_caption.SetAt(value, text);
		return;

	case 200:
		if (m_form->type == TAB_IMAGE)
		{
			char	wb[32];
//			text = &m_axform->m_strR[m_form->vals[2]];
//			text.Format("%s/%s/%s", m_axform->m_root, TABDIR, PALETTE);
//			GetPrivateProfileString("General", "Palette", "", wb, sizeof(wb), text);
			loadBitmap(wb);
			invalidateRect(&m_pRc, false);
		}
		return;
	}

	switch (value)
	{
	case -1:
		if (m_path.IsEmpty())
			return;
		break;
	default:
		m_type = ET_USER;
		m_namex = text.IsEmpty() ? m_name : text;
		m_path.Format("%s\\%s\\%s", m_axform->m_root, USRDIR, m_namex);
		break;
	}

	loadTab();

	int	last, width;
	CDC*	dc = getDC();	
	
	CFont*	font = setFont(dc);
	dc->SelectObject(font);

	m_scroll = isScrolled(dc, last, width);
	releaseDC(dc);
	invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
}

bool CamUserTab::IsTabVisible(long ID)
{
	CString	str;
	int	index = getIndexByID(ID);

	if (index >= 0)
	{
		str = m_map.GetAt(index);
		if (str.GetAt(0) == '1')
			return true;
	}

	return false;
}

void CamUserTab::SetTabVisible(long ID, bool visible)
{
	CString	str;
	int	index = getIndexByID(ID);

	if (index >= 0)
	{
		str = m_map.GetAt(index);
		str.SetAt(0, visible ? '1' : '0');
		m_map.SetAt(index, str);
		invalidateRect(&m_pRc, (m_form->type == TAB_IMAGE) ? false : true);
	}
}

void CamUserTab::loadTab()
{
	int	offs;
	char	wb[128];
	CString	tmps, tmps_;

	m_caption.RemoveAll();
	m_map.RemoveAll();
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%02d", ii);
		GetPrivateProfileString(USR_ROOT, tmps, "", wb, sizeof(wb), m_path);
		tmps = wb;

		if (tmps.IsEmpty())
			break;

		offs = tmps.Find(':');
		if (offs == -1)
			continue;

		tmps_ = tmps.Mid(offs+1);
		tmps  = tmps.Left(offs);

		tmps_.TrimLeft();
		tmps_.TrimRight();
		if (tmps_.IsEmpty())
			continue;

		m_caption.Add(tmps);
		tmps.Format("1:%s", tmps_);
		m_map.Add(tmps);
	}
	m_start = 0;
	m_count = (int)m_caption.GetSize();
}

void CamUserTab::loadBitmap(CString prefix)
{
	if (m_form->type != TAB_IMAGE)
		return;

	CString	path;
	Bitmap* pic = NULL;

	//prefix = _T("BLUETAB");
//	m_images.RemoveAll();
	/*
	for (int ii = 0; ii < 7; ii++)
	{
		switch (ii)
		{
		case 0:	path.Format("%s/%s/l_s%s%s.BMP", m_axform->m_root, IMAGEDIR, prefix, &m_axform->m_strR[m_form->vals[2]]);	break;	// select left
		case 1:	path.Format("%s/%s/s%s%s.BMP",   m_axform->m_root, IMAGEDIR, prefix, &m_axform->m_strR[m_form->vals[2]]);	break;	// select body
		case 2:	path.Format("%s/%s/r_s%s%s.BMP", m_axform->m_root, IMAGEDIR, prefix, &m_axform->m_strR[m_form->vals[2]]);	break;	// select right
		case 3:	path.Format("%s/%s/l_%s%s.BMP",  m_axform->m_root, IMAGEDIR, prefix, &m_axform->m_strR[m_form->vals[2]]);	break;	// normal left
		case 4:	path.Format("%s/%s/%s%s.BMP",    m_axform->m_root, IMAGEDIR, prefix, &m_axform->m_strR[m_form->vals[2]]);	break;	// normal body
		case 5:	path.Format("%s/%s/r_%s%s.BMP",  m_axform->m_root, IMAGEDIR, prefix, &m_axform->m_strR[m_form->vals[2]]);	break;	// normal right
		case 6:	path.Format("%s/%s/b_%s%s.BMP",  m_axform->m_root, IMAGEDIR, prefix, &m_axform->m_strR[m_form->vals[2]]);	break;	// extra bground
		}

	//	pic = m_axform->m_draw->SetBitmap((char *)path.operator LPCTSTR());
		pic = (Bitmap*)m_axform->m_pAmDraw->SetImage((char *)path.operator LPCTSTR());
		if (pic)
			m_images.SetAt(ii, pic);
	}
	*/
	CString sImage;
#ifdef _last
	sImage = CString(m_form->str2);
#else
	sImage = "3BTN.BMP";
#endif

	if (/*m_form->properties & PR_IMAGE) &&*/ !sImage.IsEmpty()/*&& m_form->vals[1] != NOVALUE*/)
	{
		CString	path;
		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, sImage);
		m_pic = new Cimage(m_axform->m_pAmDraw, path);
	}
	else
		m_pic = NULL;

}

void CamUserTab::loadSBitmap()
{
	return;
	/*
	CString	path;
	CBitmap	bitmap;
	HBITMAP	hbitmap;

//	m_simages.DeleteImageList();
	path.Format("%s/%s/%s", m_axform->m_root, IMAGEDIR, SCR_BMP);
	hbitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		bitmap.Attach(hbitmap);
		m_simages.Create(wSCR, hSCR, ILC_COLORDDB|ILC_MASK, 6, 1);
		m_simages.Add(&bitmap, RGB(255, 0, 255));
		m_simages.SetBkColor(CLR_NONE);
		bitmap.DeleteObject();
		DeleteObject(hbitmap);
	}
	*/
}

void CamUserTab::drawTab(CDC* dc, CRect pRc, bool scroll)
{
	CRect	tRc, tRc1;
	CString	tmps;
	bool	focus_this;
	CSize	size;
//	Bitmap	*hbmp, *pbmp = NULL;
//	int	cx, dx; 
	Bitmap *pbmp = NULL;
	int dx;
	bool	quit = false;
	Graphics g(dc->GetSafeHdc());
	StringFormat sf;

	dx = 0;
	dc->SetBkMode(TRANSPARENT);
	tRc  = pRc;

	CFont* font;

	for (int ii = m_start; ii < m_last; ii++)
	{
		tmps = m_map.GetAt(ii);
		if (tmps[0] != '1')
			continue;

		tmps = m_caption.GetAt(ii);
		tRc.left = pRc.left + dx;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		font = setFont(dc, focus_this ? 1 : -1);
		dc->SelectObject(font);
		
		sf.SetLineAlignment(StringAlignmentCenter);
		sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
		sf.SetHotkeyPrefix(HotkeyPrefixNone);

//		if (scroll && ii == m_last-1)
//			sf.SetAlignment(StringAlignmentNear);	
//		else
			sf.SetAlignment(StringAlignmentCenter);	

		// 한 탭의 사이즈*****************************
		int tabwidth = getSize(dc, ii, focus_this);
		dx += getSize(dc, ii, focus_this);
			
		//if (!scroll || ii < m_last-1)
		//	dx += tabwidth;
		//*******************************************
		tRc.right = tRc.left + tabwidth;

		if (tRc.right > m_pRc.right)
			tRc.right = m_pRc.right;
		

		
		/*
		tRc1 = tRc;
		cx = tRc.left;
		RectF rt;
		g.MeasureString(tmps.AllocSysString(), -1, setgFont(1), PointF(0, 0), &rt);
		size.cx = (int)rt.Width + 5;
		*/
		/*
		if (m_images.Lookup(focus_this ? 0 : 3, hbmp))			// left
		{
			if (cx + (int)hbmp->GetWidth() >= pRc.right)
			{
				g.DrawImage(hbmp, cx, tRc.top, pRc.right-cx, tRc.Height());
				quit = true;
			}
			else
				g.DrawImage(hbmp, Rect(cx, tRc.top, hbmp->GetWidth(), tRc.Height()));
	
			cx += hbmp->GetWidth();
		}
		if (quit)
			break;
		
		tRc1.left = cx;
		tRc1.right = tRc1.left + size.cx;
		if (tRc1.right >= pRc.right)
		{
			size.cx = pRc.right - tRc1.left;
			tRc1.right = pRc.right;
			quit = true;
		}
		
		if (m_images.Lookup(focus_this ? 1 : 4, hbmp))			// body
		{
			for (int jj = 0; jj < size.cx; jj++, cx++)
				g.DrawImage(hbmp, cx, tRc.top, hbmp->GetWidth(), tRc.Height());
		}
		tRc1.top += 2;
		*/

		

		if (focus_this)
		{
			m_pic->DrawPicture(g, tRc, IA_STRETCH, PI_ENTER);
			setTextColor(dc, m_ftRGB);
			dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
			/*
#ifdef _last	
			//g.DrawString(tmps.AllocSysString(), -1, setgFont(1), RECTF(tRc), &sf, setgBrush(m_ftRGB)); 
#else
			//g.DrawString(tmps.AllocSysString(), -1, setgFont(1), RECTF(tRc), &sf, setgBrush(GRGB(m_ftRGB))); 			
#endif*/
		}
		else 
		{	
			m_pic->DrawPicture(g, tRc, IA_STRETCH, PI_DEFAULT);	
			setTextColor(dc);
			dc->DrawText(tmps, tRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOPREFIX);
			/*
#ifdef _last
			g.DrawString(tmps.AllocSysString(), -1, setgFont(), RECTF(tRc), &sf, setgFBrush()); 
#else
			g.DrawString(tmps.AllocSysString(), -1, setgFont(), RECTF(tRc), &sf, setgFBrush()); 
#endif
		*/
			
		}
		/*
		if (quit)
			break;

		if (m_images.Lookup(focus_this ? 2 : 5, hbmp))			// right
		{
			if (cx + (int)hbmp->GetWidth() >= pRc.right)
			{
				g.DrawImage(hbmp, cx, tRc.top, pRc.right-cx, tRc.Height());
				quit = true;
			}
			else
				g.DrawImage(hbmp, cx, tRc.top, hbmp->GetWidth(), tRc.Height());	
		}
		if (quit)
			break;
		*/
	}
	

	if (pRc.left + dx < pRc.right)
	{
		CRect bg = pRc;
		
		bg.left = tRc.right;
		g.FillRectangle(setgBrush(), RECTG(bg));
		
		//m_pic->DrawPicture(g, bg, IA_STRETCH, PI_DEFAULT);
		/*
		if (m_images.Lookup(6, hbmp))					// extra bgroundf
		{
			for (cx = pRc.left + dx; cx < pRc.right; cx++)
				g.DrawImage(hbmp, cx, tRc.top, hbmp->GetWidth(), tRc.Height());
		}
		*/
	}
	
	
}

void CamUserTab::drawScroll(CDC* dc, CRect scrRc)
{
	CRect	rc  = scrRc;
	int	gap = rc.Width() / 2;
	CBrush*	brush = setBrush(dc, getRGB(cLGRAY));
	Graphics g(dc->GetSafeHdc());
	/*
	if (m_form->type == TAB_IMAGE)
	{
		rc.top = rc.bottom - hSCR;
		m_scr  = rc;

		rc.right = rc.left + wSCR;
		m_simages.DrawIndirect(dc, m_click == ckLeft  ? 2 : 0, CPoint(rc.left, rc.top), CSize(wSCR, hSCR),
								CPoint(0, 0), ILD_NORMAL, SRCCOPY, CLR_NONE, CLR_DEFAULT);
		rc.left  = rc.right;
		rc.right = rc.left + wSCR;
		m_simages.DrawIndirect(dc, m_click == ckRight ? 5 : 3, CPoint(rc.left, rc.top), CSize(wSCR, hSCR),
								CPoint(0, 0), ILD_NORMAL, SRCCOPY, CLR_NONE, CLR_DEFAULT);
	}
	else
	*/
	
	Image *pImage = NULL;
	{
		rc.top = rc.bottom - gap;
		m_scr  = rc;

		rc.right = rc.left + gap+1;
		if (m_click == ckLeft)
			pImage = DrawFrameControl(dc, rc, DFC_SCROLL, DFCS_SCROLLLEFT|DFCS_PUSHED);
		else
      		pImage = DrawFrameControl(dc, rc, DFC_SCROLL, DFCS_SCROLLLEFT);

		g.DrawImage(pImage, RECTG(rc));
		SAFE_DELETE(pImage);

		rc.left  = rc.right;
		rc.right = rc.left + gap;
		if (m_click == ckRight)
			pImage = DrawFrameControl(dc, rc, DFC_SCROLL, DFCS_SCROLLRIGHT|DFCS_PUSHED);
		else
			pImage = DrawFrameControl(dc, rc, DFC_SCROLL, DFCS_SCROLLRIGHT);
	
		g.DrawImage(pImage, RECTG(rc));
		SAFE_DELETE(pImage);
	}
}

void CamUserTab::drawVText(CDC* dc, CString tabs, CRect rect, UINT style, StringFormat& sf)
{
	CRect	rc;
	CSize	size;
	float	dy;
	CStringArray	arrStr;
	Graphics g(dc->GetSafeHdc());
	RectF rf;
	CString s = _T("A");

	getString(tabs, arrStr);
//	size = dc->GetOutputTextExtent("A");
	g.MeasureString(s.AllocSysString(), -1, setgFont(), PointF(0, 0), &rf);
	size = CSize((int)rf.Width, (int)rf.Height);

	rc = rect;
	dy = (float)(rect.top + TAB_EXTRA/2)-2;
	for (int ii = 0; ii < arrStr.GetSize(); ii++)
	{
		if (dy >= rect.bottom)
			break;

		rc.top = (int)dy;
		dy += (float)size.cy;
		rc.bottom = (int)dy;
		dc->DrawText(arrStr.GetAt(ii), &rc, style);
		//g.DrawString(arrStr.GetAt(ii).AllocSysString(), -1, setgFont(), RECTF(rc), &sf, setgFBrush());
		dy += (float)(size.cy * 0.5);
	}
}

void CamUserTab::drawNoneTab(CDC* dc, CRect tRc, CRect& tRc1, int cx, bool focus_this, UINT& style, StringFormat& sf)
{
	Graphics g(dc->GetSafeHdc());
	CRect	rc;
	Point	pts[6];
	CPoint  pt[6];
	CBrush*	brush;
	int	jj;

	tRc1 = tRc;
	switch (m_form->alignment)
	{
	case AL_LEFT:
		sf.SetLineAlignment(StringAlignmentNear);
		//sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
		sf.SetHotkeyPrefix(HotkeyPrefixNone);
		sf.SetAlignment(StringAlignmentCenter);
		style = DT_CENTER|DT_TOP|DT_NOPREFIX;

		tRc1.left++;	tRc1.top++;
		pts[0].X = tRc1.right;		pts[0].Y = tRc1.top;
		pts[1].X = tRc1.left+cx;	pts[1].Y = tRc1.top;
		pts[2].X = tRc1.left;		pts[2].Y = tRc1.top+cx;
		pts[3].X = tRc1.left;		pts[3].Y = tRc1.bottom-cx;
		pts[4].X = tRc1.left+cx;	pts[4].Y = tRc1.bottom;
		pts[5].X = tRc1.right;		pts[5].Y = tRc1.bottom;

		//setPen(dc, getRGB(cGRAY));
		//setBrush(dc, getRGB(cGRAY));
		//dc->SetPolyFillMode(ALTERNATE);
		//dc->Polygon(pts, 6);
		g.FillPolygon(setgBrush(clGRAY), pts, 6, FillModeAlternate);


		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].X--;
			pts[jj].Y--;
		}
		pts[0].X = tRc.right;
		pts[5].X = pts[0].X;

		/*
		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			setBrush(dc);
		}
		//dc->SetPolyFillMode(ALTERNATE);
		//dc->Polygon(pts, 6);
		*/
		g.FillPolygon(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pts, 6, FillModeAlternate);

		if (!focus_this)
		{
			rc = tRc;
			rc.left = rc.right-1;
			brush = setBrush(dc, getRGB(cYWHITE));
			//dc->FillRect(&rc, brush);	// right
			g.DrawRectangle(setgPen(clYWHITE), RECTG(rc));
			
			brush = setBrush(dc);
			tRc1.right--;
		}
		break;

	case AL_RIGHT:
		sf.SetLineAlignment(StringAlignmentNear);
		//sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
		sf.SetHotkeyPrefix(HotkeyPrefixNone);
		sf.SetAlignment(StringAlignmentCenter);

		style = DT_CENTER|DT_TOP|DT_NOPREFIX;
		tRc1.left++;	tRc1.top++;
		pts[0].X = tRc1.left;		pts[0].Y = tRc1.top;
		pts[1].X = tRc1.left;		pts[1].Y = tRc1.bottom;
		pts[2].X = tRc1.right-cx;	pts[2].Y = tRc1.bottom;
		pts[3].X = tRc1.right;		pts[3].Y = tRc1.bottom-cx;
		pts[4].X = tRc1.right;		pts[4].Y = tRc1.top+cx;
		pts[5].X = tRc1.right-cx;	pts[5].Y = tRc1.top;

		//setPen(dc, getRGB(cGRAY));
		//setBrush(dc, getRGB(cGRAY));
		//dc->SetPolyFillMode(ALTERNATE);
		//dc->Polygon(pts, 6);
		g.FillPolygon(setgBrush(clGRAY), pts, 6, FillModeAlternate);



		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].X--;
			pts[jj].Y--;
		}
		pts[0].X = tRc.left;
		pts[5].X = pts[0].X;
		/*
		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			setBrush(dc);
		}
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);
		*/
		g.FillPolygon(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pts, 6, FillModeAlternate);
		
		if (!focus_this)
		{
			rc = tRc;
			rc.right = rc.left+1;
			brush = setBrush(dc, getRGB(cYWHITE));
	//		dc->FillRect(&rc, brush);	// left
			g.DrawRectangle(setgPen(clYWHITE), RECTG(rc));
	
			brush = setBrush(dc);
			tRc1.left++;
		}
		break;

	case AL_BOTTOM:
		sf.SetLineAlignment(StringAlignmentCenter);
		sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
		sf.SetHotkeyPrefix(HotkeyPrefixNone);
//		sf.SetAlignment(StringAlignmentCenter);

		style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		tRc1.left++;	tRc1.top++;
		pts[0].X = tRc1.left;		pts[0].Y = tRc1.top;
		pts[1].X = tRc1.left;		pts[1].Y = tRc1.bottom-cx;
		pts[2].X = tRc1.left+cx;	pts[2].Y = tRc1.bottom;
		pts[3].X = tRc1.right-cx;	pts[3].Y = tRc1.bottom;
		pts[4].X = tRc1.right;		pts[4].Y = pts[1].Y;
		pts[5].X = tRc1.right;		pts[5].Y = tRc1.top;
		
//		setPen(dc, getRGB(cGRAY));
//		setBrush(dc, getRGB(cGRAY));
//		dc->SetPolyFillMode(ALTERNATE);
		g.FillPolygon(setgBrush(clGRAY), pts, 6, FillModeAlternate);

//		dc->Polygon(pts, 6);

		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].X--;
			pts[jj].Y--;
		}
		pts[0].Y = tRc.top;
		pts[5].Y = pts[0].Y;
/*
		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			setBrush(dc);
		}
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);
*/
		g.FillPolygon(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pts, 6, FillModeAlternate);

		if (!focus_this)
		{
			rc = tRc;
			rc.bottom = rc.top+1;
			brush = setBrush(dc, getRGB(cYWHITE));
			g.DrawRectangle(setgPen(clYWHITE), RECTG(rc));

//			dc->FillRect(&rc, brush);	// top
			brush = setBrush(dc);
			tRc1.top++;
		}
		break;

	case AL_TOP:
	default:
		sf.SetLineAlignment(StringAlignmentCenter);
		sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
		sf.SetHotkeyPrefix(HotkeyPrefixNone);
//		sf.SetAlignment(StringAlignmentCenter);
		
		style = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		tRc1.left++;	tRc1.top++;
		pts[0].X = tRc1.left;		pts[0].Y = tRc1.bottom;
		pts[1].X = tRc1.left;		pts[1].Y = tRc1.top+cx;
		pts[2].X = tRc1.left+cx;	pts[2].Y = tRc1.top;
		pts[3].X = tRc1.right-cx;	pts[3].Y = tRc1.top;
		pts[4].X = tRc1.right;		pts[4].Y = pts[1].Y;
		pts[5].X = tRc1.right;		pts[5].Y = tRc1.bottom;
	
	//	setPen(dc, getRGB(cGRAY));
	//	setBrush(dc, getRGB(cGRAY));
	//	dc->SetPolyFillMode(ALTERNATE);
	//	dc->Polygon(pt, 6);
		g.FillPolygon(setgBrush(clGRAY), pts, 6, FillModeAlternate);

		for (jj = 0; jj < 6; jj++)
		{
			pts[jj].X--;
			pts[jj].Y--;
		}
		pts[0].Y = tRc.bottom;
		pts[5].Y = pts[0].Y;
		
	/*	
		if (focus_this)
		{
			setPen(dc, m_fpRGB);
			brush = setBrush(dc, m_fpRGB);
		}
		else
		{
			setPen(dc);
			brush = setBrush(dc);
		}
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pt, 6);
	*/	

		g.FillPolygon(setgBrush(focus_this ? GRGB(m_fpRGB) : GRGB(m_pRGB)), pts, 6, FillModeAlternate);

		if (!focus_this)
		{
			rc = tRc;
			rc.top = rc.bottom-1;
			brush = setBrush(dc, getRGB(cYWHITE));
			g.DrawRectangle(setgPen(clYWHITE), RECTG(rc));
		//	dc->FillRect(&rc, brush);	// bottom
			
			brush = setBrush(dc);
			tRc1.bottom--;
		}
		break;
	}
}

bool CamUserTab::isScrolled(CDC* dc, int& index, int& width, int start)
{
	CString	infos;
	CRect	rect;
	bool	focus_this, scroll = false;
	int	ii = (start < 0) ? m_start : start;
	Graphics g(dc->GetSafeHdc());
		
	CFont*	font = setFont(dc);
	dc->SelectObject(font);

	index = width = 0;
	rect  = m_pRc;
	for ( ; ii < (int)m_count; ii++)
	{
		infos = m_map.GetAt(ii);
		if (infos[0] != '1')
			continue;

		focus_this = false;
		if (ii == m_index && m_hasfocus && !m_bForm)
			focus_this = true;

		font = setFont(dc, focus_this ? 1 : -1);
		dc->SelectObject(font);

		if (start == -1)
			width += getSizeOld(dc, ii, focus_this);
		else
			width += getSize(dc, ii, focus_this);
		
		if (m_vertical)
		{
			if (!index && (width > rect.Height() - (int)(rect.Width() * 0.5)))
				index = ii+1;
			if (width > rect.Height()
				|| start > 0 && (width > rect.Height() - (int)(rect.Width() * 0.5)))
			{
				scroll = true;
				break;
			}
		}
		else
		{
			int	ww;

			if (m_form->type == TAB_IMAGE)
				ww = wSCR * 2;
			else
				ww = (int)(float)(rect.Height() * 2.0 / 3.0) * 2;

			if (!index && (width + (ii+1) > rect.Width()))
				index = ii+1;
			if (width > rect.Width() || (start > 0 && (width > rect.Width())))
			{
				scroll = true;
				break;
			}
		}
	}

	if (!scroll)
		index = getLastVisibleItem()+1;
	index = (index == 0) ? m_count : index;
	return scroll;
}

int CamUserTab::getSize(CDC* dc, int index, bool focus)
{
	CSize	size;

	if (m_form->properties & PR_SORT)	// fixed
	{
		int n;
		if (m_vcount == 0)
			n = m_pRc.Width();
		else 
			n = m_pRc.Width() / m_vcount;	
		return n;
	}

	if (m_vertical)
	{
		CStringArray arrStr;
		size = dc->GetOutputTextExtent("A");
		getString(m_caption.GetAt(index), arrStr);
		return (size.cy * (int)arrStr.GetSize() + (int)(size.cy * 0.5 * (arrStr.GetSize()-1)) + TAB_EXTRA);
	}

	size = dc->GetOutputTextExtent(m_caption.GetAt(index));
	if (m_form->type == TAB_RECT)
		return (size.cx + TAB_EXTRA2);

	return (size.cx + TAB_EXTRA);
}

int CamUserTab::getSize(Graphics& g, int index, bool focus)
{
	CSize	size;
	CString ss = m_caption.GetAt(index);
	RectF rt;

	if (m_form->properties & PR_SORT)	// fixed
	{
		int n;
		if (m_vcount == 0)
			n = m_pRc.Width();
		else 
			n = m_pRc.Width() / m_vcount;	
		return n;
	}

	// Automatic
	if (m_vertical)
	{
		CStringArray arrStr;
		ss = "A";
		g.MeasureString(ss.AllocSysString(), -1, setgFont(1), PointF(0, 0), &rt);
		size = CSize((int)rt.Width, (int)rt.Height);
		getString(m_caption.GetAt(index), arrStr);
		return (size.cy * (int)arrStr.GetSize() + (int)(size.cy * 0.5 * (arrStr.GetSize()-1)) + TAB_EXTRA);
	}
	
	g.MeasureString(ss.AllocSysString(), -1, setgFont(1), PointF(0, 0), &rt);
	
	size = CSize((int)rt.Width, (int)rt.Height);

	if (m_form->type == TAB_RECT)
		return (size.cx + TAB_EXTRA2);
	
	/*if (m_form->type == TAB_IMAGE)
	{
		//Bitmap* hbmp;
		//int	dx = 0;
		/*
		if (focus)
		{
			if (m_images.Lookup(0, hbmp))			// select left
				dx += hbmp->GetWidth();
			if (m_images.Lookup(2, hbmp))			// select right
				dx += hbmp->GetWidth();
		}
		else
		{
			if (m_images.Lookup(3, hbmp))			// normal left
				dx += hbmp->GetWidth();	
			if (m_images.Lookup(5, hbmp))			// normal right
				dx += hbmp->GetWidth();
		}
		*/
	/*
		int cx, cy;
		m_pic->GetSize(cx, cy);

		return max(cx, size.cx);
		//return (size.cx + dx);
	}
	*/
	return (size.cx + TAB_EXTRA);

//	int n = m_pRc.Width() / m_count;
//	return n;
}

void CamUserTab::getString(CString string, CStringArray& arrStr)
{
	arrStr.RemoveAll();
	for (int ii = 0; ii < string.GetLength(); ii++)
	{
		if (string[ii] & 0x80)
		{
			arrStr.Add(string.Mid(ii, 2));
			ii++;
		}
		else
			arrStr.Add(string.Mid(ii, 1));
	}
}

int CamUserTab::getFirstVisibleItem(int start)
{
	CString	tmps;
	for ( ; start < m_count; start++)
	{
		tmps = m_map.GetAt(start);
		if (tmps[0] == '1')
			return start;
	}

	return -1;
}

int CamUserTab::getLastVisibleItem()
{
	CString	item;
	int	index = 0;

	for (int ii = 0; ii < m_count; ii++)
	{
		item = m_map.GetAt(ii);
		if (item[0] == '1')
			index = ii;
	}

	return index;
}

void CamUserTab::scroll(bool prev)
{
	CDC*	dc = getDC();
	int	index, width = 0;
	CString	tmps;

	if (prev)
	{
		if (m_start > 0)
		{
			int	ii;

			for (ii = m_start-1 ; ii >= 0; ii--)
			{
				tmps = m_map.GetAt(ii);
				if (tmps[0] == '1')
					break;
			}
			m_start = (ii < 0) ? getFirstVisibleItem(0) : ii;
		}
		releaseDC(dc);
		return;
	}

	if (!isScrolled(dc, index, width, m_start))
	{
		releaseDC(dc);
		return;
	}

	if (m_count == m_start+1)
		return;

	m_start = getFirstVisibleItem(m_start+1);
	releaseDC(dc);
}

int CamUserTab::getIndexByID(int ID)
{
	CString	str;

	for (int ii = 0; ii < m_map.GetSize(); ii++)
	{
		str = m_map.GetAt(ii);
		if (str.GetLength() > 2)
		{
			str = str.Mid(2);
			if (ID == atoi(str))
				return ii;
		}
	}
	return -1;
}

int CamUserTab::getStartIndex(int index)
{
	if (!m_scroll)
		return m_start;

	if (index < m_start)
	{
		while (index < m_start)
			scroll();
	}
	else if (index == m_last-1)
		m_start++;
	else if (index > m_last-1)
	{
		int	width;
		bool	scroll = true;
		CDC*	dc = getDC();
		
		CFont*	font = setFont(dc);
		dc->SelectObject(font);
		while (index >= m_last-1 && scroll)
		{
			m_start++;
			scroll = isScrolled(dc, m_last, width);
		}
		releaseDC(dc);
	}

	return m_start;
}

bool CamUserTab::isJustBeforeSelectItem(int index)
{
	CString	str;

	if (index >= 0)
	{
		for (++index; index < m_index; index++)
		{
			str = m_map.GetAt(index);
			if (str.GetAt(0) == '1')
				break;
		}
		if (index == m_index)
			return true;
	}

	return false;
}

bool CamUserTab::SelectCustomForm(CString sForm, CString sMap)
{
	CamBase* pform;
	m_axform->FindForm(sForm, pform);

	if (pform)
	{
		((CamObject*)pform)->SetObject(sMap);
		((CamObject*)pform)->Refresh(false);
	}
	return true;
}

int CamUserTab::getSizeOld(CDC* dc, int index, bool focus)
{
	CSize	size;

	if (m_vertical)
	{
		CStringArray arrStr;
		size = dc->GetOutputTextExtent("A");
		getString(m_caption.GetAt(index), arrStr);
		return (size.cy * (int)arrStr.GetSize() + (int)(size.cy * 0.5 * (arrStr.GetSize()-1)) + TAB_EXTRA);
	}

	size = dc->GetOutputTextExtent(m_caption.GetAt(index));
	if (m_form->type == TAB_RECT)
		return (size.cx + TAB_EXTRA2);


	return (size.cx + TAB_EXTRA);
}