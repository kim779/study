// CodeEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "fx_codectrl.h"
#include "CodeEdit.h"
#include "Controlwnd.h"
#include <imm.h>
// CCodeEdit
#define CHANGECODELIST	1000
IMPLEMENT_DYNAMIC(CCodeEdit, CEdit)

CCodeEdit::CCodeEdit()
{

}

CCodeEdit::CCodeEdit(CControlwnd *pCombo, CWnd* pTarget)
{
	m_pParent = pCombo;
	m_pTarget = pTarget;

	m_Unit         = -1;
	m_preCode      = _T("");
	m_bInitFocus   = true;
	m_bKeydown     = false;
	m_bValidCheck  = true;
	m_bLButtonDown = FALSE;
	m_bDrag	       = FALSE;
	m_bThread      = false;
	m_hEvent       = NULL;
}

CCodeEdit::~CCodeEdit()
{
}


BEGIN_MESSAGE_MAP(CCodeEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CCodeEdit 메시지 처리기입니다.




LRESULT CCodeEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
			case VK_UP:
			case VK_DOWN:
			case VK_PRIOR:
			case VK_NEXT:
			case VK_HOME:
			case VK_END:
			case VK_RETURN:
			{
				if (m_pParent->m_pCombo->GetDroppedState() &&
					(m_Unit == 0 ))
				{
					m_pParent->m_pCombo->Key(wParam);
					return TRUE;
				}
				break;
			}
			case VK_BACK:
			{
				int nn = m_pParent->m_pCombo->GetDroppedState();
				if ((m_Unit == 1))
				{
					CString sTmp;
					GetWindowText(sTmp.Left(sTmp.GetLength()-1));
					m_pParent->m_pCombo->m_ifind = 0;
					m_pParent->m_pCombo->SearchNumberCode(sTmp);
				}
			}
			break;
		}
	}
	return CEdit::WindowProc(message, wParam, lParam);
}


bool CCodeEdit::isNumeric(CString str)
{
	for (int ii = 0; ii < str.GetLength(); ii++)
		if (str.GetAt(ii) < '0' || str.GetAt(ii) > '9')
			return false;
	return true;
}

void CCodeEdit::ChangeHangulMode(HWND hWnd, bool bHangle)
{
	DWORD	dwConversion, dwSentence;

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (bHangle)
			ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
		else
			ImmSetConversionStatus(hImc, dwConversion & ~IME_CMODE_HANGEUL, dwSentence);
	}
}

void CCodeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int	len;
	CString tmpS = _T("");
	GetWindowText(tmpS);

	if (tmpS.IsEmpty() && nChar >= '0' && nChar <= '9')
		ChangeHangulMode(this->m_hWnd, false);

	switch (nChar)
	{
		case VK_RETURN:
			{
				len = tmpS.GetLength();
				{
					//if (len == 0)
					//	m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
					return;
				}

				if(m_Unit == 0)
				{
					if (!isHexNumeric(tmpS))
					{
						CString	tmpS = m_pParent->m_pCombo->GetSelectCode();
						if (m_pParent->m_pCombo->GetDroppedState())
							m_pParent->m_pCombo->ShowDropDown(FALSE);
						SetWindowText(tmpS);
						SetSel(0, -1);	// ljh
					}
					else
					{
						m_pParent->m_pCombo->m_DataMode = 0;
						if (len < 6)
						{
							CString	string; 
							string.Format("%06d", atoi(tmpS)); 
							SetWindowText(string); 
							SetSel(0, -1);
						}
					}

			//	m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				return;
				}
				else
				{
					//m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				}
			}
			break;
		case VK_TAB:
		break;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void CCodeEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString log;
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);

	if (nFlags == 0x8000)
		return;

	int len;

	//if (nChar != VK_RETURN)
	{
		CString	sTmp = _T("");
		GetWindowText(sTmp);
		
		len = sTmp.GetLength();

		if (len <= 0)
		{
			m_preCode = sTmp;
			m_pParent->SendCtrlMsg(2);
			return;
		}
		
		if (sTmp == m_preCode && nChar != VK_RETURN)
			return;

		m_pParent->SendCtrlMsg(2);
		m_preCode = sTmp;
		switch (m_Unit)
		{
			case 0:
				{
					if (!isHexNumeric(sTmp))
					{
					//	m_pParent->m_pCombo->SetItemHeight(-1, m_pParent->m_pCombo->m_CellHeight);
					//	if (!m_pParent->m_pCombo->GetDroppedState())
					//			m_pParent->m_pCombo->ShowDropDown(TRUE);
						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
						m_pParent->m_pCombo->SearchCode(sTmp, nChar == VK_RETURN? true: false);	
					//	if (!m_pParent->m_pCombo->GetDroppedState())
					//		if(nChar != VK_RETURN)
					//			m_pParent->m_pCombo->ShowDropDown(TRUE);
						return;
					}
					else
					{
					
						if ((m_bValidCheck && (m_pParent->m_pCombo->IsValidCode(sTmp)) || (!m_bValidCheck && len >= 12)))
						{
							GetParent()->SetWindowText(sTmp);
							//m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
							SetSel(0, -1);
							if (m_pParent->m_pCombo->GetDroppedState())
								m_pParent->m_pCombo->ShowDropDown(FALSE);
						}
					}
				}
				break;
			case 1:
				{
					if (isHexNumeric(sTmp) || sTmp.Find("-") >= 0)
					{
						m_pParent->m_pCombo->SearchNumberCode(sTmp);
						if (!m_pParent->m_pCombo->GetDroppedState())
							m_pParent->m_pCombo->ShowDropDown(TRUE);

						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
					//	if ((m_bValidCheck && (m_pParent->m_pCombo->IsValidCode(sTmp)) || (!m_bValidCheck && len >= 12)))
						
					//		GetParent()->SetWindowText(sTmp);
							//m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
					//		SetSel(0, -1);
					//		if (m_pParent->m_pCombo->GetDroppedState())
					//			m_pParent->m_pCombo->ShowDropDown(FALSE);
					}
					else
					{
						if (!m_pParent->m_pCombo->GetDroppedState())
							m_pParent->m_pCombo->ShowDropDown(TRUE);

						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
						m_pParent->m_pCombo->SearchCode(sTmp);				
						return;
					}
				}
				break;
		}
	}
}

bool CCodeEdit::isHexNumeric(CString str)
{
	if (!str.IsEmpty() && (str.GetAt(0) < '0' || str.GetAt(0) > '9'))
		return false;
	for (int ii = 1; ii < str.GetLength(); ii++)
		if ((str.GetAt(ii) < '0' || str.GetAt(ii) > '9') &&
			(str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'f' && str.GetAt(ii) != 's')) &&
			(str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'F' && str.GetAt(ii) != 'S')))
			return false;
	return true;
}

void CCodeEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEdit::OnLButtonDown(nFlags, point);
	//SetSel(0, -1);
	m_bLButtonDown = TRUE;
}

void CCodeEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bLButtonDown = FALSE;
	ReleaseCapture();
	//SetSel(0, -1);
	SetFocus();
	LoadListCode();
	CEdit::OnLButtonUp(nFlags, point);
}

void CCodeEdit::LoadListCode()
{
	m_pParent->CodeListMode();
}

void CCodeEdit::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	
	CEdit::OnMouseHWheel(nFlags, zDelta, pt);
}


void CCodeEdit::OnSetFocus(CWnd* pOldWnd)
{
	if (m_bInitFocus)
	{
		//m_bInitFocus = false;
		ChangeHangulMode(this->m_hWnd);
	}

//	m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SETFOCUS), (LPARAM)GetParent()->GetSafeHwnd());

	CEdit::OnSetFocus(pOldWnd);
	if (pOldWnd)
		pOldWnd->SendMessage(WM_KILLFOCUS, 0, 0);
}


void CCodeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bKeydown = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CCodeEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bLButtonDown && m_bDrag)
	{
		CPoint pointW = point;
		ClientToScreen(&pointW);
		
		CRect rectW;
		GetWindowRect(rectW);
		
		if (!rectW.PtInRect(pointW))
		{
			m_bLButtonDown = false;
			CString strJCode;
			GetWindowText(strJCode);
			if (!strJCode.IsEmpty())
			{
				ReleaseCapture();
				m_pParent->MouseDrag(strJCode);
			}
		}
	}
	CEdit::OnMouseMove(nFlags, point);
}


void CCodeEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == CHANGECODELIST)
	{
		LoadListCode();
		KillTimer(CHANGECODELIST);
	}
	CEdit::OnTimer(nIDEvent);
}

void CCodeEdit::ListCode(CString code)
{
	SetWindowText("");
	SetWindowText(code);
	code.TrimRight();
	code.TrimLeft();
	GetParent()->SetWindowText(code);

	if (m_pParent->m_pCombo->IsHistoryMode())
		SetTimer(CHANGECODELIST, 300, NULL);


}

UINT ThreadCode(LPVOID lpvoid)
{
	CCodeEdit* pCodeEdit = (CCodeEdit*)lpvoid;
	if (pCodeEdit == NULL)
		return FALSE;
	pCodeEdit->LoadListCode();
	return 0;
}

void CCodeEdit::InitThreadCode()
{
	if (m_bThread)
		return;
	m_bThread = true;
	m_pThread = AfxBeginThread((AFX_THREADPROC) ThreadCode, (LPVOID) this, THREAD_PRIORITY_NORMAL);
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
}

void CCodeEdit::StopThreadCode()
{
	if (!m_bThread)
		return;
	SetEvent(m_hEvent);
	m_bThread = false;
	m_pThread->PostThreadMessage(WM_QUIT, 0, 0);
	TerminateThread(m_pThread->m_hThread, 0);
	CloseHandle(m_hEvent);
}

BOOL CCodeEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int	key = (zDelta > 0)? VK_UP:VK_DOWN;
	if (m_pParent->m_pCombo->GetDroppedState() && 
		(m_Unit == 0) || (m_Unit == 1))
	{
		m_pParent->m_pCombo->Key(key);
	}
	
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}


CString CCodeEdit::FindCode()
{
	CString tmpS;
	GetWindowText(tmpS);
	tmpS.TrimLeft();
	tmpS.TrimRight();

	if(tmpS.IsEmpty())
		return _T("");

	return m_pParent->m_pCombo->GetCode(tmpS);
}
