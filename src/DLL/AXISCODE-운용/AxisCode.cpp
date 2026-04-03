// AxisCode.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "axisCode.h"
#include "user.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMPILE_MULTIMON_STUBS
#include "multimon.h"

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


/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisCodeApp object

CAxisCodeApp theApp;

int CAxisCodeApp::GetVirtualScreenX()
{
	const int virScrX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	return virScrX;
}

CString GetString(char* pChar, int nMinLen)
{
	return CString(pChar, min(nMinLen, (int)strlen(pChar)));
}

BOOL CAxisCodeApp::LoadFCode()
{
	if ( _m_arrayFcode.GetCount() > 0)
		return TRUE;
	if (_m_arrayOcode.GetCount() > 0)
		return TRUE;
	if (_m_arrayPcode.GetCount() > 0)
		return TRUE;

	int	codeN{};
	CFile	file;
	struct  fjcode  FJCode {};
	FCODE fcode{};
	CString path;
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\fjcode.dat";   //선물스프레드

	if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		return FALSE;
	}
	// 데이터의 갯수...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct fjcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&FJCode, sizeof(struct fjcode));

		fcode.code = CString(FJCode.cod2, FCodeLen);
		fcode.name = CString(FJCode.hnam, FNameLen);
		fcode.mchk = FJCode.mchk;

		_m_arrayFcode.Add(fcode);
	}
	file.Close();



	int	 len{};
	struct  ojcodh  OJCodh {};

	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\opcode2.dat";  //옵션코드

	if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		return FALSE;
	}
	len = file.Read(&OJCodh, sizeof(struct ojcodh));

	codeN = gsl::narrow_cast<int>((file.GetLength() - len) / sizeof(struct ojcode));
	for (int ii = 0; ii < codeN; ii++)
	{
		struct  ojcode OJCode;
		file.Read(&OJCode, sizeof(struct ojcode));

		_m_arrayOcode.Add(OJCode);
	}
	file.Close();



	struct  pjcode  PJCode {};
	PCODE		pcode{};
	CMapStringToPtr pcodemap;
	//CQArray	<CString, CString>	qKindArr;

	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\pjcode.dat";  //주식옵션

	if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		//MessageBox("파일[pjcode.dat]이 존재하지 않습니다.");
		return FALSE;
	}
	// 데이터의 갯수...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct pjcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&PJCode, sizeof(struct pjcode));

		pcode.kind = GetString(PJCode.tjgb, 2);
		pcode.name = GetString(PJCode.snam, 20);
		pcode.code = GetString(PJCode.codx, PCodeLen);
		pcode.hnam = GetString(PJCode.hnam, sizeof(PJCode.hnam)); pcode.hnam.TrimRight();
		pcode.month = GetString(PJCode.mont, 4);
		pcode.price = GetString(PJCode.hsga, 8);
		pcode.gcod = GetString(PJCode.gcod, 12);
		pcode.mchk = PJCode.mchk;

		if (PJCode.atmx == 1)
			pcode.atm = TRUE;
		else
			pcode.atm = FALSE;

		pcodemap.SetAt(pcode.kind, (void*)_m_arrayPcode.Add(pcode));
	}

	POSITION pos = pcodemap.GetStartPosition();
	CString sKind;
	int value{};

	//if (m_plistbox.m_hWnd && IsWindow(m_plistbox.m_hWnd))
	//{
	//	m_plistbox.ResetContent();
	//	while (pos)
	//	{
	//		pcodemap.GetNextAssoc(pos, sKind, (void*&)value);
	//		qKindArr.Add(sKind);
	//	}

	//	qKindArr.QuickSort();

	//	for (int ii = 0; ii < qKindArr.GetSize(); ii++)
	//	{
	//		sKind = qKindArr.GetAt(ii);
	//		if (pcodemap.Lookup(sKind, (void*&)value))
	//		{
	//			CString str = m_arrayPcode.GetAt(value).name;
	//			m_plistbox.SetItemData(m_plistbox.AddString(m_arrayPcode.GetAt(value).name), value);
	//		}
	//	}
	//}

	file.Close();

}

BOOL CAxisCodeApp::LoadCode()
{
	if (/*_mapCODE.size() > 0 ||*/ _mapCODEx.size() > 0)
		return TRUE;

	CFile	file;
	CString	string = AxStd::FORMAT("%s/%s/%s", m_root, TABDIR, HJCODEx);
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return FALSE;

		const int size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return FALSE;
		}

		const int count = len / sizeof(struct hjcodex);
		struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			_mapCODEx.emplace(std::move(sCode), hjc);
			if (hjc->ssgb == jmELW)
				continue;

			if (hjc->ssgb == jmETN)	//2014.10.28 KSJ ETN 추가
			{
				_vETNx.emplace_back(hjc);
				continue;
			}
			_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
			_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
		}

	}
	//else
	//{

	//	if (string.Format("%s/%s/%s", m_root, TABDIR, HJCODE);
	//		!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	//		return FALSE;

	//	const int size = gsl::narrow_cast<int>(file.GetLength());
	//	m_hcB = std::make_unique<char[]>(size + 1);
	//	const int len = file.Read(m_hcB.get(), size);
	//	file.Close();

	//	if (size > len)
	//	{
	//		m_hcB.reset();
	//		return FALSE;
	//	}

	//	const int count = len / sizeof(struct hjcode);
	//	struct	hjcode* hjc = (struct hjcode*)m_hcB.get();

	//	for (int ii = 0; ii < count; ii++, hjc++)
	//	{
	//		CString sCode = CString(hjc->code, HCodeLen).Mid(1);
	//		_mapCODE.emplace(std::move(sCode), hjc);
	//		if (hjc->ssgb == jmELW)
	//			continue;

	//		if (hjc->ssgb == jmETN)	//2014.10.28 KSJ ETN 추가
	//		{
	//			_vETN.emplace_back(hjc);
	//			continue;
	//		}
	//		_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1), 0)));
	//		_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1)), std::move(sCode));
	//	}
	//}
	return TRUE;
}

BOOL CAxisCodeApp::LoadETFCode()
{
	CString ETFile;
	CString data, line;
		
	ETFile.Format("%s/%s/efcode.dat", m_root, TABDIR);
	CStdioFile file(ETFile, CFile::modeRead);

	while(file.ReadString(line))
	{				
		switch (const char gubn = line.GetAt(0); gubn)
		{	
		case '1':
			{
				const _efopitem *efopitem = (struct _efopitem*)line.GetBuffer();
				_vETFitem.emplace_back(std::move(std::tuple(efopitem->type[0],
								CString(efopitem->opcd, sizeof(efopitem->opcd)).Trim(),
								CString(efopitem->hnam, sizeof(efopitem->hnam)).Trim())));			
			}
			break;
		case '2':
			{
				const _efoptema *efoptema = (struct _efoptema*)line.GetBuffer();
				_vETFtema.emplace_back(std::move(std::tuple(efoptema->type[0],
									efoptema->tmcd[0],
									CString(efoptema->tmnm, sizeof(efoptema->tmnm)).Trim())));						
			}
			break;
		case '3':
			{
				const _efopfore *efopfore = (struct _efopfore*)line.GetBuffer();
				_vETFfore.emplace_back(std::move(std::tuple(efopfore->type[0], 
									efopfore->frcd[0], 
									CString(efopfore->frnm, sizeof(efopfore->frnm)).Trim())));
			}
			break;
		case '4':
			{
			
				const _efopcode *efopcode = (struct _efopcode*)line.GetBuffer();
				_vETFcode.emplace_back(std::move(std::tuple(efopcode->type[0], 
									CString(efopcode->code, sizeof(efopcode->code)).Trim(), 
									CString(efopcode->hnam, sizeof(efopcode->hnam)).Trim(),
									CString(efopcode->opcd, sizeof(efopcode->opcd)).Trim(), 
									efopcode->etfm[0], 
									efopcode->etfl[0])));
			}
			break;
		default:
			break;
		}
	}

	file.Close();
	return TRUE;
}

BOOL CAxisCodeApp::loadCJCode()
{
	CString path, cjBuffer;
	struct cjcode* code{};
	
	path.Format("%s/%s/cjcode.dat", m_root, TABDIR);
	CFile file;
	
	if (file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		const int len = gsl::narrow_cast<int>(file.GetLength());
		const int readN = file.Read(cjBuffer.GetBuffer(len + 1), len);
		cjBuffer.ReleaseBuffer();
		file.Close();

		if (len > readN)
			return FALSE;

		const int codeN = len / sizeof(struct cjcode);
		code = (struct cjcode*)cjBuffer.GetBuffer();

		for (int ii = 0; ii < codeN; ii++, code++)
		{
			_vBond.emplace_back(std::make_tuple(
				CString(code->code, 12).Trim(),
				CString(code->hnam, 40).Trim(),
				code->cgub,
				code->ngub
			));
			//_mapACODE.emplace(CString(code->code, 12).Trim(), std::make_pair(CString(code->hnam, 40).Trim(), 0));
		}
		return TRUE;
	}
	return FALSE;
}