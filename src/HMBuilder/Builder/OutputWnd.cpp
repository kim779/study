
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "h/mainvar.h"
#include "h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_TRC_RICHEDIT	200

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
	m_btextpos=false;
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER, OnTrace)
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style &= ~WS_CAPTION;
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_edit.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_TABSTOP|ES_LEFT|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN,   CRect(0, 0, 0, 0), this, ID_TRC_RICHEDIT))
		  return -1;

/*	CWnd*	pWizard = (CWnd *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETWIZARD);
	if (pWizard)
	{
		LONG	ret;
		pWizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I8, (void*)&ret, (BYTE*)(VTS_I4 VTS_I4), MAKEWPARAM(setTRACE, WM_USER), (long)this->m_hWnd);
	}
	*/

	// font
	::DeleteObject(m_font.Detach());

	LOGFONT	lf;
	NONCLIENTMETRICS info;

	afxGlobalData.fontRegular.GetLogFont(&lf);

	info.cbSize = sizeof(info);
	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_font.CreateFontIndirect(&lf);

	m_edit.SetFont(&m_font);
	m_edit.SetReadOnly(true);

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 전체 클라이언트 영역을 처리해야 합니다.
	if (IsWindow(m_edit.m_hWnd))
		m_edit.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT COutputWnd::OnTrace(WPARAM wParam, LPARAM lParam)
{
	char*	pStr = (char *)lParam;
	switch (HIWORD(wParam))
	{
	case x_RCVs:
	case x_SNDs:
	//	if (!m_edit.m_bSNDRCV)
	//		break;
		{
			CString sText = _T("");
			bool	bHanFlag = false;
			int	nMaxChar = 20;

			if (HIWORD(wParam) == x_SNDs)
				sText.Format(_T("SND DATAs [%d Bytes]\n"), (int)LOWORD(wParam));
			else
				sText.Format(_T("RCV DATAs [%d Bytes]\n"), (int)LOWORD(wParam));

			AddText(sText);
			if (!LOWORD(wParam))
			{
				AddText(_T("\n"));
				return 0;
			}
			sText = _T("");
			
			if (!m_edit.m_bMode)
				nMaxChar = 80;

			for (int nRow = 0; nRow < (int)LOWORD(wParam) / nMaxChar + 1; nRow++)
			{
				CString sRow = "", sDat = " ";
				sRow.Format("%06d : ", nRow * nMaxChar);
				
				for (int ii = nRow * nMaxChar; ii < (nRow + 1) * nMaxChar; ii++)
				{
					if (ii < (int)LOWORD(wParam))
					{
						CString sTmp;
						sTmp.Format("%02x ", (unsigned char)pStr[ii]);
						sRow += sTmp;
					}
					else
						sRow += "   ";
					
					if (ii == nRow * nMaxChar + 9)
						sRow += " ";
					
					if (ii < (int)LOWORD(wParam))
					{
						char cStr = pStr[ii];
						if (cStr)
						{
							switch(cStr)
							{
							case 0x0a:
							case 0x09:
							case 0x08:
								cStr = ' ';
								break;
							}
							
							if (bHanFlag && ii == nRow * nMaxChar && cStr < 0)
								sDat = " ";
							else
								sDat += cStr;
							if (cStr < 0)
								bHanFlag = !bHanFlag;
							if (ii == (nRow + 1) * nMaxChar - 1 && bHanFlag && ii + 1 < (int)LOWORD(wParam))
							{
								sDat += pStr[ii + 1];
								bHanFlag = true;
							}
						}
						else
							sDat += (char)0x7f;
					}
					else
						sDat += " ";
				}
				sDat  += '\n';
				if (m_edit.m_bMode)
					sRow  += sDat;
				else
					sRow = sDat;
				sText += sRow;
			}
			AddText(sText);
		}
		break;

	case x_LUAs:
		{
			CString strOri = CString(pStr);
			CString sText = _T("");
			
			if (strOri.Find("(SYSTEM)") > -1)
			{				
				AddText(strOri.Mid(8), RGB(23,123,53));
				break;
			}			
			sText.Format(_T("Lua Script Error : %s\n"), strOri);
			AddText(sText, RGB(255,0,0));
		}
		break;

	case x_RTMs:
	case x_STRs:
		if (HIWORD(wParam) == x_RTMs && !m_edit.m_bRTM)
			break;
		if (HIWORD(wParam) == x_RTMs)
		{
			CString sText, sRTM = "";
			sText.Format("RTM DATAs [%d Bytes]\n", strlen(&pStr[0]));
			AddText(sText);
			sText = pStr;
			sRTM.Format("[%-12s] ", parse(sText, "\t"));
			while(!sText.IsEmpty())
			{
				sRTM += parse(sText, "\t") + "|";
				sRTM += parse(sText, "\t") + " ";
			}
			AddText(sRTM + "\n");
		}
		else
		{
			if (CString(pStr).Find(_T("(ERROR)")) > -1)
			{
				AddText(pStr, RGB(255,0,0));
				break;
			}

			AddText(pStr);
			AddText("\n");
		}
		break;
	}
	return 0;
}

void COutputWnd::AddText(CString txt, COLORREF txtColor)
{
	CHARFORMAT cf;
	m_edit.GetSelectionCharFormat(cf); 
	// change the color and set it to the selected text
	cf.crTextColor = txtColor;
	cf.dwEffects = cf.dwEffects & ~CFE_AUTOCOLOR;
	m_edit.SetSelectionCharFormat(cf);


	m_edit.SetSel(-1, -1);
	m_edit.ReplaceSel(txt.operator const char*());
	m_edit.SendMessage(WM_VSCROLL, SB_BOTTOM);
	m_btextpos = false;
}

void COutputWnd::FixedAddText(CString txt)
{
	m_edit.SetSel(-1, -1);
	if (!m_btextpos)
	{
		m_edit.GetSel(m_ltextpos,m_ltextpos);
		m_btextpos = !m_btextpos;
	}
	m_edit.SetSel(m_ltextpos, -1);
	m_edit.ReplaceSel(txt.operator const char*());
	m_edit.LineScroll(m_edit.GetLineCount() - m_edit.GetFirstVisibleLine());
}

void COutputWnd::BarTextClear()
{
	m_edit.Clear();
}

CString COutputWnd::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);
	
	if (pos == -1)
	{
		tmps = src;
		src  = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src  = src.Mid(pos + sub.GetLength());
	return tmps;
}
