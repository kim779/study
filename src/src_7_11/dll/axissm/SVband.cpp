// SVband.cpp : implementation file
//

#include "stdafx.h"
#include "SVband.h"
#include "skindlg.h"
#include "NButton.h"
#include "PalButton.h"
#include "../../h/axisvar.h"
#include "../../axis/axMsg.hxx"

#include "resource.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SKIN_CNT	6
#define	GAP		5

#define DEFAULT_W	20
#define IMG_GAP		3
#define BGAP		2


#define	SELECT_COLOR	RGB(255, 255, 255)
#define	NONE_COLOR	RGB(208, 233, 237)

#define IDX_SKIN	0
#define IDX_VS		1
#define IDX_SETUP	2
#define IDX_DEAL	3
#define IDX_MAX		4

#define	MODE_COOL	0
#define	MODE_OVER	1
#define	MODE_DOWN	2

#define VSCREENCNT	4

#define	CTRL_VS1	0x51
#define	CTRL_VS2	0x52
#define	CTRL_VS3	0x53
#define	CTRL_VS4	0x54
#define	CTRL_VS5	0x55
#define	CTRL_VS6	0x56
#define CTRL_PAL	0x57
#define CTRL_CHANGE	0x58

/////////////////////////////////////////////////////////////////////////////
// CSVband
CSVband::CSVband(bool (*callback)(int, WPARAM, LPARAM), CString home)
{
	m_axiscall = callback;
	m_home = home;
	m_skinN = 0;
	m_pushN = -1;
	m_bk = RGB(242, 242, 242);

	m_vsN = V_SCREEN1;

	m_bshape = false;
	loadPalette();

	m_dlg = new CSkinDlg(m_axiscall, home);
	m_dlg->Create(IDD_SELECTSKIN);
	m_dlg->ChangeInfo();
	m_dlg->SetFocus();

	for (int ii = 0; ii < 4; ii++)
	{
		m_pVS[ii] = NULL;
		m_pFunc[ii] = NULL;
	}
	m_pPal = NULL;
	m_mode = 0;
}

CSVband::~CSVband()
{
	m_ary.RemoveAll();
	if (m_dlg)	delete m_dlg;

	for (int ii = 0; ii < 4; ii++)
	{
		if (m_pVS[ii])		delete m_pVS[ii];
		if (m_pFunc[ii])	delete m_pFunc[ii];	}
	if (m_pPal)	delete m_pPal;
}


BEGIN_MESSAGE_MAP(CSVband, CWnd)
	//{{AFX_MSG_MAP(CSVband)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSVband message handlers

void CSVband::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect	 cRc;
	GetClientRect(cRc);

	dc.FillSolidRect(cRc, COLOR_FRAME);
//	draw_Seperate(&dc, cRc);

	move_Ctrl();
}

void CSVband::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CSVband::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
}

void CSVband::changeSkin(CString skinName)
{
	for (int ii = 0; ii < m_ary.GetSize(); ii++)
	{
		if (!skinName.CompareNoCase(m_ary.GetAt(ii)))
		{
			m_skinN = ii;
			m_pPal->set_PalColor(m_aryColor.GetAt(m_skinN), m_aryLine.GetAt(m_skinN));
			m_dlg->ChangeInfo();

			break;
		}
	}
}

void CSVband::set_Background(COLORREF color)
{
	m_bk = color;
	Invalidate(FALSE);
}

CSize CSVband::GetBandWidth()
{
	switch (m_mode)
	{
	default:
	case 0:		return CSize(291, 19);
	case 1:		return CSize(147, 19);
	case 2:		return CSize(104, 19);
	}
}

void CSVband::loadPalette()
{
	int ii = 0;
	int	pos;
	char	buffer[128];
	CString	file, tmpS, dat, skin, value;
	int	r, g, b;

	m_ary.RemoveAll();
	m_aryColor.RemoveAll();
	m_aryLine.RemoveAll();

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	DWORD dw = GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);
	if (dw <= 0)	skin = _T("");
	else		skin = buffer;
	dw = GetPrivateProfileString(GENERALSN, "Palettes", "", buffer, sizeof(buffer), file);
	if (dw <= 0)	return;

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			dat.TrimLeft();
			dat.TrimRight();
			m_ary.Add(dat);

			tmpS = dat;
			dat = "";
		}
		else
		{
			tmpS = dat.Left(pos++);
			dat = dat.Mid(pos);
			tmpS.TrimLeft();
			tmpS.TrimRight();
			m_ary.Add(tmpS);
		}
		GetPrivateProfileString(tmpS, "RGB120", "119, 119, 119", buffer, sizeof(buffer), file);
		value = buffer;
		int pos = value.Find(',');
		if (pos == -1)	r = 119;
		else
		{
			tmpS = value.Left(pos++);
			value = value.Mid(pos);
			r = atoi(tmpS);
		}
		
		pos = value.Find(',');
		if (pos == -1)	g = 119;
		else
		{
			tmpS = value.Left(pos++);
			value = value.Mid(pos);
			g = atoi(tmpS);
		}
		
		value.TrimLeft();
		value.TrimRight();
		if (value.IsEmpty())	b = 119;
		else	b = atoi(value);

		m_aryColor.Add(RGB(r, g, b));
	}

	DWORD lines[] = { RGB(61,97,177), RGB(79,149,147), RGB(87,128,78), RGB(115,113,68), RGB(109,69,64), RGB(111,86,131), RGB(124,141,173) };
	for (ii = 0; ii < 7; ii++)
	{
		m_aryLine.Add(lines[ii]);
	}

	for (ii = 0; ii < m_ary.GetSize(); ii++)
	{
		if (!skin.CompareNoCase(m_ary.GetAt(ii)))
		{
			m_skinN = ii;
			break;
		}
	}
}


void CSVband::draw_Seperate(CDC* pDC, CRect rc)
{
	CRect	sRc, cRc;

	GetClientRect(cRc);
	sRc.SetRect(rc.left+1, cRc.top + BGAP, rc.left+3, cRc.bottom - BGAP);
	pDC->Draw3dRect(sRc, GRIP_RB, GRIP_LT);
}

void CSVband::make_Ctrl()
{
	CString		imgC, imgO, imgD, text;
	DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

// 	m_pChange = new CNButton(CTRL_CHANGE);
// 	if (!m_pChange->Create(NULL, "버튼 숨기기", dwStyle, CRect(0, 0, 0, 0), this, -1))
// 	{
// 		delete m_pChange;
// 		m_pChange = NULL;
// 	}
// 	else
// 	{
// 		m_pChange->set_ToolTipText("버튼 숨기기");
// 		imgC.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_change");
// 		imgO.Format("%s\\%s\\%s_en.bmp", m_home, "image", "mb_change");
// 		imgD.Format("%s\\%s\\%s_dn.bmp", m_home, "image", "mb_change");
// 		m_pChange->set_Image(imgC, imgO, imgD);
// 	}	

// 	CString tooltip[] = { "설정", "프린트", "홈페이지", "화면잠금", "증권방송", "해외증시티커", "" };
// 	CString file[] = { "mb_setting", "PRINT", "HOMEPAGE", "LOCK", "mb_tv", "mb_radio", "" };
// 	int	ids[] = { CTRL_ENVIRONMENT, CTRL_SCREENPRINT, CTRL_HOMEPAGE, CTRL_LOCK, CTRL_TV, CTRL_RADIO };
// 	for (int ii = 0; !tooltip[ii].IsEmpty(); ii++)
// 	{
// 		m_pFunc[ii] = new CNButton(ids[ii]);
// 		if (!m_pFunc[ii]->Create(NULL, tooltip[ii], dwStyle, CRect(0, 0, 0, 0), this, -1))
// 		{
// 			delete m_pFunc[ii];
// 			m_pFunc[ii] = NULL;
// 		}
// 		else
// 		{
// 			m_pFunc[ii]->set_ToolTipText(tooltip[ii]);
// 			imgC.Format("%s\\%s\\%s.bmp", m_home, "image", file[ii]);
// 			imgO.Format("%s\\%s\\%s_en.bmp", m_home, "image", file[ii]);
// 			imgD.Format("%s\\%s\\%s_dn.bmp", m_home, "image", file[ii]);
// 			m_pFunc[ii]->set_Image(imgC, imgO, imgD);
// 		}
// 	}

// 	m_pPal = new CPalButton(CTRL_PAL);
// 	if (!m_pPal->Create(NULL, "색상 팔레트 변경", dwStyle, CRect(0, 0, 0, 0), this, -1))
// 	{
// 		delete m_pPal;
// 		m_pPal = NULL;
// 	}
// 	else
// 	{
// 		m_pPal->set_ToolTipText("색상 팔레트 변경");
// 		imgC.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_theme");
// 		imgO.Format("%s\\%s\\%s_en.bmp", m_home, "image", "mb_theme");
// 		imgD.Format("%s\\%s\\%s_dn.bmp", m_home, "image", "mb_theme");
// 		m_pPal->set_Image(imgC, imgO, imgD);
// 
// 		m_pPal->set_PalColor(m_aryColor.GetAt(m_skinN), m_aryLine.GetAt(m_skinN));
// 	}

	CString tooltip2[] = {"가상화면 1", "가상화면 2", "가상화면 3", "가상화면 4", "" };
	//CString file2[] = {"mb_virtual01", "mb_virtual02", "mb_virtual03", "mb_virtual04", "" };
	CString file2[] = {"virtual_01", "virtual_02", "virtual_03", "virtual_04", "" };
	for (int ii = 0; !tooltip2[ii].IsEmpty(); ii++)
	{
		m_pVS[ii] = new CNButton(CTRL_VS1 + ii);
		if (!m_pVS[ii]->Create(NULL, tooltip2[ii], dwStyle, CRect(0, 0, 0, 0), this, -1))
		{
			delete m_pVS[ii];
			m_pVS[ii] = NULL;
		}
		else
		{
			if (m_vsN == ii)
			{
				m_pVS[ii]->set_ToolTipText(tooltip2[ii]);
				imgC.Format("%s\\%s\\%s_dn.bmp", m_home, "image", file2[ii]);
				imgO.Format("%s\\%s\\%s_dn.bmp", m_home, "image", file2[ii]);
				imgD.Format("%s\\%s\\%s_dn.bmp", m_home, "image", file2[ii]);
				m_pVS[ii]->set_Image(imgC, imgO, imgD);
			}
			else
			{
				m_pVS[ii]->set_ToolTipText(tooltip2[ii]);
				imgC.Format("%s\\%s\\%s.bmp", m_home, "image", file2[ii]);
				imgO.Format("%s\\%s\\%s_en.bmp", m_home, "image", file2[ii]);
				imgD.Format("%s\\%s\\%s.bmp", m_home, "image", file2[ii]);
				m_pVS[ii]->set_Image(imgC, imgO, imgD);
			}
		}
	}
}

void CSVband::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

}

void CSVband::move_Ctrl()
{
	CRect rc, cRc;
	GetClientRect(rc);
	cRc.SetRect(rc.left, rc.top + 2, rc.left + 19, rc.bottom - 5);
	for (int ii = 0; ii < 4; ii++)
	{
		m_pVS[ii]->MoveWindow(cRc);

		cRc.OffsetRect(22, 0);
		if (ii == 4)
			cRc.right++;
	}
}

LRESULT CSVband::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	CString imgC, imgO, imgD;
	switch (LOWORD(wParam))
	{
	case CTRL_CHANGE:	
		m_mode++;
		if (m_mode > 2)		m_mode = 0;
		move_Ctrl();
		GetParent()->Invalidate();
		break;
	case CTRL_ENVIRONMENT:	// 설정 
	case CTRL_SCREENPRINT:	// 프린트
	case CTRL_HOMEPAGE:	// 홈페이지
	case CTRL_LOCK:		// 화면잠금
	case CTRL_TV:		// 증권방송
	case CTRL_RADIO:	// 증권라디오
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
		break;
	case CTRL_PAL:
		{
			CRect iRc;
			m_pPal->GetWindowRect(iRc);
			m_dlg->ShowSkin(CPoint(iRc.left, iRc.bottom), true);
		}
		break;
	case CTRL_VS1:
	case CTRL_VS2:
	case CTRL_VS3:
	case CTRL_VS4:
	case CTRL_VS5:
	case CTRL_VS6:
		{
			int vsN = LOWORD(wParam) - CTRL_VS1;
			if (m_vsN == vsN)
				break;

			set_ButtonImg(vsN);

			(*m_axiscall)(AXI_CHANGEBAR2, 1, m_vsN);
		}
		break;
	default:
		break;
	}
	return 0;
}

void CSVband::set_ButtonImg(int vsN)
{
	if (m_vsN == vsN)	return;

	//CString file2[] = {"mb_virtual01", "mb_virtual02", "mb_virtual03", "mb_virtual04", "" };
	CString file2[] = {"virtual_01", "virtual_02", "virtual_03", "virtual_04", "" };
	CString	imgC, imgO, imgD;
	imgC.Format("%s\\%s\\%s.bmp", m_home, "image", file2[m_vsN]);
	imgO.Format("%s\\%s\\%s_en.bmp", m_home, "image", file2[m_vsN]);
	imgD.Format("%s\\%s\\%s.bmp", m_home, "image", file2[m_vsN]);
	m_pVS[m_vsN]->set_Image(imgC, imgO, imgD);
	m_pVS[m_vsN]->Invalidate();
	
	m_vsN = vsN;
	imgC.Format("%s\\%s\\%s_dn.bmp", m_home, "image", file2[m_vsN]);
	imgO.Format("%s\\%s\\%s_dn.bmp", m_home, "image", file2[m_vsN]);
	imgD.Format("%s\\%s\\%s_dn.bmp", m_home, "image", file2[m_vsN]);
	m_pVS[m_vsN]->set_Image(imgC, imgO, imgD);
	m_pVS[m_vsN]->Invalidate();
}

void CSVband::change_VirtualScreen(int virtualScreenN)
{
	set_ButtonImg(virtualScreenN);
}

void CSVband::SetVSToolTip(int vs, CString tips)
{
	if (!m_pVS[vs])		return;

	CString tooltip;
	tooltip.Format("%s %d\n%s", "가상화면", vs + 1, tips);

	m_pVS[vs]->set_ToolTipText(tooltip);
}