// SystemTradingView.cpp : implementation of the CSystemTradingView class
//

#include "stdafx.h"
#include "SystemTrading.h"

#include "SystemTradingDoc.h"
#include "SystemTradingView.h"
#include "../Include_ST/DeclareDefine.h"
#include "editcmd.h"
#include "IMM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingView

IMPLEMENT_DYNCREATE(CSystemTradingView, CCrystalEditView)

BEGIN_MESSAGE_MAP(CSystemTradingView, CCrystalEditView)
	//{{AFX_MSG_MAP(CSystemTradingView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_EX, OnUpdateEditFind)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CCrystalEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CCrystalEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CCrystalEditView::OnFilePrintPreview)

	ON_XTP_EXECUTE(ID_EDIT_FIND_EX, OnEditFind)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingView construction/destruction

CSystemTradingView::CSystemTradingView() :
	m_bRButtonDown(FALSE),
	m_bExecutionExist(FALSE)
{
}

CSystemTradingView::~CSystemTradingView()
{
}

BOOL CSystemTradingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CCrystalEditView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingView printing

BOOL CSystemTradingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSystemTradingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSystemTradingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingView diagnostics

#ifdef _DEBUG
void CSystemTradingView::AssertValid() const
{
	CCrystalEditView::AssertValid();
}

void CSystemTradingView::Dump(CDumpContext& dc) const
{
	CCrystalEditView::Dump(dc);
}

CSystemTradingDoc* CSystemTradingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSystemTradingDoc)));
	return (CSystemTradingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingView message handlers
void CSystemTradingView::GetDefualt()
{
	m_clrBackground = RGB(255, 255, 255);
	m_clrComment = RGB(0, 192, 0);
	m_clrNotCertFunc = RGB(255, 0, 0);
	m_clrNormalText = RGB(0, 0, 0);
	m_clrNumber = RGB(128, 0, 0);
	m_clrOperator = RGB(0, 0, 0);
	m_clrKeyword = RGB(0, 0, 255);
	m_clrSelBackground = RGB(49, 106, 197);
	m_clrSelText = RGB(255, 255, 255);
	m_clrUserFunction = RGB(0, 128, 192);
}

void CSystemTradingView::GetDefualt_YES()
{
	m_clrBackground = RGB(255, 247, 238);
	m_clrComment = RGB(0, 192, 0);
	m_clrNotCertFunc = RGB(255, 0, 0);
	m_clrNormalText = RGB(0, 0, 0);
	m_clrNumber = RGB(128, 0, 0);
	m_clrOperator = RGB(0, 0, 0);
	m_clrKeyword = RGB(0, 0, 255);
	m_clrSelBackground = RGB(49, 106, 197);
	m_clrSelText = RGB(255, 255, 255);
	m_clrUserFunction = RGB(0, 128, 192);
}

void CSystemTradingView::OnInitialUpdate() 
{
	CCrystalEditView::OnInitialUpdate();

	SetSystemTradingColor();
}

void CSystemTradingView::SetSystemTradingColor()
{
	UINT nScriptType = GetDocument()->GetScriptType();
	char szSection[20];

	if(nScriptType == ST_YESTRADER)
	{
		::lstrcpy(szSection, _T("YESTRADER"));
		GetDefualt_YES();
	}
	else
	{
		::lstrcpy(szSection, _T("TRADESTATION"));
		GetDefualt();
	}

	m_clrBackground = ::GetProfileInt(szSection, _T("BackGround"), m_clrBackground);
	m_clrNormalText = ::GetProfileInt(szSection, _T("NormalText"), m_clrNormalText);
	m_clrComment = ::GetProfileInt(szSection, _T("Comment"), m_clrComment);
	m_clrOperator = ::GetProfileInt(szSection, _T("Operator"), m_clrOperator);
	m_clrNumber = ::GetProfileInt(szSection, _T("Number"), m_clrNumber);
	m_clrKeyword = ::GetProfileInt(szSection, _T("ReservedText"), m_clrKeyword);
	m_clrSelBackground = ::GetProfileInt(szSection, _T("SelBkGrnd"), m_clrSelBackground);
	m_clrSelText = ::GetProfileInt(szSection, _T("SelText"), m_clrSelText);
	m_clrUserFunction = ::GetProfileInt(szSection, _T("UserFunction"), m_clrUserFunction);
	m_clrNotCertFunc = ::GetProfileInt(szSection, _T("NoCertFunc"), m_clrNotCertFunc);

	SetFont(GetDocument()->m_lf);	
}

CCrystalTextBuffer *CSystemTradingView::LocateTextBuffer()
{
	return &GetDocument()->m_xTextBuffer;
}

DWORD CSystemTradingView::ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems)
{
	UINT nScriptType = GetDocument()->GetScriptType();
	if(nScriptType == ST_TRADESTATION)
		return ParseLineSTS(dwCookie, nLineIndex, pBuf, nActualItems);
	else if(nScriptType == ST_YESTRADER)
		return ParseLineYES(dwCookie, nLineIndex, pBuf, nActualItems);

	return 0;
}

LRESULT CSystemTradingView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
    case WM_IME_CHAR:  // 여기서 처리 하지 말고.. 
        return 0;
    case WM_IME_COMPOSITION:
        if(lParam == GCS_RESULTSTR)
        {
            TCHAR szText[256];
            ZeroMemory(szText, sizeof(szText));
            HIMC hiNow = ImmGetContext(this->m_hWnd);
            LONG lResult = ImmGetCompositionString(hiNow, GCS_RESULTSTR, szText, sizeof(szText));
            if ((lResult == IMM_ERROR_NODATA) || (lResult == IMM_ERROR_GENERAL)) 
                break;

            //SendMessage( EM_SETSEL, 0, -1 );
            //SendMessage( EM_SETSEL, -1, 0 );
            SendMessage( EM_REPLACESEL, FALSE, (LPARAM)szText );
        }
        break;
    }
	
	return CCrystalEditView::WindowProc(message, wParam, lParam);
}

void CSystemTradingView::AppendLine(LPTSTR szText)
{
	if(!QueryEditable()) return;
	if(m_pTextBuffer == NULL) return;

	m_pTextBuffer->BeginUndoGroup();

	CPoint ptCursorPos = GetCursorPos();
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	int x, y;
	m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, szText, y, x);
	ptCursorPos.x = x;
	ptCursorPos.y = y;
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	SetAnchor(ptCursorPos);
	SetSelection(ptCursorPos, ptCursorPos);
	SetCursorPos(ptCursorPos);
	EnsureVisible(ptCursorPos);

	m_pTextBuffer->FlushUndoGroup(this);
}

void CSystemTradingView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(LOGFONT));

	CClientDC dc(NULL);

	logfont.lfHeight = -MulDiv(9, dc.GetDeviceCaps(LOGPIXELSY), 72);
	logfont.lfWeight = FW_NORMAL;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	lstrcpy(logfont.lfFaceName, _T("굴림체"));

	SetFont(logfont);
}

void CSystemTradingView::OnEditFind(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		CString strFind = pControl->GetEditText();	

		DWORD dwSearchFlags = 0;
		
		CPoint ptTextPos;
		if(!FindText(strFind, GetCursorPos(), dwSearchFlags, TRUE, &ptTextPos))
		{
			CString prompt;
			prompt.Format(IDS_EDIT_TEXT_NOT_FOUND, strFind);
			AfxMessageBox(prompt);
			return;
		}

		int nFind = pControl->FindString(0, strFind);

		if(nFind == -1)
			pControl->AddString(strFind);

		HighlightText(ptTextPos, lstrlen(strFind));
		SetLasFindWhat(strFind);

//		if (!FindText(strFind, TRUE,FALSE))
//		{
//			AfxMessageBox(_T("String not Found"));
//		}

		*pResult = 1; // Handled;
	} 
}

void CSystemTradingView::OnUpdateEditFind(CCmdUI* pCmd)
{
	pCmd->Enable(TRUE);
}

void CSystemTradingView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_bRButtonDown = TRUE;
	
	CCrystalEditView::OnRButtonDown(nFlags, point);
}

void CSystemTradingView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CCrystalEditView::OnRButtonUp(nFlags, point);

	if(m_bRButtonDown)
	{
		CMenu	EditPopupMenu;
		UINT	uFlags;

		EditPopupMenu.CreatePopupMenu();

		uFlags = (IsSelection())? MF_STRING|MF_ENABLED : MF_STRING|MF_GRAYED;
		EditPopupMenu.AppendMenu(uFlags, ID_EDIT_COPY, _T("복사\tCtrl+C"));
		EditPopupMenu.AppendMenu(uFlags, ID_EDIT_CUT, _T("잘라내기\tCtrl+X"));

		uFlags = (TextInClipboard())? MF_STRING|MF_ENABLED : MF_STRING|MF_GRAYED;
		EditPopupMenu.AppendMenu(uFlags, ID_EDIT_PASTE, _T("붙여넣기\tCtrl+V"));

		EditPopupMenu.AppendMenu(MF_SEPARATOR);

		EditPopupMenu.AppendMenu(MF_STRING|MF_ENABLED, ID_EDIT_FIND, _T("찾기\tCtrl+F"));
		EditPopupMenu.AppendMenu(MF_STRING|MF_ENABLED, ID_EDIT_REPLACE, _T("찾기/바꾸기\tCtrl+H"));
		EditPopupMenu.AppendMenu(MF_SEPARATOR);

		EditPopupMenu.AppendMenu(MF_SEPARATOR);
		EditPopupMenu.AppendMenu(MF_STRING|MF_ENABLED, ID_BUILD_SINGLE_COMPILE, _T("검증"));
		EditPopupMenu.AppendMenu(MF_STRING|MF_ENABLED, ID_BUILD_SINGLE_COMPILE_DLL, _T("DLL 컴파일"));
		EditPopupMenu.AppendMenu(MF_SEPARATOR);
		EditPopupMenu.AppendMenu(MF_STRING|MF_ENABLED, ID_VIEW_PROPERTY, _T("속성"));

		POINT posCursor;
		::GetCursorPos(&posCursor);

		CXTPCommandBars::TrackPopupMenu(&EditPopupMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, posCursor.x, posCursor.y, this);

		EditPopupMenu.DestroyMenu();
	}

	m_bRButtonDown = FALSE;
}

void CSystemTradingView::SetLineSelect(UINT nLine)
{
	SetFocus();
	GoToLine(nLine);

	if(m_pTextBuffer)
	{
		CPoint ptCurPos = GetCursorPos();
		DWORD dwFlags = GetLineFlags(ptCurPos.y);
		DWORD dwMask = LF_EXECUTION;
		m_pTextBuffer->SetLineFlag(ptCurPos.y, dwMask, TRUE, FALSE);
	}

	nLine = m_pTextBuffer->GetLineWithFlag(LF_EXECUTION);
	if(nLine >= 0) m_bExecutionExist = TRUE;
	else m_bExecutionExist = FALSE;
}

void CSystemTradingView::SetLineSelect(UINT nLine, UINT nColumn)
{
	SetFocus();

	int nn = GetLineLength(nLine);
	CPoint ptSel(nColumn, nLine);
	SetSelection(ptSel, ptSel);
//	GoToLine(nLine);

	if(m_pTextBuffer)
	{
		CPoint ptCurPos = GetCursorPos();
		DWORD dwFlags = GetLineFlags(ptCurPos.y);
		DWORD dwMask = LF_EXECUTION;
		m_pTextBuffer->SetLineFlag(ptCurPos.y, dwMask, TRUE, FALSE);
	}

	nLine = m_pTextBuffer->GetLineWithFlag(LF_EXECUTION);
	if(nLine >= 0) m_bExecutionExist = TRUE;
	else m_bExecutionExist = FALSE;
}

void CSystemTradingView::ClearAllExecution()
{
	if(!m_bExecutionExist)
		return;

	if(m_pTextBuffer)
	{
		int nLineCount = GetLineCount();
		for(int I = 0; I < nLineCount; I ++)
		{
			if(m_pTextBuffer->GetLineFlags(I) & LF_EXECUTION) m_pTextBuffer->SetLineFlag(I, LF_EXECUTION, FALSE);
		}

		m_bExecutionExist = FALSE;
	}							 
}

void CSystemTradingView::ChangeScriptType(UINT nScriptType, BOOL bRedraw)
{
	if(nScriptType == ST_TRADESTATION)
		m_clrBackground = ::GetSysColor(COLOR_WINDOW);
	else if(nScriptType == ST_YESTRADER)
		m_clrBackground = RGB(255, 248, 244);

	if(bRedraw)
		CCrystalTextView::RedrawWindow();
}