// axSB.cpp : implementation file
//

#include "stdafx.h"
#include "axSB.h"
#include "axStatic.h"
#include "axButton.h"
#include "axTicker.h"
#include "resource.h"
#include "axcommon.hxx"
#include <mmsystem.h>

#include "../../axis/axmsg.hxx"
#include "../../axis/resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const	int	maxItem  = 5;
struct	_oaInfo {
	int	type;
	char	name[32];
	char	data[128];
	struct	_oaItem {
		char	flag;
		short	sym;
	} oaItem[maxItem];
} oaInfo[] = {
	{OA_RTMK0, "KP지수",  "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'C', sGVOL}, {'N', -1}}},		// sbTEXT
	{OA_RTMK2, "KP200",   "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'C', sGVOL}, {'N', -1}}},		// sbTEXT
	{OA_RTMKX, "REAL200", "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'C', sGVOL}, {'N', -1}}},		// sbTEXT
	{OA_RTMKQ, "KQ지수",  "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'C', sGVOL}, {'N', -1}}},		// sbTEXT
	{OA_RTMQ5, "KQ50",    "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'C', sGVOL}, {'N', -1}}},		// sbTEXT
	{OA_RTMQX, "REAL50",  "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'C', sGVOL}, {'N', -1}}},		// sbTEXT
	{OA_RTMFJ, "KP선물",  "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'C', sGVOL}, {'N', -1}}},		// sbTEXT
	{OA_RTMSF, "S&&P",    "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'N', sCTIM}, {'N', -1}}},		// sbTEXT
	{OA_RTMNF, "나스닥",  "", {{'N', sCURR}, {'N', sDIFF}, {'P', sUDYL}, {'N', sCTIM}, {'N', -1}}},		// sbTEXT
};
#define	L_OAINFO	sizeof(oaInfo)/sizeof(struct _oaInfo)

const COLORREF colUPX   = PALETTERGB(174,  39,   3);
const COLORREF colUP    = PALETTERGB(207,  98,  69);
const COLORREF colAVG1  = PALETTERGB(246, 218, 130);
const COLORREF colAVG2  = PALETTERGB(  0, 138,   0);
const COLORREF colDOWN  = PALETTERGB( 22,  91, 196);
const COLORREF colDOWNX = PALETTERGB(  1,  59, 145);

/////////////////////////////////////////////////////////////////////////////
// CAxSB

CAxSB::CAxSB(int type, int btnID, bool underline)
{
	m_index = -1;
	m_type  = type;
	m_text  = _T("");

	m_graph     = true;
	m_bkcolor   = (m_type == sbBTN) ? PALETTERGB(197, 196, 213) : colSB;
	m_underline = underline;

	m_down = false;
	m_btnRc.SetRectEmpty();
	
	m_downX = false;
	m_btnRcX.SetRectEmpty();
	m_button = (CAxButton *) 0;

	m_maxTick = 15;
	m_items.RemoveAll();

	m_invest = 0;
	m_listB  = (CAxTicker *) 0;
	m_static = (CAxStatic *) 0;

	if (m_type == sbLISTB)
	{
		m_static = new CAxStatic;
		m_btnID = btnID; m_button = new CAxButton(m_btnID);
	}
}

CAxSB::~CAxSB()
{
	m_font.DeleteObject();
	m_bfont.DeleteObject();

	if (m_static) delete m_static;
	if (m_button) delete m_button;

	if (m_listB)
	{
		m_listB->DestroyWindow();
		delete m_listB;
	}
	m_items.RemoveAll();
}


BEGIN_MESSAGE_MAP(CAxSB, CWnd)
	//{{AFX_MSG_MAP(CAxSB)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_INVEST_SETUP, OnButtonInvest)
	ON_COMMAND(ID_VIEW_CONCLUDE_SETUP, OnButtonConclude)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAxSB message handlers

int CAxSB::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_font.DeleteObject();
	m_font.CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, false, false,
			HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림");

	m_bfont.DeleteObject();
	m_bfont.CreateFont(12, 0, 0, 0, FW_SEMIBOLD, FALSE, false, false,
			HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림");

	return 0;
}

void CAxSB::OnPaint() 
{
	CPaintDC dc(this);
	CRect	clientRC;
	CRgn	clip;
	CDC	memDC;
	CBitmap bitmap, *oldBitmap;

	GetClientRect(&clientRC);
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, clientRC.Width(), clientRC.Height());
	oldBitmap = (CBitmap *) memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(clientRC, m_bkcolor);
	clip.CreateRectRgnIndirect(&clientRC);
	memDC.SelectClipRgn(&clip);

	switch (m_type)
	{
	case sbTEXT:
		drawTextEx(&memDC, clientRC);
		break;

	case sbGRAPH:
		if (m_graph)
		{
			CRect rateRC = clientRC;

			rateRC.DeflateRect(3, 3);
			drawRateRectangle(&memDC, rateRC);
		}
		else
			drawRateText(&memDC, clientRC);
		break;

	case sbLISTB:
		drawListButton(&memDC, clientRC);
		break;

	case sbBTN:
		drawText(&memDC, clientRC);
		break;
	}

	memDC.SelectClipRgn(NULL);
	clip.DeleteObject();

	dc.BitBlt(clientRC.left, clientRC.top, clientRC.Width(), clientRC.Height(), &memDC, 0, 0, SRCCOPY);
	if (m_type == sbBTN)
		dc.DrawEdge(clientRC, EDGE_RAISED, BF_RECT);
	else if (m_underline)
		dc.DrawEdge(clientRC, EDGE_ETCHED, BF_BOTTOM);

	memDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
}

void CAxSB::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rect;

	GetClientRect(&rect);
	switch (m_type)
	{
	case sbGRAPH:
		m_graph = !m_graph;
		Invalidate();
		break;

	case sbLISTB:
		m_down = false;
		rect.right = m_btnRc.left;
		if (m_btnRc.PtInRect(point))
		{
			if (AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axAXIS, 0), (LPARAM)true))
			{
				m_down = true;
				Invalidate();
			}
		}
		else if (rect.PtInRect(point))
		{
			CString	tick, string, dummy;

			if (m_items.GetSize() == 0)
				return;

			string = m_items.GetAt(0);
			int	pos = string.Find('\n');
			dummy  = string.Left(++pos);
			string = string.Mid(pos);

			string.TrimLeft();
			string.TrimRight();
			if (!string.IsEmpty() && m_oatype == OA_CONCLUDE)
			{
				AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axRESULT, 0),
								(LPARAM)string.operator LPCTSTR());
			}
		}
		break;

	case sbBTN:
		break;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CAxSB::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_type == sbLISTB && m_down)
	{
		m_down = false;
		Invalidate();

		if (m_btnRc.PtInRect(point) && 	m_items.GetSize() > 0)
			runningListBox();
	}	

	CWnd::OnLButtonUp(nFlags, point);
}

void CAxSB::OnMouseMove(UINT nFlags, CPoint point) 
{
	switch (m_type)
	{
	case sbTEXT:
		break;
	case sbLISTB:
		if (m_down && !m_btnRc.PtInRect(point))
		{
			m_down = false;
			Invalidate();
		}
		break;
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CAxSB::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_type == sbTEXT)
	{
		CString	desc;
		int	item = -1;
		CMenu*	popM = new CMenu();

		::GetCursorPos(&point);
		popM->CreatePopupMenu();
		for (int ii = 0; ii < L_OAINFO; ii++)
		{
			popM->AppendMenu(MF_STRING|MF_ENABLED, ID_MENU_TB2+ii, oaInfo[ii].name);
			if (m_oatype == oaInfo[ii].type)
				item = ii;
		}
		if (item >= 0)
		{
			popM->SetDefaultItem(item, TRUE);
			popM->CheckMenuItem(item, MF_BYPOSITION|MF_CHECKED);
		}

		item  = popM->TrackPopupMenu(TPM_RIGHTALIGN|TPM_RETURNCMD, point.x, point.y, this);
		item -= ID_MENU_TB2;
		delete popM;
		if (item >=0 && item < L_OAINFO)
		{
			m_oatype = oaInfo[item].type;
			m_text   = oaInfo[item].data;
			Invalidate(FALSE);
		}
	}
	else if (m_type == sbLISTB && m_items.GetSize() > 0)
	{
		const int baseID = 100;

		::GetCursorPos(&point);

		CMenu	popM;	popM.CreatePopupMenu();
		popM.AppendMenu(MFT_STRING, baseID, "전체지우기");
		int menuID = popM.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD,	point.x, point.y, this, NULL);
		popM.DestroyMenu();

		if (baseID == menuID)
		{
			if (m_static->m_hWnd != NULL)
				m_static->ClearItem();
			m_items.RemoveAll();
		}
	}

	CWnd::OnRButtonDown(nFlags, point);
}

void CAxSB::SetText(int type, CString text)
{
	int ii = 0;
	if (text.GetLength() <= 0)
		return;

	CString	tmps, str;

	if (m_type == sbLISTB && m_oatype == type)
	{
		switch (type)
		{
		case OA_RTMSH:
			str = editingInvest(text);	break;
		case OA_CONCLUDE:
			str = editingConclude(text);	break;
		}

		if (str.IsEmpty()) return;
		
		if (m_beep)
		{
			::sndPlaySound(NULL, SND_ASYNC|SND_NODEFAULT);
			if (m_file.IsEmpty())
				::MessageBeep(MB_OK);
			else
				::sndPlaySound(m_file, SND_ASYNC|SND_NODEFAULT);
		}

		if (m_static->m_hWnd != NULL)
			m_static->UpdateItem(str);

		m_items.InsertAt(0, str);
		if (m_items.GetSize() > m_maxTick)
			m_items.RemoveAt(m_items.GetUpperBound());

		return;
	}

	str  = text;
	tmps = _T("");
	for (ii = 0; ii < L_OAINFO; ii++)
	{
		if (type != oaInfo[ii].type)
			continue;

		CString	disps = _T("");
		for (int jj = 0; jj < maxItem; jj++)
		{
			if (oaInfo[ii].oaItem[jj].sym == -1)
				continue;

			CString data = getSymbolData(oaInfo[ii].oaItem[jj].sym, str);
			if (data.IsEmpty())
				continue;

			switch (oaInfo[ii].oaItem[jj].flag)
			{
			case 'C':				// comma
				data = insertCommaToNum(data);
				break;

			case 'P':				// parenthesis
				if (GetSystemMetrics(SM_CXSCREEN) < 1280)
					continue;
				tmps = data;
				data.Format("%c%c%s%c%c%c", '(', (char)0x1b, tmps, '%', (char)0x1b, ')');
				break;

			case 'N':				// normal
				if (oaInfo[ii].oaItem[jj].sym == sDIFF)
				{
					data = filterSpace(data);
					if (data.IsEmpty())
						data = _T("0.00");
				}
				if (oaInfo[ii].oaItem[jj].sym == sCTIM)
				{
					tmps = data;
					if (data.GetLength() >= 4)
						data.Format("%s:%s", tmps.Left(2), tmps.Mid(2,2));
				}
		//		if (data.IsEmpty())
		//			continue;
				break;

			case 'U':
				switch (oaInfo[ii].oaItem[jj].sym)
				{
				case sSSJS:
					tmps = data;
					data.Format(" 상승 %s", tmps);	break;
				case sSHJS:
					tmps = data;
					data.Format("(%s)", tmps);	break;
				case sBHJS:
					tmps = data;
					data.Format(" 보합 %s", tmps);	break;
				case sHRJS:
					tmps = data;
					data.Format(" 하락 %s", tmps);	break;
				case sHHJS:
					tmps = data;
					data.Format("(%s)", tmps);	break;
				}
				disps += data;
				continue;

			default:
				continue;
			}

			if ((oaInfo[ii].oaItem[jj].flag == 'N' && oaInfo[ii].oaItem[jj].sym != sCTIM)
					|| data[0] == '+' || data[0] == '-')
				data = (char)0x1b + data + (char)0x1b;

			disps += ' ' + data;
		}

		strcpy_s(oaInfo[ii].data, oaInfo[ii].name);
		strcat_s(oaInfo[ii].data, disps);
		break;
	}

	if (ii < L_OAINFO && m_oatype == oaInfo[ii].type)
	{
		m_text = oaInfo[ii].data;
		Invalidate();
	}
}

void CAxSB::SetItems(CStringArray& items)
{
	if (items.GetSize() <= 0 || m_oatype != -100)
		return;

	char	color = '=';
	m_items.RemoveAll();
	for (int ii = 0; ii < items.GetSize(); ii++)
	{
		if (!ii)
			color = items.GetAt(ii).GetAt(0);
		m_items.Add(items.GetAt(ii).Mid(1));
	}

	if (m_static->m_hWnd != NULL)
		m_static->UpdateItem(m_items.GetAt(0), color);
}


void CAxSB::SetsbConfig(DWORD item, bool beep, CString file)
{
	m_invest = item;
	if (m_oatype == OA_CONCLUDE)
		m_maxTick = item;

	m_beep  = beep;
	m_file  = file;
}

bool CAxSB::Initialize(int index, CWnd* parent, UINT nID, CRect rect, UINT oatype, HBITMAP bitmap, CString text)
{
	CRect	rc;

	m_text   = text;
	m_index  = index;
	m_ID     = nID;
	m_oatype = oatype;
	m_bitmap = bitmap;
	rc.SetRect(-rect.Width(), -rect.Height(), 0, 0);

	if (!Create(NULL, m_text, WS_CHILD|WS_VISIBLE, rc, parent, m_ID))
		return false;

	SetFont(&m_font);
	if (m_type == sbLISTB)
	{
		rc.OffsetRect(rect.Width(), rect.Height());
		rc.right -= (rc.Height()*2);
		if (!m_static->Create("", WS_CHILD|WS_VISIBLE|SS_SIMPLE, rc, this, m_ID))
			return false;

		rc.left = rc.right + rc.Height(); rc.right = rc.left + rc.Height();
		rc.DeflateRect(1, 1);
		if (!m_button->Create(NULL, WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_OWNERDRAW|BS_ICON, 
					rc, this, m_btnID))
			return false;

		m_button->disableFlate();
		m_button->SetIcon(AfxGetApp()->LoadIcon(IDI_SETUP), CSize(16,16));
	}

	return true;
}

int CAxSB::GetType()
{
	return m_oatype;
}

void CAxSB::SetType(int oatype)
{
	int	item = -1, ii;
	for (ii = 0; ii < L_OAINFO; ii++)
	{
		if (oatype == oaInfo[ii].type)
		{
			item = ii;
			break;
		}
	}
	if (item >= 0)
	{
		m_oatype = oatype;
		m_text   = oaInfo[ii].data;
		Invalidate(FALSE);
	}
}

LRESULT CAxSB::OnUser(WPARAM wParam, LPARAM lParam)
{
	if (m_type != sbLISTB)
		return 0;

	int	ii, pos;
	CString	string, stringX, dummy;

	switch (LOWORD(wParam))
	{
	case TBM_SELECTION:
		string = (char *)lParam;
		if ((short)HIWORD(wParam) < 0 || string.IsEmpty())
			break;

		for (ii = 0; ii < m_items.GetSize(); ii++)
		{
			stringX = m_items.GetAt(ii);
			if (stringX.Find(string) == -1)
				continue;

			pos = stringX.Find('\n');
			if (pos == -1) 
				continue;

			dummy   = stringX.Left(++pos);
			stringX = stringX.Mid(pos);

			if (atoi(stringX) > 0 && m_oatype == OA_CONCLUDE)
			{
				AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axRESULT, 0),
									(LPARAM)stringX.operator LPCTSTR());
				break;
			}
		}
		break;

	case TBM_NOSELECTION:
		SetFocus();
		break;
	}

	return 0;
}

void CAxSB::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

void CAxSB::adjust(CToolBar* tBar, UINT nID, CRect rect)
{
	if (m_type != sbLISTB) return;

	if (this->m_hWnd != NULL)
		SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);

	int height = rect.Height() +1; rect.right  -= (height * 2);
	if (m_static->m_hWnd != NULL)
	{
		m_static->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
		rect.left = rect.right + height; rect.right = rect.left + height;
		m_button->SetWindowPos(NULL, rect.left, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}
}

void CAxSB::drawText(CDC* dc, CRect rect)
{
	if (m_text.IsEmpty())
		return;

	CFont*	 oldFont    = dc->SelectObject(&m_font);
	int	 oldBkMode  = dc->SetBkMode(TRANSPARENT);
	COLORREF oldFgColor = dc->SetTextColor(PALETTERGB(0, 0, 0));

	dc->DrawText(m_text, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	dc->SelectObject(oldFont);
	dc->SetTextColor(oldFgColor);
	dc->SetBkMode(oldBkMode);
}

void CAxSB::drawTextEx(CDC* dc, CRect rect)
{
	if (m_text.IsEmpty())
		return;

	CString	str  = m_text;
	COLORREF textColor  = PALETTERGB(0, 0, 0);
	CFont*	 oldFont    = dc->SelectObject(&m_font);
	int	 oldBkMode  = dc->SetBkMode(TRANSPARENT);
	COLORREF oldFgColor = dc->SetTextColor(textColor);

	CSize	size;
	COLORREF rgb;
	bool	isColor = false;
	CString	tmps, text  = str;
	int	pos, index = 0;

	text = str;
	for (int ii = 0; ii >= 0; )
	{
		ii = str.Find(0x1b);
		if (ii >= 0)
		{
			isColor = true;
			text = str.Left(ii);
			switch (str[++ii])
			{
			case '+':
				rgb = PALETTERGB(255, 0, 0);
				break;
			case '-':
				rgb = PALETTERGB(0, 0, 255);
				break;
			default:
				rgb = textColor;
				break;
			}
			str = str.Mid(ii);
		}
		else
			text = str;

		dc->SelectObject(&m_font);
		switch (index)
		{
		case 0:
			pos = text.Find(0x20);
			if (pos >= 0)
			{
				tmps = text.Left(pos);
				text = text.Mid(pos);
			}
			dc->SelectObject(&m_font);
			size = dc->GetTextExtent("종합KQX");
			rect.SetRect(rect.left, rect.top, rect.left + size.cx, rect.bottom);
			dc->DrawText(tmps, rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			break;

		case 1:
			pos = text.Find('+');
			if (pos >= 0)
				text.Delete(pos, 1);
			pos = text.Find('-');
			if (pos >= 0)
				text.Delete(pos, 1);
			dc->SelectObject(&m_bfont);
			size = dc->GetTextExtent(" 999.99");
			rect.SetRect(rect.right, rect.top, rect.right + size.cx, rect.bottom);
			dc->DrawText(text, rect, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			if (text.IsEmpty())
				break;

			switch (index)
			{
			case 3:
				dc->SelectObject(&m_bfont);
				size = dc->GetTextExtent("+99.99");
				break;
			case 5:
				size = dc->GetTextExtent("+99.99%");
				break;
			default:
				size = dc->GetTextExtent(text);
				break;
			}
			rect.SetRect(rect.right, rect.top, rect.right + size.cx, rect.bottom);
			dc->DrawText(text, rect, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			break;

		default:
			break;
		}
		index++;
		if (isColor)
		{
			dc->SetTextColor(rgb);
			isColor = false;
		}
	}

	dc->SelectObject(oldFont);
	dc->SetTextColor(oldFgColor);
	dc->SetBkMode(oldBkMode);
}

void CAxSB::drawRateRectangle(CDC* dc, CRect rect)
{
	int ii = 0;
	int jj = 0;
	if (m_text.IsEmpty())
		return;

	char	datB[128], rWb[16];
	int	datL = m_text.GetLength();
	strcpy_s(datB, (char *) m_text.operator LPCTSTR());

	int	up, upx, avg, down, downx, total;

	if ((up = m_text.Find("상승 ")) != -1)
		up += 5;
	if ((upx = m_text.Find("(")) != -1)
		upx   += 1;
	if ((avg = m_text.Find("보합 ")) != -1)
		avg   += 5;
	if ((down = m_text.Find("하락 "))!= -1)
		down  += 5;

	CString	tmps = m_text.Mid(down);
	if ((downx = tmps.Find("(")) != -1)
		downx += 1+down;

	ZeroMemory(rWb, sizeof(rWb));
	for (ii = up, jj = 0; up >= 0 && ii < datL; ii++)
	{
		if (datB[ii] >= '0' && datB[ii] <= '9')
		{
			rWb[jj++] = datB[ii];
			continue;
		}

		if (datB[ii] == '(' || datB[ii] == ' ')
		{
			rWb[jj] = '\0';
			break;
		}
	}
	up = 0;
	if (strlen(rWb))
		up = atoi(rWb);

	ZeroMemory(rWb, sizeof(rWb));
	for (ii = upx, jj = 0; upx >= 0 && ii < datL; ii++)
	{
		if (datB[ii] >= '0' && datB[ii] <= '9')
		{
			rWb[jj++] = datB[ii];
			continue;
		}

		if (datB[ii] == ')' || datB[ii] == ' ')
		{
			rWb[jj] = '\0';
			break;
		}
	}
	upx = 0;
	if (strlen(rWb))
		upx = atoi(rWb);

	ZeroMemory(rWb, sizeof(rWb));
	for (ii = avg, jj = 0; avg >= 0 && ii < datL; ii++)
	{
		if (datB[ii] >= '0' && datB[ii] <= '9')
		{
			rWb[jj++] = datB[ii];
			continue;
		}

		if (datB[ii] == '(' || datB[ii] == ' ')
		{
			rWb[jj] = '\0';
			break;
		}
	}
	avg = 0;
	if (strlen(rWb))
		avg = atoi(rWb);

	ZeroMemory(rWb, sizeof(rWb));
	for (ii = down, jj = 0; down >= 0 && ii < datL; ii++)
	{
		if (datB[ii] >= '0' && datB[ii] <= '9')
		{
			rWb[jj++] = datB[ii];
			continue;
		}
		
		if (datB[ii] == '(' || datB[ii] == ' ')
		{
			rWb[jj] = '\0';
			break;
		}
	}
	down = 0;
	if (strlen(rWb))
		down = atoi(rWb);

	ZeroMemory(rWb, sizeof(rWb));
	for (ii = downx, jj = 0; downx >= 0 && ii < datL; ii++)
	{
		if (datB[ii] >= '0' && datB[ii] <= '9')
		{
			rWb[jj++] = datB[ii];
			continue;
		}
		
		if (datB[ii] == '(' || datB[ii] == ' ')
		{
			rWb[jj] = '\0';
			break;
		}
	}
	downx = 0;
	if (strlen(rWb))
		downx = atoi(rWb);

	total = upx + up + avg + down + downx;
	if (total == 0)
		return;
	
	CBrush	brush, *obrush;
	CRect	dRc;

	dRc = rect;
	dRc.right = dRc.left + rect.Width() * upx / total;
	brush.CreateSolidBrush(colUPX);
	obrush = dc->SelectObject(&brush);
	dc->FillRect(&dRc, &brush);
	dc->SelectObject(obrush);
	brush.DeleteObject();

	dRc.left  = dRc.right;
	dRc.right = dRc.left + rect.Width() * up / total;
	brush.CreateSolidBrush(colUP);
	obrush = dc->SelectObject(&brush);
	dc->FillRect(&dRc, &brush);
	dc->SelectObject(obrush);
	brush.DeleteObject();

	dRc.left = dRc.right;
	dRc.right = dRc.left + rect.Width() * avg / total;
	brush.CreateSolidBrush(colAVG1);
	obrush = dc->SelectObject(&brush);
	dc->FillRect(&dRc, &brush);
	dc->SelectObject(obrush);
	brush.DeleteObject();

	dRc.left = dRc.right;
	dRc.right = dRc.left + rect.Width() * down / total;
	brush.CreateSolidBrush(colDOWN);
	obrush = dc->SelectObject(&brush);
	dc->FillRect(&dRc, &brush);
	dc->SelectObject(obrush);
	brush.DeleteObject();

	rect.left = dRc.right;
	brush.CreateSolidBrush(colDOWNX);
	obrush = dc->SelectObject(&brush);
	dc->FillRect(&rect, &brush);
	dc->SelectObject(obrush);
	brush.DeleteObject();
}

void CAxSB::drawRateText(CDC* dc, CRect rect)
{
	if (m_text.IsEmpty())
		return;

	CFont*	oldFont     = dc->SelectObject(&m_font);
	int	oldBkMode   = dc->SetBkMode(TRANSPARENT);
	COLORREF oldFgColor = dc->SetTextColor(PALETTERGB(0, 0, 0));
	UINT	nformat     = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	int idx;
	CSize	size;
	CString	str, text;

	str = m_text;
	if ((idx = str.Find("상승")) == -1)
		return;

	text = str.Left(idx);
	str  = str.Mid(idx++);
	dc->DrawText(text, rect, nformat);
	size = dc->GetOutputTextExtent(text);
	rect.left += size.cx;

	if ((idx = str.Find("보합")) == -1)
		return;
	text = str.Left(idx);
	str = str.Mid(idx++);
	dc->SetTextColor(colUP);
	dc->DrawText(text, rect, nformat);
	size = dc->GetOutputTextExtent(text);
	rect.left += size.cx;

	if ((idx = str.Find("하락")) == -1)
		return;
	text = str.Left(idx);
	str = str.Mid(idx++);
	dc->SetTextColor(colAVG2);
	dc->DrawText(text, rect, nformat);
	size = dc->GetOutputTextExtent(text);
	rect.left += size.cx;

	text = str;
	dc->SetTextColor(colDOWN);
	dc->DrawText(text, rect, nformat);
	size = dc->GetOutputTextExtent(text);
	rect.left += size.cx;

	dc->SelectObject(oldFont);
	dc->SetTextColor(oldFgColor);
	dc->SetBkMode(oldBkMode);
}

void CAxSB::drawListButton(CDC* dc, CRect rect)
{
	m_btnRc      = rect;
	m_btnRc.left = m_btnRc.right - (rect.Height()*2 + 2);
	m_btnRc.right = m_btnRc.left + (rect.Height());
	m_btnRc.DeflateRect(1, 0);
	
	UINT state = DFCS_SCROLLDOWN;
	if (m_down) state |= DFCS_PUSHED;

	dc->DrawFrameControl(m_btnRc, DFC_SCROLL, state);
}

CString	CAxSB::editingInvest(CString datB)
{
	CString	string = _T("");

	datB += '\t';

	string = getSymbolData(sSHK1, datB);
	if (string.IsEmpty() || !itemEnabled(string))
		return _T("");

	string = getSymbolData(sSHDA, datB);
	if (string.IsEmpty())
		return _T("");

	return string;
}

CString	CAxSB::editingConclude(CString datB)
{
	char* cJUID = "905";		// 주문자ID
	char* cCNUM = "934";		// 체결일련번호

	char* cCTIM = "931";		// 체결시간
	char* cACCN = "907";		// 계좌번호
	char* cCODX = "021";		// 종목코드
	char* cENAM = "932";		// 영문종목명
	char* cMDMS = "909";		// 매도,매수
	char* cCHSU = "917";		// 체결수량
	char* cCHGA = "916";		// 체결단가

	int	idx;
	CString	string, symbol, entry, temp, user;
	CMapStringToString fms;

	idx = datB.Find('|');
	if (idx == -1) return _T("");
	user = datB.Left(idx++);
	datB = datB.Mid(idx);

	idx = datB.Find('\r');
	if (idx == -1) return _T("");

	string = datB.Left(idx++);
	datB = datB.Mid(idx);

	while (!string.IsEmpty())
	{
		idx = string.Find('\t');
		if (idx == -1) return _T("");
		symbol = string.Left(idx++);
		string = string.Mid(idx);

		idx = string.Find('\t');
		if (idx == -1)
		{
			entry = string; string = _T("");
		}
		else
		{
			entry = string.Left(idx++); string = string.Mid(idx);
		}
		fms.SetAt(symbol, entry);
	}

	if (!fms.Lookup(cJUID, string) || user.Compare(CString(string, user.GetLength())))	// user check
		return _T("");

	if ((idx = datB.Find('\n')) != -1)
	{
		string = datB.Left(idx++);
		datB = datB.Mid(idx);

		while (!string.IsEmpty())
		{
			idx = string.Find('\t');
			if (idx == -1)	break;

			symbol = string.Left(idx++);
			string = string.Mid(idx);

			idx = string.Find('\t');
			if (idx == -1)
			{
				entry = string;	string = _T("");
			}
			else
			{
				entry = string.Left(idx++); string = string.Mid(idx);
			}
			fms.SetAt(symbol, entry);
		}
	}
	if (!fms.Lookup(cCNUM, string) || string.IsEmpty())		// 체결일련번호 check
		return _T("");

	while (!datB.IsEmpty())
	{
		idx = datB.Find('\t');
		if (idx == -1) break;

		symbol = datB.Left(idx++);
		datB = datB.Mid(idx);

		idx = datB.Find('\t');
		if (idx == -1)
		{
			entry = datB;	datB = _T("");
		}
		else
		{
			entry = datB.Left(idx++); datB = datB.Mid(idx);
		}
		fms.SetAt(symbol, entry);
	}

	entry = _T("");
	if (fms.Lookup(cCTIM, string) && string.GetLength() >= 6)
		entry.Format("[%s]", string);

	if (fms.Lookup(cACCN, string) && string.GetLength() >= 13)		// for kiwoom_fb
	{
		temp.Format("%s-%s-%s", string.Mid(0, 4), string.Mid(4, 4), string.Mid(8, 2));
		entry += " " + temp;
	}
	if (fms.Lookup(cCODX, string) && !string.IsEmpty())
	{
		entry += " " + string;
	}
	if (fms.Lookup(cMDMS, string) && !string.IsEmpty())
	{
		if (string.GetAt(0) == '1') entry += _T(" 매도");
		if (string.GetAt(0) == '2') entry += _T(" 매수");
	}
	if (fms.Lookup(cCHSU, string) && !string.IsEmpty())
	{
		temp.Format("%4s", string);
		entry += " " + temp;
	}
	if (fms.Lookup(cCHGA, string) && !string.IsEmpty())
	{
		entry += "   " + string;
	}

	return entry;
}

CString CAxSB::getSymbolData(short symbol, CString& str)
{
	int	idx;
	CString	rtmCode, rtmData;
	enum _search { sOK = 0, sSKIP = 1, sNONE = 2 } search;
	
	rtmCode = _T("");
	rtmData = str;
	search  = sNONE;
	while (!rtmData.IsEmpty())
	{
		if ((idx = rtmData.Find('\t')) == -1)
		{
			rtmData = _T("");
			break;
		}
		
		rtmCode = rtmData.Left(idx++);
		rtmData = rtmData.Mid(idx);
		switch (search)
		{
		case sOK:
			if (rtmCode.IsEmpty())
				break;
			return rtmCode;

		case sSKIP:
			search = sNONE;
			continue;

		case sNONE:
			break;
		}

		if (atoi(rtmCode) == symbol)
			search = sOK;
		else
			search = sSKIP;
	}

	return _T("");
}

CString CAxSB::insertCommaToNum(CString str)
{
	CString data = str;
	if (data.GetLength() <= 3 )
		return data;

	data.MakeReverse();
	str = _T("");
	for (int ii = 0; ii < data.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			str += ',';

		str += data[ii];
	}

	str.MakeReverse();
	return str;
}

CString CAxSB::filterSpace(CString str)
{
	int	pos = 0;
	char	wb[64];

	for (int ii = 0; ii < str.GetLength(); ii++)
	{
		if (str.GetAt(ii) == ' ')
			continue;

		wb[pos++] = str.GetAt(ii);
	}
	wb[pos] = 0x00;
	return wb;
}

void CAxSB::runningListBox()
{
	if (m_listB == NULL)
	{
		m_listB = new CAxTicker();
		m_listB->Create(this);
	}

	if (m_items.GetSize() > 0)
		m_listB->UpdateListBox(m_items);

	CRect	winRC, listRC;
	this->GetWindowRect(&winRC);
	m_listB->GetWindowRect(&listRC);

	int sx, sy, cx, cy;
	CRect rcWA;
	
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWA, 0);
	if (winRC.bottom + listRC.Height() > rcWA.bottom)
	{
		sx = winRC.left < rcWA.left ? rcWA.left : winRC.left;
		sy = winRC.top - listRC.Height();
	}
	else
	{
		sx = winRC.left < rcWA.left ? rcWA.left : winRC.left;
		sy = winRC.bottom;
	}

//	cx = winRC.Width() > listRC.Width() ? winRC.Width() : listRC.Width();
	cx = winRC.Width();
	cy = listRC.Height();

	m_listB->MoveWindow(sx, sy, cx, cy);
	m_listB->ShowWindow(SW_SHOW);
	m_listB->SetForegroundWindow();
}

bool CAxSB::itemEnabled(CString item)
{
	if (item.IsEmpty() || !m_invest)
		return false;

	switch (atoi(item))
	{
	case  0: return true;
	case  1: if (m_invest & investJK) return true;	break;
	case  2: if (m_invest & investBH) return true;	break;
	case  3: if (m_invest & investTS) return true;	break;
	case  4: if (m_invest & investUH) return true;	break;
	case  5: if (m_invest & investCK) return true;	break;
	case  6: if (m_invest & investKK) return true;	break;
	case  7: if (m_invest & investKT) return true;	break;
	case  8: if (m_invest & investKE) return true;	break;
	case  9: if (m_invest & investEK) return true;	break;
	case 10: if (m_invest & investJU) return true;	break;
	default: break;
	}

	return false;
}

void CAxSB::drawingColor(COLORREF bkGround)
{
	m_bkcolor = bkGround;
	Invalidate();
}

void CAxSB::OnButtonInvest()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_INVEST_SETUP, 0), (LPARAM) 0);
}

void CAxSB::OnButtonConclude()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_CONCLUDE_SETUP, 0), (LPARAM) 0);
}

