// Controlwnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "fx_edit.h"
#include "Controlwnd.h"
#include "CodeEdit.h"
#include "../../../h/axisfire.h"
// CControlwnd
#pragma warning(disable:4996)
IMPLEMENT_DYNAMIC(CControlwnd, CWnd)

CControlwnd::CControlwnd()
{

	EnableAutomation();
}

CControlwnd::~CControlwnd()
{
}

void CControlwnd::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	DISP_FUNCTION_ID(CControlwnd, "GetData", dispidGetData, GetData, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "SetData", dispidSetData, SetData, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetEnable", dispidSetEnable, SetEnable, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CControlwnd, "SetName", dispidSetName, SetName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetID", dispidSetID, SetID, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetVisible", dispidSetVisible, SetVisible, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CControlwnd, "GetUserName", dispidGetUserName, GetUserName, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// 참고: IID_IControlwnd에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {7733226F-155D-48E3-88D0-8CBBF5C5DECD}
static const IID IID_IControlwnd =
{ 0x7733226F, 0x155D, 0x48E3, { 0x88, 0xD0, 0x8C, 0xBB, 0xF5, 0xC5, 0xDE, 0xCD } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()


// CControlwnd 메시지 처리기입니다.

LRESULT CControlwnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			char* pbuf= (char*)lParam;
			struct  _ledgerH* pledger = (struct  _ledgerH*)pbuf;
			CString strkey;
			strkey = CString(pledger->fnam, 30).TrimRight();

			if(strkey == "selectUsrNm")
			{
				char* pdata = new char[sizeof(struct mod)];
				memset(pdata, 0, sizeof(struct  mod));
				memcpy(pdata, (char*)&pbuf[sizeof(struct  _ledgerH)], sizeof(struct mod));
				struct  mod* pmod = (struct mod*)pdata;

				CString temps, senddata;
				temps =  CString(pmod->name, 30).TrimRight();
				temps.TrimLeft();
			//	this->m_strMsgName = temps;
			}
		}
		break;
	case DLL_DOMINO:
		{

		}
		break;
	case DLL_TRIGGER:
		{
			CString strData = (LPCTSTR)lParam;
			AfxMessageBox(strData);
		}
		break;
	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			CString tmp( (LPCTSTR)lParam );
			int ipoint = HIWORD(wParam);
			Invalidate();
		}
		break;
	}
	
	return 0;
}


void CControlwnd::SetParam(_param *pParam)
{
	m_Param.key   = pParam->key;
	
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
/*
	CString tmps, strdata;
	tmps = m_Param.options;
	strdata = Parser(tmps, _T("/g"));
	m_Unit = atoi(tmps);
	*/
}


CString CControlwnd::Parser(CString &srcstr, CString substr)
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

int CControlwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_pEdit = new CCodeEdit(this);
	m_pEdit->Create(ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|ES_LEFT, CRect(0,0,10,10), this, 1001);

	CClientDC dc(this);
	m_Font.CreatePointFont(m_Param.point * 10, m_Param.fonts, &dc);
	SetFont(&m_Font, TRUE);
	m_pEdit->SetFont(&m_Font, true);

	m_home = Variant(homeCC, "");
	m_sUserName = Variant(nameCC, "");
	m_sUserID = Variant(userCC, "");
	m_pEdit->m_sUserName = m_sUserName;
	m_pEdit->m_sUserID = m_sUserID;
	return 0;
}

CString CControlwnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strValue;
	char* pValue = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pValue > 1)
		strValue = pValue;
	else
		return _T("");

	return strValue;
}


void CControlwnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0)
		ResizeControls();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CControlwnd::ResizeControls()
{
	CRect	rect, editRC, btnRC;
	GetClientRect(&rect);

	rect.DeflateRect(1,1);
	editRC = btnRC = rect;
	btnRC.left = btnRC.right - btnRC.Height() + 1;

	editRC = rect;

	editRC.right -= 1;
	editRC.left += 1;
	editRC.top += 1;
	editRC.bottom -= 1;

	CSize	sz = CSize(0, 0);
	CDC	*pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(&m_Font);
	sz = pDC->GetOutputTextExtent("하");
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);
	
	if(m_pEdit != NULL)
	{
		m_pEdit->MoveWindow(editRC.left, editRC.top, editRC.Width(), editRC.Height());
		m_pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}

	if (sz.cy > 0 && m_pEdit != NULL)
	{
		if (editRC.Height() > sz.cy)
		{
			int	y = (editRC.Height() - sz.cy)/2;
			editRC.DeflateRect(0, y);
			m_pEdit->MoveWindow(editRC.left, editRC.top, editRC.Width(), editRC.Height());
			m_pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			Invalidate();
		}
	}

	
}

void CControlwnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(179,179,179));

	rc.DeflateRect(1, 1, 1, 1);
	dc.FillSolidRect(rc, RGB(255, 255, 255));
}


BSTR CControlwnd::GetData(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	// TODO: 여기에 디스패치 처리기를 추가합니다.
	if(m_pEdit != NULL)
		m_pEdit->GetWindowText(strResult);
	return strResult.AllocSysString();
}


void CControlwnd::GetledgerH(struct _ledgerH* pLedgerH)
{
	CString strtemp;
	strtemp = "FITS-HTS";
	memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

	strtemp = "FITSUsrNmSO";
	memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

	strtemp = "selectUsrNm";
	memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
}

bool CControlwnd::sendTR(CString trC, char* pBytes, int nBytes)
{
	int	len = L_userTH + nBytes + m_Param.name.GetLength() + 1 + sizeof(struct  _ledgerH);
	char*	sndB = new char[len];
	ZeroMemory(sndB, sizeof(sndB));
	sprintf(sndB, _T("%s\t"), m_Param.name);
		
	struct _userTH* udat = (struct _userTH*)&sndB[m_Param.name.GetLength() + 1];
	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = m_Param.key;	
	udat->stat = 0;
	
	struct  _ledgerH ledgerH, *pledgerH;	
	pledgerH = new _ledgerH;
	int nn = sizeof(struct  _ledgerH);
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH);
	CopyMemory(&sndB[L_userTH + m_Param.name.GetLength() + 1], &ledgerH, sizeof(struct  _ledgerH));
	CopyMemory(&sndB[L_userTH + m_Param.name.GetLength() + 1 + sizeof(struct  _ledgerH)], pBytes, nBytes);

	int iret = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes + sizeof(struct  _ledgerH)), (LPARAM)sndB);
	
	delete [] sndB;
	if(iret)
		return true;
	else
		return false;
}

void CControlwnd::MessangerMemoPop(LPCTSTR userid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strMsgUser.Empty();
	char  *pbuf = NULL;
	char  *psub = NULL;

	psub = new char[sizeof(struct mid) + 1];
	memset(psub, 0, sizeof(struct mid) + 1);
	
	struct mid* pmid = new struct mid;
	FillMemory(pmid, sizeof(struct  mid), ' ');

	memcpy(pmid->userid, (char*)m_sUserID.operator LPCSTR(), m_sUserID.GetLength());
	memcpy(psub, pmid, sizeof(struct mid));

	if(sendTR("PIBOPFRM", psub, sizeof(struct mid)))
		m_strMsgUser = userid;
}

void CControlwnd::SetData(LPCTSTR strdata)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	strResult.Format(_T("%s"), strdata);
	m_pEdit->SetWindowText(strResult);
	m_pEdit->Invalidate();
}


void CControlwnd::SetEnable(LONG benable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(benable == -1)
		m_pEdit->SetReadOnly(false);
	else
		m_pEdit->SetReadOnly(true);
}


void CControlwnd::SetName(LPCTSTR name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strdata;
	strdata.Format(_T("%s"), name);
	strdata.TrimLeft();
	strdata.TrimRight();
	m_strMsgName = strdata;

	m_pEdit->SetWindowText(m_strMsgName);
	m_pEdit->Invalidate();

CString msg;
msg.Format(_T("@@@@ SetName = [%s]\n"), m_strMsgName);
OutputDebugString(msg);
	// TODO: 여기에 디스패치 처리기를 추가합니다.
}


void CControlwnd::SetID(LPCTSTR id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strdata;
	strdata.Format(_T("%s"), id);
	strdata.TrimLeft();
	strdata.TrimRight();
	m_strMsgUser = strdata;

CString msg;
msg.Format(_T("@@@@ SetName = [%s]\n"), m_strMsgUser);
OutputDebugString(msg);
}


void CControlwnd::SetVisible(LONG bvisible)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(bvisible == -1)
		ShowWindow(SW_HIDE);
	else
		ShowWindow(SW_SHOW);
}


void CControlwnd::GetUserName(LPCTSTR id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return;

	struct mid
	{
		char userid[30];
	};

	CString strdata;
	m_strMsgUser.Empty();
	char  *pbuf = NULL;
	char  *psub = NULL;

	psub = new char[sizeof(struct mid) + 1];
	memset(psub, 0, sizeof(struct mid) + 1);
	
	struct mid* pmid = new struct mid;
	FillMemory(pmid, sizeof(struct  mid), ' ');
	strdata.Format(_T("%s"), id);
	strdata.TrimRight();
	memcpy(pmid->userid, (char*)strdata.operator LPCSTR(), strdata.GetLength());
	memcpy(psub, pmid, sizeof(struct mid));

	if(sendTR("PIBOPFRM", psub, sizeof(struct mid)))
		m_strMsgUser = id;
}
