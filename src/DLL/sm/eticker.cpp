// eticker.cpp : implementation file
//

#include "stdafx.h"
#include "eticker.h"
#include "smdefine.h"
#include "ReportCtrl.h"
#include "resource.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"
#include "../../h/comcode.h"
#include <afxtempl.h>

#define L_INDEX		sizeof(struct kscode)
#define L_FCOMM		sizeof(struct fcomm)
#define L_FREC		sizeof(struct frec)
#define FINDEX_BASE	77
#define GAP		5
#define	BTNHEIGHT	20
#define BKCOLOR		RGB(240, 240, 240)
#define BCOLOR		RGB(  0,  55,   0)
#define BTNCOLOR	RGB(255, 255, 191)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma warning(disable : 26400)  //사용하지 않는 클래스 warning 제거
#pragma warning(disable : 26409)
#pragma warning(disable : 26401)
/////////////////////////////////////////////////////////////////////////////
// CETicker
CETicker::CETicker(CWnd* parent, int kind, int id, CString home, CString usrName, bool (*axiscall)(int, WPARAM, LPARAM))
{
	m_parent = parent;
	m_kind   = kind;
	m_id     = id;
	m_home   = home;
	m_userName = usrName;
	m_axiscall = axiscall;
	m_report = NULL;
	m_down = 0;

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(100, fontName);
}

CETicker::~CETicker()
{
	m_arCodeByIndex.RemoveAll();
	m_font.DeleteObject();
#ifndef DF_USESTL_ETICKER
	if (m_report)	delete m_report;
#endif
}


BEGIN_MESSAGE_MAP(CETicker, CWnd)
	//{{AFX_MSG_MAP(CETicker)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CETicker::Init()
{
	CRect	wRc, eRc, rRc, cRc;

	m_parent->GetWindowRect(wRc);
	
	switch (m_kind)
	{
	case TKIND_INDEX:
		{
			eRc.SetRect(0, 0, 120, 150);

			SetWindowPos(&wndTopMost, wRc.right - eRc.Width(), wRc.top - eRc.Height(),
					eRc.Width(), eRc.Height(), SWP_SHOWWINDOW);

			GetClientRect(cRc);
			rRc.SetRect(GAP, GAP, eRc.Width() - GAP, eRc.bottom - BTNHEIGHT - 2*GAP);
#ifdef DF_USESTL_ETICKER
			m_report = std::make_unique<CReportCtrl>();
			if (!m_report->Create(this, (int)1, rRc, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER))
			{
				m_report = NULL;
			}
#else 
			m_report = new CReportCtrl();
			if (!m_report->Create(this, (int)1, rRc, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER))
			{
				delete m_report;
				m_report = NULL;
			}
#endif
			else
			{
				m_report->InsertColumn(0, "ETicker Item");
				m_report->SetCheckboxeStyle(RC_CHKBOX_NORMAL);	// Enable checkboxes
				loadItem();

				m_btnRect[0].SetRect(GAP, cRc.bottom - GAP - BTNHEIGHT, cRc.right - BTNHEIGHT - 2*GAP, cRc.bottom - GAP);
				m_btnRect[1].SetRect(m_btnRect[0].right + GAP, m_btnRect[0].top, cRc.right - GAP, cRc.bottom - GAP);
			}
		}
		break;
	case TKIND_UPDOWN: 
		{
			eRc.SetRect(0, 0, 150, 150);
			SetWindowPos(&wndTopMost, wRc.right - eRc.Width(), wRc.top - eRc.Height(),
					eRc.Width(), eRc.Height(), SWP_SHOWWINDOW);
			GetClientRect(cRc);
			m_btnRect[0].SetRect(GAP, cRc.bottom - GAP - BTNHEIGHT, cRc.right - BTNHEIGHT - 2*GAP, cRc.bottom - GAP);
			m_btnRect[1].SetRect(m_btnRect[0].right + GAP, m_btnRect[0].top, cRc.right - GAP, cRc.bottom - GAP);
		}
		break;
	case TKIND_INTER:
		break;
	default:
		break;
	}
}

void CETicker::loadItem()
{
	loadINDEX();
	loadFCODE();

	CRect	cRc;
	m_report->GetClientRect(cRc);
	m_report->SetColumnWidth(0, cRc.Width());

	char	buf[64]{};
	CString	file, section, keys, value, codes;
	section.Format("BAR_INFO_%02d", m_id);

	file.Format("%s\\%s\\%s\\axisticker.ini", m_home, USRDIR, m_userName);
	for (int ii = 0; ; ii++)
	{
		keys.Format("%02d", ii);
		const DWORD dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
		if (dw <= 0)	break;
		value = buf;
		const int pos = value.Find(';');
		if (pos != -1)	codes = value.Left(pos);

		if (!m_arCodeByIndex.Lookup(codes, value))
			continue;
		m_report->SetItemStates(atoi(value), RC_ITEM_CHECKED);
	}
	m_report->SetBkColor(BTNCOLOR);
	m_report->SetItemBkColor(-1, -1, BTNCOLOR);
}

void CETicker::loadINDEX()
{
	CFile		fileH;
	CString		file, Code, Name, idxS;
#ifdef DF_USESTL_ETICKER
	std::unique_ptr<char[]> readB;
	std::unique_ptr<class _Index> index;
#else 
	char* readB{};
	class _Index* index{};
#endif
	
	struct	kscode* kscode{};
	
	
	file.Format("%s\\%s\\%s", m_home, TABDIR, KSCODE_FN);
  	if (!fileH.Open(file, CFile::modeRead|CFile::typeBinary))
	{
		::MessageBox(m_hWnd, "Don't read cfcode file.", "Error!", MB_ICONASTERISK);
		return;
	}

	int fileL = gsl::narrow_cast<int>(fileH.GetLength());
	if (fileL < L_INDEX)
	{
		fileH.Close();
		return;
	}

#ifdef DF_USESTL_ETICKER
	readB = std::make_unique<char[]>(fileL + 1);
	fileH.Read(readB.get(), fileL);
	fileH.Close();
	kscode = (struct kscode*)readB.get();
	for (int ii = 0; fileL >= L_INDEX; ii++)
	{
		index = std::make_unique<class _Index>();
		index->Code = CString(kscode[ii].code, L_SCOD);
		index->Name = CString(kscode[ii].name, L_INAM);

		const int	idx = m_report->GetItemCount();
		m_report->InsertItem(idx, index->Name);
		m_ary.Add((CObject*)index.get());
		idxS.Format("%d", idx);
		m_arCodeByIndex.SetAt(index->Code, idxS);
		fileL -= L_INDEX;
	}
#else 
	readB = new char[fileL + 1];
	fileH.Read(readB, fileL);
	fileH.Close();
	kscode = (struct kscode*)readB;
	for (int ii = 0; fileL >= L_INDEX; ii++)
	{
		index = new _Index;
		index->Code = CString(kscode[ii].code, L_SCOD);
		index->Name = CString(kscode[ii].name, L_INAM);

		const int	idx = m_report->GetItemCount();
		m_report->InsertItem(idx, index->Name);
		m_ary.Add((CObject*)index);
		idxS.Format("%d", idx);
		m_arCodeByIndex.SetAt(index->Code, idxS);
		fileL -= L_INDEX;
	}
	delete[] readB;
#endif
}

void CETicker::loadFCODE()
{
	CFile		fileH;
	CString		file, Code, Name, idxS;
	char* readB{}, * pBytes{};
	class _Index* index{};
	
	file.Format("%s\\%s\\%s", m_home, TABDIR, FCODE_FN);
  	if (!fileH.Open(file, CFile::modeRead|CFile::typeBinary))
	{
		::MessageBox(m_hWnd, "Don't read cfcode file.", "Error!", MB_ICONASTERISK);
		return;
	}

	const int fileL = gsl::narrow_cast<int>(fileH.GetLength());
	if (fileL < L_INDEX)
	{
		fileH.Close();
		return;
	}

	readB = new char [fileL+1];
	pBytes = readB;
	fileH.Read(readB, fileL);
	fileH.Close();

	const struct	fcode* fcode = (struct fcode *) pBytes;
	int	cnt = atoi(CString(fcode->ncom, sizeof(fcode->ncom)));
	pBytes += sizeof(fcode->ncom);

	CArray < int, int > ary;
	const struct fcomm * fcomm = (struct fcomm *) pBytes;
	for (int ii = 0; ii < cnt; ii++)
	{
		pBytes += L_FCOMM;
		ary.Add(atoi(CString(fcomm[ii].ncod, sizeof(fcomm[ii].ncod))));
	}

	struct frec* frec{};
	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		frec = (struct frec *) pBytes;
		cnt = ary.GetAt(ii);
		pBytes += cnt*L_FREC;

		index = new _Index;
		index->Code.Format("%03d", FINDEX_BASE + ii);
		index->Name = CString(frec->name, L_FNAM);
		m_ary.Add((CObject *) index);
		cnt = m_report->GetItemCount();
		m_report->InsertItem(cnt, index->Name.Mid(1));
		idxS.Format("%d", cnt);
		m_arCodeByIndex.SetAt(index->Code, idxS);
	}
	ary.RemoveAll();
	delete [] readB;
}

void CETicker::save()
{
	CString	file, section, keys, value, codes;
	section.Format("BAR_INFO_%02d", m_id);

	file.Format("%s\\%s\\%s\\axisticker.ini", m_home, USRDIR, m_userName);
	WritePrivateProfileSection(section, "", file);
	WritePrivateProfileString(section, "INDEXK", "0", file);

	_Index*	index = NULL;
	int	selectCnt = 0;
	for (int ii = 0, idx = 0; ii < m_report->GetItemCount(); ii++)
	{
		if (m_report->GetItemStates(ii) & RC_ITEM_CHECKED)
		{
			index = (_Index *) m_ary.GetAt(ii);
			keys.Format("%02d", idx++);
			value.Format("%s;%s", index->Code, index->Name);
			WritePrivateProfileString(section, keys, value, file);
			selectCnt++;
		}
	}
	if (selectCnt < 1)
	{
		index = (_Index *) m_ary.GetAt(0);
		value.Format("%s;%s", index->Code, index->Name);
		WritePrivateProfileString(section, "00", value, file);
	}

	(*m_axiscall)(AXI_EACHTICKER, 0, (LPARAM) m_id);
	PostMessage(WM_CLOSE);
}

/////////////////////////////////////////////////////////////////////////////
// CETicker message handlers

void CETicker::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect	cRc;
	GetClientRect(cRc);

	CDC	memDC;
	if (!memDC.CreateCompatibleDC(&dc))
	{
		dc.SetBkColor(BKCOLOR);
		dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
		dc.Draw3dRect(cRc, BCOLOR, BCOLOR);

		draw_Btn(&dc);
	}
	else
	{
		CBitmap	bmp, *pbmp;
		bmp.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		pbmp = (CBitmap *) memDC.SelectObject(&bmp);
		memDC.FillSolidRect(cRc, BKCOLOR);
		memDC.Draw3dRect(cRc, BCOLOR, BCOLOR);

		draw_Btn(&memDC);

		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}
}

void CETicker::draw_Btn(CDC* pDC, int idx, bool bDown)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	
	CFont*	font = pDC->SelectObject(&m_font);
	pDC->SetBkMode(TRANSPARENT);
	
	CString	text;
	text.LoadString(STSM_TEXT_OK);

	if (idx & BUTTON_OK)
	{
		pDC->FillSolidRect(m_btnRect[0], BTNCOLOR);
		pDC->DrawText(text, m_btnRect[0], DT_VCENTER|DT_CENTER|DT_SINGLELINE);
		if (bDown)
			pDC->Draw3dRect(m_btnRect[0], RGB(0, 0, 0), RGB(255, 255, 255));
		else	pDC->Draw3dRect(m_btnRect[0], RGB(255, 255, 255), RGB(0, 0, 0));
	}

	if (idx & BUTTON_CANCEL)
	{
		pDC->FillSolidRect(m_btnRect[1], BTNCOLOR);
		pDC->DrawText("X", m_btnRect[1], DT_VCENTER|DT_CENTER|DT_SINGLELINE);
		if (bDown)
			pDC->Draw3dRect(m_btnRect[1], RGB(0, 0, 0), RGB(255, 255, 255));
		else	pDC->Draw3dRect(m_btnRect[1], RGB(255, 255, 255), RGB(0, 0, 0));
	}

	pDC->SelectObject(font);
}

void CETicker::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE)
		PostMessage(WM_CLOSE);
}

void CETicker::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_btnRect[0].PtInRect(point))
	{
		m_down = BUTTON_OK;
		draw_Btn(NULL, BUTTON_OK, true);
	}
	if (m_btnRect[1].PtInRect(point))
	{
		m_down = BUTTON_CANCEL;
		draw_Btn(NULL, BUTTON_CANCEL, true);
	}

	if (m_down)	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void CETicker::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_down)
	{
		ReleaseCapture();
		if (m_down == BUTTON_OK)
			draw_Btn(NULL, BUTTON_OK);
		if (m_down == BUTTON_CANCEL)
			draw_Btn(NULL, BUTTON_CANCEL);
		m_down = 0;

		if (m_btnRect[0].PtInRect(point))
			save();
		if (m_btnRect[1].PtInRect(point))
			PostMessage(WM_CLOSE);
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

#pragma warning(default : 26400)
#pragma warning(default : 26409)
#pragma warning(default : 26401)