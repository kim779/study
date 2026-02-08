// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Market.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	P_DELI		0x7f
#define	P_TAB		0x09
#define	P_NEW		0x0a

#define	C_BACKIDX	181
#define	C_LBLBACKIDX	77
#define	C_MEDOBACKIDX	82
#define	C_MESUBACKIDX	85
#define	C_LBLBACKIDX2	78
#define	C_LINEIDX	43
#define	C_DATAFGIDX	69
#define	C_LBLFGIDX	130

COLORREF signtxtColor[] = {0x0000ff, 0xff0000};

struct	_syms
{
	char*	sym;
	char*	rts;
	int	idatatype;	//0-title, 1-data, 2-not draw
	int	imerge;
	int	imergecnt;
	UINT	ntxtFormat;
	int	idivfld;
	int	icoloridx; 
	int	idrawline;	//0-none, 1-xline, 2-yline, 3-both
	int	ifgcolor;
	int	ivisibleopt;
	char*	szetc;
	int	ilink;		//1-타이틀-1, 2-거래량, 3-외국계합
	int	irowtype;	//1-타이틀, 2-데이타부, 3-매도/매수합, 4-외국계합
} syms[] = {
	{ "1021",	"021",		2, 0, 1, DT_CENTER,	2, 0, 3, 0, 0, "",	0, 0},	// rts key

	{ "",		"증감",		0, 0, 1, DT_CENTER,	2, 0, 3, 0, 1, "",	0, 1},	// static - schg---29
	{ "",		"매도상위",	0, 1, 1, DT_CENTER,	1, 0, 3, 0, 0, "",	1, 1},	// static - svol
	{ "",		"",		0, 1, 0, DT_CENTER,	0, 0, 3, 0, 0, "",	1, 1},	// static - svol
	{ "",		"매수상위",	0, 1, 1, DT_CENTER,	1, 0, 3, 0, 0, "",	2, 1},	// static - svol
	{ "",		"",		0, 1, 0, DT_RIGHT,	0, 0, 3, 0, 0, "",	2, 1},	// static - bvol-31
	{ "",		"증감",		0, 0, 6, DT_CENTER,	0, 0, 1, 0, 1, "",	0, 1},	// static - bchg

	{ "2136",	"136",		1, 0, 0, DT_RIGHT,	0, 1, 2, 1, 1, "",	0, 2},	//6
	{ "2131",	"131",		1, 0, 0, DT_RIGHT,	1, 1, 2, 1, 0, "",	3, 2},	
	{ "2121",	"121",		1, 0, 0, DT_LEFT,	0, 1, 2, 1, 0, "",	0, 2},	
	{ "2151",	"151",		1, 0, 0, DT_LEFT,	1, 2, 2, 1, 0, "",	0, 2},	
	{ "2161",	"161",		1, 0, 0, DT_RIGHT,	0, 2, 2, 1, 0, "",	4, 2},
	{ "2166",	"166",		1, 0, 0, DT_RIGHT,	0, 2, 0, 1, 1, "",	0, 2},	


	{ "2137",	"137",		1, 0, 0, DT_RIGHT,	0, 1, 2, 1, 1, "",	0, 2},	//12
	{ "2132",	"132",		1, 0, 0, DT_RIGHT,	1, 1, 2, 1, 0, "",	3, 2},
	{ "2122",	"122",		1, 0, 0, DT_LEFT,	0, 1, 2, 1, 0, "",	0, 2},
	{ "2152",	"152",		1, 0, 0, DT_LEFT,	1, 2, 2, 1, 0, "",	0, 2},
	{ "2162",	"162",		1, 0, 0, DT_RIGHT,	0, 2, 2, 1, 0, "",	4, 2},
	{ "2167",	"167",		1, 0, 0, DT_RIGHT,	0, 2, 0, 1, 1, "",	0, 2},

	{ "2138",	"138",		1, 0, 0, DT_RIGHT,	0, 1, 2, 1, 1, "",	0, 2},	//18
	{ "2133",	"133",		1, 0, 0, DT_RIGHT,	1, 1, 2, 1, 0, "",	3, 2},
	{ "2123",	"123",		1, 0, 0, DT_LEFT,	0, 1, 2, 1, 0, "",	0, 2},
	{ "2153",	"153",		1, 0, 0, DT_LEFT,	1, 2, 2, 1, 0, "",	0, 2},
	{ "2163",	"163",		1, 0, 0, DT_RIGHT,	0, 2, 2, 1, 0, "",	4, 2},
	{ "2168",	"168",		1, 0, 0, DT_RIGHT,	0, 2, 0, 1, 1, "",	0, 2},

	{ "2139",	"139",		1, 0, 0, DT_RIGHT,	0, 1, 2, 1, 1, "",	0, 2},	//24
	{ "2134",	"134",		1, 0, 0, DT_RIGHT,	1, 1, 2, 1, 0, "",	3, 2},
	{ "2124",	"124",		1, 0, 0, DT_LEFT,	0, 1, 2, 1, 0, "",	0, 2},
	{ "2154",	"154",		1, 0, 0, DT_LEFT,	1, 2, 2, 1, 0, "",	0, 2},
	{ "2164",	"164",		1, 0, 0, DT_RIGHT,	0, 2, 2, 1, 0, "",	4, 2},
	{ "2169",	"169",		1, 0, 0, DT_RIGHT,	0, 2, 0, 1, 1, "",	0, 2},
	
	{ "2140",	"140",		1, 0, 0, DT_RIGHT,	0, 1, 3, 1, 1, "",	0, 2},	//30
	{ "2135",	"135",		1, 0, 0, DT_RIGHT,	1, 1, 3, 1, 0, "",	3, 2},
	{ "2125",	"125",		1, 0, 0, DT_LEFT,	0, 1, 3, 1, 0, "",	0, 2},
	{ "2155",	"155",		1, 0, 0, DT_LEFT,	1, 2, 3, 1, 0, "",	0, 2},
	{ "2165",	"165",		1, 0, 0, DT_RIGHT,	0, 2, 3, 1, 0, "",	4, 2},
	{ "2170",	"170",		1, 0, 0, DT_RIGHT,	0, 2, 1, 1, 1, "",	0, 2},	//35

	//TOT
	{ "",		"",		0, 0, 1, DT_RIGHT,	2, 3, 3, 1, 1, "",	0, 3},	// static - schg---36
	{ "",		"000",		0, 0, 2, DT_RIGHT,	1, 3, 3, 1, 0, "",	0, 3},	// static - svol
	{ "",		"매도합",	0, 0, 3, DT_CENTER,	0, 0, 3, 0, 0, "",	0, 3},	// static - svol
	{ "",		"매수합",	0, 0, 4, DT_CENTER,	1, 0, 3, 0, 0, "",	0, 3},	// static - svol
	{ "",		"000",		0, 0, 5, DT_RIGHT,	0, 3, 3, 1, 0, "",	0, 3},	// static - bvol-31
	{ "",		"",		0, 0, 6, DT_RIGHT,	0, 3, 1, 1, 1, "",	0, 3},	// static - bchg

	{ "",		"",		0, 0, 1, DT_RIGHT,	2, 3, 2, 1, 1, "",	0, 4},	// static - schg---29
	{ "2176",	"176",		1, 0, 2, DT_RIGHT,	1, 3, 2, 0, 0, "",	0, 4},	// static - svol
	{ "",		"외국계합",	0, 1, 1, DT_CENTER,	0, 0, 2, 0, 0, "",	5, 4},	// static - svol
	{ "",		"",		0, 1, 0, DT_CENTER,	1, 0, 2, 0, 0, "",	5, 4},	// static - svol
	{ "2177",	"177",		1, 0, 5, DT_RIGHT,	0, 3, 2, 1, 0, "",	0, 4},	// static - bvol-31
	{ "",		"",		0, 0, 6, DT_RIGHT,	0, 3, 0, 1, 1, "",	0, 4},	// static - bchg

	{ "2126",	"126",		2, 0, 0, DT_RIGHT,	0, 0, 0, 1, 0, "2121",	0, 0},	// 매도거래원 코드 --33
	{ "2127",	"127",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2122",	0, 0},
	{ "2128",	"128",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2123",	0, 0},
	{ "2129",	"129",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2124",	0, 0},
	{ "2130",	"130",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2125",	0, 0},

	{ "2156",	"156",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2151",	0, 0},	// 매수거래원 코드
	{ "2157",	"157",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2152",	0, 0},
	{ "2158",	"158",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2153",	0, 0},
	{ "2159",	"159",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2154",	0, 0},
	{ "2160",	"160",		2, 0, 0, DT_RIGHT,	0, 0, 3, 1, 0, "2155",	0, 0},
};

#define	CNT_syms	sizeof(syms)/sizeof(struct _syms)

#define	oopTR	"POOPPOOP"
#define	oKEY	0x01
#define	bKEY	0x02

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	m_strLinkMap = "";
	m_bRecv = FALSE;
	m_bTotShow = TRUE; //매도합/매수합줄 보이기
}

CControlWnd::~CControlWnd()
{
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_arDrawList.RemoveAll();
	m_arBefDrawList.RemoveAll();
	m_arMkCodeList.RemoveAll();

	m_RealMap.RemoveAll();
	m_iDivfldWCnt = 2;
	m_iDivfldHCnt = 0;

	CString sIdx;

	for (int ii=1; ii<CNT_syms; ii+=6)
	{
		for (int c=ii; c<ii+6; c++)
		{
			if (syms[ii].idatatype == 2)
			{
				m_arMkCodeList.Add(MarketList(1, syms[c].sym, syms[c].szetc, syms[c].rts));
				continue;
			}

			if (syms[ii].idatatype != 2)
			{
				if (syms[c].idivfld == 2)m_iDivfldHCnt++;
				if (!m_iVisibleType) 
				{
					if (syms[c].ivisibleopt) continue;
				}

				if (!m_bTotShow)
				{
					if (syms[c].irowtype == 3) continue;
				}

				
				m_arDrawList.Add(MarketList(syms[ii].idatatype, syms[c].sym, syms[c].rts,  CRect(0,0,0,0),
								syms[c].ntxtFormat, syms[c].imerge, syms[c].imergecnt,
								m_clrColorIdx[syms[c].icoloridx], m_clrColor[syms[c].icoloridx] , syms[c].idrawline,
								syms[c].ifgcolor, syms[c].ivisibleopt, syms[c].ilink));

				sIdx.Format("%d", m_arDrawList.GetUpperBound());
				m_RealMap.SetAt(syms[c].rts, sIdx);
			}

			if (CNT_syms <= ii) break;
		}
	}	
	return 0;
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC	backdc;
	backdc.CreateCompatibleDC(&dc); 
	CBitmap bitmap,  *pOldBitmap;
	CRect rectClient;
	GetClientRect(rectClient);

	bitmap.CreateCompatibleBitmap(&dc, rectClient.right, rectClient.bottom);
	pOldBitmap = (CBitmap*)backdc.SelectObject(&bitmap);
	backdc.SetBkMode(TRANSPARENT);
	
	backdc.FillSolidRect(rectClient, m_clrBgColor); 
	CFont* pOldFont = backdc.SelectObject(m_pFont);	



	Draw(&backdc, rectClient);
	dc.BitBlt(rectClient.left , rectClient.top , rectClient.right , rectClient.bottom , &backdc, 0,0, SRCCOPY);
	bitmap.DeleteObject(); 
	backdc.SelectObject(pOldBitmap);
	backdc.SelectObject(pOldFont); 
	DeleteObject(pOldFont);
	DeleteObject(pOldBitmap);
	ReleaseDC(&backdc);		
	// Do not call CWnd::OnPaint() for painting messages
}


void CControlWnd::Draw(CDC* pDC, CRect rcClient)
{
	MarketList plist;
	CRect	rc, rctmp;
	int	itop = 0;
	CString sText;
	int	icolor = 0;
	int	itop2 = 0, ibefidx = 0;
	int	ibefw[15]{};
	CPen	pen(PS_SOLID, 1, m_clrLineColor);
	CPen*	 pOldPen = pDC->SelectObject(&pen); 

	m_maedo.RemoveAll();
	m_maesu.RemoveAll();
	for (int jj = 0; jj < m_arDrawList.GetSize(); jj++)
	{
		plist = m_arDrawList.GetAt(jj);
		if (plist.rc.Width() <= 0)
			continue;

		rctmp = plist.rc;
		if (plist.imerge)
		{
			if (plist.imergecnt > 0)
				rctmp.right = m_arDrawList.GetAt(jj+1).rc.right;
			else	continue;
		}

		if (jj > 5)
		{
			if (ibefidx < 15)
				ibefw[ibefidx++] = plist.rc.right -1 ; 
		}

		if (rctmp.bottom +m_iDivH >= rcClient.bottom)
			rctmp.bottom += m_iDivH;

		if (rctmp.right + 2 >= rcClient.right) 
			rctmp.right = rcClient.right+1;

		rc = rctmp;
		if (rctmp.left <= 2)
			rctmp.DeflateRect(0,1,1,1); 
		else 
			rctmp.DeflateRect(1,1,1,1); 

		if (plist.iline == 0 || plist.iline == 2)
			rctmp.InflateRect(0,0,0,1); 

		pDC->FillSolidRect(rctmp, plist.bgcolor); 
		if (plist.iline == 2 || plist.iline == 3)
		{
			pDC->MoveTo(rctmp.right, rc.top);
			pDC->LineTo(rctmp.right, rc.bottom);
		}

		if (plist.iline == 1 || plist.iline == 3)
		{
			pDC->MoveTo(rctmp.left, rctmp.bottom);
			pDC->LineTo(rctmp.right, rctmp.bottom); 
		}

		itop2 = max(itop2, rctmp.bottom);
		if (plist.ifgcolor)
			pDC->SetTextColor(m_clrDataFgColor); 
		else	pDC->SetTextColor(m_clrLblFgColor); 

		rc = rctmp;
		rc.DeflateRect(2,0,2,-2); 

		sText = SetComma(plist.sText);
		if (sText.FindOneOf("+-") > -1)
		{
			sText = sText.Mid(1);
			if (plist.symbol.Left(3) == "212")
				icolor = 1;
			else	icolor= 0;
			pDC->SetTextColor(signtxtColor[icolor]); 
		}

		if (plist.symbol == "2176") pDC->SetTextColor(signtxtColor[1]); 
		if (plist.symbol == "2177") pDC->SetTextColor(signtxtColor[0]); 
	
		if (!sText.Trim().IsEmpty())
		{
			CString s, tmp = sText;

			tmp.Remove(',');
			if (!isNumber(tmp))
			{
				if (jj % 2 == 0)
				{
					for (int kk = 0; kk < m_maedo.GetSize(); kk++)
					{
						s = m_maedo.GetAt(kk);
						if (s.Trim() == sText)
						{
							MessageBox("중복된 회원사가 있읍니다.김덕기대리에게 전달해 주세요.","회원사 중복시험",MB_OK);
							return;
						}
					}

					m_maedo.Add(sText);
				}
				else
				{
					for (int kk = 0; kk < m_maesu.GetSize(); kk++)
					{
						s = m_maesu.GetAt(kk);
						if (s.Trim() == sText)
						{
							MessageBox("중복된 회원사가 있읍니다.김덕기대리에게 전달해 주세요.","회원사 중복시험",MB_OK);
							return;
						}
					}

					m_maesu.Add(sText);
				}
			}
		}

		pDC->DrawText(sText, rc, plist.nTxtFormat);  
	}

	if (m_iOptBefore)
	{
		CString sTitle[] = {"매도율", "", "전일매도상위", "", "전일매수상위", "매수율", "" };
		const	int	irowH = (rcClient.bottom - itop2) / 6;
		CRect	rr = CRect(rcClient.left, itop2+1, rcClient.right, itop2 + irowH);

		pDC->FillSolidRect(rr, m_clrColor[0]); 
		pDC->MoveTo(rcClient.left, itop2);
		pDC->LineTo(rcClient.right, itop2); 
		
		itop = itop2;
		const	int	ikey = 1;
		int	ileft = 0, icolor = 1;

		for (int ii = 0; ii < 6; ii++)
		{
			if (sTitle[ii] == "")
				itop = itop2 + irowH ;
			else	itop = itop2;

			rr = CRect(ileft+1, itop2 + irowH+1, ibefw[ii], rcClient.bottom-1);
			pDC->FillSolidRect(rr, m_clrColor[icolor]); 
			pDC->MoveTo(ibefw[ii], itop);
			pDC->LineTo(ibefw[ii], rcClient.bottom-1); 

			ileft = ibefw[ii];
			if (ii > ikey)
				icolor = 2;
		}

		pDC->MoveTo(rcClient.left, itop2 + irowH);
		pDC->LineTo(rcClient.right, itop2 + irowH); 

		pDC->SetTextColor(m_clrLblFgColor); 
		ileft = 0;
		for (int ii = 0;ii < 6; ii++)
		{
			if (!sTitle[ii].IsEmpty())
			{
				rr = CRect(ileft+1, itop2+1, ibefw[ii], itop2 + irowH);
				if (rr.right -2 >= rcClient.right)
					rr.right = rcClient.right;
				rr.DeflateRect(0, 1, 0, 0); 
				pDC->DrawText(sTitle[ii], rr, DT_CENTER|DT_SINGLELINE|DT_VCENTER);  
				ileft = ibefw[ii];
			}
		}

		//DATA
		itop = itop2 + irowH;
		ileft = 0;

		UINT	nFormat = 0;
		for (int kk = 0; kk < 6; kk++)
		{
			ileft = 0;
			for (int ii = 0; ii < 6; ii++)
			{
				rr = CRect(ileft, itop+1, ibefw[ii], itop + irowH);
				if (rr.right >= rcClient.right)
					rr.right = rcClient.right-1;
				nFormat = DT_RIGHT|DT_SINGLELINE|DT_VCENTER;
				sText = m_strBeforeList[kk][ii];
				if (sText.IsEmpty())
					continue; 

				pDC->SetTextColor(m_clrDataFgColor); 
				if (ii == 2|| ii == 3)
				{
					nFormat = DT_LEFT|DT_SINGLELINE|DT_VCENTER;
					if (sText.FindOneOf("+-") > -1)
					{
						sText = sText.Mid(1);
						icolor = (ii == 2) ? 1 : 0;
						pDC->SetTextColor(signtxtColor[icolor]); 
					}
				}
	
				rr.DeflateRect(2,1,2,1); 
				pDC->DrawText(sText, rr, nFormat);  
				ileft = ibefw[ii];
			}
			itop += irowH;
		}
	}

	pDC->MoveTo(rcClient.left, rcClient.top);
	pDC->LineTo(rcClient.right, rcClient.top); 

	pDC->MoveTo(rcClient.left, rcClient.bottom-1);
	pDC->LineTo(rcClient.right, rcClient.bottom-1); 

	pDC->SelectObject(pOldPen); 
	DeleteObject(pOldPen);
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	ResizePanelBox();
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	MarketList plist;
	CString sLinkMap, sData;
	BOOL	bEvent = FALSE;

	for (int ii = 0; ii < m_arMkCodeList.GetSize(); ii++)
	{
		plist = m_arMkCodeList.GetAt(ii);
		if (plist.rc.PtInRect(point))
		{
			if (!m_strLinkMap.IsEmpty())
			{
				if (plist.sText.IsEmpty())
					continue;

				bEvent = TRUE;
				sData.Format("1301\t%s\ned_trcd\t%s", m_strCode, plist.sText);
				sLinkMap.Format("%s/t/s/p5/d%s ", m_strLinkMap,  sData);
				SendAxisMessage(MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)sLinkMap.GetString());
			}
			break;
		}
	}

	if (!bEvent)
	{
		for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
		{
			plist = m_arDrawList.GetAt(ii);
			if (plist.ilink > 0 && !m_strLinkMapX[plist.ilink].IsEmpty())
			{
				if (plist.rc.PtInRect(point))
				{
					
					sData.Format("1301\t%s", m_strCode);
					if (plist.ilink == 5)
						sData.Format("1301\t%s\nTRAD\t%s", m_strCode, "888");

					sLinkMap.Format("%s/t/s/p5/d%s ", m_strLinkMapX[plist.ilink],  sData);
					SendAxisMessage(MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)sLinkMap.GetString());
					break;
				}
			}
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	MarketList plist;
	CString sLinkMap, sData;
	BOOL	bEvent = FALSE;

	for (int ii = 0; ii < m_arMkCodeList.GetSize(); ii++)
	{
		plist = m_arMkCodeList.GetAt(ii);
		if (plist.sText.IsEmpty())
			continue;

		if (plist.rc.PtInRect(point))
		{
			if (!m_strLinkMap.IsEmpty())
			{
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
				bEvent = TRUE;
			}
			break;
		}
	}

	if (!bEvent)
	{
		for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
		{
			plist = m_arDrawList.GetAt(ii);
			if (plist.sText.IsEmpty())
				continue;

			if (plist.ilink > 0 && !m_strLinkMapX[plist.ilink].IsEmpty())
			{
				if (plist.rc.PtInRect(point))
				{
					::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
					break;
				}
			}
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CControlWnd::SetParam(struct _param* param, CWnd *parent)
{
	m_pMainWindow = parent;
	m_key = param->key;
	m_strParamName = CString(param->name, param->name.GetLength());
	m_pFont = getAxFont(_T("굴림체"), 9);

	m_clrBgColor = getAxColor(C_BACKIDX);

	m_clrColorIdx[0] = C_LBLBACKIDX;
	m_clrColorIdx[1] = C_MEDOBACKIDX;
	m_clrColorIdx[2] = C_MESUBACKIDX;
	m_clrColorIdx[3] = C_LBLBACKIDX2;
	m_clrColorIdx[4] = C_LINEIDX;
	m_clrColorIdx[5] = C_DATAFGIDX;
	m_clrColorIdx[6] = C_LBLFGIDX;

	m_clrColor[0] = getAxColor(C_LBLBACKIDX);
	m_clrColor[1] = getAxColor(C_MEDOBACKIDX);
	m_clrColor[2] = getAxColor(C_MESUBACKIDX);
	m_clrColor[3] = getAxColor(C_LBLBACKIDX2);
	m_clrLineColor = getAxColor(C_LINEIDX);

	m_clrDataFgColor = getAxColor(C_DATAFGIDX);
	m_clrLblFgColor = getAxColor(C_LBLFGIDX);

	for (int ii = 0; ii < 10; ii++)
		m_strLinkMapX[ii] = _T("");

	CString sopt = param->options;
	m_strLinkMap = "IB230200";
	m_iOptBefore = 0;
	if (!sopt.IsEmpty())
	{
		m_iVisibleType = CInt(GetFindOptionString(sopt, "s", "/"));
		m_linkCursor = CInt(GetFindOptionString(sopt, "c", "/"));
		m_strLinkMap = GetFindOptionString(sopt, "l", "/") ;
//1-타이틀-1, 2-거래량, 3-외국계합
		m_strLinkMapX[1] = GetFindOptionString(sopt, "l1", "/") ;
		m_strLinkMapX[2] = GetFindOptionString(sopt, "l2", "/") ;
		m_strLinkMapX[3] = GetFindOptionString(sopt, "l3", "/") ;
		m_strLinkMapX[4] = GetFindOptionString(sopt, "l4", "/") ;
		m_strLinkMapX[5] = GetFindOptionString(sopt, "l5", "/") ;

		m_iOptBefore =  CInt(GetFindOptionString(sopt, "b", "/"));
		if (m_iOptBefore)
			m_iVisibleType = 1;

		CString sval = GetFindOptionString(sopt, "t", "/");
		sval.Trim();
		if (!sval.IsEmpty()) 
			m_bTotShow = CInt(sval);
	}
}

COLORREF CControlWnd::getAxColor(int color)
{
	if (color & 0x02000000)
		return color;
	return SendAxisMessage(MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CFont* CControlWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct	_fontR	fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)SendAxisMessage(getFONT, (LPARAM)&fontR);
}

LRESULT CControlWnd::SendAxisMessage(WPARAM wParam, LPARAM lParam)
{
	return m_pMainWindow->SendMessage(WM_USER, wParam, lParam);
}

LONG CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		ParseRecvData(exth->key, exth->data);
		WriteLog((char*)exth->data, false);
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		break;
//	case DLL_ALERT:
//		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		alertR = (struct _alertR*)lParam;
		ParseRealData(alertR);
		break;
	case DLL_SETPAL:
		{
			m_clrBgColor = getAxColor(C_BACKIDX);
			m_clrColor[0] = getAxColor(C_LBLBACKIDX);
			m_clrColor[1] = getAxColor(C_MEDOBACKIDX);
			m_clrColor[2] = getAxColor(C_MESUBACKIDX);
			m_clrColor[3] = getAxColor(C_LBLBACKIDX2);
			m_clrLineColor = getAxColor(C_LINEIDX);

			m_clrDataFgColor = getAxColor(C_DATAFGIDX);
			m_clrLblFgColor = getAxColor(C_LBLFGIDX);

			MarketList plist;
			for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
			{
				plist = m_arDrawList.GetAt(ii);
				plist.bgcolor  = getAxColor(plist.ibgidx);
				m_arDrawList.SetAt(ii, plist);

			}
			Invalidate(FALSE);
		}
		break;

	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			m_pFont = getAxFont(CString((char*)lParam), HIWORD(wParam));
			Invalidate(FALSE);
		}
 		break;

	case DLL_GUIDE:
		return 1;

	case DLL_TRIGGER: //설정확인도 여기서
	case DLL_DOMINO:
	default:break;
	}

	return 0;
}

void CControlWnd::ParseRealData(struct _alertR* alertR)
{
	if (!m_bRecv || m_arDrawList.GetSize() <= 0)
		return;
	
	CString sRealCode = alertR->code;
	if (sRealCode.IsEmpty() || m_strRealCode != sRealCode)
		return;
	
	CMapStringToString arMapList;
	MarketList plist;
	CString sval;
	int	irts = 0;
	DWORD*	data = nullptr;
	
	for (int kk = alertR->size-1; kk >= 0; kk--)
	{
		data = (DWORD*)alertR->ptr[kk];
		for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
		{
			plist = m_arDrawList.GetAt(ii);
			if (plist.symbol.IsEmpty())
				continue;
			
			irts = atoi(plist.srtsSymbol);
			if (data[irts])
			{
				sval = (char*)data[irts];
				sval.Trim();
				if (sval == "0")
					sval = "";
				
				plist.sText = sval;
				m_arDrawList.SetAt(ii, plist);
				InvalidateRect(plist.rc); 
			}
		}
		
		
		for (int ii = 0; ii < m_arMkCodeList.GetSize(); ii++)
		{
			plist = m_arMkCodeList.GetAt(ii);
			irts = atoi(plist.srtsSymbol);
			if (data[irts])
			{
				sval = (char*)data[irts];
//				sval = Trim(sval);
				plist.sText = MakeMKCode(sval);
				m_arMkCodeList.SetAt(ii, plist);
			}
		}
	}
	
	SetTotValue(TRUE);
}

CString CControlWnd::MakeMKCode(CString sCode)
{
	CString sMkCode = sCode;

	sMkCode.Trim();
	if (sMkCode.IsEmpty() || sMkCode.GetLength() >= 3)
		return sMkCode;

	CString sTmp;
	const	int	iend = 3 - sMkCode.GetLength();
	for (int ii = 0; ii < iend; ii++)
		sTmp += "0";

	return sTmp + sMkCode;
}

CRect CControlWnd::GetMkRect(CString sSymbol)
{
	MarketList plist;
	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		if (plist.symbol == sSymbol)
			return plist.rc ;
	}

	return CRect(0,0,0,0);
}

void CControlWnd::ParseRecvData(int key, CString sData)
{
	if (key == bKEY)
	{
		int	iaridx = 2;

		CString sList[50], sResult[50];
		for (int ii = 0; ii < 2; ii++)
		{
			//char    trnm[20];   /* 거래원 명
			sList[0] = sData.Left(5*20); //-----------------2, 3
			sData    = sData.Mid(100);  
			SplitDivString(sList[0], sResult, 20);
			for (int jj = 0; jj < 5; jj++)
				m_strBeforeList[jj][iaridx] = sResult[jj].Trim();

			//char    tvol[12];   /* 거래(매수,매도)량    */-------------------1, 4
			sList[1] = sData.Left(5*12); 
			sData    = sData.Mid(60);  
			SplitDivString(sList[1], sResult, 12);
			if (ii > 0)
				iaridx++;
			else	iaridx--;
			for (int jj = 0; jj < 5; jj++)
				m_strBeforeList[jj][iaridx] = SetComma(sResult[jj].Trim());

			//char    trat[8];    /* 거래(매수,매도)비율  *//--------------0, 5
			sList[2] = sData.Left(5*8); 
			sData    = sData.Mid(40);  
			SplitDivString(sList[2], sResult, 8);
			if (ii > 0)
				iaridx++;
			else	iaridx--;
			for (int jj = 0; jj < 5; jj++)
				m_strBeforeList[jj][iaridx] = sResult[jj].Trim();

			//char    trad[8];    /* 거래원코드          *//
			sList[3] = sData.Left(5*8); 
			sData    = sData.Mid(40);  

			iaridx = 3;
		}

		SendTR(m_strSendCode);
		return;
	}

	CStringArray astrResult;
	MarketList plist;
	CString sTmp;
	int	idx = 1;

	m_RealMkCodeMap.RemoveAll();
	if (SplitString(sData, (char)P_TAB, astrResult) > 0)
	{
		astrResult[0].Trim();
		if (astrResult[0].GetLength() < 6)
			return;
		
		m_strRealCode = astrResult[0];
		for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
		{
			plist = m_arDrawList.GetAt(ii);
			if (plist.symbol.IsEmpty())
				continue;

			if (astrResult.GetSize() <= idx)
				break; 

			plist.sText = astrResult[idx++];
			plist.sText.Trim();
			if (plist.sText == "0")
				plist.sText = "";
			m_arDrawList.SetAt(ii, plist);
		}

		for (int ii = 0; ii < m_arMkCodeList.GetSize(); ii++)
		{
			if (astrResult.GetSize() <= idx)
				break;
			
			plist = m_arMkCodeList.GetAt(ii);
			if (plist.srtsSymbol == "000")
				continue;
			if (plist.symbol.IsEmpty())
				continue;

			plist.sText = astrResult[idx]; idx++;
			plist.sText.Trim();

			plist.rc = GetMkRect(plist.sEtc);
			m_arMkCodeList.SetAt(ii, plist);
		}
	}

	SetTotValue();
	Invalidate();
	m_bRecv = TRUE;
}

void CControlWnd::SetTotValue(BOOL bRedraw)
{
	if (!m_bTotShow)
		return;

	int	itot = 0, itmp = 0;
	MarketList plist;

	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		if (plist.srtsSymbol == "000")
		{
			itot = 0;
			itmp = ii-(5*m_ivisiblecnt);
			for (int jj = 0; jj < 5; jj++)
			{
				if (m_arDrawList.GetSize() <= itmp)
					break;

				itot += CInt(m_arDrawList.GetAt(itmp).sText);
				itmp += m_ivisiblecnt;
			}

			if (itot <= 0)
				plist.sText = "";
			else	plist.sText.Format("%d", itot); 
			m_arDrawList.SetAt(ii, plist);
			if (bRedraw)
				InvalidateRect(plist.rc); 
		}
	}
}

void CControlWnd::ResizePanelBox()
{
	CRect	rcClient, rc;

	m_ivisiblecnt = (!m_iVisibleType) ? 4 : 6;
//0.22
	GetClientRect(&rcClient);

	int	ivolw = (int)(double(rcClient.Width()) * .28f);
	int	iw = rcClient.Width() / m_ivisiblecnt;

	if (m_ivisiblecnt == 6)
	{
		iw = (rcClient.Width()-ivolw) / 4;
		ivolw /= 2;
	}
	else	ivolw = iw;

	int	idivW = (rcClient.Width()) % m_ivisiblecnt;
	if (idivW > 1)
		idivW /= m_iDivfldWCnt;

	int	irows = 8;
	if (m_iOptBefore)
		irows = 14;
	if (!m_bTotShow)
		irows -= 1;

	const	int ih = rcClient.Height() / irows;
	int	idivH = rcClient.Height() % irows;

	m_iDivH = idivH;
	m_iDivW = idivH;
	if (idivH > 1)
		idivH /= m_iDivfldHCnt;

	const	int	itop = rcClient.top;

	rc = rcClient;
	rc.left = rcClient.left;
	rc.right = rcClient.left+ ivolw;
	if (!m_iVisibleType)
		rc.right += idivW;
	rc.top = itop;
	rc.bottom = itop + ih;

	MarketList plist;
	for (int ii = 0; ii < m_arDrawList.GetSize(); ii+=m_ivisiblecnt)
	{
		for (int jj = ii; jj < ii + m_ivisiblecnt; jj++)
		{
			if (m_iVisibleType) 
			{
				if (syms[jj].idivfld == 1) //cx
				{
					rc.right += idivW;
				}
			}

			plist = m_arDrawList.GetAt(jj);
			plist.rc = rc;
			
			m_arDrawList.SetAt(jj, plist);

			rc.left = rc.right-1;
			rc.right = rc.left+ iw+1;

			if (m_arDrawList.GetSize() <= jj)
				break;
		}

		rc.left = rcClient.left-1;
		rc.right = rcClient.left+ ivolw;
		if (!m_iVisibleType)
			rc.right += idivW;
		rc.top = rc.bottom-1;
		rc.bottom = rc.top + ih+ 1;
		if (syms[ii].idivfld == 2 && m_iOptBefore)
			rc.bottom += idivH;
	}

	for (int ii = 0; ii < m_arMkCodeList.GetSize(); ii++)
	{
		plist = m_arMkCodeList.GetAt(ii);
		plist.rc = GetMkRect(plist.sEtc);
		m_arMkCodeList.SetAt(ii, plist);
	}
}

void CControlWnd::SendTR(CString sCode)
{
	CString	text, code, stmp;
	MarketList plist;

	m_bRecv = FALSE;
	stmp.Format("1301%c%s\t1021\t", 0x7f, sCode);

	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		if (plist.symbol.IsEmpty())
			continue;

		if (plist.iType == 1 && plist.srtsSymbol == plist.sText)
			plist.sText = "";
		if (plist.sText  == "000")
			plist.sText = "";
		m_arDrawList.SetAt(ii, plist);

		stmp += plist.symbol;
		stmp += '\t';
	}

	for (int ii = 0; ii < m_arMkCodeList.GetSize(); ii++)
	{
		plist = m_arMkCodeList.GetAt(ii);
		if (plist.symbol.IsEmpty())
			continue;

		stmp += plist.symbol;
		stmp += '\t';
	}

	text.Format("%s%c%c", stmp, P_NEW, P_TAB); 
	sendTR(oopTR, (char *)text.GetString(), text.GetLength(), oKEY, US_OOP);
}

void CControlWnd::SendTR_Before(CString sCode)
{
	if (m_strSendCode.IsEmpty())
		return;

	char	sndb[64]{};
	int	sndl = 0;
	CString	tmps;

	tmps.Format("%s%c", sCode, P_DELI);		// 종목코드
	CopyMemory(&sndb[sndl], tmps, tmps.GetLength());
	sndl += tmps.GetLength();

	sndb[sndl++] = P_NEW;
	sndb[sndl++] = P_TAB;
	sndb[sndl]   = 0x00;

	sendTR("bo200522", sndb, sndl, bKEY, 0);
}

void CControlWnd::sendTR(CString trCode, char* sndb, int sndl, UINT nKey, int istat)
{
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(sndl + L_userTH + 128);
	struct	_userTH	udat {};
	int	idx = 0;

	// key & control name
	pWb[idx++] = (char)nKey;
	CopyMemory(&pWb[idx], m_strParamName, m_strParamName.GetLength());
	idx += m_strParamName.GetLength();
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, trCode, min(sizeof(udat.trc), trCode.GetLength()));
	udat.key  = m_key;
	udat.stat = istat | US_KEY;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], sndb, sndl);

	SendAxisMessage(MAKEWPARAM(invokeTRx, sndl), (LPARAM)sndb);
}

void CControlWnd::Send(LPCTSTR sCode) 
{
	if (strlen(sCode) == 0 ||  !m_strCode.CompareNoCase(sCode))
		return;

	m_bRecv = FALSE;
	m_strSendCode = sCode;

	for (int ii = 0; ii < 6; ii++)
	{
		for (int jj = 0; jj < 6; jj++)
			m_strBeforeList[ii][jj] = "";
	}

	MarketList plist;
	for (int ii = 0; ii < m_arMkCodeList.GetSize(); ii++)
	{
		plist = m_arMkCodeList.GetAt(ii);
		if (plist.symbol.IsEmpty() || plist.srtsSymbol == "000")
			continue;

		plist.sText = "";
		m_arMkCodeList.SetAt(ii, plist);
	}

	Reset();
	if (m_iOptBefore)
		SendTR_Before(m_strSendCode);
	else	SendTR(sCode);

	m_strCode = CString(sCode);
}

void CControlWnd::Reset()
{
	for (int ii = 0; ii < 6; ii++)
	{
		for (int jj = 0; jj < 6 ; jj++)
			m_strBeforeList[ii][jj] = "";
	}

	MarketList plist;
	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		if (!plist.symbol.IsEmpty())
		{
			plist.sText = "";
			m_arDrawList.SetAt(ii, plist);
		}
	}
}

void CControlWnd::WriteLog(CString msg, bool bAppend)
{
	CFile	file;
	UINT	nOpenFlags = CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone;

	if (!bAppend)
		nOpenFlags |= CFile::modeNoTruncate;

	if (file.Open("MARKET_DEBUG.log", nOpenFlags))
	{
		file.SeekToEnd();
		file.Write(msg,msg.GetLength());
		file.Flush();
		file.Close();
	}
}