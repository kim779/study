// TraceBar.cpp: implementation of the CTraceBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axiswork.h"
#include "mainvar.h"
#include "TraceBar.h"
#include "../h/axisfire.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraceBar::CTraceBar()
{
	m_btextpos=false;
}

CTraceBar::~CTraceBar()
{

}

BEGIN_MESSAGE_MAP(CTraceBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CTraceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnTrace)
END_MESSAGE_MAP()

int CTraceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_resultEdit.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_TABSTOP|
		          ES_LEFT|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN, 
			  CRect(0, 0, 0, 0), this, ID_TRC_RICHEDIT))
			  return -1;

	m_pWizard = (CWnd*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETWIZARD);
	if (m_pWizard)
	{
		long ret;
		m_pWizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void*)&ret, (BYTE*)(VTS_I4 VTS_I4), MAKEWPARAM(setTRACE, WM_USER), (LPARAM)this->m_hWnd);
	}

	if (!m_font.CreatePointFont(100, "FixedSys"))
		if (!m_font.CreatePointFont(80, "±¼¸²Ã¼"))
			if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
				return -1;

	m_resultEdit.SetFont(&m_font);
	m_resultEdit.SetReadOnly(true);

	return 0;
}

void CTraceBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(rc);
	if (IsWindow(m_resultEdit.m_hWnd))
		m_resultEdit.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE);
}

BOOL CTraceBar::PreTranslateMessage(MSG* pMsg) 
{
	return CSizingControlBarG::PreTranslateMessage(pMsg);
}

LRESULT CTraceBar::OnTrace(WPARAM wParam, LPARAM lParam)
{
	char *pStr = (char*)lParam;
	switch(HIWORD(wParam))
	{
	case x_RCVs:
	case x_SNDs:
		if (!m_resultEdit.m_bSNDRCV)
			break;
		{
			CString sText = _T("");
			bool	bHanFlag = false;
			int	nMaxChar = 20;

			if (HIWORD(wParam) == x_SNDs)
				sText.Format("SND DATAs [%d Bytes]\n", (int)LOWORD(wParam));
			else
				sText.Format("RCV DATAs [%d Bytes]\n", (int)LOWORD(wParam));

			AddText(sText);
			if (!LOWORD(wParam))
			{
				AddText("\n");
				return 0;
			}
			sText = _T("");
			
			if (!m_resultEdit.m_bMode)
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
				if (m_resultEdit.m_bMode)
					sRow  += sDat;
				else
					sRow = sDat;
				sText += sRow;
			}
			AddText(sText);
		}
		break;
	case x_RTMs:
	case x_STRs:
		if (HIWORD(wParam) == x_RTMs && !m_resultEdit.m_bRTM)
			break;
		if (HIWORD(wParam) == x_RTMs)
		{
			CString sText, sRTM = "";
			sText.Format("RTM DATAs [%d Bytes]\n", strlen(&pStr[0]));
			AddText(sText);
			sText = pStr;
			sRTM.Format("[%-12s] ", Parser(sText, "\t"));
			while(!sText.IsEmpty())
			{
				sRTM += Parser(sText, "\t") + "|";
				sRTM += Parser(sText, "\t") + " ";
			}
			AddText(sRTM + "\n");
		}
		else
		{
			AddText(pStr);
			AddText("\n");
		}
		break;
	}
	return 0;
}

void CTraceBar::AddText(CString txt)
{
	m_resultEdit.SetSel(-1, -1);
	m_resultEdit.ReplaceSel(txt.operator const char*());
	m_resultEdit.SendMessage(WM_VSCROLL, SB_BOTTOM);
	m_btextpos = false;
}

void CTraceBar::FixedAddText(CString txt)
{
	m_resultEdit.SetSel(-1, -1);
	if (!m_btextpos)
	{
		m_resultEdit.GetSel(m_ltextpos,m_ltextpos);
		m_btextpos = !m_btextpos;
	}
	m_resultEdit.SetSel(m_ltextpos, -1);
	m_resultEdit.ReplaceSel(txt.operator const char*());
	m_resultEdit.LineScroll(m_resultEdit.GetLineCount() - m_resultEdit.GetFirstVisibleLine());
}

void CTraceBar::BarTextClear()
{
	m_resultEdit.Clear();
}

CString CTraceBar::Parser(CString &srcstr, CString substr)
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
