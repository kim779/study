#include "stdafx.h"
#include "IB877000.h"
#include "Tujaja.h"

#include "../../h/jmcode.h"
#include "dataio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SAVELOGFILE

CTujaja::CTujaja(CWnd *pParent) : CAxisExt(pParent)
{
	CString path, key;
	path.Format("%s\\%s\\", Variant(homeCC,""), "image");
	m_hBtn2[0] = GetAxBitmap(path + "2btn.bmp");
	m_hBtn2[1] = GetAxBitmap(path + "2btn_dn.bmp");
	m_hBtn2[2] = GetAxBitmap(path + "2btn_en.bmp");
	m_hBtn20[0] = GetAxBitmap(path + "20btn.bmp");
	m_hBtn20[1] = GetAxBitmap(path + "20btn_dn.bmp");
	m_hBtn20[2] = GetAxBitmap(path + "20btn_en.bmp");
	LoadHJCode();
	LoadCategory();

	DWORD dwSize=0;
	std::unique_ptr<char[]> buff = std::make_unique<char[]>(1024);


	path.Format("%s\\tab\\SJB.INI", Variant(homeCC,""));
	for(int n=1; n<99; ++n)
	{
		key.Format("L%d", n);
		dwSize = GetPrivateProfileString("TJJ_DESC", key, "", buff.get(), 1023, path);
		m_strDesc += CString(buff.get(), dwSize);
		m_strDesc.TrimLeft();
		m_strDesc.TrimRight();
		m_strDesc += "\r\n";
	}
	m_strDesc = "\r\n" + m_strDesc;
	m_strTotal.Empty();
}

CTujaja::~CTujaja()
{
}

BEGIN_MESSAGE_MAP(CTujaja, CWnd)
	//{{AFX_MSG_MAP(CTujaja)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PARSE, OnParse)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

void CTujaja::OnPaint() 
{
	CPaintDC dc(this);
	
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, m_crBodyBk);

// 	CString strdbg;
// 	strdbg.Format("%d, %d, %d, %d\n", m_rcDesc.left, m_rcDesc.top, m_rcDesc.right, m_rcDesc.bottom);
// 	OutputDebugString(strdbg);

	CFont *pOldFont = dc.SelectObject(m_pFont);
 	dc.DrawText(m_strDesc, m_rcDesc, DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS);
	dc.DrawText(m_strTotal, m_rcTotal, DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS);
	dc.SelectObject(pOldFont);
}

std::shared_ptr<CfxImgButton> CTujaja::CreateImgButton(LPCSTR caption, int nID, CBitmap** bmp)
{
	std::shared_ptr<CfxImgButton> pBtn = std::make_shared<CfxImgButton>(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP());
		return pBtn;
	}
	else return NULL;
}

void CTujaja::SafeDelete( CWnd *pWnd )
{
	if (pWnd)
	{
		pWnd->DestroyWindow();
		delete pWnd;
	}	
}

int CTujaja::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pInput = std::make_unique<CRichEditCtrl>();
	m_pInput->Create(WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN|WS_DLGFRAME, CRect(0,0,0,0), this, IDC_INPUT);
	m_pInput->SetFont(m_pFont);
	m_pInput->LimitText(1024*640);		//2015.04.14 KSJ 사이즈 640k 세팅
	m_pInput->SetEventMask(ENM_MOUSEEVENTS);

	m_pParse = CreateImgButton("분석", IDC_PARSE, m_hBtn20);
	m_pSend = CreateImgButton("전송", IDC_SEND, m_hBtn20);
	
	m_pSeqn = std::make_unique<CComboBox>();
	if (m_pSeqn->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(0,0,0,0), this, IDC_SEQN))
	{
		m_pSeqn->SetFont(GetAxFont("굴림체", 12, false, false));

		DWORD dwSize=0;
		char buff[128]{};
		CString fnam, key;
		fnam.Format("%s/tab/SJB.INI", Variant(homeCC, ""));
		for(int n=1; n<99; ++n)
		{
			key.Format("%d", n);
			dwSize = GetPrivateProfileString("TJJ_SEQN", key,  "", buff, sizeof(buff)-1, fnam);
			if (dwSize==0) continue;
			buff[dwSize] = 0;
			m_pSeqn->AddString(buff);
		}
	}
	SetWindowTheme(m_pSeqn->GetSafeHwnd(), L"", L"");


	m_pResult = std::make_unique<CGridCtrl>();
	if (m_pResult->Create(CRect(0,0,0,0), this, IDC_RESULT))
	{
		//m_pResult->SetEditable(FALSE);
		m_pResult->EnableSelection(FALSE);
		m_pResult->SetFrameFocusCell(FALSE);
		m_pResult->ShowScrollBar(SB_VERT, TRUE);
		m_pResult->SetRowCount(1);
		m_pResult->SetColumnCount(6);
		m_pResult->SetFixedRowCount(1);
		m_pResult->SetFixedColumnCount(0);
		m_pResult->SetBkColor( m_crWhite );
		
		CGridCellBase *pc = m_pResult->GetDefaultCell(TRUE, FALSE);
		if (pc)
		{
			pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
			pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
		}
		
		pc = m_pResult->GetDefaultCell(FALSE, FALSE);
		if (pc)
		{
			pc->SetFormat(DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		}
		
		CString heads[] = {"투자자", "투자자코드", "종목명", "종목코드", "매도/매수", "수량"};
		const int   width[] = {80, 80, 125, 60, 60, 60};
		for(int n=0; n<m_pResult->GetColumnCount(); ++n)
		{
			m_pResult->SetItemText(0, n, heads[n]);
			m_pResult->SetColumnWidth(n, width[n]);
			m_pResult->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}

	return 0;
}

void CTujaja::OnDestroy() 
{
	//SafeDelete(m_pInput);
	//SafeDelete(m_pSend);
	//SafeDelete(m_pParse);
	//SafeDelete(m_pSeqn);
	//SafeDelete(m_pResult);

	CWnd::OnDestroy();	
}

void CTujaja::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(rc);
	rc.top += PADDING_VALUE;
	int pos_y=0;
	const int grid_h = 160, btn_w = 48, btn_h = 24;

	m_pInput->MoveWindow(rc.left, rc.top, rc.right, grid_h);
	pos_y = rc.top + grid_h + PADDING_VALUE;

	m_pParse->MoveWindow(rc.left, pos_y, rc.Width()-300, btn_h);
	pos_y += btn_h + PADDING_VALUE;
	
	m_pResult->MoveWindow(rc.left, pos_y, rc.Width()-300, rc.bottom-pos_y);
	rc.left = rc.right - 300 + PADDING_VALUE;

	m_pSeqn->MoveWindow(rc.left, pos_y, rc.Width(), btn_h);
	m_pSeqn->SetWindowPos(NULL, rc.left, pos_y, rc.Width(), btn_h+200, SWP_NOMOVE|SWP_NOZORDER);

	m_rcTotal.SetRect(rc.left, pos_y-btn_h, rc.right, pos_y);	//2016.08.26 KSJ 

	m_rcDesc.SetRect(rc.left, pos_y+btn_h+PADDING_VALUE/2, rc.right, rc.bottom-btn_h-PADDING_VALUE/2);

	m_pSend->MoveWindow(rc.left, rc.bottom-btn_h, rc.Width(), btn_h);
}

void CTujaja::LoadHJCode()
{
	m_hnamMap.RemoveAll();

	CString fnam;
	fnam.Format("%s\\tab\\hjcode3.dat", Variant(homeCC, ""));

	FILE *fp = fopen(fnam, "rb");
	if (!fp) return;
	fseek(fp, 0, SEEK_END);
	
	int pos = 0;
	const long fsiz = ftell(fp);

	std::unique_ptr<char[]> buff = std::make_unique<char[]>(fsiz);
	
	fseek(fp, 0, SEEK_SET);
	while(pos<fsiz)
	{
		pos += fread(buff.get()+pos, 1, fsiz-pos, fp);
	}
	fclose(fp);

	const int nrec = fsiz/sizeof(struct hjcodex);
	struct hjcodex *hjcode = (struct hjcodex*)buff.get();
	for(int n=0; n<nrec; ++n, ++hjcode)
	{
		if (hjcode->code[0]!='A') continue;

		CString code(hjcode->code, sizeof(hjcode->code));
		CString hnam(&hjcode->hnam[1], sizeof(hjcode->hnam)-1);
		code.Remove(0); code.TrimRight();
		hnam.Remove(0); hnam.TrimRight();
		m_hnamMap.SetAt(hnam, code);
	}
	
}

void CTujaja::OnParse()
{
	m_pResult->SetRowCount(1);
	m_strTotal.Empty();

	CString strTemp, strLine;
	char buff[1024*640];		//2015.04.14 KSJ 사이즈 640k 세팅
	int dlen = m_pInput->GetWindowText(buff, sizeof(buff)-1);
	buff[dlen] = 0;

	testSaveFile("분석Start", buff, dlen);	//2015.04.13

	vector<StringProxy> lines;
	ParseString(buff, buff+dlen, '\n', lines);

	CString tjcd, code, msgb, tjnm, fnam;
	//int nBlock = 0;		//2016.08.29 길이가 짧을때 처음 블록사이즈
	int nTotal=0, nMsCount=0, nMdCount=0, nError=0;	//총갯수, 매수, 매도 갯수

	fnam.Format("%s\\tab\\SJB.INI", Variant(homeCC, ""));

	const int hnam_w = GetPrivateProfileInt("TJJ", "NAME_W", 18, fnam);
	const int code_w = GetPrivateProfileInt("TJJ", "CODE_W",  6, fnam);

	for(size_t n=0; n<lines.size(); ++n)
	{
		CString line = lines[n].ToCStr();
		line.Remove(0);
		line.Remove('\r');
		line.TrimLeft();
		line.TrimRight();

		//OutputDebugString("[" + line + "]\n");

		if (line.Left(2)=="--")
		{
			//OutputDebugString("parse category\n");
			POSITION pos = m_catgMap.GetStartPosition();
			while(pos!=NULL)
			{
				//2015.04.14 KSJ ,가 있으면 잘라서 비교한다. 
				//'--보험,기타금융기관 매도' 와 같이 데이터가 들어온다.
				CString sTjnm;
				sTjnm = line.Find(",") != -1 ? line.Left(line.Find(",")) : line;

				m_catgMap.GetNextAssoc(pos, tjnm, tjcd);
				if (sTjnm.Find(tjnm)!=-1) break;
				tjcd.Empty();
			}
			
			if (tjcd.IsEmpty()) tjnm = "";

			if      (line.Find("매도")!=-1) msgb = "매도";
			else if (line.Find("매수")!=-1) msgb = "매수";
			else                            msgb = "";

			if (line.Find("순 매수")!=-1) msgb = "";
			if (line.Find("순 매도")!=-1) msgb = "";
		}
		else
		{
			while( line.GetLength()>hnam_w )
			{
				line.TrimLeft();	//2016.08.09 KSJ 종목명이 18자리를 넘어갈 경우가 발생하므로 왼쪽 스페이스를 없애준다.
				if (msgb=="" || msgb.IsEmpty()) break;
				if (tjcd=="" || tjcd.IsEmpty()) break;

				int nLine = line.Find(" ");	//2016.08.09 KSJ line길이가 24자릿수를 넘어 갈수도 있으므로 처음에 잘 잘라준다.
				
				if(nLine > 24)			//2016.08.09 KSJ 처음이나 중간에 있을땐 스페이스가 있다.
				{
					nLine = nLine - (hnam_w + code_w);
				}
				else if(nLine == -1)	//2016.08.09 KSJ 마지막에 있을땐 스페이스가 없다.
				{
					nLine = line.GetLength() - (hnam_w + code_w);
				}
				else
					nLine = 0;

				CString block = line.Left(hnam_w+code_w+nLine);	//2016.08.09 KSJ nLine 만큼
				block.TrimLeft();
				block.TrimRight();
				//OutputDebugString("BLOCK[" + block + "]\n");

				CString code, hnam, damt, snum;
				int npos = block.ReverseFind(' ');
				if (npos!=-1)
				{
					hnam = block.Left(npos);
					damt = block.Mid(npos, hnam_w+code_w-npos+nLine);	//2016.08.09 KSJ nLine 만큼
		
					hnam.TrimRight();

					//2016.08.29 KSJ 만약 자릿수가 일반보다 더 작을때는 종목명안에 스페이스가 또 들어가있다.
					//종목명안에 스페이스가 있는 경우가 있다..(포스코 ICT) 뒤에껄 구분해서 숫자면 자릿수가 작은거고
					//문자면 그냥 종목명이다.
					npos = hnam.ReverseFind(' ');
					
					if(npos != -1)
					{
						CString strData;
						block = hnam;
						strData = block.Mid(npos, block.GetLength()- npos);
						strData.TrimRight(); strData.TrimLeft();
						
						//숫자일땐 수량과 종목명
						if(atof(strData) > 0 || !strData.Compare("0.0") || !strData.Compare(".0") || !strData.Compare("0.") || !strData.Compare("0"))
						{
							nLine = damt.GetLength() * -1;
							
							block = hnam;
							hnam = block.Left(npos);
							damt = block.Mid(npos, block.GetLength()- npos);	//2016.08.09 KSJ nLine 만큼
						}
						else	//숫자가 아닐떈 실제로 종목명에 스페이스들어간 경우
						{

						}
					}
				}
				else	//2016.08.09 KSJ 종목명이 길어지면  ' '자리가 없어진다. 그러므로 '.'을 기준으로 앞에 숫자까지가 종목명으로 판단함.
				{
					npos = line.Find(".");

					for(int n=npos-1; n > 0; --n)
					{
						if(!isNum(block.GetAt(n)))
						{
							npos = n+1;
							break;
						}
					}

					hnam = block.Left(npos);
					damt = block.Mid(npos, block.GetLength() - npos + 1);
					//OutputDebugString("XXX->" + hnam + "\n");
				}

				hnam.TrimRight();
				damt.TrimRight();
				
				if(hnam.GetLength() > 0)	//2016.09.02 KSJ 종목명이 없으면 검사하지 않는다.
				{
					if (!m_hnamMap.Lookup(hnam, code))
					{
						CString key, val;
						POSITION pos = m_hnamMap.GetStartPosition();
						while(pos)
						{
							m_hnamMap.GetNextAssoc(pos, key, val);
							if (key.GetLength()>hnam.GetLength())
							{
								if (key.Left(hnam.GetLength()) == hnam)
								{
									hnam = key;
									code = val;
									break;
								}
							}
						}
					}
					//OutputDebugString("[" + code + "]\n");
					if (code.GetLength()==7)
					{
						const int row = m_pResult->InsertRow("");
						COLORREF fcol = m_crBlack;
						if(msgb=="매도")
						{
							nMdCount++;
							fcol = m_crMinus;
						}
						else if (msgb=="매수")
						{
							nMsCount++;
							fcol = m_crPlus;
						}
						
						nTotal++;

						m_pResult->SetItemText(row, 0, tjnm); m_pResult->SetItemFormat(row, 0, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
						m_pResult->SetItemText(row, 1, tjcd); m_pResult->SetItemFormat(row, 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
						m_pResult->SetItemText(row, 2, hnam); m_pResult->SetItemFormat(row, 2, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
						m_pResult->SetItemText(row, 3, code); m_pResult->SetItemFormat(row, 3, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
						m_pResult->SetItemText(row, 4, msgb); m_pResult->SetItemFormat(row, 4, DT_CENTER|DT_VCENTER|DT_SINGLELINE); m_pResult->SetItemFgColour(row, 4, fcol);
						m_pResult->SetItemText(row, 5, damt); m_pResult->SetItemFormat(row, 5, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);  m_pResult->SetItemFgColour(row, 5, fcol);

						//2016.08.26 KSJ 수량이 0이면 색깔로 보여주기
						if(atof(damt) == 0)
						{
							m_pResult->SetItemBkColour(row, 0, m_crFocus);
							m_pResult->SetItemBkColour(row, 1, m_crFocus);
							m_pResult->SetItemBkColour(row, 2, m_crFocus);
							m_pResult->SetItemBkColour(row, 3, m_crFocus);
							m_pResult->SetItemBkColour(row, 4, m_crFocus);
							m_pResult->SetItemBkColour(row, 5, m_crFocus);

							nError++;
						}

						//const char *heads[] = {"투자자", "투자자코드", "종목명", "종목코드", "매도/매수", "수량"};
						CString heads[] = { "투자자", "투자자코드", "종목명", "종목코드", "매도/매수", "수량" };

						strLine.Format("[%s][%s][%s][%s][%s][%s]\n", tjnm,tjcd,hnam,code,msgb,damt);
						strTemp += strLine;
					}
					else
					{
						MessageBox("종목코드를 찾을 수 없습니다.\n[" + block + "]");

						nError++;
					}
				}
				else	//2016.09.02 KSJ 종목명이 없으면!!
				{
					MessageBox("종목명을 찾을 수 없습니다.\n[" + block + "]");

					nError++;
				}
				line.Delete(0, min(line.GetLength(),hnam_w+code_w+nLine));	//2016.08.09 KSJ nLine 만큼
			}
		}
	}

	m_pResult->Refresh();

	//2016.08.26 KSJ 총건수 표시
	m_strTotal.Format("[총 건수: %d]    [매도: %d]    [매수: %d]", nTotal, nMdCount, nMsCount);

	CDC * dc = GetDC();
	dc->FillSolidRect(&m_rcTotal, m_crBodyBk);
		
	CFont *pOldFont = dc->SelectObject(m_pFont);
	dc->DrawText(m_strTotal, m_rcTotal, DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS);
	dc->SelectObject(pOldFont);

	if(nError > 0)	
	{
		CString strMsg;
		strMsg.Format("이상종목 [%d]개가 발생하였습니다. 확인하시기 바랍니다.", nError);
		MessageBox(strMsg, "IBK투자증권", MB_OK);
	}
	//2016.08.26 KSJ 총건수 표시 End
	
	//2015.04.13 KSJ
	testSaveFile("GRID DATA", strTemp, strTemp.GetLength());
}

void CTujaja::LoadCategory()
{
	m_catgMap.RemoveAll();

	CString keyv, fnam, valu;
	const DWORD buff_size = 1024;
	std::unique_ptr<char[]> buff = std::make_unique<char[]>(buff_size);

	DWORD dlen=0;

	fnam.Format("%s\\tab\\SJB.INI", Variant(homeCC, ""));

	for(int n=0; n<99; ++n)
	{
		keyv.Format("%d", n);
		dlen = GetPrivateProfileString("CATEGORY", keyv, "", buff.get(), buff_size-1, fnam);
		
		if (dlen>0)
		{
			vector<StringProxy> lines;
			ParseString(buff.get(), buff.get()+dlen+1, ',', lines);
			for(size_t i=0; i<lines.size(); ++i)
			{
				CString tval = lines[i].ToCStr();
				tval.TrimLeft();
				tval.TrimRight();
				m_catgMap.SetAt(tval, keyv);
			}
		}
	}
}

void CTujaja::OnSend()
{
	if (m_pSeqn->GetCurSel()==-1)
	{
		MessageBox("차수를 선택하시기 바랍니다.", "IBK투자증권", MB_OK);
		return;
	}

	m_tuja_queue.clear();

	CString strTemp;
	const DWORD buff_size = 2048;

	std::unique_ptr<char[]> buff = std::make_unique<char[]>(buff_size);
	char tmp[128]{}, seqn[12]{};
	int scnt=0, row=0, dlen=0, mult=0;
	const int nrec = m_pResult->GetRowCount()-1;
	pidotuja_mid *mid = (pidotuja_mid *)buff.get();

	CString strTotal;
	int nTotal=0, nMsCount=0, nMdCount=0;	//총갯수, 매수, 매도 갯수

	CString fnam;
	fnam.Format("%s/tab/SJB.INI", Variant(homeCC,""));
	mult = GetPrivateProfileInt("TJJ", "MULT", 1000, fnam);

	sprintf(seqn, "%d", m_pSeqn->GetCurSel());

	FillMemory(buff.get(), buff_size, ' ');
	mid->flag[0] = 'I';
	CopyMemory(mid->seqn, seqn, strlen(seqn));
	
	for(scnt=1, row=1; row<=nrec; ++row, ++scnt)
	{
		pidotuja_grid *grid = &mid->grid[scnt-1];
		CString xvol, tjcd, code, msgb;
		tjcd = m_pResult->GetItemText(row, 1); tjcd.TrimRight();
		code = m_pResult->GetItemText(row, 3); code.TrimRight();
		msgb = m_pResult->GetItemText(row, 4); msgb.TrimRight();
		
		//2016.08.26 KSJ 색깔 초기화
		m_pResult->SetItemBkColour(row, 0, m_crWhite);
		m_pResult->SetItemBkColour(row, 1, m_crWhite);
		m_pResult->SetItemBkColour(row, 2, m_crWhite);
		m_pResult->SetItemBkColour(row, 3, m_crWhite);
		m_pResult->SetItemBkColour(row, 4, m_crWhite);
		m_pResult->SetItemBkColour(row, 5, m_crWhite);
		
		if (tjcd.GetLength()==0 || code.GetLength()==0 || msgb.GetLength()==0)
		{
			--scnt;
			continue;
		}

		xvol.Format("%d", (int)(atof(m_pResult->GetItemText(row, 5))*mult));
	
		memcpy(grid->cod2, code, code.GetLength());
		if (msgb=="매도")
		{
			nMdCount++;
			grid->msgb[0] = '1';
		}
		else if	(msgb=="매수")
		{
			nMsCount++;
			grid->msgb[0] = '2';
		}
		else    continue;

		nTotal++;

		//2016.08.26 KSJ 수량이 0일때는 어디줄인지 알려준다.
		/*
		if(!xvol.Compare("0"))
		{
			m_pResult->SetItemBkColour(row, 0, m_crFocus);
			m_pResult->SetItemBkColour(row, 1, m_crFocus);
			m_pResult->SetItemBkColour(row, 2, m_crFocus);
			m_pResult->SetItemBkColour(row, 3, m_crFocus);
			m_pResult->SetItemBkColour(row, 4, m_crFocus);
			m_pResult->SetItemBkColour(row, 5, m_crFocus);

			m_pResult->SetScrollPos(SB_VERT, (row-2) * 18);
			if(row ==1)		m_pResult->SendMessage(WM_VSCROLL, SB_TOP, 0);
			else			m_pResult->SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);

			m_pResult->Invalidate();

			CString strMsg;
			strMsg += "전송실패!!\n\n";
			strMsg += "[" + m_pResult->GetItemText(row, 0) + "]  ";
			strMsg += "[" + m_pResult->GetItemText(row, 1) + "]  ";
			strMsg += "[" + m_pResult->GetItemText(row, 2) + "]  ";
			strMsg += "[" + m_pResult->GetItemText(row, 3) + "]  ";
			strMsg += "[" + m_pResult->GetItemText(row, 4) + "]  ";
			strMsg += "[" + m_pResult->GetItemText(row, 5) + "]";
			strMsg += "\n\n데이터를 다시 확인 바랍니다.";

			MessageBox(strMsg, "IBK투자증권", MB_OK);
			return;
		}
		*/

		memcpy(grid->tjcd, tjcd, tjcd.GetLength());
		memcpy(grid->xvol, xvol, xvol.GetLength());
		
		if (scnt==MAX_PIDOTUJA)
		{
			sprintf(tmp, "%d", scnt);
			memcpy(mid->nrec, tmp, strlen(tmp));
			dlen = sizeof(pidotuja_mid) + ((scnt-1)*sizeof(pidotuja_grid));
			// escdream check 
			m_tuja_queue.push_back( vector<char>(buff.get(), buff.get()+dlen) );
			scnt = 0;

			FillMemory(buff.get(), buff_size, ' ');
			mid->flag[0] = 'I';
			CopyMemory(mid->seqn, seqn, strlen(seqn));
		}
	}
	if (scnt>1)
	{
		sprintf(tmp, "%d", scnt);
		memcpy(mid->nrec, tmp, strlen(tmp));
		dlen = sizeof(pidotuja_mid) + ((scnt-1)*sizeof(pidotuja_grid));
		m_tuja_queue.push_back( vector<char>(buff.get(), buff.get()+dlen) );
	}

	if (!m_tuja_queue.empty())
	{
		//2015.05.13 SKJ
		for(size_t i = 0; i < m_tuja_queue.size(); i++)
		{
			strTemp += CString(&m_tuja_queue[i][0], m_tuja_queue[i].size());
			strTemp += "\n";
		}
		testSaveFile("전송End", strTemp, strTemp.GetLength());

		m_pResult->Invalidate();
		
		//2016.08.26 KSJ 주의사항 추가
		CString strMsg, strData;	
		strMsg.Format("");
		strMsg += "[주의]\n\n전송 시 실시간으로 투자자 데이터가 반영됩니다.\n";
		strData.Format("\n[차수: %d차]  [총 건수: %d]  [매도: %d]  [매수: %d]\n", m_pSeqn->GetCurSel()+1,nTotal, nMdCount, nMsCount);
		strMsg += strData;
		strMsg += "\n투자자 데이터를 정말 전송하시겠습니까?";

		if(MessageBox(strMsg, "IBK투자증권", MB_YESNO|MB_ICONWARNING) == IDYES)
		{
			SendTR("pidotuja", 0, &m_tuja_queue[0][0], m_tuja_queue[0].size(), TK_PIDOTUJA );
			m_tuja_queue.erase(m_tuja_queue.begin());
		}
		//2016.08.26 KSJ 주의사항 추가 End
	}
}

LRESULT CTujaja::OnUser( WPARAM wParam, LPARAM lParam )
{
	const int msg = LOBYTE(LOWORD(wParam));
	const int key = HIBYTE(LOWORD(wParam));
	const int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;
	
	switch(msg)
	{
	case DLL_INB:
		break;
		
	case DLL_OUB:
		switch(key)
		{
		case TK_PIDOTUJA: ResponsePIDOTUJA(dat, len); break;
		}
		break;
		
		case DLL_ALERT:
			break;
	}
	return 0;
}

BOOL CTujaja::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	MSGFILTER * lpMsgFilter = (MSGFILTER *)lParam; 
	if ((wParam == IDC_INPUT) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER) && (lpMsgFilter->msg == WM_RBUTTONDOWN))
	{
		CPoint point;                                            
		::GetCursorPos(&point); //where is the mouse?
		CMenu menu; //lets display out context menu :) 
		DWORD dwSelectionMade=0;                                       
		VERIFY(menu.LoadMenu(IDR_MENU1) );
		CMenu *pmenuPopup = menu.GetSubMenu(0);
		ASSERT(pmenuPopup != NULL);                                       
		dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD), point.x, point.y, this);
		pmenuPopup->DestroyMenu(); 
			
		if(dwSelectionMade == ID_EDIT_COPY)				m_pInput->Copy();
		else if(dwSelectionMade == ID_EDIT_CUT)			m_pInput->Cut();
		else if(dwSelectionMade == ID_EDIT_PASTE)		m_pInput->Paste();
		else if(dwSelectionMade == ID_EDIT_SELECT_ALL)	m_pInput->SetSel(0, -1);
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}


void CTujaja::ResponsePIDOTUJA( LPCSTR dat, int len )
{
	if (!m_tuja_queue.empty())
	{
		SendTR("pidotuja", 0, &m_tuja_queue[0][0], m_tuja_queue[0].size(), TK_PIDOTUJA );
		m_tuja_queue.erase(m_tuja_queue.begin());
	}
	else
	{
		MessageBox("전송완료", "IBK투자증권", MB_OK);
	}
}

void CTujaja::testSaveFile(CString strName, LPCSTR datB, int size)
{
	//2015.04.13 KSJ
#ifndef SAVELOGFILE
	return;
#endif

	CString strPath(_T("")), titletime;
	strPath.Format("%s\\IB8770_Log.dat", Variant(homeCC,""));
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s][%s]\n", strCurTime, strName);
	file.Write(titletime, titletime.GetLength());
	file.SeekToEnd(); 	
	
	file.Write(datB, size);
	file.WriteString("\n\n");
	
	file.Close();	
}

bool CTujaja::isNum(char c)
{
	char *pnum = "0123456789";
	
	for(int i=0; i< int(strlen(pnum)) ; i++)
	{
		if(c == pnum[i]) return true;
	}
		
	return false;
}
