// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Cfg.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"
#include "../../h/userdecrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WPL_SETROOT	100
#define WPL_READCFG	200
#define WPL_SHOWDLG	201

#define FILE_CONFIG	"CFG.INI"
#define CONFIRM_DLG	"CONFIRMDLG"
#define INIT_PRICE	"INITPRICE"
#define INIT_QTY	"INITQTY"
#define ENTER_ORDER	"ENTERORDER"

#define	TAB_ACCOUNT	"TABACCOUNT"
#define	TAB_PASSWORD	"TABPASSWORD"
#define	TAB_CODE	"TABCODE"
#define	TAB_MMGB	"TABMMGB"
#define	TAB_QTY		"TABQTY"
#define	TAB_PRICE	"TABPRICE"
#define	TAB_BUTTON	"TABBUTTON"

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent	= NULL;
	m_publicSection	= "";
	m_mapSection	= "";
	m_cfgDlg	= "";
	m_hCfg		= NULL;
	m_pCfg		= NULL;
}

CControlWnd::~CControlWnd()
{
	//FreeMapDll();
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
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "SetMap", _SetMap, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "ShowDlg", _ShowDlg, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "ReadCfg", _ReadCfg, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "LoadDefaultCfg", _LoadDefaultCfg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SaveCfg", _SaveCfg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "WriteCfg", _WriteCfg, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "LoadCfg", _LoadCfg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetCfgDlg", _SetCfgDlg, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetPrivateCfgName", _SetPrivateCfgName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetPublicCfgName", _SetPublicCfgName, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CF5D97B6-E166-469E-AD3C-845A7E92859D}
static const IID IID_IControlWnd =
{ 0xcf5d97b6, 0xe166, 0x469e, { 0xad, 0x3c, 0x84, 0x5a, 0x7e, 0x92, 0x85, 0x9d } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers


void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
		
}

void CControlWnd::CreateCtrl(CWnd* pParent, struct _param* pParam)
{
	m_pParent = pParent;

	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_Param.rect, m_pParent, 100);

	CString sSaveCfg, sCommonCfg, sCfgMapName, tmp = m_Param.options;

	m_sUserDir.Format("%s\\user\\%s", Variant(homeCC, ""), CAxisProfile::GetHashValue(Variant(nameCC, "")));
	sSaveCfg = OptionParser(tmp, "/m");
	sCommonCfg = OptionParser(tmp, "/c");
	sCfgMapName = OptionParser(tmp, "/d");
	m_sCfgFile = OptionParser(tmp, "/f");
	m_sCfgFile.TrimRight();
	if (m_sCfgFile.IsEmpty())
		m_sCfgFile = FILE_CONFIG;
	if (!sSaveCfg.IsEmpty())
		_SetMap(sSaveCfg, sCommonCfg, sCfgMapName);
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CControlWnd::SetView(int type, CString data)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.GetString());
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;

	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
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

void CControlWnd::LoadCfg()
{
	CString sUserFile, sCommonFile;

	sCommonFile.Format("%s\\mtbl\\%s", Variant(homeCC, ""), m_sCfgFile);
	sUserFile.Format("%s\\%s", m_sUserDir, m_sCfgFile);  

	// 신규추가정보를 읽어들일 수 있도록 CommonFile을 먼저읽는다
	m_publicIdents.RemoveAll();
	m_mapIdents.RemoveAll();
	AddSectionInfo(m_publicSection, sCommonFile, m_publicIdents);
	AddSectionInfo(m_mapSection, sCommonFile, m_mapIdents);

	/*
	// 공통항목 초기값 설정
	CString strResult;
	if (!m_publicIdents.Lookup(CONFIRM_DLG, strResult))	m_publicIdents.SetAt(CONFIRM_DLG, "T");
	if (!m_publicIdents.Lookup(INIT_PRICE, strResult))	m_publicIdents.SetAt(INIT_PRICE, "T");
	if (!m_publicIdents.Lookup(INIT_QTY, strResult))	m_publicIdents.SetAt(INIT_QTY, "T");
	if (!m_publicIdents.Lookup(ENTER_ORDER, strResult))	m_publicIdents.SetAt(ENTER_ORDER, "F");
	if (!m_publicIdents.Lookup(TAB_ACCOUNT, strResult))	m_publicIdents.SetAt(TAB_ACCOUNT, "T");
	if (!m_publicIdents.Lookup(TAB_PASSWORD, strResult))	m_publicIdents.SetAt(TAB_PASSWORD, "T");
	if (!m_publicIdents.Lookup(TAB_CODE, strResult))	m_publicIdents.SetAt(TAB_CODE, "T");
	if (!m_publicIdents.Lookup(TAB_MMGB, strResult))	m_publicIdents.SetAt(TAB_MMGB, "T");
	if (!m_publicIdents.Lookup(TAB_QTY, strResult))		m_publicIdents.SetAt(TAB_QTY, "T");
	if (!m_publicIdents.Lookup(TAB_PRICE, strResult))	m_publicIdents.SetAt(TAB_PRICE, "T");
	if (!m_publicIdents.Lookup(TAB_BUTTON, strResult))	m_publicIdents.SetAt(TAB_BUTTON, "T");
	*/

	// 개별 화면의 설정정보를 읽어들어 CommonFile의 정보를 덮어씌운다
	AddSectionInfo(m_publicSection, sUserFile, m_publicIdents);
	AddSectionInfo(m_mapSection, sUserFile, m_mapIdents);
}

void CControlWnd::SaveCfg()
{
	CString sUserFile;
	sUserFile.Format("%s\\%s", m_sUserDir, m_sCfgFile);  

	if (m_publicIdents.GetCount() > 0)
		WriteSectionInfo(m_publicSection, sUserFile, m_publicIdents);

	if (m_mapIdents.GetCount() > 0)
		WriteSectionInfo(m_mapSection, sUserFile, m_mapIdents);
}

BOOL CControlWnd::SectionExists(CString Section, CString file)
{
	char	datb[80]{};

	return (GetPrivateProfileString(Section, NULL, NULL, datb, sizeof(datb), file) != 0);
}

int CControlWnd::AddSectionInfo(CString Section, CString file, CMapStringToString &map)
{
	Section.TrimRight();
	if (Section.IsEmpty())
		return 0;

	char	datb[4096]{};
	char	*pData = datb;
	char	tmpb[256]{};

	// Section에 있는 Ident(Key) 모두 추출
	const	int	len = GetPrivateProfileString(Section, NULL, NULL, datb, sizeof(datb), file);
	if (len == 0)
		return 0;

	CString val;
	CStringArray arr;

	while (*pData != 0)
	{
		val.Format("%s", pData);
		val.MakeUpper();
	
		arr.Add(val);
		pData = pData + val.GetLength() + 1;
	}

	// 각 Key의 Value값을 읽어 map에 저장
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		GetPrivateProfileString(Section, arr.GetAt(ii), "", tmpb, sizeof(tmpb), file);
		val.Format("%s", tmpb);
		map.SetAt(arr.GetAt(ii), val);
	}

	return map.GetCount();
}

void CControlWnd::WriteSectionInfo(CString Section, CString file, const CMapStringToString &map)
{
	CString sUserFile;
	CString key, val;

	sUserFile.Format("%s\\%s", m_sUserDir, m_sCfgFile);
	for (POSITION pos = map.GetStartPosition(); pos != NULL; )
	{
		map.GetNextAssoc( pos, key, val );	
		WritePrivateProfileString(Section, key, val, sUserFile);
	}
}

void CControlWnd::FreeMapDll()
{
	/*
	if  (m_hCfg)
	{
		if (m_pCfg)
		{
			m_pCfg->DestroyWindow(); delete m_pCfg;
		}

		FreeLibrary(m_hCfg);
		m_hCfg = NULL;
		m_pCfg = NULL;
	}
	*/
}

BOOL CControlWnd::_SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMap) 
{
	CString sPrivateCfgName(PrivateCfgName), sPublicCfgName(PublicCfgName);

	if (sPrivateCfgName.IsEmpty() && sPublicCfgName.IsEmpty()) return FALSE;

	m_mapSection = sPrivateCfgName;
	m_publicSection = PublicCfgName;
	m_cfgDlg = CfgMap;

	FreeMapDll();
	LoadCfg();

	return TRUE;
}

void CControlWnd::_LoadDefaultCfg() 
{
	// 초기값 읽어오기
	CString sCommonFile;

	sCommonFile.Format("%s\\mtbl\\%s", Variant(homeCC, ""), m_sCfgFile);

	m_publicIdents.RemoveAll();
	m_mapIdents.RemoveAll();
	AddSectionInfo(m_publicSection, sCommonFile, m_publicIdents);
	AddSectionInfo(m_mapSection, sCommonFile, m_mapIdents);
}

void CControlWnd::_SaveCfg() 
{
	SaveCfg();
}

BOOL CControlWnd::_ShowDlg() 
{
	if (m_cfgDlg.IsEmpty())
	{
		AfxMessageBox("[CX_Cfg] ConfigMap이 설정되지 않았습니다. SetMap()");
		return FALSE;
	}

	Variant(pushCC, "ModalResult\tCANCEL");

	CFileFind find;
	CString sRoot, sFile;

	sRoot = Variant(homeCC, "");
	sFile.Format("%s\\dev\\%s.dll", sRoot, m_cfgDlg);
	if (find.FindFile(sFile))
	{
		// DLL 설정화면
		CWnd* (APIENTRY * axCreateDlg)(CWnd*) = nullptr;

		m_hCfg = LoadLibrary(sFile);
		if (m_hCfg)
		{
			axCreateDlg = (CWnd * (APIENTRY*)(CWnd*)) GetProcAddress(m_hCfg, "axCreateDlg");
			if (axCreateDlg)
			{
				m_pCfg = axCreateDlg(this);
				m_pCfg->SendMessage(WM_USER, MAKEWPARAM(WPL_SETROOT, 0), (LPARAM)sRoot.GetString());

				const	int	nResult = m_pCfg->SendMessage(WM_USER, MAKEWPARAM(WPL_SHOWDLG, 0), 0);
				if (nResult)
				{

				}
			}
		}

		if (!m_hCfg || !axCreateDlg)
		{
			m_mapSection = "";
			TRACE("%s 파일은 설정DLL이 아닙니다.", sFile);
			return FALSE;		
		}
	}
	else
	{
		// Wizard Map 설정화면
		CString sMap;
		sMap.Format("%s /t /s /p5 /dcfgPrivateName\t%s\ncfgPublicName\t%s\n", 
			m_cfgDlg, m_mapSection, m_publicSection);
		SetView(typeMODAL, sMap);
	}

	CString sResult;
	sResult = Variant(popCC, "ModalResult"); sResult.TrimRight();
	if (sResult.CompareNoCase("OK") == 0)
	{
		LoadCfg();
		return TRUE;
	}

	return FALSE;
}

BSTR CControlWnd::_ReadCfg(LPCTSTR Ident) 
{
	CString strResult, strIdent(Ident);
	strIdent.MakeUpper();

	if (!m_mapIdents.Lookup(strIdent, strResult))
	{
		if (!m_publicIdents.Lookup(strIdent, strResult))
			strResult = "";
	}
	return strResult.AllocSysString();
}

void CControlWnd::_WriteCfg(LPCTSTR Ident, LPCTSTR Value) 
{
	CString strIdent(Ident), strVal(Value), strResult;
	strIdent.TrimRight();
	if (strIdent.IsEmpty()) return;

	strIdent.MakeUpper();
	strVal.Trim();
	if (m_mapIdents.Lookup(strIdent, strResult))
	{
		m_mapIdents.SetAt(strIdent, strVal);
	}
	else
	{
		if (m_publicIdents.Lookup(strIdent, strResult))
		{
			m_publicIdents.SetAt(strIdent, strVal);
		}
		else
		{
			CString sErr;
			sErr.Format("No search a Key(%s) in Map(%s) Config.", strIdent, m_mapSection);
			AfxMessageBox(sErr);
		}
	}
}

void CControlWnd::_LoadCfg() 
{
	LoadCfg();
}

void CControlWnd::_SetCfgDlg(LPCTSTR CfgMapName) 
{
	m_cfgDlg = CfgMapName;
	m_cfgDlg.Trim();
}

void CControlWnd::_SetPrivateCfgName(LPCTSTR PrivateCfgName) 
{
	m_mapSection = PrivateCfgName;
	m_mapSection.Trim();
}

void CControlWnd::_SetPublicCfgName(LPCTSTR PublicCfgName) 
{
	m_publicSection	= PublicCfgName;
	m_publicSection.Trim();
}
