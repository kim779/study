// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB201600.h"
#include "MainWnd.h"
#include "../../h/memDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF	signtxtColor[] = {0x0000ff, 0xff0000};


/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pParent)
{
	m_pMainWindow = pParent;
	m_fontsize = 9;
	
	m_iRows = 0;
	m_iCols = 0;
	m_iHoga = 0;
	m_bIsOpen = false;

	m_bThreadRTM = false;
	m_pThreadRTM = nullptr;
	m_hEventRTM = nullptr;
	m_bRTMReady = FALSE;

	m_bDestroy = false;

	m_sInfo = nullptr;

}



CMainWnd::~CMainWnd()
{
	m_bkBrush.DeleteObject();
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {E403C6C9-E765-44FB-80E9-6889251EE8B1}
static const IID IID_IMainWnd =
{ 0xe403c6c9, 0xe765, 0x44fb, { 0x80, 0xe9, 0x68, 0x89, 0x25, 0x1e, 0xe8, 0xb1 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers
BOOL CMainWnd::create(CWnd* parent) 
{
	if (!CWnd::Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0, 0, MAP_WIDTH, MAP_HEIGHT), parent, 100))
		return FALSE;
	
//	OnInit();
	CreateChild();

	return TRUE;
}

void CMainWnd::CreateChild()
{
	const COLORREF color = GetColor(64);
	constexpr DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	CRect clientRC, rc, btnRc;

	if(m_pMatrixWnd == nullptr)
	{
		m_pMatrixWnd = std::make_unique<CMatrixWnd>(this, m_pMainWindow);
		m_pMatrixWnd->Create(nullptr, nullptr, dwStyle, CRect(0, 0, 160, 100), this, IDC_MAXRIXMAP);
	}
	
	m_pMatrixWnd->ShowWindow(SW_HIDE);	
	GetClientRect(&clientRC);
    //컨트롤들의 위치시작점 조정
	rc.top = 0;
	rc.bottom = offsetGH;
	rc.left = 10;//0->10
	rc.right = clientRC.right  + gap2;
	
	if(m_pToolWnd == nullptr)
	{
		m_pToolWnd = std::make_unique<CToolWnd>(this, m_pMainWindow);
		m_pToolWnd->Create(nullptr, nullptr, dwStyle, rc, this, IDC_TOOL);
	}

	m_pToolWnd->m_btnMatrix->GetWindowRect(btnRc);

	ScreenToClient(btnRc);
	
	rc.top = btnRc.bottom;
	rc.bottom = rc.top + 100;
	rc.left = btnRc.right;
	rc.right = rc.left + 100;


	rc.top = offsetGH;
	rc.bottom = clientRC.bottom + offsetGH;
	rc.left = 10;//0->10
	rc.right = clientRC.right;
	
	if(m_pViewWnd == nullptr)
	{
		m_pViewWnd = std::make_unique<CViewWnd>(this, m_pMainWindow);
		m_pViewWnd->Create(nullptr, nullptr, dwStyle, rc, this, IDC_VIEW);	
	}


	//파일에서 정보 불러들이기
	//loadScreenInfo(0);

	m_sInfo = std::make_unique<SISEWND_INFO>();

	m_pToolWnd->m_nBtnIndex = 0;
	m_nBtnIndex = 0;
	m_pToolWnd->ClickButton(m_pToolWnd->m_nBtnIndex);
	SendMessage(WM_MANAGE,MK_LOADSNAP,m_nBtnIndex);

	LoadRange();

}

void CMainWnd::loadScreenInfo(int index)
{
	CString strPath(_T("")),rkey,ckey,hkey,bkey;
	CString row(_T("")), col(_T("")), hoga(_T(""));
	int iType;

	strPath.Format("%s\\%s\\%s\\saveInfo.ini", m_root, USRDIR, m_name);

	iType = GetPrivateProfileInt(szMatrix, "TYPE", 0, strPath);

	if(iType == 0)
	{
		rkey = "row";
		ckey = "col";
		hkey = "hoga";

		m_iCols = GetPrivateProfileInt(szMatrix, ckey, 1, strPath);
		m_iRows = GetPrivateProfileInt(szMatrix, rkey, 2, strPath);
		m_iHoga = GetPrivateProfileInt(szMatrix, hkey, 1, strPath);

		rkey.Format("row%02d",0);
		ckey.Format("col%02d",0);
		hkey.Format("hoga%02d",0);
		
		//::DeleteFile(strPath);
		
		col.Format("%d", m_iCols);
		row.Format("%d", m_iRows);
		hoga.Format("%d", m_iHoga);
		
		WritePrivateProfileString(szMatrix, rkey, (LPCTSTR)row, (LPCTSTR)strPath);
		WritePrivateProfileString(szMatrix, ckey, (LPCTSTR)col, (LPCTSTR)strPath);
		WritePrivateProfileString(szMatrix, hkey, (LPCTSTR)hoga, (LPCTSTR)strPath);
		WritePrivateProfileString(szMatrix, "TYPE", "1", (LPCTSTR)strPath);
	}

	rkey.Format("row%02d",index);
	ckey.Format("col%02d",index);
	hkey.Format("hoga%02d",index);
	bkey.Format("combo%02d",index);
	
	m_iCols = GetPrivateProfileInt(szMatrix, ckey, 1, strPath);
	m_iRows = GetPrivateProfileInt(szMatrix, rkey, 2, strPath);

	const int iSelect = GetPrivateProfileInt(szMatrix, "Select", 0, strPath);

	if(iSelect == 1)
	{
		m_iHoga = GetPrivateProfileInt(szMatrix, hkey, 1, strPath);
		m_iCombo = GetPrivateProfileInt(szMatrix, bkey, 0, strPath);
	}
	else
	{
		m_iHoga = GetPrivateProfileInt(szMatrix, "hoga", 1, strPath);
		m_iCombo = GetPrivateProfileInt(szMatrix, "combo", 0, strPath);
	}

	if(m_iRows == 0)
		m_iRows = 1;

	if(m_iCols == 0)
		m_iCols = 1;
	
	if(m_iHoga != 10 && m_iHoga != 5)
		m_iHoga = HOGATYPE5;

	m_pToolWnd->initRadioBtn(m_iHoga);
	m_pToolWnd->initCheckBox(iSelect);

	SendMessage(WM_MANAGE, MK_INITROWCOL);

	//m_pToolWnd->SetComboIndex(m_iCombo);
	SetTimer(2000,600,nullptr);
//	SetTimer(1050, 50, nullptr);
}

void CMainWnd::saveScreenInfo(int index)
{
	CString strPath(_T("")),rkey,ckey,hkey,bkey;
	CString row(_T("")), col(_T("")), hoga(_T("")), combo(_T(""));
	
	strPath.Format("%s\\%s\\%s\\saveInfo.ini", m_root, USRDIR, m_name);
	 
	rkey.Format("row%02d",index);
	ckey.Format("col%02d",index);
	hkey.Format("hoga%02d",index);
	bkey.Format("combo%02d",index);
	
	//::DeleteFile(strPath);
	
	col.Format("%d", m_iCols);
	row.Format("%d", m_iRows);
	hoga.Format("%d", m_iHoga);
	combo.Format("%d", m_iCombo);
	
	WritePrivateProfileString(szMatrix, rkey, (LPCTSTR)row, (LPCTSTR)strPath);
	WritePrivateProfileString(szMatrix, ckey, (LPCTSTR)col, (LPCTSTR)strPath);

	const int iSelect = GetPrivateProfileInt(szMatrix, "Select", 0, strPath);
	
	if(iSelect == 1)
	{
		WritePrivateProfileString(szMatrix, hkey, (LPCTSTR)hoga, (LPCTSTR)strPath);
		WritePrivateProfileString(szMatrix, bkey, (LPCTSTR)combo, (LPCTSTR)strPath);
	}
	else
	{
		WritePrivateProfileString(szMatrix, "hoga", (LPCTSTR)hoga, (LPCTSTR)strPath);
		WritePrivateProfileString(szMatrix, "combo", (LPCTSTR)combo, (LPCTSTR)strPath);
	}
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1050)
	{
		KillTimer(1050);
		SendMessage(WM_MANAGE, MK_INITROWCOL);	
	}
	else if(nIDEvent == 2000)
	{
		KillTimer(2000);
		m_pToolWnd->SetComboIndex(m_iCombo);
	}

	CWnd::OnTimer(nIDEvent);
}


BOOL CMainWnd::Create()
{
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	m_user = Variant(userCC, "");
	m_font = getAxFont(_T("굴림체"), 9);
	
	m_strPal = GetPalette();
	
	Variant(titleCC, "복수종목 현재가2");
	return create(m_pMainWindow);
}



LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	const LONG ret = 0;

	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETPAL:
		ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
		break;
	case DLL_OUB:
 		m_sync.Lock();
 		m_pViewWnd->RecvData(HIBYTE(LOWORD(wParam)), (char*)lParam);
  		m_sync.Unlock();
		break;
	case DLL_ALERT:
		break;
	case DLL_ALERTx:
		{
			struct _alertR* alertR;
			alertR = (struct _alertR*)lParam;

			DWORD* data;
			data = (DWORD*)alertR->ptr[0];

			//2016.02.18 KSJ alertR->code가 7자리일 경우에는 한자리 빼준다.
			CString strCode;
			strCode = alertR->code;
			strCode.TrimLeft(); strCode.TrimRight();
			if(strCode.GetLength() == 7)	strCode.Delete(0);
			//

			if(data[701])
			{
				for(int ii=0 ; ii<m_iCols ; ii++)
				{
					for(int jj=0 ; jj<m_iRows ; jj++)
					{
						CString sMktype, scode;
						sMktype = m_pViewWnd->m_pSiseWnd[ii][jj]->GetMarket();

						if (sMktype == _T("통합"))
							scode = "M.A" + m_pViewWnd->m_pSiseWnd[ii][jj]->GetCode();
						else if(sMktype == _T("NXT"))
							scode = "N.A" + m_pViewWnd->m_pSiseWnd[ii][jj]->GetCode();
						else if (sMktype == _T("KRX"))
							scode =  m_pViewWnd->m_pSiseWnd[ii][jj]->GetCode();

						if(!strCode.Compare(scode))
						{
							m_pViewWnd->m_pSiseWnd[ii][jj]->m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
						}
					}
				}
			}
		}
		break;
	case DLL_NOTICE:
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		parsingDomino((char*)lParam);
		break;
	case DLL_GUIDE:
		break;
	}
	return ret;
}
//데이타를 수신하여 파싱한 다음 바로 화면을 그리는 구조
void CMainWnd::parsingDomino(CString datB)
{
	int hoga{};
	CString codeCount = "5";
	CString* codes{};
	const char chTab = ':';
	const char chBar = '|';

	// codeCount = parseX(datB, chTab);
// 
// 	while(!datB.IsEmpty())
// 	{
// 		entry = parseX(datB, chBar);
// 	}
// 	
// 	if(codeCount == "no" || codeCount == "CODE_1")
// 	{
// 		return;
// 	}
	//m_sInfo->rows = -1;


	CString* tmp = GetSplit(datB,'\t');

	CString mapNo = datB.Mid(0,6);

	if(mapNo != "IB2022")
	{
		return;
	}

	codeCount = tmp[1];
	
	codes = GetSplit(tmp[2],'|');

	const int nCount = atoi(codeCount);

	const int nMonitorType = GetMonitorStatus();

	int maxCols = 0;

	if(nMonitorType == DUAL_MONITOR)
	{
		maxCols = 8;
		//m_sInfo->mode = DUAL_MONITOR;
	}
	else
	{
		maxCols = 4;
		//m_sInfo->mode = SINGLE_MONITOR;
	}

	int nRows = 0;

	if(nCount > maxCols)
	{
		nRows = maxCols;
	}
	else
	{
		nRows = nCount;
	}

	int nCols = nCount / maxCols;


	if(nCount % maxCols > 0)
	{
		nCols++;
	}

	m_iRows = nRows;
	m_iCols = nCols;
	
	OperSize(nRows, nCols);
	hoga = m_pToolWnd->GetRadioCheck();
	
	m_pViewWnd->GoanSimSettingSiseWnd(nRows, nCols, hoga,nCount % maxCols,codes,nCount);

// 	m_sInfo->rows = nRows;
// 	m_sInfo->cols = nCols;
// 	m_sInfo->count = nCount;
// 	m_sInfo->maxcols = maxCols;
// 	m_sInfo->codes = codes;
}

int CMainWnd::GetMonitorStatus()
{
	CRect appRect;
    //Parent 체크하는 부분 추가 필요
	
	CWnd* parent = m_pMainWindow->GetParent();
	
	if(parent == nullptr)
		return SINGLE_MONITOR;

	CWnd* pparent = parent->GetParent();

	if(pparent == nullptr)
		return SINGLE_MONITOR;

	pparent->GetWindowRect(&appRect);

	if(MAP_WIDTH * 4 < appRect.Width())
	{
		return DUAL_MONITOR;
	}
	else
	{
		return SINGLE_MONITOR;
	}

}

CString* CMainWnd::GetSplit(CString sText, TCHAR sSplit) 
{
	 //m_printData.clear();
	 CString sResult;
	 int tokenSize = 0;

	 if(sText != "")
	 {
		 // afxwin.h을 이용하여 AfxExtractSubString함수를 이용한다.
		 while(AfxExtractSubString(sResult, sText, tokenSize, sSplit)) {
			if(sResult.GetLength() > 0)
			{
				m_token[tokenSize] = sResult;
			}

			tokenSize += 1;
		 }
	 }

	 return m_token;
}


CString CMainWnd::parseX(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}




LONG CMainWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	const LONG ret = 0;
	CRect rc, btnRc;
	int hoga{};

	if (IS_NULL(m_pViewWnd))
		return 0;

	switch (LOWORD(wParam))
	{
	case MK_SETPAL:
		m_strPal = GetPalette();
		m_clrPallete = (COLORREF)lParam;

		Invalidate(FALSE);

		break;
	case MK_SHOWMATRIX:
		m_pMatrixWnd->GetClientRect(rc);
		m_pToolWnd->m_btnMatrix->GetWindowRect(btnRc);
		ScreenToClient(btnRc);
		
		rc.top = btnRc.bottom;
		rc.bottom = rc.top + rc.bottom;
		rc.left = btnRc.right - btnRc.left / 2;
		rc.right = rc.left + rc.right;

		m_pMatrixWnd->MoveWindow(rc, TRUE); 
		m_pMatrixWnd->ShowWindow(SW_SHOW);
		break;

	case MK_HIDEMATRIX:
		m_pMatrixWnd->ShowWindow(SW_HIDE);
		break;

	case MK_SETROWCOL:
		OperSize(m_pMatrixWnd->GetRows(), m_pMatrixWnd->GetCols());
		hoga = m_pToolWnd->GetRadioCheck();

		//안보이는 화면들은 000000으로 종목처리
		m_pViewWnd->SettingSiseWnd(m_iRows, m_iCols, hoga);	
		m_pMatrixWnd->initMatrix();
		m_pMatrixWnd->SetCols(0);
		m_pMatrixWnd->SetRows(0);
		m_pViewWnd->CallSnapShot();
		break;
	case MK_DOMINOSETROWCOL:			// 관심종목에서 도미노 데이터 넘길때 동작
		OperSize(m_iRows, m_iCols);
		hoga = m_pToolWnd->GetRadioCheck();
		//안보이는 화면들은 000000으로 종목처리
		m_pViewWnd->SettingSiseWnd(m_iRows, m_iCols, hoga);	
		break;
	case MK_INITROWCOL:
		OperSize(m_iRows, m_iCols);
		m_pViewWnd->initSettingSiseWnd(m_iRows, m_iCols, m_iHoga);	
		break;
	case MK_OPERSIZE:
		OperSize(CSize(LOWORD(lParam), HIWORD(lParam)));
		break;
	case MK_SHOW10HOGA:
		m_iHoga = HOGATYPE10;
		m_pViewWnd->changeSiseWnd(m_iRows, m_iCols, HOGATYPE10);
		break;
	case MK_SHOW5HOGA:
		m_iHoga = HOGATYPE5;
		m_pViewWnd->changeSiseWnd(m_iRows, m_iCols, HOGATYPE5);
		break;
	case MK_CHANGETAB:
		m_iCombo = m_pToolWnd->GetComboIndex();
		m_pViewWnd->chanegeTab(m_pToolWnd->GetComboIndex());
		break;
	case MK_LOADSNAP:
		m_nBtnIndex = (int)lParam;
		loadScreenInfo(m_nBtnIndex);
		m_pViewWnd->loadCodeInfo(m_nBtnIndex);
		break;
	case MK_SNAPSHOT:
		saveScreenInfo((int)lParam);
		m_pViewWnd->saveCodeInfo((int)lParam);
		break;
	case MK_MARKETSNAPSHOT:
		saveScreenInfo((int)lParam);
		m_pViewWnd->saveMarketInfo((int)lParam);
		break;
	case MK_RANGEPERCENT:
		{
			CString str = (char*)lParam;
			CString sHigh = str.Mid(0,2);
			CString sLow = str.Mid(3,2);

			for(int ii=0 ; ii<m_iCols ; ii++)
			{
				for(int jj=0 ; jj<m_iRows ; jj++)
				{
					m_pViewWnd->m_pSiseWnd[ii][jj]->SetRangePercent(atoi(sHigh),atoi(sLow));
				}
			}
		}
		break;
	case MK_ALLPERCENT:
		{
			const int nPercent = (int)lParam;

			if(!nPercent)
			{
				for(int ii=0 ; ii<m_iCols ; ii++)
				{
					for(int jj=0 ; jj<m_iRows ; jj++)
					{
						m_pViewWnd->m_pSiseWnd[ii][jj]->SetRangePercent(0,0);
						m_pViewWnd->m_pSiseWnd[ii][jj]->SetEnablePrice(true);
					}
				}
			}
			else
			{
				for(int ii=0 ; ii<m_iCols ; ii++)
				{
					for(int jj=0 ; jj<m_iRows ; jj++)
					{
						m_pViewWnd->m_pSiseWnd[ii][jj]->SetEnablePrice(false);
					}
				}
			}
		}
		break;
	}
	return ret;
}

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
/*	bool	toggle;
	CString	string;
	
	switch (LOWORD(wParam))
	{
	case IDC_MAXRIX:
		CreateChild();
		break;
	}
*/	
	return CWnd::OnCommand(wParam, lParam);
}

void CMainWnd::SetPallette()
{
//	COLORREF	color = GetColor(64);
	m_strPal = GetPalette();
	
	Invalidate();
}

BOOL CMainWnd::OnInit()
{
	CRect	rect, clientRC; 
	GetClientRect(clientRC);
	
	CString	imgPath; 
	imgPath.Format("%s/%s/", m_root, IMAGEDIR);

	Variant(titleCC, _T("복수종목 현재가2"));
	SetOwner(m_pMainWindow);
	
	
	m_bkBrush.DeleteObject();
	m_bkBrush.CreateSolidBrush(GetColor(181));
	
	rect.SetRect(0, 0, 0, 0);
	
	rect.top = clientRC.top + gap2;
	rect.left = clientRC.left + gap3;
	rect.right = rect.left + 20;
	rect.bottom = rect.top + 20;
	
	m_bitmap_nm = getAxBitmap(imgPath + "GTI_MATRIX.bmp")->operator HBITMAP();
	m_bitmap_dn = getAxBitmap(imgPath + "GTI_MATRIX_dn.bmp")->operator HBITMAP();
	m_bitmap_hv = getAxBitmap(imgPath + "GTI_MATRIX_en.bmp")->operator HBITMAP();
	
	m_btnMatrix.Create("", rect, this, IDC_MAXRIX);
	m_btnMatrix.SetFont(m_font, false);  
	m_btnMatrix.SetImgBitmap(m_bitmap_nm, m_bitmap_dn, m_bitmap_hv);
	

	return TRUE;
}

void CMainWnd::OperSize(int row, int col)
{
	CSize size;
	//화면 사이즈보다 클경우 스크롤을 생성시킨다
	size.cx = row * MAP_WIDTH + 10;//원점의 left가 10이 늘어난만큼 폭을 10 늘여준다.
	size.cy = col * MAP_HEIGHT + offsetGH;
	
	m_pMainWindow->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(size.cx, size.cy));
	
	MoveWindow(0, 0, size.cx + 10, size.cy);


	m_pToolWnd->MoveWindow(10, 0, size.cx, offsetGH);
	
	if (m_pViewWnd)
		m_pViewWnd->MoveWindow(10, offsetGH, size.cx, size.cy);
}

void CMainWnd::OperSize(CSize size)
{
	m_pMainWindow->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, nullptr), MAKELPARAM(size.cx, size.cy));

	MoveWindow(0, 0, size.cx, size.cy);
}

void CMainWnd::SetIsOpen(bool data)
{
	m_bIsOpen = data;
}	
			
BOOL CMainWnd::GetIsOpen()
{
	return m_bIsOpen;
}


void CMainWnd::SetParam(struct	_param* param, CWnd *parent)
{
	m_pMainWindow = parent;
	m_key = param->key;
	m_strParamName = CString(param->name, param->name.GetLength());
	m_font = getAxFont(_T("굴림체"), 9);
	m_clrBgColor = GetColor(C_BACKIDX);
	
	m_clrColorIdx[0] = C_LBLBACKIDX;
	m_clrColorIdx[1] = C_MEDOBACKIDX;
	m_clrColorIdx[2] = C_MESUBACKIDX;
	m_clrColorIdx[3] = C_LBLBACKIDX2;
	m_clrColorIdx[4] = C_LINEIDX;
	m_clrColorIdx[5] = C_DATAFGIDX;
	m_clrColorIdx[6] = C_LBLFGIDX;
	
	m_clrColor[0] = GetColor(C_LBLBACKIDX);
	m_clrColor[1] = GetColor(C_MEDOBACKIDX);
	m_clrColor[2] = GetColor(C_MESUBACKIDX);
	m_clrColor[3] = GetColor(C_LBLBACKIDX2);
	m_clrLineColor = GetColor(C_LINEIDX);
	
	m_clrDataFgColor = GetColor(C_DATAFGIDX);
	m_clrLblFgColor = GetColor(C_LBLFGIDX);
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pMainWindow->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if((long)dta > 1)
			retvalue = CString(dta);

	return retvalue;
}

void CMainWnd::init()
{
	Variant(titleCC, "복수종목 현재가2");
	m_home = Variant(homeCC);
	m_user = Variant(nameCC);
	m_id = Variant(userCC);
	m_bkBrush.DeleteObject();
	m_bkBrush.CreateSolidBrush(GetColor(64));
	
	CreateChild();
}

LRESULT CMainWnd::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	
	return m_pMainWindow->SendMessage(WM_USER, wParam, lPAram);
}

CBitmap* CMainWnd::getAxBitmap(CString path)
{
	return (CBitmap*)m_pMainWindow->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

CBrush* CMainWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pMainWindow->SendMessage(WM_USER, getBRUSH, (long)clr);
}


CFont* CMainWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)SendAxisMessage( getFONT, (long)&fontR);
}

int	CMainWnd::GetHoga()
{
	return m_iHoga;
}

COLORREF CMainWnd::GetColor(int color)
{
	if (color & 0x02000000)
		return color;

	return SendAxisMessage( MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
/*	
	CDC	backdc;
	backdc.CreateCompatibleDC(&dc); 
	CBitmap bitmap,  *pOldBitmap;
	CRect rectClient;
	GetClientRect(rectClient);
	
	bitmap.CreateCompatibleBitmap(&dc, rectClient.right, rectClient.bottom);
	pOldBitmap = (CBitmap*)backdc.SelectObject(&bitmap);
	backdc.SetBkMode(OPAQUE);
	
	backdc.FillSolidRect( rectClient, m_clrBgColor); 
	CFont* pOldFont = backdc.SelectObject(m_font);	
	
	Draw(&backdc, rectClient);	
	dc.BitBlt( rectClient.left , rectClient.top , rectClient.right , rectClient.bottom , &backdc, 0,0, SRCCOPY );
	bitmap.DeleteObject(); 
	backdc.SelectObject(pOldBitmap);
	backdc.SelectObject(pOldFont); 
	DeleteObject(pOldFont);
	DeleteObject(pOldBitmap);
	ReleaseDC(&backdc);	
*/
	CRect rect;

	GetClientRect(&rect);

	rect.right = 10;
	
	CBrush brush;
	
	m_clrBgColor = GetColor(64);

	brush.CreateSolidBrush(m_clrBgColor);
	
	CBrush* pOldBrush = dc.SelectObject(&brush);
	dc.FillRect(rect,&brush);
	dc.SetBkColor(m_clrBgColor);

	DeleteObject(&pOldBrush);
	brush.DeleteObject();
// 	int nMonitorType = GetMonitorStatus();
// 
// 	if(nMonitorType != m_sInfo->mode)
// 	{
// 		m_sInfo->mode = nMonitorType;
// 
// 		if(nMonitorType == DUAL_MONITOR)
// 			m_sInfo->maxcols = 8;
// 		else
// 			m_sInfo->maxcols = 4;
// 
// 		m_sInfo->rows = 0;
// 
// 		if(m_sInfo->count > m_sInfo->maxcols)
// 		{
// 			m_sInfo->rows = m_sInfo->maxcols;
// 		}
// 		else
// 		{
// 			m_sInfo->rows = m_sInfo->count;
// 		}
// 
// 		m_sInfo->cols = m_sInfo->count / m_sInfo->maxcols;
// 
// 
// 		if(m_sInfo->count % m_sInfo->maxcols > 0)
// 		{
// 			m_sInfo->cols++;
// 		}
// 
// 		OperSize(m_sInfo->rows, m_sInfo->cols);
// 
// 		m_pViewWnd->GoanSimSettingSiseWnd(m_sInfo->rows, m_sInfo->cols, m_pToolWnd->GetRadioCheck(),m_sInfo->count % m_sInfo->maxcols,m_sInfo->codes,m_sInfo->count);
// 	}
 }

void CMainWnd::Draw(CDC *pDC, CRect rcClient)
{
	CRect rect,rectb;
	
	GetClientRect(&rect);

	//background
	pDC->FillSolidRect(&rect,GetColor(181));//64
	rectb = rect;

	//upper panel
	rect.bottom = offsetGH + gap2;
	rect.left = rect.left + gap1;
	rect.top = rect.top + gap1;
	rect.right = rect.right - gap1;

	pDC->FillSolidRect(&rect,GetColor(66));
	DrawRoundBitmap(pDC, rect, ROUND_CONTENTS);
}

void CMainWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMainWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != nullptr)
	{
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}

	DeleteObject(BitMap);
	memDC.DeleteDC();
}


CString CMainWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	CString strPath = Variant(homeCC, "");
	
	file.Format("%s\\%s\\%s", strPath, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

int	CMainWnd::GetRows()
{
	return m_iRows;
}


int	CMainWnd::GetCols()
{
	return m_iCols;
}


void CMainWnd::SetRows(int num)
{
	m_iRows = num;
}


void CMainWnd::SetCols(int num)
{
	m_iCols = num;
}


void CMainWnd::Procedure(CString strProc, int mapkey)
{
	m_pMainWindow->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(GetMajor(), mapkey)), (LPARAM)(LPCTSTR)strProc);
}

int CMainWnd::GetMajor()
{
	return m_pMainWindow->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0L);
}

void CMainWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(LoadCursor(0,IDC_HAND));

	CWnd::OnMouseMove(nFlags, point);
}

void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect, matrixRc;
	
	GetClientRect(rect);
	m_pMatrixWnd->GetClientRect(matrixRc);
	
	//AfxGetApp()->GetMainWnd()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	this->GetParent()->GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CWnd::OnLButtonDown(nFlags, point);
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	//saveScreenInfo(m_nBtnIndex);
	m_bDestroy = true;
	::CloseHandle(m_hEventRTM);
}



LRESULT CMainWnd::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnNcHitTest(point);
}

void CMainWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnClose();
}

void CMainWnd::LoadRange()
{
	m_pToolWnd->loadrange();

	if (m_pViewWnd)
	for(int ii=0 ; ii<m_iCols ; ii++)
	{
		for(int jj=0 ; jj<m_iRows ; jj++)
		{
			
			m_pViewWnd->m_pSiseWnd[ii][jj]->LoadPriceRange();
		}
	}
}
