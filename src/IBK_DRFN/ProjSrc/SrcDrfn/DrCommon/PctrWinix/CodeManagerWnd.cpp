// GwanWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PctrWinix.h"
#include "CodeManagerWnd.h"

#include "../../../SrcSite/include/axisfire.h"	
#include "../../../SrcSite/include/interSt.h"
#include "../../../SrcSite/include/user.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/commonDefines.h" // 0024696: 파생상품 코드체계 변경 제도개선

#define defcode_nKOSPI		2
#define defcode_nKOSDAQ		3
#define defcode_nCALLCODE	5
#define defcode_nPUTCODE	6
#define defcode_nKOPSI200	7
#define defcode_nKOSDAQ50	8
#define defcode_nKRX100		9
#define defcode_nELWCODE	12
#define defcode_nELWBASE	13
#define defcode_nSINJUCODE	14
#define defcode_nGROUPCODE	19
#define defcode_nINTEREST	20

#define defcode_nKOSPIUP	50
#define defcode_nKOSDAQUP	51
#define defcode_nKOSPIex	52
#define defcode_nKOSDAQex	53

#define defcode_nKBc		101
#define defcode_nSECc		102
#define defcode_nSKc		103
#define defcode_nPOSCOc		104
#define defcode_nKTc		105
#define defcode_nKEPCOc		106
#define defcode_nHYUNDAIc	107
#define defcode_nKBp		201
#define defcode_nSECp		202
#define defcode_nSKp		203
#define defcode_nPOSCOp		204
#define defcode_nKTp		205
#define defcode_nKEPCOp		206
#define defcode_nHYUNDAIp	207

struct _exKospi {
	char* code;
	char* name;
} exKospi[] = {
	{"101", "KOSPI200"},	{"107", "건설기계"},
	{"108", "조선운송"},	{"109", "철강소재"},
	{"110", "에너지화학"},	{"111", "정보통신"},
	{"112", "금융"},		{"113", "필수소비재"},
	{"114", "자유소비재"},	{"115", "KOSPI200 동일가중"},
	{"401", "KOSPI100"},	{"402", "KOSPI50"},
	{"403", "KOSPI100 동일가중"},	{"404", "KOSPI50 동일가중"}
};
const int cnt_exKospi = sizeof(exKospi) / sizeof(struct _exKospi);

struct _exKosdaq {
	char* code;
	char* name;
} exKosdaq[] = {
	//{"301", "KOSDAQ50"},		// 20070608
	//	{"242", "프리미어"},
	{"303", "KOSTAR"},
};

const int cnt_exKosdaq = sizeof(exKosdaq) / sizeof(struct _exKosdaq);

int CompareChar( const void *elem1, const void *elem2 ) 
{ 
    CString *str1 = (CString*)elem1;
    CString *str2 = (CString*)elem2;
    return str1->Compare((*str2));
}

int compareSortName( const void *arg1, const void *arg2 )
{
	struct _sortStr* pLVal= (struct _sortStr*)arg1;
	struct _sortStr* pRVal= (struct _sortStr*)arg2;
	
	return pLVal->name.Compare(pRVal->name);
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCodeManagerWnd

CCodeManagerWnd::CCodeManagerWnd()
{
}

CCodeManagerWnd::~CCodeManagerWnd()
{
}

BEGIN_MESSAGE_MAP(CCodeManagerWnd, CWnd)
	//{{AFX_MSG_MAP(CCodeManagerWnd)
	ON_MESSAGE(WM_USER, OnUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCodeManagerWnd::OpenWindow()
{
	CString		szClassName;	
	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);

	BOOL bCreate = CreateEx(0L,														//dwExStyle, 
					(LPCSTR)szClassName,
					"",
					WS_VISIBLE,	// | WS_CHILD, 
					0, 0, 0, 0, 
					HWND_DESKTOP, 
					NULL,
					NULL);
	if(bCreate) 
		ShowWindow(SW_HIDE);

	IMainInfoManager* pManager = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if(pManager)
	{
		m_szRootDir = pManager->GetRootDir();

		loadingHJcode();
		loadingFJcode();
		loadingOJcode();
		loadingPJcode();
		loadingUPcode();
		loadingELWcode();
		loadingSfCode();
		loadingUSCode();
		loadingCFcode();
		loadingMFcode();
		loadingMOcode();
		loadingWOcode();
		loadingRealKey();
	}

	return bCreate;
}

#define	getCODE			0x45		// char* GetCode(_codeR*)
									// lParam : _codeR*
LRESULT CCodeManagerWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	BYTE bt = LOBYTE(LOWORD(wParam));

	switch(bt)
	{
		case getCODE:	return _GetCode(lParam);
		case codeTYPE:	return _GetCodeType(lParam);
		case codeNAME:	return _GetCodeName(lParam);
		default: break;
	}

	return 0L;
}

LRESULT CCodeManagerWnd::_GetCode(LPARAM lParam)
{
	_codeR *pcodeR = (_codeR*)lParam;

	CString strKey, strCode;
	CStringArray arrCode;
	CStringArray arrName;

	if(pcodeR->kind == CDKIND_ELW_BASE)
	{
		strCode.Format("%s", pcodeR->code);
		if(!strCode.IsEmpty())
			GetELWBaseToList(strCode, arrCode, arrName);
		else
			GetELWBaseTreeList(arrCode, arrName);
	}
	else if(pcodeR->kind == CDKIND_ELW_PB)
	{
		strCode.Format("%s", pcodeR->code);
		if(!strCode.IsEmpty())
			GetELWPBToList(strCode, arrCode, arrName);
		else
			GetELWPBTreeList(arrCode, arrName);
	}
	else if(pcodeR->kind == CDKIND_SFUTURE)
	{
		strCode.Format("%s", pcodeR->code);
		if(!strCode.IsEmpty())
			GetSFutCodeToList(strCode, arrCode, arrName);
		else
			GetSFutTreeList(arrCode, arrName);
	}
	else if(pcodeR->kind == CDKIND_INDEX_KOSTAR)
	{
		arrCode.RemoveAll();
		arrName.RemoveAll();

		arrCode.Add("303");
		arrName.Add("KOSTAR");
	}
	else if(pcodeR->kind == CDKIND_FOREIGN)
	{
		strCode.Format("%s", pcodeR->code);
		GetForeignToList(atoi(strCode), arrCode, arrName);
	}
	else if(pcodeR->kind == CDKIND_FOREIGN_ALL)
	{
		arrCode.RemoveAll();
		arrName.RemoveAll();

		uscode   stCode; 
		CString code, name;
		
		for (int i = 0; i < m_uscode.GetSize(); i++)
		{
			stCode = m_uscode.GetAt(i);
			
			code = CString(stCode.symb, 16);
			name = CString(stCode.knam, 40);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
	else if(pcodeR->kind == CDKIND_FOREIGN_MAJOR)
	{
		GetForeignMajorToList(arrCode, arrName);
	}
	else if(pcodeR->kind == CDKIND_INDEX_CODECHANGE)
	{
		strCode.Format("%s", pcodeR->code);
		if(!strCode.IsEmpty())
			m_strData = GetRecentChxUpCodeList(strCode);

		return (LRESULT)(LPCSTR)m_strData;
	}
	else if(pcodeR->kind == CDKIND_ETN)
		GetEtnToList(arrCode, arrName);	
	else if(pcodeR->kind == CDKIND_VF)
		GetEtnToList(arrCode, arrName);	
	else if(pcodeR->kind == CDKIND_SF)
	{
		strCode.Format("%s", pcodeR->code);
		if(!strCode.IsEmpty())
			GetSF2CodeToList(strCode, arrCode, arrName);
		else
			GetSF2TreeList(arrCode, arrName);
	}
	else
	{
		strCode.Format("%s", pcodeR->code);
		if(!strCode.IsEmpty())
			GetIBKMasterData(atoi(strCode), arrCode, arrName);
		else
		{
			strKey.Format("%d", pcodeR->kind);
			GetIBKMasterData(atoi(strKey), arrCode, arrName);
		}
	}

	if(arrCode.GetSize() <= 0)
		return 0L;

	CString strData(""), strTemp;
	for(int i = 0; i < arrCode.GetSize(); i++)
	{
		if(i == 0)
		{
			strTemp.Format("%s", arrCode.GetAt(i));
			strTemp += '\t';
			strTemp.Format("%s%s", strTemp, arrName.GetAt(i));
			strTemp += '\t';
		}
		else
		{
			strTemp.Format("%s%s", strTemp, arrCode.GetAt(i));
			strTemp += '\t';
			strTemp.Format("%s%s", strTemp, arrName.GetAt(i));
			strTemp += '\t';
		}
		strData += strTemp;
		strTemp = "";
	}
	m_strData = strData;

	return (LRESULT)(LPCSTR)m_strData;
}

LRESULT CCodeManagerWnd::_GetCodeType(LPARAM lParam)
{
	LPCSTR szItemCode = (LPCSTR)lParam;

	return 0;
}

LRESULT CCodeManagerWnd::_GetCodeName(LPARAM lParam)
{
	LPCSTR szItemCode = (LPCSTR)lParam;

	if(strstr(szItemCode, "@") != NULL || strstr(szItemCode, "$") != NULL)
	{
		uscode   stCode; 
		CString code, name;
		
		for (int i = 0; i < m_uscode.GetSize(); i++)
		{
			stCode = m_uscode.GetAt(i);
			code = CString(stCode.symb, 16);

			if(strstr(szItemCode, code))
				m_strData = CString(stCode.knam, 40);
		}
	}
	else
	{
		SFCODE scode;
		m_mapsfcode.Lookup(szItemCode, scode);

		m_strData = scode.hnam;
	}

	return (LRESULT)(LPCSTR)m_strData;
}

void CCodeManagerWnd::loadingHJcode()
{
	if (m_hjcode.GetSize() > 0) return;

	m_maphjcode.RemoveAll();

	CFile	fileH;
	CString	filePath;
	struct  hjcodex  hjcode;
	struct	_shjcode s_hjcode;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, HJCODEx);
  	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("종목 파일을 읽을 수 없습니다.");
		return;
	}

	int countC = fileH.GetLength() / sizeof(struct hjcodex);

	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&hjcode, sizeof(struct hjcodex));

		m_maphjcode.SetAt(hjcode.code, hjcode);

		s_hjcode.name = CString(hjcode.hnam, HNameLen);
		int idx = s_hjcode.name.Find('\0');
		if (idx != -1) s_hjcode.name = s_hjcode.name.Left(idx);
		s_hjcode.name.MakeUpper(); s_hjcode.name.TrimRight();
//		s_hjcode.name = s_hjcode.name.Mid(1, s_hjcode.name.GetLength());

		s_hjcode.symb = CString(hjcode.symb, HSymbolLen);
		s_hjcode.symb.TrimRight();

		s_hjcode.ecng = hjcode.ecng;
		s_hjcode.size = hjcode.size;
		strcpy(s_hjcode.ucdm , hjcode.acdm);
		strcpy(s_hjcode.ucds , hjcode.acds);
		s_hjcode.jjug = hjcode.jjug;
		s_hjcode.kpgb = hjcode.kpgb;
		s_hjcode.ucmd = hjcode.ucmd;
		s_hjcode.kosd = (char)hjcode.kosd;
		s_hjcode.ssgb = hjcode.ssgb;
		s_hjcode.wsgb = hjcode.wsgb;
		s_hjcode.jsiz = hjcode.jsiz;
		s_hjcode.itgb = hjcode.itgb;
		s_hjcode.star = hjcode.star;
		s_hjcode.unio = hjcode.unio;
		s_hjcode.prmr = hjcode.prmr;//prmr->kqpg
		s_hjcode.uc10 = hjcode.uc10;
		s_hjcode.uc20 = hjcode.uc20;
		s_hjcode.sec1 = hjcode.sec1;
		s_hjcode.sec2 = hjcode.sec2;
		s_hjcode.sec3 = hjcode.sec3;
		s_hjcode.sec4 = hjcode.sec4;
		s_hjcode.sec5 = hjcode.sec5;

		switch (hjcode.ssgb)
		{
		case jmSINJU:
		case jmSINJS:
		case jmHYFND:
			s_hjcode.code = CString(hjcode.code, HCodeLen + 2);
			break;
		default:
			s_hjcode.code = CString(hjcode.code, HCodeLen + 2).Mid(1);
			break;
		}

		switch (s_hjcode.ssgb)
		{
		case jmHYFND:
//			continue;
		default:
			m_hjcode.Add(s_hjcode);
			break;
		}
    }

	//이부분 추가
	QSortCArray( m_hjcode, CompareChar ); 
	fileH.Close();
}

void CCodeManagerWnd::loadingFJcode()
{
	if (m_fjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  fjcode  fjcode;
	struct	_sjcode s_jcode;

	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, FJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("선물 파일을 읽을 수 없습니다.");
		return;
	}

	int countC = fileH.GetLength() / sizeof(struct fjcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&fjcode, sizeof(struct fjcode));

		s_jcode.code = CString(fjcode.cod2, FCodeLen);
		s_jcode.name = CString(fjcode.hnam, FNameLen);

		m_fjcode.Add(s_jcode);
	}

	fileH.Close();
}

void CCodeManagerWnd::loadingOJcode()
{
	if (m_ojcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;

	struct  ojcodh  ojcodH;
	struct  ojcode  ojcode;

	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, OJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("옵션 파일을 읽을 수 없습니다.");
		return;
	}

	fileH.Read(&ojcodH, sizeof(struct ojcodh));

	int countC = (fileH.GetLength() - sizeof(struct ojcodh)) / sizeof(struct ojcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&ojcode, sizeof(struct ojcode));
		m_ojcode.Add(ojcode);
	}

	fileH.Close();
}

void CCodeManagerWnd::loadingPJcode()
{
	if (m_pjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  pjcode  pjcode;

	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, PJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("현물옵션 파일을 읽을 수 없습니다.");
		return;
	}

	int countC = fileH.GetLength() / sizeof(struct pjcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&pjcode, sizeof(struct pjcode));
		m_pjcode.Add(pjcode);
	}

	fileH.Close();
}

void CCodeManagerWnd::loadingUPcode()
{
	if (m_upcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  upcode  upcode;

	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, UPCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("업종코드 파일을 읽을 수 없습니다.");
		return;
	}

	int countC = fileH.GetLength() / sizeof(struct upcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&upcode, sizeof(struct upcode));
		m_upcode.Add(upcode);
	}

	fileH.Close();
}

void CCodeManagerWnd::loadingELWcode()
{
	if (m_elwbase.GetSize() > 0)
		return;

	int	codeN;
	CFile	file;
	_sjcode	sjcode;
	struct  elwcode  ELWCode;
	_elwdata elwdata;
	PBCODE pbcode;
	int idx;
	CString strELWCode;
	struct  hjcodex  hjcode;

	CString path = m_szRootDir + "\\tab\\elwcod.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))	return;

	codeN = file.GetLength() / sizeof(struct elwcode);
	
	bool	bFound;
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));
		
		elwdata.code = CString(ELWCode.codx, sizeof(ELWCode.codx));
		elwdata.code.TrimRight();	
		elwdata.name.Format("%s", CString(ELWCode.hnam, ELWHNameLen).Mid(1));
		elwdata.name.TrimRight(); elwdata.name.TrimLeft();
		elwdata.ktype = CString(ELWCode.krye, 2);
		elwdata.pcode = CString(ELWCode.bgce, 12);
		elwdata.pname = CString(ELWCode.hbge, 20);
		elwdata.mjmt  = CString(ELWCode.mjmt, 12);

		for (int jj = 0; jj < 5; jj ++)
		{
			elwdata.bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12);
			elwdata.bcode[jj].TrimRight();
		}

		// ELW마스터에는 있고 HJCODE마스터에는 없는 경우가 있어서 찾아준다.
		strELWCode = "J" + elwdata.code;
		if(m_maphjcode.Lookup(strELWCode, hjcode))
			m_ELWdata.Add(elwdata);
		
		idx = FindPBCode(elwdata.pcode);
		
		if (idx < 0)
		{
			pbcode.code = elwdata.pcode;
			pbcode.name = elwdata.pname;
			m_arrayPBCode.Add(pbcode);
		}
		
		for (int jj = 0; jj < 5; jj ++)
		{
			CString bcode = CString((char*)ELWCode.gcjs[jj], 12);
			bcode.TrimRight();
			if (bcode == "") break;
			
			bcode = bcode.Mid(1);
			
			bFound = false;
			for (int kk = 0; kk < m_elwbase.GetSize(); kk++)
			{
				if (bcode.CompareNoCase(m_elwbase.GetAt(kk).code) == 0)
				{
					bFound = true;
					break;
				}
			}
			
			if (!bFound)
			{
				CString	bname = GetCodeName(bcode);
				if (!bname.IsEmpty())	
					;
				else if (bcode.CompareNoCase("OSPI200") == 0)
					continue;
				else if (bcode.CompareNoCase("ASKET") == 0)
					break;
				else
					continue;
				
				
				sjcode.code = bcode;
				sjcode.name = bname;
				
				sjcode.name.TrimRight();
				
				m_elwbase.Add(sjcode);
			}
		}
	}
	
	QSortCArray( m_elwbase, compareSortName); 
	
	file.Close();
}

void CCodeManagerWnd::loadingCFcode()
{
	if (m_ccode.GetSize() > 0) return;
	
	CFile	fileH;
	CString	filePath;
	struct  ccode  stCode;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, "cfcode.dat");
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("상품선물 파일을 읽을 수 없습니다.");
		return;
	}
	
	int countC = fileH.GetLength() / sizeof(struct ccode);
	CString strCode;
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&stCode, sizeof(struct ccode));

		strCode = CString(stCode.jcod, sizeof(stCode.jcod));
		strCode.TrimLeft();
		strCode.TrimRight();

		if(!strCode.CompareNoCase("GLD") || !strCode.CompareNoCase("MGD") || !strCode.CompareNoCase("LHG"))
			continue ;

		m_ccode.Add(stCode);
    }

	fileH.Close();
}

void CCodeManagerWnd::loadingRealKey()
{
	if(m_mapRealKey.GetCount() > 0) return;

	m_mapRealKey.RemoveAll();
	
	CFile	fileH;
	CString	filePath, strRealKey, strUpCode;
	struct  _RealKey  RealKey;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, "rupcod.dat");
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("rupcod.dat 파일을 읽을 수 없습니다.");
		return;
	}
	
	int countC = fileH.GetLength() / sizeof(struct _RealKey);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&RealKey, sizeof(struct _RealKey));
		strRealKey = CString(RealKey.rcod, 5);
		strRealKey.Format("%s", strRealKey.Mid(1, strRealKey.GetLength()));
		strUpCode = CString(RealKey.ucod, 3);

		m_mapRealKey.SetAt(strRealKey, strUpCode);
	}
	
	fileH.Close();
}

int CCodeManagerWnd::FindPBCode(CString code)
{
	bool bRegistered = false;
	PBCODE pbcode;
	for (int ii = 0; ii < m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		if (pbcode.code == code)
		{
			return ii;
		}
	}
	return -1;
}

void CCodeManagerWnd::loadingSfCode()
{
	if(m_mapsfcode.GetCount() > 0) return;

	int	codeN = 0;

	CFile	file;
	struct  sfcode  SFCode;
	SFCODE		scode;

 	m_mapsfinfo.RemoveAll();
 	m_sfbase.RemoveAll();
 	m_mapsfcode.RemoveAll();

	int		nFirst = 0;
	CString path, sztmp = _T("");
  	
	path = m_szRootDir + "\\tab\\sfcode.dat";

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
 		AfxMessageBox("주식선물코드 파일을 읽을 수 없습니다.");
		return ;
	}

	codeN = file.GetLength() / sizeof(struct sfcode);

	SFCODEINFO sfinfo;
	SFCODEBASE sfbase;
	CMapStringToString mapDate;
	CString preCodx;
	int nIndex = 0;

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&SFCode, sizeof(struct sfcode));
		
		scode.codx = CString(SFCode.codx, 8);
		scode.hnam = CString(SFCode.hnam, 50);
		scode.enam = CString(SFCode.enam, 50);
		scode.mchk = SFCode.mchk;
		scode.gcod = CString(SFCode.gcod, 12);
		scode.gnam = CString(SFCode.gnam, 32);

		m_mapsfcode.SetAt(scode.codx,scode);

		scode.gcod.TrimRight();
		scode.gnam.TrimRight();

		sfbase.basecodx = scode.gcod;
		sfbase.basenam = scode.gnam;

		if(ii == 0)
		{
			preCodx = scode.gcod;
			nIndex = 0;
			m_sfbase.Add(sfbase);
		}

		if(preCodx != scode.gcod)
		{
			if(nIndex < 16)
			{
				for(int a=nIndex;a<17;a++)
				{
					sfinfo.codx[a] = "";
					sfinfo.nam[a] = "";
				}

				m_mapsfinfo.SetAt(preCodx,sfinfo);
			}

			m_sfbase.Add(sfbase);
			nIndex = 0;	
		}

		sfinfo.codx[nIndex] = scode.codx;
		sfinfo.nam[nIndex] = scode.hnam;
		sfinfo.nam[nIndex].TrimLeft();
		nIndex++;

		preCodx = scode.gcod;

		m_mapsfinfo.SetAt(scode.gcod,sfinfo);
	}

	file.Close();
}

void CCodeManagerWnd::loadingUSCode()
{
	if (m_uscode.GetSize() > 0) return;
	
	CFile	fileH;
	CString	filePath;
	struct  _uscode  stCode;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, "uscode.dat");
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("해외종목 파일을 읽을 수 없습니다.");
		return;
	}
	
	int countC = fileH.GetLength() / sizeof(struct _uscode);
	
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&stCode, sizeof(struct _uscode));
		m_uscode.Add(stCode);
    }
	
	fileH.Close();
}

void CCodeManagerWnd::loadingMFcode()
{
	if (m_mfcode.GetSize() > 0) return;
	
	CFile	fileH;
	CString	filePath;
	struct  fjcode  fjcode;
	struct	_sjcode s_jcode;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, MFCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		// IBK의 요청으로 인하여 주석처리함.
		//AfxMessageBox("미니 선물 파일을 읽을 수 없습니다.");
		return;
	}
	
	int countC = fileH.GetLength() / sizeof(struct fjcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&fjcode, sizeof(struct fjcode));
		
		s_jcode.code = CString(fjcode.cod2, FCodeLen);
		s_jcode.name = CString(fjcode.hnam, FNameLen);
		
		m_mfcode.Add(s_jcode);
	}
	
	fileH.Close();
}

void CCodeManagerWnd::loadingMOcode()
{
	if (m_mocode.GetSize() > 0) return;
	
	CFile	fileH;
	CString	filePath;
	
	struct  ojcodh  ojcodH;
	struct  ojcode  ojcode;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, MOCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		// IBK의 요청으로 인하여 주석처리함.
		//AfxMessageBox("옵션 파일을 읽을 수 없습니다.");
		return;
	}
	
	fileH.Read(&ojcodH, sizeof(struct ojcodh));
	
	int countC = (fileH.GetLength() - sizeof(struct ojcodh)) / sizeof(struct ojcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&ojcode, sizeof(struct ojcode));
		m_mocode.Add(ojcode);
	}
	
	fileH.Close();
}

void CCodeManagerWnd::loadingWOcode()
{
	if (m_wocode.GetSize() > 0) return;
	
	CFile	fileH;
	CString	filePath;
	
	struct  ojcodh  ojcodH;
	struct  ojcode  ojcode;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, "wocode.dat");
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		// IBK의 요청으로 인하여 주석처리함.
		//AfxMessageBox("옵션 파일을 읽을 수 없습니다.");
		return;
	}
	
	fileH.Read(&ojcodH, sizeof(struct ojcodh));
	
	int countC = (fileH.GetLength() - sizeof(struct ojcodh)) / sizeof(struct ojcode);
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&ojcode, sizeof(struct ojcode));
		m_wocode.Add(ojcode);
	}
	
	fileH.Close();
}


CString CCodeManagerWnd::GetCodeName(CString code)
{
	code = code.Left(12);
	code.TrimRight();
	code.TrimLeft();
//	code.Remove(0);
	
//	int codeL = code.GetLength();
	int codeL = strlen(code);

	if (codeL == 6)					// kospi code
	{
		_shjcode   hjcode; 
		for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
		{
			hjcode = m_hjcode.GetAt(ii);
			if (code != hjcode.code)
				continue;

			switch (hjcode.kosd)
			{
			case jmKOSPI:
			case jmKOSDAQ:
			case jm3RD:
				if (hjcode.ssgb == jmELW)
					return hjcode.name;
				break;
			}
			return hjcode.name.Mid(1);
		}

	}
	else if (codeL == 9)				// 신주인수권 추가
	{
		_shjcode   hjcode; 
		for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
		{
			hjcode = m_hjcode.GetAt(ii);
			if (code != hjcode.code)
				continue;
			
			switch (hjcode.kosd)
			{
			case jmSINJU:
				if (hjcode.ssgb == jmSINJU)
					return hjcode.name;
				break;
			}
			return hjcode.name.Mid(1);
		}
	}
	else if (codeL <= 3)				// upjong code
	{
		struct	upcode	upcode;
		for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
		{
			upcode = m_upcode.GetAt(ii);

			CString upCodeStr;
			int upCodeInt;
			upCodeStr.Format("%d%02d", upcode.jgub, upcode.ucod);
			upCodeInt = atoi(upCodeStr);

			if (atoi(code) != upCodeInt)
				continue;

			return CString(upcode.hnam, UNameLen);
		}
	}
	else if (IS_FUT_CODE_1(code[0]) || IS_SPREAD_CODE_1(code[0]))	// future code // 0024696: 파생상품 코드체계 변경 제도개선
	{
		_sjcode   sjcode;
		for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
		{
			sjcode = m_fjcode.GetAt(ii);
			if (code != sjcode.code)
				continue;

			return sjcode.name;
		}

	}
	else if (IS_CALL_CODE_1(code[0]) || IS_PUT_CODE_1(code[0])) // 0024696: 파생상품 코드체계 변경 제도개선
	{
		if (code[1] == '0')			// future option code
		{
			struct ojcode	ojcode;
			for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
			{
				ojcode = m_ojcode.GetAt(ii);

				for (int jj = 0; jj < 11; jj++)
				{
					if (ojcode.call[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.call[jj].cod2, OCodeLen))
						continue;

					return CString(ojcode.call[jj].hnam, ONameLen);
				}
				for (int jj = 0; jj < 11; jj++)
				{
					if (ojcode.put[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.put[jj].cod2, OCodeLen))
						continue;

					return CString(ojcode.put[jj].hnam, ONameLen);
				}
			}
		}
		else					// 
		{
			struct	pjcode pjcode;
			for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
			{
				pjcode = m_pjcode.GetAt(ii);
				if (code != CString(pjcode.codx, PCodeLen))
					continue;

				return CString(pjcode.hnam, sizeof(pjcode.hnam));
			}
		}
	}

	return _T("");
}

void CCodeManagerWnd::GetIBKMasterData(UINT selItem, CStringArray &arrCode, CStringArray &arrName)
{
	CString itemname;
	CString cg_item, result_item;

	switch (selItem)
	{
	case CDKIND_ALL:			// all_coode
		GetAllCodeToList(arrCode, arrName);
		break;
	case defcode_nKOSPI:
		GetKospiToList(arrCode, arrName);
		break;
	case defcode_nKOSDAQ:
		GetKosdaqToList(arrCode, arrName);
		break;
	case CDKIND_FUTURE_K200:
		GetFutureToList(arrCode, arrName);
		break;
	case CDKIND_OPTION_K200:
		GetCallCodeToList(arrCode, arrName);
// 		break;
// 	case defcode_nPUTCODE:
		GetPutCodeToList(arrCode, arrName);
		break;
	case CDKIND_MINI_FUTURE:
		GetMiniFutureToList(arrCode, arrName);
		break;
	case CDKIND_MINI_OPTION:
		GetMiniCallCodeToList(arrCode, arrName);
		GetMiniPutCodeToList(arrCode, arrName);
		break;
	case CDKIND_WEEKLY_OPTION:
		GetWeeklyCallCodeToList(arrCode, arrName);
		GetWeeklyPutCodeToList(arrCode, arrName);
		break;
	case defcode_nKOPSI200:
		GetKospi200ToList(arrCode, arrName);
		break;
	case defcode_nKOSDAQ50:
		GetKosdaq50ToList(arrCode, arrName);
		break;
	case defcode_nKRX100:
		GetKRX100ToList(arrCode, arrName);
		break;
	case CDKIND_FREEBOARD:
		GetFreeCodeToList(arrCode, arrName);
		break;
	case CDKIND_ETF:
		GetETFCodeToList(arrCode, arrName);
		break;
	case defcode_nELWBASE:
		GetELWBaseToList("", arrCode, arrName);
		break;
		
	case defcode_nSINJUCODE:
		GetSinjuCodeToList(arrCode, arrName);
		break;
	case CDKIND_INDEX_KOSPI:
		GetKospiJisuToList(arrCode, arrName);
		break;
	case CDKIND_INDEX_KOSDAQ:
		GetKosdaqJisuToList(arrCode, arrName);
		break;
	case CDKIND_KOSPI:
		GetKospiUpList(arrCode, arrName);
		break;
	case CDKIND_KOSDAQ:
		GetKosdaqUpList(arrCode, arrName);
		break;
	case CDKIND_KRX:
		GetKRXUpList(arrCode, arrName);
		break;
	case CDKIND_KNX:
		GetKNXToList(arrCode, arrName);
		break;
	case CDKIND_INDEX_KRX:
		GetKRXJisuToList(arrCode, arrName);
		break;
	case CDKIND_INDEX_KNX:
		GetKNXUpList(arrCode, arrName);
		break;
	case CDKIND_THEMELIST:
		GetThemaList(arrCode, arrName);
		break;
	case CDKIND_INDEX_K200:
		GetKOSPI200Index(arrCode, arrName);
		break;
	case CDKIND_INDEX_K100:
		GetKOSPI100Index(arrCode, arrName);
		break;
	case CDKIND_FOREIGN_INDEX:		// 해외지수 트리리스트
		GetForeignIndexTreeList(arrCode, arrName);
		break;
	case CDKIND_FOREIGN_ITEM:		// 해외종목 트리리스트
		GetForeignItemTreeList(arrCode, arrName);
		break;
	case CDKIND_FOREIGN_FUTURE:		// 해외선물 트리리스트
		GetForeignFutureTreeList(arrCode, arrName);
		break;
	case CDKIND_FOREIGN_RATE:		// 해외금리 트리리스트
		GetForeignRateTreeList(arrCode, arrName);
		break;
	case CDKIND_FOREIGN_DEBT:		// 해외국채 트리리스트
		GetForeignDebtTreeList(arrCode, arrName);
		break;
	case CDKIND_FOREIGN_EXCHANGE:	// 해외 환율 트리리스트
		GetForeignExchangeTreeList(arrCode, arrName);
		break;
	case CDKIND_KOFEXLIST:			// 상품선물 트리리스트
		GetKOFEXTreeList(arrCode, arrName);
		break;
	case CDKIND_INDEX_FREEBOARD:
		GetFreeUpToList(arrCode, arrName);
		break;
	case CDKIND_ETN:
		GetEtnToList(arrCode, arrName);
		break;
	case CDKIND_VF:
		GetVFToList(arrCode, arrName);
		break;
	case CDKIND_SF:
		GetSF2UpList(arrCode, arrName);
		break;
		
	case defcode_nKBc: case defcode_nKBp:
		GetPjcodeToList("10", selItem == defcode_nKBc ? '2' : '3', arrCode, arrName);
		break;
	case defcode_nSECc: case defcode_nSECp:
		GetPjcodeToList("11", selItem == defcode_nSECc ? '2' : '3', arrCode, arrName);
		break;
	case defcode_nSKc: case defcode_nSKp:
		GetPjcodeToList("12", selItem == defcode_nSKc ? '2' : '3', arrCode, arrName);
		break;
	case defcode_nPOSCOc: case defcode_nPOSCOp:
		GetPjcodeToList("13", selItem == defcode_nPOSCOc ? '2' : '3', arrCode, arrName);
		break;
	case defcode_nKTc: case defcode_nKTp:
		GetPjcodeToList("14", selItem == defcode_nKTc ? '2' : '3', arrCode, arrName);
		break;
	case defcode_nKEPCOc: case defcode_nKEPCOp:
		GetPjcodeToList("15", selItem == defcode_nKEPCOc ? '2' : '3', arrCode, arrName);
		break;
	case defcode_nHYUNDAIc: case defcode_nHYUNDAIp:
		GetPjcodeToList("16", selItem == defcode_nHYUNDAIc ? '2' : '3', arrCode, arrName);
		break;
		
	default:
		if (selItem <= 0)
			break;
		
		switch (selItem / 100)
		{
		case defcode_nKOSPIUP:		// 코스피 업종리스트
			GetKospiUpToList(selItem, arrCode, arrName);
			break;
		case defcode_nKOSDAQUP:		// 코스닥 업종리스트
			GetKosdaqUpToList(selItem, arrCode, arrName);
			break;
		case defcode_nINTEREST:
			GetInterestToList(selItem, arrCode, arrName);
			break;	
		case CDKIND_KRX:			// KRX 업종리스트
			GetKRXToList(selItem, arrCode, arrName);
			break;
		case CDKIND_KOFEX:			// 상품선물 종목
			GetKOFExCodeToList(selItem, arrCode, arrName);
			break;
		}
		
		switch (selItem / 1000)
		{
		case defcode_nKOSPIex:
			GetKospiExToList(selItem, arrCode, arrName);
			break;
		case defcode_nKOSDAQex:
			GetKosdaqExToList(selItem, arrCode, arrName);
			break;
		case CDKIND_THEME:
			GetThemaCodeToList(selItem, arrCode, arrName);
			break;
		case defcode_nGROUPCODE:
			GetGroupCodeToList(selItem, arrCode, arrName);
			break;
		}
	}
}

void CCodeManagerWnd::GetAllCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKospiToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKosdaqToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSDAQ:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKospi200ToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
			if (hjcode.kpgb >= 1)
				break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKosdaq50ToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSDAQ:
			if (hjcode.kpgb >= 1)
				break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKRX100ToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.unio != 1)
			continue;

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetFreeCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jm3RD)
			continue;

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetELWPBToList(CString strTile, CStringArray &arrCode, CStringArray &arrName)
{
	_elwdata elwdata;
	CString code, name, ktype, pcode, pname, mjmt;
	CString rdocheck;

	rdocheck.Format("%02d", 3);
	
	int nelwdata = m_ELWdata.GetSize();
	for(int ii = 0; ii < nelwdata; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
		
		code = CString(elwdata.code, ELWCodeLen);
		name = CString(elwdata.name, ELWHNameLen);
		ktype = CString(elwdata.ktype, 2);
		pcode = CString(elwdata.pcode, 12);
		pname = CString(elwdata.pname, 20);
		mjmt = CString(elwdata.mjmt, 12);
		
		if(pname.Compare(strTile) == 0 && rdocheck.Compare("03") == 0)
		{
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetELWBaseToList(CString strTitle, CStringArray &arrCode, CStringArray &arrName)
{
	_elwdata elwdata;
	CString code, name, ktype, pcode, pname, mjmt, bcode[5];
	CString rdocheck;
	CString bscode, bsname, basename;
	_sjcode sjcode;
	
	rdocheck.Format("%02d", 03);
	
	int nelwdata = m_ELWdata.GetSize();
	int jj = 0;
	for(int ii = 0; ii < nelwdata; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
		
		code = CString(elwdata.code, ELWCodeLen);
		name = CString(elwdata.name, ELWHNameLen);
		ktype = CString(elwdata.ktype, 2);
		pcode = CString(elwdata.pcode, 12);
		pname = CString(elwdata.pname, 20);
		mjmt = CString(elwdata.mjmt, 12);
		
		for ( jj = 0; jj < 5; jj ++)
		{
			bcode[jj] = CString(elwdata.bcode[jj], 12);
			bcode[jj].TrimRight();
			bcode[jj] = bcode[jj].Mid(1);
		}
		
		if(bcode[0].Compare("OSPI200") == 0)
		{
			basename = "KOSPI200";
		}
		else if(bcode[0].Compare("ASKET") == 0)
		{
			basename = "BASKET";	
		}
		else
		{
			int nelwbase = m_elwbase.GetSize();
			for(jj = 0; jj < nelwbase; jj++)
			{
				sjcode = m_elwbase.GetAt(jj);
				bscode = sjcode.code;
				bsname = sjcode.name;
				
				if(bscode.Compare(bcode[0]) == 0)
				{
					basename = bsname;
					break;
				}
			}
		}
		
		if(basename.Compare(strTitle) == 0 && rdocheck.Compare("03") == 0)
			AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetETFCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjcode.ssgb != jmETF)
				continue;
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetSinjuCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
				break;
			default:
				continue;
			}
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetFutureToList(CStringArray &arrCode, CStringArray &arrName)
{
	_sjcode   scode;
	CString	code, name;

	for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
	{
		scode = m_fjcode.GetAt(ii);

		code = CString(scode.code, FCodeLen);
		name = CString(scode.name, FNameLen);

		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;

		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetCallCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct ojcode	ojcode;
	CString	code, name;

	for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	{
		ojcode = m_ojcode.GetAt(ii);

		for (int jj = 0; jj < 11; jj++)
		{
			if (ojcode.call[jj].yorn != '1')
				continue;

			code = CString(ojcode.call[jj].cod2, OCodeLen);
			name = CString(ojcode.call[jj].hnam, ONameLen);

			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetPutCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct ojcode	ojcode;
	CString	code, name;

	for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	{
		ojcode = m_ojcode.GetAt(ii);

		for (int jj = 0; jj < 11; jj++)
		{
			if (ojcode.put[jj].yorn != '1')
				continue;

			code = CString(ojcode.put[jj].cod2, OCodeLen);
			name = CString(ojcode.put[jj].hnam, ONameLen);

			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetKospiJisuToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	int ii = 0;
	for ( ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);

		if (upcode.jgub != upKOSPI)
			continue;

		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		AppendItemToList1(code, name, arrCode, arrName);
	}

	for (ii = 0; ii < cnt_exKospi; ii++)
	{
		code = exKospi[ii].code;
		name = exKospi[ii].name;

		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKosdaqJisuToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;

	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);

		if (upcode.jgub != upKOSDAQ)
			continue;

		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		AppendItemToList1(code, name, arrCode, arrName);
	}
/*
	for (ii = 0; ii < cnt_exKosdaq; ii++)
	{
		code = exKosdaq[ii].code;
		name = exKosdaq[ii].name;

		AppendItemToList1(code, name, arrCode, arrName);
	}
*/
}

void CCodeManagerWnd::GetKospiUpList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	int ii = 0;
	for ( ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub != upKOSPI)
			continue;
		
		code.Format("%02d%02d", defcode_nKOSPIUP, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		AppendItemToList1(code, name, arrCode, arrName);
	}
	
	for (ii = 0; ii < cnt_exKospi; ii++)
	{
		code.Format("%02d%s", defcode_nKOSPIex, exKospi[ii].code);
		name = exKospi[ii].name;
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKosdaqUpList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub != upKOSDAQ)
			continue;
		
		code.Format("%02d%02d", defcode_nKOSDAQUP, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		AppendItemToList1(code, name, arrCode, arrName);
	}
	/*
	for (ii = 0; ii < cnt_exKosdaq; ii++)
	{
		code.Format("%02d%s", defcode_nKOSDAQex, exKosdaq[ii].code);
		name = exKosdaq[ii].name;
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
	*/
}

void CCodeManagerWnd::GetKRXUpList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub == 6)
		{
			code.Format("%02d%02d", CDKIND_KRX, upcode.ucod);
			name =  CString(upcode.hnam, UNameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
		else if(upcode.jgub == 9)
		{
			code.Format("%02d%02d", CDKIND_KRX, upcode.ucod);
			name = CString(upcode.hnam, UNameLen);

			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetKRXJisuToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub == 6)
		{
			code.Format("%01d%02d", (int)upcode.jgub, upcode.ucod);
			name =  CString(upcode.hnam, UNameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
		else if(upcode.jgub == 9)
		{
			code.Format("%01d%02d", (int)upcode.jgub, upcode.ucod);
			name = CString(upcode.hnam, UNameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetKNXToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	 CString code, name;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjcode.ssgb != jmKONEX)
				continue;
			break;
		default:
			continue;
		}

		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKNXUpList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = m_upcode.GetSize(); ii >= 0; --ii)
	{
		upcode = m_upcode.GetAt(ii);

		if (upcode.jgub == 5 && upcode.ucod == 1)
		{
			code.Format("%d%02d", (int)upcode.jgub, upcode.ucod);
			name =  CString(upcode.hnam, UNameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);

			break;
		}
	}
}

void CCodeManagerWnd::GetThemaList(CStringArray &arrCode, CStringArray &arrName)
{
	int readL = 0; char readB[256];
	CString	filePath, string, stringx, data, skey;
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, "themcd.ini");
	
	for (int ii = 0; ii < 200; ii++)
	{
		skey.Format("%02d", ii);
		readL = GetPrivateProfileString(_T("FOLDER"), skey, "", readB, sizeof(readB), filePath);
		if (readL <= 0) break;
		
		string = CString(readB, readL); 
		stringx = parseX(string, ";"); 
		stringx.TrimRight();
		data.Format("%02d%03d", CDKIND_THEME, atoi(string));

		arrCode.Add(data);
		arrName.Add(stringx);
	}
}

void CCodeManagerWnd::GetPjcodeToList(CString code, char kind, CStringArray &arrCode, CStringArray &arrName)
{
	struct	pjcode pjcode;
	CString	codx, name, tjgb;
	for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
	{
		pjcode = m_pjcode.GetAt(ii);

		codx = CString(pjcode.codx, PCodeLen);
		tjgb = CString(pjcode.tjgb, 2);

		if (codx.GetAt(0) == kind && tjgb == code)
		{
			name = CString(pjcode.hnam, sizeof(pjcode.hnam));

			AppendItemToList1(codx, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetKospiUpToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	int kind = selItem % 100;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSPI)
			continue;

		switch (hjcode.ssgb)
		{
		case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
			continue;
		}

		append = false;

		if (kind == 1)
		{
			append = true;
		}
		else if (kind < 5)
		{
			if ((int)hjcode.size == kind)
				append = true;
		}
		else if (kind < 27)
		{
			if ((int)hjcode.ucds == kind || (int)hjcode.ucdm == kind)
				append = true;
		}
		else if (kind == 27)
		{
			if ((int)hjcode.jjug == kind)
				append = true;
		}
		else if (kind < 41)
		{
			if ((int)hjcode.jsiz == kind)
				append = true;
		}
		else if (kind == 41)
		{
			if((int)hjcode.jsiz == kind)
				append = true;
		}
		else if (kind == 42)
		{
			if ((int)hjcode.wsgb == 1)
				append = true;
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKosdaqUpToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	int kind = selItem % 100;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSDAQ)
			continue;

		switch (hjcode.ssgb)
		{
		case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
			continue;
		}

		append = false;

		if (kind == 1)
		{
			append = true;
		}
		else if (kind == 2)
		{
			if ((int)hjcode.jjug == kind)
				append = true;
		}
		else if (kind < 5)
		{
			if ((int)hjcode.size == kind)
				append = true;
		}
		else if (kind == 5)
		{
			if ((int)hjcode.size == 4 && (int)hjcode.jjug == 2)
				append = true;
		}
		else if (kind < 15 || kind == 41)
		{
			if ((int)hjcode.ucdm == kind)
				append = true;
		}
		else if (kind < 38)
		{
			if ((int)hjcode.ucds == kind)
				append = true;
		}
		else if (kind < 41)
		{
			if ((int)hjcode.jsiz == kind - 37)
				append = true;
		}
		else if (kind == 42)
		{
			if ((int)hjcode.prmr == kind - 41)
				append = true; 
		}
		else if(kind == 43)	// 우량기업
		{
			if ((int)hjcode.uc20 == 1)
				append = true;
		}
		else if(kind == 44)	// 벤처기업
		{
			if ((int)hjcode.uc20 == 2)
				append = true;
		}
		else if(kind == 45)	// 중견기업
		{
			if ((int)hjcode.uc20 == 4)
				append = true;
		}
		else if(kind == 46)	// 신성장기업
		{
			if ((int)hjcode.uc20 == 8)
				append = true;
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKospiExToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	int kind = selItem % 1000;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSPI)
			continue;

		append = false;

		switch (kind)
		{
		case 101: // KOSPI200
		case 115: // KOSPI200 동일가중
			if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2 || (int)hjcode.kpgb == 1)
				append = true;
			break;
		case 107: // 건설기계
			if ((int)hjcode.uc10 == 1)
				append = true;
			break;
		case 108: // 조선운송
			if ((int)hjcode.uc10 == 2)
				append = true;
			break;
		case 109: // 철강소재
			if ((int)hjcode.uc10 == 3)
				append = true;
			break;
		case 110: // 에너지화학
			if ((int)hjcode.uc10 == 4)
				append = true;
			break;
		case 111: // 정보통신
			if ((int)hjcode.uc10 == 5)
				append = true;
			break;
		case 112: // 금융
			if ((int)hjcode.uc10 == 6)
				append = true;
			break;
		case 113: // 필수소비재
			if ((int)hjcode.uc10 == 7)
				append = true;
			break;
		case 114: // 자유소비재
			if ((int)hjcode.uc10 == 8)
				append = true;
			break;
		case 401: // KOSPI100
		case 403: // KOSPI100 동일가중
			if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2)
				append = true;
			break;
		case 402: // KOSPI50
		case 404: // KOSPI50 동일가중
			if ((int)hjcode.kpgb == 3)
				append = true;
			break;
// 		case 403: // KOSPI IT
// 			if ((int)hjcode.itgb == 1)
// 				append = true;
// 			break;
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKosdaqExToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	int kind = selItem % 1000;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSDAQ)
			continue;

		append = false;

		switch (kind)
		{
		case 301: // KOSDAQ50
			if ((int)hjcode.kpgb == 1)
				append = true;
			break;
		case 302: // KOSDAQ IT
			if ((int)hjcode.itgb == 1)
				append = true;
			break;
		case 303: // KOSTAR
			if ((int)hjcode.star == 1)
				append = true;
			break;				
		}

		if (!append) continue;

		code = hjcode.code;
		name = hjcode.name;

		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetThemaCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	CString	skey; skey.Format("%03d", selItem % 1000);

	char	readB[512];
	UINT	readL;
	CString	filePath, code, name, string = _T("");

	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, "themcd.ini");

	readL = GetPrivateProfileString(_T("TMCODE"), skey, "", readB, sizeof(readB), filePath);
	if (readL > 0) string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		code = parseX(string, ";");
		name = GetCodeName(code);

		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetGroupCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	CString	skey; skey.Format("%03d", selItem % 1000);
	
	char	readB[512];
	UINT	readL;
	CString	filePath, code, name, string = _T("");
	
	filePath.Format("%s/%s/%s", m_szRootDir, TABDIR, "gropcd.ini");
	
	readL = GetPrivateProfileString(_T("GRCODE"), skey, "", readB, sizeof(readB), filePath);
	if (readL > 0) string = CString(readB, readL);
	
	while (!string.IsEmpty())
	{
		code = parseX(string, ";");
		name = GetCodeName(code);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetInterestToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	int gno = selItem % 100;
	
	CString	filePath;
	CString szUserDir = GetUserDir();

	if (ExistFile(gno, true))
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_szRootDir, USRDIR, szUserDir, gno);
	else if (ExistFile(gno))
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_szRootDir, USRDIR, szUserDir, gno);
	else
		return ;
	
	UINT	readL;
	CString	code, name;
	struct	_inters interest;
	
	CFile	fileH(filePath, CFile::modeRead);
	if (fileH.m_hFile == CFile::hFileNull)
		return ;
	
	int nMaxJONGMOK = 100;
	for (int ii = 0; ii < nMaxJONGMOK; ii++)
	{
		readL = fileH.Read(&interest, sz_inters);
		if (readL < sz_inters)
			break;
		
		code = CString(interest.code, sizeof(interest.code));
		name = GetCodeName(code);//interest.name;
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
	fileH.Close();
}

void CCodeManagerWnd::GetSFutCodeToList(CString strKey, CStringArray &arrCode, CStringArray &arrName)
{
	if(m_mapsfinfo.GetCount() <= 0)
		return ;

	SFCODEINFO sfinfo;
	m_mapsfinfo.Lookup(strKey, sfinfo);

	for(int i = 0; i < 17; i++)
	{
		arrCode.Add(sfinfo.codx[i]);
		arrName.Add(sfinfo.nam[i]);
	}
}

void CCodeManagerWnd::GetELWBaseTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	if(m_elwbase.GetSize() <= 0)
		return ;

	arrCode.Add("KOSPI200");
	arrName.Add("KOSPI200");
	arrCode.Add("BASKET");
	arrName.Add("BASKET");
	
	_sjcode sjcode;
	for(int i = 0; i < m_elwbase.GetSize(); i++)
	{
		sjcode = m_elwbase.GetAt(i);
		
		arrCode.Add(sjcode.name);
		arrName.Add(sjcode.name);
	}
}

void CCodeManagerWnd::GetELWPBTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	if(m_arrayPBCode.GetSize() <= 0)
		return ;

	PBCODE stPbcode;
	for(int i = 0; i < m_arrayPBCode.GetSize(); i++)
	{
		stPbcode = m_arrayPBCode.GetAt(i);

		arrCode.Add(stPbcode.name);
		arrName.Add(stPbcode.name);
	}
}

void CCodeManagerWnd::GetSFutTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	if(m_sfbase.GetSize() <= 0)
		return ;

	SFCODEBASE sfBase;

	for(int i = 0; i < m_sfbase.GetSize(); i++)
	{
		sfBase = m_sfbase.GetAt(i);
		arrCode.Add(sfBase.basecodx);
		arrName.Add(sfBase.basenam);
	}
}

void CCodeManagerWnd::GetKRXToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	int kind = selItem % 100;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		append = false;
		
		switch (kind)
		{
		case 1: // KRX 자동차
			if ((int)hjcode.sec1 & SEC1_AT)
				append = true;
			break;
		case 2: // KRX 반도체
			if ((int)hjcode.sec1 & SEC1_SM)
				append = true;
			break;
		case 3: // KRX 건강
			if ((int)hjcode.sec1 & SEC1_HC)
				append = true;
			break;
		case 4: // KRX 은행
			if ((int)hjcode.sec1 & SEC1_BK)
				append = true;
			break;
		case 5: // KRX 정보통신
			if ((int)hjcode.sec2 & SEC2_IT)
				append = true;
			break;
		case 6: // KRX 화학에너지
			if ((int)hjcode.sec2 & SEC2_EC)
				append = true;
			break;
		case 7: // KRX 철강
			if ((int)hjcode.sec2 & SEC2_ST)
				append = true;
			break;
		case 8: // KRX 필수소비재
			if ((int)hjcode.sec2 & SEC2_CS)
				append = true;
			break;
		case 9: // KRX 미디어통신
			if ((int)hjcode.sec3 & SEC3_MT)
				append = true;
			break;
		case 10: // KRX 건설
			if ((int)hjcode.sec3 & SEC3_CO)
				append = true;
			break;
		case 11: // KRX 금융서비스
			if ((int)hjcode.sec3 & SEC3_FB)
				append = true;
			break;
		case 12: // KRX 증권
			if ((int)hjcode.sec3 & SEC3_SI)
				append = true;
			break;
		case 13: // KRX 조선
			if ((int)hjcode.sec4 & SEC4_SC)
				append = true;
			break;
		case 14: // KRX SRI
			if ((int)hjcode.sec4 & SEC4_SR)
				append = true;
			break;
		case 15: // KRX 보험
			if ((int)hjcode.sec4 & SEC4_IS)
				append = true;
			break;
		case 16: // KRX 운송
			if ((int)hjcode.sec4 & SEC4_TP)
				append = true;
			break;
		case 17: // KRX 소비자유통
			if ((int)hjcode.sec5 & SEC5_RT)
				append = true;
			break;
		case 18: // KRX 레저엔터테인먼트
			if ((int)hjcode.sec5 & SEC5_LS)
				append = true;
			break;
		case 19: // KRX100 동일가중
			if ((int)hjcode.sec5 & SEC5_KX)
				append = true;
			break;
		case 99: // KRX통합지수
			if (hjcode.unio == 1)
				append = true;
			break;
		}
		
		if (!append) continue;
		
		code = hjcode.code;
		name = hjcode.name;
		
		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetKOSPI200Index(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub == 1)
		{
			code.Format("%01d%02d", (int)upcode.jgub, upcode.ucod);
			name =  CString(upcode.hnam, UNameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetKOSPI100Index(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub == 4)
		{
			code.Format("%01d%02d", (int)upcode.jgub, upcode.ucod);
			name =  CString(upcode.hnam, UNameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetForeignIndexTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	CString	code, name;
	CStringArray arrTreeName;
	arrTreeName.Add("미국");
	arrTreeName.Add("아시아");
	arrTreeName.Add("유럽");
	arrTreeName.Add("MSCI");
	arrTreeName.Add("기타");

	code.Format("%02d%01d%01d", CDKIND_FOREIGN_INDEX, 0, 9);
	arrCode.Add(code);
	arrName.Add("전체");

	for (int i = 0; i < arrTreeName.GetSize(); i++)
	{
		code.Format("%02d%01d%01d", CDKIND_FOREIGN_INDEX, 0, i+1);
		name = arrTreeName.GetAt(i);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetForeignItemTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	CString	code, name;
	CStringArray arrTreeName;
	arrTreeName.Add("미국");	arrTreeName.Add("일본");
	arrTreeName.Add("홍콩");	arrTreeName.Add("대만");
	arrTreeName.Add("영국");	arrTreeName.Add("프랑스");
	arrTreeName.Add("독일");	arrTreeName.Add("ADR");
	arrTreeName.Add("GDR");
	
	code.Format("%02d%01d%01d", CDKIND_FOREIGN_ITEM, 1, 9);
	arrCode.Add(code);
	arrName.Add("전체");

	for (int i = 0; i < arrTreeName.GetSize(); i++)
	{
		code.Format("%02d%01d%01d", CDKIND_FOREIGN_ITEM, 1, i+1);
		name = arrTreeName.GetAt(i);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetForeignFutureTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	CString	code, name;
	CStringArray arrTreeName;
	arrTreeName.Add("CME지수선물");
	arrTreeName.Add("에너지");
	arrTreeName.Add("상품선물");
	
	code.Format("%02d%01d%01d", CDKIND_FOREIGN_FUTURE, 2, 9);
	arrCode.Add(code);
	arrName.Add("전체");

	for (int i = 0; i < arrTreeName.GetSize(); i++)
	{
		code.Format("%02d%01d%01d", CDKIND_FOREIGN_FUTURE, 2, i+1);
		name = arrTreeName.GetAt(i);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetForeignRateTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	CString	code, name;
	CStringArray arrTreeName;
	arrTreeName.Add("리보금리");
	arrTreeName.Add("미국금리");
	
	code.Format("%02d%01d%01d", CDKIND_FOREIGN_RATE, 3, 9);
	arrCode.Add(code);
	arrName.Add("전체");

	for (int i = 0; i < arrTreeName.GetSize(); i++)
	{
		code.Format("%02d%01d%01d", CDKIND_FOREIGN_RATE, 3, i+1);
		name = arrTreeName.GetAt(i);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetForeignDebtTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	CString	code, name;
	CStringArray arrTreeName;
	arrTreeName.Add("미국");
	arrTreeName.Add("일본");
	arrTreeName.Add("영국");
	arrTreeName.Add("독일");
	
	code.Format("%02d%01d%01d", CDKIND_FOREIGN_DEBT, 4, 9);
	arrCode.Add(code);
	arrName.Add("전체");

	for (int i = 0; i < arrTreeName.GetSize(); i++)
	{
		code.Format("%02d%01d%01d", CDKIND_FOREIGN_DEBT, 4, i+1);
		name = arrTreeName.GetAt(i);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetForeignExchangeTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	CString code;

	code.Format("%02d%01d%01d", CDKIND_FOREIGN_EXCHANGE, 5, 9);
	arrCode.Add(code);
	arrName.Add("전체");
}

void CCodeManagerWnd::GetForeignToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	CString strKey;
	strKey.Format("%02d", selItem % 100);

	int ndatg = 0, njsgb = 0;

	ndatg = atoi(strKey.Left(1)) + 48;
	njsgb = atoi(strKey.Right(1)) + 48;

	uscode   stCode; 
	CString code, name;

	for (int i = 0; i < m_uscode.GetSize(); i++)
	{
		stCode = m_uscode.GetAt(i);

		if(stCode.datg != ndatg)
			continue;

		if(stCode.jsgb == njsgb)
		{
			code = CString(stCode.symb, 16);
			name = CString(stCode.knam, 40);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
		else if(stCode.datg == 53 && njsgb == 57)	// 환율
		{
			code = CString(stCode.symb, 16);
			name = CString(stCode.knam, 40);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
		else if(njsgb == 57)						// 전체
		{
			code = CString(stCode.symb, 16);
			name = CString(stCode.knam, 40);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetKOFEXTreeList(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();
	
	if(m_ccode.GetSize() <= 0)
		return ;
	
	ccode stCode;
	CString code, name;
	int ntjgb = 0;
	
	for(int i = 0; i < m_ccode.GetSize(); i++)
	{
		stCode = m_ccode.GetAt(i);

		if(atoi(stCode.tjgb) != ntjgb && atoi(stCode.tjgb) != 0)
		{
			code.Format("%d%02d", CDKIND_KOFEX, atoi(stCode.tjgb));
			name = CString(stCode.jnam, 40);

			AppendItemToList1(code, name, arrCode, arrName);

			ntjgb = atoi(stCode.tjgb);
		}
	}
}

void CCodeManagerWnd::GetKOFExCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName)
{
	int nKind = selItem % 100;

	ccode stCode;
	CString code, name;

	for(int i = 0; i < m_ccode.GetSize(); i++)
	{
		stCode = m_ccode.GetAt(i);

		if(atoi(stCode.tjgb) == nKind)
		{
			code = CString(stCode.codx, 8);
			name = CString(stCode.hnam, 50);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetForeignMajorToList(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	arrCode.Add("DJI@DJI");		arrName.Add("다우존스 산업지수");

	arrCode.Add("NAS@IXIC");	arrName.Add("나스닥종합 산업지수");

	arrCode.Add("SPI@SPX");		arrName.Add("S&P500 지수");

	arrCode.Add("CME$SP");		arrName.Add("S&P500 선물");

	arrCode.Add("CME$ES");		arrName.Add("Mini S&P500 선물");

	arrCode.Add("CME$ND");		arrName.Add("나스닥100 선물");

	arrCode.Add("CME$NQ");		arrName.Add("Mini 나스닥100 선물");

	arrCode.Add("NII@NI225");	arrName.Add("니케이225");

	arrCode.Add("HSI@HSI");		arrName.Add("항셍지수");

	arrCode.Add("TWS@TI01");	arrName.Add("대만가권지수");

	arrCode.Add("SHS@000002");	arrName.Add("중국Shanghai A Share");

	arrCode.Add("LNS@FTSE100");	arrName.Add("영국 FTSE100");

	arrCode.Add("XTR@DAX30");	arrName.Add("독일 Xetra DAX");

	arrCode.Add("PAS@CAC40");	arrName.Add("프랑스 CAC40");
}

void CCodeManagerWnd::GetFreeUpToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub != upFREEBOARD)
			continue;
		
		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

CString CCodeManagerWnd::GetRecentChxUpCodeList(CString strCode)
{
	LPCSTR szRealKey = strCode;
	CString	strUpcode;
	
	m_mapRealKey.Lookup(szRealKey, strUpcode);

	return strUpcode;
}

void CCodeManagerWnd::AppendItemToList1(CString code, CString name, CStringArray &arrCode, CStringArray &arrName)
{
	int nItem = 0;
	
	name.TrimRight();

	arrCode.Add(code);
	arrName.Add(name);
}

CString CCodeManagerWnd::parseX(CString &srcstr, CString substr)
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

bool CCodeManagerWnd::ExistFile(int gno, bool temp /*=false*/)
{
	CString	filePath;
	CString szUserDir = GetUserDir();

	if (temp)
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_szRootDir, USRDIR, szUserDir, gno);
	else
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_szRootDir, USRDIR, szUserDir, gno);
	
	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;
	
	return true;
}

LPCSTR CCodeManagerWnd::GetUserDir()
{
	if(m_szUserDir.IsEmpty())
	{
		IMainInfoManager* pManager = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
		if(pManager)
		{
			m_szUserDir = pManager->GetUserID();
		}
	}
	return (LPCTSTR)m_szUserDir;
}

void CCodeManagerWnd::GetEtnToList(CStringArray &arrCode, CStringArray &arrName)
{
	_shjcode   hjcode; 
	CString code, name;
	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjcode.ssgb != jmETN)
				continue;
			break;
		default:
			continue;
		}
		
		code = CString(hjcode.code, HCodeLen);
		name = CString(hjcode.name, HNameLen);
		
		AppendItemToList1(code, name.Mid(1), arrCode, arrName);
	}
}

void CCodeManagerWnd::GetVFToList(CStringArray &arrCode, CStringArray &arrName)
{

}

void CCodeManagerWnd::GetSF2UpList(CStringArray &arrCode, CStringArray &arrName)
{
	
}

void CCodeManagerWnd::GetSF2CodeToList(CString strKey, CStringArray &arrCode, CStringArray &arrName)
{
	if(m_mapsfinfo.GetCount() <= 0)
		return ;
	
	SFCODEINFO sfinfo;
	m_mapsfinfo.Lookup(strKey, sfinfo);
	
	for(int i = 0; i < 17; i++)
	{
		arrCode.Add(sfinfo.codx[i]);
		arrName.Add(sfinfo.nam[i]);
	}
}

void CCodeManagerWnd::GetSF2TreeList(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();
	
	if(m_sfbase.GetSize() <= 0)
		return ;
	
	SFCODEBASE sfBase;
	
	for(int i = 0; i < m_sfbase.GetSize(); i++)
	{
		sfBase = m_sfbase.GetAt(i);
		arrCode.Add(sfBase.basecodx);
		arrName.Add(sfBase.basenam);
	}
}

void CCodeManagerWnd::GetETCUpToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct upcode	upcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);
		
		if (upcode.jgub != upETC)
			continue;
		
		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetMiniFutureToList(CStringArray &arrCode, CStringArray &arrName)
{
	_sjcode   scode;
	CString	code, name;
	
	for (int ii = 0; ii < m_mfcode.GetSize(); ii++)
	{
		scode = m_mfcode.GetAt(ii);
		
		code = CString(scode.code, FCodeLen);
		name = CString(scode.name, FNameLen);
		
		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;
		
		AppendItemToList1(code, name, arrCode, arrName);
	}
}

void CCodeManagerWnd::GetMiniCallCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct ojcode	ojcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_mocode.GetSize(); ii++)
	{
		ojcode = m_mocode.GetAt(ii);
		
		for (int jj = 0; jj < 11; jj++)
		{
			if (ojcode.call[jj].yorn != '1')
				continue;
			
			code = CString(ojcode.call[jj].cod2, OCodeLen);
			name = CString(ojcode.call[jj].hnam, ONameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetMiniPutCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct ojcode	ojcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_mocode.GetSize(); ii++)
	{
		ojcode = m_mocode.GetAt(ii);
		
		for (int jj = 0; jj < 11; jj++)
		{
			if (ojcode.put[jj].yorn != '1')
				continue;
			
			code = CString(ojcode.put[jj].cod2, OCodeLen);
			name = CString(ojcode.put[jj].hnam, ONameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}


void CCodeManagerWnd::GetWeeklyCallCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct ojcode	ojcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_wocode.GetSize(); ii++)
	{
		ojcode = m_wocode.GetAt(ii);
		
		for (int jj = 0; jj < 11; jj++)
		{
			if (ojcode.call[jj].yorn != '1')
				continue;
			
			code = CString(ojcode.call[jj].cod2, OCodeLen);
			name = CString(ojcode.call[jj].hnam, ONameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}

void CCodeManagerWnd::GetWeeklyPutCodeToList(CStringArray &arrCode, CStringArray &arrName)
{
	struct ojcode	ojcode;
	CString	code, name;
	
	for (int ii = 0; ii < m_wocode.GetSize(); ii++)
	{
		ojcode = m_wocode.GetAt(ii);
		
		for (int jj = 0; jj < 11; jj++)
		{
			if (ojcode.put[jj].yorn != '1')
				continue;
			
			code = CString(ojcode.put[jj].cod2, OCodeLen);
			name = CString(ojcode.put[jj].hnam, ONameLen);
			
			AppendItemToList1(code, name, arrCode, arrName);
		}
	}
}