// OpSensCal.cpp : implementation file
//

#include "stdafx.h"
#include "IB401500.h"
#include "OpSensCal.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpSensCal

//COpSensCal::COpSensCal()
//{
//	m_parent	= NULL;
//	m_COpSensCal	= NULL;
//	m_IOpSensCal	= NULL;
//	m_hInst		= NULL;
//}
//
//COpSensCal::~COpSensCal()
//{
//}


BEGIN_MESSAGE_MAP(COpSensCal, CWnd)
	//{{AFX_MSG_MAP(COpSensCal)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpSensCal message handlers

int COpSensCal::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1; 
	
	CString path;
	path.Format("%s\\%s\\%s", m_root, DEVDIR, "DX_OPSENSCAL.DLL");
	m_hInst = LoadLibrary(path);
	if (!m_hInst)
	{
		TRACE("DX_OptionSens control load error....\n");
		return -1;
	}

	axCreate = (CCmdTarget* (APIENTRY *)(char*)) GetProcAddress(m_hInst, "axCreateEx");
	if (axCreate == NULL)
	{
		TRACE("DX_OptionSens axCreate proc error...\n");
		return -1;
	}

	m_COpSensCal = axCreate((LPSTR)(LPCTSTR)m_root);
	m_IOpSensCal = std::make_unique< IOpSensCal >(m_COpSensCal->GetIDispatch(TRUE));
	
	return 0;
}

void COpSensCal::OnDestroy() 
{
	if (m_COpSensCal)
	{
		delete m_COpSensCal; m_COpSensCal = NULL;
		m_IOpSensCal->DetachDispatch();//	delete m_IOpSensCal;
	}
	if (m_hInst) FreeLibrary(m_hInst);

	CWnd::OnDestroy();
}

BOOL COpSensCal::CreateOpSensCal(CWnd* parent, CString root)
{
	m_parent = parent;
	m_root   = root;

	const CRect rect{};
	return CWnd::Create(NULL, "OpSensCal", WS_CHILD|WS_VISIBLE, rect, parent, 1001);
}

CString COpSensCal::GetJCode()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetJCode();
}

void COpSensCal::SetJCode(LPCTSTR jCode)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetJCode(jCode);
}

CString COpSensCal::GetBedangJisu()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetBedangJisu();
}

void COpSensCal::SetBedangJisu(LPCTSTR bdjs)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetBedangJisu(bdjs);
}

CString COpSensCal::GetStrike()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetStrike();
}

void COpSensCal::SetStrike(LPCTSTR strike)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetStrike(strike);
}

CString COpSensCal::GetCurrValue()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetCurrValue();
}

void COpSensCal::SetCurrValue(LPCTSTR cur)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetCurrValue(cur);
}

CString COpSensCal::GetCd()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetCd();
}

void COpSensCal::SetCd(LPCTSTR cd)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetCd(cd);
}

CString COpSensCal::GetDelta()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetDelta();
}

CString COpSensCal::GetGamma()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetGamma();
}

CString COpSensCal::GetTheta()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetTheta();
}

CString COpSensCal::GetRho()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetRho();
}

CString COpSensCal::GetVegga()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetVegga();
}

CString COpSensCal::GetTheoryValue()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetTheoryValue();
}

CString COpSensCal::GetIv()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetIv();
}

CString COpSensCal::GetBzRemainDays()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetBzRemainDays();
}

void COpSensCal::SetBzRemainDays(LPCTSTR bzRemainDay)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetBzRemainDays(bzRemainDay);
}

CString COpSensCal::GetCalRemainDays()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetCalRemainDays();
}

void COpSensCal::SetCalRemainDays(LPCTSTR calRemainDay)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetCalRemainDays(calRemainDay);
}

CString COpSensCal::GetK200Cur()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetK200Cur();
}

void COpSensCal::SetK200Cur(LPCTSTR K200Cur)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetK200Cur(K200Cur);
}

CString COpSensCal::GetFutureCur()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetFutureCur();
}

void COpSensCal::SetFutureCur(LPCTSTR FutureCur)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetFutureCur(FutureCur);
}

CString COpSensCal::GetTime()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetTime();
}

void COpSensCal::SetTime(LPCTSTR time)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetTime(time);
}

CString COpSensCal::GetUserName()
{
	if (m_IOpSensCal == NULL) return "";
	return m_IOpSensCal->GetUserName();
}

void COpSensCal::SetUserName(LPCTSTR userName)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetUserName(userName);
}


void COpSensCal::Calc(long CalcItem)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->Calc(CalcItem);
}

void COpSensCal::ReadCfg()
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->ReadCfg();
}

void COpSensCal::SetHv(LPCTSTR Hv90, LPCTSTR Hv60, LPCTSTR Hv30, LPCTSTR Hv21)
{
	if (m_IOpSensCal == NULL) return;
	m_IOpSensCal->SetHv(Hv90, Hv60, Hv30, Hv21);
}
