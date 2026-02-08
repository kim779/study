#include "stdafx.h"
#include "IB877000.h"
#include "SiHwang.h"

#include "dataio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *subg_t[] = { "동시시황", "매매동향", "거래소종목", "코스닥종목", "경제", "산업", "환율", "해외", "기타", NULL };
int   subg_v[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, -1 };

CSiHwang::CSiHwang(CWnd *pParent) : CAxisExt(pParent)
{
	m_bWait = FALSE;
	m_news_srow = -1;
	m_usid = Variant(userCC, "");

	CString path;
	path.Format("%s\\%s\\", Variant(homeCC,""), "image");
	m_hBtn2[0] = GetAxBitmap(path + "2btn.bmp");
	m_hBtn2[1] = GetAxBitmap(path + "2btn_dn.bmp");
	m_hBtn2[2] = GetAxBitmap(path + "2btn_en.bmp");
	m_hBtn6[0] = GetAxBitmap(path + "6btn.bmp");
	m_hBtn6[1] = GetAxBitmap(path + "6btn_dn.bmp");
	m_hBtn6[2] = GetAxBitmap(path + "6btn_en.bmp");
}

CSiHwang::~CSiHwang()
{
}

BEGIN_MESSAGE_MAP(CSiHwang, CWnd)
	//{{AFX_MSG_MAP(CSiHwang)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_VSCROLL_PS_END, OnScrollEnd)
	ON_BN_CLICKED(IDC_BTN_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_BTN_PRETAG, OnPreTag)
	ON_BN_CLICKED(IDC_CHKINPUT,   OnChkInp)
	ON_BN_CLICKED(IDC_BTN_TMPL1,  OnTmpl1)
END_MESSAGE_MAP()


void CSiHwang::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, m_crBodyBk);
}

LRESULT CSiHwang::OnUser( WPARAM wParam, LPARAM lParam )
{
	const int msg = LOBYTE(LOWORD(wParam));
	const int key = HIBYTE(LOWORD(wParam));
	const int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;
	
	switch(msg)
	{
	case DLL_INB:
		break;
	case DLL_OUBx:
		{
			struct _extTHx* extTHx = (struct _extTHx*)lParam;
			CString slog;
	//		slog.Format("1 [%d]   [%d]", extTHx->key, extTHx->size);
	//		AfxMessageBox(slog);
			if (extTHx->key == TK_PIBONEWS)
			{
				char* pBytes = extTHx->data;
				//pBytes += 258;
				
			
				ResponsePIBONEWS(pBytes, extTHx->size); break;
			}
		}
		break;
	case DLL_OUB:
		switch(key)
		{
		case TK_PIBO7710: ResponsePIBO7710(dat, len); break;
		case TK_PIBONEWS: ResponsePIBONEWS(dat, len); break;
		case TK_PIDOSIHW: ResponsePIDOSIHW(dat, len); break;
		}
		break;

	case DLL_ALERT:
		break;
	}
	return 0;
}

void CSiHwang::RequestPIBO7710(LPCSTR nkey)
{
	m_bWait = TRUE;

	pibo7710_mid mid;
	
	FillMemory(&mid, sizeof(mid), ' ');
// 	CopyMemory(mid.fday, "20111024", 8);
	//CopyMemory(mid.gubn, "1000000000000", 13);
//	CopyMemory(mid.gubn, "100000000000000", 15);
	CopyMemory(mid.gubn, "10000000000000000000", 20);
	CopyMemory(mid.subg, "999", 3);
	CopyMemory(mid.grid_i.nrow, "0100", 4);
	CopyMemory(mid.grid_i.vflg, "1", 1);
	CopyMemory(mid.grid_i.gdir, "2", 1);
	CopyMemory(mid.grid_i.sdir, "2", 1);
	CopyMemory(mid.grid_i.ikey, "\x00", 1);
	CopyMemory(mid.grid_i.ikey, "0000", 4);
	if (nkey) 
	{
		mid.grid_i.ikey[0] = KY_PGDN;
		CopyMemory(mid.grid_i.save, nkey, sizeof(mid.grid_i.save));
	}

	if (m_pGrid->GetSafeHwnd() && nkey==NULL)
	{
		m_pGrid->SetRowCount(1);
	}

	SendTR("pibo7710", 0, (LPCSTR)&mid, sizeof(mid), TK_PIBO7710);	//2013.07.04 KSJ pibf --> pibo로 수정
}

void CSiHwang::ResponsePIBO7710( LPCSTR dat, int len )
{
	m_bWait = FALSE;

	pibo7710_mod *mod = (pibo7710_mod*)dat;
	
	if (mod->grid_o.xpos[0] & PS_NEXT)
		m_nkey = CString( mod->grid_o.save, sizeof(mod->grid_o.save) );
	else 
		m_nkey.Empty();
	
	if (m_pGrid->GetSafeHwnd())
	{
		const int nrec = Str2Int(mod->nrec, sizeof(mod->nrec));
		for(int n=0; n<nrec; ++n)
		{
			pibo7710_grid *gd = &mod->grid[n];

			CString date;
			CString titl(gd->titl, sizeof(gd->titl));
			CString subg(gd->subg, sizeof(gd->subg));
			CString keyv(gd->keyv, sizeof(gd->keyv));

			date.Format("%.4s/%.2s/%.2s %.2s:%.2s", 
				&gd->date[0], &gd->date[4], &gd->date[6], &gd->time[0], &gd->time[2]);
			
			titl.TrimRight();
			subg.TrimRight();

			const int row = m_pGrid->InsertRow("");
			m_pGrid->SetItemFormat(row, 0, DT_VCENTER|DT_CENTER|DT_SINGLELINE); m_pGrid->SetItemText(row, 0, date);
			m_pGrid->SetItemFormat(row, 1, DT_VCENTER|DT_LEFT  |DT_SINGLELINE); m_pGrid->SetItemText(row, 1, titl);
			m_pGrid->SetItemFormat(row, 2, DT_VCENTER|DT_CENTER|DT_SINGLELINE); m_pGrid->SetItemText(row, 2, subg);
			m_pGrid->SetItemText(row, 3, keyv);
		}
	}
}

int CSiHwang::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pGrid = std::make_unique<CGridCtrl>(0, 0, 0, 0);
	if (m_pGrid->Create(CRect(0,0,0,0), this, IDC_GRID))
	{
		m_pGrid->SetEditable(FALSE);
		m_pGrid->EnableSelection(FALSE);
		m_pGrid->SetFrameFocusCell(FALSE);
		m_pGrid->ShowScrollBar(SB_VERT, TRUE);
		m_pGrid->SetRowCount(1);
		m_pGrid->SetColumnCount(4);
		m_pGrid->SetFixedRowCount(1);
		m_pGrid->SetFixedColumnCount(0);
		m_pGrid->SetBkColor( m_crWhite );

		CGridCellBase *pc = m_pGrid->GetDefaultCell(TRUE, FALSE);
		if (pc)
		{
			pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
			pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
		}

		pc = m_pGrid->GetDefaultCell(FALSE, FALSE);
		if (pc)
		{
			pc->SetFormat(DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		}
		
		CString heads[4] = {"시간", "제목", "분류", "KEYV"};
		const int   width[] = {120, 570, 80, 0};
		for(int n=0; n<m_pGrid->GetColumnCount(); ++n)
		{
			m_pGrid->SetItemText(0, n, heads[n]);
			m_pGrid->SetColumnWidth(n, width[n]);
			m_pGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}

	m_pTitle = std::make_unique <CRichEditCtrl >();
	m_pTitle->Create(WS_VISIBLE|WS_CHILD|WS_DLGFRAME, CRect(0,0,0,0), this, IDC_TITLE);
	m_pTitle->SetFont( GetAxFont("굴림체", 12, false, false) );
	m_pTitle->SetEventMask(ENM_MOUSEEVENTS);

	m_pBody = std::make_unique <CRichEditCtrl>();
	m_pBody->Create(WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_WANTRETURN|WS_DLGFRAME, CRect(0,0,0,0), this, IDC_BODY);
	m_pBody->LimitText();
	m_pBody->SetFont(m_pFont);
	m_pBody->SetEventMask(ENM_MOUSEEVENTS);
	
	m_pSubg = std::make_unique <CComboBox>();
	m_pSubg->Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST|WS_VSCROLL, CRect(0,0,120,200), this, IDC_CB_SUBG);
	m_pSubg->SetFont( GetAxFont("굴림체", 13, false, false) );
	for(int n=0; subg_t[n]; ++n)
		m_pSubg->AddString(subg_t[n]);
	
	m_pChkSendTime = std::make_unique <CButton>();
	m_pChkSendTime->Create("강제시간입력", WS_VISIBLE|WS_CHILD|BS_AUTOCHECKBOX, CRect(0,0,0,0), this, IDC_CHKINPUT);
	m_pChkSendTime->SetFont(m_pFont);

	m_pDate = std::make_unique <CRichEditCtrl>();
	m_pDate->Create(WS_VISIBLE|WS_CHILD|ES_NUMBER, CRect(0,0,0,0), this, IDC_DATE);
	m_pDate->SetFont(GetAxFont("굴림체", 12, false, false));
	m_pDate->EnableWindow(FALSE);
	m_pDate->LimitText(8);
	m_pDate->SetEventMask(ENM_MOUSEEVENTS);

	m_pTime = std::make_unique <CRichEditCtrl>();
	m_pTime->Create(WS_VISIBLE|WS_CHILD|ES_NUMBER, CRect(0,0,0,0), this, IDC_TIME);
	m_pTime->SetFont(GetAxFont("굴림체", 12, false, false));
	m_pTime->EnableWindow(FALSE);
	m_pTime->LimitText(6);
	m_pTime->SetEventMask(ENM_MOUSEEVENTS);

	m_pInsert = CreateImgButton("입력", IDC_BTN_INSERT, m_hBtn2);
	m_pUpdate = CreateImgButton("수정", IDC_BTN_UPDATE, m_hBtn2);
	m_pDelete = CreateImgButton("삭제", IDC_BTN_DELETE, m_hBtn2);
	m_pPreTag = CreateImgButton("<PRE>", IDC_BTN_PRETAG, m_hBtn2);
	m_pTmp1Tag = CreateImgButton("템플릿", IDC_BTN_TMPL1, m_hBtn2);

	return 0;
}

void CSiHwang::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(0, PADDING_VALUE/2);

	const int btn_w = 40, btn_h = 24;
	int pos_x = 0, pos_y = rc.top;

	const int grid_h = 198;
	if (m_pGrid->GetSafeHwnd())
	{
		m_pGrid->MoveWindow(pos_x, pos_y, rc.Width(), grid_h);
		pos_y += grid_h + PADDING_VALUE*2;
	}

	m_rcTitle.SetRect(pos_x, pos_y, pos_x+rc.Width(), pos_y+btn_h);
	m_rcControl.SetRect(pos_x, rc.bottom-btn_h, pos_x+rc.Width(), rc.bottom);
	m_rcBody.SetRect(pos_x, m_rcTitle.bottom+PADDING_VALUE, pos_x+rc.Width(), m_rcControl.top-PADDING_VALUE);

	const int subg_w = 120;
	m_pSubg->MoveWindow(m_rcTitle.left, m_rcTitle.top, subg_w, btn_h);
	m_pSubg->SetWindowPos(NULL, m_rcTitle.left, m_rcTitle.top, subg_w, btn_h+subg_w, SWP_NOMOVE|SWP_NOZORDER);
	m_pTitle->MoveWindow(m_rcTitle.left+subg_w+PADDING_VALUE/2, m_rcTitle.top, m_rcTitle.Width()-subg_w-PADDING_VALUE/2, btn_h);
	

	m_pBody->MoveWindow(&m_rcBody);

	{
		pos_x = m_rcControl.right - btn_w;
		pos_y = m_rcControl.top;

		//m_pPreTag->MoveWindow(m_rcControl.left, pos_y, btn_w, btn_h);
		m_pTmp1Tag->MoveWindow(m_rcControl.left, pos_y, btn_w, btn_h);

		m_pChkSendTime->MoveWindow(m_rcControl.left+btn_w+PADDING_VALUE+30, pos_y, (int)(btn_w*2.5), btn_h);
		m_pDate->MoveWindow(m_rcControl.left+(btn_w*4)+(PADDING_VALUE*2), pos_y, btn_w*2, btn_h);
		m_pTime->MoveWindow(m_rcControl.left+(btn_w*6)+(PADDING_VALUE*3), pos_y, btn_w*2, btn_h);

		m_pDelete->MoveWindow(pos_x, pos_y, btn_w, btn_h); pos_x -= btn_w + PADDING_VALUE/2;
		m_pUpdate->MoveWindow(pos_x, pos_y, btn_w, btn_h); pos_x -= btn_w + PADDING_VALUE/2;
		m_pInsert->MoveWindow(pos_x, pos_y, btn_w, btn_h); pos_x -= btn_w + PADDING_VALUE/2;
	}
}

LRESULT CSiHwang::OnScrollEnd( WPARAM wParam, LPARAM lParam )
{
	if (!m_bWait && !m_nkey.IsEmpty())
		RequestPIBO7710(m_nkey);

	return 0;
}

void CSiHwang::OnDestroy() 
{
/*
	SafeDelete(m_pGrid);
	SafeDelete(m_pTitle);
	SafeDelete(m_pBody);
	SafeDelete(m_pInsert);
	SafeDelete(m_pUpdate);
	SafeDelete(m_pDelete);
	SafeDelete(m_pSubg);
	SafeDelete(m_pPreTag);
	SafeDelete(m_pDate);
	SafeDelete(m_pTime);
	SafeDelete(m_pChkSendTime);
	SafeDelete(m_pTmp1Tag);
*/
	CWnd::OnDestroy();
}

void CSiHwang::SafeDelete( CWnd *pWnd )
{
	if (pWnd)
	{
		pWnd->DestroyWindow();
		delete pWnd;
	}
}

BOOL CSiHwang::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam==IDC_GRID)
	{
		GV_DISPINFO *pi = (GV_DISPINFO*)lParam;
		NM_GRIDVIEW *view = (NM_GRIDVIEW*)lParam;

		if (view->hdr.code == NM_DBLCLK && view->iRow>0)
		{
			m_news_keyv = m_pGrid->GetItemText(view->iRow, 3);
			RequestPIBONEWS(m_news_keyv);

			m_news_date = m_news_keyv.Mid(0, 8);
			m_news_gubn = "000000" + m_news_keyv.Mid(8, 2);
			m_news_subg = "0000" + m_news_keyv.Mid(10, 4);
			m_news_seqn = m_news_keyv.Mid(14, 10);
			m_news_seqn = m_news_seqn.Right(8);
			m_news_time = m_news_seqn.Left(6);
			
// 			CString emsg;
// 			emsg.Format("[KSJ]date[%s] time[%s] gubn[%s] subg[%s] seqn[%s]\n",
// 				m_news_date, m_news_time, m_news_gubn, m_news_subg, m_news_seqn);
// 			OutputDebugString(emsg);

			if (m_news_srow!=-1)
			{
				for(int i=0; i<m_pGrid->GetColumnCount(); ++i)
					m_pGrid->SetItemBkColour( m_news_srow, i, m_crWhite );	
				m_pGrid->RedrawRow(m_news_srow);
			}
			for(int i=0; i<m_pGrid->GetColumnCount(); ++i)
				m_pGrid->SetItemBkColour( view->iRow, i, m_crLine );
			m_pGrid->RedrawRow(view->iRow);
			m_news_srow = view->iRow;

			m_news_subg_idx = m_pSubg->FindString(0, m_pGrid->GetItemText(view->iRow, 2));
			m_pSubg->SetCurSel(m_news_subg_idx);

		}
	}
	else
	{
		MSGFILTER * lpMsgFilter = (MSGFILTER *)lParam; 
		if ((lpMsgFilter->nmhdr.code == EN_MSGFILTER) && (lpMsgFilter->msg == WM_RBUTTONDOWN))
		{
			CPoint point;                                            
			::GetCursorPos(&point); //where is the mouse?
			CMenu menu; //lets display out context menu :) 
			DWORD dwSelectionMade = 0;
			VERIFY(menu.LoadMenu(IDR_MENU1) );
			CMenu *pmenuPopup = menu.GetSubMenu(0);
			ASSERT(pmenuPopup != NULL);                                       
			dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD), point.x, point.y, this);
			pmenuPopup->DestroyMenu(); 

			CRichEditCtrl *pRich = NULL;
			switch(wParam)
			{
				case IDC_BODY:
					pRich = m_pBody.get();
					break;

				case IDC_TITLE:
					pRich = m_pTitle.get();
					break;

				case IDC_TIME:
					pRich = m_pTime.get();
					break;

				case IDC_DATE:
					pRich = m_pDate.get();
					break;

				default:
					return CWnd::OnNotify(wParam, lParam, pResult);
			}

			if(dwSelectionMade == ID_EDIT_COPY)				pRich->Copy();
			else if(dwSelectionMade == ID_EDIT_CUT)			pRich->Cut();
			else if(dwSelectionMade == ID_EDIT_PASTE)		pRich->Paste();
			else if(dwSelectionMade == ID_EDIT_SELECT_ALL)	pRich->SetSel(0, -1);
		}
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CSiHwang::RequestPIBONEWS( LPCSTR keyv )
{
	/*
	pibonews_mid mid;
	FillMemory(&mid, sizeof(mid), ' ');
	CopyMemory(&mid, (LPCSTR)keyv, strlen(keyv));
	SendTR("pibonews", 0, (LPCSTR)&mid, sizeof(mid), TK_PIBONEWS);
	*/

	pnewsrc_mid mid;
	FillMemory(&mid, sizeof(mid), ' ');
	CopyMemory(&mid, (LPCSTR)keyv, strlen(keyv));
	SendTR("pnewsrch", 0, (LPCSTR)&mid, sizeof(mid), TK_PIBONEWS);
}

void CSiHwang::ResponsePIBONEWS( LPCSTR dat, int len )
{
	pnewsrc_mod *mod = (pnewsrc_mod*)dat;
	LPCSTR data = &mod->data[0];
	const int dlen = len - offsetof(pnewsrc_mod, data);

	CString titl(mod->titl, sizeof(mod->titl));

	CString body(data, dlen);
	body.Replace("text://<font style='font-size:12px'> ", "");
	body.Replace("<br>", "\n");
	body.Remove(0);

	titl.TrimRight();
	body.TrimRight();
	
	m_pTitle->SetWindowText(titl);
	m_pBody->SetWindowText(body);

	/*
	pibonews_mod *mod = (pibonews_mod*)dat;
	
	LPCSTR data = &mod->data[0];
	int dlen = len - offsetof(pibonews_mod, data);
	
	CString titl(mod->titl, sizeof(mod->titl));

#if FALSE
	CString body, tbody;	
	const int llen = 130;
	for(int i=0; i<dlen; ++i)
	{
		tbody.Format("%.*s", llen, ((LPCSTR)data)+(i*llen));
		tbody.Remove(0);
		body += tbody;
		body += "\n";
	}
#else
	CString body(data, dlen);
	body.Replace("text://<font style='font-size:12px'> ", "");
	body.Replace("<br>", "\n");
	body.Remove(0);
#endif

	titl.TrimRight();
	body.TrimRight();
	
	m_pTitle->SetWindowText(titl);
	m_pBody->SetWindowText(body);
	*/
}

std::shared_ptr<CfxImgButton> CSiHwang::CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp)
{
	std::shared_ptr<CfxImgButton> pBtn = std::make_shared<CfxImgButton>(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return NULL;
}

void CSiHwang::OnInsert()
{
	int tlen=0, blen=0;
	char tmp[128]{};
	std::unique_ptr<char[]> data = std::make_unique<char[]>(2048);
	CString titl, body, tbody;
	
	m_pTitle->GetWindowText(titl); titl.TrimLeft(); titl.TrimRight();
	m_pBody->GetWindowText(body); titl.TrimLeft(); titl.TrimRight();
	
	tlen = min(TIT_LEN, titl.GetLength());
	blen = body.GetLength();

	if (tlen==0)
	{
		MessageBox("제목을 입력하시기 바랍니다.");
		return;
	}
	if (blen==0)
	{
		MessageBox("본문을 입력하시기 바랍니다.");
		return;
	}
	if (m_pSubg->GetCurSel()<0)
	{
		MessageBox("시황구분을 입력하시기 바랍니다.");
		return;
	}

	m_sihw_queue.clear();
	
	while(body.GetLength()>0)
	{
		pidosihw_mid *mid = (pidosihw_mid*)data.get();
		
		FillMemory(mid, sizeof(pidosihw_mid), ' ');
		CopyMemory(mid->flag, "I", 1);
		CopyMemory(mid->step, "M", 1);
		CopyMemory(mid->usid, m_usid, m_usid.GetLength());
		CopyMemory(mid->gubn, "00000001", sizeof(mid->gubn));

		if (m_pChkSendTime->GetCheck())
		{
			CString strDate, strTime;
			m_pDate->GetWindowText(strDate);
			m_pTime->GetWindowText(strTime);
			strDate = strDate.Left(8);
			strTime = strTime.Left(6);
			CopyMemory(mid->date, (LPCSTR)strDate, strDate.GetLength());
			CopyMemory(mid->time, (LPCSTR)strTime, strTime.GetLength());
		}

		sprintf(tmp, "%08d", subg_v[m_pSubg->GetCurSel()]);
		CopyMemory(mid->subg, tmp, sizeof(mid->subg));

		CopyMemory(mid->titl, titl, tlen);

		//seqn 정보 넣음.
		CopyMemory(mid->seqn, (LPCSTR)m_news_seqn, sizeof(mid->seqn));

		tbody = body.Left( min(DAT_LEN, body.GetLength()) );
		CopyMemory(mid->data, tbody, tbody.GetLength());
		sprintf(tmp, "%08d", tbody.GetLength());
		CopyMemory(mid->dlen, tmp, 8);

		body.Delete(0, tbody.GetLength());

		m_sihw_queue.push_back(vector<char>(data.get(), data.get()+sizeof(pidosihw_mid)+tbody.GetLength()-1));
	}

	if (!m_sihw_queue.empty())
	{
		pidosihw_mid *mid;
		
		mid = (pidosihw_mid*)&m_sihw_queue.front()[0];
		mid->step[0] = 'F';

		mid = (pidosihw_mid*)&m_sihw_queue.back()[0];
		mid->step[0] = 'L';

		SendTR("pidosihw", 0, (LPCSTR)&m_sihw_queue.front()[0], m_sihw_queue.front().size(), TK_PIDOSIHW);
		m_sihw_queue.erase(m_sihw_queue.begin());
	}

	m_pTitle->SetWindowText("");
	m_pBody->SetWindowText("");
	m_pSubg->SetCurSel(-1);
}

void CSiHwang::OnUpdate()
{
	if (m_news_keyv.IsEmpty() || m_news_keyv.GetLength()==0)
	{
		MessageBox("선택된 시황이 없습니다.", "오류", MB_OK|MB_ICONERROR);
		return;
	}

	if (m_news_subg_idx != m_pSubg->GetCurSel())
	{
		CString emsg, osubg, nsubg;
		m_pSubg->GetLBText(m_news_subg_idx, osubg);
		m_pSubg->GetLBText(m_pSubg->GetCurSel(), nsubg);
		emsg.Format("수정의 경우 [구분]을 바꿀 수 없습니다. [%s]->[%s]", osubg, nsubg);
		MessageBox(emsg, "오류", MB_OK|MB_ICONERROR);
		return;
	}

	int tlen=0, blen=0;
	char tmp[128]{};
	std::unique_ptr<char[]> data = std::make_unique<char[]>(2048);
	CString titl, body, tbody;
	
	m_pTitle->GetWindowText(titl);
	m_pBody->GetWindowText(body);
	
	tlen = min(TIT_LEN, titl.GetLength());
	blen = body.GetLength();
	
	if (tlen==0)
	{
		MessageBox("제목을 입력하시기 바랍니다.");
		return;
	}
	if (blen==0)
	{
		MessageBox("본문을 입력하시기 바랍니다.");
		return;
	}
	if (m_pSubg->GetCurSel()<0)
	{
		MessageBox("시황구분을 입력하시기 바랍니다.");
		return;
	}

	m_sihw_queue.clear();
	
	while(body.GetLength()>0)
	{
		pidosihw_mid *mid = (pidosihw_mid*)data.get();
		
		FillMemory(mid, sizeof(pidosihw_mid), ' ');
		CopyMemory(mid->flag, "U", 1);
		CopyMemory(mid->step, "M", 1);
		CopyMemory(mid->usid, m_usid, m_usid.GetLength());
		CopyMemory(mid->date, (LPCSTR)m_news_date, sizeof(mid->date));
		CopyMemory(mid->time, (LPCSTR)m_news_time, sizeof(mid->time));
		CopyMemory(mid->gubn, "00000001", sizeof(mid->gubn));
		CopyMemory(mid->subg, (LPCSTR)m_news_subg, sizeof(mid->subg));
		CopyMemory(mid->seqn, (LPCSTR)m_news_seqn, sizeof(mid->seqn));
		CopyMemory(mid->titl, titl, tlen);
		
		tbody = body.Left( min(DAT_LEN, body.GetLength()) );
		CopyMemory(mid->data, tbody, tbody.GetLength());
		sprintf(tmp, "%08d", tbody.GetLength());
		CopyMemory(mid->dlen, tmp, 8);
		
		body.Delete(0, tbody.GetLength());
		
		m_sihw_queue.push_back(vector<char>(data.get(), data.get()+sizeof(pidosihw_mid)+tbody.GetLength()-1));
	}
	
	if (!m_sihw_queue.empty())
	{
		pidosihw_mid *mid;
		
		mid = (pidosihw_mid*)&m_sihw_queue.front()[0];
		mid->step[0] = 'F';
		
		mid = (pidosihw_mid*)&m_sihw_queue.back()[0];
		mid->step[0] = 'L';
		
		//2013.07.05 수정일때는 한페이지 이면 S를 넣어준다.
		//서버에서 초기화 하기 위해서..
		if(m_sihw_queue.size() == 1)
		{
			mid = (pidosihw_mid*)&m_sihw_queue.back()[0];
			mid->step[0] = 'S';	
		}
		//END

		SendTR("pidosihw", 0, (LPCSTR)&m_sihw_queue.front()[0], m_sihw_queue.front().size(), TK_PIDOSIHW);
		m_sihw_queue.erase(m_sihw_queue.begin());
	}
	
	m_pTitle->SetWindowText("");
	m_pBody->SetWindowText("");
	m_pSubg->SetCurSel(-1);
}

void CSiHwang::OnDelete()
{
	if (m_news_keyv.IsEmpty() || m_news_keyv.GetLength()==0)
	{
		MessageBox("선택된 시황이 없습니다.", "오류", MB_OK|MB_ICONERROR);
		return;
	}

	std::unique_ptr<char[]>data = std::make_unique<char[]>(2048);

	pidosihw_mid *mid = (pidosihw_mid*)data.get();
	
	FillMemory(mid, sizeof(pidosihw_mid), ' ');
	CopyMemory(mid->flag, "D", 1);
	CopyMemory(mid->step, "L", 1);
	CopyMemory(mid->usid, m_usid, m_usid.GetLength());
	CopyMemory(mid->date, (LPCSTR)m_news_date, sizeof(mid->date));
	CopyMemory(mid->time, (LPCSTR)m_news_time, sizeof(mid->time));
	CopyMemory(mid->gubn, "00000001", sizeof(mid->gubn));
	CopyMemory(mid->subg, (LPCSTR)m_news_subg, sizeof(mid->subg));
	CopyMemory(mid->seqn, (LPCSTR)m_news_seqn, sizeof(mid->seqn));
	CopyMemory(mid->dlen, "00000000", 8);
	SendTR("pidosihw", 0, (LPCSTR)mid, sizeof(pidosihw_mid), TK_PIDOSIHW);
	
	m_pTitle->SetWindowText("");
	m_pBody->SetWindowText("");
	m_pSubg->SetCurSel(-1);
}

void CSiHwang::ResponsePIDOSIHW( LPCSTR dat, int len )
{
	//2012.10.24 KSJ 받아온 mod의 seqn의 값이 있으면 기존의 mid에 받아온 값을 넣어준다.
	CString strSeqn;
	if (!m_sihw_queue.empty())
	{
		pidosihw_mid *mid = nullptr;
		pidosihw_mod *mod = (pidosihw_mod *)dat;
		
		mid = (pidosihw_mid*)&m_sihw_queue.front()[0];
		strSeqn = CString(mod->seqn, 8);
		
		strSeqn.TrimRight(); strSeqn.TrimLeft();

		if (!strSeqn.IsEmpty())
		{
			CopyMemory(mid->seqn, (LPCSTR)strSeqn, sizeof(mid->seqn));
			m_news_seqn = strSeqn;
		}

// 		CString emsg;
// 		emsg.Format("[KSJ] ResponsePIDOSIHWseqn[%s][%s][%s]\n", m_news_seqn, strSeqn, dat);
// 		OutputDebugString(emsg);

		SendTR("pidosihw", 0, (LPCSTR)&m_sihw_queue.front()[0], m_sihw_queue.front().size(), TK_PIDOSIHW);
		m_sihw_queue.erase(m_sihw_queue.begin());
		return;
	}
	RequestPIBO7710();
}

void CSiHwang::OnPreTag()
{
	CString body;
	m_pBody->GetWindowText(body);
	body.TrimLeft();
	body.TrimRight();

	body.Replace("<pre>", "<PRE>");
	body.Replace("</pre>", "</PRE>");

	if (body.Find("<PRE>")!=-1)
	{
		body.Replace("<PRE>", "");
		body.Replace("</PRE>", "");
	}
	else 
	{
		body = "<PRE>\r\n" + body + "\r\n</PRE>";
	}
	m_pBody->SetWindowText(body);
}

HBRUSH CSiHwang::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	const int nID = pWnd->GetDlgCtrlID();
	if (nID==IDC_CHKINPUT)
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(m_crBodyBk);
		return ::CreateSolidBrush(m_crBodyBk);
	}

	return hbr;
}

void CSiHwang::OnChkInp()
{
	if (m_pChkSendTime->GetCheck())	
	{
		CString sTmp;
		const CTime tm = CTime::GetCurrentTime();
		sTmp.Format("%04d%02d%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay());
		m_pDate->SetWindowText( sTmp );
		sTmp.Format("%02d%02d%02d", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
		m_pTime->SetWindowText( sTmp );

		m_pUpdate->EnableWindow(FALSE);
		m_pDelete->EnableWindow(FALSE);
		m_pDate->EnableWindow(TRUE);
		m_pTime->EnableWindow(TRUE);
	}
	else
	{
		m_pUpdate->EnableWindow(TRUE);
		m_pDelete->EnableWindow(TRUE);
		m_pDate->EnableWindow(FALSE);
		m_pTime->EnableWindow(FALSE);
		m_pDate->SetWindowText("");
		m_pTime->SetWindowText("");
	}
	
}

void CSiHwang::OnTmpl1()
{
	DWORD dwSize=0;
	const DWORD bufSize = 2048;
	std::unique_ptr<char[]> buff = std::make_unique<char[]>(bufSize);
	CString path, body, keyv;
	path.Format("%s/tab/SJB.INI", Variant(homeCC, ""));
	
	for(int n=1; n<99; ++n)
	{
		keyv.Format("L%d", n);
		dwSize = GetPrivateProfileString("SH_TEMPATE_1", keyv, "", buff.get(), bufSize-1, path);	
		buff[dwSize++] = '\r';
		buff[dwSize++] = '\n';
		buff[dwSize] = 0;
		body += buff.get();
	}
	body.TrimLeft();
	body.TrimRight();
	m_pBody->SetWindowText(body);
}
