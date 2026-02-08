// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "sample5.h"
#include "MainWnd.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_REFRESH813	WM_USER+2428
#define WM_DATATRANS	WM_USER+2424

#define	LEN_USID	12 
#define	LEN_GRPD	4 
#define	LEN_ECOD	4
#define	LEN_EMSG	80
#define	LEN_NREC	4	//반복
#define	LEN_ACNO	11
#define	LEN_ACNM	40
#define	LEN_ACPW	8
#define	LEN_GREC	4	//이중반복
#define	LEN_GPID	4
#define	LEN_FLAG	1

#define LEN_GPNM	40

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pParent, _param* pParam)
{
	EnableAutomation();

	m_pParent = pParent;
	SetParam(pParam);
	
	m_sRoot    = GetTypeData(homeCC, "");
	m_sUserID  = GetTypeData(nameCC, "");
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_DATATRANS, OnDataTrans)
END_MESSAGE_MAP()


	

BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_PROPERTY_NOTIFY(CMainWnd, "data", m_data, OnDataChanged, VT_BSTR)
	DISP_PROPERTY_EX(CMainWnd, "visible2", GetVisible2, SetVisible2, VT_BOOL)
	DISP_PROPERTY_EX(CMainWnd, "visible", GetVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX(CMainWnd, "strTemp", GetStrTemp, SetStrTemp, VT_BSTR)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "RequestTR", RequestTR, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Config", Config, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetTotalDay", GetTotalDay, VT_I4, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetDisplayDay", GetDisplayDay, VT_I4, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "OnVisibleChanged", OnVisibleChanged, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetTypeData", GetTypeData, VT_BSTR, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetMemoData", GetMemoData, VT_BSTR, VTS_I2 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "Refresh813", Refresh813, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CMainWnd, "GetWidth", GetWidth, VT_I4, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetHeight", GetHeight, VT_I4, VTS_NONE)
	DISP_PROPERTY_PARAM(CMainWnd, "Get813Group", _Get813Group, SetNotSupported, VT_BSTR, VTS_BSTR)
	DISP_PROPERTY_PARAM(CMainWnd, "Get813Account", _GetGet813Account, SetNotSupported, VT_BSTR, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {198FD698-9A68-4214-BAFD-6285312F3DED}
static const IID IID_IMainWnd =
{ 0x198fd698, 0x9a68, 0x4214, { 0xba, 0xfd, 0x62, 0x85, 0x31, 0x2f, 0x3d, 0xed } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers


LRESULT CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:		// Wizard request TR Header
				// return char*
		break;
	case DLL_OUB:		// Received Data
				// HIBYTE(LOWORD(WPARAM)) : key, HIWORD(WPARAM) : DATA size
				// HIBYTE(LOWORD(WPARAM)) : DATA size for MSB 8bits (control FORM OUB)
		break;
	case DLL_ALERT:		// RealTime data 
				// HIWORD(WPARAM) : stat
				// update : if control DLL,  DATAs
				// otherwise  CODE \t DATAs				
		break;
	case DLL_TRIGGER:	// Trigger data
				// HIBYTE(LOWORD(WPARAM)) : key
				// HIWORD(WPARAM) : trigger stat
				// trigger stat(true)  -> text : PROC \t PARAM
				//           (false) -> text : SYM \t DATA		
		break;
	case DLL_DOMINO:	// Domino data
				// LPARAM : text, SYM \t DATA (\n ... )
				// HIWORD(WPARAM) : domino stat			
		break;
	case DLL_NOTICE:	// account contract
				// lParam : account \t account name \n….
		break;
	case DLL_DROP:		// OnDrop data
				// lParam : point.x \t point.y \n SYM \t DATA (\t ...)
				// LOBYTE(HIWORD(WPARAM)) : source window ID
				// HIBYTE(HIWORD(WPARAM)) : destination window ID
		break;
	case DLL_AXIS:
		break;
	case DLL_EXCEL:		// Wizard request excel data
		break;
	case DLL_INVOKE:
		break;
	case DLL_SETPAL:	// Change Palette
		break;
	case DLL_SETFONT:	// set font only. not resize screen
		break;
	case DLL_SETFCB:	// blink
		break;
	case DLL_SETFONTx:	// set font & resize screen
		break;
	case DLL_SELECT:
		break;
	case DLL_ACCOUNT:	// update account combo items
		SetAccountInfo((char *)lParam);
		break;
	case DLL_GUIDE:	// set guide message
		break;
	}
	return TRUE;
}


LRESULT CMainWnd::OnDataTrans(WPARAM wParam, LPARAM lParam)
{
	CString tmp;

	tmp = (char*) lParam;
	//MessageBox("CMainWnd::OnDataTrans 데이터 수신: \n"+tmp);
	SetAccountInfo(tmp);
	//m_data = tmp;
	return TRUE;
}

void CMainWnd::SetProperties() 
{
	// TODO: Add your dispatch handler code here

}

BSTR CMainWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

void CMainWnd::OnVisibleChanged() 
{
	// TODO: Add your dispatch handler code here

}

BOOL CMainWnd::RequestTR() 
{
	// TODO: Add your dispatch handler code here

	return TRUE;
}

BOOL CMainWnd::Config() 
{
	// TODO: Add your dispatch handler code here

	return TRUE;
}

long CMainWnd::GetTotalDay() 
{
	// TODO: Add your dispatch handler code here

	return 0;
}

long CMainWnd::GetDisplayDay() 
{
	// TODO: Add your dispatch handler code here

	return 0;
}
/*
void CMainWnd::OnPaint() 
{

	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, RGB(123, 154, 41));
	dc.DrawEdge(rcClient, EDGE_SUNKEN, BF_FLAT|BF_RECT);
	// TODO: Add your message handler code here
	dc.DrawText(m_caption,-1, CRect(10,10,150,30), 0);
	// Do not call CWnd::OnPaint() for painting messages

}
*/

BOOL CMainWnd::GetVisible2() 
{
	// TODO: Add your property handler here
	
	return IsWindowVisible();;
}

void CMainWnd::SetVisible2(BOOL bNewValue) 
{
	// TODO: Add your property handler here
	if (bNewValue)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
	
}

BOOL CMainWnd::GetVisible() 
{
	// TODO: Add your property handler here

	return TRUE;
}

void CMainWnd::SetVisible(BOOL bNewValue) 
{
	// TODO: Add your property handler here

}


BSTR CMainWnd::GetStrTemp() 
{
	CString strResult;
	// TODO: Add your property handler here

	return strResult.AllocSysString();
}

void CMainWnd::SetStrTemp(LPCTSTR lpszNewValue) 
{
	// TODO: Add your property handler here
	//m_caption.SetSysString(CString(lpszNewValue));
	m_caption = (CString) lpszNewValue;

	CMainWnd::RedrawWindow();
}

void CMainWnd::Initialize(BOOL tp)
{
	m_caption = "CX_DATATRANS";
	SetWindowText("CX_DATATRANS");
}

BOOL CMainWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pParent = pParentWnd;
	ShowWindow(SW_HIDE);

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CMainWnd::SetAccountInfo(CString acnt_info)
{
	 m_data = acnt_info;
	 //PostMessage(WM_LBUTTONDBLCLK, 0,0); 
	 m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)),
				(LPARAM)m_Param.name.GetString());
}

void CMainWnd::OnDataChanged() 
{
	// TODO: Add notification handler code
	//상위 부모 폼에게 정보를 변경해서 알려줌

	CString tmp, tmp2;
	int	cnt = 0;
	CWnd*	cont = nullptr;
	CWnd*	p_screen = GetParent();
	
	while (p_screen != nullptr)
	{
		/*
		if(cont){
			cont->GetWindowText(tmp);
			MessageBox("cont: "+tmp);
		}
		*/
		p_screen->GetWindowText(tmp);
		
		if (tmp == "BASE")
		{
			cont = p_screen->FindWindow(NULL, "BASE");
			p_screen->PostMessage(WM_DATATRANS, 0, (LPARAM)m_data.GetString());
			break;
		}
		
		//3번째가 BASE 2번째 돌때가 PANE이었음
		p_screen = p_screen->GetParent();
		cnt++;
		if (cnt > 7)
		{
			//MessageBox("못찾았습니다.");
			break;
		}
	}
	
}

void CMainWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	SetAccountInfo("AN10|00110000014|zPwd|0423|");
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CMainWnd::SetParam(_param *pParam)
{
	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
}

BSTR CMainWnd::GetTypeData(short typ, LPCTSTR str1) 
{
	CString strResult;

	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, typ), (LPARAM)str1);

	if ((long)dta > 1)
		strResult = dta;

	//strResult.Replace("너다타\t\t0", "너다타\t\t5");

	return strResult.AllocSysString();
}

BSTR CMainWnd::GetMemoData(short typ, LPCTSTR idata, LPCTSTR mdata) 
{
	CString strResult;
	CString sKey = idata;
	
	sKey.TrimRight();
	if (typ < 3)
	{
		CFileFind	finder;
		CString dir, dat;
		char	key[12]{}, lBytes[4]{};

		dir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, USER, m_sUserID);
		if (!finder.FindFile(dir))
			return strResult.AllocSysString();

		CFile file;
		
		if (file.Open(dir, CFile::modeRead | CFile::shareDenyNone))
		{
			UINT	nBytesRead = 0;
			do
			{
				nBytesRead = file.Read(key, sizeof(key));
				if (nBytesRead == sizeof(key))
				{
					nBytesRead = file.Read(lBytes, sizeof(lBytes));
					if (nBytesRead == sizeof(lBytes))
					{
						int	lSize = atoi(CString(lBytes, 4));
						nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);

						if ((int)nBytesRead != lSize)
						{
							dat.ReleaseBuffer();
							break;
						}
						if (typ == 1)
						{
							strResult = strResult + key + "|";
						}
						else
						{
							//AfxMessageBox(sKey+")("+key+")");
							if (!sKey.Compare(key))
							{							
								strResult = dat;
								//AfxMessageBox(strResult);
								dat.ReleaseBuffer();
								
								break;
							}
						}

						dat.ReleaseBuffer();
					}
					else
						break;
				}
				else
					break;
					
			} while ((int)nBytesRead);
			file.Close();
		}
		return strResult.AllocSysString();
	}

	CString dat;
	char	key[12]{}, lBytes[4]{};
	CString str, rDir, wDir;

	rDir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, USER, m_sUserID);
	wDir.Format("%s\\%s\\%s\\memo.mm2", m_sRoot, USER, m_sUserID);
			
	CFile wFile(wDir, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone);

	str = mdata;
	if (!str.IsEmpty())
	{
		FillMemory(key, sizeof(key), ' ');
		FillMemory(lBytes, sizeof(lBytes), ' ');

		strcpy(key, sKey.operator LPCTSTR());
		itoa(str.GetLength(), lBytes, 10);
		wFile.Write(key, 12);
		wFile.Write(lBytes, 4);
		wFile.Write(str, str.GetLength());
	}

	CFileFind	finder;
	if (!finder.FindFile(rDir))
	{
		wFile.Close();
		CopyFile(wDir, rDir, false);
		DeleteFile(wDir);
		return strResult.AllocSysString();
	}
		
	CFile rFile;
	
	if (rFile.Open(rDir, CFile::modeRead | CFile::shareDenyNone))
	{
		UINT	nBytesRead = 0;
		do
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');

			nBytesRead = rFile.Read(key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = rFile.Read(lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);

					if ((int)nBytesRead != lSize)
						break;

					if (!sKey.Compare(key))
					{
						dat.ReleaseBuffer();
						continue;
					}
					wFile.Write(key, 12);
					wFile.Write(lBytes, 4);
					wFile.Write(dat, nBytesRead);
					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;

		} while ((int)nBytesRead);
		rFile.Close();
	}
	wFile.Close();
	CopyFile(wDir, rDir, false);
	DeleteFile(wDir);

	return strResult.AllocSysString();
}

BSTR CMainWnd::_Get813Group(LPCTSTR groupid) 
{
	CString strResult;
	CString	Path;
	int	pos  = 0, nrec = 0;
	CString gpid, gpnm;
	bool	find = false;
	CFile	file;

	Path.Format("%s\\%s\\%s\\grpList.dat", m_sRoot, USRDIR, m_sUserID);
	if (file.Open(Path, CFile::modeRead | CFile::shareDenyNone))
	{
		const	int	len = (int)file.GetLength();
		if (len > 0)	
		{
			std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(len);
			file.Read(pBuf.get(), len);
							
			while (true)
			{
				find = false;
				gpid = CString(pBuf.get()+pos, LEN_GPID);  pos+=LEN_GPID;
				gpnm = CString(pBuf.get()+pos, LEN_GPNM);  pos+=LEN_GPNM;
		
				//pos+=1;			//서비스에서 리드해올땐 필요없음
				//AfxMessageBox(acno);
				//if (find) m_grid->AddRow("0\t"+acno+"\t"+acnm+"\t"+acpw);
				strResult+=(gpid+"\t"+gpnm+"\n");
				nrec++;
				if (pos >= len) break;
			}
			pBuf.reset();
		}
		file.Close();
	}

	CString snrec;
	
	snrec.Format("%d", nrec);
	strResult = snrec+"\t"+strResult;
	//AfxMessageBox(strResult);

	return strResult.AllocSysString();
}

BSTR CMainWnd::_GetGet813Account(LPCTSTR grp) 
{
	CString strResult;
	CString	Path;
	CString cgrp = grp;
	int	pos = 0;
	int	nrec = 0, grec = 0;
	CString acno, acnm, acpw, gpid;
	CStringArray gary;
	bool	find = false;
	CFile	file;

	Path.Format("%s\\%s\\%s\\AccList.dat", m_sRoot, USRDIR, m_sUserID);
	if (file.Open(Path, CFile::modeRead | CFile::shareDenyNone))
	{
		const	int	len = (int)file.GetLength();
		if (len > 0)	
		{
			std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(len);
			file.Read(pBuf.get(), len);
							
			while (true)
			{
				find = false;
				acno = CString(pBuf.get()+pos, LEN_ACNO);  pos+=LEN_ACNO;
				acnm = CString(pBuf.get()+pos, LEN_ACNM);  pos+=LEN_ACNM;
				acpw = CString(pBuf.get()+pos, LEN_ACPW);  pos+=LEN_ACPW;
				grec = atoi(CString(pBuf.get()+pos, LEN_GREC));  pos+=LEN_GREC;
				if (cgrp.IsEmpty()) find = true;
				gary.RemoveAll();
				for (int ii = 1; ii <=grec; ii++)
				{
					gpid = CString(pBuf.get()+pos, LEN_GPID);  pos+=LEN_GPID;
					gary.Add(gpid);
					if (gpid == cgrp) find = true;
				}
					
				//pos+=1;			//서비스에서 리드해올땐 필요없음
				//AfxMessageBox(acno);
				if (find) strResult += acno+"\t"+acnm+"\t"+acpw+"\n";
				nrec++;
				if (pos >= len) break;

					
			}
			pBuf.reset();
		}
		file.Close();
	}

	CString snrec;
	snrec.Format("%d",nrec);
	strResult = snrec+"\t"+strResult;
	return strResult.AllocSysString();
}

void CMainWnd::Refresh813(long tp) 
{
	CString title;
	CWnd*	wnd = nullptr;
	CWinApp* app = AfxGetApp();

	app->GetMainWnd()->GetWindowText(title);
	if (title == "IBK hot Trading")
		wnd = FindWindow(NULL,"IBK hot Trading");
	else if (title == "[직원]IBK hot Trading")
	{
		wnd = FindWindow(NULL,"[직원]IBK hot Trading");
	}
	else if (title == "IBK개발")
		wnd = FindWindow(NULL,"IBK개발");
	else if (title == "[직원]IBK개발")
		wnd = FindWindow(NULL,"[직원]IBK개발");

	if (!wnd)  //SDI인경우 메인폼 접근이 안되서 편법이용
	{
		wnd = FindWindow(NULL,"IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK hot Trading");
		if (!wnd) wnd = FindWindow(NULL,"IBK개발");		
		if (!wnd) wnd = FindWindow(NULL,"[직원]IBK개발");	
	}
	if (wnd) wnd->SendMessage(WM_REFRESH813, 0, tp);
}

long CMainWnd::GetWidth()
{
	return GetSystemMetrics(SM_CXVIRTUALSCREEN);
}

long CMainWnd::GetHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}

