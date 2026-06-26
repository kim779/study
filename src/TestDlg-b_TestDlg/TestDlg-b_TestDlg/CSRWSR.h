#pragma once
//cs_RW


class cs_RW
{
public:
	cs_RW();
	virtual ~cs_RW();

public:

	CString m_sFile;

//파일이 존재하는지
	BOOL isExist(CString sPath);

//파일을 상태를 검사한다
	//파일 수정 날짜
	BOOL GetFile_MOD_Date(CString filePath, CString& sDate, DOUBLE date = 0);

//파일  생성 날짜
	BOOL GetFile_MAKE_Date(CString filePath, CString& sDate, DOUBLE date = 0);

//파일 만들기
	BOOL MakeFile_MB(CString strFileName, CString strPath = "", BOOL bShowFolder = FALSE);
	BOOL MakeFile_Uni(WCHAR strFileName, WCHAR* strPath = nullptr);

//멀티바이트 파일 읽기 
	BOOL ReadFile_MB(CString strFile, CString strSection, CString& strResult);

//유니코드 파일 읽기 
	BOOL ReadFile_Uni(CString strFile, CString strSection, CStringW& wstrResult, CString& strResult);

//uni코드 형태인지 알아내기 <-- 아직 이상함
	BOOL IsUniCodeFile(char* filePath);  

//ansi - ASCII  1바이트 (EUC-KR, CP949)
//UTF-8(Unicode Transformation Format - 8-bit)
//UTF-8은 유니코드를 표현하는데 사용되는 가변 길이 문자 인코딩입니다. UTF-8은 1바이트부터 4바이트
//convert ansi string to unicode string
	DWORD convert_ansi_to_unicode(CStringW& swUnicode, const char* ansi, const int isize);

//convert unicode to ansi string
	DWORD convert_unicode_to_ansi(CString& sAnsi, const wchar_t* wchr, const int isize);

//convert ansi unicode to utf-8 string
	DWORD convert_unicode_to_utf8(CString& strUTF8, const wchar_t* wchr, const int isize);
	
//convert unicode to utf8 string


//convert utf8 to unicode string


	//날짜 비교
	template<typename t1, typename t2>
	BOOL IsDateBigAandB(t1 atime, t2 btime)
	{
		return atime > btime;
	}

	template<int t1, typename t2>
	BOOL IsDateBigAandB(int atime, t2 btime)
	{
		int ival = btime;
		return atime > btime;
	}



};


