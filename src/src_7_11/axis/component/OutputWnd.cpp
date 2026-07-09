// OutputWnd.cpp : implementation file
//

#include "stdafx.h"
#include "../axis.h"
#include "OutputWnd.h"


// COutputWnd
#define	ID_TRC_RICHEDIT	200

IMPLEMENT_DYNAMIC(COutputWnd, CSizingControlBarG)

COutputWnd::COutputWnd()
{
	m_btextpos=false;
	m_font = NULL;
}

COutputWnd::~COutputWnd()
{

}

BEGIN_MESSAGE_MAP(COutputWnd, CSizingControlBarG)
	ON_MESSAGE(WM_USER, OnTrace)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

LRESULT COutputWnd::OnTrace(WPARAM wParam, LPARAM lParam)
{
	char*	pStr = (char *)lParam;

	int itype =LOWORD(wParam);
	char* pdata = (char*)lParam;
			
	CString strOri = CString(pdata);
	CString sText = _T("");
		
	sText.Format(_T("info: %s\n"), strOri);
	AddText(sText, RGB(255,0,0));

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
	m_edit.ReplaceSel(txt);
	m_edit.SendMessage(WM_VSCROLL, SB_BOTTOM);
	m_btextpos = false;
}


// COutputWnd message handlers

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_edit.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_TABSTOP|ES_LEFT|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN,   CRect(0, 0, 0, 0), this, ID_TRC_RICHEDIT))
		  return -1;

	/*
	// font
	::DeleteObject(m_font.Detach());

	LOGFONT	lf;
	NONCLIENTMETRICS info;

	afxGlobalData.fontRegular.GetLogFont(&lf);

	info.cbSize = sizeof(info);
	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	//lf.lfItalic = info.lfMenuFont.lfItalic;

	m_font.CreateFontIndirect(&lf);
	
	
	*/
	if(m_font != NULL)
		m_edit.SetFont(m_font);

	m_edit.SetReadOnly(true);

	return 0;
}


void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CSizingControlBarG::OnSize(nType, cx, cy);

	if (IsWindow(m_edit.m_hWnd))
		m_edit.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
