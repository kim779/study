// CLBparent.cpp: 구현 파일
//

#include "stdafx.h"
#include "CPopListWnd.h"
#include "resource.h"
#include "ControlWnd.h"

// CPopListWnd

IMPLEMENT_DYNAMIC(CPopListWnd, CWnd)
CString parser(CString& srcstr, CString substr)
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



CPopListWnd::CPopListWnd()
{

}

CPopListWnd::~CPopListWnd()
{
    CString str;
}


BEGIN_MESSAGE_MAP(CPopListWnd, CWnd)
    ON_WM_KILLFOCUS()
    ON_MESSAGE(WM_USER, OnMessage)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_DESTROY()
    ON_WM_LBUTTONDOWN()
    ON_WM_ACTIVATE()
    ON_WM_SIZE()
    ON_BN_CLICKED(ID_BTN_CLEAR, OnBtnClear)
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


#define DF_MENU_STOCK   "Menu_S"        //주식 프레임 메뉴
#define DF_MENU_STRADE   "Trade_S"      
#define DF_MENU_SSISE   "SISE_S"
#define DF_MENU_SCHART   "CHART_S"
#define DF_MENU_SBALANCE   "BALANCE_S"
#define DF_MENU_SHISTORY  "HISTORY_S"
 
#define DF_MENU_FUTURE   "Menu_F"     //파생 프레임 메뉴
#define DF_MENU_FTRADE  "Trade_F"
#define DF_MENU_FSISE   "SISE_F"
#define DF_MENU_FCHART   "CHART_F"
#define DF_MENU_FBALANCE   "BALANCE_F"
#define DF_MENU_FHISTORY  "HISTORY_F"

#define DF_MENU_SUBX_TRD   0     //매매 관련 화면 리스트
#define DF_MENU_SUBX_SISE   1     //시세 관련 화면 리스트
#define DF_MENU_SUBX_CHART   2     //차트 관련 화면 리스트
#define DF_MENU_SUBX_BALANCE   3     //잔고 관련 화면 리스트

BOOL CPopListWnd::MakePopMenu(CString sData, CPoint point)
{
    CString skey{}, stmp{}, sCode{}, sName{}, sFilepath{};
    CString sSMenu{}, sSHMenu{};
    CString sFMenu{}, sFHMenu{};

    //
    sFilepath = "F:\\util\\HTS\\IBK투자증권 HTS\\tab\\axPOPMenu.INI";  //여기선 탭이지만 개인 히스토리가 필요하므로   
    //

    CHMenu	 frameMenu, * menu{}, * subMenu{}, * popup{};
    sName = sData;
    sCode = parser(sName, "\t");

    DWORD rc;
    char* ptr{}, wb[1024]{};
    int iGubn = 0;
        
    //종목 시장 분류
    switch (iGubn) 
    {
        case 0:  //주식
        {
            //frame 메뉴 로드
            rc = GetPrivateProfileString(DF_MENU_STOCK, "menu", "", wb, 1024, sFilepath);
            sFMenu.Format("%s", wb);
            sFMenu.Trim();
       
            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }
        }
        break;
        case 1:  //파생
        {
            //frame 메뉴 로드
            rc = GetPrivateProfileString(DF_MENU_FUTURE, "menu", "", wb, 1024, sFilepath);
            sFMenu.Format("%s", wb);
            sFMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }
        }
        break;
    }
    
    popup = new CHMenu();
    popup->CreatePopupMenu();

    CStringArray arrFMenu, arrSMenu, arrHMenu;
    m_pCodelist->StringSplit(sFMenu, arrFMenu, _T(';'));
   
    int	menuN{};
    int    nID{};
    CMapStringToString _map{};
 
    for (int ii = 0; ii < arrFMenu.GetSize(); ii++)
    {
        //서브 메뉴 로드  ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
        switch (ii)
        {
        case DF_MENU_SUBX_TRD:  //매매 관련 화면 리스트
        {

            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 매매 메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "Trade_S_Hmenu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();

        }
        break;
        case DF_MENU_SUBX_SISE: //시세 관련 화면 리스트
        {
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_SSISE : DF_MENU_FSISE, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 시세 메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "SISE_H_menu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();

        }
        break;
        case DF_MENU_SUBX_CHART: //차트 관련 화면 리스트
        {
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_SCHART : DF_MENU_FCHART, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 차트  메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "CHART_S_menu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();
        }
        break;
        case DF_MENU_SUBX_BALANCE: //잔고 관련 화면 리스트
        {
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_SBALANCE : DF_MENU_FBALANCE, "menu", "", wb, 1024, sFilepath);
            sSMenu.Format("%s", wb);
            sSMenu.Trim();

            if (!rc)
            {
                sFMenu.Empty();
                return FALSE;
            }

            //히스토리 - 잔고  메뉴 내역
            rc = GetPrivateProfileString(iGubn == 0 ? DF_MENU_STRADE : DF_MENU_FTRADE, "BALANCE_S_menu", "", wb, 1024, sFilepath);
            sSHMenu.Format("%s", wb);
            sSHMenu.Trim();
        }
        break;
        }
        //서브 메뉴 로드  ↑↑↑↑↑↑↑↑↑↑↑↑↑↑ 

        menu = new CHMenu();
        menu->CreatePopupMenu();

        stmp.Format("%s", arrFMenu.GetAt(ii));
        stmp.Replace("|", " ");
        popup->InsertMenuX(ii , MF_POPUP | MF_BYPOSITION | MF_STRING, (UINT)menu->m_hMenu, stmp);

        CHMenu* psub = (CHMenu*)popup->GetSubMenu(ii);
        int id = popup->GetMenuItemCount();
        
        m_pCodelist->StringSplit(sSHMenu, arrSMenu, _T(';'));
        for (int jj = 0; jj < arrSMenu.GetSize(); jj++)
        {
            stmp.Format("%s", arrSMenu.GetAt(jj));
            stmp.Replace("|", " ");
            int ret = psub->AppendMenuX(MF_BYPOSITION | MF_STRING, ++menuN, stmp, nullptr);   

            skey.Format("%d", menuN);
          //_map.SetAt(skey, stmp.Mid(stmp.Find(" ")));
            _map.SetAt(skey, stmp);

        }
        sSMenu.Empty();

        psub->AppendMenuX(MF_SEPARATOR);
        //히스토리 내역  추가
        m_pCodelist->StringSplit(sSMenu, arrHMenu, _T(';'));
        for (int jj = 0; jj < arrHMenu.GetSize(); jj++)
        {
            stmp.Format("%s", arrHMenu.GetAt(jj));
            stmp.Replace("|", " ");
            int ret = psub->AppendMenuX(MF_BYPOSITION | MF_STRING, ++menuN, stmp, nullptr);

            skey.Format("%d", menuN);
            _map.SetAt(skey, stmp.Mid(stmp.Find(" ")));
            _map.SetAt(skey, stmp);

        }
    }

   


    CPoint pt;
    GetCursorPos(&pt);


    stmp.Format("CLBparen cursor -- > [pt]  top=[%d]\t right=[%d]", pt.x, pt.y);
    OutputDebugString("\r\n" + stmp);

    stmp.Format("CPopListWnd[point]   point.x=[%d]\t point.y=[%d]", point.x, point.y);
    OutputDebugString("\r\n" + stmp);

    if (_bRightPop) //우측으로 띄울때
    {
        pt.x += point.x;
        pt.y -= point.y;

        pt.y -= 3;
        pt.x += 18;
    }
    else
    {

    }

    int index{};
    index = popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, this);

    switch(index)
    {
        default:
        {

        }
        break;
    }

    
    skey.Format("%d", index);
    if (_map.Lookup(skey, stmp))
    {
      //  stmp = skey + "   " + stmp;
        AfxMessageBox(m_sVal);
    }
}


LRESULT CPopListWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
        case LIST_MSG_POP_MENU:
        {
            CString stmp, scode, sval;
            stmp.Format("%s", (char*)lParam);
            scode = parser(stmp, "\n");
            sval = parser(stmp, "\t");
            CPoint point;
            point.x = atoi(sval);
            point.y = atoi(stmp);
            MakePopMenu(scode, point);
        }
        break;
        case LIST_MSG_ONE_CLICK:
        case LIST_MSG_DBL_CLICK:
        case LIST_MSG_RETURN_CLICK:
        {
            CString stmp, scode, sname;
            stmp.Format("%s", (char*)lParam);
            scode = parser(stmp, "\t");
            sname = stmp;

            if (LOWORD(wParam) == LIST_MSG_ONE_CLICK)
            {
                m_pParent->m_bSettingCode = true;
                m_pParent->m_pCodeCtrl->SetEditData(sname,true);
                m_pParent->m_bSettingCode = false;
                return 0;
            }

            OutputDebugString("[cx_combolist][CPopListWnd][OnMessage] MSG_DBL_CLICK code=" + scode + "\n");  //m_bSettingCode
          //  m_pParent->m_bSettingCode = true;
            m_pParent->m_pCodeCtrl->SetEditData(sname);
          // m_pParent->m_bSettingCode = false;
            m_pParent->AddItem(scode, sname);

            m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_HIDE, 0);
            if (m_pParent && m_pParent->GetSafeHwnd())
            {
                if(LOWORD(wParam) == LIST_MSG_DBL_CLICK)
                    m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_ENTER, (LPARAM)(LPSTR)(LPCTSTR)scode);  //리스트 엔터 종목 알려주기
                else
                    m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_DBCLICKCODE, (LPARAM)(LPSTR)(LPCTSTR)scode);  //리스트 더블클릭 종목 알려주기
            }
        }
        break;
        case LIST_MSG_REMOVE_ITEM:
        {
            CString stmp, scode, sname;
            stmp.Format("%s", (char*)lParam);
            scode = parser(stmp, "\t");
            sname = stmp;

            if (m_nPopupType == POPUP_TYPE_HISTORY)
            {
           /*     if (m_pParent && m_pParent->GetSafeHwnd())
                    m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_REMOVE,
                        (LPARAM)(LPSTR)(LPCTSTR)scode);*/
            }
            else
            {
                // 서치팝업은 삭제 통보 안함
                OutputDebugString("[CPopListWnd] 서치팝업 삭제 무시\n");
            }
        }
        break;
        case LIST_MSG_EDIT_FOCUS:
        {
            ShowWindow(SW_HIDE);

            // CControlWnd 에 통보
            if (m_pParent && m_pParent->GetSafeHwnd())
                m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_EDIT_FOCUS, 0);
        }
        break;
    }
    return TRUE;
}

// CPopListWnd 메시지 처리기
BOOL CPopListWnd::CreatePopUpWindow(CWnd* pParent, CRect rec)
{
    //test
    CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL);

    m_pParent = (CControlWnd*)pParent;
    int ret = CreateEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,              // WS_EX_TOOLWINDOW 제거
        className,
        NULL,
        WS_POPUP ,
        rec,
        pParent,                    // ← 부모 제대로 전달
        0,
        NULL);

    
    return ret;
}

//if (!m_pListPop->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, sClassName, NULL,
//	WS_POPUP | WS_BORDER, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy),
//	NULL, NULL, NULL))

BOOL CPopListWnd::CreateListBox(CString items, int nPopWidth)
{
    OutputDebugString("[CPopListWnd] CreateListBox\n");

    CRect rec;
    GetClientRect(rec);

    const int nBtnHeight = 20;
    const int nBorder = 1; // 테두리 두께

    //CRect rcList(rec.left, rec.top, rec.right, rec.bottom - nBtnHeight);
    //CRect rcBtn(rec.left, rec.bottom - nBtnHeight, rec.right, rec.bottom);

    CRect rcList(
        rec.left + nBorder,
        rec.top + nBorder,
        rec.right - nBorder,
        rec.bottom - nBtnHeight - nBorder);

    CRect rcBtn(
        rec.left + nBorder,
        rec.bottom - nBtnHeight,
        rec.right - nBorder,
        rec.bottom - nBorder);

    m_pCodelist = std::make_unique<CCodeListCtrl>(nullptr, items);
    m_pCodelist->Create(
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SINGLESEL,
        rcList, this, 9898);

    if (m_nPopupType == POPUP_TYPE_SEARCH)
        m_pCodelist->m_bAllowDelete = FALSE;  // 서치팝업 삭제 불가
    else
        m_pCodelist->m_bAllowDelete = TRUE;   // 히스토리팝업 삭제 가능

    if (m_nType == VS1_TYPE)
    {
        int nTotal = nPopWidth;
        if (nTotal <= 0)
        {
            CRect rcPop;
            GetClientRect(&rcPop);
            nTotal = rcPop.Width();
        }
        // 1. 최대 코드 길이 계산
        CStringArray arrTmp;
        m_pCodelist->StringSplit(items, arrTmp, _T('\t'));
        int nMaxCodeLen = 0;
        for (int i = 0; i < arrTmp.GetCount(); i++)
        {
            CString sItem = arrTmp.GetAt(i);
            if (sItem.IsEmpty()) continue;
            CString sname = sItem;
            CString scode = parser(sname, " ");
            nMaxCodeLen = max(nMaxCodeLen, scode.GetLength());
        }

        BOOL bVScroll = IsVScrollVisible(arrTmp.GetCount());
        int nScrollW = bVScroll ? GetSystemMetrics(SM_CXVSCROLL) : 0;

        // 2. 실제 픽셀 크기 측정
        CClientDC dc((CWnd*)m_pCodelist.get());
        CFont* pFont = m_pCodelist->GetFont();
        CFont* pOldFont = nullptr;
        if (pFont) pOldFont = dc.SelectObject(pFont);
        CString sMaxCode;
        sMaxCode.Format("%.*s", nMaxCodeLen, "WWWWWWWWWW");
        CSize sz = dc.GetTextExtent(sMaxCode);
        if (pOldFont) dc.SelectObject(pOldFont);

        // 3. 너비 계산
        const int nCodeW = max(sz.cx + 10, 50);
        const int nBtnW = 17;
        CRect rcPop;
        GetClientRect(&rcPop);
        const int nTotalW = nTotal - nScrollW;
        int nNameW = max(nTotalW - nCodeW - nBtnW, 60);
       // nNameW -= 10;

        CString slog;
        slog.Format("[CPopListWnd][listsize] CreateListBox nCodeW=%d nNameW=%d nScrollW=%d\n",
            nCodeW, nNameW, nScrollW);
        OutputDebugString(slog);

        // 4. InsertColumn 에서 너비 설정 (SetColumnWidth 제거)
        m_pCodelist->InsertColumn(0, "종목코드", LVCFMT_LEFT, nCodeW);
        m_pCodelist->InsertColumn(1, "종목명", LVCFMT_LEFT, nNameW);
        m_pCodelist->InsertColumn(2, "", LVCFMT_CENTER, nBtnW);

        m_pCodelist->LoadClosePng(
            m_pParent->m_sRoot + "\\image\\btn_square_close.png");
    }
    else if (m_nType == VS2_TYPE)
    {
        const int nTotal = rcList.Width();
        m_pCodelist->InsertColumn(0, "", LVCFMT_LEFT, nTotal);
    }

    // 이미지 리스트
    if (m_nType == VS2_TYPE)
    {
        m_imageList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 2, 1);
        CBitmap bmp;
        if (bmp.LoadBitmap(IDB_TOOL_IMGLIST))
        {
            m_imageList.Add(&bmp, RGB(255, 0, 255));
            bmp.DeleteObject();
        }
        m_pCodelist->SetImageList(&m_imageList);
        m_pCodelist->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_TRACKSELECT);
    }

    // 데이터 추가 - 역순 삽입
    CStringArray arr;
    m_pCodelist->StringSplit(items, arr, _T('\t'));

    for (int idx = arr.GetCount() - 1; idx >= 0; idx--)
    {
        CString sItem = arr.GetAt(idx);
        if (sItem.IsEmpty()) continue;

        CString sname = sItem;
        CString scode = parser(sname, " ");

        m_pCodelist->InsertItem(0, scode);
        m_pCodelist->SetItemText(0, 1, sname);
        m_pCodelist->SetItemText(0, 2, "");

        CString slog;
        slog.Format("[CPopListWnd] CreateListBox idx=%d code=%s name=%s\n",
            idx, scode, sname);
        OutputDebugString(slog);
    }

    // 전체삭제 버튼
    m_pBtnClear = std::make_unique<CfxImgButton>();
    m_pBtnClear->Create("전체 삭제", rcBtn, this, ID_BTN_CLEAR);
    m_pBtnClear->LoadPng(
        m_pParent->m_sRoot + "\\image\\AxAllDelete.png",
        m_pParent->m_sRoot + "\\image\\AxAllDelete_DN.png",
        m_pParent->m_sRoot + "\\image\\AxAllDelete_EN.png");

    return TRUE;
}

void CPopListWnd::OnKillFocus(CWnd* pNewWnd)
{
    CString slog;
    slog.Format("\t\n[CPopListWnd] -------------------------------kill focus pNewWnd=[%x] [%x] m_nPopupType=[%d]", pNewWnd, m_pCodelist.get(), m_nPopupType);
    OutputDebugString(slog);
    if (m_nPopupType == POPUP_TYPE_SEARCH)
        return;
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    if (pNewWnd && (pNewWnd == m_pCodelist.get() || IsChild(pNewWnd)))
        return;

    if (m_pParent->m_bSettingCode)
        return;

    ShowWindow(SW_HIDE);

    // 부모(CControlWnd)에 숨김 통보
 
    BOOL bBtnClicked = FALSE;
    if (m_pParent && m_pParent->GetSafeHwnd())
        m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_HIDE, 0);  //CPopListWnd::OnKillFocus 로 안보이게


    CWnd::OnKillFocus(pNewWnd);
}

void CPopListWnd::InitMenu()
{

}



int CPopListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.
     // 모니터 중앙으로 이동
  /*  int screenCx = GetSystemMetrics(SM_CXSCREEN);
    int screenCy = GetSystemMetrics(SM_CYSCREEN);

    int width = 200;
    int height = 200;

    int x = (screenCx - width) / 2;
    int y = (screenCy - height) / 2;

    SetWindowPos(
        &wndTopMost,
        x, y,
        width, height,
        SWP_SHOWWINDOW);

    CString slog;
    slog.Format("[CPopListWnd] OnCreate 중앙좌표 x=%d y=%d\n", x, y);
    OutputDebugString(slog);*/
    
    return 0;
}


void CPopListWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rc;
    GetClientRect(&rc);

    // 타입별 테두리 색상
    COLORREF clrBorder;
    if (m_nPopupType == POPUP_TYPE_SEARCH)
        clrBorder = RGB(100, 100, 100); // 회색 
    else
        clrBorder = RGB(0, 120, 215);   // 파란색 

    // 테두리 그리기
    dc.Draw3dRect(&rc, clrBorder, clrBorder);

    // 상단에 타입 표시 텍스트 (선택사항)
    CRect rcTitle(rc.left + 1, rc.top + 1, rc.right - 1, rc.top + 15);
    COLORREF clrBg = (m_nPopupType == POPUP_TYPE_SEARCH)
        ? RGB(0, 120, 215)
        : RGB(100, 100, 100);

    dc.FillSolidRect(&rcTitle, clrBg);
    dc.SetTextColor(RGB(255, 255, 255));
    dc.SetBkMode(TRANSPARENT);

    CString sTitle = (m_nPopupType == POPUP_TYPE_SEARCH)
        ? "검색"
        : "히스토리";

    dc.DrawText(sTitle, &rcTitle,
        DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}



void CPopListWnd::OnDestroy()
{
    CWnd::OnDestroy();

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    CString str;
}


void CPopListWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CString slog;
    slog.Format("[CPopListWnd] onLButtondown  point =[%d][%d]", point.x, point.y);
    OutputDebugString(slog);
  
    CWnd::OnLButtonDown(nFlags, point);
}


void CPopListWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CWnd::OnActivate(nState, pWndOther, bMinimized);
    CString slog;
    slog.Format("[CPopListWnd] OnActivate nState=%d\n", nState);
    OutputDebugString(slog);
    if (nState == WA_INACTIVE)
    {
        // 자식으로 가는 경우 무시
        if (pWndOther && (pWndOther == m_pCodelist.get()
            || IsChild(pWndOther)))
        {
            OutputDebugString("[CPopListWnd] OnActivate 자식 무시\n");
            return;
        }

        if (m_pParent->m_bSettingCode)
            return;

        OutputDebugString("[CPopListWnd] OnActivate 외부 비활성화 닫기\n");
        ShowWindow(SW_HIDE);

        if (m_pParent && m_pParent->GetSafeHwnd())
            m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_HIDE, 0);
    }
    else if (nState == WA_ACTIVE)
    {
        if (m_nPopupType == POPUP_TYPE_SEARCH)
        {
            OutputDebugString("[CPopListWnd] SearchPop - 포커스 유지\n");
            return;
        }

        if (m_pCodelist && m_pCodelist->GetSafeHwnd())
            m_pCodelist->SetFocus();
    }
}


void CPopListWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (cx <= 0 || cy <= 0) return;

    const int nBtnHeight = 20;
    const int nBorder = 1;
    const int nScrollW = GetSystemMetrics(SM_CXVSCROLL);

    // CCodeListCtrl
    if (m_pCodelist && m_pCodelist->GetSafeHwnd())
    {
        int nListW = cx - nBorder * 2;
        int nListH = cy - nBtnHeight - nBorder * 2;

        m_pCodelist->MoveWindow(nBorder, nBorder, nListW, nListH);

        // 컬럼 너비 재조정 (스크롤바 고려)
        BOOL bVScroll = IsVScrollVisible(m_pCodelist->GetItemCount());
        int  nScrW = bVScroll ? nScrollW : 0;

        const int nCodeW = 70;
        const int nBtnW = 17;
        const int nNameW = max(nListW - nCodeW - nBtnW - nScrW, 60);

        m_pCodelist->SetColumnWidth(0, nCodeW);
        m_pCodelist->SetColumnWidth(1, nNameW);
        m_pCodelist->SetColumnWidth(2, nBtnW);

        CString slog;
        slog.Format("[CPopListWnd][listsize] OnSize list w=%d h=%d nNameW=%d\n",
            nListW, nListH, nNameW);
        OutputDebugString(slog);
    }

    // m_pBtnClear
    if (m_pBtnClear && m_pBtnClear->GetSafeHwnd())
    {
        m_pBtnClear->MoveWindow(
            nBorder,
            cy - nBtnHeight,
            cx - nBorder * 2,
            nBtnHeight - nBorder);

        CString slog;
        slog.Format("[CPopListWnd] OnSize BtnClear w=%d h=%d\n",
            cx - nBorder * 2, nBtnHeight - nBorder);
        OutputDebugString(slog);
    }
}

void CPopListWnd::OnBtnClear()
{
    OutputDebugString("[CPopListWnd] OnBtnClear 전체삭제\n");

    if (!m_pCodelist || !m_pCodelist->GetSafeHwnd()) return;

    if (m_pParent && m_pParent->GetSafeHwnd())
        if (m_nPopupType == POPUP_TYPE_SEARCH)
            return;

    m_pCodelist->DeleteAllItems();

    // 부모에 전체삭제 통보
    if (m_pParent && m_pParent->GetSafeHwnd())
            m_pParent->PostMessage(WM_POPLISTWINDOW, POPLIST_CLEARALL, 0);
}

void CPopListWnd::RefreshList(CString items)
{
    OutputDebugString("[CPopListWnd] RefreshList\n");

    if (!m_pCodelist || !m_pCodelist->GetSafeHwnd()) return;

    m_pCodelist->DeleteAllItems();

    if (items.IsEmpty())
    {
        OutputDebugString("[CPopListWnd] RefreshList items\n");
        return;
    }

    CStringArray arr;
    m_pCodelist->StringSplit(items, arr, _T('\t'));

    int nMaxCodeLen = 0;
    for (int idx = 0; idx < arr.GetCount(); idx++)
    {
        CString sItem = arr.GetAt(idx);
        if (sItem.IsEmpty()) continue;

        CString sname = sItem;
        CString scode = parser(sname, " ");
        nMaxCodeLen = max(nMaxCodeLen, scode.GetLength());
    }


    CClientDC dc((CWnd*)m_pCodelist.get());
    CFont* pFont = m_pCodelist->GetFont();
    CFont* pOldFont = nullptr;
    if (pFont) pOldFont = dc.SelectObject(pFont);


    CString sMaxCode;
    sMaxCode.Format("%.*s", nMaxCodeLen, "WWWWWWWWWW");
    CSize sz = dc.GetTextExtent(sMaxCode);
    if (pOldFont) dc.SelectObject(pOldFont);

    const int nCodeW = sz.cx + 10;
    const int nBtnW = 17;

    BOOL bVScroll = IsVScrollVisible(arr.GetCount());
    int nScrollW = bVScroll ? GetSystemMetrics(SM_CXVSCROLL) : 0;

    CRect rcList;
    m_pCodelist->GetClientRect(&rcList);
    const int nTotalW = rcList.Width() - nScrollW;
    const int nNameW = max(nTotalW - nCodeW - nBtnW, 60);

    m_pCodelist->SetColumnWidth(0, nCodeW);
    m_pCodelist->SetColumnWidth(1, nNameW);
    m_pCodelist->SetColumnWidth(2, nBtnW);

    CString slog;
    slog.Format("[CPopListWnd] RefreshList nMaxCodeLen=%d nCodeW=%d nNameW=%d nScrollW=%d listW=%d\n",
        nMaxCodeLen, nCodeW, nNameW, nScrollW, rcList.Width());
    OutputDebugString(slog);

    for (int idx = arr.GetCount() - 1; idx >= 0; idx--)
    {
        CString sItem = arr.GetAt(idx);
        if (sItem.IsEmpty()) continue;

        CString sname = sItem;
        CString scode = parser(sname, " ");

        m_pCodelist->InsertItem(0, scode);
        m_pCodelist->SetItemText(0, 1, sname);
        m_pCodelist->SetItemText(0, 2, "");

        slog.Format("[CPopListWnd] RefreshList idx=%d code=%s name=%s\n",
            idx, scode, sname);
        //   OutputDebugString(slog);
    }

    if (m_pCodelist->GetItemCount() > 0)
        m_pCodelist->SetItemStates(0, RC_ITEM_SELECTED | RC_ITEM_FOCUSED);

    OutputDebugString("[CPopListWnd] RefreshList \n");
}




void CPopListWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    OutputDebugString("[CPopListWnd] OnMouseMove 완료\n");

    CWnd::OnMouseMove(nFlags, point);
}


BOOL CPopListWnd::IsVScrollVisible(int nItemCount)
{
    if (m_nPopupType == POPUP_TYPE_SEARCH) return TRUE;
    if (!m_pCodelist || !m_pCodelist->GetSafeHwnd()) return FALSE;
    if (nItemCount <= 0) return FALSE;

    CRect rcList;
    m_pCodelist->GetClientRect(&rcList);

    // 행 높이 측정
    int nRowHeight = 20; // 기본값
    if (m_pCodelist->GetItemCount() > 0)
    {
        CRect rcItem;
        m_pCodelist->GetItemRect(0, &rcItem, LVIR_BOUNDS);
        nRowHeight = rcItem.Height();
        if (nRowHeight <= 0) nRowHeight = 20;
    }

    // 전체 아이템 높이 vs 리스트 높이
    int nTotalHeight = nItemCount * nRowHeight;

    CString slog;
    slog.Format("[IsVScrollVisible] nItemCount=%d nRowH=%d totalH=%d listH=%d\n",
        nItemCount, nRowHeight, nTotalHeight, rcList.Height());
    OutputDebugString(slog);

    return (nTotalHeight > rcList.Height());
}