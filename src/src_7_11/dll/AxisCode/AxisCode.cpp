// AxisCode.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AxisCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILE_FJCODE		"fjcode.dat" // 선물코드
#define FILE_COCODE		"cocode.dat" // 상품코드
#define FILE_OPCODE		"opcode.dat" // 옵션코드
#define FILE_UPCODE		"upcode.dat" // 업종코드

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CAxisCodeApp

BEGIN_MESSAGE_MAP(CAxisCodeApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisCodeApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisCodeApp construction

CGlobal Global;

CAxisCodeApp::CAxisCodeApp()
{
	// TODO: add construction code here,
	m_RootPath = _T("");
	m_Myfolder = _T("");

	// 64bit 변환
	SetDialogBkColor(USER_BACKCOLOR);
	// Place all significant initialization in InitInstance
}

CAxisCodeApp::~CAxisCodeApp()
{
	INT_PTR ii = 0;
	for (ii = m_arrayHJCode.GetSize() - 1; ii >= 0 ; ii--)
		SAFE_DELETE(m_arrayHJCode[ii]);
	m_arrayHJCode.RemoveAll();

	struct JCode* jcode;
	for (ii = m_arrayDlgJCode->GetSize() - 1; ii >= 0 ; ii--)
	{
		jcode = (struct JCode*)m_arrayDlgJCode->GetAt(ii);
		if (jcode->pInf)
			delete (_elwcode*)jcode->pInf;
		
		delete jcode;
	}
	m_arrayDlgJCode->RemoveAll();
	SAFE_DELETE(m_arrayDlgJCode);
	
	for (ii = m_arrayUSCode.GetSize() - 1; ii >= 0 ; ii--)
		SAFE_DELETE(m_arrayUSCode[ii]);
	m_arrayUSCode.RemoveAll();
	
	for (ii = m_arrayCJCode.GetSize() - 1; ii >= 0 ; ii--)
		SAFE_DELETE(m_arrayCJCode[ii]);
	m_arrayCJCode.RemoveAll();

	for (ii = m_arrayUBCode.GetSize() - 1; ii >= 0 ; ii--)
		SAFE_DELETE(m_arrayUBCode[ii]);
	m_arrayUBCode.RemoveAll();
	
	for (ii = m_arrayFDCode.GetSize() - 1; ii >= 0 ; ii--)
		SAFE_DELETE(m_arrayFDCode[ii]);
	m_arrayFDCode.RemoveAll();

	m_arrayUpcode.RemoveAll();
	m_arrayFcode.RemoveAll();
	m_arrayOcode.RemoveAll();
	m_ArrayOMonth.RemoveAll();
	m_arraySFcode.RemoveAll();
	m_arrayPcode.RemoveAll();

	m_arrayELWCode.RemoveAll();
	struct _elwcode* ncode;
	for (ii = 0; ii < m_arrayDlgELWCode.GetSize(); ii++)
	{
		ncode = m_arrayDlgELWCode.GetAt(ii);
		delete ncode;
	}	
	m_arrayDlgELWCode.RemoveAll();
	m_arrayElwMonth.RemoveAll();
	m_arrayBASECode.RemoveAll();
	m_arrayPBCode.RemoveAll();
}

BOOL CAxisCodeApp::loadFCode()
{
	if (m_arrayFcode.GetSize() > 0)
		return FALSE;

	int	codeN;

	CFile	file;
	struct  fjcode  FJCode;
	FCODE fcode;

	CString path = m_RootPath + "/tab/" + FILE_FJCODE;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("종목 화일을 읽을 수 없습니다.");
		return FALSE;
	}

	codeN = (int)file.GetLength() / sizeof(struct fjcode);

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&FJCode, sizeof(struct fjcode));

		fcode.code = CString(FJCode.cod2, FCodeLen);
		fcode.name = CString(FJCode.hnam, FNameLen);
		fcode.mchk = FJCode.mchk;
		
		m_arrayFcode.Add(fcode);
	}

	file.Close();

	return TRUE;
}

BOOL CAxisCodeApp::loadCOCode()
{
	if (m_arrayCOcode.GetSize() > 0)
		return FALSE;

	int	codeN;

	CFile	file;
	struct  cocode  COCode;
	COCODE cocode;

	CString path = m_RootPath + "/tab/" + FILE_COCODE;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("종목 화일을 읽을 수 없습니다.");
		return FALSE;
	}

	codeN = (int)file.GetLength() / sizeof(struct cocode);

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&COCode, sizeof(struct cocode));
		cocode.codx = CString(COCode.codx, COCodxLen);
		cocode.hnam = CString(COCode.hnam, COHnamLen);

		cocode.enam = CString(COCode.enam, COEnamLen);
		cocode.filler = COCode.filler;
		cocode.tjgb = CString(COCode.tjgb, COTjgbLen);
		cocode.gcjs = CString(COCode.gcjs, COGcjsLen);
		cocode.gnam = CString(COCode.gnam, COGnamLen);
		cocode.cmgb = COCode.cmgb;
		cocode.fill = CString(COCode.fill, COFillLen);

		m_arrayCOcode.Add(cocode);
	}

	file.Close();

	return TRUE;
}

BOOL CAxisCodeApp::loadOJCode()
{
	if (m_arrayOcode.GetSize() > 0)
		return FALSE;

	m_ArrayOMonth.RemoveAll();
	int	codeN, len;

	CFile	file;
	struct  ojcodh  OJCodh;
	struct  ojcode  OJCode;

	CString path = m_RootPath + "/tab/" + FILE_OPCODE;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxMessageBox("종목 화일을 읽을 수 없습니다.");
		return FALSE;
	}

	len = file.Read(&OJCodh, sizeof(struct ojcodh));

	for (int jj = 0; jj < 4; jj++)
	{
		CString str = CString(OJCodh.cjym[jj], 6).Right(4);
		m_ArrayOMonth.Add(str);
	}

	codeN = (int)(file.GetLength() - len) / sizeof(struct ojcode);

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));
		m_arrayOcode.Add(OJCode);
	}
	
	file.Close();
	return TRUE;
}

BOOL CAxisCodeApp::LoadUPCode()
{
	int	codeN;

	CFile	file;
	UPCODE  UPcode;
	struct upcode code;
	CString path; 

	path = m_RootPath + "\\tab\\" + FILE_UPCODE;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return FALSE;
	
	codeN = (int)file.GetLength()/sizeof(upcode);

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&code, sizeof(struct upcode));

		UPcode.kind = code.jgub;			// 장구분...
		UPcode.ucod.Format("%d", code.ucod);		// 업종코드 ...
		UPcode.name = CString(code.hnam, UNameLen);	// 한글명...
		m_arrayUpcode.Add(UPcode);
	}

	file.Close();

	return TRUE;
}

BOOL CAxisCodeApp::LoadUSCode()
{
	TRY
	{
		CString		path = m_RootPath + "/tab/" + USCODE_FILE;
		CFile		file;
		if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
			return FALSE;

		_USCode		usCode;
		UINT		nBytesRead;
		do
		{
			nBytesRead = file.Read(&usCode, sizeof(usCode));
			if (nBytesRead == sizeof(usCode))
			{
				_USCode *pUSCode = new _USCode;
				CopyMemory(pUSCode, &usCode, sizeof(usCode));
				m_arrayUSCode.Add(pUSCode);
			}
		} while((int)nBytesRead);

		file.Close();
	} CATCH(CFileException, e)
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
		#endif
	}
	END_CATCH
		
	return TRUE;
}


BOOL CAxisCodeApp::LoadUBCode()
{
	CString path;
	int codeN;
	
	CFile file;
	path = m_RootPath + "\\tab\\" + CJCODE_FILE;   
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return FALSE;
	
	codeN = (int)file.GetLength() / sizeof(struct cjcode);
	
	for (int ii = 0; ii < codeN; ii++)
	{
		
	}
	
	file.Close();
	return TRUE;
}



BOOL CAxisCodeApp::LoadAGCode()
{
	CString path, tmps;
	int codeN;
	struct agcode code;
	AGCODE* acode = NULL;
	CFile file;
	
	path = m_RootPath + "\\tab\\" + _T("issorgcd.dat");   
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	//if (!file.Open(path, CFile::modeRead))
		return false;

	int ilen = (int)file.GetLength();
	codeN = (int)file.GetLength() / sizeof(struct agcode);
	char* pdata = NULL;
	CString strtemp, tmpS;
	m_arrayAGCode.RemoveAll();

	for (int ii = 0; ii < codeN; ii++)
	{
		acode = new AGCODE;
		memset(acode, 0, L_agcode);
		
		file.Read(&code, sizeof(struct agcode));

		tmps = CString(code.code, 6);
		tmps.TrimRight();
		tmps.TrimLeft();
		memcpy(acode->code, code.code, 6);

		tmps = CString(code.name, 50);
		tmps.TrimRight();
		tmps.TrimLeft();
		memcpy(acode->name, tmps, tmps.GetLength());

		tmps = CString(code.up, 2);
		tmps.TrimRight();
		memcpy(acode->up, tmps, tmps.GetLength());

		//memcpy(acode->gubn, code.gubn, 1);
		acode->gubn = code.gubn;

		tmps = CString(code.balcode, 14);
		tmps.TrimRight();
		memcpy(acode->balcode, tmps, tmps.GetLength());
		/*
		if(code.gubn == '0')
			acode->gubn = 'N';
		else
			acode->gubn = 'Y';
			*/
		m_arrayAGCode.Add(acode);
	}
	
	return TRUE;
}

BOOL CAxisCodeApp::LoadCJCode()
{
	CString path, tmps;
	int codeN;
	struct CJcode code;
	CJCODE* cjCode;
	CJCODESTR* cjstrCode = NULL;
	CFile file;
	
	path = m_RootPath + "\\tab\\" + _T("IssBondMast.txt");   
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return false;
	
	int ilen = (int)file.GetLength();
	codeN = (int)file.GetLength() / sizeof(struct CJcode);
	char* pdata = NULL;
	CString strtemp, tmpS;

	m_arrayCJCode.RemoveAll();

	m_arrlist.RemoveAll();

	m_arrayGukCode.RemoveAll();
	m_arrayTongCode.RemoveAll();
	m_arrayTukCode.RemoveAll();
	m_arrayJiCode.RemoveAll();
	m_arrayGumCode.RemoveAll();
	m_arrayHeuiCode.RemoveAll();
	m_arrayMyCode.RemoveAll();
	m_arrayDangCode.RemoveAll();
	m_arrayBalCode.RemoveAll();

	m_arrGuklist.RemoveAll();
	m_arrTonglist.RemoveAll();
	m_arrTuklist.RemoveAll();
	m_arrJilist.RemoveAll();
	m_arrGumlist.RemoveAll();
	m_arrHeuilist.RemoveAll();
	m_arrMylist.RemoveAll();
	m_arrDanglist.RemoveAll();
	m_arrBallist.RemoveAll();

	for (int ii = 0; ii < codeN; ii++)
	{
		cjCode = new CJCODE;
		cjstrCode = new CJCODESTR;

		memset(cjCode, 0, L_CJcode);
		
		file.Read(&code, sizeof(struct CJcode));
		
		memcpy(cjCode->code, code.code, 12);
		tmps = CString(code.name, 100);
		tmps.TrimRight();
		if(tmps.Find(_T("ELS")) >= 0 )
			continue;
		memcpy(cjCode->name, tmps, tmps.GetLength());
		memcpy(cjCode->kind, code.kind, 4);
		memcpy(cjCode->idct, code.idct, 6);
		tmps = CString(code.idct, 6);
		memcpy(cjCode->valday, code.valday, 10);
		memcpy(cjCode->mtday, code.mtday, 10);
		memcpy(cjCode->mtry, code.mtry, 6);
		memcpy(cjCode->itrt, code.itrt, 10);
		memcpy(cjCode->intertype, code.intertype, 6);
		memcpy(cjCode->credit, code.credit, 6);
		memcpy(cjCode->minfore, code.minfore, 10);
		memcpy(cjCode->minthree, code.minthree, 10);
		tmps = CString(code.update, 14);
		memcpy(cjCode->update, tmps, 14);
		
		m_arrayCJCode.Add(cjCode);

		cjstrCode->code = CString(cjCode->code, 12);
		cjstrCode->name = CString(cjCode->name, 100);
		cjstrCode->kind = CString(cjCode->kind, 4);
		cjstrCode->idct = CString(cjCode->idct, 6);
		cjstrCode->valday = CString(cjCode->valday, 10);
		cjstrCode->mtday = CString(cjCode->mtday, 10);
		cjstrCode->mtry = CString(cjCode->mtry, 6);
		cjstrCode->itrt = CString(cjCode->itrt, 10);
		cjstrCode->intertype = CString(cjCode->intertype, 6);
		cjstrCode->credit = CString(cjCode->credit, 6);
		cjstrCode->minfore = CString(cjCode->minfore, 10);
		cjstrCode->minthree = CString(cjCode->minthree, 10);
		cjstrCode->update = CString(cjCode->update, 14);

		m_arrlist.Add(cjstrCode);

		switch(atoi(cjstrCode->kind))
		{
			case 1000:  //국채
				{
					m_arrayGukCode.Add(cjCode);
					m_arrGuklist.Add(cjstrCode);
				}
				break;
			case 4000:  //지방
				{
					m_arrayJiCode.Add(cjCode);
					m_arrJilist.Add(cjstrCode);
				}
				break;
			case 3000:  //특수
				{
					m_arrayTukCode.Add(cjCode);
					m_arrTuklist.Add(cjstrCode);
				}
				break;
			case 2000:  //통안
				{
					m_arrayTongCode.Add(cjCode);
					m_arrTonglist.Add(cjstrCode);
				}
				break;
			case 5000:  //금융
				{
					m_arrayGumCode.Add(cjCode);
					m_arrGumlist.Add(cjstrCode);
				}
				break;
			case 6000:  //회사
				{
					m_arrayHeuiCode.Add(cjCode);
					m_arrHeuilist.Add(cjstrCode);
				}
				break;
		}
	}

	file.Close();
	return true;
}

BOOL CAxisCodeApp::LoadFDCode()
{
	TRY
	{
		CString		path = m_RootPath + "/tab/" + FDCODE_FILE;
		CFile		file;
		if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
			return FALSE;

		_FDCode		FDCode;
		UINT		nBytesRead;
		do
		{
			nBytesRead = file.Read(&FDCode, sizeof(FDCode));
			if (nBytesRead == sizeof(FDCode))
			{
				_FDCode *pFDCode = new _FDCode;
				CopyMemory(pFDCode, &FDCode, sizeof(FDCode));
				m_arrayFDCode.Add(pFDCode);
			}
		} while((int)nBytesRead);
		
		file.Close();
	} CATCH(CFileException, e)
	{
		#ifdef _DEBUG
		afxDump << "Fund File could not be opened " << e->m_cause << "\n";
		#endif
	}
	END_CATCH
	
	return TRUE;
}

void CAxisCodeApp::LoadSFCode()
{
	CString	path;
	int		codeN;
	CFile	file;
	struct  sfcode  SFCode;
	FCODE item;
	int ii = 0;

	path = m_RootPath + "\\tab\\" + SFCODE_FILE;
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return;
	
	// 데이터의 갯수...
	codeN = (int)file.GetLength() / sizeof(struct sfcode);
	
	//m_plistbox.AddString("전체");
	for (ii = 0; ii < codeN; ii++)
	{
		file.Read(&SFCode, sizeof(struct sfcode));
		item.code = CString(SFCode.codx, 8);
		item.name = CString(SFCode.hnam, 50);
		item.mchk = SFCode.mchk;
		item.gnam = CString(SFCode.gnam, 40);

		m_arraySFcode.Add(item);
	}
		
	file.Close();
}

BOOL CAxisCodeApp::loadPjCode()			//주식옵션.. 코드 종목...
{
	int		codeN;
	CFile		file;
	struct  pjcode  PJCode;
	PCODE		pcode;
	CString		path;
	
	path = m_RootPath + "\\tab\\" + PJCODE_FILE;
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	// 데이터의 갯수...
	codeN = (int)file.GetLength() / sizeof(struct pjcode);
	
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&PJCode, sizeof(struct pjcode));
		
		pcode.kind = GetString(PJCode.tjgb, 2);
		pcode.name = GetString(PJCode.snam, 20);
		pcode.code = GetString(PJCode.codx, PCodeLen);
		pcode.hnam = GetString(PJCode.hnam, sizeof(PJCode.hnam)); pcode.hnam.TrimRight();
		pcode.month = GetString(PJCode.mont, 4);
		pcode.price = GetString(PJCode.hsga, 8);
		pcode.gcod  = GetString(PJCode.gcod, 12);
		pcode.mchk = PJCode.mchk;
		
		if (PJCode.atmx == 1)
			pcode.atm = TRUE;
		else
			pcode.atm = FALSE;
		
		m_arrayPcode.Add(pcode);
	}
	
	file.Close();
	return TRUE;
}


BOOL CAxisCodeApp::loadJCode()			// 주식 현물 로드...
{		
	int	codeN;		
	CFile	file;
	struct	hjcode*  HJCode;
	struct	JCode*	jCode;

	m_arrayDlgJCode = new CPtrArray;
	CString path = m_RootPath + "\\tab\\" + HJCODE_FILE;
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	codeN = (int)file.GetLength() / sizeof(struct hjcode);

	for (int ii = 0; ii < codeN; ii++)
	{		
		HJCode = new struct hjcode;
		file.Read(HJCode, sizeof(struct hjcode));
		m_arrayHJCode.Add(HJCode);

//CJongmukDlg 사용
		jCode = new struct JCode;
		// 각 종목의 정보 표시...
		jCode->Name = CString(HJCode->hnam, HNameLen);
		jCode->Name.TrimRight();
		//jCode->Name = jCode->Name.Mid(1);	// 여기서 처리..
		jCode->fill = HJCode->jsiz;		//
		CopyMemory(jCode->stgb, HJCode->stgb, sizeof(HJCode->stgb));	//
		jCode->size = HJCode->size;
		jCode->ucdm = HJCode->ucdm;
		jCode->ucds = HJCode->ucds;
		jCode->jjug = HJCode->jjug;
		jCode->kpgb = HJCode->kpgb;
		jCode->ucmd = HJCode->ucmd;
		jCode->kosd = (char)HJCode->kosd;
		jCode->ssgb = HJCode->ssgb;
		jCode->itgb = HJCode->itgb;
		jCode->wsgb = HJCode->wsgb;
		jCode->Code = CString(HJCode->code, HCodeLen + 2);	//
		jCode->Code = jCode->Code.Mid(1);
		jCode->star = HJCode->star;
		jCode->unio = HJCode->unio;
		jCode->jsiz = HJCode->jsiz;
		jCode->pInf = NULL;

		m_arrayDlgJCode->Add(jCode);
/////////////////////////////////////////////
	 }

	file.Close();
	return TRUE;
}

bool CAxisCodeApp::findJCode(CString code, CString& name)
{
	hjcode*	jCode;
	for (int ii = 0; ii < m_arrayHJCode.GetSize(); ii++)
	{
		jCode = m_arrayHJCode.GetAt(ii);
		if (jCode->code == code)
		{
			name = CString(jCode->hnam, HNameLen);
			name.TrimRight();
			return true;
		}
	}
	return false;
}

BOOL CAxisCodeApp::loadELWCode()
{
	int	codeN;
	int	ii, jj, kk;
	CFile	file;
	struct  elwcode  ELWCode;
	struct _elwcode* savecode;
//	struct	JCode*	jCode;
	CString jName;
	CString path = m_RootPath + "\\tab\\" + ELWCODE_FILE;
	CString sMonth;
	CString tmpS;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	codeN = (int)file.GetLength() / sizeof(struct elwcode);

	struct	_sortStr *pMonthSortArr = NULL, *pBaseSortArr = NULL, *pPBSortArr = NULL;
	pMonthSortArr = new struct _sortStr[codeN];
	pBaseSortArr = new struct _sortStr[codeN];
	pPBSortArr = new struct _sortStr[codeN];
	
	bool	bFound = false;
	int nMonth = 0, nBase = 0, pbNum = 0;
	PBCODE pbcode;

	for (ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));
		// 코드저장
		m_arrayELWCode.Add(ELWCode);

//Dlg 사용///////////////////////////////////////////
		savecode = new struct _elwcode;
		jName = CString(ELWCode.codx, ELWCodeLen);
		savecode->code = jName.Mid(1);
		savecode->name = CString(ELWCode.hnam, ELWHNameLen);
		savecode->name.TrimRight();
		savecode->ktype = CString(ELWCode.krye, sizeof(ELWCode.krye));
		savecode->pcode = CString(ELWCode.bgce, sizeof(ELWCode.bgce));
		savecode->pname = CString(ELWCode.hbge, sizeof(ELWCode.hbge));
		savecode->mjmt  = CString(ELWCode.mjmt, sizeof(ELWCode.mjmt));
	
		for (jj = 0; jj < 5; jj ++)
		{
			savecode->bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12);
			savecode->bcode[jj].TrimRight();
		}
		
		// 발행기관
		bFound = false;
		for (kk = 0; kk < pbNum; kk++)
		{
			if (savecode->pcode.CompareNoCase(pPBSortArr[kk].code) == 0)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			pPBSortArr[pbNum].code = savecode->pcode;
			pPBSortArr[pbNum].name = savecode->pname;
			pbNum++;
		}

		// 기초자산
		for (jj = 0; jj < 5; jj ++)
		{
			CString bcode;
			bcode.Format("%s", savecode->bcode[jj]); bcode.TrimRight();
			if (bcode == "") break;
			bFound = false;
			for (kk = 0; kk < nBase; kk++)
			{
				if (bcode.CompareNoCase(pBaseSortArr[kk].code) == 0)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				if (findJCode(bcode, tmpS))
					pBaseSortArr[nBase].name = tmpS;
				else if (bcode.CompareNoCase("KOSPI200") == 0)
					continue;
					//pBaseSortArr[nBase].name = bcode;
				else
					continue;
				pBaseSortArr[nBase].code = bcode;
				nBase++;
			}
		}

		// 만기월
		bFound = false;
		sMonth.Format("%s.%s", savecode->mjmt.Left(4), savecode->mjmt.Mid(4, 2));
		for (jj = 0; jj < nMonth; jj++)
		{
			if (sMonth.CompareNoCase(pMonthSortArr[jj].code) ==0)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			pMonthSortArr[nMonth].code = sMonth;
			nMonth++;
		}

		// ELW코드저장
		m_arrayDlgELWCode.Add(savecode);
	}

	// combo에 만기년월 combo
//	m_comMonth.AddString("전체");
	qsort((void*)pMonthSortArr, nMonth, sizeof(struct _sortStr), _sortCode);
	for (ii = 0; ii < nMonth; ii++)
	{
		m_arrayElwMonth.Add(pMonthSortArr[ii].code);
//		m_comMonth.AddString(pMonthSortArr[ii].code);
	}

	// 기초자산(한글명순)
	BASECODE basecode;
	qsort((void*)pBaseSortArr, nBase, sizeof(struct _sortStr), _sortName);
	// KOSPI200 은 앞으로
	basecode.code = "KOSPI200";
	basecode.name = "KOSPI200";
	m_arrayBASECode.Add(basecode);
	for (ii = 0; ii < nBase; ii++)
	{
		basecode.code = pBaseSortArr[ii].code;
		basecode.name = pBaseSortArr[ii].name;
		m_arrayBASECode.Add(basecode);
	}

	// 발행기관(코드순)
	qsort((void*)pPBSortArr, pbNum, sizeof(struct _sortStr), _sortCode);
	for (ii = 0; ii < pbNum; ii++)
	{
		pbcode.code = pPBSortArr[ii].code;
		pbcode.name = pPBSortArr[ii].name;
		m_arrayPBCode.Add(pbcode);
	}

	delete[] pMonthSortArr;	pMonthSortArr = NULL;
	delete[] pBaseSortArr;	pBaseSortArr = NULL;
	delete[] pPBSortArr;	pPBSortArr = NULL;

	file.Close();
	return TRUE;
}

CString CAxisCodeApp::GetString(char *pChar, int nMinLen)
{
	return CString(pChar, min(nMinLen, (int)strlen(pChar)));
}


void CAxisCodeApp::LoadAllCode()
{
	/*
	loadJCode();
	loadELWCode();
	loadFCode();
	loadCOCode();
	loadOJCode();
	LoadUPCode();
	LoadUSCode();
	LoadSFCode();
	loadPjCode();
	LoadFDCode();
	*/
	LoadCJCode();
	LoadUBCode();
	LoadAGCode();

	LoadGlobal();
}

void CAxisCodeApp::LoadGlobal()
{
	Global.LoadBitmaps();
	Global.LoadIcons();

	Global.SetRGB(IDX_RGB_MASK, RGB(255,0,255));
	Global.SetRGB(IDX_RGB_BACKGROUND, RGB(255,255,255));

	Global.SetRGB(IDX_RGB_FONT, RGB(51,51,51));
	Global.SetRGB(IDX_RGB_FONT_CAPTION, RGB(0,0,0));
	Global.SetRGB(IDX_RGB_FONT_CAPTION_INACTIVE, RGB(178,178,178));
	Global.SetRGB(IDX_RGB_CONTROL_BORDER, RGB(204,204,204));

	Global.SetFont(IDX_FONT_SMALL, _T("Arial"), 8);
	Global.SetFont(IDX_FONT_MEDIUM, _T("Arial"), 9);
	Global.SetFont(IDX_FONT_LARGE, _T("Arial"), 11, TRUE);
}

bool CAxisCodeApp::IsCodeType(CString code, int type)
{
	bool	rValue = false; 

	switch (type)
	{
	case 1: // Sector 지수
		{
			CString str;
			hjcode*	jcode;
			for (int ii = 0 ; ii < m_arrayHJCode.GetSize() ; ii++)
			{
				jcode = m_arrayHJCode.GetAt(ii);

				str = CString(jcode->code, 12);
				str = str.Mid(1);
				str.TrimLeft(); 

				if (str.Compare(code) == 0)
				{
					if (*(unsigned short*)jcode->stgb != 0)
						rValue = true;

					break;
				}
			}
		}
		break;
	case 2: // ETF
		{
			CString str;
			hjcode*	jcode;
			for (int ii = 0 ; ii < m_arrayHJCode.GetSize() ; ii++)
			{
				jcode = m_arrayHJCode.GetAt(ii);

				str = CString(jcode->code, 12);
				str = str.Mid(1);
				str.TrimLeft(); 

				if (str.Compare(code) == 0)
				{
					if (jcode->ssgb == jmETF)
						rValue = true;

					break;
				}
			}
		}
		break;
	case 3: // 해외ETF
		{
			CString str;
			hjcode*	jcode;
			for (int ii = 0 ; ii < m_arrayHJCode.GetSize() ; ii++)
			{
				jcode = m_arrayHJCode.GetAt(ii);

				str = CString(jcode->code, 12);
				str = str.Mid(1);
				str.TrimLeft(); 

				if (str.Compare(code) == 0)
				{
					if (jcode->ssgb == jmFETF)
						rValue = true;

					break;
				}
			}
		}
		break;
	case 4: // 해외원주
		{
			CString str;
			hjcode*	jcode;
			for (int ii = 0 ; ii < m_arrayHJCode.GetSize() ; ii++)
			{
				jcode = m_arrayHJCode.GetAt(ii);

				str = CString(jcode->code, 12);
				str = str.Mid(1);
				str.TrimLeft(); 

				if (str.Compare(code) == 0)
				{
					if (jcode->ssgb == jmFREG)
						rValue = true;

					break;
				}
			}
		}
		break;
	}

	return rValue;
}

BOOL CAxisCodeApp::FindName(int kind, CString name, char* code)
{
	if (m_RootPath.IsEmpty() || name.IsEmpty())
		return FALSE;
	
	CString str;
	CJcode *jcode;
	code[0] = 0x00;

	BOOL	rValue = FALSE;
	name.MakeUpper();
	name.TrimRight();
	name.TrimLeft();
	switch(kind)
	{
		case 130:  //종목명이 넘어와서 비교한다.
			{
				for (int ii = 0; ii < m_arrayCJCode.GetSize(); ii++)
				{
					jcode = m_arrayCJCode.GetAt(ii);
					str = CString(jcode->name, 50);
					str.TrimLeft(); 
				
					if (str.Compare(name) == 0)
					{
						str = CString(jcode->code, 12);
						str.TrimLeft(); str.TrimRight();
						strcpy(code, str);  //종목코드 반환
						return true;
					}
				}	
			}
			break;
		case 131:  //종목코드가 넘어와서 비교한다
			{
				for (int ii = 0; ii < m_arrayCJCode.GetSize(); ii++)
				{
					jcode = m_arrayCJCode.GetAt(ii);
					str = CString(jcode->code, 12);
					str.TrimLeft(); 
					str.TrimRight();
					if (str.Compare(name) == 0)
					{
						str = CString(jcode->name, 50);
						str.TrimLeft(); str.TrimRight();
						strcpy(code, str);  //종목명 반환
						return true;
					}
				}	
				
				if(name.GetLength() > 0)
				{
					strcpy(code, name);
					return true;
				}
			}
			break;
		case 132: //회사코드가 오면 회사명을 반환한다
			{
				AGCODE* pcode = NULL; 
				for (int ii = 0; ii < m_arrayAGCode.GetSize(); ii++)
				{
					pcode = m_arrayAGCode.GetAt(ii);
					str = CString(pcode->code, 6);
					str.TrimLeft(); 
					str.TrimRight();
					if (str.Compare(name) == 0)
					{
						str = CString(pcode->name, 50);
						str.TrimLeft(); str.TrimRight();
						strcpy(code, str);  //종목명 반환
						return true;
					}
				}	
				
				if(name.GetLength() > 0)
				{
					strcpy(code, name);
					return true;
				}
			}
		case 133: //회사명오면 회사코드을 반환한다
			{
				AGCODE* pcode = NULL; 
				for (int ii = 0; ii < m_arrayAGCode.GetSize(); ii++)
				{
					pcode = m_arrayAGCode.GetAt(ii);
					str = CString(pcode->name, 50);
					str.TrimLeft(); 
					str.TrimRight();
					if (str.Compare(name) == 0)
					{
						str = CString(pcode->code, 6);
						str.TrimLeft(); str.TrimRight();
						strcpy(code, str);  //종목명 반환
						return true;
					}
				}	
				
				if(name.GetLength() > 0)
				{
					strcpy(code, name);
					return true;
				}

			}
			break;
	}
	
	return rValue;
}

BOOL CAxisCodeApp::FindCode(int kind, CString code, char* name)
{
	if (m_RootPath.IsEmpty())
		return FALSE;
		
	CJcode *jcode;
	name[0] = 0x00;

	BOOL	rValue = FALSE;
	code.MakeUpper();
	switch(kind)
	{
	case whichTYPE:
		{
			
			if (m_arrayCJCode.GetSize() <= 0)
				return FALSE;

			CString str;
			for (int ii = 0 ; ii < m_arrayCJCode.GetSize(); ii++)
			{
				jcode = m_arrayCJCode.GetAt(ii);

				str = CString(jcode->code, 12);
				str.TrimLeft();

				if (str.Compare(code) == 0)
				{
					str = CString(jcode->name, 100);
					str.TrimLeft(); str.TrimRight();
					strcpy(name, str);
					return TRUE;
				}	
			}
		}
		break;
	case codeNAME:
		{
			CString str;
			for (int ii = 0; ii < m_arrayCJCode.GetSize(); ii++)
			{
				jcode = m_arrayCJCode.GetAt(ii);
				str = CString(jcode->code, 12);
				str.TrimLeft(); 
				//ZeroMemory(name, 25);
				
				if (str.Compare(code) == 0)
				{
					
					str = CString(jcode->name, 100);
					str.TrimLeft(); str.TrimRight();
					strcpy(name, str);
					return true;
				}
			}	
		}
	case futureNAME:
		{		
		}	
		break;
	case optionNAME:
		{
		}
		break;
	case commdtNAME:
		{		
		}
		break;
	}
	
	return rValue;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisCodeApp object

CAxisCodeApp theApp;





