// ToolWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "ToolWnd.h"
#include "../../control/fx_misc/misctype.h"
#include "IHCheck.h"
#include "../../h/interst.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SIZE_TABITEMWIDTH	80
#define SIZE_TABITEMHEIGHT	19
/////////////////////////////////////////////////////////////////////////////
// CToolWnd

CToolWnd::CToolWnd(CWnd* pMainWnd) : CBaseWnd(pMainWnd)
{
	m_activefield = _T("");
	m_bBitmap = true;
	m_bBig = false;

	// ADD PSH 20070918
	m_bMoveCfg  = FALSE;
	m_nMoveSave = 0;
	m_selectedTab = -1;
	// END ADD
}

CToolWnd::~CToolWnd()
{
}


BEGIN_MESSAGE_MAP(CToolWnd, CBaseWnd)
	//{{AFX_MSG_MAP(CToolWnd)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolWnd message handlers


void CToolWnd::OperInit()
{
	m_fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);
	HBITMAP	hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};

	hBITMAP    = getBitmap(m_fileIMG + "5BTN.BMP");
	hBITMAP_DN = getBitmap(m_fileIMG + "5BTN_dn.BMP");
	hBITMAP_HV = getBitmap(m_fileIMG + "5BTN_dn.BMP");
	const UINT ids[] = {IDC_BT_CONFIG, IDC_BT_EXPECT};
	const char* const texts[] = {"등록/수정\n관심종목설정", "예상\n예상체결가"};
	CString textExpect = "자동\n자동으로 예상체결가보기";

	const bool	bchecks[] = {false, true};

	CFont *font;
	font = GetAxFont("굴림체",9,true);

	m_btTEXT[0].Create(texts[0], CRect(0, 0, 60, SIZE_BUTTONS), this, ids[0], bchecks[0]);
	m_btTEXT[0].SetFont(font, false);  
	m_btTEXT[0].SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btTEXT[0].SetTextColor(GetAxColor(186));//BUFFET

	hBITMAP    = getBitmap(m_fileIMG + "2BTN.bmp");
	hBITMAP_DN = getBitmap(m_fileIMG + "2BTN_dn.bmp");
	hBITMAP_HV = getBitmap(m_fileIMG + "2BTN_en.bmp");


	m_btTEXT[1].Create(texts[1], CRect(0, 0, 60, SIZE_BUTTONS), this, ids[1], bchecks[1]);
	m_btTEXT[1].SetFont(m_pFont, false);  
	m_btTEXT[1].SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

// 	m_btChangeField.Create("등락", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_CHANGEFIELD, false);
// 	m_btChangeField.SetFont(m_pFont, false);  
// 	m_btChangeField.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

// 	m_btPrevView.Create("이전", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_PREVVIEW, false);
// 	m_btPrevView.SetFont(m_pFont, false);  
// 	m_btPrevView.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

// 	m_btNextView.Create("다음", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_NEXTVIEW, false);
// 	m_btNextView.SetFont(m_pFont, false);  
// 	m_btNextView.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	m_cbArrange.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_ARRANGE);
	m_cbArrange.SetFont(m_pFont);

	m_cbViewType.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_VIEWTYPE);
	m_cbViewType.SetFont(m_pFont);

	m_cbPageView.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_PAGEVIEW);
	m_cbPageView.SetFont(m_pFont);

///////////////////////////////////////////////////
	m_chkAUTO = std::make_unique<CIHCheck>("자동 예상체결 보기");
	m_chkAUTO->Create(textExpect, WS_CHILD|BS_AUTOCHECKBOX|BS_LEFT|BS_VCENTER|BS_FLAT, CRect(0, 0, 0, 0), this, IDC_BT_AUTO);
	m_chkAUTO->SetFont(m_pFont);

	m_cbGROUP.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_GROUP);
	m_cbGROUP.SetFont(m_pFont);

	m_menuField.CreatePopupMenu();
	// END ADD

 	m_chkAUTO->ShowWindow(SW_HIDE);
		
	loadcfg();
}

void CToolWnd::OperDestory()
{
	savecfg();
	
	m_btPREV.DestroyWindow();
	m_btNEXT.DestroyWindow();
	//m_btBASKET.DestroyWindow();

	for ( int ii = 0 ; ii < 2 ; ii++ )
	{
		m_btTEXT[ii].DestroyWindow();
	}

	if (m_chkAUTO)
		m_chkAUTO->DestroyWindow();

	m_cbGROUP.DestroyWindow();
}

HBITMAP CToolWnd::getBitmap(CString path)
{
	const CBitmap*	pBitmap = (CBitmap*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETBITMAP, (LPARAM)(char*)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

void CToolWnd::OperResize(int cx, int cy)
{
	m_rect = CRect(0, 0, cx, cy);
	
	if (cx <= 0 || cy <= 0)
		return;

	CRect	rect = m_rect;

	// ADD PSH 20070911
	//CRect	rcTemp, rctmp;
	// END ADD

	if (!IsInit())
	{
		m_arDIVIDE.RemoveAll();
		
		rect.OffsetRect(2, 3);
		// 콤보
		rect.right = rect.left + SIZE_COMBO + SIZE_COMBO;
		rect.bottom = rect.top + SIZE_DROPDOWN;
		
		// group 콤보
		m_cbGROUP.MoveWindow(rect, FALSE);
		m_cbGROUP.ShowWindow(SW_SHOW);	

		rect.left = rect.right + SIZE_GAPS;
		rect.right = rect.left + 76;
		rect.bottom = rect.top + SIZE_BUTTONS;
 		m_btTEXT[0].MoveWindow(rect, FALSE);
		
		rect.left = rect.right + GAP1;
		rect.right = rect.left + SIZE_COMBO;
		rect.bottom = rect.top + SIZE_DROPDOWN;

 		m_cbArrange.MoveWindow(rect, FALSE);
		m_cbArrange.ShowWindow(SW_SHOW);

		rect.left = rect.right + GAP1;
		rect.right = rect.left + SIZE_COMBO;
		rect.bottom = rect.top + SIZE_DROPDOWN;

 		m_cbViewType.MoveWindow(rect, FALSE);
		m_cbViewType.ShowWindow(SW_SHOW);

		rect.left = rect.right + GAP1;
		rect.right = rect.left + SIZE_BTWIDTH;
		rect.bottom = rect.top + SIZE_BUTTONS;
 		//m_btChangeField.MoveWindow(rect, FALSE);

		rect.left = rect.right + GAP1;
		rect.right = rect.left + SIZE_BTWIDTH;
		rect.bottom = rect.top + SIZE_BUTTONS;
 		//m_btPrevView.MoveWindow(rect, FALSE);

		rect.left = rect.right + GAP1;
		rect.right = rect.left + SIZE_BTWIDTH;
		rect.bottom = rect.top + SIZE_BUTTONS;
 		//m_btNextView.MoveWindow(rect, FALSE);

		rect.left = rect.right + GAP1;
		rect.right = rect.left + 60;
		rect.bottom = rect.top + SIZE_DROPDOWN;

 		m_cbPageView.MoveWindow(rect, FALSE);

		rect = AddDivide(rect);
	}
}


BOOL CToolWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//BOOL	bCheck = FALSE;
	CString	sztmp = _T("");
	int	GroupNumber = 0;
	//int	nOver = 0;

	switch (LOWORD(wParam))
	{
	case IDC_BT_UP:
		m_bBig = false;
		m_pMainWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
		break;
	case IDC_BT_DN:
		m_bBig = true;
		m_pMainWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
		break;
	case IDC_BT_NEXT:
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEVISIBLE, IDC_BT_NEXT));
		break;
	case IDC_BT_PREV:
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEVISIBLE, IDC_BT_PREV));
		break;
	case IDC_BT_BASKET:
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_BASKETVISIBLE, (m_btBASKET.IsChecked()) ? MO_SHOW : MO_HIDE));
		break;
	case IDC_BT_HAWKEYE:
		{
		const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			pWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		}
		break;
	case IDC_BT_PLUS:
		{
			const int	ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GROUP, 1), (LPARAM)m_chkEXPAND->GetCheck());
			if (ncnt >= MAX_GROUP)
				m_btPLUS.EnableWindow(FALSE);
			else if (ncnt == 2)
				m_btMINUS.EnableWindow(TRUE);
		}
		break;
	case IDC_BT_MINUS:
		{
			const int	ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MK_GROUP, (LPARAM)m_chkEXPAND->GetCheck());
			if (ncnt == 1)
				m_btMINUS.EnableWindow(FALSE);
			else if (ncnt == (MAX_GROUP - 1))
				m_btPLUS.EnableWindow(TRUE);

			// ADD PSH 20070911
			Invalidate();
			// END ADD
		}
		break;
	case IDC_BT_CONFIG:
		{
			//선택한 탭, 넘기기 -> 서버작업 끝나고 반영
			GroupNumber = m_cbGROUP.GetCurSel();
			int	nOver = -1;

			if(m_cbViewType.GetCurSel() == 1)
				nOver = MO_SELECT;
			else
				nOver = MO_VISIBLE;

			//분할표시된 그룹의 처음에 선택되도록 수정
			if(nOver == MO_VISIBLE)
			{
				const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
				pWnd->SendMessage(WM_MANAGE, MK_SELECTFIRST);
			}

			sztmp.Format("%s /p5 /S /d %s%c%d%c%s%c", MAP_CONFIG, "selectGROUP", P_TAB ,GroupNumber , P_NEW , "NO", P_TAB);
			m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typePOPUP), (LPARAM)sztmp.operator LPCTSTR());

// 			int index = m_btntab.GetCurSel();
// 			SetCbPageView();
// 			SendTree(index);			
		}
		break;
	case IDC_CB_GROUP://m_btntab->m_cbGroup replace
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			const int	nIndex = m_cbGROUP.GetCurSel();
			if (nIndex >= 0 && nIndex < m_cbGROUP.GetCount())
			{
				if(m_selectedTab != -1)
				{
					saveGroupwhenClose(m_selectedTab);
				}
				
				m_selectedTab = nIndex;
				
				m_cbGROUP.SetCurSel(nIndex);
				
				SetCbPageView();

				SendTree(nIndex);				
			}			
		}
		break;
	case IDC_MENU_TOOL:
		{
			CRect rcBtn;
			m_btTool.GetWindowRect(rcBtn);
			
			const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TOOLBUTTON, 0), (long)&rcBtn);
		}
		break;
	case IDC_CB_VIEWTYPE:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				const int	nIndex = m_cbGROUP.GetCurSel();
				
				SetViewType();
				if (nIndex >= 0 && nIndex < m_cbGROUP.GetCount())
				{
					if(m_selectedTab != -1)
					{
						const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
						pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE2, (LPARAM)m_selectedTab);
					}

					m_selectedTab = nIndex;

					SendTree(nIndex);
				}	
			}	
		}
		break;
// 	case IDC_BT_CHANGEFIELD:
// 		{
// 			CString text;
// 			m_btChangeField.GetWindowText(text);
// 
// 
// 			if(text == "등락")
// 			{
// 				m_btChangeField.SetWindowText("대비");
// 				m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CHANGEDIFFFIELD, MO_ON));
// 			}
// 			else
// 			{
// 				m_btChangeField.SetWindowText("등락");
// 				m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CHANGEDIFFFIELD, MO_OFF));
// 			}
// 			
// 		}
// 		break;
// 	case IDC_BT_PREVVIEW:
// 		{
// 			clickmovePageBtn(1);	
// 		}
// 		break;
// 	case IDC_BT_NEXTVIEW:
// 		{
// 			clickmovePageBtn(2);
// 		}
// 		break;
// 	case IDC_CB_PAGEVIEW:
// 		{
// 			if (HIWORD(wParam) == CBN_SELCHANGE)
// 			{
// 					CheckPrevNextBtn();
// 					int	nIndex = m_cbGROUP.GetCurSel();
// 					SendTree(nIndex);
// 			}
// 		}
// 		break;
	case IDC_CB_ARRANGE:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				const int nIndex = m_cbArrange.GetCurSel();
				m_pMainWnd->SendMessage(WM_MANAGE, MK_ARRANGE, nIndex);
			}
		}
		break;
	// END ADD
	}
	return CBaseWnd::OnCommand(wParam, lParam);
}

void CToolWnd::SetViewType()
{
	int	ncnt = -1;

	if(m_cbViewType.GetCurSel() == 1)
	{
		ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GROUP, 0), (LPARAM)FALSE);

/*	2014.13.28 KSJ 체결이퀄라이저 만들다가 중지한 기능이므로 주석처리함.
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_EQUALIZER, 1));
*/
	}
	else
	{
 		ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GROUP, 1), (LPARAM)FALSE);

/*	2014.13.28 KSJ 체결이퀄라이저 만들다가 중지한 기능이므로 주석처리함.
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_EQUALIZER, 0));
*/
	}
}

// void CToolWnd::CheckPrevNextBtn()
// {
// 	int tp = m_cbPageView.GetCurSel();
// 	
// 	int maxCount = m_cbPageView.GetCount() - 1;
// 
// 	if(tp == 0)
// 	{
// 		m_btPrevView.EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_btPrevView.EnableWindow(TRUE);
// 	}
// 
// 	if(tp == maxCount)
// 	{
// 		m_btNextView.EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_btNextView.EnableWindow(TRUE);
// 	}
// }


// void	CToolWnd::clickmovePageBtn(int type)
// {
// 	int	nIndex = m_cbGROUP.GetCurSel();
// 	int tp = m_cbPageView.GetCurSel();
// 	int tpCount = m_cbPageView.GetCount() - 1;
// 
// 	if(type == 1)
// 	{
// 		tp = tp - 1;
// 	}
// 	else
// 	{
// 		tp = tp + 1;
// 	}
// 
// 	if(tp < 0)
// 		tp = 0;
// 	else if(tp > tpCount)
// 		tp = tpCount;
// 		
// 	if(tp < 1)
// 	{
// 		m_btPrevView.EnableWindow(false);
// 	}
// 	else
// 	{
// 		m_btPrevView.EnableWindow(true);
// 	}
// 		
// 	if(tp >= tpCount)
// 	{
// 		m_btNextView.EnableWindow(false);		
// 	}
// 	else
// 	{
// 		m_btNextView.EnableWindow(true);
// 	}
// 		
// 	m_cbPageView.SetCurSel(tp);
// 
// 	SendTree(nIndex);	
// }


int CToolWnd::countInterest(int group)
{
	struct _oldinters
	{
		char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		char	code[12];	// 종목코드
		char	name[32];	// 종목명
		char	xprc[10];	// 보유단가
		char	xnum[10];	// 보유수량
		char	xupnum[16];	// 상장 주식수
		char	filler[24];	// Reserved
		int		bookmark;   
	};

struct	_oldbookmark {
	char	gubn[1];		// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];		// 종목코드
	char	name[32];		// 종목명
	int 	bookmark;	// 북마크 여부
};

struct  _oldinters oldInter {};
struct  _oldbookmark oldbook {};

const UINT newSize = sizeof(_inters);
const UINT oldSize = sizeof(_oldinters);

const UINT oldbooksize = sizeof(_oldbookmark);
const UINT newbooksize = sizeof(_bookmarkinfo);

	UINT fileSize = 0;
	
	bool isOld = false;			//새 구조체인지 판단
	
	CString	filePath, filePath2;
	int	count = 0;

	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, group);

//	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(filePath))
		return -1;

	UINT	readL{};
	// MODIFY PSH 20070914
	//CString	code = _T(""), amount = _T(""), price = _T("");
	CString	code = _T(""), amount = _T(""), price = _T(""), name = _T("");
//	int bookmark = 0;

	bool isFile = true;

	// END MODIFY
	struct	_inters interest;
	struct  _bookmarkinfo bInfo {};

	CFile	fileH(filePath, CFile::modeRead);
	CFile   fileB;

	if( !fileB.Open(filePath2, CFile::modeRead) )
	{
		isFile = false;		//파일 없을경우
	}

	//파일 사이즈로 옛날 inter인지 신규 inter인지 판단
	fileSize = gsl::narrow_cast<UINT>(fileH.GetLength());

	if (fileH.m_hFile == CFile::hFileNull)
		return 0;

	if (fileB.m_hFile == CFile::hFileNull)
	{
		isFile = false;		//파일이 없을경우
	}

	//강제적으로 두번째만 읽어서 종목코드란이 비어있는지 확인한다. 
	//UINT nBytesRead{};
	int flag = 0;
	flag = 1;
	
	fileH.SeekToBegin();

 	for (int ii = 0; ii < MAX_LINE ; ii++)
	{
		if(flag == 1)
		{
			readL = fileH.Read(&interest, sz_inters);
			
			if (readL < sz_inters)
				break;
		}
		else
		{
			readL = fileH.Read(&oldInter, sizeof(_oldinters));
			isOld = true; 
			
			if (readL < sizeof(_oldinters))
				break;
		}

		if(isOld == true)
		{
			CopyMemory(interest.code, oldInter.code, sizeof(interest.code));
		}

		code.Format("%.*s", sizeof(interest.code), interest.code);
		code.TrimRight();

		if(!code.IsEmpty())
		{
			count++;
		}
	}
	fileH.Close();

	return count;
}


void CToolWnd::SetGroupDataView()
{

 	CWnd* pgWnd;
	pgWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
 	pgWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INITGRID, 0), 0);

	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int		nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	//MO_VISIBLE모드일때는 항상 맨 앞을 선택하도록 한다
	if(nOver == MO_VISIBLE)
	{
		pgWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)0);
	}

}

void CToolWnd::OperDraw(CDC* pDC)
{
	CBaseWnd::OperDraw(pDC);

	const int	ncnt = m_arDIVIDE.GetSize();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		DrawSEP(pDC, m_arDIVIDE.GetAt(ii));	
	}

	CFont*	oldfont = pDC->SelectObject(m_pFont);
	pDC->DrawText("초", m_rcSECOND, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	// DEL PSH 20070912
	//pDC->DrawText("F", m_rcFIELD, DT_SINGLELINE|DT_VCENTER|DT_CENTER); 
	// END DEL
	pDC->SelectObject(oldfont);

	// ADD PSH 20070907
	CPen NewPen, *pOldPen = nullptr;
	NewPen.CreatePen(PS_SOLID, 1, m_clrOutLine);

	pOldPen = pDC->SelectObject(&NewPen);

//	pDC->MoveTo(0, m_rect.bottom - 1);
//	pDC->LineTo(m_rect.Width(), m_rect.bottom - 1);
	pDC->FillSolidRect(m_rect, GetAxColor(66));
//	DrawRoundBitmap(pDC, m_rect, ROUND_PANEL);

	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
	// END ADD
}

CRect CToolWnd::AddDivide(CRect rect)
{
	rect.left = rect.right + SIZE_GAPS;
	rect.right = rect.left + 1;
	m_arDIVIDE.Add(rect);
	return rect;
}

CRect CToolWnd::MoveButton(CRect rcPrev, CSize size, CSize gap, CfxImgButton* pButton)
{
	CRect	rect;
	rect = GetNextRect(rcPrev, size, gap);
	pButton->MoveWindow(rect, FALSE);
	pButton->ShowWindow(SW_SHOW);
	pButton->Invalidate();
	m_arCONTROL.Add((CWnd*)pButton);
	return rect;
}

CRect CToolWnd::GetNextRect(CRect rcPrev, CSize size, CSize gap)
{
	CRect	rect;
	
	rect.left = (gap.cx) ? rcPrev.right + gap.cx : rcPrev.left;
	rect.right = rect.left + size.cx;
	rect.top = (gap.cy) ? rcPrev.bottom + gap.cy : rcPrev.top;
	rect.bottom = rect.top + size.cy;

	return rect;
}

void CToolWnd::DrawSEP(CDC* pDC, CRect rect)
{
	CPen	pen1(PS_SOLID, 1, RGB(128, 128, 128)), pen2(PS_SOLID, 1, RGB(255, 255, 255));
	CPen*	oldpen = pDC->SelectObject(&pen1);

	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.left, rect.bottom);

	pDC->SelectObject(&pen2);
	pDC->MoveTo(rect.TopLeft() + CPoint(1, 0));
	pDC->LineTo(rect.left + 1, rect.bottom);

	pDC->SelectObject(oldpen);
	pen1.DeleteObject();
	pen2.DeleteObject();
}

void CToolWnd::HideControl(CRect rcBasket)
{
	CRect	rect;
	const int	ncnt = m_arCONTROL.GetSize();
	CWnd*	pWnd = nullptr;
	
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		pWnd = m_arCONTROL.GetAt(ii);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		if (rect.IntersectRect(rcBasket, rect))
			pWnd->ShowWindow(SW_HIDE);
		else
			pWnd->ShowWindow(SW_SHOW);
	}
}

HBRUSH CToolWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBaseWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_BT_AUTO:
	case IDC_CHECK_PLUSMINUS:
		pDC->SetBkMode(TRANSPARENT);
		hbr = *m_pBrush;
	}
	return hbr;
}

void CToolWnd::OperPallette()
{
//	m_pBrush = GetAxBrush(m_clrPallete);

//	COLORREF	color = GetAxColor(14);
	const COLORREF	crback = GetAxColor(66);
	m_pBrush = GetAxBrush(crback);

//	m_btntab.SetColor(RGB(255, 255, 255), color, RGB(255, 255, 32), color, m_clrPallete);
//	m_btntab.SetColor(RGB(255, 255, 255), color, RGB(255, 255, 32), color, crback);
//	m_btntab.SetColor(GetAxColor(48), GetAxColor(48), GetAxColor(130), color, crback);

	// ADD PSH 20070907
	m_clrOutLine = GetAxColor(65);
	// END ADD
}

LONG CToolWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	bool bchk = FALSE;

	switch (LOWORD(wParam))
	{
	case MK_GETGROUP:
		{
			CMapStringToString*	pMap = (CMapStringToString*)lParam;
			const int	ncnt = m_cbGROUP.GetCount();
			CString	sztmp = _T(""), szval = _T("");
			int	nval = 0;
			
			for ( int ii = 0 ; ii < ncnt; ii++ )
			{
				m_cbGROUP.GetLBText(ii, sztmp);
				nval = m_cbGROUP.GetItemData(ii);
				szval.Format("%02d", nval);
				pMap->SetAt(szval, sztmp);
			}
			return pMap->GetCount();
		}
		break;
	case MK_EXPECT:
		if (m_btTEXT[1].IsChecked())
			ret = 1;
		break;
	case MK_SETEXPECT:
		if((int)lParam == 0)
			m_btTEXT[1].SetCheck(false);
		else
			m_btTEXT[1].SetCheck(true);
		break;
	case MK_AUTOEXPECT:
		bchk = m_chkAUTO->GetCheck();
		
		if (bchk)
			ret = 1;
		break;
	case MK_SETAUTOCHECK:
		m_chkAUTO->SetCheck((int)lParam);
		if(m_chkAUTO->GetCheck() == TRUE)
			m_btTEXT[1].SetCheck(false);
		ret = 1;
		break;
	case MK_TOOLSIZE:
		ret = (LONG)m_bBig;
		break;
	case MK_CHART:
		ret = (LONG)m_btCHART.IsChecked();
		break;
// 	case MK_TURNCODE:
// 		m_btTURN.SetCheck(false);
// 		m_spinTIME->EnableWindow(TRUE);
// 		break;
	case MK_SELGROUP:
		if (HIWORD(wParam) == MO_SET)
			SelectGroup((int)lParam);
		break;
	case MK_GETAFIELD:	// get active field
		{
// 			int	nPos = m_cbFIELD.GetCurSel();
// 			ret = (LONG)m_cbFIELD.GetItemData(nPos);
		}
		break;
	case MK_HAWKEYE:
		ret = (LONG)m_btHAWKEYE.IsChecked();
		break;
	case MK_SETUPOK:
		ret = Reload();
		break;
	case MK_RELOADLIST:
		ReloadList2();
		break;
	case MK_SENDTREE:
		{
			const int index = m_cbGROUP.GetCurSel();
			SetCbPageView();
			SendTree(index);
		}
		break;
	case MK_ISEXPAND:
		ret = (LONG)m_chkEXPAND->GetCheck();
		break;
	case MK_INPUTOVER:
		if(m_cbViewType.GetCurSel() == 1)
			ret = MO_SELECT;
		else
			ret = MO_VISIBLE;
		break;
	// ADD PSH 20070918
	case MK_GETMOVECFG:
		ret = m_bMoveCfg;
		break;
	case MK_SETMOVECFG:
		m_bMoveCfg = lParam;
		break;
	case MK_GETMOVESAVE:
		ret = m_nMoveSave;
		break;
	case MK_SETMOVESAVE:
		m_nMoveSave = lParam;
		break;
	case MK_SELECTGROUP:						//종합화면에서 2022열면서 그룹정보 보낼 시
		{
			const int index = HIWORD(wParam);

			const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE, (LPARAM)m_selectedTab);
			dominoSelectGroup(index);
		}
		break;
	case MK_REMAIN:
		{
			const int index = HIWORD(wParam);
			m_cbGROUP.SetCurSel(index);
		}
		break;
	case MK_GETCBINDEX:
		ret = m_cbGROUP.GetCurSel();
		break;
	// END ADD
	case MK_REFRESH:
		clickBtn();
		break;
	case MK_EQUALIZER:
// 		if(lParam)
// 			m_btEqualizer.SetCheck(true);
// 		else
// 			m_btEqualizer.SetCheck(false);

		break;
	case MK_GETVIEWPAGE:
		ret = m_cbPageView.GetCurSel();
		break;
	case MK_GETARRANGE:
		ret = m_cbArrange.GetCurSel();
		break;
	case MK_SETARRANGE:
		{
		const int nIndex = m_cbArrange.GetCurSel();
			m_pMainWnd->SendMessage(WM_MANAGE, MK_ARRANGE, nIndex);
		}
		break;
	case MK_CHANGEARRANGE:
		{
		const int nIndex = HIWORD(wParam);
			m_cbArrange.SetCurSel(nIndex);
			m_pMainWnd->SendMessage(WM_MANAGE, MK_ARRANGE, nIndex);
		}
		break;
	case MK_SETVIEWTYPE:
		{
			SetViewType();
		}
		break;
	case MK_CHANGEVIEWTYPE:
		{
			loadcfg();

			const int nView = HIWORD(wParam);
			//2012.04.05 KSJ 받은것 구분자로 자름
// 			BOOL bRemain = (BOOL)lParam;
			CString strTemp, strRemain, strIndex;
			strTemp.Format("%s", (char*)lParam);

			AfxExtractSubString(strIndex, strTemp, 0, (char)0x09);
			AfxExtractSubString(strRemain, strTemp, 1, (char)0x09);

			const int nIndex = atoi(strIndex); //- 1 ;	//받은 것에서 -1 해야 본래 인덱스나옴

			const BOOL bRemain = (BOOL)atoi(strRemain);
			// 

			m_cbViewType.SetCurSel(nView);
// 			int nIndex = m_cbGROUP.GetCurSel();
			// KSJ
			
			SetViewType();

			if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_SMALLCONFIG); 
			}
			else
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_LARGECONFIG); 
			}
			
			//if (nIndex >= 0 && nIndex < m_cbGROUP.GetCount())
			{
				//if(m_selectedTab != -1)
				//{
				//	const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
				//	pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE2, (LPARAM)m_selectedTab);
				//}
				
				//m_selectedTab = nIndex;		
				if(bRemain != TRUE)
					SendTree(nIndex);
			}
		}
		break;
	case MK_SETGROUP:
		{
			const int nIndex = (int)lParam;
			m_cbGROUP.SetCurSel(nIndex);
		}
		break;
	case MK_VIEWREFRESH:
		{
			
		const int nIndex = m_cbGROUP.GetCurSel();
						
			if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_SMALLCONFIG); 
			}
			else
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_LARGECONFIG); 
			}
			
			if (nIndex >= 0 && nIndex < m_cbGROUP.GetCount())
			{
				if(m_selectedTab != -1)
				{
					const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
					pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE2, (LPARAM)m_selectedTab);
				}
				
				m_selectedTab = nIndex;
				
				SendTree(nIndex);
			}
		}
		break;
	case MK_SETPAGEVIEW:
		{
			m_cbPageView.SetCurSel((int)lParam);
		}
		break;
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}

	return ret;
}

void CToolWnd::savecfg()
{
	CString	sztmp;

	saveGroupIndex(m_cbGROUP.GetCurSel());

// 	sztmp.Format("%d", m_chkEXPAND->GetCheck());
// 	WritePrivateProfileString(SEC_TOOL, KEY_EXPAND, sztmp, m_fileCFG);
// 
	sztmp.Format("%d", m_chkAUTO->GetCheck());
	WritePrivateProfileString(SEC_TOOL, KEY_AUTO, sztmp, m_fileCFG);
// 
// 	// ADD PSH 20070918
// 	sztmp.Format("%d", m_bMoveCfg);
// 	WritePrivateProfileString(SEC_TOOL, KEY_MOVECFG, sztmp, m_fileCFG);
// 
// 	sztmp.Format("%d", m_nMoveSave);
// 	WritePrivateProfileString(SEC_TOOL, KEY_MOVESAVE, sztmp, m_fileCFG);
	// END ADD

	int	nmode = MO_SELECT;
	if(m_cbViewType.GetCurSel() == 1)
		nmode = MO_SELECT;
	else
		nmode = MO_VISIBLE;

	sztmp.Format("%d", nmode);
	WritePrivateProfileString(SEC_TOOL, KEY_FILLMODE, sztmp, m_fileCFG);
	
// 	nmode = m_btEqualizer.IsChecked();
// 	sztmp.Format("%d", nmode);
// 	WritePrivateProfileString(SEC_TOOL, KEY_EQUALIZER, sztmp, m_fileCFG);

	sztmp.Format("%d", (int)m_bBig);
	WritePrivateProfileString(SEC_TOOL, KEY_BIG, sztmp, m_fileCFG);

// 	sztmp = m_spinTIME->GetText();
// 	WritePrivateProfileString(SEC_TOOL, KEY_TIME, sztmp, m_fileCFG);

// 	CString tempFile; tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);
// 	int	nsel = m_cbFIELD.GetCurSel();
// 	int	ndata = m_cbFIELD.GetItemData(nsel);
// 	sztmp.Format("%02d", ndata);
// 	WritePrivateProfileString(SEC_FIELDS, KEY_ACTIVE, sztmp.operator LPCTSTR(), tempFile);

	int nIndex = m_cbArrange.GetCurSel();
	sztmp.Format("%d", (int)nIndex);
	WritePrivateProfileString(SEC_TOOL, KEY_CBARRANGE, sztmp, m_fileCFG);

	nIndex = m_cbViewType.GetCurSel();
	sztmp.Format("%d", (int)nIndex);
	WritePrivateProfileString(SEC_TOOL, KEY_VIEWTYPE, sztmp, m_fileCFG);
}

void CToolWnd::loadcfg()
{
	// MODIFY PSH 20070918
	//BOOL	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EXPAND, 0, m_fileCFG);
	BOOL	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EXPAND, 1, m_fileCFG);
	// END MODIFY
	//m_chkEXPAND->SetCheck(bCheck);

	
	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_AUTO, 1, m_fileCFG);
//	m_chkAUTO->SetCheck(bCheck);
	
// 	if(bCheck)
// 	{
// 		m_btTEXT[1].EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_btTEXT[1].EnableWindow(TRUE);
// 	}

	// ADD PSH 20070918
	m_bMoveCfg  = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_MOVECFG, 0, m_fileCFG);
	m_nMoveSave = GetPrivateProfileInt(SEC_TOOL, KEY_MOVESAVE, 0, m_fileCFG);
	// END ADD
	
	const int	nmode = (int)GetPrivateProfileInt(SEC_TOOL, KEY_FILLMODE, MO_SELECT, m_fileCFG);
	
	m_cbViewType.SetCurSel(nmode);
	

	m_bBig = true;

//	m_bBig = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_BIG, 1, m_fileCFG);
// 	bool m_bIsEqual = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EQUALIZER, 0, m_fileCFG);
// 
// 	if(m_bIsEqual)
// 		m_btEqualizer.SetCheck(true);
// 	else
// 		m_btEqualizer.SetCheck(false);
	int	ncount = (int)GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);
	
	if (ncount <= 0)
		ncount = 1;
	
	if (ncount > MAX_GROUP)
		ncount = MAX_GROUP;

// 	if (ncount == 1)
// 		m_btMINUS.EnableWindow(FALSE);
// 	else if (ncount == MAX_GROUP)
// 		m_btPLUS.EnableWindow(FALSE);
	
	const int	nval = GetPrivateProfileInt(SEC_TOOL, KEY_TIME, 3, m_fileCFG);
// 	CString	sztmp;
// 	sztmp.Format("%d", nval);
// 	m_spinTIME->SetText(sztmp);
	
	if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
		m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_SMALLFIELD);
	else
		m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_LARGEFIELD);
 	
	CFileFind finder;
	if (!finder.FindFile(m_fileFIELD))
	{
		 CString	string;

		 if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		 {
			 if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
				 string = DEF_FIELD_CONTENT1;
			 else
				 string = DEF_FIELD_CONTENT;

			 WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
		 }
		 else
		 {
			 if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
				 string = ((CMainWnd*)m_pMainWnd)->m_Smallfields;
			 else
				 string = ((CMainWnd*)m_pMainWnd)->m_Largefields;
			 
			 WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
		 }
	}
	else
	{
		CString	string;
		
		if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		{
			int	readL = 0;
			char	readB[256];
			
			readL = GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", "", readB, sizeof(readB), m_fileFIELD);
			
			CString tmp = CString(readB, readL);
			
			if(tmp == "")
			{
				if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
					string = DEF_FIELD_CONTENT1;
				else
					string = DEF_FIELD_CONTENT;
				
				WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
			}
		}
		else
		{	
			int	readL = 0;
			char	readB[256];
			
			readL = GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", "", readB, sizeof(readB), m_fileFIELD);
			
			CString tmp = CString(readB, readL);

			if(tmp == "")
			{
				if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
					string = ((CMainWnd*)m_pMainWnd)->m_Smallfields;
				else
					string = ((CMainWnd*)m_pMainWnd)->m_Largefields;
				
				WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
			}
		 }
	}
 
 	m_filePPOLIO.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);	

// 	ncount = (int)GetPrivateProfileInt(SEC_TOOL, KEY_SELECTGROUP, 0, orgCfg);
// 	//m_cbGROUP.SetCurSel(ncount);
// 	m_cbGROUP.SetCurSel(ncount);

	loadTabItem();

	const CString cb[] = {"등록순", "업종순", "종목명순", "종목코드순"};

	int nTemp{}, jj{};
	for (int ii = 0; ii<4 ; ii++)
	{
			nTemp = m_cbArrange.AddString(cb[ii]);
			jj = ii+1;
			m_cbArrange.SetItemData(nTemp, jj);
	}
	
	CString cb2[] = {"20종목씩", "전체종목"};
	
	for (int ii = 0; ii<2 ; ii++)
	{
		nTemp = m_cbViewType.AddString(cb2[ii]);
		jj = ii+1;
		m_cbViewType.SetItemData(nTemp, jj);
	}

	ncount = (int)GetPrivateProfileInt(SEC_TOOL, KEY_CBARRANGE, 0, m_fileCFG);
	m_cbArrange.SetCurSel(ncount);

	
	ncount = (int)GetPrivateProfileInt(SEC_TOOL, KEY_VIEWTYPE, 0, m_fileCFG);
	m_cbViewType.SetCurSel(ncount);
	
	SetCbPageView();	

	//m_btPrevView.EnableWindow(FALSE);
}

void CToolWnd::SetCbPageView()
{
	CString strI;
	int nTemp{}, jj{};
	m_cbPageView.ResetContent();
	const int cbIndex = m_cbGROUP.GetCurSel();	
	const int maxCount = countInterest(cbIndex+1);
	int cbCount = 0;
	int remain = 0;

	if(maxCount > 20)
	{
		cbCount = maxCount / 20;
		remain = maxCount % 20;

		if(remain > 0)
			cbCount++;
		//m_btNextView.EnableWindow(true);
	}
	else if(maxCount > 0 && maxCount <= 20)
	{
		cbCount = 1;

		//m_btNextView.EnableWindow(false);
	}
	
	for (int ii = 0; ii<cbCount ; ii++)
	{
		strI.Format("%d", ii+1);
		nTemp = m_cbPageView.AddString(strI);
		jj = ii+1;
		m_cbPageView.SetItemData(nTemp, jj);
	}
	
	m_cbPageView.SetCurSel(0);
	//m_btPrevView.EnableWindow(false);
	
}

void CToolWnd::RemoveAllMenu()
{
	const int nCnt = m_menuField.GetMenuItemCount();

	for(int nIdx = nCnt - 1; nIdx >= 0; nIdx--)
	{
		m_menuField.DeleteMenu(nIdx, MF_BYPOSITION);
	}
}

CString CToolWnd::loadFieldnm(int fno)
{
	CString ret = _T("");

	CFileFind finder;
	if (!finder.FindFile(m_fileFIELD))
	{
		CString	string = DEF_FIELD_CONTENT1;
		WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
	}
	
	return ret;
}

void CToolWnd::loadTabItem()
{ 
	m_cbGROUP.ResetContent();

	int readL = 0, idx = 0, nTemp = 0;
	char readB[1024];
	CString	string, gno, gname;

	readL = GetPrivateProfileString(SEC_GROUPORDER, "00", "", readB, sizeof(readB), m_filePPOLIO);
	string = CString(readB, readL);
	
	for (int ii = 0; !string.IsEmpty(); ii++)
	{
		gno = IH::Parser(string, ";");
		readL = GetPrivateProfileString(SEC_GROUPNAME, gno, "", readB, sizeof(readB), m_filePPOLIO);
		gname = CString(readB, readL);

		if (!gname.IsEmpty())
		{
			idx = atoi(gno);
			nTemp = m_cbGROUP.AddString(gname);
			m_cbGROUP.SetItemData(nTemp, idx);
		}
	}

	CString orgCfg;
	orgCfg.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_CONFIG);

	const int ncount = (int)GetPrivateProfileInt(SEC_TOOL, KEY_SELECTGROUP, 0, orgCfg);
	//m_cbGROUP.SetCurSel(ncount);
	m_cbGROUP.SetCurSel(ncount);
	if (0 < m_cbGROUP.GetCount())
	{
		m_cbGROUP.SetCurSel(0);
	}
}


void CToolWnd::dominoSelectGroup(int index)
{
	//if (index >= 0 && index < m_cbGROUP.GetCount())
	//{
	//	m_cbGROUP.SetCurSel(index);
		
		SendTree(index);
	//}
}

void CToolWnd::SetSel(int nIndex)
{
	const int	ncnt = m_cbGROUP.GetCount();
	if (nIndex < 0 || nIndex >= ncnt)
		return;

	m_cbGROUP.SetCurSel(nIndex);
	//m_btntab.SetCurSel(nIndex);
	m_selectedTab = nIndex;

	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	pWnd->SendMessage(WM_MANAGE, MK_SELECTTREE, (LPARAM)nIndex);
}

//관심그룹 index 저장
void CToolWnd::saveGroupIndex(int index)
{
	CString	str;
	str.Format("%02d", index);
	WritePrivateProfileString(SEC_TOOL, KEY_SELECTGROUP, str.operator LPCTSTR(), m_fileCFG);
}

void CToolWnd::saveGroupwhenClose(int index)
{
	const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE, (LPARAM)index);
}

//테스트용
void CToolWnd::testSaveFile()
{
	CString strPath(_T("")), m_root, m_user;
	strPath.Format("c:\\test.ini");
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	CString titletime;
	
	titletime.Format("[%s]ToolWnd::SelectTab", strCurTime);

	WritePrivateProfileString("IB202200", titletime, "select Botton", (LPCTSTR)strPath);
}

// void CToolWnd::OnSelchangeBtntab(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	m_btntab.SetFocus();
// 	clickBtn();
// 	*pResult = 0;
// 
// //	testSaveFile();
// }

void CToolWnd::clickBtn()
{
	const int	nIndex = m_cbGROUP.GetCurSel();
	if (nIndex >= 0 && nIndex < m_cbGROUP.GetCount())
	{
		m_cbGROUP.SetCurSel(nIndex);
		
		if(m_selectedTab != -1)
		{
			saveGroupwhenClose(m_selectedTab);
		}
		m_selectedTab = nIndex;
		
		SendTree(nIndex);
	}
}


void CToolWnd::SendTree(int nIndex)
{
	//관심그룹 index 저장, Tree는 1베이스이므로 +1
//	saveGroupIndex(nIndex+1);

	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	//const UINT	kind = MAKE_TREEID(xINTEREST, 0, 2, nIndex, TD_ITEM);
	//pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), nIndex);

	const int tp = m_cbViewType.GetCurSel();
	if(tp == 1)
	{
		pWnd->SendMessage(WM_MANAGE, MK_VIEWTYPE, (LPARAM)0);
	}
	else
	{
		pWnd->SendMessage(WM_MANAGE, MK_VIEWTYPE, (LPARAM)(m_cbPageView.GetCurSel()+1));
	}
	
	//pWnd->SendMessage(WM_MANAGE, MK_SELECTTREE, (LPARAM)nIndex);
	pWnd->SendMessage(WM_MANAGE, MK_CALLINTEREST, nIndex);
}


void CToolWnd::SelectGroup(UINT kind)
{
	const int	ncount = (int)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ALL));
	
// 	if (ncount == 1)
// 	{
// 		m_btMINUS.EnableWindow(FALSE);
// 	}
// 	else if (ncount == MAX_GROUP)
// 	{
// 		m_btPLUS.EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_btMINUS.EnableWindow(TRUE);
// 		m_btPLUS.EnableWindow(TRUE);
// 	}
	const int n1 = m_cbGROUP.GetCurSel();
	const int n = m_cbGROUP.GetItemData(m_cbGROUP.GetCurSel());
//	saveGroupIndex(n);
	SetSel(n-1);
}

int CToolWnd::loadActiveField()
{
	CString tempFile; 
	if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
		tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_SMALLFIELD);
	else
		tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_LARGEFIELD);

	CFileFind finder; if (!finder.FindFile(tempFile)) return -1;

	int	readL = 0;
	char	readB[256]{};
	
	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
	{
		readL = GetPrivateProfileString(SEC_FIELDS, KEY_ACTIVE, "00", readB, sizeof(readB), tempFile);
	}
	else
	{
		readL = GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_FIELDS, KEY_ACTIVE, "00", readB, sizeof(readB), tempFile);
	}

	return atoi(CString(readB, readL));
}

int CToolWnd::Reload()
{
	const int ncntBeforeload = m_cbGROUP.GetCount();		//변경전 그룹 개수
	int	ncursel = 0;
	int	ndata = 0;
	const int	ret = 0;

	// m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);	
// 	CString szfield = loadFieldnm(ndata);
// 
// 	if (m_activefield.CompareNoCase(szfield))
// 	{
// 		ret = 1;
// 		m_activefield.Format("%s", szfield);
// 	}

	m_filePPOLIO.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);	
	ncursel = m_cbGROUP.GetCurSel();
	ndata = -1;

	if (ncursel < 0)
		ndata = -1;
	else
		ndata = m_cbGROUP.GetItemData(ncursel);
	
	loadTabItem();

	
	const int	ncnt = m_cbGROUP.GetCount();

	//그룹 개수가 하나 늘어났을 경우, 마지막에 생성된 탭에 선택 되도록 변경 09.11.19
// 	if(ncnt == ncntBeforeload+1)
// 	{
// 		m_cbGROUP.SetCurSel(ncnt-1);
// 		//m_btntab.SetCurSel(ncnt-1);
// 		m_selectedTab = ncnt-1;
// //		SendTree(ncnt-1);
// 	}
// 	else
// 	{
// 		for ( int ii = 0 ; ii < ncnt ; ii++ )
// 		{
// 			ncursel = m_cbGROUP.GetItemData(ii);
// 			if (ncursel == ndata)
// 			{
// 				m_cbGROUP.SetCurSel(ii);
// 				//m_btntab.SetCurSel(ii);
// 				m_selectedTab = ii;
// //				SendTree(ii);
// 			}
// 		}
// 	}

	return ret;
}


void CToolWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CBaseWnd::OnMouseMove(nFlags, point);
}

LRESULT CToolWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CBaseWnd::WindowProc(message, wParam, lParam);
}

void CToolWnd::ReloadList()
{
	//CheckPrevNextBtn();
	const int	nIndex = m_cbGROUP.GetCurSel();
	SetCbPageView();
	SendTree(nIndex);
}

void CToolWnd::ReloadList2()
{
	//CheckPrevNextBtn();
	const int	nIndex = m_cbGROUP.GetCurSel();
	SendTree(nIndex);
}


void CToolWnd::SelectMode(int nmode)
{
	m_btAddSelect.SetCheck(false);
	m_btAddVisible.SetCheck(false);
	m_btAddFull.SetCheck(false);

	switch (nmode)
	{
	case MO_VISIBLE:
		m_btAddVisible.SetCheck(true);
		break;
	case MO_FULL:
//		m_btAddFull.SetCheck(true);
//		break;
	case MO_SELECT:
	default:
		m_btAddSelect.SetCheck(true);
		break;
	}
}

// CString CToolWnd::GetFieldChangeBtn()
// { 
// 	CString result;
// 
// 	m_btChangeField.GetWindowText(result); 
// 
// 	return result;
// }