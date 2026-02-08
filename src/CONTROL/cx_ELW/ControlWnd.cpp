// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_ELW.h"
#include "ControlWnd.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_parent = nullptr;
	m_xelws.RemoveAll();
}

CControlWnd::~CControlWnd()
{
	m_xelws.RemoveAll();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "GetName", _GetName, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetBgce", _GetBgce, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetGcjs", _GetGcjs, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetHbge", _GetHbge, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "IsElwCode", _IsElwCode, VT_BOOL, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {6D18E266-4474-4BC9-9F1B-0BE1339E1464}
static const IID IID_IControlWnd =
{ 0x6d18e266, 0x4474, 0x4bc9, { 0x9f, 0x1b, 0xb, 0xe1, 0x33, 0x9e, 0x14, 0x64 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

// 한글명
BSTR CControlWnd::_GetName(LPCTSTR code)
{
	CString strResult;
	_xelw	xelw;

	for (int ii = 0; ii < m_xelws.GetSize(); ii++)
	{
		xelw = m_xelws.GetAt(ii);
		if (!xelw.code.CompareNoCase(code))
		{
			strResult = xelw.hnam;
			break;
		}
	}

	return strResult.AllocSysString();
}

// 발행기관코드
BSTR CControlWnd::_GetBgce(LPCTSTR code) 
{
	CString strResult;
	_xelw	xelw;

	for (int ii = 0; ii < m_xelws.GetSize(); ii++)
	{
		xelw = m_xelws.GetAt(ii);
		if (!xelw.code.CompareNoCase(code))
		{
			strResult = xelw.bgce;
			break;
		}
	}

	return strResult.AllocSysString();
}

// 기초자산 1 ~ 5
BSTR CControlWnd::_GetGcjs(LPCTSTR code) 
{
	CString strResult;
	_xelw	xelw;

	for (int ii = 0; ii < m_xelws.GetSize(); ii++)
	{
		xelw = m_xelws.GetAt(ii);
		if (!xelw.code.CompareNoCase(code))
		{
			strResult = xelw.gcjs;
			break;
		}
	}

	return strResult.AllocSysString();
}

// 한글발행기관명
BSTR CControlWnd::_GetHbge(LPCTSTR code) 
{
	CString strResult;
	_xelw	xelw;

	for (int ii = 0; ii < m_xelws.GetSize(); ii++)
	{
		xelw = m_xelws.GetAt(ii);
		if (!xelw.code.CompareNoCase(code))
		{
			strResult = xelw.hbge;
			break;
		}
	}

	return strResult.AllocSysString();
}

BOOL CControlWnd::_IsElwCode(LPCTSTR code) 
{
	_xelw	xelw;

	for (int ii = 0; ii < m_xelws.GetSize(); ii++)
	{
		xelw = m_xelws.GetAt(ii);
		if (!xelw.code.CompareNoCase(code))
			return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////
//
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	loadingELW();
	
	return 0;
}

bool CControlWnd::loadingELW()
{
	CString	filePath;
	CFileFind finder;

	m_xelws.RemoveAll();
	filePath.Format("%s/%s/%s", m_root, TABDIR, ELWCODE);
	if (!finder.FindFile(filePath))
		return false;

	CFile	fileH;
  	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
		return false;

	int	size = (int)fileH.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	if (size != fileH.Read(pWb.get(), size))
	{
		fileH.Close();
		pWb.reset();
		return false;
	}
	fileH.Close();

	_xelw   xelw;
	CString	string, stringx;
	struct	elwcode* elwcode = (struct elwcode*)pWb.get();;

	size /= sizeof(struct elwcode);
	for (int cnt = 0; cnt < size; cnt++, elwcode++)
	{
		xelw.code = CString(elwcode->codx, sizeof(elwcode->codx));	xelw.code.Trim();
		xelw.hnam = CString(elwcode->hnam, sizeof(elwcode->hnam));	xelw.hnam.Trim();
		xelw.enam = CString(elwcode->enam, sizeof(elwcode->enam));	xelw.enam.Trim();
		xelw.symb = CString(elwcode->symb, sizeof(elwcode->symb));	xelw.symb.Trim();

		xelw.lphn = _T("");
		for (int ii = 0; ii < sizeof(elwcode->lphn) / sizeof(elwcode->lphn[0]); ii++)
		{
			string = CString(elwcode->lphn[ii], sizeof(elwcode->lphn[ii]));
			string.Trim();
//			if (!string.IsEmpty())
			{
				stringx.Format("%-3s", string);
				xelw.lphn += stringx;
			}
		}

		xelw.lpse = CString(elwcode->lpse, sizeof(elwcode->lpse));	xelw.lpse.Trim();
		xelw.hbge = CString(elwcode->hbge, sizeof(elwcode->hbge));	xelw.hbge.Trim();
		xelw.ebge = CString(elwcode->ebge, sizeof(elwcode->ebge));	xelw.ebge.Trim();
		xelw.bgce = CString(elwcode->bgce, sizeof(elwcode->bgce));	xelw.bgce.Trim();

		xelw.gcjs = _T("");
		for (int ii = 0; ii < sizeof(elwcode->gcjs) / sizeof(elwcode->gcjs[0]); ii++)
		{
			string = CString(elwcode->gcjs[ii], sizeof(elwcode->gcjs[ii]));
			string.Trim();
//			if (!string.IsEmpty())
			{
				stringx.Format("%-12s", string);
				xelw.gcjs += stringx;
			}
		}

		xelw.grat = _T("");
		for (int ii = 0; ii < sizeof(elwcode->grat) / sizeof(elwcode->grat[0]); ii++)
		{
			string = CString(elwcode->grat[ii], sizeof(elwcode->grat[ii]));
			string.Trim();
//			if (!string.IsEmpty())
			{
				stringx.Format("%-5s", string);
				xelw.grat += stringx;
			}
		}

		xelw.gcge = CString(elwcode->gcge, sizeof(elwcode->gcge));	xelw.gcge.Trim();
		xelw.hsge = CString(elwcode->hsge, sizeof(elwcode->hsge));	xelw.hsge.Trim();
		xelw.krye = CString(elwcode->krye, sizeof(elwcode->krye));	xelw.krye.Trim();
		xelw.khbe = CString(elwcode->khbe, sizeof(elwcode->khbe));	xelw.khbe.Trim();
		xelw.mjmt = CString(elwcode->mjmt, sizeof(elwcode->mjmt));	xelw.mjmt.Trim();

		m_xelws.Add(xelw);
	}
	pWb.reset();

	return true;
}

void CControlWnd::SetParam(_param *param)
{
	m_param.key     = param->key;
	m_param.name    = CString(param->name, param->name.GetLength());
	m_param.rect    = CRect(param->rect.left, param->rect.top, param->rect.right, param->rect.bottom);
	m_param.fonts   = CString(param->fonts, param->fonts.GetLength());
	m_param.point   = param->point;
	m_param.style   = param->style;
	m_param.tRGB    = param->tRGB;
	m_param.pRGB    = param->pRGB;
	m_param.options = CString(param->options, param->options.GetLength());
}

///////////////////////////////////////////////////////////////////////////////
//
CString CControlWnd::parseX(CString &srcstr, CString substr)
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

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}
