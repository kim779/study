// COubWnd.cpp: 구현 파일
//

#include "stdafx.h"
#include "CX_Account.h"
#include "COubWnd.h"
#include "resource.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/memDC.h"

#include "AccountCtrl.h"

#include "AccCrypto.h"


IMPLEMENT_DYNAMIC(COubWnd, CWnd)

#define TM_MOVECHECK 9999
#define TM_SLIDESTART 9998
#define TM_SLIDING		 9997

// -----------------------------------------------
// 레이아웃 상수
// -----------------------------------------------
const int headerH = 20;
const int dataH = 20;
const int titleH = 18;
const int tabW = 140;
const int gap1 = 1;
const int chkH = 18;	// 체크박스 영역 높이 (대리인 전용)

COubWnd::COubWnd()
{

}

COubWnd::~COubWnd()
{
}


BEGIN_MESSAGE_MAP(COubWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// COubWnd 메시지 처리기
// ==============================================
// 해시 (단방향 - FNV-1a 32bit)
// ==============================================
CString COubWnd::HashAccount(const CString& sAccNo)
{
	DWORD hash = 2166136261u;
	for (int i = 0; i < sAccNo.GetLength(); i++)
	{
		hash ^= (BYTE)sAccNo[i];
		hash *= 16777619u;
	}
	CString sHash;
	sHash.Format(_T("%08X"), hash);		// 예: "A3F2C1D0"
	return sHash;
}

CString COubWnd::EncryptAccount(const CString& sAccNo)
{
	CByteArray arr;
	for (int i = 0; i < sAccNo.GetLength(); i++)
	{
		BYTE b = (BYTE)sAccNo[i];
		b ^= s_xorKey[i % s_keyLen];		// XOR
		b += (BYTE)(i * 7);				// 위치별 오프셋 추가
		arr.Add(b);
	}
	return BytesToHex(arr);
}

CString COubWnd::DecryptAccount(const CString& sEncrypted)
{
	CByteArray arr;
	HexToBytes(sEncrypted, arr);

	CString sResult;
	for (int i = 0; i < arr.GetSize(); i++)
	{
		BYTE b = arr[i];
		b -= (BYTE)(i * 7);				// 위치별 오프셋 제거
		b ^= s_xorKey[i % s_keyLen];		// XOR 복원
		sResult += (TCHAR)b;
	}
	return sResult;
}

// ==============================================
// ini 유틸
// ==============================================

// 체크 시 오늘 날짜를 해시 키로 ini에 저장
void COubWnd::SaveHideDate()
{
	if (m_sAccNo.IsEmpty() || m_sIniPath.IsEmpty())
		return;
#ifdef DF_ACC_HASH
	CString sKey = HashAccount(m_sAccNo);
#else
	CString sKey = EncryptAccount(m_sAccNo);

#endif
	COleDateTime today = COleDateTime::GetCurrentTime();
	CString sDate = today.Format(_T("%Y%m%d"));

	WritePrivateProfileString(_T("AgentPopup"), sKey, sDate, m_sIniPath);

}

// 7일 억제 중인지 확인 (TRUE = 억제 중 → 팝업 스킵)
bool COubWnd::IsHiddenToday()
{
	if (m_sAccNo.IsEmpty() || m_sIniPath.IsEmpty())
		return false;

#ifdef DF_ACC_HASH
	CString sKey = HashAccount(m_sAccNo);
#else
	CString sKey = EncryptAccount(m_sAccNo);
#endif

	TCHAR szDate[16] = {};
	GetPrivateProfileString(_T("AgentPopup"), sKey, _T(""),
		szDate, 16, m_sIniPath);

	if (_tcslen(szDate) != 8)
		return false;

	CString s(szDate);
	int year = _ttoi(s.Mid(0, 4));
	int month = _ttoi(s.Mid(4, 2));
	int day = _ttoi(s.Mid(6, 2));

	COleDateTime saved(year, month, day, 0, 0, 0);
	COleDateTime now = COleDateTime::GetCurrentTime();

	COleDateTimeSpan span = now - saved;
	return (span.GetTotalDays() < 7.0);	// 7일 이내면 억제
}



void COubWnd::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CPen* pOldPen;
	dc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	drawCaption(&dc);
	drawData(&dc);

	// 닫기(X) 버튼
	GetClientRect(&rc);
	rc.left = rc.right - 14;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;

	pOldPen = (CPen*)dc.SelectObject(
		getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

	dc.Rectangle(rc);
	rc.DeflateRect(1, 1);
	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	rc.InflateRect(1, 1);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right - 1, rc.bottom - 1);
	dc.MoveTo(rc.right - 1, rc.top);
	dc.LineTo(rc.left, rc.bottom - 1);

	dc.SelectObject(pOldPen);
}

CPen* COubWnd::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pWizard->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CFont* COubWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

void COubWnd::drawCaption(CDC* pDC)
{
	CFont* pOldFont = nullptr;
	CRect clientRc, rect;

	GetClientRect(&clientRc);
	rect = clientRc;

	rect.top += gap1;
	rect.bottom = rect.top + titleH;
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));
	pDC->SetTextColor(RGB(38, 55, 100));
	pOldFont = (CFont*)pDC->SelectObject(getAxFont(_T("굴림체"), 9, 3));

	rect.right = rect.left + tabW;
	pDC->SelectObject(pOldFont);

	rect.left = rect.right;
	rect.right = clientRc.right;
	rect.DeflateRect(0, 2, 2, 2);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_HIGHLIGHT));
}

void COubWnd::drawData(CDC* pDC)
{
	CRect rect, drc;
	GetClientRect(&rect);
	rect.top = rect.top + headerH;
	rect.DeflateRect(1, 1);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	CFont* pOldFont = (CFont*)pDC->SelectObject(getAxFont(_T("굴림체"), 8, 0));

	drc = rect;
	drc.top += 5;

	// 대리인이면 하단 chkH 만큼 체크박스 영역 확보
	if (m_bIsAgent)
		drc.bottom -= chkH;

	pDC->DrawText(m_sMsg, drc, DT_LEFT | DT_VCENTER | DT_WORDBREAK);

	// ── 체크박스 그리기 (대리인 전용) ──────────────────
	if (m_bIsAgent)
	{
		CRect rcChk;
		rcChk.left = rect.left + 5;
		rcChk.bottom = rect.bottom - 3;
		rcChk.top = rcChk.bottom - 13;
		rcChk.right = rcChk.left + 13;
		m_rcCheckBox = rcChk;	// 클릭 판정용 저장

		// 체크박스 테두리
		pDC->Rectangle(rcChk);

		// 체크 표시
		if (m_bDontShow)
		{
			CPen* pOldPen = (CPen*)pDC->SelectObject(
				getAxPen(RGB(0, 0, 200), 2, PS_SOLID));
			pDC->MoveTo(rcChk.left + 2, rcChk.top + 6);
			pDC->LineTo(rcChk.left + 5, rcChk.bottom - 2);
			pDC->LineTo(rcChk.right - 2, rcChk.top + 2);
			pDC->SelectObject(pOldPen);
		}

		// 체크박스 옆 문구
		CRect rcTxt = rcChk;
		rcTxt.left = rcChk.right + 4;
		rcTxt.right = rect.right;
		pDC->DrawText(_T("7일동안 안띄웁니다."), rcTxt,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	// ────────────────────────────────────────────────────

	pDC->SelectObject(pOldFont);
}

void COubWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void COubWnd::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	//PostMessage(WM_CLOSE, 0, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void COubWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// ── 체크박스 클릭 (대리인 전용) ──────────────────
	if (m_bIsAgent && m_rcCheckBox.PtInRect(point))
	{
		m_bDontShow = !m_bDontShow;
		Invalidate();
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	// ─────────────────────────────────────────────────

	// 닫기(X) 버튼
	CRect rc;
	GetClientRect(&rc);
	rc.left = rc.right - 14;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;

	if (rc.PtInRect(point))
	{
		SetCapture();
		m_bDrag = true;
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void COubWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrag)
	{
		ReleaseCapture();
		m_bDrag = false;
		PostMessage(WM_CLOSE, 0, 0);
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void COubWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_MOVECHECK:
		{
			if(m_bShow)
			{
				CRect rec, tmprec;
				m_pParent->GetWindowRect(&rec);
				if (rec != m_recParent)
				{
					GetWindowRect(tmprec);
					tmprec.OffsetRect(rec.left - m_recParent.left, rec.top - m_recParent.top);
					MoveWindow(tmprec, true);
					m_recParent = rec;
				}

				if (rec.left < 100 && rec.top < 100)
				{
					ShowWindow(SW_HIDE);
					KillTimer(TM_SLIDING);
					m_bShow = false;
					PostMessage(WM_CLOSE, 0, 0);
					return;
				}
			}
			else
			{

			}
		}
		break;
		case TM_SLIDESTART:
		{
			KillTimer(TM_SLIDESTART);
			SetTimer(TM_SLIDING, 50, nullptr);
		}
		break;
		case TM_SLIDING:
		{
			CRect tmprec;
			GetWindowRect(tmprec);
			tmprec.bottom -= 10;
			MoveWindow(tmprec, true);

			if (tmprec.Height() < 50)
			{
				KillTimer(TM_SLIDING);
				m_bShow = false;
				PostMessage(WM_CLOSE, 0, 0);
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


int COubWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	GetClientRect(m_rect);
	m_pParent->GetWindowRect(m_recParent);
	m_bShow = true;
	SetTimer(TM_MOVECHECK, 10, nullptr);
	SetTimer(TM_SLIDESTART, 2000, nullptr);
	return 0;
}


void COubWnd::OnDestroy()
{
	CWnd::OnDestroy();
	m_bShow = false;

	// 체크됐으면 ini에 날짜 저장
	if (m_bIsAgent && m_bDontShow)
		SaveHideDate();

	((CAccountCtrl*)m_pParent)->RemoveOubWnd(this);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
