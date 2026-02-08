#include "stdafx.h"
#include "libctr.h"
#include "CX_FDS.h"
#include "../../h/axisfire.h"
#include "../../h/ledger.h"

#define	homeCC			0x0a

#define DF_LOG

void WriteLog( LPCSTR home,LPCSTR log, ... )
{
	CString slog;
	slog.Format("[FDS][%s]", log);
	OutputDebugString(slog);
	return;
#if 1
	FILE *fp = fopen( (CString)home + "\\exe\\axis.log", "a+");
	if (!fp)
		return;

	const	CTime time = CTime::GetCurrentTime();
	fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] ").GetString());
	
	va_list argptr;
	va_start(argptr, log);
	vfprintf(fp, log, argptr);
	va_end(argptr);
	fprintf(fp, "\n");
	
	fclose(fp);
#endif
}

CString Get_DN_Value(CString sPath)
{
	if (1)
	{
		CString slog;
		int readL = 0;
		char readB[1024];
		CString sfile;
		sfile.Format("%s\\%s\\AXISENC.ini", sPath, "tab");
		readL = GetPrivateProfileString("ADDRESS", "main", "", readB, sizeof(readB), sfile);

		slog.Format("[FDS] Get_DN_Value  readB=[%s]  readL=[%d]", readB, readL);
		OutputDebugString(slog);

		if (readL <= 0)
			return "";

		CString sWnd;
		sWnd.Format("%s", readB);
		int ddata = atoi(sWnd);
		HWND hWnd = (HWND)ddata;

		CWnd* m_pMainFrame{};
		m_pMainFrame = CWnd::FromHandle(hWnd);
		sWnd = CString((LPCTSTR)m_pMainFrame->SendMessage(WM_USER, MAKEWPARAM(0x16, 0), 0));
		sWnd = "|" + sWnd;

		slog.Format("[FDS] 메인으로부터 [%s] ]", sWnd);
		WriteLog(sPath, slog);

		return sWnd;
	}
	else
	{
		CString slog{};
		int readL = 0;
		char readB[1024];
		CString sfile;
		sfile.Format("%s\\%s\\AXISENC.INI", sPath, "tab");
		readL = GetPrivateProfileString("FDS", "DN", "", readB, sizeof(readB), sfile);

		slog.Format("[FDS] Get_DN_Value [%d]", readL);
		OutputDebugString(slog);

		if (readL <= 0)
		{
			int readL = 0;
			char readB[1024];
			CString sfile;
			sfile.Format("%s\\%s\\AXISENC.ini", sPath, "tab");
			readL = GetPrivateProfileString("ADDRESS", "main", "", readB, sizeof(readB), sfile);

			slog.Format("[FDS] Get_DN_Value  readB=[%s]  readL=[%d]", readB, readL);
			OutputDebugString(slog);

			if (readL <= 0)
				return "|";

			CString sWnd;
			sWnd.Format("%s", readB);
			int ddata = atoi(sWnd);
			HWND hWnd = (HWND)ddata;

			CWnd* m_pMainFrame{};
			m_pMainFrame = CWnd::FromHandle(hWnd);
			sWnd = CString((LPCTSTR)m_pMainFrame->SendMessage(WM_USER, MAKEWPARAM(0x16, 0), 0));
			sWnd = "|" + sWnd;

			OutputDebugString("[FDS] 메인으로부터 DN = " + sWnd);

			return sWnd;
		}
		else
		{
			CString sVal;
			sVal.Format("|%s", readB);

			OutputDebugString("[FDS] 파일로 부터 DN = " + sVal);

			return sVal;
		}
	}
	
	return _T("");
}

__declspec(dllexport) int APIENTRY axFDSValue(char* trN, char* srcB, int srcL, char* desB)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sPath;
	CString strFilePath;
	CFile	file;
	CString sBuf;
	TCHAR	chFileName[512]{};

	GetModuleFileName(NULL, chFileName, MAX_PATH);

	strFilePath.Format(_T("%s"), chFileName);

	strFilePath = strFilePath.Left(strFilePath.ReverseFind('\\'));
	strFilePath.Replace("\\exe", "");

	sPath.Format("%s\\tab\\FDSSERVICE.DAT", strFilePath);
	if (file.Open(sPath, CFile::modeRead | CFile::shareDenyNone))
	{
		const	long	len = (long)file.GetLength();
		std::unique_ptr<char[]> buf = std::make_unique<char[]>(len + 1);
		file.Read(buf.get(), len);
		sBuf = CString(buf.get(), len);
		sBuf.MakeUpper();
		buf.reset();
		file.Close();
	}

	CString strTRN = trN;
	strTRN.MakeUpper();
	WriteLog(strFilePath, strTRN + "  FDS START");
	if (sBuf.Find(strTRN, 0) < 0)
	{
		if (srcL < sizeof(_ledgerH))
		{
			CString slog;
			slog.Format("[FDS] sBuf=[%d] strTRN=[%s] srcL=[%d] ", sBuf.GetLength(), strTRN, srcL);
			WriteLog(strFilePath,"FDS SIZE ERROR\n");
			return 0;
		}

		const	struct	_ledgerH *ledger = (struct _ledgerH *)srcB;

		strTRN = CString(ledger->svcd, sizeof(ledger->svcd));
		strTRN.MakeUpper();
		if (sBuf.Find(strTRN, 0) < 0)
		{
			WriteLog(strFilePath,"FDS NOT SERVICE\n");
			return 0;
		}
	}

	//testcide trN 이 FDSSERVICE.DAT 의 FDS setcion에 있는지 확인
	bool bfind{};
	CString line;
	bool inFDSSection = false;
	CStdioFile stdfile(sPath, CFile::modeRead);
	while (stdfile.ReadString(line))
	{
		line.Trim(); // 양쪽 공백 제거

		// 섹션 시작점 찾기
		if (line == _T("[FDS]"))
		{
			inFDSSection = true;
			continue;
		}

		if (line.Left(1) == "[")
			inFDSSection = false;

		if (inFDSSection && line.Compare(strTRN) == 0)
		{
			bfind = true;
			break;
		}
	}
	stdfile.Close();
	//////////////////////////////////////////////////

	CString sfile;
	sfile.Format("%s\\tab\\%s", strFilePath, "axisENC.ini");

	char buff[256]{};
	GetPrivateProfileString("FDS", "version", "0", buff, sizeof(buff) - 1, sfile);

	CString version;
	version.Format("%s", buff);
	version.TrimRight();

	if (version == "0")
	{
		sPath.Format("%s\\dev\\KTBSOL.dll", strFilePath);

		HMODULE	hInfoDll = LoadLibrary(sPath);
		if (hInfoDll == nullptr)
		{
			WriteLog(strFilePath, "FDS KTBSOL DLL ERROR\n");
			return 0;
		}

		int	nRet = 0;
		std::unique_ptr<char[]> pValue = nullptr;
		typedef int (typeInitialize_DLL_Api)();
		typedef int (typeGetScanResultApiLen)();
		typedef int (typeGetScanResultApi)(char* pszVal);

		typeInitialize_DLL_Api* Initialize_DLL_Api = (typeInitialize_DLL_Api*)GetProcAddress(hInfoDll, _T("Initialize_DLL_Api"));
		if (Initialize_DLL_Api)
			Initialize_DLL_Api();

		typeGetScanResultApiLen* GetScanResultApiLen = (typeGetScanResultApiLen*)GetProcAddress(hInfoDll, _T("GetScanResultApiLen"));
		if (GetScanResultApiLen)
		{
			nRet = GetScanResultApiLen();
			pValue = std::make_unique<char[]>(nRet);

			FillMemory(desB, nRet, ' ');
			desB[nRet] = 0x00;
		}

		typeGetScanResultApi* GetScanResultApi = (typeGetScanResultApi*)GetProcAddress(hInfoDll, _T("GetScanResultApi"));
		if (GetScanResultApi && pValue)
		{
			nRet = GetScanResultApi(pValue.get());
			CopyMemory(desB, pValue.get(), nRet);
		}
		if (pValue)
			pValue.reset();

#ifdef DF_LOG
		CString stmp, sval;
		sval.Format("%s", desB);
		stmp.Format("FDS  vserion1 len = [%d]  TR=[%s] val=[%s]", nRet, trN, sval.Left(150));
		WriteLog(strFilePath, stmp);
#endif


		FreeLibrary(hInfoDll);
		WriteLog(strFilePath, "FDS END");

		return nRet;
	}
	else
	{
		int	nRet = 0;
		sPath.Format("%s\\dev\\I3GManager.dll", strFilePath);

		HMODULE	hInfoDll = LoadLibrary(sPath);
		if (hInfoDll == nullptr)
		{
			WriteLog(strFilePath, "FDS I3GManager DLL load ERROR\n");
			return 0;
		}

		int nThreadTimeOut = 20;	// thread timeout
		char szWData[2048] = { 0, };
		typedef bool (*NPGetHtsWData_Func) (__in int nThreadTimeOut, __out char* szWData);
		// 데이터 수집
		if (hInfoDll) 
		{
			FARPROC pF = GetProcAddress(hInfoDll, "NPGetHtsWData");
			if (pF)
			{
				NPGetHtsWData_Func npFunc = (NPGetHtsWData_Func)pF;
				bool bResult = npFunc(nThreadTimeOut, szWData);
				if (bResult) 
				{
					CString sval;
					sval.Format("%s", szWData);
					sval.TrimRight();

					//tsetcode
					if (bfind)
					{
						sval += Get_DN_Value(strFilePath);
						memset(szWData, ' ', 2048);
						memcpy(szWData, (LPCTSTR)sval, sval.GetLength());
					}

					nRet = sval.GetLength();

	#ifdef DF_LOG
					CString stmp;
					stmp.Format("FDS  version2 bfind = [%d]  len = [%d] TR=[%s]", bfind, nRet, trN);
					WriteLog(strFilePath, stmp);
	#endif
					if (1)
					{
						int ilen{};
						if (nRet < 1024)
						{
							ilen = 1024;
							stmp.Format("FDS  version2 nRet[%d] is lower than 1024", nRet);
							WriteLog(strFilePath, stmp);
						}
						else
							ilen = nRet;

						FillMemory(desB, ilen, ' ');
						desB[ilen] = 0x00;
						CopyMemory(desB, szWData, nRet);
						nRet = ilen;
					}
					else
					{
						FillMemory(desB, nRet, ' ');
						desB[nRet] = 0x00;
						CopyMemory(desB, szWData, nRet);
					}
				

					if (hInfoDll)
					{
						FreeLibrary(hInfoDll);
						hInfoDll = NULL;
					}
					return nRet;
				}
				else
				{
					WriteLog(strFilePath, "FDS I3GManager DLL NPGetHtsWData ERROR\n");
					return 0;
				}
			}
		}
	}
}
