// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_News.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define G_COL	132
/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_szFileName = _T("");
        m_gubn = _T("");
	m_date = _T("");

}

CControlWnd::~CControlWnd()
{
	EmptyFile();
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnMessage1)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_EX(CControlWnd, "gubn", GetNotSupported, SetGubn, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "Start", Start, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "date", GetNotSupported, SetDate, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {404FB079-E530-4F77-9D0E-1FCC52D045A6}
static const IID IID_IControlWnd =
{ 0x404fb079, 0xe530, 0x4f77, { 0x9d, 0xe, 0x1f, 0xcc, 0x52, 0xd0, 0x45, 0xa6 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		ParsingWeb(exth->data, exth->size);
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!
		/*
		{
		char *pcBuffer = (char*)lParam;
		m_cs.Lock();
		ParsingWeb(pcBuffer, HIWORD(wParam));
		m_cs.Unlock();
		}
		*/
		break;
	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			CRect	rc;
			GetWindowRect(&rc);

			m_pHtml->SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), NULL);
			Invalidate();
		}
		break;
	default:break;
	}
	return 0;
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pHtml = new CAxBrowser;
	m_pHtml->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, m_Param.rect.Width(), m_Param.rect.Height()), this, 0);
	
	if (m_gubn.IsEmpty())
		Start();

	return 0;
}

void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	m_gubn = OptionParser(m_Param.options, "/g");
	m_date = _T("");
	m_sRoot = Variant(homeCC, ""); m_sUser = Variant(nameCC, "");
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();	
}

void CControlWnd::ParsingWeb(char *pcBuffer, int size)
{
	if (!m_szFileName.IsEmpty())
		DeleteFile(m_szFileName);

	m_szFileName.Format("%s\\User\\%s%X.htm", m_sRoot, m_sUser, GetTickCount());
	int	nRec = atoi(CString(pcBuffer, 4));

	if (nRec <= 0)
		return;

	CFile	file;
	if (!file.Open(m_szFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone))
		return;

	CString sData, szTemp;
	const	int	nSize = nRec * G_COL;

	pcBuffer += 4;
	szTemp = CString(pcBuffer, 30);
	if (szTemp.Find("text://") != -1)
	{
		CString szText = CString(pcBuffer, nSize);
		szText = szText.Mid(7);
		file.Write(szText, nSize - 7);
	}
	else 
	{
#if 0
		sData.Format("<html><body><font face = %s size = \"%dpt\"><pre>", m_Param.fonts, (m_Param.point <= 9 ? 2:4));
#else 
		sData.Format("<html><body><font face= %s><span style=\"font-size:%dpt;\"><pre>", m_Param.fonts, m_Param.point);
#endif
		file.Write(sData, sData.GetLength());
		for (int nRow = 0; nRow < nRec; nRow++)
		{
			szTemp = CString(pcBuffer + (nRow * G_COL), G_COL);
			sData.Format("%s", szTemp); sData.TrimRight();

			sData += " <br>";
			sData.Replace(" ", "&nbsp;");
			file.Write(sData, sData.GetLength());
		}
		sData.Format("</pre></span></font></body></html>");
		file.Write(sData, sData.GetLength());
	}
	file.Close();
	Sleep(100);

	TRY
	{
		m_pHtml->Navigate2(m_szFileName);
	}
	CATCH(CInternetException, e)
	{
		TRACE("except=%d\n", e->m_dwError);
	}
	END_CATCH;
}

void CControlWnd::SendTR(CString name, char* data, BYTE type, int nLen)
{
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(nLen + L_userTH + 128);
	struct	_userTH	udat {};
	int	idx = m_Param.name.GetLength();

	// control name
	CopyMemory(pWb.get(), m_Param.name, idx);
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, name, name.GetLength());
	udat.key = m_Param.key; 
	udat.stat = type;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], data, nLen);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM)pWb.get());

	pWb.reset();
}

void CControlWnd::SendWeb(CString str)
{
	struct _mid	mid {};

	if (m_gubn.GetLength() >= 2)
		CopyMemory(mid.gubn, (LPCTSTR) m_gubn, sizeof(mid.gubn));
	if (m_date.GetLength() == 8)
		CopyMemory(mid.date, (LPCTSTR) m_date, sizeof(mid.date));

	SendTR("pihoifns", (char*)&mid, NULL, sizeof(mid));
}

void CControlWnd::EmptyFile()
{
	DeleteFile(m_szFileName);
}

void CControlWnd::Start() 
{
	PostMessage(WM_USER + 1, 99);
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (cx <= 0 || cy <= 0)
		return;

	CRect	rc;
	GetWindowRect(&rc);

	m_pHtml->SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), NULL);
	Invalidate();
}

long CControlWnd::OnMessage1(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case 99:	// SendTR
		SendWeb("111");
		break;
	}

	return 0;
}

void CControlWnd::SetGubn(LPCTSTR lpszNewValue) 
{
	m_gubn = lpszNewValue;
}

void CControlWnd::SetDate(LPCTSTR lpszNewValue) 
{
	m_date = lpszNewValue;
}

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;

	return strResult.AllocSysString();
}

void CControlWnd::SetProperties(LPCTSTR sParam) 
{
	m_Param.options = sParam;
}

