// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "MainWnd.h"
#include "ioformat.h"
#include "../../h/grid.h"

//#import  <msxml6.dll>
#include <atlconv.h>
#include <MsXml2.h>

//using namespace MSXML2;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SEND_OUBS		0
#define SEND_GROUPLIST		1
#define SEND_CODES		2
#define SEND_GRID		3

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pWnd)
: CAxisExt(pWnd)
{
	m_nType = SEND_OUBS;
	m_pGridArray.RemoveAll();
	m_bContinue = FALSE;
	m_bSecond = FALSE;
	m_kind = 0;
	m_bTrigger = FALSE;

	for(int ii=0 ; ii < MAX_GRID; ii++)
		m_pGroupWnd[ii] = nullptr;

	m_bRTMReady = FALSE;
	m_bStartMsg = FALSE;

	m_nCntGrid = 1;
	m_nCurGrid = 1;
	m_nPreCurGrid = -1;

	m_linkMap   = _T("");
	m_fontsize  = 9;
	m_fontname = _T("굴림체");
	m_bCurr = TRUE;
	m_bName = TRUE;
	m_bAllBold = FALSE;

	m_line = 1;
	m_dblAction = dblNONE;
	m_selAction = 1;

	m_bDLLDATA = FALSE;
	m_bLastStat = FALSE;
}

CMainWnd::~CMainWnd()
{
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

LRESULT CMainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
	{
		return 1;
	}
	else if ( message == WM_DESTROY )
	{
//		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INTSAVE, 0));
	}
	else if(message == WM_USER+9111)	//메인이 닫힐때.
	{		
		m_bDLLDATA = TRUE;

		//char* wb;

		//wb = (char*)lParam;

		//// 		int nKey = atoi(m_strTag);
		//// 		
		//// 		if(nKey < 10)
		//// 		{
		//// 			nKey = nKey + 10000;
		//// 		}
		//// 
		//// 		m_strTag = "";
		//// 		m_strTag.Format("%d",nKey);


		//sprintf(wb, m_strTag);
		return 1;	
	}
	else if(message == WM_USER+9333)	//LASTSTAT가 올라올때
	{
		if(m_bStartMsg)
		{
			CString strTmp = (char*)lParam;
			
			strTmp.Replace("USRDEFDATA","");
			
			strTmp.TrimLeft();
			strTmp.TrimRight();


			CString strCfg,filepath;
			strCfg.Format("intercfg%s.ini",strTmp);
			filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

			m_fileCFG = filepath;

			CFileFind finder;

			if(finder.FindFile(filepath))
				::DeleteFile(filepath);

			return 1;
		}

		m_bStartMsg = TRUE;
		m_bLastStat = TRUE;

		CString str = (char*)lParam;

		int pos = str.Find("\n");
		
		if(pos >= 0)
		{
			while(pos >= 0)
			{
				CString sToken = SplitString(str,"\n");
				if(sToken.Find("USRDEFDATA") >= 0)
				{
					sToken.Replace("USRDEFDATA","");

					str = sToken;
					break;
				}

				pos = str.Find("\n");
			}

			if(str.Find("USRDEFDATA") >= 0)
			{
				str.Replace("USRDEFDATA","");
			}
		}
		else
		{
			str.Replace("USRDEFDATA","");
		}

		str.TrimLeft();
		str.TrimRight();
		m_strTag = str;

		//ShowWindow(SW_SHOW);
//		m_pWnd->GetParent()->ShowWindow(SW_SHOW);
		
		if(m_strTag == "LASTSTAT" || m_strTag == "")
		{
			m_strTag = "";
			
			int nKey = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);
			
			if(nKey < 10)
			{
				nKey = nKey + 1000;
			}
			
			m_strTag.Format("%d",nKey);
			
			CString strCfg,filepath;
			strCfg.Format("intercfg%s.ini",m_strTag);
			filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);
			
			m_fileCFG = filepath;

			CString basefile;
			basefile.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, "intercfg.ini");
			
			
			CFileFind finder;
			
			if(finder.FindFile(basefile))
				::CopyFile(basefile,filepath,FALSE);
		}
		else
		{
			CString strCfg,filepath,filepathNew;
			strCfg.Format("intercfg%s.ini",m_strTag);
			filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

			m_fileCFG = filepath;

			int nKey = atoi(m_strTag);
					
			if(nKey < 10)
			{
				nKey = nKey + 10000;

				m_strTag = "";
				m_strTag.Format("%d",nKey);
				
				strCfg.Format("intercfg%s.ini",m_strTag);
				filepathNew.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);

				CFileFind finder;

				if(finder.FindFile(filepath))
					::CopyFile(filepath,filepathNew,FALSE);
			}
		}
		
		Init();

		//Invalidate(false);
		ResizeOper(m_size.cx,m_size.cy);

		//m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));
	
		return 1;
	}
	else if(message == WM_USER+9444)	//메뉴를 열었을때
	{
		m_bStartMsg = TRUE;
		m_bLastStat = FALSE;

		if(m_strTag == "")
		{
			int nKey = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);

			if(nKey < 10)
			{
				nKey = nKey + 1000;
			}

			m_strTag.Format("%d",nKey);
		}


		CString strCfg,filepath;
		strCfg.Format("intercfg%s.ini",m_strTag);
		filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);
		
		m_fileCFG = filepath;


		Init();

		// 2011.12.09 KSJ
		CString basefile;
		basefile.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, "intercfg.ini");
		
		CFileFind finder;
		
		if(finder.FindFile(basefile))
			::CopyFile(basefile,filepath,FALSE);
		
		//Invalidate(false);
		ResizeOper(m_size.cx,m_size.cy);

		//m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

CString CMainWnd::SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	const int iFind = strData.Find(strToken); 
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}	
	return sResult;
}

LRESULT CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:	
		break;
	case DLL_OUB: 
		{
			CRecvData rdata;
			rdata.SetData(HIBYTE(LOWORD(wParam)), HIWORD(wParam), (char*)lParam);
			OperDLLOUB(rdata);
		}
		break;
	case DLL_OUBx:
		{
			struct	_extTH* th = (struct _extTH*)lParam;
			CRecvData rdata;
			rdata.SetData(th->size, th->key, (char*)(lParam + L_extTH));
			OperDLLOUB(rdata);
		}
		break;
	case DLL_ALERTx:
		{
			for(int ii =0; ii < m_nCntGrid; ii++)
				if(m_pGroupWnd[ii] != nullptr)
					m_pGroupWnd[ii]->SendMessage(WM_USER, wParam, lParam);
			break;
		}
	case DLL_TRIGGER:
		parsingTrigger((char*)lParam);
		break;
	case DLL_DOMINO:	
	case DLL_NOTICE:	
	case DLL_DROP:	
	case DLL_AXIS:		
	case DLL_EXCEL:	
	case DLL_INVOKE:	
	case DLL_SETPAL:	
		Invalidate(FALSE);
		break;
	case DLL_SETFONT:	
	case DLL_SETFCB:	
	case DLL_SETFONTx:	
	case DLL_SELECT:	
	case DLL_ACCOUNT:	
	case DLL_GUIDE:	
		{
	
			// xxx
			if (m_nCurGrid == 0)
				return 0;

			if(m_pGroupWnd[m_nCurGrid-1] != nullptr)
				m_pGroupWnd[m_nCurGrid-1]->SendMessage(WM_USER, wParam, lParam);
			break;
		}
	}
	return TRUE;
}

LRESULT CMainWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
		case MK_SELGROUP:
			switch(LOBYTE(HIWORD(wParam)))
			{
				case MO_SET:
					break;
			}
			break;
		case MK_CLOSEGRID:
			{
				const int nIndex = (int)lParam;
				int nGroup{};
				CString strGroup;
				if(m_nCntGrid > 1)
				{
					if(m_nCntGrid == nIndex +1)		
						DelGrid();
					else
					{
						for (int ii = nIndex + 1; ii < m_nCntGrid; ii++)
						{
							//2016.04.19 KSJ 삭제버튼 누른 그룹부터 한칸씩 앞당기면서 조회하고 
							//마지막 그리드를 지운다
							nGroup = m_pGroupWnd[ii]->GetGroupIndex();
							strGroup.Format("%d", nGroup + 1);
							m_pGroupWnd[ii - 1]->queryCodes(strGroup, GetGroupName(nGroup + 1));
						}
						DelGrid();
					}
				}
			}
			break;
	}
	return TRUE;
}
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_home = Variant(homeCC, _T(""));
	m_user = Variant(nameCC, _T(""));
	m_name = Variant(nameCC, _T(""));

	// TODO: Add your specialized creation code here
	SetTimer(8787,50,nullptr);
	return 0;
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1000)
	{
		KillTimer(1000);
	}
	else if(nIDEvent == 8787)
	{		
		KillTimer(8787);
		
		if(m_bStartMsg != TRUE)
		{
			m_bStartMsg = TRUE;
			
			if(m_strTag == "")
			{
				const int nKey = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);			
				m_strTag.Format("%d",nKey);
			}
			
			
			CString strCfg,filepath;
			strCfg.Format("intercfg%s.ini",m_strTag);
			filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, strCfg);
			m_fileCFG = filepath;

			Init();
			
			//ShowWindow(SW_SHOW);
			//			m_pWnd->GetParent()->ShowWindow(SW_SHOW);
		}
		Invalidate(false);
		
		//2012.01.13 KSJ 맵화면에서 관심종목을 열때 툴바가 생성되지 않았는데 열릴 때가 많음 그래서 따로 멤버변수로 저장하고
		//생성되면 메세지 보내줔.
		// 		if(m_bDominoToolWnd && m_pToolWnd != nullptr)
		// 			m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELECTGROUP, (LPARAM)m_nGroupIndex));			
	}
	CWnd::OnTimer(nIDEvent);
}

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch(LOWORD(wParam))
	{
		case IDC_CB_GROUP:
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				OnChangecbGroup();
			}
			break;
		case IDC_BT_PLUS:
			{
				if (m_nCntGrid >= MAX_GRID)
				{
					m_btPLUS.EnableWindow(FALSE);
				}
				else
				{
					if (m_nCntGrid == 2)	m_btMINUS.EnableWindow(TRUE);
					AddGrid();
				}
			}
			break;
		case IDC_BT_MINUS:
			{
				if (m_nCntGrid == 1)
				{
					m_btMINUS.EnableWindow(FALSE);
				}
				else
				{
					if (m_nCntGrid == (MAX_GRID - 1))	m_btPLUS.EnableWindow(TRUE);

					DelGrid();
				}
			}
			break;
		case IDC_BT_CONFIG:
			{
				int nGroupNumber;
				//선택한 탭, 넘기기 -> 서버작업 끝나고 반영
				nGroupNumber = m_cbGroup->GetCurSel();
				int	nOver = -1;
				nOver = MO_SELECT;
				
				savecfg();
				CString sztmp;
				sztmp.Format("%s /p5 /S /d %s%c%d%c%s%c", MAP_CONFIG, "selectGROUP", P_TAB ,nGroupNumber , P_NEW , "NO", P_TAB);
				SetPopup(sztmp);
			}
			break;
		case IDC_BT_SAVE:
			{
				for(int ii = 0; ii < m_nCntGrid; ii++)
				{
					m_pGroupWnd[ii]->SendMessage(WM_COMMAND, IDC_BT_SAVE_GRID, 0);
				}
			}
			break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	Draw(&dc);
	// Do not call CWnd::OnPaint() for painting messages
}

void CMainWnd::Draw(CDC *pDC)
{
	//배경그리기
	pDC->FillSolidRect(0, 0, m_size.cx, m_size.cy, GetIndexColor(66));
	
	for(int ii = 0; ii < m_nCntGrid; ii++)
	{
		if (ii == m_nCurGrid - 1)
		{
			if (m_pGroupWnd[ii] != nullptr)
				m_pGroupWnd[ii]->DrawTitle(TRUE);
		}
		else
		{
			if (m_pGroupWnd[ii] != nullptr)
				m_pGroupWnd[ii]->DrawTitle(FALSE);
		}
	}
}

void CMainWnd::DrawRoundRectangle(CDC *pDC, CRect rc)
{
	const CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, m_crBodyBk);
	DrawBitmap(pDC, rc.left,    rc.top,      5, 5, m_hRndBmp[0]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.top,      5, 5, m_hRndBmp[1]->operator HBITMAP());
	DrawBitmap(pDC, rc.left,    rc.bottom-5, 5, 5, m_hRndBmp[2]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.bottom-5, 5, 5, m_hRndBmp[3]->operator HBITMAP());
}

void CMainWnd::DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol)
{
	HDC hDC = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP tBmp = (HBITMAP)::SelectObject(hDC, hBitmap);
	::TransparentBlt(pDC->m_hDC, x, y, w, h, hDC, 0, 0, w, h, tCol);
	::SelectObject(hDC, tBmp);
	::DeleteObject(hDC);
}

void CMainWnd::DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol)
{
	DrawBitmap(pDC, pRC->left, pRC->top, pRC->Width(), pRC->Height(), hBitmap, tCol);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//2011.12.27 KSJ 최소화하면 cy가 4가된다.
	if (cx <= 10 || cy <= 10)
	{
		return;
	}

	//m_size = CSize(cx, cy);

	ResizeOper(cx, cy);
}

void CMainWnd::OnChangecbGroup()
{	
	const int nIndex = m_bTrigger ? m_pGroupWnd[(m_nCurGrid - 1) > 0 ? (m_nCurGrid - 1) : 0]->GetGroupIndex() : m_cbGroup->GetCurSel();
	m_cbGroup->GetLBText(nIndex, m_strGroupName);

	if(nIndex == -1) return;
	else if (nIndex == -2)	m_strGroupName = "";	//2016.05.04 KSJ 빈그리드일떄는 -2로 온다. 그러면 그룹이름을 지워준다.


	CString strIndex;
	//콤보박스는 0부터 시작이고 그룹넘버는 1부터 시작
	strIndex.Format("%d", nIndex+1);
	
	BOOL bFind = FALSE;

//	//2016.04.19 KSJ 현재 선택된 그리드가 전체갯수보다 클수가 없다.
//	if (m_nCntGrid < m_nCurGrid)	m_nCurGrid = m_nCntGrid;
	
	m_nCurGrid = min(m_nCntGrid, m_nCurGrid);
	if((m_nCurGrid >= 1) && (m_pGroupWnd[m_nCurGrid-1]->GetGroupIndex() == nIndex))
	{
		bFind = FALSE;
	}
	else if( m_nCntGrid > 1 )
	{
		for(int ii = 0; ii < m_nCntGrid; ii++)
			if(m_pGroupWnd[ii]->GetGroupIndex() == nIndex) bFind = TRUE;
	}

	// 	//2016.05.02 KSJ 현재그룹 다시 콤보박스에 세팅
	// 	m_cbGroup->SetCurSel(m_pGroupWnd[m_nCurGrid-1]->GetGroupIndex());

	if (m_nCurGrid <= 0)
		return;

	if (bFind)	Variant(guideCC, "같은 그룹이 이미 있습니다.");
	else		m_pGroupWnd[m_nCurGrid - 1]->queryCodes(strIndex, m_strGroupName);
}

void CMainWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMainWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	int nGroup = -1;

	if (m_nCurGrid != m_nPreCurGrid)
	{
		for (int ii = 0; ii < m_nCntGrid; ii++)
		{
			if (ii == m_nCurGrid - 1)
			{
				m_pGroupWnd[ii]->DrawTitle(TRUE);
			}
			else
				m_pGroupWnd[ii]->DrawTitle(FALSE);
		}
		//2016.04.26 KSJ 선택시 빈그룹이면 콤보박스도 빈걸로 선택
		if (m_nCurGrid > 0)
		{
			if (m_pGroupWnd[m_nCurGrid - 1]->GetGroupIndex() == -2)
				nGroup = -1;
			else
				nGroup = m_pGroupWnd[m_nCurGrid - 1]->GetGroupIndex();

		}
		m_cbGroup->SetCurSel(nGroup);
	}

	m_nPreCurGrid = m_nCurGrid;
	CWnd::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
void CMainWnd::Init()
{
//XMSG("...");
	HBITMAP	hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};

	Variant(titleCC, "[2027]관심종목Lite");

	CString img_path;
	img_path.Format(_T("%s\\image\\axpanel%s1_"), (LPCSTR)m_home, "blue");
	
	m_hRndBmp[0] = GetAxBitmap((LPCSTR)( img_path + _T("lt.bmp")));
	m_hRndBmp[1] = GetAxBitmap((LPCSTR)( img_path + _T("rt.bmp")));
	m_hRndBmp[2] = GetAxBitmap((LPCSTR)( img_path + _T("lb.bmp")));
	m_hRndBmp[3] = GetAxBitmap((LPCSTR)( img_path + _T("rb.bmp")));

	m_profile.Format(CONFIG_INI, m_home, m_name);
	m_fileIMG.Format("%s\\%s\\", m_home, IMAGEDIR);

	//파일복사
	CString basefile;
	basefile.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_CONFIG);
	
	CFileFind finder;
	
	//마지막 저장화면 읽어올때 저장된게 있으면 복사하지 않는다.
	if (finder.FindFile(basefile))
	{
		if (!finder.FindFile(m_fileCFG) || !m_bLastStat)	::CopyFile(basefile, m_fileCFG, FALSE);
	}

	//그룹갯수
	int nCount;
	nCount = GetPrivateProfileInt("GROUP", "COUNT", 1, m_fileCFG);	
	if (nCount > MAX_GRID)	nCount = MAX_GRID;	//2016.06.28 KSJ
	
	loadcfg();

	int nWidth;
	nWidth = m_size.cx/nCount /*- SIZE_GAPS * nCount*/;

	CRect rect(SIZE_GAPS, 0, nWidth > 50 ? nWidth:100, m_size.cy);
	CRect rcTemp;
	
 	rect.top += 25;
	rect.bottom -= SIZE_DIVIDE;

	m_arRECT.Add(rect);
	m_arRATE.Add(rect.Width());

	m_size = CSize(rect.right, rect.bottom + SIZE_DIVIDE);

	InitGroup(rect, nCount);

	//그룹선택 콤보박스
	m_cbGroup = std::make_unique<CComboBox>();
	m_cbGroup->Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(rect.left, rect.top-22, rect.left+150, SIZE_BUTTONS), this, IDC_CB_GROUP);
	m_cbGroup->MoveWindow(CRect(rect.left, rect.top-22, rect.left+150, rect.top+100));
	m_cbGroup->SetFont(m_pFont, FALSE);
	m_cbGroup->ShowWindow(SW_SHOW);
	SetWindowTheme(m_cbGroup->GetSafeHwnd(), L"", L"");

	//설정버튼
	hBITMAP    = GetAxHBitmap(m_fileIMG + "조회BTN.BMP");
	hBITMAP_DN = GetAxHBitmap(m_fileIMG + "조회BTN_dn.BMP");
	hBITMAP_HV = GetAxHBitmap(m_fileIMG + "조회BTN_dn.BMP");
	
	rcTemp.left = rect.left + 160;
	rcTemp.top = rect.top-22;
	rcTemp.right = rcTemp.left + 37;
	rcTemp.bottom = rcTemp.top + SIZE_BUTTONS;
	
	m_btCONFIG.Create("설정\n관심종목설정", rcTemp, this, IDC_BT_CONFIG);
	m_btCONFIG.SetFont(m_pFont, false);  
	m_btCONFIG.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	
	//저장버튼
	hBITMAP = GetAxHBitmap(m_fileIMG + "2BTN.BMP");
	hBITMAP_DN = GetAxHBitmap(m_fileIMG + "2BTN_dn.BMP");
	hBITMAP_HV = GetAxHBitmap(m_fileIMG + "2BTN_dn.BMP");
	
	rcTemp.left = rcTemp.right + SIZE_GAPS;
	rcTemp.right = rcTemp.left + 36;
	
	m_btSAVE.Create("저장\n전체 관심종목저장", rcTemp, this, IDC_BT_SAVE);
	m_btSAVE.SetFont(m_pFont, false);  
	m_btSAVE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	
	//마이너스 버튼
	hBITMAP = GetAxHBitmap(m_fileIMG + "MINUSBTN.bmp");
	hBITMAP_DN = GetAxHBitmap(m_fileIMG + "MINUSBTN_dn.bmp");
	hBITMAP_HV = GetAxHBitmap(m_fileIMG + "MINUSBTN_en.bmp");
	
	rcTemp.left = rcTemp.right + SIZE_GAPS;
	rcTemp.right = rcTemp.left + SIZE_BUTTONS;
	
	m_btMINUS.Create("\n화면줄이기", rcTemp, this, IDC_BT_MINUS);
	m_btMINUS.SetFont(m_pFont, false);  
	m_btMINUS.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	
	//플러스 버튼
	hBITMAP    = GetAxHBitmap(m_fileIMG + "PLUSBTN.bmp");
	hBITMAP_DN = GetAxHBitmap(m_fileIMG + "PLUSBTN_dn.bmp");
	hBITMAP_HV = GetAxHBitmap(m_fileIMG + "PLUSBTN_en.bmp");
	
	rcTemp.left = rcTemp.right + SIZE_GAPS;
	rcTemp.right = rcTemp.left + SIZE_BUTTONS;
	
	m_btPLUS.Create("\n화면늘이기", rcTemp, this, IDC_BT_PLUS);
	m_btPLUS.SetFont(m_pFont, false);  
	m_btPLUS.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	rect.right += SIZE_GAPS;

	m_size.cx += SIZE_GAPS;

	rect.top -= 25;

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKEWPARAM(m_size.cx, m_size.cy));

	//배경그리기
	CDC *pDC = GetDC();

	pDC->FillSolidRect(rect, GetIndexColor(66));
	ReleaseDC(pDC);

	queryGroupList();
	RequestMarketTime();

	if (m_nCntGrid == 1)
		m_btMINUS.EnableWindow(FALSE);
	else if (m_nCntGrid == MAX_GRID)
		m_btPLUS.EnableWindow(FALSE);

	ResizeOper(m_size.cx, m_size.cy);
}

void CMainWnd::InitGroup(CRect rect, int nCount)
{
	for(int ii= 0; ii < nCount; ii++)
	{
		if(ii == 0)
		{
			m_pGroupWnd[ii] = std::make_shared<CGroupWnd>(m_pParent, ii);	
			m_pGroupWnd[ii]->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE, rect, this, IDC_GROUP+ii);
			m_pGroupWnd[ii]->InitGrid();
		}
		else	
			AddGrid();
	}
	m_nCurGrid = min(m_nCntGrid - 1, m_nCurGrid);
}

CString CMainWnd::GetGroupName(int nGroupIndex)
{
	CString strRet;

	if(nGroupIndex <= 0)	strRet = m_strGroupName;
	else			m_cbGroup->GetLBText(nGroupIndex - 1, strRet);

	return strRet;
}

void CMainWnd::SetCurGroup(int nSel)
{
	if(m_cbGroup != nullptr) m_cbGroup->SetCurSel(nSel); 
}

void CMainWnd::SetSelectGroup(int nSel)
{
	m_nCurGrid = nSel;
}	//현재 선택된 그룹 화면에 보이는 그룹(추가한 그룹)

void CMainWnd::SendOper(class CSendData* sdata)
{
	SendTR(sdata->GetTRCODE(), std::byte(US_PASS), sdata->GetData(), sdata->GetDataLen(), sdata->GetDataKey());
}

//장운영시간 조회 
void CMainWnd::RequestMarketTime()
{
	CSendData	sdata;
	char key{};
	_trkey* trkey = (struct _trkey*)&key;	
	trkey->kind = TRKEY_MARKETTIME;
	
	sdata.SetData("bo221199", key, "1", 1, "");		
	SendOper(&sdata);
}

struct MarketTime
{
	char row1[30];	//장전시간외
	char row2[30];	//동시호가
	char row3[30];	//장내시간
	char row4[30];	//동시호가
	char row5[30];	//장후시간외
	char row6[30];	//시간외 단일가
};

void CMainWnd::SetMarketTime(char* datB)	//2012.11.26 KSJ 장운영구분 조회
{
	struct MarketTime *mkTime = (struct MarketTime *)datB;
	
	CString strRow1, strRow2, strRow3, strRow4, strRow5, strRow6;
	CString strTemp;
	
	strRow1 = CString(mkTime->row1, 30);	//장전시간외
	strRow2 = CString(mkTime->row2, 30);	//동시호가
	strRow3 = CString(mkTime->row3, 30);	//장내시간
	strRow4 = CString(mkTime->row4, 30);	//동시호가
	strRow5 = CString(mkTime->row5, 30);	//장후시간외
	strRow6 = CString(mkTime->row6, 30);	//시간외 단일가
	
	m_strMarketTime = strRow2;
}
//////////////////////////////////////////////////////////////////////////


void CMainWnd::queryGroupList()
{
	m_cbGroup->ResetContent();

	CSendData sdata;
		
	char key{};
	_trkey* trkey = (struct _trkey*)&key;
	int	sendL = 0;
	std::string sendB;
	sendB.resize(16 * 1024);
	const int isz_updn = sz_uinfo;
	
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';


	trkey->kind = SEND_GROUPLIST;
	
	sdata.SetData(trUPDOWN, key, sendB.data(), sendL, "");
	SendOper(&sdata);
}


void CMainWnd::parsingGroupList(char *datB, int datL)
{
	if (datL < sizeof(_glistex) + 4)
		return;

	CString data(datB, datL);
	const int cnt = atoi(data.Left(4));
	data = data.Mid(4);
	data.Trim();

	if (!data.IsEmpty() && cnt > 0)
	{
		std::vector<std::pair<CString, CString>> vGroupName;
		const gsl::span<_glistex> groupList((_glistex*)data.GetString(), cnt);
		
		std::sort(groupList.begin(), groupList.end(), [](auto item, auto item1){
			return CString(item.ngrs, 2).Trim().Compare((CString(item1.ngrs, 2).Trim())) < 0;
		});
		
		for_each(groupList.begin(), groupList.end(), [&](auto item) {
			const CString groupKey = CString(item.ngrs, 2).Trim();
			const CString groupName = CString(item.gnam, 30).Trim();

			m_cbGroup->AddString(groupName);
		});
	}

	if (cnt == 0)
		return;

	m_bTrigger = FALSE;

	CString strGroup, strGrid;
	int nGroup{}, nKind{};

	for(int ii = 0; ii < m_nCntGrid; ii++)
	{
		strGrid.Format("GRID%02d", ii);
		nKind = GetPrivateProfileInt(strGrid, KEY_DATA, 0, m_fileCFG);	
		nGroup = max(CAST_TREEID(nKind)->value, 1);		//2016.03.30 KSJ 그룹을 그대로 가져오기			
		
		if ((nGroup - 1) < m_cbGroup->GetCount())
		{
			if(ii == 0 )	
			{
				
				m_cbGroup->SetCurSel(nGroup - 1);
				m_nCurGrid = 1;
			}

			if (nGroup > 150)
				break;

			strGroup.Format("%d", nGroup);
			m_pGroupWnd[ii]->queryCodes(strGroup, GetGroupName(nGroup));
		}
	}
}

void CMainWnd::parsingTrigger(CString datB)
{
	CString	symbol, strDataB;	
	strDataB = datB;	
	symbol = IH::Parser(datB, "\t");
	
	if (symbol == _T("OnPortfolio") && datB == _T("ok"))
	{	
		m_bTrigger = TRUE;		
		queryGroupList();
		for (int ii = 0; ii < m_nCntGrid; ii++)
			if (m_pGroupWnd[ii] != nullptr)
				m_pGroupWnd[ii]->FieldSetup(false);
		//loadfield();
		
	}	
	else if (symbol == _T("1301"))	// 2012.04.19 KSJ 관심종목이 활성화 되어있을때는 트리거가 안보내져서 추가함.
	{
		Variant(triggerCC, strDataB);
	}
}


LONG CMainWnd::OperDLLOUB(CRecvData& rdata)
{
	WORD kind = rdata.key;
	struct _trkey* trkey = (struct _trkey*)&kind;

	switch (trkey->kind) 
	{
	case TRKEY_MARKETTIME:	//0212.11.25 장운영시간 조회
		SetMarketTime(rdata.data);
		break;
	case TRKEY_GRIDNEW:
	case TRKEY_GRIDNEWS:
	case TRKEY_GRIDROW:
	case TRKEY_GRIDSAVE:
		{
			for(int ii =0; ii < m_nCntGrid; ii++)
			{
				if(m_pGroupWnd[ii] != nullptr && m_pGroupWnd[ii]->GetGridIndex() == trkey->group)
					m_pGroupWnd[ii]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
			}
		}
		break;
	case SEND_GROUPLIST:
		parsingGroupList(rdata.data, rdata.size);
		break;
	case SEND_CODES:
	case SEND_GRID:
		{
			if(m_bTrigger)
			{
				for(int ii = 0; ii < m_nCntGrid; ii++)
				{
					if(m_pGroupWnd[ii] != nullptr)
					{
						m_pGroupWnd[ii]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
						m_pGroupWnd[ii]->Invalidate(FALSE);
					}
				}
				m_bTrigger = FALSE;
			}
			else
			{
				if ( IS_WITHIN(0, m_nCntGrid - 1, (int)trkey->group) )
				{
					m_pGroupWnd[trkey->group]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
					m_pGroupWnd[trkey->group]->Invalidate(FALSE);
				}
			}
		}
	default:
		break;
	}		
	return 0;
}

void CMainWnd::ResizeOper(int cx, int cy)
{
	if(!m_bStartMsg) return;

	int	minx = 0;
	const int miny = SIZE_TOOL + SIZE_GROUPMINY;
	const int ncnt = m_nCntGrid;
	
	minx += (ncnt * SIZE_GROUPMINX);

	if (cx < minx)
		cx = minx;
	if (cy < miny)
		cy = miny;

	m_size = CSize(cx, cy);
	const CRect rect(0, 0, m_size.cx, m_size.cy);

	MakeGridRect(rect);

	CRect	rcTool(GAP1, GAP2, m_size.cx - GAP1, SIZE_TOOL / 2 + 3);

	m_rcDivide[0] = CRect(0, 0, 0, 0);
	m_rcDivide[1] = CRect(0, 0, 0, 0);

	m_rcPanel[0] = CRect(0, 0, 0, 0);
	m_rcPanel[1] = CRect(0, 0, 0, 0);
	m_rcPanel[2] = CRect(0, 0, 0, 0);

	//GRID BACKGROUND OUTBOX
	m_rcPanel[2].right = rcTool.right+GAP1;
	m_rcPanel[2].bottom = m_size.cy;
	
	//GRID OUTBOX
	m_rcPanel[1].left = GAP1;
	m_rcPanel[1].top = rcTool.bottom +SIZE_DIVIDE*3/2 ;//back
	m_rcPanel[1].right = rcTool.right;
	m_rcPanel[1].bottom = m_size.cy - GAP1;

	rcTool.top = rcTool.bottom + SIZE_DIVIDE*2;//BUFFET ADD SIZE_DIVIDE

	rcTool.left = SIZE_DIVIDE*2/3;
	rcTool.left += 0;
	rcTool.bottom = m_size.cy - SIZE_DIVIDE;//buffet
	rcTool.right = rcTool.right - SIZE_DIVIDE*2/3;//BUFFET

	CRect	rcGrid;	

	if(m_arRECT.GetSize() > 0)
	{
		for ( int ii = 0 ; ii < m_nCntGrid ; ii++ )
		{
			rcGrid = m_arRECT.GetAt(ii);

			if(m_pGroupWnd[ii] != nullptr)		
				m_pGroupWnd[ii]->MoveWindow(rcGrid);
		}
	}

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(m_size.cx, m_size.cy));

	Invalidate(FALSE);//BUFFET 	
}

void CMainWnd::MakeGridRect(CRect rcFull)
{
	m_arRECT.RemoveAll();
	m_arDIVIDE.RemoveAll();
	m_arUpDIV.RemoveAll();
	m_arDnDIV.RemoveAll();
	
	const int nTotSize = m_size.cx;
	int nGapSize = 0;
	
	nGapSize = SIZE_DIVIDE * m_nCntGrid +2 ;
	
	CRect rcGrid = rcFull;
	rcGrid.right = 0;
	CRect rcDivide = rcFull;
	
	CRect rcUp = rcFull;
	CRect rcDn = rcFull;
	
	rcUp.bottom = rcGrid.top + SIZE_DIVIDE;
	rcDn.top = rcGrid.bottom - SIZE_DIVIDE;
	
	rcGrid.top = rcGrid.top + 25;//BUFFET BLOCK
	rcGrid.bottom  = rcGrid.bottom - SIZE_DIVIDE;//BUFFET BLOCK
	
	int nWidth = (rcFull.Width() - nGapSize) / m_nCntGrid;
	
	for (int i = 0; i < m_nCntGrid+1; i++)//BUFFET +1
	{
		rcGrid.left = rcGrid.right;
		
		rcUp.left = rcGrid.right;
		rcDn.left = rcGrid.right;
		
		rcGrid.left += SIZE_DIVIDE;		
		if (i == m_nCntGrid - 1 )//BUFFET +1
			nWidth = (rcFull.Width() - nGapSize) - (nWidth * (m_nCntGrid - 1));
		
		rcGrid.right = rcGrid.left + nWidth ;
		
		rcUp.right = rcGrid.right  ;
		rcDn.right = rcGrid.right  ;
		
		rcDivide.left = rcGrid.left - SIZE_DIVIDE;
		
		rcDivide.right = rcGrid.left;
		
		if (i < m_nCntGrid+1 )//BUFFET +1
			m_arDIVIDE.Add(rcDivide);// 
		
		if ( i < m_nCntGrid)
		{
			m_arRECT.Add(rcGrid);
			m_arUpDIV.Add(rcUp);
			m_arDnDIV.Add(rcDn);
			
		}
	}
}

void CMainWnd::AddGrid()
{
	const int nWidth = m_arRECT.GetAt(m_nCntGrid -1).Width();
	CRect rect = m_arRECT.GetAt(m_nCntGrid -1);
	const int nScrMax = GetSystemMetrics(SM_CXVIRTUALSCREEN); 
	
	m_nCntGrid++;

	if (m_arRECT.GetSize())
	{				
		if (  m_nCntGrid * nWidth > nScrMax ) //over screen size
		{
			m_nCntGrid--;
			MessageBox("표시할 수 있는 범위를 벗어 났습니다.\n     화면사이즈를 줄여 주십시오.","IBK투자증권", MB_OK); //2013.07.08 문구 변경
		}else
		{
			if(m_nCntGrid < MAX_GRID)
			{
				int nIndex = m_nCntGrid -1;

				rect.left = rect.right + SIZE_GAPS;
				rect.right += nWidth + SIZE_GAPS;
				m_size.cx += nWidth + SIZE_GAPS;
				
				if (IS_WITHIN(0, m_nCntGrid - 1, nIndex))
				{
					m_pGroupWnd[nIndex] = std::make_shared<CGroupWnd>(m_pParent, nIndex);
					m_pGroupWnd[nIndex]->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE, rect, this, IDC_GROUP + nIndex);
					m_pGroupWnd[nIndex]->InitGrid();
				}
				m_arRECT.Add(rect);
				m_arRATE.Add(rect.Width());

				ResizeOper(m_size.cx,m_size.cy);

				if (m_btMINUS.GetSafeHwnd())
					m_btMINUS.EnableWindow(TRUE);
			}
		}
	}
	else
	{
		m_nCntGrid--;
	}

	if (m_nCntGrid > MAX_GRID)
		m_btPLUS.EnableWindow(FALSE);

	m_nCntGrid = min(m_nCntGrid, MAX_GRID - 1);	
}

void CMainWnd::DelGrid()
{
	const int   nWidth  = m_arRECT.GetAt(m_nCntGrid -1).Width();
	const CRect rect    = m_arRECT.GetAt(m_nCntGrid -1);
	const int   nScrMax = GetSystemMetrics(SM_CXVIRTUALSCREEN); 

	m_nCntGrid--;
	if(m_nCntGrid < MAX_GRID)
	{
		const int nIndex = m_nCntGrid;				
		m_size.cx -= (nWidth + SIZE_GAPS);
		if (nIndex > 0)
		{
			m_pGroupWnd[nIndex]->DestroyWindow();
			m_pGroupWnd[nIndex].reset();
		}
		m_btPLUS.EnableWindow(TRUE);
		m_arRECT.RemoveAt(m_nCntGrid);
		
		if(m_nCntGrid == 1)	
			m_btMINUS.EnableWindow(FALSE);
		
		ResizeOper(m_size.cx,m_size.cy);	
	}	
}

void CMainWnd::DrawDivide()
{
	CDC *pDC = GetDC();

	pDC->FillSolidRect(&m_rcPanel[2], GetIndexColor(64));

	pDC->FillSolidRect(&m_rcPanel[0], GetIndexColor(192));

	pDC->FillSolidRect(&m_rcPanel[1], GetIndexColor(192));

	ReleaseDC(pDC);
}

int CMainWnd::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X') ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		ss++;
		firsttime = false;
	}
	[[gsl::suppress(26408)]]
	free(mstr);
	return result;
}

void CMainWnd::loadfield()
{
	CString filePath;
	filePath.Format("%s/%s/%s/%s", m_home, USRDIR, m_user, FILE_FIELD);
	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;
	
	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_BKCOLOR, "1", readB, sizeof(readB), filePath);

	m_chUse = readB[0];//BK 사용/미사용

	if (readB[0] == '2')
	{
		readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_COLOR2, string, readB, sizeof(readB), filePath);
		if (readL > 0)
		{
			string = CString(readB, readL);

			m_line = atoi(IH::Parser(string, deliMITER));
			if (m_line <= 0) m_line = 1;
			m_bkColor1 = _httoi(IH::Parser(string, deliMITER));
			m_bkColor2 = _httoi(IH::Parser(string, deliMITER));
		}
	}
	else
	{
		readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_COLOR1, "", readB, sizeof(readB), filePath);
		if (readL > 0)
		{
			string = CString(readB, readL);

			m_line = 1;
			m_bkColor1 = _httoi(IH::Parser(string, deliMITER));
			m_bkColor2 = m_bkColor1;
		}
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FRCOLOR, "", readB, sizeof(readB), filePath);

	if (readL > 0)
	{
		string = CString(readB, readL);

		for (int ii = 0; ii < 4; ii++)
			m_fgColor[ii] = _httoi(IH::Parser(string, deliMITER));
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_LINKMAP, "1", readB, sizeof(readB), filePath);
	if (readL > 0)
	{
		string = CString(readB, readL);

		stringx = IH::Parser(string, deliMITER);
		if (stringx.GetLength() > 0)
		{
			switch (stringx.GetAt(0))
			{
			case '1': m_dblAction = dblCURR;	break;
			case '2': m_dblAction = dblCHART;	break;
			case '4':
					m_dblAction = dblUSER;
					m_linkMap = IH::Parser(string, deliMITER); break;
			default:  m_dblAction = dblCURR;	break;
			}
		}
	}
	else
	{
		m_dblAction = dblCURR;
	}

	m_selAction = GetPrivateProfileInt(SEC_INTERFIELD, "SELACTION", 1, filePath);

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FONTPOINT, "9", readB, sizeof(readB), filePath);
	
	if (readL > 0) 
		m_fontsize = atoi(CString(readB, readL));

	if (m_fontsize < 8 || m_fontsize > 15)
		m_fontsize = 9;
	
	m_rowH = DEF_ROWH;
	switch (m_fontsize)
	{
	case 8:
		m_rowH = DEF_ROWH - 4;
		break;
	case 11:
		m_rowH = DEF_ROWH + 2;
		break;
	case 12:
		m_rowH = DEF_ROWH + 4;
		break;
	case 13:
		m_rowH = DEF_ROWH + 6;
		break;
	case 14:
		m_rowH = DEF_ROWH + 8;
		break;
	case 15:
		m_rowH = DEF_ROWH + 10;
		break;
	}	

	m_fontname = _T("굴림체");

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FONTNAME, "굴림체", readB, sizeof(readB), filePath);	
	if (readL > 0) 
		m_fontname.Format("%s", readB);

	m_bCurr = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_CURRBOLD, 1, filePath);
	m_bName = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_NAMEBOLD, 1, filePath);
	m_bAllBold = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_ALLBOLD, 0, filePath);
}

void CMainWnd::loadcfg()
{
	// load resource
	m_pBrush = GetAxBrush(RGB(100, 100, 100));
	m_clrBack = GetIndexColor(68);
	m_bkColor1 = GetIndexColor(68);
	m_bkColor2 = GetIndexColor(77);//LINECOLOR BUFFET
	m_chUse = 0x00;
	
	for (int ii = 0; ii < 4; ii++)
		m_fgColor[ii] = GetIndexColor(39);
		
	const  int	readL = 0;
	char	readB[1024]{};
	CString	string, stringx;
	
	// height
//	m_size.cx = m_size.cy = 0;
	m_size.cy = GetPrivateProfileInt(SEC_MAIN, KEY_HEIGHT, SIZE_HEIGHT, m_fileCFG);

	memset(readB, 0x00, sizeof(readB));
	GetPrivateProfileString(SEC_GROUP, KEY_RATE, "", readB, sizeof(readB), m_fileCFG);
	string.Format("%s", readB);
	
	int nGroup;
	nGroup = GetPrivateProfileInt(SEC_MAIN, KEY_GROUPSIZE, SIZE_GROUP, m_fileCFG);

 	m_size.cx = nGroup;

	if (string.IsEmpty())
	{
		for (int ii = 0 ; ii < m_nCntGrid ; ii++ )
		{
			string += "100,";
		}
	}
	
	//SetArray(string);
	m_nCurGrid = GetPrivateProfileInt(SEC_GROUP, KEY_SELECT, 0, m_fileCFG) + 1;
	
	loadfield();
}

void CMainWnd::savecfg()
{
	// delete resource
	CString	sztmp = _T(""), sztmp2 = _T(""), title = _T("");
	// group count
	sztmp.Format("%d", m_nCntGrid);	
	WritePrivateProfileString(SEC_GROUP, KEY_COUNT, sztmp.operator LPCTSTR(), m_fileCFG);
	
	// groupwnd size rate
	sztmp = _T("");
	const int ncnt = m_arRATE.GetSize();

	WritePrivateProfileString(SEC_GROUP, KEY_RATE, sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_size.cx);//BUFFET ADD FOR SIZE 
	WritePrivateProfileString(SEC_MAIN, KEY_GROUPSIZE, sztmp, m_fileCFG);
	
	sztmp.Format("%d", m_size.cy);
	WritePrivateProfileString(SEC_MAIN, KEY_HEIGHT, sztmp, m_fileCFG);	
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	CString basefile;
	basefile.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_CONFIG);

	CFileFind finder;
	
	// TODO: Add your message handler code here
	for(int ii = 0; ii < m_nCntGrid; ii++)
	{
		if(m_pGroupWnd[ii] != nullptr)
		{	
			m_pGroupWnd[ii]->DestroyWindow();
			m_pGroupWnd[ii].reset();
		}
	}
	
	savecfg();
	if(finder.FindFile(m_fileCFG))
	{
		::DeleteFile(basefile);
		::CopyFile(m_fileCFG,basefile,FALSE);
		::DeleteFile(m_fileCFG);
	}

	if(m_cbGroup != nullptr)	m_cbGroup->DestroyWindow();
}