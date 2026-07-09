// MapWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TB205001.h"
#include "MapWnd.h"


// CMapWnd

IMPLEMENT_DYNAMIC(CMapWnd, CWnd)

CMapWnd::CMapWnd()
{
	m_pWnd = NULL;
}

CMapWnd::CMapWnd(CWnd* pWizard)
{
	//m_pWizard = pWizard;
}

CMapWnd::~CMapWnd()
{
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
	{
		pMain->PostMessage(WM_USER + 500 , axINTERSETOFF, (LPARAM)this);
	}

}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER + 999, OnAXIS)
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()



// CMapWnd 메시지 처리기입니다.


LRESULT CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
		{
			char* pBytes = (char *)lParam;
			struct	_ledgerH * pledger = NULL;
			pledger = (struct _ledgerH*)lParam;
			CString strkey;
			strkey = CString(pledger->fnam, 30).TrimRight();

			//int igubn = atoi(CString(&pBytes[sizeof(_ledgerH)], 1));
			if(strkey  == "selectAttGrpList")    //관심 리스트 조회 
			{
				m_pWnd->oubSearchList(pBytes);
			}
			else if(strkey == "doAttGrpItm")   // 관심종목 조회 , 추가 및 변경 
			{
				m_pWnd->oubInterListCode(pBytes);
			}
			else if(strkey == "saveAttGrpSeq")   //관심 순서 저장
			{
				m_pWnd->oubInterListOrder(pBytes);
			}
			else if(strkey == "doAttGrp")  //관심그룹 추가/변경/삭제
			{
				m_pWnd->oubInterList(pBytes);
			}		
			else if(strkey = "deleteAttGrpMulti")
			{
				m_pWnd->oubRemoveAllgroup(pBytes);
			}
		}
		break;
		case DLL_ALERTx:
		{	
			
		}
		break;
		case DLL_TRIGGER:
		{
			
		}
	}
	return 0;
}

void CMapWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, RGB(255,255,255));
}


int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	Variant(titleCC, _T("관심종목설정"));
	
	m_pWnd = new CInterestPanel(this);
	m_pWnd->Create(IDD_INTEREST, this);
	m_pWnd->ShowWindow(SW_SHOW);

	return 0;
}


void CMapWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	if(m_pWnd != NULL)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		m_pWnd->MoveWindow(&rcClient);
	}
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMapWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CWnd::OnTimer(nIDEvent);
}

CString CMapWnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strValue;
	char* pValue = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pValue > 1)
		strValue = pValue;
	else
		return _T("");

	return strValue;
}

int CMapWnd::GetKey()
{
	int key = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, minorCC), 0L);
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0L);
}

CString CMapWnd::GetRoot()
{
	return Variant(homeCC);
}

CString CMapWnd::GetName()
{
	return Variant(nameCC);
}

CString CMapWnd::GetUser()
{
	return Variant(userCC);
}

CWnd* CMapWnd::GetWizard()
{
	return m_pWizard;
}

CBitmap* CMapWnd::GetBitmap(CString strPath)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)(LPCTSTR)strPath);
}

COLORREF CMapWnd::GetIndexColor(int nIndex)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

CString CMapWnd::Parser(CString &strSrc, CString strSub /* = _T("\t") */)
{
	if (strSrc.Find(strSub) == -1)
	{
		CString temp = strSrc;
		strSrc.Empty();
			return temp;
	}
	else
	{
		CString  strTemp = strSrc.Left(strSrc.Find(strSub));
		strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
		return strTemp;
	}

	return _T("");
}

LRESULT CMapWnd::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case axADDINTERSET:
			{
				CString strdata, tmps, strcode, strname, strsuik, strmangi;
				strdata.Format(_T("%s"), (char*)lParam);
				
				CString strgroup;
				if(!m_pWnd->addcodeCheck(strgroup))
				{
					CString msg;
					msg.Format(_T("기본그룹인 %s에는 종목을 추가할 수 없습니다."), strgroup);
					AfxMessageBox(msg);
				}
				else
					m_pWnd->addCode(strdata);
			}
			break;
		case axDELINTERSET:
			{
				m_pWnd->DeleteItem();
			}
			break;
	}
	return 1;
}

void CMapWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CWnd *pMain = AfxGetMainWnd();
	if(pMain->GetSafeHwnd())
	{
		pMain->SendMessage(WM_USER + 500 , axINTERSETOFF, (LPARAM)this);
	}
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(closeDLL, 0), 0);
	CWnd::OnClose();
}


BOOL CMapWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWnd::PreCreateWindow(cs);
}


void CMapWnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
