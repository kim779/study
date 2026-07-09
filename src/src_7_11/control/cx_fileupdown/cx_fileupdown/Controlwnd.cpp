// Controlwnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "cx_fileupdown.h"
#include "Controlwnd.h"
#include "UserSaveDlg.h"
#include "Downdlg.h"
#include "../../../h/axisfire.h"
// CControlwnd
#include "seed_kisa.tab"
IMPLEMENT_DYNAMIC(CControlwnd, CWnd)


#define ENCBYTE 20000
#define MAXBYTE 40000

#define TM_ARRUPLOAD 9997
#define TM_UPLOAD 9999
#define TM_DOWNLOAD 9998
CControlwnd::CControlwnd()
{

	EnableAutomation();
	m_pBitmap[MsNo] = NULL;
	m_pBitmap[MsOver] = NULL;
	m_pBitmap[MsDown] = NULL;

	m_bDown = FALSE;
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_MouseState = MsNo;
	m_type = 0;
	m_bSending = false;
	m_pmid = new struct mid_down;
	memset(m_pmid, 0x00, sizeof(struct mid_down));
	m_stropen = "Y";
}

CControlwnd::~CControlwnd()
{
	if(m_pcrypt != NULL)
	{
		delete m_pcrypt;
		m_pcrypt = NULL;
	}
}

void CControlwnd::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	DISP_FUNCTION_ID(CControlwnd, "SetType", dispidSetType, SetType, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetCmPass", dispidSetCmPass, SetCmPass, VT_EMPTY, VTS_BSTR)
	//DISP_FUNCTION_ID(CControlwnd, "UploadFile", dispidUploadFile, UploadFile, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "UploadFile", dispidUploadFile, UploadFile, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	//DISP_FUNCTION_ID(CControlwnd, "DownFile", dispidDownFile, DownFile, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "DownFile", dispidDownFile, DownFile, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "GetPercent", dispidGetPercent, GetPercent, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()

// 참고: IID_IControlwnd에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {5C7A69CB-ACA9-4B83-8787-851D94C43603}
static const IID IID_IControlwnd =
{ 0x5C7A69CB, 0xACA9, 0x4B83, { 0x87, 0x87, 0x85, 0x1D, 0x94, 0xC4, 0x36, 0x3 } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()


// CControlwnd 메시지 처리기입니다.


LRESULT CControlwnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_SETFONT:
		case DLL_SETFONTx:
			{
				CString tmp( (LPCTSTR)lParam );
				int ipoint = HIWORD(wParam);

				_fontR font;
				font.name = (LPSTR)(LPCTSTR)m_Param.fonts;
				font.point = ipoint;
				font.italic = false;
				font.bold = FW_NORMAL;
		
				Invalidate();
			}
			break;
		case DLL_OUB:
		{
			char* pBytes = (char *)lParam;
			struct	_ledgerH * pledger = NULL;
			pledger = (struct _ledgerH*)lParam;
			CString strkey, strresult;
			strkey = CString(pledger->fnam, 30).TrimRight();

			strresult.Format(_T("%s"), pledger->rtit);
			strresult.TrimRight();
			if(strresult.Find("Error") >= 0)
			{
				AfxMessageBox("down load service error");
				return -1;
			}

			if(strkey  == "fileDownload")   
			{
				int ilen = 0;
				CString tmps;
				char* m_pdata = new char[sizeof(struct mod_down) + 1];
				memset(m_pdata, 0x00, sizeof(struct  mod_down) + 1);
				memcpy(m_pdata, (char*)&pBytes[sizeof(struct  _ledgerH)], sizeof(struct mod_down));
				struct  mod_down* pmod = (struct mod_down*)m_pdata;
				
				CString strnext;
				strnext.Format(_T("%c"), pmod->next);
			
				tmps.Format(_T("%.19s"), pmod->len);
				tmps.TrimLeft();
				tmps.TrimRight();
				ilen = atoi(tmps);
				m_itotal += ilen;
				m_cnt++;

				if(strnext == "E")
				{
					m_pReDataArr.RemoveAll();
					m_relenArr.RemoveAll();
					m_bSending = false;
					m_strResult = "-1";
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)(LPCTSTR)m_Param.name);	
					return false;
				}

				if(strnext == "Y")
				{			
					m_decB = new BYTE[ilen];
					memset(m_decB, 0x00, ilen);
					pmod->data[ilen + 1] = NULL;
	
					tmps.Format(_T("%d"), m_pcrypt->base64_decode(m_decB, pmod->data, ilen));
					parr.Add(m_decB);
					lenarr.Add(tmps);				
					/*
					parr.Add(pmod->data);
					tmps.Format(_T("%.19s"), pmod->len);
					tmps.TrimLeft();
					tmps.TrimRight();
					lenarr.Add(tmps);
					*/
				}
				else
				{			
					m_plastdata = new BYTE[ilen + 1];
					memset(m_plastdata, 0x00, ilen + 1);
					pmod->data[ilen + 1] = NULL;
	
					tmps.Format(_T("%d"), m_pcrypt->base64_decode(m_plastdata, pmod->data, ilen));
					parr.Add(m_plastdata);
					lenarr.Add(tmps);			
					/*
					parr.Add(pmod->data);
					tmps.Format(_T("%.19s"), pmod->len);
					tmps.TrimLeft();
					tmps.TrimRight();
					lenarr.Add(tmps);
					*/
				}
		
				m_decB = new BYTE[19];
				memset(m_decB, 0x00 , 19);
				memcpy(m_decB, pmod->totalLen, 19);
				tmps.Format(_T("%.19s"), m_decB);
				tmps.TrimLeft();
				tmps.TrimRight();
			
				double dtotla = atol(tmps);
				m_strResult.Format(_T("%.0f"), ((20000 * atoi(m_strdownCnt))/dtotla) * 100);

				if(atoi(m_strResult) >= 100)
					m_strResult = _T("99");
			
				m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (LPARAM)(LPCTSTR)m_Param.name);

				if(strnext == "Y")
				{
					m_pmid = new struct mid_down;
					FillMemory(m_pmid, sizeof(struct mid_down), ' ');

					memcpy(m_pmid->filepath, (char*)m_strComPath.operator LPCSTR(), m_strComPath.GetLength());
					memcpy(m_pmid->filename, (char*)m_strFilename.operator LPCSTR(), m_strFilename.GetLength());
					memcpy(m_pmid->uuid, (char*)m_struuid.operator LPCSTR(), m_struuid.GetLength());
				
					int icnt = atoi(m_strdownCnt);
					icnt++;
					m_strdownCnt.Format(_T("%d"), icnt);
					memset(m_pmid->cnt, 0, 19);
					memcpy(m_pmid->cnt, (char*)m_strdownCnt.operator LPCSTR(), m_strdownCnt.GetLength());

					SetTimer(TM_DOWNLOAD, 100, NULL);
				}
				else
				{
					m_strResult = "100";
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (LPARAM)(LPCTSTR)m_Param.name);

					CString path,strdata;
					CFile file;
					CString strpath;
					CString  tmpS;
									
					int cnt =0;
					CString tmps;
							
					if (!file.Open(m_strDownPath, CFile::modeWrite|CFile::modeCreate))
						return 1;

					CString sWrite;
					for(int ii = 0 ; ii < parr.GetSize() ; ii++)
					{
						DWORD	pos = (int)file.SeekToEnd();
						file.Seek((long) pos, CFile::begin);
						int nnn = atoi(lenarr.GetAt(ii));
						file.Write(parr.GetAt(ii), atoi(lenarr.GetAt(ii)) );
					}
					file.Close();
					
					m_bSending = false;
					
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)(LPCTSTR)m_Param.name);	
					if(m_stropen == "Y")
						ShellExecute(NULL, _T("open"), m_strDownPath, NULL,NULL, SW_SHOWNORMAL);
				}
			}
			return 0;
		}
		break;
	}
	
	return 0;
}

void CControlwnd::GetledgerHDown(struct _ledgerH* pLedgerH)
{
	CString strtemp;
	strtemp = "FITS-HTS";
	memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

	strtemp = "FITSComFileSO";
	memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

	strtemp = "fileDownload";
	memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
}


void CControlwnd::GetledgerH(struct _ledgerH* pLedgerH)
{
	CString strtemp;
	strtemp = "FITS-HTS";
	memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

	strtemp = "FITSComFileSO";
	memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

	strtemp = "fileUpload";
	memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
}

void CControlwnd::SendTR()
{
	CString tmpS;
	struct	_userTH* userTH;	
	char*	cDataBuffer = new char [2048];
	memset(cDataBuffer, ' ' , 2048);
	
	sprintf(cDataBuffer, "%s\t", m_Param.name);

	userTH = (struct _userTH *)&cDataBuffer[m_Param.name.GetLength() + 1];

	strcpy(userTH->trc, _T("PIBOPFRM"));
	userTH->key = 1;
	userTH->stat = US_KEY;

	struct  _ledgerH ledgerH, *pledgerH;	
	pledgerH = new _ledgerH;
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	FillMemory(pledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH);
	int nn = sizeof(struct  _ledgerH);
	tmpS = CString((char *)&ledgerH, sizeof(struct  _ledgerH));

	CopyMemory(&cDataBuffer[L_userTH + m_Param.name.GetLength() + 1], tmpS, tmpS.GetLength());

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, tmpS.GetLength()), (LPARAM) cDataBuffer);

}

void* CControlwnd::Getparam()
{
	return (void*)&m_Param;
}

void CControlwnd::SetParam(_param *pParam)
{
	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
}

CString CControlwnd::Parser(CString &srcstr, CString substr)
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

int CControlwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_strHome = (char*)m_pWizard ->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
	m_strComPath.Empty();
	
	if (m_pBitmap[MsNo] == NULL)
		m_pBitmap[MsNo] = GetBitmap(m_strHome + _T("\\image\\설정FRAME.BMP"));
	if (m_pBitmap[MsOver] == NULL)
		m_pBitmap[MsOver] = GetBitmap(m_strHome + _T("\\image\\설정FRAME_EN.BMP"));
	if (m_pBitmap[MsDown] == NULL)
		m_pBitmap[MsDown] = GetBitmap(m_strHome + _T("\\image\\설정FRAME_DN.BMP"));
		
	m_pcrypt = new Crypt;
	return 0;
}

CBitmap* CControlwnd::GetBitmap(CString path)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

void CControlwnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (cx > 0 && cy > 0)
		GetClientRect(&m_btnRect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CControlwnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect	WinRC;
	GetClientRect(&WinRC);

	if (m_pBitmap[m_MouseState])
	{
		dc.FillSolidRect(WinRC, RGB(100, 100, 100));
		WinRC.DeflateRect(1, 1, 1, 1);
		dc.FillSolidRect(WinRC, RGB(255, 255, 255));

		if (m_bDown)
			DrawBitmap(&dc, m_btnRect, m_pBitmap[MsDown]);
		else if (m_bHover)
			DrawBitmap(&dc, m_btnRect, m_pBitmap[MsOver]);
		else
			DrawBitmap(&dc, m_btnRect, m_pBitmap[m_MouseState]);
	}
	else
	{
		dc.FillSolidRect(WinRC, GetSysColor(COLOR_WINDOW));
		dc.DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   

		if (m_bDown)
			dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX|DFCS_PUSHED);
		else
			dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}
}

void CControlwnd::DrawBitmap(CDC *pDC, CRect rc, CBitmap *pBitmap)
{
	CString tmpS;
	if (pBitmap)
	{
		CDC		memDC;
		CBitmap		*pSBitmap;

		BITMAP stBitmapInfo;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);

		pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);

		memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
	else
	{
		pDC->FillSolidRect(rc, RGB(196,196,196));
	}
}

void CControlwnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bDown = FALSE;
	InvalidateRect(&m_btnRect);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CControlwnd::Downloadfile(CString strpath)
{
	AfxMessageBox(strpath);
}

bool CControlwnd::sendTR(CString trC, char* pBytes, int nBytes, bool bup)
{
	int	len = L_userTH + nBytes +  m_Param.name.GetLength() + 1 + sizeof(struct  _ledgerH);
	char*	sndB = new char[len];
	ZeroMemory(sndB, sizeof(sndB));
	sprintf(sndB, _T("%s\t"), m_Param.name);
		
	struct _userTH* udat = (struct _userTH*)&sndB[m_Param.name.GetLength() + 1];
	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = m_Param.key;	
	udat->stat = 0;

	struct  _ledgerH ledgerH, *pledgerH;	
	pledgerH = new _ledgerH;
	int nn = sizeof(struct  _ledgerH);
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');

	if(bup)
		GetledgerH(&ledgerH);
	else
		GetledgerHDown(&ledgerH);

	CopyMemory(&sndB[L_userTH + m_Param.name.GetLength() + 1], &ledgerH, sizeof(struct  _ledgerH));
	CopyMemory(&sndB[L_userTH + m_Param.name.GetLength() + 1 + sizeof(struct  _ledgerH)], pBytes, nBytes);

//	m_strlog.Format(_T("^^^ 1 cDataBuffer m_cnt=[%.10s]   [%x]\n"), sndB, m_pWizard);
//OutputDebugString(m_strlog);

	int iret = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes + sizeof(struct  _ledgerH)), (LPARAM)sndB);
	delete [] sndB;

//	m_strlog.Format(_T("^^^ 2 cDataBuffer m_cnt=[%.10s]   [%x]\n"), sndB, m_pWizard);
//OutputDebugString(m_strlog);
	if(iret)
		return true;
	else 
		return false;

	//return iret;
}

void CControlwnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime =1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CControlwnd::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover = true;
	InvalidateRect(&m_btnRect);
	return 0;
}

LRESULT CControlwnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = false;
	m_bHover = false;
	InvalidateRect(&m_btnRect);

	return 0;
}

void CControlwnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bDown = TRUE;
	InvalidateRect(&m_btnRect);
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlwnd::SetType(LPCTSTR type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strtemp;
	strtemp.Format(_T("%s"), type);

	m_type = atoi(strtemp);
	// TODO: 여기에 디스패치 처리기를 추가합니다.
}

void CControlwnd::SetCmPass(LPCTSTR path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strComPath.Format(_T("%s"), path);
	m_strComPath.TrimRight();
}
/*
void CControlwnd::UploadFile(LPCTSTR cmfile, LPCTSTR file)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString filepath, tmps, cmpath;

	if(m_bSending)
		return;

	cmpath.Format(_T("%s"), cmfile);
	cmpath.TrimRight();

	filepath.Format(_T("%s"), file);
	filepath.TrimRight();
	//FileUpload(path);

	tmps.Format(_T("%s\t%s"), cmpath, filepath);
	m_upArr.Add(tmps);
	KillTimer(TM_ARRUPLOAD);
	m_strResult.Empty();
	SetTimer(TM_ARRUPLOAD, 1000, NULL);
}
*/

void CControlwnd::UploadFile(LPCTSTR cmfile, LPCTSTR file, LPCTSTR struuid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString filepath, tmps, cmpath;

	if(m_bSending)
		return;

	cmpath.Format(_T("%s"), cmfile);
	cmpath.TrimRight();

	filepath.Format(_T("%s"), file);
	filepath.TrimRight();
	//FileUpload(path);

	tmps.Format(_T("%s\t%s\t%s"), cmpath, filepath, struuid);
	m_upArr.Add(tmps);
	KillTimer(TM_ARRUPLOAD);
	m_strResult.Empty();
	SetTimer(TM_ARRUPLOAD, 1000, NULL);
}
/*
void CControlwnd::FileUpload(CString cmpath,CString strpath)
{
	CString		file, str, tmpS, strlog;
	CFile		hFile;
	char  *pbuf = NULL;
	char  *psub = NULL;
	int icnt = 0;
	int ileft = 0;
	m_strComPath = cmpath;
	
	//if(m_bSending)
	//	return;

	if(m_strComPath.IsEmpty())
		return;

	if (!hFile.Open(strpath, CFile::modeRead))
	{
		AfxMessageBox(_T("file open fail"));
		return;
	}

	int pos = strpath.ReverseFind(_T(_T('\\')));
	if (pos != -1)
		m_fileN = strpath.Mid(pos+1);

	int fileL = hFile.GetLength();
	pbuf = new char[fileL];

	memset(pbuf, 0, fileL);
	hFile.Read(&pbuf[0], fileL);

	hFile.Close();

	icnt = fileL / ENCBYTE;
	ileft = fileL % ENCBYTE;

	m_lenArr.RemoveAll();
	m_pDataArr.RemoveAll();

	CStringArray  lenArr;
	CPtrArray	  pDataArr;
	m_itotal = 0;

	for(int ii = 0 ; ii < icnt ; ii++)
	{
		BYTE tmp[ENCBYTE*2] = {0,};
		
		psub = new char[ENCBYTE];
		memcpy(psub, (char*)&pbuf[ENCBYTE * ii], ENCBYTE);

		int ilen = m_pcrypt->base64_encode(tmp, (BYTE*)psub, ENCBYTE);
		m_itotal += ilen;
		BYTE* m_outB = new BYTE[ilen + 1];
		memset(m_outB, 0x00, ilen + 1);
		memcpy(m_outB, tmp, ilen);
		str.Format(_T("%d"), ilen);
		m_lenArr.Add(str);
		m_pDataArr.Add(m_outB);
	}

	if(ileft != 0)
	{
		BYTE tmp[ENCBYTE*2] = {0,};
		
		psub = new char[ileft];
		memcpy(psub, (char*)&pbuf[ENCBYTE * icnt], ileft);

		int ilen = m_pcrypt->base64_encode(tmp, (BYTE*)psub, ileft);
		m_itotal += ilen;
		BYTE* m_outB = new BYTE[ilen];
		memcpy(m_outB, tmp, ilen);
		str.Format(_T("%d"), ilen);
		m_lenArr.Add(str);
		m_pDataArr.Add(m_outB);
	}

	int ntotal = 0;
	m_bSending = true;
	m_cnt = 0;
	m_totalSendCnt = m_pDataArr.GetSize();
	m_strResult.Empty();
	SetTimer(TM_UPLOAD, 100, NULL);
}
*/

void CControlwnd::FileUpload(CString cmpath,CString strpath, CString uuid)
{
	CString		file, str, tmpS, strlog;
	CFile		hFile;
	char  *pbuf = NULL;
	char  *psub = NULL;
	int icnt = 0;
	int ileft = 0;
	m_strComPath = cmpath;
	m_struuid = uuid;
	//if(m_bSending)
	//	return;

	if(m_strComPath.IsEmpty())
		return;

	if (!hFile.Open(strpath, CFile::modeRead|CFile::shareDenyNone))
	{
		AfxMessageBox(_T("file open fail"));
		int nn = GetLastError();
		return;
	}

	int pos = strpath.ReverseFind(_T(_T('\\')));
	if (pos != -1)
		m_fileN = strpath.Mid(pos+1);

	int fileL = (int)hFile.GetLength();
	pbuf = new char[fileL];

	memset(pbuf, 0, fileL);
	hFile.Read(&pbuf[0], fileL);

	hFile.Close();

	icnt = fileL / ENCBYTE;
	ileft = fileL % ENCBYTE;

	m_lenArr.RemoveAll();
	m_pDataArr.RemoveAll();

	CStringArray  lenArr;
	CPtrArray	  pDataArr;
	m_itotal = 0;

	for(int ii = 0 ; ii < icnt ; ii++)
	{
		BYTE tmp[ENCBYTE*2] = {0,};
		
		psub = new char[ENCBYTE];
		memcpy(psub, (char*)&pbuf[ENCBYTE * ii], ENCBYTE);

		int ilen = m_pcrypt->base64_encode(tmp, (BYTE*)psub, ENCBYTE);
		m_itotal += ilen;
		BYTE* m_outB = new BYTE[ilen + 1];
		memset(m_outB, 0x00, ilen + 1);
		memcpy(m_outB, tmp, ilen);
		str.Format(_T("%d"), ilen);
		m_lenArr.Add(str);
		m_pDataArr.Add(m_outB);
	}

	if(ileft != 0)
	{
		BYTE tmp[ENCBYTE*2] = {0,};
		
		psub = new char[ileft];
		memcpy(psub, (char*)&pbuf[ENCBYTE * icnt], ileft);

		int ilen = m_pcrypt->base64_encode(tmp, (BYTE*)psub, ileft);
		m_itotal += ilen;
		BYTE* m_outB = new BYTE[ilen];
		memcpy(m_outB, tmp, ilen);
		str.Format(_T("%d"), ilen);
		m_lenArr.Add(str);
		m_pDataArr.Add(m_outB);
	}

	int ntotal = 0;
	m_bSending = true;
	m_cnt = 0;
	m_totalSendCnt = m_pDataArr.GetSize();
	m_strResult.Empty();
	SetTimer(TM_UPLOAD, 100, NULL);
}

/*
void CControlwnd::DownFile(LPCTSTR path, LPCTSTR filename)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_bSending)
		return;

	CString tmps;
	m_strComPath.Format(_T("%s"), path);
	m_strComPath.TrimRight();
	m_strFilename.Format(_T("%s"), filename);
	m_strFilename.TrimRight();

	CString filepath;
	char name_filter[] = "All Files (*.*)|*.*|";
	CFileDialog ins_dlg(TRUE, "", m_strFilename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);
	// 파일 형식 콤보박스에 나열된 필터들 중에서 2번째 항목(*.cpp)을 선택한다.
	ins_dlg.m_ofn.nFilterIndex = 2;

	if(ins_dlg.DoModal() == IDOK)
	{
		filepath = ins_dlg.GetPathName();
	}
	else
		return;

	tmps.Format(_T("%s"), filepath);
	if(tmps.IsEmpty())
		return;
	

	
	CFileFind find;
	if (find.FindFile(filepath))
	{
		if(::MessageBox(this->m_hWnd, _T("동일한 파일이 존재합니다. 계속 하시겠습니까?"), "K-Bond", MB_OKCANCEL) == IDCANCEL)
			return;
	}

	m_strDownPath.Format(_T("%s"), filepath);
	
//	m_strDownPath.Format(_T("%s\\%s"), cfilepath, m_strFilename);
	m_strDownPath.TrimLeft();
	m_strDownPath.TrimRight();

	m_strdownCnt = "1";
	struct mid_down* pmid = new struct mid_down;
	FillMemory(pmid, sizeof(struct mid_down), ' ');

	memcpy(pmid->filepath, (char*)m_strComPath.operator LPCSTR(), m_strComPath.GetLength());
	memcpy(pmid->filename, (char*)m_strFilename.operator LPCSTR(), m_strFilename.GetLength());
	memset(pmid->cnt, 0, 19);
	memcpy(pmid->cnt, (char*)m_strdownCnt.operator LPCSTR(), m_strdownCnt.GetLength());
	//pmid->cnt[0] = 1;

	m_pReDataArr.RemoveAll();
	m_relenArr.RemoveAll();
	m_itotal = 0;
	parr.RemoveAll();
	lenarr.RemoveAll();
	m_cnt = 0;
	m_bSending = true;
	if(sendTR("PIBOPFRM", (char*)pmid, sizeof(struct mid_down), false))
	{
		HCURSOR hwaitcursor = ::LoadCursor(NULL, IDC_WAIT);
		SetCursor(hwaitcursor);
	//	BeginWaitCursor();
	}
}
*/

void CControlwnd::DownFile(LPCTSTR path, LPCTSTR filename, LPCTSTR uuid , LPCTSTR bopen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_bSending)
		return;

	CString tmps;
	m_strComPath.Format(_T("%s"), path);
	m_strComPath.TrimRight();
	m_strFilename.Format(_T("%s"), filename);
	m_strFilename.TrimRight();
	m_struuid.Format(_T("%s"), uuid);
	m_struuid.TrimRight();
	m_struuid = uuid;
	
	m_stropen.Format(_T("%s"), bopen);
	m_stropen.TrimRight();
	
	CString filepath;
	char name_filter[] = "All Files (*.*)|*.*|";
	CFileDialog ins_dlg(FALSE, "", m_strFilename, OFN_HIDEREADONLY, name_filter, NULL);
	// 파일 형식 콤보박스에 나열된 필터들 중에서 2번째 항목(*.cpp)을 선택한다.
	ins_dlg.m_ofn.nFilterIndex = 2;

	if(ins_dlg.DoModal() == IDOK)
	{
		filepath = ins_dlg.GetPathName();
	}
	else
		return;

	tmps.Format(_T("%s"), filepath);
	if(tmps.IsEmpty())
		return;

	CFileFind find;
	if (find.FindFile(filepath))
	{
		if(::MessageBox(this->m_hWnd, _T("동일한 파일이 존재합니다. 계속 하시겠습니까?"), "K-Bond", MB_OKCANCEL) == IDCANCEL)
			return;
	}

	m_strDownPath.Format(_T("%s"), filepath);
	m_strDownPath.TrimLeft();
	m_strDownPath.TrimRight();

	m_strdownCnt = "1";
	struct mid_down* pmid = new struct mid_down;
	FillMemory(pmid, sizeof(struct mid_down), ' ');

	memcpy(pmid->filepath, (char*)m_strComPath.operator LPCSTR(), m_strComPath.GetLength());
	memcpy(pmid->filename, (char*)m_strFilename.operator LPCSTR(), m_strFilename.GetLength());
	memcpy(pmid->uuid, (char*)m_struuid.operator LPCSTR(), m_struuid.GetLength());
	memset(pmid->cnt, 0, 19);
	memcpy(pmid->cnt, (char*)m_strdownCnt.operator LPCSTR(), m_strdownCnt.GetLength());
	//pmid->cnt[0] = 1;

	m_pReDataArr.RemoveAll();
	m_relenArr.RemoveAll();
	m_itotal = 0;
	parr.RemoveAll();
	lenarr.RemoveAll();
	m_cnt = 0;
	m_bSending = true;
	if(sendTR("PIBOPFRM", (char*)pmid, sizeof(struct mid_down), false))
	{
		HCURSOR hwaitcursor = ::LoadCursor(NULL, IDC_WAIT);
		SetCursor(hwaitcursor);
	//	BeginWaitCursor();
	}
}


BOOL CControlwnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetCursor(LoadCursor(NULL, m_bSending ?  IDC_WAIT : IDC_ARROW));
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CControlwnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString tmpS, temp, tmps;
	switch(nIDEvent)
	{
		case TM_ARRUPLOAD:
			{
				KillTimer(TM_ARRUPLOAD);
				if(m_upArr.GetSize() > 0)
				{
					tmpS = m_upArr.GetAt(0);
					temp = Parser(tmpS, _T("\t"));
					tmps = Parser(tmpS, _T("\t"));
					m_upArr.RemoveAt(0);
					FileUpload(temp, tmps, tmpS);
				}	
			}
			break;
		case TM_UPLOAD:
			{
				CString tmpS, strlog, tmps;
				char  *psub = NULL;
				psub = new char[sizeof(struct mid) + 1];
				memset(psub, 0x00, sizeof(struct mid) + 1);
				//memset(psub, ' ', sizeof(struct mid));
	
				struct mid* pmid = new struct mid;
				//FillMemory(pmid, sizeof(struct  mid), ' ');
				FillMemory(pmid, sizeof(struct  mid), 0x00);

				memcpy(pmid->filepath, (LPSTR)(LPCTSTR)m_strComPath, m_strComPath.GetLength());
				memcpy(pmid->filename, (LPSTR)(LPCTSTR)m_fileN, m_fileN.GetLength());
				memcpy(pmid->uuid, (LPSTR)(LPCTSTR)m_struuid, m_struuid.GetLength());
	
				memcpy(pmid->data, (BYTE*)m_pDataArr.GetAt(0), atoi(m_lenArr.GetAt(0)));

				tmpS.Format(_T("%d"), atoi(m_lenArr.GetAt(0)));
				memcpy(pmid->len, (LPSTR)(LPCTSTR)tmpS, tmpS.GetLength());

				tmpS.Format(_T("%d"), 	m_cnt);
				memcpy(pmid->cnt, (LPSTR)(LPCTSTR)tmpS, tmpS.GetLength());

				memcpy(psub, pmid, sizeof(struct mid));

				m_strResult.Format(_T("%.0d"), (((m_cnt + 1) * 100)/m_totalSendCnt));

				if(atoi(m_strResult) >= 99)
					m_strResult = _T("99");

				m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (LPARAM)(LPCTSTR)m_Param.name);
				if(sendTR("PIBOPFRM", psub, sizeof(struct mid)))
				{
					m_lenArr.RemoveAt(0);
					m_pDataArr.RemoveAt(0);
					m_cnt++;
					if(m_pDataArr.GetSize() == 0)
					{
						KillTimer(TM_UPLOAD);	
						if(m_upArr.GetSize() > 0)
						{
							tmpS = m_upArr.GetAt(0);
							temp = Parser(tmpS, _T("\t"));
							tmps = Parser(tmpS, _T("\t"));
							FileUpload(temp, tmps, tmpS);
							m_upArr.RemoveAt(0);
						}
						else if(m_upArr.GetSize() == 0)
						{
							m_strResult = "100";
							m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)(LPCTSTR)m_Param.name);
							m_bSending = false;
							m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (LPARAM)(LPCTSTR)m_Param.name);
						}
					}
				}
			}
			break;
		case TM_DOWNLOAD:
			{
				KillTimer(TM_DOWNLOAD);
				sendTR("PIBOPFRM", (char*)m_pmid, sizeof(struct mid_down), false);
			}
			break;
	}
	CWnd::OnTimer(nIDEvent);
}


BSTR CControlwnd::GetPercent(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	strResult = m_strResult;
	return strResult.AllocSysString();
}

