#include "pch.h"
#include "CSRWSR.h"

#define L_MAPN 8
#define DF_LIMIT 5
#define DF_GLOVAL_CS 

extern CRITICAL_SECTION g_CriticalSection;

cs_RW::cs_RW()
{
   
}


cs_RW::~cs_RW()
{
   // EnterCriticalSection(&g_CriticalSection);
}

BOOL cs_RW::isExist(CString sPath)
{
	CFileFind cfind;
    return  cfind.FindFile(sPath);
}

//파일을 수정한 날짜
BOOL cs_RW::GetFile_MOD_Date(CString filePath, CString& sDate, DOUBLE date)
{
	BOOL bResult = FALSE;
    CFileStatus fileStatus;

    if (CFile::GetStatus(filePath, fileStatus))
    {
        bResult = TRUE;
        CTime modificationTime = fileStatus.m_mtime;
        sDate.Format("%4d%02d%02%02%02%02", modificationTime.GetYear(), modificationTime.GetMonth(),
            modificationTime.GetDay(), modificationTime.GetHour(),
            modificationTime.GetMinute(), modificationTime.GetSecond());

        //int           가능 범위 2^32 = 4,294,967,296
        //DOUBLE  가능 범위 2^64
        date = atof(sDate);
        return bResult;
    }
	return bResult;
}

//파일  생성 날짜
BOOL cs_RW::GetFile_MAKE_Date(CString filePath, CString& sDate, DOUBLE date)
{
    BOOL bResult = FALSE;
    CFileStatus fileStatus;
    CString strDate;
    if (CFile::GetStatus(filePath, fileStatus))
    {
        bResult = TRUE;
        CTime modificationTime = fileStatus.m_ctime;
        strDate.Format("%4d%02d%02d%02d%02d%02d", modificationTime.GetYear(), modificationTime.GetMonth(),
            modificationTime.GetDay(), modificationTime.GetHour(),
            modificationTime.GetMinute(), modificationTime.GetSecond());

        //int           가능 범위 2^32 = 4,294,967,296
        //DOUBLE  가능 범위 2^64
        date = atof(sDate);
        sDate = strDate;
        return bResult;
    }
    return bResult;
}


//파일 만들기
BOOL cs_RW::MakeFile_MB(CString strFileName, CString strPath, BOOL bShowFolder)
{
//    WritePrivateProfileString
    CString spath, strFilePath;
    if (strPath.IsEmpty())
    {
        char chfile[500]{};
        GetModuleFileName(nullptr, chfile, 260);
        strPath.Format("%s", chfile);
        strPath.Trim();
        int ifind = strPath.ReverseFind('\\');
        strPath = strPath.Left(ifind);
     }
    strFilePath.Format("%s\\%s", strPath, strFileName);
    
    BOOL bSuccess = WritePrivateProfileString(_T("MAKEFILE"), _T("file"), strFilePath + "한글", strFilePath);
    
    if (bShowFolder)
    {
        HINSTANCE result = ShellExecute(NULL, "explore", strPath, NULL, NULL, SW_SHOWNORMAL);
        if ((int)result <= 32) {
            // ShellExecute 함수가 실패한 경우
            AfxMessageBox("탐색기를 열 수 없습니다.", MB_OK | MB_ICONERROR);
            return 1;
        }
    }

    return bSuccess;
}

//멀티바이트 파일 읽기 
BOOL cs_RW::ReadFile_MB(CString strFile,  CString strSection, CString& strResult)
{
    if (!isExist(strFile))
        return FALSE;

    DWORD rc;
    char* ptr{}, wb[1024]{};
    rc = GetPrivateProfileSection(strSection, wb, sizeof(wb), strFile);

    if (!rc)
    {
        strResult.Empty();
        return FALSE;
    }

    strResult.Format("%s", wb);
    strResult.Trim();

    CString string;
    int off{};
    ptr = wb;
    for (; rc > 0; ptr++, rc--)
    {
        string = ptr;
        ptr += string.GetLength();
        rc -= string.GetLength();
    }

    return TRUE;
}

//유니코드 파일 읽기 
BOOL cs_RW::ReadFile_Uni(CString strFile, CString strSection, CStringW& wstrResult, CString& strResult)
{
    if (!isExist(strFile))
        return FALSE;

    WCHAR* ptr{}, wchfile[1024]{};
    CStringW strw(strFile);
    LPCWSTR wstrFile = strw;

    CStringW strsectionw(strSection);
    LPCWSTR wstrsection = strsectionw;

    DWORD rc;
    rc = GetPrivateProfileSectionW(strsectionw, wchfile,  sizeof(wchfile), wstrFile);

    if (!rc)
    {
        wstrResult.Empty();
        strResult.Empty();
        return FALSE;
    }

    CStringW string;
    ptr = wchfile;
	for (; rc > 0; ptr++, rc--)
	{
		string = ptr;
		ptr += string.GetLength();
		rc -= string.GetLength();
	}

    CStringW wstrbuff(wchfile);
    wstrResult = wstrbuff;
    wstrResult.Trim();

    strResult = CW2A(wstrResult);
    strResult.Trim();
    return TRUE;
}

//먼가 불완전하다
BOOL cs_RW::IsUniCodeFile(char* filePath)
{
    // 파일을 바이너리 모드로 열기
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    // 파일의 처음 몇 바이트 읽기
    const int bufferSize = 1024;
    char buffer[bufferSize];
    file.read(buffer, bufferSize);

    // 읽은 데이터가 유니코드인지 확인
    bool isUnicode = false;
    for (int i = 0; i < bufferSize; ++i) {
        if (buffer[i] == '\0') {
            isUnicode = true;
            break;
        }
    }

    // 결과 출력
    if (isUnicode) {
        std::cout << "UTF-16 (유니코드) 파일입니다." << std::endl;
    }
    else {
        std::cout << "멀티바이트 (ANSI) 파일입니다." << std::endl;
    }

    // 파일 닫기
    file.close();
    return FALSE;
}

DWORD cs_RW::convert_ansi_to_unicode(CStringW& swUnicode, const char* ansi, const int isize)
{
    if (1)
    {
        CStringW str1;
        str1 = CA2W(ansi, CP_ACP);
        swUnicode = str1;
    }
    else
    {
        DWORD dError = 0;
        wstring wstr;
        do
        {
            if ((nullptr == ansi) || (0 == isize))
            {
                dError = ERROR_INVALID_PARAMETER;
                break;
            }

            wstr.clear();

            //메모리 확보
            int ilen = MultiByteToWideChar(CP_ACP, 0, ansi, static_cast<int>(isize), nullptr, 0);
            if (ilen == 0)
            {
                dError = GetLastError();
                break;
            }

            wstr.resize(ilen);
            //변환
            if (0 == MultiByteToWideChar(CP_ACP, 0, ansi, static_cast<int>(isize), const_cast<wchar_t*>(wstr.c_str()), static_cast<int>(wstr.size())))
            {
                dError = GetLastError();
                break;
            }
            else
            {
                swUnicode = wstr.c_str();
            }
        } while (false);

        return dError;
    }
    
}

DWORD cs_RW::convert_unicode_to_ansi(CString& sAnsi, const wchar_t* wchr, const int isize)
{
    /*
    #define CP_ACP                    0           // default to ANSI code page
    #define CP_OEMCP                  1           // default to OEM  code page
    #define CP_MACCP                  2           // default to MAC  code page
    #define CP_THREAD_ACP             3           // current thread's ANSI code page
    #define CP_SYMBOL                 42          // SYMBOL translations
    #define CP_UTF7                   65000       // UTF-7 translation
    #define CP_UTF8                   65001       // UTF-8 translation
    */
    if (1)
    {
        USES_CONVERSION;
        LPCSTR stemp = W2A(wchr);
      //  CString  stemp1 = CW2A(wchr, CP_ACP);  //위 아래 결과가 같다. 컴파일러의 비트가 영향을 미칠까?
        sAnsi = sAnsi;
        return TRUE;
    }
    else
    {
        DWORD dError = 0;

        CString sResult;
        do
        {
            if ((nullptr == wchr) || (0 == isize))
            {
                dError = ERROR_INVALID_PARAMETER;
                break;
            }

            sResult.Empty();
            int ilen = WideCharToMultiByte(CP_ACP, 0, wchr, static_cast<int>(isize), nullptr, 0, nullptr, nullptr);
            if (ilen == 0)
            {
                dError = GetLastError();
                break;
            }
            sAnsi.Empty();
            if (0 == WideCharToMultiByte(CP_ACP, 0, wchr,
                static_cast<int>(isize),
                const_cast<char*>(sAnsi.GetBuffer(0)),
                static_cast<int>(isize), nullptr, nullptr))
            {
                dError = GetLastError();
                break;
            }
        } while (false);

        return dError;

    }
}

DWORD cs_RW::convert_unicode_to_utf8(CString& strUTF8, const wchar_t* wchr, const int isize)
{
    DWORD dError = 0;
    std::string swUTF8;
    CString sResult;
    do
    {
        if ((nullptr == wchr) || (0 == isize))
        {
            dError = ERROR_INVALID_PARAMETER;
            break;
        }
        swUTF8.clear();

        int ilen = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wchr, static_cast<int>(isize), nullptr, 0, nullptr, nullptr);
        if (ilen == 0)
        {
            dError = GetLastError();
            break;
        }
             
        swUTF8.resize(ilen);
        if (0 == WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, 
            wchr,
            static_cast<int>(isize),
            const_cast<char*>(swUTF8.c_str()),
            static_cast<int>(swUTF8.size()),
            nullptr, nullptr))
        {
            dError = GetLastError();
            break;
        }
        else
        {
            strUTF8 = swUTF8.c_str();
        }


    } while (false);

    return dError;
}


