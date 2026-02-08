// RTSWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "RTSWnd.h"
#include "MainWnd.h"
#include "../../control/fx_gridex/ugvscrol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	S_FILE		"u_config.ini"
#define	flagRTS		"w  "
#define	keyRTS		"quote"

/////////////////////////////////////////////////////////////////////////////
// CRTSWnd

CRTSWnd::CRTSWnd(CWnd* pMainWnd) : CBaseWnd(pMainWnd)
{
	m_strTitle = _T("");
	m_nKind = -1;
	m_pGrid = NULL;

	m_pConfig = new Cconfig;
	m_bOn = FALSE;
	m_nRow = -1;
}

CRTSWnd::~CRTSWnd()
{
	delete m_pConfig;

	if ( m_pGrid ) delete m_pGrid;//BUFFET
}


BEGIN_MESSAGE_MAP(CRTSWnd, CBaseWnd)
	//{{AFX_MSG_MAP(CRTSWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRTSWnd message handlers

void CRTSWnd::OperDraw(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, m_clrPallete);

	DrawTitle(pDC, rcClient);
}

void CRTSWnd::DrawTitle(CDC* pDC, CRect rcClient)
{
	CRect rcTitle = rcClient;
	rcTitle.bottom = rcTitle.top + 24;

	pDC->FillSolidRect(rcTitle, RGB(255, 128, 128));
	CFont* pOldFont = pDC->SelectObject(GetAxFont(_T("굴림체"), 9, TRUE, FALSE));
	CString strTitle = _T(" ::실시간 체결내역 (체결대금 1000만원 이상):: ");
	strTitle += m_strTitle;
	pDC->DrawText(strTitle, rcTitle, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	pDC->SelectObject(pOldFont);
}

void CRTSWnd::SetSelectItem(CString strTitle, UINT nKind)
{
	if (m_bOn)
		SendTR(TRUE);

	m_strTitle = strTitle;
	m_nKind = nKind;
	Invalidate();

	m_pGrid->EmptyAll();

	if (m_nRow > -1)
	{
		int nCount = m_pGrid->GetNumberCols();
		for (int i = 0; i < nCount; i++)
			m_pGrid->QuickSetBackColor(i, m_nRow, GetAxColor(CLR_BACK));

		m_pGrid->RedrawAll();
		m_nRow = -1;
	}

	SendTR();
}

void CRTSWnd::OnDestroy() 
{
	CBaseWnd::OnDestroy();
	
	if (LIB_IsWndAlive(m_pGrid))
		LIB_DeleteWnd(m_pGrid);
}

void CRTSWnd::OperInit()
{
	m_pGrid = new CRTSGrid;
	m_pGrid->CreateGrid(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_RTSGRID);	

	LoadConfig();
}

void CRTSWnd::OperResize(int cx, int cy)
{
	if (!m_pGrid->GetSafeHwnd())
		return;

	CRect rcGrid(0, 24, cx, cy);
	m_pGrid->MoveWindow(rcGrid);
}

void CRTSWnd::LoadConfig()
{
	char	wb[32];
	CString	path, tmps;

	path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, S_FILE);

	// 체결량 범위   (vol * 1,000)
	GetPrivateProfileString(_T("2022"), "vol", "3", wb, sizeof(wb), path);
	m_pConfig->m_vol = max(1, atoi(wb));

	// 체결금액 범위 (amt * 10,000,000)
	GetPrivateProfileString(_T("2022"), "amt", "3", wb, sizeof(wb), path);
	m_pConfig->m_amt = max(1, atoi(wb));

	// and / or
	GetPrivateProfileString(_T("2022"), "and", "0", wb, sizeof(wb), path);
	m_pConfig->m_and = atoi(wb) ? true : false;

	// 가격범위 설정
	GetPrivateProfileString(_T("2022"), "pricef", "0", wb, sizeof(wb), path);
	m_pConfig->m_price = atoi(wb) ? true : false;

	// 가격범위 시작값
	GetPrivateProfileString(_T("2022"), "sprice", "0", wb, sizeof(wb), path);
	m_pConfig->m_sprc = atoi(wb);

	// 가격범위 끝값
	GetPrivateProfileString(_T("2022"), "eprice", "0", wb, sizeof(wb), path);
	m_pConfig->m_eprc = atoi(wb);

	// 상한
	GetPrivateProfileString(_T("2022"), "updn", "11111", wb, sizeof(wb), path);
	tmps = wb;
	int ii = 0;
	for (int ii = 0; ii < tmps.GetLength(); ii++)
	{
		switch (ii)
		{
		case 0:	m_pConfig->m_ulimit = (tmps.GetAt(ii) == '1') ? true : false;
		case 1:	m_pConfig->m_up     = (tmps.GetAt(ii) == '1') ? true : false;
		case 2:	m_pConfig->m_flat   = (tmps.GetAt(ii) == '1') ? true : false;
		case 3:	m_pConfig->m_dlimit = (tmps.GetAt(ii) == '1') ? true : false;
		case 4:	m_pConfig->m_down   = (tmps.GetAt(ii) == '1') ? true : false;
		}
	}
	for ( ; ii < 5; ii++)
	{
		switch (ii)
		{
		case 0:	m_pConfig->m_ulimit = true;
		case 1:	m_pConfig->m_up     = true;
		case 2:	m_pConfig->m_flat   = true;
		case 3:	m_pConfig->m_dlimit = true;
		case 4:	m_pConfig->m_down   = true;
		}
	}
}

void CRTSWnd::SaveConfig()
{
	CString	path, tmps;

	path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, S_FILE);

	// 체결량 범위   (vol * 1,000)
	tmps.Format("%d", m_pConfig->m_vol);
	WritePrivateProfileString(_T("2022"), "vol", tmps, path);

	// 체결금액 범위 (amt * 10,000,000)
	tmps.Format("%d", m_pConfig->m_amt);
	WritePrivateProfileString(_T("2022"), "amt", tmps, path);

	// 가격범위 설정
	WritePrivateProfileString(_T("2022"), "pricef", m_pConfig->m_price ? _T("1") : _T("0"), path);

	// 가격범위 시작값
	tmps.Format("%d", m_pConfig->m_sprc);
	WritePrivateProfileString(_T("2022"), "sprice", tmps, path);

	// 가격범위 끝값
	tmps.Format("%d", m_pConfig->m_eprc);
	WritePrivateProfileString(_T("2022"), "eprice", tmps, path);
	
	// and / or
	WritePrivateProfileString(_T("2022"), "and", m_pConfig->m_and ? _T("1") : _T("0"), path);

	// 상한 / 상승 / 보합 / 하한 / 하락
	tmps.Empty();
	tmps += m_pConfig->m_ulimit ? _T("1") : _T("0");
	tmps += m_pConfig->m_up     ? _T("1") : _T("0");
	tmps += m_pConfig->m_flat   ? _T("1") : _T("0");
	tmps += m_pConfig->m_dlimit ? _T("1") : _T("0");
	tmps += m_pConfig->m_down   ? _T("1") : _T("0");
	WritePrivateProfileString(_T("2022"), "updn", tmps, path);
}

void CRTSWnd::SetUp()
{
	Csetup*	setup;

	setup = new Csetup(NULL, m_pConfig);
	if (setup->DoModal() == IDOK)
	{
		setup->GetConfig(m_pConfig);
		SendTR();
		SaveConfig();
//		setColumns();
	}
	delete setup;
}

void CRTSWnd::SendTR(BOOL bClear)
{
	char	wb[L_userTH+L_inR];
	struct	_userTH* userth;	
	struct	_inR*	inR;
	CString	strTemp;

	FillMemory(&wb, sizeof(wb), ' ');
	userth = (struct _userTH *)wb;
	CopyMemory(userth->trc, trN, sizeof(userth->trc));
	userth->key  = 0;
	userth->stat = 0;

	inR = (struct _inR *)&wb[L_userTH];
	if (bClear)
		inR->func[0] = 'C';
	else
	{
		m_bOn = FALSE;
		inR->func[0] = 'S';
		strTemp.Format("%03d", m_pConfig->m_vol);
		CopyMemory(inR->cvol, strTemp.operator LPCTSTR(), sizeof(inR->cvol));
		strTemp.Format("%03d", m_pConfig->m_amt);
		CopyMemory(inR->camt, strTemp.operator LPCTSTR(), sizeof(inR->camt));
		inR->ador[0] = m_pConfig->m_and     ? 'a' : 'o';
		inR->prcf[0] = m_pConfig->m_price   ? '1' : '0';
		strTemp.Format("%07d", m_pConfig->m_sprc);
		CopyMemory(inR->sprc, strTemp.operator LPCTSTR(), sizeof(inR->sprc));
		strTemp.Format("%07d", m_pConfig->m_eprc);
		CopyMemory(inR->eprc, strTemp.operator LPCTSTR(), sizeof(inR->eprc));
		inR->updn[0] = m_pConfig->m_ulimit  ? '1' : '0';
		inR->updn[1] = m_pConfig->m_up      ? '1' : '0';
		inR->updn[2] = m_pConfig->m_flat    ? '1' : '0';
		inR->updn[3] = m_pConfig->m_dlimit  ? '1' : '0';
		inR->updn[4] = m_pConfig->m_down    ? '1' : '0';
		inR->tree[0] = '0';
		inR->tree[1] = '0';
	}

	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_inR), (LPARAM)(LPCTSTR)wb);
}

long CRTSWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		m_csSync.Lock();
		Dispatch(HIBYTE(LOWORD(wParam)), (char *)lParam, HIWORD(wParam));
		m_csSync.Unlock();
		break;

	case DLL_NOTICE:
		m_csSync.Lock();
		Alert((char*)lParam);
		m_csSync.Lock();
		break;
	}

	return 0;
}

void CRTSWnd::Dispatch(int nKey, char* pDataB, int nDataL)
{
	struct	_outR*	outR;

	if (nDataL < L_outR)
		return;

	outR = (struct _outR *)pDataB;
	m_bOn = (outR->retc[0] == '0') ? TRUE : FALSE;

	if (m_bOn)
		m_pGrid->EmptyAll();
}

void CRTSWnd::Alert(CString strData)
{
	if (!m_bOn)
		return;

	int	nPos;
	CString strRTS(_T(""));
	CString strValue(_T(""));
	struct	_rtsR*	rtsR;

	while (true)
	{
		if (strData.GetLength() < L_rtsR)
			break;

		rtsR = (struct _rtsR *)(char *)strData.operator LPCTSTR();
		strRTS  = CString(rtsR->flag, sizeof(rtsR->flag));
		if (strRTS.CompareNoCase(flagRTS))
			return;

		nPos  = atoi(CString(rtsR->datl, sizeof(rtsR->datl)));
		strValue = strData.Mid(L_rtsR, nPos);
		if ((int)(L_rtsR + nPos) < strData.GetLength())
			strData = strData.Mid(L_rtsR + nPos);
		else
			strData.Empty();

		nPos = strValue.Find('\t');
		if (nPos == -1)
			continue;

		strRTS = strValue.Left(nPos++);
		if (strRTS.CompareNoCase(keyRTS))
			continue;

		strValue = strValue.Mid(nPos);
		int nCol = m_pGrid->GetNumberCols();

		CStringArray arrRTS;

		for (int i = 0; i < nCol; i++)
		{
			if (strValue.IsEmpty())
				break;

			nPos = strValue.Find('\t');
			if (nPos == -1)
			{
				strRTS = strValue;
				strValue.Empty();
			}
			else
			{
				strRTS  = strValue.Left(nPos++);
				strValue = strValue.Mid(nPos);
			}

			arrRTS.Add(strRTS);
		}

		int nCount = arrRTS.GetSize();
		if (nCount == 9)
		{
			if (IsValidCode(arrRTS.GetAt(0)))
			{
				int nPrevRow = m_nRow;
				m_nRow++;
				
				if (m_nRow >= m_pGrid->GetNumberRows())
					m_nRow = 0;

				for (int i = 0; i < nCount; i++)
				{
					strRTS = arrRTS.GetAt(i);

					if (i == 4 && !strRTS.IsEmpty())
					{
						CString strTemp;
						char szUpDn = strRTS.GetAt(0);
						strRTS.Delete(0);
						FormatThousand(strRTS);

						if (szUpDn == _T('1') || szUpDn == _T('2'))
						{
							strTemp.Format("%s%7s", szUpDn == _T('1') ? _T("↑") : _T("▲"), strRTS);
							strRTS = strTemp;

							m_pGrid->QuickSetTextColor(3, m_nRow, GetAxColor(CLR_UP));
							m_pGrid->QuickSetTextColor(4, m_nRow, GetAxColor(CLR_UP));
							m_pGrid->QuickSetTextColor(5, m_nRow, GetAxColor(CLR_UP));
							m_pGrid->QuickSetTextColor(7, m_nRow, GetAxColor(CLR_UP));
						}
						else if (szUpDn == _T('4') || szUpDn == _T('5'))
						{
							strTemp.Format("%s%7s", szUpDn == _T('1') ? _T("↓") : _T("▼"), strRTS);
							strRTS = strTemp;
							m_pGrid->QuickSetTextColor(3, m_nRow, GetAxColor(CLR_DN));
							m_pGrid->QuickSetTextColor(4, m_nRow, GetAxColor(CLR_DN));
							m_pGrid->QuickSetTextColor(5, m_nRow, GetAxColor(CLR_DN));
							m_pGrid->QuickSetTextColor(7, m_nRow, GetAxColor(CLR_DN));
						}
					}
					else if (i == 3 || i == 5 || i == 7 && !strRTS.IsEmpty())
					{
						char ch = strRTS.GetAt(0);
						
//						if (ch == _T('-'))
//							m_pGrid->QuickSetTextColor(i, m_nRow, GetAxColor(CLR_DN));
//						else
//							m_pGrid->QuickSetTextColor(i, m_nRow, GetAxColor(CLR_UP));
						
						strRTS.Delete(0);
						
						CString strDec;
						
						if (i == 5)
						{
							int nPos = strRTS.Find(_T('.'));
							
							if (nPos == -1)
								return;
							
							strDec = strRTS.Right(strRTS.GetLength() - nPos);
							strRTS = strRTS.Left(nPos);
						}
						
						FormatThousand(strRTS);
						
						if (i == 5)
						{
							strRTS += strDec;
							strRTS.Insert(0, ch);
						}
					}
					else if (i == 6 && !strRTS.IsEmpty())
					{
						FormatThousand(strRTS);
					}


					m_pGrid->QuickSetText(i, m_nRow, strRTS);
					m_pGrid->ClearSelections();
			
					if (nPrevRow > -1)
					{
						m_pGrid->QuickSetBackColor(i, nPrevRow, GetAxColor(CLR_BACK));
						m_pGrid->RedrawRow(nPrevRow);
					}

					m_pGrid->QuickSetBackColor(i, m_nRow, GetAxColor(CLR_FOCUS));
					m_pGrid->RedrawRow(m_nRow);
				}

				if (m_nRow == 0)
					((CUGVScroll*)m_pGrid->GetScrollBarCtrl(SB_VERT))->VScroll(SB_TOP, m_nRow);
				else
				{
					if (m_pGrid->GetBottomRow() <= m_nRow)
						((CUGVScroll*)m_pGrid->GetScrollBarCtrl(SB_VERT))->VScroll(SB_THUMBPOSITION, 
										m_nRow - ((m_pGrid->GetBottomRow() - m_pGrid->GetTopRow()) / 2));
				}
			}
		}
	}
}

BOOL CRTSWnd::IsValidCode(CString strCode)
{
	return ((CMainWnd*)m_pMainWnd)->IsValidCode(strCode);
}

void CRTSWnd::FormatThousand(CString& strVal)
{
	int nThound = 0;
	
	for (int i = 0; i < strVal.GetLength(); i++)
	{
		if ((i - nThound) % 3 == 0 && i != (3 * nThound) + nThound)
		{
			strVal.Insert(strVal.GetLength() - i, _T(","));
			nThound++;
		}
	}
}