// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CfgWnd.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILE_CONFIG	"CFG.INI"

/////////////////////////////////////////////////////////////////////////////
// CCfg

CCfg::CCfg()
{
	m_pParent	= nullptr;
	m_publicSection	= "";
	m_mapSection	= "";
	m_cfgDlg	= "";
}

CCfg::~CCfg()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCfg message handlers

void CCfg::CreateCtrl(CWnd* pParent, struct _param* pParam)
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

	CString sSaveCfg, sCommonCfg, sCfgMapName, tmp = m_Param.options;

	m_sUserDir.Format("%s\\user\\%s", Variant(homeCC, ""), Variant(nameCC, ""));
	sSaveCfg = OptionParser(tmp, "/m");
	sCommonCfg = OptionParser(tmp, "/c");
	sCfgMapName = OptionParser(tmp, "/d");
	m_sCfgFile = OptionParser(tmp, "/f");
	m_sCfgFile.TrimRight();
	if (m_sCfgFile.IsEmpty())
		m_sCfgFile = FILE_CONFIG;
	if (!sSaveCfg.IsEmpty())
		SetMap(sSaveCfg, sCommonCfg, sCfgMapName);
}

CString CCfg::Variant(int comm, CString data)
{
	CString	retvalue;
	const	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CCfg::SetView(int type, CString data)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.GetString());
}

CString CCfg::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;

	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CString CCfg::Parser(CString &srcstr, CString substr)
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

void CCfg::LoadCfg()
{
	CString sUserFile, sCommonFile;

	sCommonFile.Format("%s\\mtbl\\%s", Variant(homeCC, ""), m_sCfgFile);
	sUserFile.Format("%s\\%s", m_sUserDir, m_sCfgFile);  

	// 신규추가정보를 읽어들일 수 있도록 CommonFile을 먼저읽는다
	m_publicIdents.RemoveAll();
	m_mapIdents.RemoveAll();
	AddSectionInfo(m_publicSection, sCommonFile, m_publicIdents);
	AddSectionInfo(m_mapSection, sCommonFile, m_mapIdents);

	// 개별 화면의 설정정보를 읽어들어 CommonFile의 정보를 덮어씌운다
	AddSectionInfo(m_publicSection, sUserFile, m_publicIdents);
	AddSectionInfo(m_mapSection, sUserFile, m_mapIdents);
}

void CCfg::SaveCfg(bool SavePrivateOnly)
{
	CString sUserFile;

	sUserFile.Format("%s\\%s", m_sUserDir, m_sCfgFile);  
	if (!SavePrivateOnly && m_publicIdents.GetCount() > 0)
		WriteSectionInfo(m_publicSection, sUserFile, m_publicIdents);

	if (m_mapIdents.GetCount() > 0)
		WriteSectionInfo(m_mapSection, sUserFile, m_mapIdents);
}

int CCfg::AddSectionInfo(CString Section, CString file, CMapStringToString &map)
{
	Section.TrimRight();
	if (Section.IsEmpty())
		return 0;

	char	dat[1024]{};
	char	*pData = dat;

	// Section에 있는 Ident(Key) 모두 추출
	const	int	len = GetPrivateProfileString(Section, NULL, NULL, dat, 1023, file);
	if (len == 0)
		return 0;

	CString val;
	CStringArray arr;

	while (*pData != 0)
	{
		val = CString(pData);
		val.MakeUpper();
	
		arr.Add(val);
		pData = pData + val.GetLength() + 1;
	}

	// 각 Key의 Value값을 읽어 map에 저장
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		GetPrivateProfileString(Section, arr.GetAt(ii), "", dat, sizeof(dat), file);
		map.SetAt(arr.GetAt(ii), CString(dat));
	}

	return map.GetCount();
}

void CCfg::WriteSectionInfo(CString Section, CString file, const CMapStringToString &map)
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

BOOL CCfg::SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMap) 
{
	CString sPrivateCfgName(PrivateCfgName), sPublicCfgName(PublicCfgName);

	if (sPrivateCfgName.IsEmpty() && sPublicCfgName.IsEmpty())
		return FALSE;

	m_mapSection = sPrivateCfgName;
	m_publicSection = PublicCfgName;
	m_cfgDlg = CfgMap;

	LoadCfg();

	return TRUE;
}

void CCfg::LoadDefaultCfg() 
{
	// 초기값 읽어오기
	CString sCommonFile;

	sCommonFile.Format("%s\\mtbl\\%s", Variant(homeCC, ""), m_sCfgFile);
	m_publicIdents.RemoveAll();
	m_mapIdents.RemoveAll();
	AddSectionInfo(m_publicSection, sCommonFile, m_publicIdents);
	AddSectionInfo(m_mapSection, sCommonFile, m_mapIdents);
}

BOOL CCfg::ShowDlg() 
{
	if (m_cfgDlg.IsEmpty())
	{
		AfxMessageBox("[CX_Cfg] ConfigMap이 설정되지 않았습니다. SetMap()");
		return FALSE;
	}

	// Wizard Map 설정화면
	CString sMap;

	sMap.Format("%s /t /s /p5 /dcfgPrivateName\t%s\ncfgPublicName\t%s\n", 
			m_cfgDlg, m_mapSection, m_publicSection);
	SetView(typeMODAL, sMap);
	LoadCfg(); 

	return TRUE;
}

CString CCfg::ReadCfg(LPCTSTR Ident) 
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

void CCfg::WriteCfg(LPCTSTR Ident, LPCTSTR Value) 
{
	CString strIdent(Ident), strVal(Value), strResult;

	strIdent.TrimRight();
	if (strIdent.IsEmpty())
		return;

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
			if (strIdent=="MSGPOPUP")
			{
				m_mapIdents.SetAt(strIdent, strVal);
				return;
			}
			CString sErr;
			sErr.Format("No search a Key(%s) in Map(%s) Config.", strIdent, m_mapSection);
			AfxMessageBox(sErr);
		}
	}
}

void CCfg::SetPrivateCfgName(LPCTSTR PrivateCfgName) 
{
	m_mapSection = PrivateCfgName;
	m_mapSection.Trim();
}

void CCfg::SetPublicCfgName(LPCTSTR PublicCfgName) 
{
	m_publicSection	= PublicCfgName;
	m_publicSection.Trim();
}

BOOL CCfg::GetNoFee()
{
	CString userPath;

	userPath.Format("%s\\user\\%s\\%s", Variant(homeCC, ""), Variant(nameCC, ""), "ORDCFG.INI");
	return (BOOL)GetPrivateProfileInt("Common", "NoFee", 0, userPath);
}
